#include "dr-web-test/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QStringList arguments, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Интерполятор");
    if (arguments.size() == 2) {
        sh_p_interp_widget = std::shared_ptr<InterpolationWidget>(new InterpolationWidget(arguments.back()));
        if (sh_p_interp_widget->isFailed())
            m_failed = true;
    } else {
        sh_p_interp_widget = std::shared_ptr<InterpolationWidget>(new InterpolationWidget);
    }
    ui->widgetLayout->addWidget(sh_p_interp_widget.get());

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isFailed()
{
    return m_failed;
}



