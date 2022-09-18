#ifndef NETWORK_H
#define NETWORK_H
#include"MD5.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QtNetwork/QHostInfo>
//#pragma pack(push)
//#pragma pack(1)//单字节对齐(Qt似乎是默认4字节对齐)
struct Message
{
    unsigned int message_length;//报文长度
    char message_type;//报文类型
    char field_type;//字段类型
    unsigned char des_ip[4]={0,0,0,0};//目的IP
    unsigned char des_src[4]={0,0,0,0};//源IP
    unsigned int sign_length;//数字签名长度
    QByteArray data;//数据内容
    QByteArray sign;//数字签名

};

struct Mes
{
    unsigned int message_length;//报文长度
    char message_type;//报文类型
    char field_type;//字段类型
    unsigned char des_ip[4]={0,0,0,0};//目的IP
    unsigned char des_src[4]={0,0,0,0};//源IP
    unsigned int sign_length;//数字签名长度
    Mes(Message a)
    {
        message_length=a.message_length;
        message_type=a.message_type;
        field_type=a.field_type;
        for(int i=0;i<4;i++)
        {
            des_ip[i]=a.des_ip[i];
            des_src[i]=a.des_src[i];
            sign_length=a.sign_length;
        }
    }
};



//#pragma pack(pop)
struct mes_len
{
    unsigned int message_length;
};
#endif // NETWORK_H

QString getMyIpString();
bool package_message(char message_type,char field_type,QByteArray  messge,QTcpSocket *socket,char * n, int e,char *sign_n=nullptr,char *sign_d=nullptr);
int send_message(QTcpSocket *socket,Message message);
bool package_message_noencrypt(char message_type,char field_type,QByteArray  messge,QTcpSocket *socket);
Message receive_message(QTcpSocket *socket,char * n, char * d,char * sign_n=nullptr, int sign_e=0);
Message receive_message_nodecrypt(QTcpSocket *socket);
