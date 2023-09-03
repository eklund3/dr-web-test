#ifndef INTERPOLATIONWIDGET_H
#define INTERPOLATIONWIDGET_H

#include <QWidget>

#include <QTableWidgetItem>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QFuture>
#include <mutex>

#include "datastructs.h"

enum methods {
    linear = 0,
    lagrange = 1,
    cubic = 2
};

namespace Ui {
class InterpolationWidget;
}

class InterpolationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InterpolationWidget(QString file_name = QString(), QWidget *parent = nullptr);
    ~InterpolationWidget();

    bool isFailed();

signals:
    void update_graph();

    void change_controls_state(bool state);

    void input_await(int row, bool state);

private slots:
    void on_table_cellChanged(int row, int column);

    void on_methodBox_currentIndexChanged(int index);

    void on_update_graph();

    void on_controls_state_change(bool state);

    void on_input_await(int row_awaiting, bool state);

private:
    // парсит csv файл и заполняет points
    // возвращает false если не удалось, true иначе
    bool read_csv(QString file_name, Points& points);

    //в отдельном потоке отрисовывает график
    void draw_graph_exec();

    bool redraw_table();

    bool change_coord(float new_value, int row,
                      bool is_x, bool *was_permutation);

    bool insert_new_point_after(int at, Point p);

    bool remove_point(int at);

    bool points_contains(float x, int* count = nullptr);

    Points m_points;
    std::mutex data_mutex;
    std::condition_variable await_draw_cv;
    QFutureWatcher<void> m_future_watcher;
    std::atomic_bool ready2draw = false;
    std::atomic_bool toExit = false;

    bool m_failed = false;
    bool m_await_input = false;

    int m_last_added_row = -1;
    bool m_isEditing = false;

    bool eventFilter(QObject* target, QEvent* event);
private:
    Ui::InterpolationWidget *ui;
};

#endif // INTERPOLATIONWIDGET_H
