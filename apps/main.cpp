#include "dr-web-test/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(a.arguments());
    if (w.isFailed())
        return 0;
    w.show();
    return a.exec();
}
