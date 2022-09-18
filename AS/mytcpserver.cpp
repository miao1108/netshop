#include "mytcpserver.h"
#include<QDebug>
MYtcpserver::MYtcpserver(QObject *parent) : QTcpServer(parent)
{

}
void MYtcpserver::incomingConnection(qintptr socketDescriptor)
{
    m_socketList.append(socketDescriptor);

    Serverthread *thread = new Serverthread(socketDescriptor);
    qDebug()<<socketDescriptor<<endl;
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
