#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //TCP连接
    TCPServer=new MYtcpserver;
    TCPServer->listen(QHostAddress::AnyIPv4, 6667);
}

MainWindow::~MainWindow()
{
    delete ui;
}
