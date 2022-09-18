#ifndef MD5_H
#define MD5_H
#include<RSA.h>
#include<QByteArray>
#include <QCryptographicHash>
#include<QString>
#include<QDebug>
QByteArray sign_create(QByteArray data ,char * n,char * d);//数字签名生成
bool sign_judge(QByteArray data,QByteArray sign,char * n,int e);//数字签名的校验
#endif // MD5_H

