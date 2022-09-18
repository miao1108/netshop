#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTcpServer>
#include<QThread>
#include<QMutex>

QMutex mutexx;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*连接数据库
    QProcess process;
    process.start("C:/Program Files/MySQL/MySQL Server 5.7/bin/mysqld.exe");
    db= QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("netshop");       //这里输入你的数据库名
    db.setUserName("root");
    db.setPassword("dm089084...");   //这里输入你的密码
    if (!db.open()) {
    QMessageBox::critical(0, QObject::tr("无法打开数据库"),"无法创建数据库连接！ ", QMessageBox::Cancel);
    qDebug()<<"不能连接"<<"connect to mysql error"<<db.lastError().text();
    }
    qDebug()<<"连接成功"<<"connect to mysql OK";*/

    //TCP连接
    /*TCPServer=new QTcpServer;
    TCPServer->listen(QHostAddress::Any, 6666);
    TCPServer->waitForNewConnection(-1);
    socket=TCPServer->nextPendingConnection();


    */
    TCPServer=new MYtcpserver;
    TCPServer->listen(QHostAddress::AnyIPv4, 6666);
    /*TCPServer=new QTcpServer(this);
    TCPServer->listen(QHostAddress::Any, 6666);
    connect(TCPServer,&QTcpServer::newConnection,this,[=](){
                QTcpSocket * socket=TCPServer->nextPendingConnection();
                int locate=socketlist.size();
                socketlist.push_back(socket);
                QObject::connect(socket, &QTcpSocket::readyRead, this, [=](){
                    socket_Read_Data(socket);
                });
                /*
                QThread  *sub = new QThread ;
                MYthread *work= new MYthread(socket);
                work->moveToThread(sub);	// 移动到子线程中工作
                QObject::connect(socket, SIGNAL(&QTcpSocket::readyRead), work, SLOT(socket_Read_Data));
                sub->start();
            });*/


}

MainWindow::~MainWindow()
{

    delete ui;
}

