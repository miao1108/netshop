#include"MD5.h"

QByteArray  sign_create(QByteArray data ,char * n,char * d)//数字签名生成与加密,并且拼接到正文后面
{
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    QString strMD5 = hash.toHex();
    //qDebug()<<strMD5;

    char *ch = strMD5.toLatin1().data();
    //qDebug()<<ch;
    char *md5_encrypt=decrypt(ch,n,d);
    QByteArray res;
    res.append(md5_encrypt);
    //data->append(md5_encrypt);
    return res;
}
bool sign_judge(QByteArray data,QByteArray sign,char * n,int e)//数字签名的校验
{
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    QString strMD5 = hash.toHex();

    qDebug()<<strMD5;
    char *ch;
    ch = sign.data();
    char *md5_decrypt=encrypt(ch,n,e);
   // qDebug()<<md5_decrypt;
    QString strMD5_1=QString(md5_decrypt);
    //qDebug()<<strMD5_1;
    if(strMD5==strMD5_1)
        return true;
    else return false;
}
