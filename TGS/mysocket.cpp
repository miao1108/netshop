#include "mysocket.h"

MYsocket::MYsocket(int sockDesc,QObject *parent) : QTcpSocket(parent)
{
    m_sockDesc=sockDesc;
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
    TGS_handel(recv_mes);

}


void MYsocket::TGS_handel(Message recv_mes){\
    int e=65537;
    //****解析报文中的数据
    CtoTGS ctotgs;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>ctotgs.IDv>>ctotgs.m_ticket_tgs>>ctotgs.authenticator;

    //****ticket_tgs 解密
    char* ticket_decrypt=decrypt(ctotgs.m_ticket_tgs,TGSkey_n,TGSkey_d);
    QByteArray ticket_decrypt_data(ticket_decrypt);
    ticket_decrypt_data.remove(0,1);
    ticket_decrypt_data=QByteArray::fromHex(ticket_decrypt_data);

    //****qbytearr change ticket_tgs
    QBuffer ticketbuf(&ticket_decrypt_data);
    ticketbuf.open(QIODevice::ReadOnly);
    QDataStream ticketin(&ticketbuf);
    Ticket ticketnew;
    ticketin>>ticketnew.Key_cn>>ticketnew.IDc>>ticketnew.ID>>ticketnew.TS>>ticketnew.lifetime;

    //****authenticator 解密
     QByteArray auth=DES_de(ticketnew.Key_cn,ctotgs.authenticator);

    //****qbytearr change authenticator
     Authenticator authresult;
     QBuffer authbuffer(&auth);
     authbuffer.open(QIODevice::ReadOnly);
     QDataStream authin(&authbuffer);
     authin>>authresult.IDc>>authresult.TS;

     //****生成ticket_v
     Ticket ticket_v;
     ticket_v.Key_cn=Get_K();
     ticket_v.IDc=ticketnew.IDc;
     ticket_v.ID=ctotgs.IDv;
     QTime ts4time=QTime::currentTime();
     ticket_v.TS=ts4time.toString("h:m:s.z");
     ticket_v.lifetime=24;

     //****ticket_v转为QBytearr
     QByteArray qbyteticket_v;
     QBuffer ticketvbuffer(&qbyteticket_v);
     ticketvbuffer.open(QIODevice::WriteOnly);
     QDataStream ticketvout(&ticketvbuffer);
     ticketvout<<ticket_v.Key_cn<<ticket_v.IDc<<ticket_v.ID<<ticket_v.TS<<ticket_v.lifetime;
     ticketvbuffer.close();

     //****ticket_v加密
     qbyteticket_v=qbyteticket_v.toHex();
     qbyteticket_v.insert(0,QByteArray("1"));
     char* qbyteticket_v_encrypt=encrypt(qbyteticket_v.data(),Vkey_n,e);

     //****TGStoC
     TGStoC tgstoc;
     tgstoc.IDv=ctotgs.IDv;
     tgstoc.Kcv=ticket_v.Key_cn;
     tgstoc.TS4=ticket_v.TS;
     tgstoc.m_ticket_v=qbyteticket_v_encrypt;

     //****TGStoC转为QBytearr
     QByteArray qbytetgstoc;
     QBuffer tgstocbuffer(&qbytetgstoc);
     tgstocbuffer.open(QIODevice::WriteOnly);
     QDataStream tgstocout(&tgstocbuffer);
     tgstocout<<tgstoc.IDv<<tgstoc.Kcv<<tgstoc.TS4<<tgstoc.m_ticket_v;
     tgstocbuffer.close();

     //****TGStoC加密并发送
     QByteArray tgstocSend=DES_en(ticketnew.Key_cn,qbytetgstoc);
     package_message_noencrypt(5,0,tgstocSend,this);
     int adb=1;

}
