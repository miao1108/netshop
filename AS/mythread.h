#ifndef MYTHREAD_H
#define MYTHREAD_H

#include<QTcpServer>
#include<QTcpSocket>
#include <QObject>
#include<QDataStream>
#include<QBuffer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include<QTime>
class MYthread : public QObject
{
    Q_OBJECT
public:
    explicit MYthread(QTcpSocket * thissocket,QObject *parent = nullptr);


protected:
    //void run()override;

signals:

public slots:
   //void dowork();
   void socket_Read_Data();
};

#endif // MYTHREAD_H
