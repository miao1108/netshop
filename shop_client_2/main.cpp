#include "mainwindow.h"
#include <QApplication>
#include "gmp.h"
#include "DES.h"
#include <QDebug>
#include <iostream>
using namespace std;



int main(int argc, char* argv[])
{
    //QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    //qputenv("QT_SCALE_FACTOR", "0.8");
    //qreal  cx = GetSystemMetrics(SM_CXSCREEN);
    //qreal scale = cx / 960;				// 960 = 1920 / 2
    //qputenv("QT_SCALE_FACTOR", QString::number(scale).toLatin1());

//    qputenv("QT_SCALE_FACTOR","1.2");
//        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//        QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    //这个是Windows平台用来获取屏幕宽度的代码，
    //因为在qApplication实例初始化之前，QGuiApplication::screens();无法使用。



    return a.exec();
}
