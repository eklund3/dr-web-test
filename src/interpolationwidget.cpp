#include "dr-web-test/interpolationwidget.h"
#include "ui_interpolationwidget.h"

#include "dr-web-test/linearinterp.h"
#include "dr-web-test/lagrangepoly.h"
#include "dr-web-test/cubicspline.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPainter>

#define XCOL    0
#define YCOL    1
#define PREC    3          //максимальное кол-во значащих цифр после запятой
#define EPS     1.0/pow(10.0, PREC+1)

#define IS_EQUAL(x1,x2) abs(x1 - x2) < EPS

#define TO_PREC(val) std::round(pow(10.0, PREC) * val) / pow(10.0, PREC)

InterpolationWidget::InterpolationWidget(QString file_name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InterpolationWidget)
{
    ui->setupUi(this);

    if (!file_name.isEmpty()) {
        if (file_name.contains(".csv")) {
            if (read_csv(file_name, m_points)) {
                ui->status->setText("Данные успешно загружены!");
            } else {
                m_failed = true;
            }
        } else {
            m_failed = true;
        }
        if (m_failed) {
            QMessageBox errorMsg;
            errorMsg.setText("Не удаётся открыть файл "+ file_name +"!");
            errorMsg.setWindowTitle("Ошибка");
            errorMsg.setIcon(QMessageBox::Critical);
            errorMsg.exec();
            return;
        }
    } else {
        ui->status->setText("Работа в режиме ручного заполнения");
    }

    ui->customPlot->addGraph(); //Для исходных точек
    ui->customPlot->addGraph(); //Для интерполированных
    ui->customPlot->graph(0)->setPen(QPen(Qt::black, 6));
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::LineStyle::lsNone);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDot));
    ui->customPlot->setInteraction(QCP::iRangeDrag);
    ui->customPlot->setInteraction(QCP::iRangeZoom);

    ui->table->installEventFilter(this);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->methodBox->addItem("Линейный", 0);
    ui->methodBox->addItem("Лагранж", 1);
    ui->methodBox->addItem("Кубик", 2);

    QObject::connect(this, &InterpolationWidget::update_graph,
                     this, &InterpolationWidget::on_update_graph);
    QObject::connect(this, &InterpolationWidget::change_controls_state,
                     this, &InterpolationWidget::on_controls_state_change);


    QFuture future = QtConcurrent::run(this, &InterpolationWidget::draw_graph_exec);
    m_future_watcher.setFuture(future);
    while (!ready2draw);

    if (m_points.empty())
        m_points = {Point(0.0,0.0)};
    else {
        redraw_table();
        await_draw_cv.notify_all();
    }
}

InterpolationWidget::~InterpolationWidget()
{
    std::unique_lock lk(data_mutex);
    toExit = true;
    lk.unlock();
    await_draw_cv.notify_all();
    m_future_watcher.waitForFinished();
    delete ui;
}

bool InterpolationWidget::isFailed()
{
    return m_failed;
}

void InterpolationWidget::on_table_cellChanged(int row, int column)
{
    QString sNew_value = ui->table->item(row, column)->text();
    sNew_value.replace(',','.');
    bool ok = false;
    float new_value = sNew_value.toFloat(&ok);
    bool is_x = column == XCOL ? true : false;
    if (!ok) {
        ui->status->setText("Не удается установить значение \"" + sNew_value + "\". Проверьте корректность ввода!");
        ui->table->blockSignals(true);
        ui->table->item(row, column)->setText(QString::number(
                                                  is_x ? m_points[row].x : m_points[row].y,
                                                  10, PREC));
        ui->table->blockSignals(false);
        m_isEditing = false;
        return;
    }
    ui->table->blockSignals(true);
    ui->table->item(row, column)->setText(QString::number(new_value, 10, PREC));
    ui->table->blockSignals(false);
    bool need2redraw = false;
    if (m_last_added_row != -1 && row != m_last_added_row) {
        remove_point(m_last_added_row);
        need2redraw = true;
        if (m_last_added_row < row)
            row--;
    }
    bool was_permutation;
    if (change_coord(new_value, row, is_x, &was_permutation)) {
        if (was_permutation || need2redraw)
            redraw_table();
        m_last_added_row = -1;
        await_draw_cv.notify_all();
    }
}

void InterpolationWidget::on_methodBox_currentIndexChanged(int index)
{
    QString method = ui->methodBox->itemText(index);
    await_draw_cv.notify_all();
}

void InterpolationWidget::on_update_graph()
{
    ui->customPlot->rescaleAxes();
    ui->customPlot->xAxis->scaleRange(1.1);
    ui->customPlot->yAxis->scaleRange(1.1);
    ui->customPlot->replot();
}

void InterpolationWidget::on_controls_state_change(bool state)
{
    ui->table->setEnabled(state);
    ui->methodBox->setEnabled(state);
}

bool InterpolationWidget::read_csv(QString file_name, Points &points)
{
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    } else {
        QTextStream in(&file);
        bool ok;
        QString header = in.readLine();
        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields.size() != 2) {
                return false;
            }
            float x = fields.front().toFloat(&ok);
            if (!ok) {
                return false;
            }
            float y = fields.back().toFloat(&ok);
            if (!ok) {
                return false;
            }
            m_points.push_back(Point(TO_PREC(x), TO_PREC(y)));
        }
    }
    file.close();
    std::sort(m_points.begin(), m_points.end(), [](const Point& a, const Point& b) {return a.x < b.x;});
    auto last = std::unique(m_points.begin(), m_points.end(), [](const Point& a, const Point& b) {return abs(a.x - b.x) < EPS;});
    m_points.erase(last, m_points.end());
    return true;
}

void InterpolationWidget::draw_graph_exec()
{
    while (true) {
        std::unique_lock lk(data_mutex);
        ready2draw = true;
        await_draw_cv.wait(lk);
        if (toExit) {
            break;
        }
        if (m_points.empty())
            continue;
        ready2draw = false;
        change_controls_state(false);
        ui->status->setText("Рисую график... Пожалуйста, подождите...");
        size_t points_count = m_points.size()*4;
        float step = (m_points.back().x - m_points.front().x) / static_cast<float>(points_count);

        //отрисовываем узловые точки
        QVector<double> x_init(m_points.size(), 0), y_init(m_points.size(),0);
        for (size_t i = 0; i < m_points.size(); i++) {
            x_init[i] = m_points[i].x;
            y_init[i] = m_points[i].y;
        }
        ui->customPlot->graph(0)->setData(x_init, y_init);
        if (m_points.size() < 4) {
            ui->status->setText("Недостаточно данных для интерполяции.");
            ui->customPlot->graph(1)->data()->clear();
            emit update_graph();
            continue;
        }

        //отрисовываем интерполированные данные
        QVector<double> x_interp(points_count-1, 0.0), y_interp(points_count-1,0.0);
        LinearInterp linear;
        LagrangePoly lagrange;
        CubicSpline spline;
        IInterpolator* interpolator;
        switch ( ui->methodBox->currentData().toInt() )
        {
            case methods::linear:
            {
                interpolator = &linear;
            }
            break;

            case methods::lagrange:
            {
                interpolator = &lagrange;
            }
            break;

            case methods::cubic:
            {
                spline.setData(m_points);
                interpolator = &spline;
            }
            break;

            default:
            {
                interpolator = &linear;
            }
                break;
        }

        float x = m_points.front().x;
        int i = 0;
        float end = m_points.back().x - step;
        while (x < end && !(IS_EQUAL(x, end))) {
            x += step;
            x_interp[i] = x;
            y_interp[i] = interpolator->at(x, m_points);
            i++;
        }
        x_interp.append(x_init);
        y_interp.append(y_init);
        ui->customPlot->graph(1)->setData(x_interp, y_interp);
        ui->status->setText("Готово!");
        emit update_graph();
        change_controls_state(true);
    }
}

bool InterpolationWidget::redraw_table()
{
    std::scoped_lock lk(data_mutex);
    ui->table->blockSignals(true);
    ui->table->setRowCount(m_points.size());
    for (int i = 0; i < m_points.size(); i++) {
        QTableWidgetItem *pCell = ui->table->item(i, XCOL);
        Point& p = m_points[i];
        if (!pCell) {
            pCell = new QTableWidgetItem;
            ui->table->setItem(i, XCOL, pCell);
        }
        pCell->setText(QString::number(p.x, 10, PREC));

        pCell = ui->table->item(i, YCOL);
        if (!pCell) {
            pCell = new QTableWidgetItem;
            ui->table->setItem(i, YCOL, pCell);
        }
        pCell->setText(QString::number(p.y, 10, PREC));
    }
    ui->table->blockSignals(false);
    return true;
}

bool InterpolationWidget::change_coord(float new_value, int row, bool is_x, bool *was_permutation)
{
    float x_val = is_x ? new_value : m_points[row].x;
    float& val = is_x ?  m_points[row].x : m_points[row].y;
    if (abs(new_value - val) < EPS)
        return false;

    bool need2sort = false;
    *was_permutation = false;

    if (row != 0 && row != m_points.size()) {
        if (x_val <= m_points[row-1].x || x_val >= m_points[row+1].x) {
            need2sort = true;
        }
    } else if (row == 0) {
        if (x_val >= m_points[row+1].x) {
            need2sort = true;
        }
    } else if (row == m_points.size()) {
        if (x_val <= m_points[row-1].x) {
            need2sort = true;
        }
    }

    val = new_value;
    if (need2sort) {
        std::sort(m_points.begin(), m_points.end(), [](const Point& a, const Point& b) {return a.x < b.x;});
        auto last = std::unique(m_points.begin(), m_points.end(), [](const Point& a, const Point& b) {return abs(a.x - b.x) < EPS;});
        m_points.erase(last, m_points.end());
        *was_permutation = true;
    }
    return true;
}

bool InterpolationWidget::insert_new_point_after(int at, Point p)
{
    std::scoped_lock lk(data_mutex);
    m_points.insert(m_points.begin() + at + 1, p);
    return true;
}

bool InterpolationWidget::remove_point(int at)
{
    std::scoped_lock lk(data_mutex);
    if (at < 0 || at >= m_points.size() ||
            m_points.size() <= 1)
        return false;
    m_points.erase(m_points.begin() + at);
    return true;
}

bool InterpolationWidget::points_contains(float x, int *count)
{
    std::scoped_lock lk(data_mutex);
    int counter = 0;
    for (Point& p : m_points) {
        if (IS_EQUAL(p.x, x))
            counter++;
    }
    if (counter) {
        if (count != nullptr)
            *count = counter;
        return true;
    } else
        return false;
}

bool InterpolationWidget::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui->table) {
        if (event->type() == QEvent::Type::KeyPress) {
            QKeyEvent* ev = static_cast<QKeyEvent*>(event);
            if (ev->isAutoRepeat())
                return false;
            if (ev->key() == Qt::Key::Key_Insert) {
                if (ui->table->isPersistentEditorOpen(ui->table->currentItem()))   //ещё редактируем
                        return false;
                if (m_last_added_row == -1) {
                    int row = ui->table->currentRow();
                    bool contain_zero = points_contains(0.0);
                    insert_new_point_after(row,  Point(0.0,0.0));
                    redraw_table();
                    ui->table->selectRow(row+1);
                    if (contain_zero)
                        m_last_added_row = row+1;
                    else {
                        std::sort(m_points.begin(), m_points.end(),
                                  [](const Point& a, const Point& b) {return a.x < b.x;});
                        auto last = std::unique(m_points.begin(), m_points.end(),
                                                [](const Point& a, const Point& b) {return abs(a.x - b.x) < EPS;});
                        m_points.erase(last, m_points.end());
                        await_draw_cv.notify_all();
                    }
                } else {
                    QMessageBox warningMsg;
                    warningMsg.setText("Координаты последней добавленной точки в строке " + QString::number(m_last_added_row+1) + " ещё не заполнены! Прежде чем добавлять новую точку, заполните предыдущюю точку!");
                    warningMsg.setWindowTitle("Предупреждение");
                    warningMsg.setIcon(QMessageBox::Warning);
                    warningMsg.exec();
                }

            } else if (ev->key() == Qt::Key::Key_Delete) {
                int row = ui->table->currentRow();
                if (m_last_added_row != -1) {
                    if (row != m_last_added_row) {
                        remove_point(m_last_added_row);
                        if (m_last_added_row < row)
                            row--;
                    }
                    m_last_added_row = -1;
                }
                remove_point(row);
                redraw_table();
                await_draw_cv.notify_all();
            }
            return false;
        }
    }
    return false;
}

