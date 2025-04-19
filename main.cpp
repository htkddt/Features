#include "mainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    /* Start app */
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
