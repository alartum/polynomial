#include "mainwindow.h"
#include "polynomial.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
//    Polynomial P;
//    P.append(1);
//    P.append(1);
//    Polynomial Q;
//    Q.append(2);
//    Q.append(1);

//    qDebug() << (P == Q);

//    return 0;
}
