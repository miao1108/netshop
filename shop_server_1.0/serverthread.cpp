#include "serverthread.h"

Serverthread::Serverthread(int sockDesc,QObject *parent) : QThread(parent)
{
  m_sockDesc=sockDesc;
}

void Serverthread::run()
{
    m_socket = new MYsocket(m_sockDesc);

    if (!m_socket->setSocketDescriptor(m_sockDesc)) {
        return ;
    }
    connect(m_socket, &MYsocket::disconnected, this, &Serverthread::disconnectToHost);
    this->exec();
}


void Serverthread::disconnectToHost()
{
    m_socket->disconnectFromHost();
    this->quit();
}
