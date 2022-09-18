#include "mysocket.h"
#include<QThread>
#include<QMutex>
extern QMutex mutexx;
MYsocket::MYsocket(int sockDesc,QObject *parent) : QTcpSocket(parent)
{
    m_sockDesc=sockDesc;
    //QString connectionName = QString::number(*static_cast<int*>(QThread::currentThreadId()));
       {
           mutexx.lock();
           db = QSqlDatabase::addDatabase("QMYSQL", QString("%1").arg(sockDesc));
           db.setHostName("localhost");
           db.setDatabaseName("netshop");       //这里输入你的数据库名
           db.setUserName("root");
           db.setPassword("dm089084...");   //这里输入你的密码
           if (!db.open())
           {
               qDebug() << "thread 1 open db failed!";
           }
           mutexx.unlock();
           qDebug() << "thread 1: " << QString("%1").arg(sockDesc);
       }
       //QSqlDatabase::removeDatabase(QString("%1").arg(sockDesc));
    connect(this, SIGNAL(readyRead()), this, SLOT(socket_Read_Data()));
}
void MYsocket::socket_Read_Data()
{
    QByteArray m_buffer;
    Message recv_mes;
    QByteArray buffer = this->readAll();
    m_buffer.append(buffer);
    int total_length = m_buffer.size();
    while(total_length)
    {
        QDataStream recv_data(m_buffer);
        recv_data.setVersion(QDataStream::Qt_5_12);
        int m_len=sizeof(mes_len);
        if(total_length<m_len)
            break;//包长不够
        //mes_len *msg1=(mes_len*)m_buffer.left(m_len).data();
        unsigned int he_len;
        recv_data>>he_len;
        //qDebug()<<m_buffer.left(m_len).length();
        int head_len=m_len+he_len+4;
        if(total_length<head_len)
            break;

        QByteArray t1;
        recv_data>>t1;
        Mes *header=(Mes *)t1.data();
        //Mes *header=(Mes *)m_buffer.mid(m_len,head_len).data();

        int all_len=head_len+header->message_length+header->sign_length+4;

        if(total_length<all_len)
            break;
        QByteArray t2;
        recv_data>>t2;
        //QByteArray t2=m_buffer.mid(head_len,all_len);
        Message res;
        res.message_length=header->message_length;
        res.message_type=header->message_type;
        res.field_type=header->field_type;
        for(int i=0;i<4;i++)
        {
            res.des_ip[i]=header->des_ip[i];
            res.des_src[i]=header->des_src[i];
            res.sign_length=header->sign_length;
        }

        res.data=t2.left(header->message_length);
        res.sign=t2.right(header->sign_length);
        recv_mes=res;

        buffer = m_buffer.right(total_length -all_len);
        total_length = buffer.size();
        m_buffer = buffer;
    }
    AS_handel(recv_mes);

}


void MYsocket::AS_handel(Message recv_mes){
    int e=65537;
    //****解析报文中的数据
    QBuffer recmesbuf(&recv_mes.data);
    recmesbuf.open(QIODevice::ReadOnly);
    QDataStream recmein(&recmesbuf);
    int ctoas_idc,ctoas_idtgs;
    QString ctoas_ts1;
    recmein>>ctoas_idc>>ctoas_idtgs>>ctoas_ts1;

    //****ticket
    Ticket tastoc;
    tastoc.ID=ctoas_idtgs;
    tastoc.IDc=ctoas_idc;
    tastoc.Key_cn=Get_K();//之后要改
    QTime ts2time=QTime::currentTime();
    tastoc.TS=ts2time.toString("h:m:s.z");
    tastoc.lifetime=24;//oneday
    //****ticket change to qbytearr
    QByteArray tdata;
    QBuffer tbuffer(&tdata);
    tbuffer.open(QIODevice::WriteOnly);
    QDataStream tout(&tbuffer);
    tout<<tastoc.Key_cn<<tastoc.IDc<<tastoc.ID<<tastoc.TS<<tastoc.lifetime;
    tbuffer.close();
    tdata=tdata.toHex();
    tdata.insert(0,QByteArray("1"));
    //****one encryp
    char* one_encrypt=encrypt(tdata.data(),TGSkey_n,e);

    /*
    //****two encrypt
    QByteArray twodata(one_encrypt);
    twodata=twodata.toHex();
    twodata.insert(0,QByteArray("1"));
    char* two_encrypt=encrypt(twodata.left(10).data(),n,e);

    //****one ticket dencrypt
    char * result=decrypt(two_encrypt,n,d);
    QByteArray qresult(result);
    qresult.remove(0,1);
    qresult=QByteArray::fromHex(qresult);
    char *miao=qresult.data();

    //****two ticket dencrypt
    char * tworesult=decrypt(qresult.data(),n,d);
    QByteArray twoqresult(tworesult);
    twoqresult.remove(0,1);
    twoqresult=QByteArray::fromHex(twoqresult);

    QBuffer buf(&twoqresult);
    buf.open(QIODevice::ReadOnly);
    QDataStream in(&buf);
    Ticket tastocnew;
    in>>tastocnew.Key_cn>>tastocnew.IDc>>tastocnew.ID>>tastocnew.TS>>tastocnew.lifetime;*/

    //*****astoc
    AStoC astoc;
    astoc.IDtgs=ctoas_idtgs;
    astoc.TS2=ts2time.toString("h:m:s.z");
    astoc.lifetime=24;//oneday
    //astoc.m_ticket_tgs=encrypt(tdata.data(),n,e);
    astoc.Key_c_tgs=tastoc.Key_cn;

    //****astoc change qbytearr
    QByteArray asdata;
    QBuffer buffer(&asdata);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out<<astoc.Key_c_tgs<<astoc.IDtgs<<astoc.TS2<<astoc.lifetime;
    buffer.close();

    //****查询c的公钥
    char* Ckey_n;
    QSqlQuery query(db);
    QString txt=QString("select Upubkey from user where Uid=%1").arg(ctoas_idc);
    bool sucess=query.exec(txt);
    if(query.first()){
        QString ckey_nstr=query.value(0).toString();
        std::string str=ckey_nstr.toStdString();
        Ckey_n=new char[strlen(str.c_str())+1];
        strcpy(Ckey_n,str.c_str());
    }


    //****astoc 加密
    asdata=asdata.toHex();
    asdata.insert(0,QByteArray("1"));
    char* asencrypt=encrypt(asdata.data(),Ckey_n,e);

    //****astoc and ticket send
    QByteArray as_ticket;
    QBuffer allbuffer(&as_ticket);
    allbuffer.open(QIODevice::WriteOnly);
    QDataStream allout(&allbuffer);
    allout<<asencrypt<<one_encrypt;
    allbuffer.close();
    package_message_noencrypt(4,0,as_ticket,this);

    int adb=123;
}
