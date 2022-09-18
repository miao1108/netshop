#include "mainwindow.h"
#include "ui_mainwindow.h"

QMutex mutexx;
QMutex mutexx_t;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TCPServer=new MYtcpserver;
    TCPServer->listen(QHostAddress::AnyIPv4, 6668);
    qDebug()<<"start to listen"<<endl;

}

MainWindow::~MainWindow()
{
    delete ui;
}


