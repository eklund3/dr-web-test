#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "interpolationwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QStringList arguments,
               QWidget *parent = nullptr);
    ~MainWindow();

    bool isFailed();

private:
    bool m_failed = false;

    std::shared_ptr<InterpolationWidget> sh_p_interp_widget;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
