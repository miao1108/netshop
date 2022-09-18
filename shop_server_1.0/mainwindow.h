#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"mytcpserver.h"
#include <QMainWindow>
#include"gmp.h"
#include<QDebug>
#include<iostream>
#include<QTcpServer>
#include<QBuffer>
#include<mysocket.h>
#include<QMutex>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MYtcpserver *TCPServer;



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
