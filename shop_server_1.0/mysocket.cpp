#include "mysocket.h"

#include <QMessageBox>

int e=65537;
MYsocket::MYsocket(int sockDesc,QObject *parent) : QTcpSocket(parent)
{
    p->n=Vkey_n;
    p->d=Vkey_d;
    p->e=65537;
    socket=this;
    m_sockDesc=sockDesc;
    {
        mutexx.lock();
        db = QSqlDatabase::addDatabase("QMYSQL", QString("%1").arg(sockDesc));
        db.setHostName("localhost");
        db.setDatabaseName("netshop");       //这里输入你的数据库名
        db.setUserName("root");
        db.setPassword("dm089084...");   //这里输入你的密码
        /*db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setPort(3306);
        db.setDatabaseName("netshop");       //这里输入你的数据库名
        db.setUserName("root");
        db.setPassword("123456");   //这里输入你的密码*/
        if (!db.open())
        {
            qDebug() << "thread 1 open db failed!";
        }
        mutexx.unlock();
        qDebug() << "thread 1: " << QString("%1").arg(sockDesc);
    }
    connect(this, SIGNAL(readyRead()), this, SLOT(socket_Read_Data()));
}

void MYsocket::socket_Read_Data()//当有数据过来之后读数据
{
    QByteArray buffer = socket->readAll();
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
        res.sign_length=header->sign_length;
        for(int i=0;i<4;i++)
        {
            res.des_ip[i]=header->des_ip[i];
            res.des_src[i]=header->des_src[i];

        }

        res.data=t2.left(header->message_length);
        res.sign=t2.right(header->sign_length);
        //后面为解密并且还原，这里需要注意，需要设计如何选择解密方法以及密钥，还有，不要忘记解密完要调用remove和fromHex

        buffer = m_buffer.right(total_length -all_len);
        total_length = buffer.size();
        m_buffer = buffer;

        recv_mes=res;

        message_handel();//处理收到的报文

    }
}

void MYsocket::message_handel()//处理收到的报文，并执行相应的操作
{

    int message_type=int(recv_mes.message_type);//报文类型
    int field_type=int(recv_mes.field_type);//字段类型
    switch(message_type)
    {
        case 0://收到错误报文
        {
            switch(field_type)//检测报文类型
            {
            case 0:
                break;
            case 1:
                break;
            }
            break;
        }

        case 1://收到普通用户报文
        {
            switch(field_type)//检测报文类型
            {
            case 0:
                break;
            case 1:
                break;
            }
            break;
        }

        case 65://普通用户控制报文
        {
            switch(field_type)
            {
            case 6:
                V_handel();
                break;
            case 7://更改购物车内商品数量、清理购物车
                Update_trolley_num();
                break;
            case 8://修改个人信息
                modinf_handel();
                break;
            case 9://处理加入购物车
                addcart_handle();
                break;
            case 10://商品页直接购买
                Goods_order();
                break;
            case 11://购物车下单
                Trolley_order();
                break;
            case 12:
                VIP_handel();
                break;
            case 14://处理登陆请求
                qDebug()<<"logoin check";
                Login_Check();
                break;
            case 15://处理商品表查询
                qDebug()<<"inquire goods";
                Inquire_goods();
                break;
            case 16:
                qDebug()<<"inquire trolley";
                Inquire_trolley();
                break;
            case 19:
                qDebug()<<"inquire specified goods";
                Inquire_goods();
                break;
            case 20:
                qDebug()<<"inquire different kinds of goods";
                Inquire_goods();
                break;
            case 22:
                qDebug()<<"inquire specified goods trolley";
                Inquire_trolley();
                break;
            case 23:
                Inquire_trolley();
                break;
            case 24://查询用户信息
                userinf_handel();
                break;
            case 25://修改密码
                modpsw_handel();
                break;
            case 26:
                qDebug()<<"page jump";
                Page_jump();
                break;
            case 27:
                qDebug()<<"page jump trolley";
                Page_jump_t();
                break;
            case 28:
                Inquire_all_trolley();
                break;

            }
        }
        case 66:
        {
            switch(field_type)//检测报文类型
            {
            case 1:
                Login_Check();
                break;
            case 2:
                inquire_sql_to_manager();
                break;
            case 3:
                insert_good_table();
                break;
            }
            break;
        }
        case 2:
        {
            switch(field_type)//检测报文类型
            {
            case 1:
                delete_good();
                break;
            case 2:
                delete_good();
                break;
            case 3:
                manager_pic();
                break;
            case 4:
                insert_good_table();
                break;
            }
            break;
        }
    }
}

bool  MYsocket::modify_db(QString txt) {//增删改
    query = new QSqlQuery(db);

    if(!query->exec(txt))
    {
        QSqlError lastError = query->lastError();
        qDebug() << lastError.text();
        return false;
    }
    return true;
}

QSqlQuery MYsocket::query_db(QString txt) {//查询
    query = new QSqlQuery(db);
    if(!query->exec(txt))
    {
        QSqlError lastError = query->lastError();
        qDebug() << lastError.text();
    }
    return *query;
}

QSqlTableModel* MYsocket::model_db(QString txt) {//查询
   model = new QSqlTableModel(nullptr,db);

   model->setTable(txt);
    if(model->select()){//不是空表
        qDebug()<<"返回成功";
        return model;
    }
    else
    {
        qDebug()<<"该表为空表";
        return model;
    }
}

void MYsocket::sql_to_table(QString txt)
{
    model=model_db(txt);
    int row=model->rowCount();
    int col=model->columnCount();
    send_table.clear();
    send_img_byte.clear();
    QVector<QString> temp;

    if(txt=="good")
    {
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<col-1;j++)
            {
                temp.push_back(model->record(i).value(j).toString());
             }
            send_table.push_back(temp);
            temp.clear();
        }

    }
    else
    {
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<col;j++)
            {
                //item=new QStandardItem();
                //res->setItem(i,j,item);
                temp.push_back(model->record(i).value(j).toString());

            }
            send_table.push_back(temp);
            temp.clear();
        }
    }
    return;
}

void MYsocket::V_handel()//用户认证
{
    //****解析报文中的数据
    CtoV ctov;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>ctov.m_ticket_v>>ctov.authenticator;

    //****ticket_V 解密
    char* ticket_decrypt=decrypt(ctov.m_ticket_v,Vkey_n,Vkey_d);
    QByteArray ticket_decrypt_data(ticket_decrypt);
    ticket_decrypt_data.remove(0,1);
    ticket_decrypt_data=QByteArray::fromHex(ticket_decrypt_data);

    //****qbytearr change ticket_v
    QBuffer ticketbuf(&ticket_decrypt_data);
    ticketbuf.open(QIODevice::ReadOnly);
    QDataStream ticketin(&ticketbuf);
    Ticket ticketnew;
    ticketin>>ticketnew.Key_cn>>ticketnew.IDc>>ticketnew.ID>>ticketnew.TS>>ticketnew.lifetime;
    g_Key_cv=ticketnew.Key_cn;
    g_idc=ticketnew.IDc;
    //****查询c的公钥

    QSqlQuery query(db);
    QString txt=QString("select Upubkey from user where Uid=%1").arg(g_idc);
    bool sucess=query.exec(txt);
    if(query.first()){
        QString ckey_nstr=query.value(0).toString();
        std::string str=ckey_nstr.toStdString();
        g_Ckey_n=new char[strlen(str.c_str())+1];
        strcpy(g_Ckey_n,str.c_str());
    }


    //****authenticator 解密
     QByteArray auth=DES_de(ticketnew.Key_cn,ctov.authenticator);

    //****qbytearr change authenticator
     Authenticator authresult;
     QBuffer authbuffer(&auth);
     authbuffer.open(QIODevice::ReadOnly);
     QDataStream authin(&authbuffer);
     authin>>authresult.IDc>>authresult.TS;

     //****VtoC
     VtoC vtoc;
     vtoc.TS5=authresult.TS;//TS5+1;
     QTime ts5=QTime::fromString(vtoc.TS5,"h:m:s.z");
     ts5=ts5.addSecs(1);
     vtoc.TS5=ts5.toString("h:m:s.z");

     //****VtoC转为QBytearr
     QByteArray qbytevtoc;
     QBuffer vtocbuffer(&qbytevtoc);
     vtocbuffer.open(QIODevice::WriteOnly);
     QDataStream vtocout(&vtocbuffer);
     vtocout<<vtoc.TS5;
     vtocbuffer.close();

     //****TGStoC加密并发送
     QByteArray vtocSend=DES_en(ticketnew.Key_cn,qbytevtoc);
     package_message_noencrypt(67,1,vtocSend,socket);
     int adb=1;
     DES_key_to_client=g_Key_cv;
     //DES_key_to_client=QByteArray::fromHex(DES_key_to_client);

}

void MYsocket::Login_Check()//登录检验
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);
    //res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);
    //不要数字签名的话这一行去掉，DES的解密，res为message类型，res.data为message的data部分
    sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);
    //不要数字签名的话这一行去掉

    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadOnly);
    QDataStream in(&buf);
    QByteArray qbt_sql;
    in>>qbt_sql;
    QString sql(qbt_sql.data());
    buf.close();

    QByteArray data;
    QBuffer buff(&data);
    buff.open(QIODevice::WriteOnly);
    QDataStream out(&buff);

    QSqlQuery inquire(db);
    if(!inquire.exec(sql))
         qDebug()<<"inquire failed!";

    bool iqur=inquire.next();
    if(iqur)
    {
        //返回登录成功
        out<<iqur;
        //返回个人信息
        if(inquire.value(0).toString()!="100001")
        {
            for(int i=0;i<6;i++)
            {
                if(i<5)
                {
                    QString qstr_value=inquire.value(i).toString();
                    QByteArray qbt_value=qstr_value.toUtf8();
                    //out<<qbt_value;
                }
                else
                {
                    QImage head_img;
                    head_img.loadFromData(inquire.value(i).toByteArray(),"JPG");
                    QByteArray head_qbt=getImageData(head_img);
                    out<<head_qbt;
                }
            }
        }
    }
    else
    {
        //返回登录失败
        qDebug()<<"login error";
        out<<iqur;
    }
    buff.close();
    package_message_DES(67,6,data,socket,DES_key_to_client,p->n,p->d);
}

void MYsocket::Inquire_goods()//查询商品
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);
    //res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);
    //不要数字签名的话这一行去掉，DES的解密，res为message类型，res.data为message的data部分
    sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);
    //不要数字签名的话这一行去掉

    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadOnly);
    QDataStream in(&buf);
    QByteArray qbt_sql;
    in>>qbt_sql;
    QString sql(qbt_sql.data());
    buf.close();

    QByteArray data;
    QBuffer buff(&data);
    buff.open(QIODevice::WriteOnly);
    QDataStream out(&buff);

    QSqlQuery inquire(db);
    if(!inquire.exec(sql))
        qDebug()<<"inquire failed!";

    QVector<QVector<QString>> goods_infm;
    QVector<QByteArray> goods_img;
    QVector<QString> temp;
    int goods_num=6;

    bool iqur=inquire.next();
    if(iqur)
    {
        //返回查询成功
        out<<iqur;
        //返回商品信息
        for(int i=0;i<6;i++)//遍历记录
        {
            temp.clear();
            for(int j=0;j<6;j++)//遍历字段
            {
                if(j<5)//信息
                {
                    QString qstr_value=inquire.value(j).toString();
                    //qDebug()<<qstr_value;
                    temp.push_back(qstr_value);
                }
                else//图片
                {
                    QImage img;
                    img.loadFromData(inquire.value(j).toByteArray(),"JPG");
                    QByteArray img_qbt=getImageData(img);
                    goods_img.push_back(img_qbt);
                }
            }
            goods_infm.push_back(temp);
            goods_num=i+1;
            if(!inquire.next())break;
        }
        out<<goods_num<<goods_infm<<goods_img;
    }
    else
    {
        //返回查询失败
        qDebug()<<"inquire goods error";
        out<<iqur;
    }
    buff.close();
    package_message_DES(3,2,data,socket,DES_key_to_client,p->n,p->d);
}

void MYsocket::Page_jump()//页面跳转
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);
    //res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);
    //不要数字签名的话这一行去掉，DES的解密，res为message类型，res.data为message的data部分
    sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);
    //不要数字签名的话这一行去掉

    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadOnly);
    QDataStream in(&buf);
//    QByteArray qbt_sql;
    QVector<QString> sql;
    int page;
//    in>>qbt_sql>>page;
    in>>sql>>page;
//    QString sql(qbt_sql.data());

    buf.close();

    QByteArray data;
    QBuffer buff(&data);
    buff.open(QIODevice::WriteOnly);
    QDataStream out(&buff);

    QSqlQuery inquire_num(db);
    if(!inquire_num.exec(sql[0]))
        qDebug()<<"inquire failed!";

    int iqur=inquire_num.next();
    qDebug()<<iqur;
    if(iqur)
    {
        //查询成功
        out<<iqur;
        int num=inquire_num.value(0).toInt();
        //判断page（用户目标页）和num（商品总数）的关系
        int start_num=6*(page-1);
        int page_ok;
        if(start_num<num)//<<iqur<<page_ok<<goods_num<<goods_infm<<goods_img
        {
            page_ok=1;
            out<<page_ok;
            //当前页可查，返回商品
//            QString sql2="SELECT * FROM GOOD LIMIT "+QString::number(start_num)+","+QString::number(start_num+6)+";";

            QSqlQuery inquire(db);
//            if(!inquire.exec(sql2))
            if(!inquire.exec(sql[1]))
                qDebug()<<"inquire failed!";
            QVector<QVector<QString>> goods_infm;
            QVector<QByteArray> goods_img;
            QVector<QString> temp;
            int goods_num=6;
            bool check=inquire.next();
            qDebug()<<check;

            for(int i=0;i<6;i++)//遍历记录
            {
                temp.clear();
                for(int j=0;j<6;j++)//遍历字段
                {
                    if(j<5)//信息
                    {
                        QString qstr_value=inquire.value(j).toString();
                        //qDebug()<<qstr_value;
                        temp.push_back(qstr_value);
                    }
                    else//图片
                    {
                        QImage img;
                        img.loadFromData(inquire.value(j).toByteArray(),"JPG");
                        QByteArray img_qbt=getImageData(img);
                        goods_img.push_back(img_qbt);
                    }
                }
                goods_infm.push_back(temp);
                goods_num=i+1;
                if(!inquire.next())break;
            }
            out<<goods_num<<goods_infm<<goods_img;
        }
        else//<<iqur<<page_ok<<max_page
        {
            page_ok=0;
            out<<page_ok;
            //返回最大页数
//            int max_page=num/6;
//            if(max_page%6) max_page++;
            int max_page=num/6;
            if(num%6) max_page++;
            out<<max_page;
        }

    }
    else//<<iqur
    {
        //返回查询失败
        qDebug()<<"inquire goods error";
        out<<iqur;
    }
    buff.close();
    package_message_DES(67,8,data,socket,DES_key_to_client,p->n,p->d);
}

void MYsocket::Inquire_spcf_goods()
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);
    //res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);
    //不要数字签名的话这一行去掉，DES的解密，res为message类型，res.data为message的data部分
    sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);
    //不要数字签名的话这一行去掉

    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadOnly);
    QDataStream in(&buf);
    QByteArray qbt_sql;
    in>>qbt_sql;
    QString sql(qbt_sql.data());
    buf.close();

    QByteArray data;
    QBuffer buff(&data);
    buff.open(QIODevice::WriteOnly);
    QDataStream out(&buff);

    QSqlQuery inquire(db);
    if(!inquire.exec(sql))
         qDebug()<<"inquire failed!";

    QVector<QVector<QString>> goods_infm;
    QVector<QByteArray> goods_img;
    QVector<QString> temp;
    int goods_num=6;

    bool iqur=inquire.next();
    if(iqur)
    {
        //返回查询成功
        out<<iqur;
        //返回商品信息
        for(int i=0;i<6;i++)//遍历记录
        {
            temp.clear();
            for(int j=0;j<6;j++)//遍历字段
            {
                if(j<5)//信息
                {
                    QString qstr_value=inquire.value(j).toString();
                    //qDebug()<<qstr_value;
                    temp.push_back(qstr_value);
                }
                else//图片
                {
                    QImage img;
                    img.loadFromData(inquire.value(j).toByteArray(),"JPG");
                    QByteArray img_qbt=getImageData(img);
                    goods_img.push_back(img_qbt);
                }
            }
            goods_infm.push_back(temp);
            goods_num=i+1;
            if(!inquire.next())break;
        }
        out<<goods_num<<goods_infm<<goods_img;
    }
    else
    {
        //返回查询失败
        qDebug()<<"inquire goods error";
        out<<iqur;
    }
    buff.close();
    package_message_DES(3,2,data,socket,DES_key_to_client,p->n,p->d);
}

void MYsocket::Inquire_trolley()//查询购物车
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);//不要数字签名的话这一行去掉
    sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);//不要数字签名的话这一行去掉

    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadOnly);
    QDataStream in(&buf);
    QByteArray qbt_sql;
    in>>qbt_sql;
    QString sql(qbt_sql.data());
    buf.close();

    QByteArray data;
    QBuffer buff(&data);
    buff.open(QIODevice::WriteOnly);
    QDataStream out(&buff);

    QSqlQuery inquire(db);
    if(!inquire.exec(sql))
        qDebug()<<"inquire failed!";

    QVector<QVector<QString>> goods_infm;
    QVector<QByteArray> goods_img;
    QVector<QString> temp;
    int goods_num=6;

    bool iqur=inquire.next();
    if(iqur)
    {
        //返回查询成功
        out<<iqur;
        //返回商品信息
        for(int i=0;i<6;i++)//遍历记录
        {
            temp.clear();
            for(int j=0;j<7;j++)//遍历字段
            {
                if(j<6)//信息
                {
                    QString qstr_value=inquire.value(j).toString();
                    //qDebug()<<qstr_value;
                    temp.push_back(qstr_value);
                }
                else//图片
                {
                    QImage img;
                    img.loadFromData(inquire.value(j).toByteArray(),"JPG");
                    QByteArray img_qbt=getImageData(img);
                    goods_img.push_back(img_qbt);
                }
            }
            goods_infm.push_back(temp);
            goods_num=i+1;
            if(!inquire.next())break;
        }
        out<<goods_num<<goods_infm<<goods_img;
    }
    else
    {
        //返回查询失败
        qDebug()<<"inquire goods error";
        out<<iqur;
    }
    buff.close();
    package_message_DES(3,4,data,socket,DES_key_to_client,p->n,p->d);
}

void MYsocket::addcart_handle()//加购物车
{
    //****des解密
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);      //不要数字签名的话这一行去掉

    //****解析报文中的数据
    int idc;
    QVector<QVector<QString>> reccart;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>idc>>reccart;

    QSqlQuery query(db);
    bool sendsuccess;
    //****处理数据，写入数据库
    for(int i=0;i<reccart.size();++i){
        int num=0;
        bool success;
        query.exec(QString("select snum from cart where gname='%1'and uid=%2").arg(reccart[i][0]).arg(idc));
        if(query.first()){
            num=query.value(0).toInt();
        }
        if(num==0){//no good
            success=query.exec(QString("insert into cart values('%1',%2,%3,%4,%5)")
                               .arg(reccart[i][0]).arg(reccart[i][1].toInt()).arg(reccart[i][2].toDouble())
                               .arg(reccart[i][3].toDouble()).arg(idc));
        }
        else{
            int upda=num+reccart[i][1].toInt();
            success=query.exec(QString("update cart set snum=%1 where gname='%2'and uid=%3").arg(upda).arg(reccart[i][0]).arg(idc));
        }
        sendsuccess=success;
    }

    QByteArray infSend;
    QBuffer infbuffer(&infSend);
    infbuffer.open(QIODevice::WriteOnly);
    QDataStream infout(&infbuffer);
    infout<<sendsuccess;
    infbuffer.close();

    package_message_DES(65,9,infSend,socket,DES_key_to_client,Vkey_n,Vkey_d);
}

void MYsocket::userinf_handel(){
    //****des解密
    recv_mes.data=DES_de(g_Key_cv,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(g_Key_cv,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,e);      //不要数字签名的话这一行去掉

    //****解析报文中的数据
    QString txtinf,txtorder,txtvip;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>txtinf>>txtorder>>txtvip;

    //****查询个人信息
    QString uname,uphe;//图片到时候要改
    QByteArray upic;
    QImage pic;
    QSqlQuery query(db);
    bool sucess=query.exec(txtinf);
    if(query.first()){
        uname=query.value(0).toString();
        uphe=query.value(1).toString();
        pic.loadFromData(query.value(2).toByteArray(),"JPG");
        upic=getImageData(pic);
    }

    //****查询vip
    bool vip=false;
    QString datestring;
    QSqlQuery queryvip(db);
    queryvip.exec(txtvip);
    if(queryvip.first()){
        vip=true;
        datestring=queryvip.value(0).toString();
    }

    //****查询订单信息
    QVector<QVector<QString>> orders;
    QSqlQueryModel* model= new QSqlQueryModel;
    model->setQuery(txtorder,db);
    int row=model->rowCount();
    int col=model->columnCount();
    for(int i=0;i<row;++i){
        QVector<QString> qstr;
        for(int j=0;j<col;++j){
           qstr.push_back(model->data(model->index(i,j)).toString());
        }
        orders.push_back(qstr);
    }
    int order_size=orders.size();

    //****打包发送
    QByteArray infSend;
    QBuffer infbuffer(&infSend);
    infbuffer.open(QIODevice::WriteOnly);
    QDataStream infout(&infbuffer);
    infout<<uname<<uphe<<upic<<vip;
    if(vip)infout<<datestring;
    infout<<order_size;
    if(order_size>0)infout<<orders;
    infbuffer.close();

    package_message_DES(65,24,infSend,socket,g_Key_cv,Vkey_n,Vkey_d);
    int adsk=1;
}

void MYsocket::modinf_handel()
{
    //****des解密
    recv_mes.data=DES_de(g_Key_cv,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(g_Key_cv,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,e);      //不要数字签名的话这一行去掉

    //****解析报文中的数据
    bool ifimg;
    QString txt;
    QByteArray upic;
    QString name,phone;
    int id;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>ifimg;
    if(ifimg)recin>>name>>phone>>upic>>id;
    else recin>>txt;

    QImage img=getImage(upic);
    //ui->label->setPixmap(QPixmap::fromImage(img));
    QByteArray imageData;
    QBuffer buffer(&imageData);
    img.save(&buffer, "jpg");
    QVariant var(imageData);

    //****修改
    bool sucess;
    QSqlQuery query(db);
    if(ifimg){
        query.prepare("update user set uname=?,uphe=?,upic=? where uid=?");

        query.addBindValue(name);

        query.addBindValue(phone);

        query.addBindValue(var);

        query.addBindValue(id);

        sucess=query.exec();
        if(!sucess){
            QSqlError le=query.lastError();
            qDebug()<<le.text();
        }
    }
    else{
    sucess=query.exec(txt);}

    //****打包发送
    QByteArray infSend;
    QBuffer infbuffer(&infSend);
    infbuffer.open(QIODevice::WriteOnly);
    QDataStream infout(&infbuffer);
    infout<<sucess;
    infbuffer.close();

    package_message_DES(65,8,infSend,socket,g_Key_cv,Vkey_n,Vkey_d);
    int adsk=1;
}

void MYsocket::modpsw_handel(){
    //****des解密
    recv_mes.data=DES_de(g_Key_cv,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(g_Key_cv,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,e);      //不要数字签名的话这一行去掉

    //****解析报文中的数据
    QString txtfind,oldpsw,newpsw;
    int userid;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>txtfind>>userid>>oldpsw>>newpsw;

    //****查询原来的密码
    QString dbpsw;
    QSqlQuery query(db);
    bool sucess=query.exec(txtfind);
    if(query.first()){
        dbpsw=query.value(0).toString();
    }

    bool ifmodpsw=false;
    QString newstrMD5,oldstrMD5;
    //****对比密码
    QByteArray oldpswarr,newpswarr;
    oldpswarr.append(oldpsw);
    newpswarr.append(newpsw);
    QByteArray oldhash = QCryptographicHash::hash(oldpswarr, QCryptographicHash::Md5);
    QByteArray newhash = QCryptographicHash::hash(newpswarr, QCryptographicHash::Md5);
    oldstrMD5 = oldhash.toHex();
    newstrMD5=newhash.toHex();
    if(oldstrMD5==dbpsw){
        ifmodpsw=true;
        QString txtupdate=QString("update user set upass='%1' where uid=%2").arg(newstrMD5).arg(userid);
        if(!query.exec(txtupdate)){
            ifmodpsw=false;;
        }
    }

    QByteArray infSend;
    QBuffer infbuffer(&infSend);
    infbuffer.open(QIODevice::WriteOnly);
    QDataStream infout(&infbuffer);
    infout<<ifmodpsw;
    infbuffer.close();

    package_message_DES(65,25,infSend,socket,g_Key_cv,Vkey_n,Vkey_d);
    int adsk=1;


}

void MYsocket::VIP_handel(){
    //****des解密
    recv_mes.data=DES_de(g_Key_cv,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(g_Key_cv,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,e);      //不要数字签名的话这一行去掉

    //****解析报文中的数据
    int type, userid;
    QString uname;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>userid>>uname>>type;

    QDate date = QDate::currentDate();
    if(type==1){
        date=date.addMonths(1);
    }
    if(type==2){
        date=date.addMonths(3);
    }
    if(type==3){
        date=date.addYears(1);
    }
    QString datestring=date.toString("yyyy-MM-dd");
    QSqlQuery query(db);
    bool sucess=query.exec(QString("insert into vip(Uid,Endtime) values(%1,'%2')").arg(userid).arg(datestring));

    QByteArray infSend;
    QBuffer infbuffer(&infSend);
    infbuffer.open(QIODevice::WriteOnly);
    QDataStream infout(&infbuffer);
    infout<<sucess<<datestring;
    infbuffer.close();

    package_message_DES(65,12,infSend,socket,g_Key_cv,Vkey_n,Vkey_d);

    int ahk=1;
}

void MYsocket::inquire_sql_to_manager()//处理管理员查询表请求
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了

    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分

    bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);                  //不要数字签名的话这一行去掉
    //qDebug()<<"res"<<recv_mes.data.toHex();
    //qDebug()<<"res"<<recv_mes.sign.toHex();
    qDebug()<<judge;
    //QString txt=QString(recv_mes.data);
    QBuffer buffer1(&recv_mes.data);
    buffer1.open(QIODevice::ReadOnly);
    QDataStream in(&buffer1);
    QString txt;
    in>>txt;
    sql_to_table(txt);
    QByteArray send_data;
    QBuffer buffer(&send_data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out<<send_table;

//    if(txt=="good")
//    {
//        out<<send_img_byte;
//        qDebug()<<send_img_byte[0].length();
//    }
    buffer.close();
    int i=3;
    int y=3;
    package_message_DES(i,y,send_data,socket,DES_key_to_client,p->n,p->d);//数字签名DES加密

}

void MYsocket::insert_good_table()
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);
    qDebug()<<judge;
    QBuffer buffer(&recv_mes.data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);
    QVector<QString>recv_str;
    QByteArray img_data;
    in>>recv_str>>img_data;
    QImage img;
    img=getImage(img_data);
    bool sucess;
    QSqlQuery query(db);

    query.prepare("insert into good(Gname,Gnum,Gprice,Gdiscount,Gtype,Gimage) values(?,?,?,?,?,?)");

    query.addBindValue(recv_str[0]);

    query.addBindValue(recv_str[1]);

    query.addBindValue(recv_str[2]);

    query.addBindValue(recv_str[3]);
    query.addBindValue(recv_str[4]);
    QVariant var(img_data);
    query.addBindValue(var);
    mutexx_t.lock();
    sucess=query.exec();
    if(!sucess){
        QSqlError le=query.lastError();
        qDebug()<<le.text();
   }
    mutexx_t.unlock();
    sql_to_table("good");
    QByteArray send_data;
    QBuffer buffer1(&send_data);
    buffer1.open(QIODevice::WriteOnly);
    QDataStream out(&buffer1);
    out<<send_table;
    out<<send_img_byte;
    buffer1.close();
    int i=3;
    int y=3;
    package_message_DES(i,y,send_data,socket,DES_key_to_client,p->n,p->d);//数字签名DES加密

}

void MYsocket::delete_good()
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);
    qDebug()<<judge;
    QBuffer buffer(&recv_mes.data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);
    QString delete_sql;
    in>>delete_sql;
    mutexx_t.lock();
    bool success=modify_db(delete_sql);
    mutexx_t.unlock();
    sql_to_table("good");
    QByteArray send_data;
    QBuffer buffer1(&send_data);
    buffer1.open(QIODevice::WriteOnly);
    QDataStream out(&buffer1);
    out<<send_table;
    //out<<send_img_byte;
    buffer1.close();
    int i=3;
    int y=3;
    package_message_DES(i,y,send_data,socket,DES_key_to_client,p->n,p->d);//数字签名DES加密
}

void MYsocket::manager_pic()
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);
    qDebug()<<judge;
    QBuffer buffer(&recv_mes.data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);
    QString manager_select_sql;
    in>>manager_select_sql;
    QSqlQuery query(db);
    bool sucess=query.exec(manager_select_sql);
    QImage pic;
    QByteArray upic;
    if(query.first()){
       pic.loadFromData(query.value(0).toByteArray(),"JPG");
       upic=getImageData(pic);
    }
    QByteArray send_data;
    QBuffer buffer1(&send_data);
    buffer1.open(QIODevice::WriteOnly);
    QDataStream out(&buffer1);
    out<<upic;
    buffer1.close();
    int i=67;
    int y=4;
    package_message_DES(i,y,send_data,socket,DES_key_to_client,p->n,p->d);//数字签名DES加密
}

//*****220520*****//

void MYsocket::Page_jump_t()//购物车页面跳转
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);
    sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);

    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadOnly);
    QDataStream in(&buf);
    QVector<QString> sql;
    int page;
    in>>sql>>page;

    //qDebug()<<sql[0];
    //qDebug()<<sql[1];

    buf.close();

    QByteArray data;
    QBuffer buff(&data);
    buff.open(QIODevice::WriteOnly);
    QDataStream out(&buff);

    QSqlQuery inquire_num(db);
    if(!inquire_num.exec(sql[0]))
        qDebug()<<"inquire failed!";

    int iqur=inquire_num.next();
    if(iqur)
    {
        //查询成功
        out<<iqur;
        int num=inquire_num.value(0).toInt();
        //判断page（用户目标页）和num（商品总数）的关系
        int start_num=6*(page-1);
        int page_ok;
        if(start_num<num)//<<iqur<<page_ok<<goods_num<<goods_infm<<goods_img
        {
            page_ok=1;
            out<<page_ok;
            //当前页可查，返回商品
//            QString sql2="SELECT * FROM GOOD LIMIT "+QString::number(start_num)+","+QString::number(start_num+6)+";";

            QSqlQuery inquire(db);
//            if(!inquire.exec(sql2))
            if(!inquire.exec(sql[1]))
                qDebug()<<"inquire failed!";

            QVector<QVector<QString>> goods_infm;
            QVector<QByteArray> goods_img;
            QVector<QString> temp;
            int goods_num=6;
            bool check=inquire.next();
            int a=inquire.size();

            for(int i=0;i<6;i++)//遍历记录
            {
                temp.clear();
                for(int j=0;j<7;j++)//遍历字段
                {
                    if(j<6)//信息
                    {
                        QString qstr_value=inquire.value(j).toString();
                        //qDebug()<<qstr_value;
                        temp.push_back(qstr_value);
                    }
                    else//图片
                    {
                        QImage img;
                        img.loadFromData(inquire.value(j).toByteArray(),"JPG");
                        QByteArray img_qbt=getImageData(img);
                        goods_img.push_back(img_qbt);
                    }
                }
                goods_infm.push_back(temp);
                goods_num=i+1;

                if(!inquire.next())break;
            }
            out<<goods_num<<goods_infm<<goods_img;
        }
        else//<<iqur<<page_ok<<max_page
        {
            page_ok=0;
            out<<page_ok;
            //返回最大页数
            //int max_page=num/6;
            //if(max_page%6) max_page++;
            int max_page=num/6;////////////////////////////////////////////////////////////////////////改
            if(num%6) max_page++;//max_page -> num/////////////////////////////////////////////////////改
            out<<max_page;
        }

    }
    else//<<iqur
    {
        //返回查询失败
        qDebug()<<"inquire goods error";
        out<<iqur;
    }
    buff.close();
    package_message_DES(67,9,data,socket,DES_key_to_client,p->n,p->d);
}

void MYsocket::Update_trolley_num()//更改购物车内商品数量、清理购物车
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);//不要数字签名的话这一行去掉
    sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);//不要数字签名的话这一行去掉

    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadOnly);
    QDataStream in(&buf);
    QByteArray qbt_sql;
    in>>qbt_sql;
    QString sql(qbt_sql.data());
    buf.close();

    QByteArray data;
    QBuffer buff(&data);
    buff.open(QIODevice::WriteOnly);
    QDataStream out(&buff);

    QSqlQuery update(db);
    if(!update.exec(sql))
    {
        qDebug()<<"inquire failed!";
    }
    else
    {
        package_message_DES(67,10,data,socket,DES_key_to_client,p->n,p->d);
    }
}

void MYsocket::Goods_order()//商品页直接购买商品
{
    QVector<QString> select_sql;
    QVector<QString> update_sql;
    QVector<QString> record_sql;
    QVector<QVector<QString>> under_order;
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);
    qDebug()<<judge;
    QBuffer buffer(&recv_mes.data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);
    QVector<QString>recv_str;
    QByteArray img_data;
    in>>under_order>>select_sql>>update_sql>>record_sql;
    buffer.close();
    mutexx_t.lock();
    bool sucess;
    bool num_sucess=true;
    bool final_suc=true;
    QByteArray send_data;
    QBuffer buffer1(&send_data);
    buffer1.open(QIODevice::WriteOnly);
    QDataStream out(&buffer1);
    for(int i=0;i<under_order.size();i++)
    {
       QSqlQuery query(db);
       sucess=query.exec(select_sql[i]);
       if(sucess==false)final_suc=false;
       if(query.first()){
          if(query.value(0).toInt()<under_order[i][1].toInt())//购买的数量超过商品表数量
          {
           num_sucess=false;
           final_suc=false;
           under_order[i][1]=query.value(0).toString();
          }
       }
       else
       {
           under_order.erase(under_order.begin() + i);//如果该商品根本不存在，则直接从缓存中去掉
           final_suc=false;
       }
    }
    if(final_suc==true&&num_sucess==true)
    {
       for(int i=0;i<under_order.size();i++)
       {
           QSqlQuery query(db);
           sucess=query.exec(update_sql[i]);
           sucess=query.exec(record_sql[i]);
       }
      out<<final_suc;
    }
    else
    {
       out<<final_suc<<under_order;
    }
    buffer1.close();
    int i=67;
    int y=3;
    package_message_DES(i,y,send_data,socket,DES_key_to_client,p->n,p->d);//数字签名DES加密
    mutexx_t.unlock();
}

void MYsocket::Trolley_order()//购物车下单
{
    QVector<QString> select_sql;
        QVector<QString> update_sql;
        QVector<QString> record_sql;
        QString del_cart;
        QVector<QVector<QString>> under_order;
        recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
        recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
        bool judge=sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);
        qDebug()<<judge;
        QBuffer buffer(&recv_mes.data);
        buffer.open(QIODevice::ReadOnly);
        QDataStream in(&buffer);
        QVector<QString>recv_str;
        QByteArray img_data;
        in>>under_order>>select_sql>>update_sql>>record_sql>>del_cart;
        buffer.close();
        //前面都是接包

        mutexx_t.lock();
        bool sucess;
        bool num_sucess=true;//这个变量来判断购买的数量是否超过商品表的数量
        bool final_suc=true;//这个变量用来判断是否所有的查询都通过了
        QByteArray send_data;
        QBuffer buffer1(&send_data);
        buffer1.open(QIODevice::WriteOnly);
        QDataStream out(&buffer1);
    //    out<<upic;
    //    buffer1.close();

        //遍历sql语句，首先遍历所有的查询，看看购买的数量是否超过商品表数量
        for(int i=0;i<under_order.size();i++)
        {
            QSqlQuery query(db);
            sucess=query.exec(select_sql[i]);
            if(sucess==false)final_suc=false;
            if(query.first()){
               if(query.value(0).toInt()<under_order[i][1].toInt())//如果购买的数量超过商品表数量
               {
                num_sucess=false;
                final_suc=false;
                //under_order[i][1]=query.value(0).toString();//缓存更新到正确的数量，如果是购物车下单的话，这里没必要，只需要提醒用户更新购物车表就可以了
               }
            }
            else
            {
                //under_order.erase(under_order.begin() + i);//如果该商品根本不存在，则直接从缓存中去掉，如果是购物车下单的话，这里没必要，只需要提醒用户更新购物车表就可以了
                final_suc=false;
            }
        }
        if(final_suc==true&&num_sucess==true)//如果查询全部通过
        {
            QSqlQuery query(db);//执行更新商品表和添加销售记录语句，购物车下单还需要删除购物车表
            for(int i=0;i<under_order.size();i++)
            {
                sucess=query.exec(update_sql[i]);
                sucess=query.exec(record_sql[i]);
            }
             sucess=query.exec(del_cart);
           out<<final_suc;//告诉用户查询成功
        }
        else
        {
            out<<final_suc;//查询失败，通知用户失败，并且传新的缓存（如果是购物车下单的话，这里不需要传缓存，只需要提醒用户更新购物车表就可以了）
        }
        buffer1.close();
        int i=67;
        int y=11;
        package_message_DES(i,y,send_data,socket,DES_key_to_client,p->n,p->d);//数字签名DES加密
        mutexx_t.unlock();
}

void MYsocket::Inquire_all_trolley()//查询整个购物车
{
    recv_mes.data=DES_de(DES_key_to_client,recv_mes.data);
    recv_mes.sign=DES_de(DES_key_to_client,recv_mes.sign);//不要数字签名的话这一行去掉
    sign_judge(recv_mes.data,recv_mes.sign,g_Ckey_n,p->e);//不要数字签名的话这一行去掉

    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadOnly);
    QDataStream in(&buf);

    //QByteArray qbt_sql;
    QVector<QString> sql;
    in>>sql;
    //QString sql(qbt_sql.data());
    buf.close();

    QByteArray data;
    QBuffer buff(&data);
    buff.open(QIODevice::WriteOnly);
    QDataStream out(&buff);

    QSqlQuery inquire(db),inquire_num(db);
    if(!inquire_num.exec(sql[0]))
        //QMessageBox::information(this,"notice","inquire failed!");
        qDebug()<<"inquire failed!";
    if(!inquire.exec(sql[1]))
        //QMessageBox::information(this,"notice","inquire failed!");
        qDebug()<<"inquire failed!";

    QVector<QVector<QString>> goods_infm;
    QVector<QString> temp;

    bool iqur=inquire.next();
    bool iqur_n=inquire_num.next();
    int goods_num=0;

    if(iqur_n)
        goods_num=inquire_num.value(0).toInt();
    else
        iqur=false;

    if(iqur)
    {
        //返回查询成功
        out<<iqur;
        //返回商品信息
        QVector<QVector<QString>> tteemmpp;
        for(int i=0;i<goods_num;i++)//遍历记录
        {
            temp.clear();
            for(int j=0;j<4;j++)//遍历字段
            {
                QString qstr_value=inquire.value(j).toString();
                //qDebug()<<qstr_value;
                temp.push_back(qstr_value);
            }
            //goods_infm.push_back(temp);
            tteemmpp.push_back(temp);
            if(!inquire.next())break;
        }
        goods_infm=tteemmpp;
        out<<goods_num<<goods_infm;
    }
    else
    {
        //返回查询失败
        qDebug()<<"inquire goods error";
        out<<iqur;
    }
    buff.close();
    package_message_DES(67,12,data,socket,DES_key_to_client,p->n,p->d);
}
