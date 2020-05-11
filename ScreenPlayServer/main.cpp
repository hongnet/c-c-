#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    std::async(std::launch::async,[&w](){
//        w.Bind("127.0.0.1");
//    });


    return a.exec();
}
