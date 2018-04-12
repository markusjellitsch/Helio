#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();


   // w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowCancelButtonHint);
    //w.showFullScreen();

    return a.exec();
}
