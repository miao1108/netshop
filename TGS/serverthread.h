#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
#include "mysocket.h"
#include <QThread>

class Serverthread : public QThread
{
    Q_OBJECT
public:
    explicit Serverthread(int sockDesc,QObject *parent = nullptr);

private:
    MYsocket *m_socket;
    int m_sockDesc;
    void run() override;

signals:
    void dataReady(const QString &ip, const QByteArray &data);

private slots:
    void disconnectToHost();

};

#endif // SERVERTHREAD_H
