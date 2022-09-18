#ifndef KERBEROS_H
#define KERBEROS_H
#include<QString>
#include<QTcpServer>

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

struct Authenticator{
    int IDc;
    QString TS;
};

struct CtoTGS{
    int IDv;
    char* m_ticket_tgs;
    QByteArray authenticator;
};

struct TGStoC{
    QByteArray Kcv;
    int IDv;
    QString TS4;
    char* m_ticket_v;
};
struct VtoC{
   QString TS5;
};
struct Pubdata{
    QString TS1;
    QString TS5;
    int IDc;
    int IDv;
    QByteArray Key_c_tgs;
    QByteArray Key_c_v;
    char *mykey_d;
    char *mykey_n;
    QTcpSocket *socket;

    QString uname;
    QString upho;
};
#endif // KERBEROS_H

