#include "mainwindow.h"

#include <QApplication>
#include"gmp.h"
//#include"MD5.h"
#include<QDebug>
#include<iostream>
#include<QTcpServer>
//#include"network.h"
#include<QBuffer>
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
