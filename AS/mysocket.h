#ifndef MYSOCKET_H
#define MYSOCKET_H
#include<QTime>
#include <QTcpSocket>
#include"network.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include<QBuffer>
#include"DES.h"
//extern QSqlDatabase db;

static char TGSkey_n[]="c432a812fda6c2b150efa577acf70721de1799acf9e789d054ceca82cc3af6f211ab3cf458b4016c32fc286c6d27d05baa5eccef0c594c2cb3d6eeca57cdc901911f4a990d062e2206a323bd0a3650607a9093cec0cc6eeb47b21316d643708f89e76c4a739f027ac6f93565dd68ffe4a5480f79b9163935238d6b05113003";
static char TGSkey_d[]="26401e067dcf4a3bd9a4af24d2e2e250170590df69655bb7f320d1d8884aa2a4750f6675d8e95731aa0b01328a616dbcc14d7434d2ba822060fb71589f9fae1fc1079cbe9d0a0aafcc378cd9edd29dbb151c3df3326207f77b153dfb7cbc8dba7e7f805c3cbe8d2e97b6770b8e3c2f718b1d53ec0f4348e197a5f8f431ef19";

class MYsocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MYsocket(int sockDesc,QObject *parent = nullptr);
    void AS_handel(Message recv_mes);

signals:


public slots:
   void socket_Read_Data();

private:
    int m_sockDesc;
    QSqlDatabase db;
};
struct CtoAS{
    int IDc;
    int IDtgs;
    QString TS1;
};
struct Ticket {
QByteArray Key_cn;//c与tgs的des密钥
int IDc;
int ID;
QString TS;
int lifetime;//hour
};

struct AStoC{
QByteArray Key_c_tgs;//c与tgs的des密钥
int IDtgs;
QString TS2;
int lifetime;
//char* m_ticket_tgs;//放着加密的ticket
};
#endif // MYSOCKET_H
