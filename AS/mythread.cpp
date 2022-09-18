#include "mythread.h"

extern QSqlDatabase db;

MYthread::MYthread(QTcpSocket * thissocket,QObject *parent) : QObject(parent)
{

}

