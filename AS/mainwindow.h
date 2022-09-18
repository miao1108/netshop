#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"mytcpserver.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include<QTcpServer>
#include<QTcpSocket>
#include <QProcess>
#include <QMessageBox>
//extern QSqlDatabase db;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MYtcpserver *TCPServer;

    ~MainWindow();


private:

    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
