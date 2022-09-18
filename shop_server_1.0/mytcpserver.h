#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include"serverthread.h"
#include <QTcpServer>

class MYtcpserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit MYtcpserver(QObject *parent = Q_NULLPTR);


private:
    QList<int> m_socketList;
    void incomingConnection(qintptr socketDescriptor) override;



};

#endif // MYTCPSERVER_H
