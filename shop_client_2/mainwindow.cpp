//pragma execution_character_set(“utf-8”)

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTime>

#define AS_IP "127.0.0.1"
#define TGS_IP "127.0.0.1"
#define Server_IP "127.0.0.1"

static char Ckey1_n[]="b8e1aa68799754e0e6aa176cf21cc352acad2a32513f07c651d9b3191e26adfdc0679b6f9c17000db180a20cbbfc1bb5c845aaabf60bb4bb512f6c8f91a5f54848d59e774c340128c898a21011d613bb7221bf0ce32c31b80b140101d54471479fc1559aada00a04a9f2b3fd773fbff864c1ae9fa52efdff39d3277d2cbb2f3";
static char Ckey1_d[]="59122f9d41e74cb745a9f0bb4631e150756cc00599eaf9fdebf52c3a0fb2a6e4b7cbb2b124cc57f440ee2e246074e78cbebe01a9f3525472627cac9ac69ae3fbf46d8efb38b0615e59e6ceca8f08a6f950e122bdcb1c5f01c06a30635a89f1ff77e4bbb0157a96caecce9eed92badccc2bf402516a2159564c326bf5ac01ef1";
static char Ckey2_n[]="5bd604ccbf3c196d459ca001f88fbdac217c974e2db797427666691c6922d8fc6e620857ef75729406c372891fe4834a2796c28e097ff34fdfd786017682b3cf03429d933242c18f508d37fbeeafd939c8cae11265b865e0087b368d966262b7767dc8652bc7b5247cfa7df9f05a1b820db94e95a6253869f60542d85ffc686b";
static char Ckey2_d[]="48a6a92968e81c9d6b829abab46ed278480d8e8d547177022734a156ce7db8f70a4417a1a9585a8a6c475666d08a30ab0b62a30b46cca829ba458047cbff5e4b167a9787b50f67fd382340a4e8803bebdb52bca252616eabb36893744cf6aad9df412f50838e3f8862bfec4c584adf2d5c933aa90f878a38b395477e4ba0fb01";
static char Ckey3_n[]="28bce507db6c54db4fc1c68715540de0eff698794003ffdf015852b0d670fba564471117c9b1b233a4ac874a7b9045ec922e6bc18216453eac7f87e68577120850d5ca55d36435a35fee50604ea0c690e9500d908f246a32781ab6c21b3a11a3fb610fa5901650de1854e90033bd80e7bb5f164929831c0cbd841ed2b2217d4f";
static char Ckey3_d[]="1333341ec7e3fefa7e54ade4ecc078ea132efe1de775fb1a77dfd0d957b858625328cb7d92aa157b1199df09d3c2e3b1e413cce59f123000a1fc5968e46e8228bbad69852eb3cbe7c044461f6653111565eaedc33e00bf637defb2fcacf79706470bf622be0aedcf9f0abd4194bfa37b5337755478f6fd220e392f277e29d5c1";
int e=65537;
char Vkey_n[]="a42ec64809f7e777bae17b10d33bf5df4bb605d6124cda58fbe0d7322342bd3af61810e2ff62b33b0bcf09c5dd155d2a6e2fabd4ee655f78ad2cf19b0ada671893a843fe307e6eb9df63be5853dc97a48aeaf19996b05e471fb14bd0ee2a9f3d0d521c158d627855397900c218b78a6561228cf3064054de70a2b772e53d4f";
//char RSA_n[] = "b8e1aa68799754e0e6aa176cf21cc352acad2a32513f07c651d9b3191e26adfdc0679b6f9c17000db180a20cbbfc1bb5c845aaabf60bb4bb512f6c8f91a5f54848d59e774c340128c898a21011d613bb7221bf0ce32c31b80b140101d54471479fc1559aada00a04a9f2b3fd773fbff864c1ae9fa52efdff39d3277d2cbb2f3";
//char RSA_d[] = "59122f9d41e74cb745a9f0bb4631e150756cc00599eaf9fdebf52c3a0fb2a6e4b7cbb2b124cc57f440ee2e246074e78cbebe01a9f3525472627cac9ac69ae3fbf46d8efb38b0615e59e6ceca8f08a6f950e122bdcb1c5f01c06a30635a89f1ff77e4bbb0157a96caecce9eed92badccc2bf402516a2159564c326bf5ac01ef1";
//char DES_KEY[] = "01010000010000000000010101010101010001000100000100000100000101000100010001010100010100010100010101000100000101010101010000010000";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wlogin->show();
    notice=new QLabel(this);
    QPixmap pixmap=QPixmap(":/picture/good_change.png");
    pixmap=pixmap.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->pushButton->setIconSize(QSize(40,40));
    ui->pushButton->setIcon(pixmap);



    Set_style();//设置样式


    tmp_buffer.push_back("***");
    if(adbuf.empty())adbuf.push_back(tmp_buffer);
    wgood = new WGood(id,this,adbuf);

    ui->scrollArea->setWidget(wgood);
    wd=0;
    wtrolley=new WTrolley(id,this);

    //获取程序当前运行目录
    //QString operation_path = QCoreApplication::applicationDirPath();
    //qDebug() << operation_path;

    //connect(myuser,SIGNAL(Transmit_img_to_mwd(QImage)),this,SLOT(Change_head(QImage)));//更改首页头像

    connect(wlogin, SIGNAL(Send_uid_to_mwd(QString,QString)), this, SLOT(Login_check(QString, QString)));//登录成功传id
    //connect(myuser,SIGNAL(Send_to_wsk(QVector<QString>)),this,SLOT(Send_pack_show(QVector<QString>)));//抓包

    connect(wgood,SIGNAL(Refresh_goods_list(int)),this,SLOT(Refresh_goods(int)));//刷新商品页
    connect(wgood,SIGNAL(Get_goods_number(int)),this,SLOT(To_page(int)));//跳转页面
    connect(wgood,SIGNAL(Search_goods_list(QString)),this,SLOT(Search_goods(QString)));//搜索查询
    connect(wgood,SIGNAL(Search_species_list(int)),this,SLOT(Search_species(int)));//种类查询
    connect(wgood,SIGNAL(Change_view_mode(int)),this,SLOT(Change_goods_view_mode(int)));//更改商品页显示模式
    connect(wgood,SIGNAL(addcart(int,QVector<QVector<QString>>)),this,SLOT(Addcart_send(int,QVector<QVector<QString>>)));//加购物车

    connect(wtrolley,SIGNAL(Refresh_goods_list_t(int)),this,SLOT(Refresh_goods_t(int)));//刷新购物车页
    connect(wtrolley,SIGNAL(Change_view_mode_t(int)),this,SLOT(Change_goods_view_mode_t(int)));//更改购物车页显示模式
    connect(wtrolley,SIGNAL(Get_goods_number_t(int)),this,SLOT(To_page_t(int)));//购物车跳转页面
    connect(wtrolley,SIGNAL(Search_goods_list_t(QString)),this,SLOT(Search_goods_t(QString)));//购物车搜索查询
    connect(wtrolley,SIGNAL(Search_species_list_t(int)),this,SLOT(Search_species_t(int)));//购物车种类查询
    connect(wtrolley,SIGNAL(Update_trolley_num(QString)),this,SLOT(Update_trolley_number(QString)));//更改购物车内商品的数量
    connect(wtrolley,SIGNAL(Inquire_all_trolley()),this,SLOT(Inquire_all_trolley_goods()));//查询整个购物车

    //connect(myuser,SIGNAL(Send_to_wsk(QVector<QString>)),this,SLOT(Send_pack_show(QVector<QString>)));//抓包
    //connect(myuser,SIGNAL(Send_pack_show_des_usr(QVector<QString> need_data)),this,SLOT(send_pack_show_des(QVector<QString> need_data)));

    p->e = 65537;




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::socket_Read_Data()
{

    QByteArray buffer = socket->readAll();
    m_buffer.append(buffer);
    int total_length = m_buffer.size();
    while (total_length)
    {
        QDataStream recv_data(m_buffer);
        recv_data.setVersion(QDataStream::Qt_5_12);
        int m_len = sizeof(mes_len);
        if (total_length < m_len)
            break;//包长不够
        //mes_len *msg1=(mes_len*)m_buffer.left(m_len).data();
        unsigned int he_len;
        recv_data >> he_len;
        //qDebug() << m_buffer.left(m_len).length();
        int head_len = m_len + he_len + 4;
        if (total_length < head_len)
            break;

        QByteArray t1;
        recv_data >> t1;
        Mes* header = (Mes*)t1.data();
        //Mes *header=(Mes *)m_buffer.mid(m_len,head_len).data();

        int all_len = head_len + header->message_length + header->sign_length + 4;

        if (total_length < all_len)
            break;

        QByteArray t2;
        recv_data >> t2;
        //QByteArray t2=m_buffer.mid(head_len,all_len);
        Message res;
        res.message_length = header->message_length;
        res.message_type = header->message_type;
        res.field_type = header->field_type;
        for (int i = 0; i < 4; i++)
        {
            res.des_ip[i] = header->des_ip[i];
            res.des_src[i] = header->des_src[i];
            res.sign_length = header->sign_length;
        }

        res.data = t2.left(header->message_length);
        res.sign = t2.right(header->sign_length);
        //后面为解密并且还原，这里需要注意，需要设计如何选择解密方法以及密钥，还有，不要忘记解密完要调用remove和fromHex
        //后面为解密并且还原，这里需要注意，需要设计如何选择解密方法以及密钥，还有，不要忘记解密完要调用remove和fromHex

        recv_mes = res;

        buffer = m_buffer.right(total_length - all_len);
        total_length = buffer.size();
        m_buffer = buffer;

        message_handel();//处理报文

    }

}

void MainWindow::message_handel()//处理收到的报文，并执行相应的操作
{
    int message_type = int(recv_mes.message_type);//报文类型
    int field_type = int(recv_mes.field_type);//字段类型
    switch (message_type)
    {
    case 0://收到错误报文
    {
        switch (field_type)//检测报文类型
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
        switch (field_type)
        {
        case 0:
            break;
        case 1:
            break;
        }
        break;
    }
    case 2://收到管理员报文
    {
        switch (field_type)
        {
        case 0:
            break;
        case 1:
            break;
        }
        break;
    }
    case 3://收到服务器数据报文
    {
        switch (field_type)
        {
        case 0:
            break;
        case 1:
            break;
        case 2://处理商品查询反馈
            Show_goods();//显示商品信息
            break;
        case 4://处理商品查询反馈
            Show_goods_t();//显示商品信息
//            break;
        }
        break;
    }
    case 4://收到AS报文
    {
        AS_handel();
        break;
    }

    case 5://收到tgs发的报文
    {
        TGS_handel();
        break;
    }
    case 65:
    {
        switch (field_type)
        {
        case 0:
            break;
        case 8:
            myuser->modifyinfs->deal_aftermod(recv_mes);
            break;
        case 9:
            addcart_recive();
            break;
        case 12:
            myuser->VIP_handel(recv_mes);
            break;
        case 24:
             myuser->deal_initi(recv_mes);
             break;
        case 25:
             myuser->modifypsws->deal_aftermod(recv_mes);
             break;
        }
        break;
    }
    case 67://收到服务器控制报文
    {
        switch (field_type)
        {
        case 0:
            break;
        case 1:
            C_handel();
            break;
        case 3://商品页购买反馈处理
            Goods_order_handle();
            break;
        case 6://接收登录反馈
            Login_confirm();
            break;
        case 8://处理翻页结果
            Page_handel();
            break;
        case 9://处理购物车翻页
            Page_handel_t();
            break;
        case 10://更改购物车内商品数量、清理购物车的反馈
            Refresh_goods_t(6);
            break;
        case 11://购物车下单反馈
            Trolley_order_handle();
            break;
        case 12://查询整个购物车的反馈
            Trolley_all_handle();
            break;
        }
        break;
    }
    }
}

void MainWindow::Kerberos(int idc)
{
    /*连接AS，与AS发消息*/
    //****ctoas
    CtoAS ctoas;
    ctoas.IDc=idc;//sql查询得到
    ctoas.IDtgs=22;
    QTime time=QTime::currentTime();
    ctoas.TS1=time.toString("h:m:s.z");
    g_allshare.TS1=ctoas.TS1;
    g_allshare.IDc=ctoas.IDc;
    g_allshare.IDv=33;

    //****ctoas change to qbytearr
    QByteArray ctoasSend;//ctoasSend.append((char*)&ctoas, sizeof(CtoAS));
    QBuffer ctoasbuffer(&ctoasSend);
    ctoasbuffer.open(QIODevice::WriteOnly);
    QDataStream ctoasout(&ctoasbuffer);
    ctoasout<<ctoas.IDc<<ctoas.IDtgs<<ctoas.TS1;
    ctoasbuffer.close();

    socket=new QTcpSocket;
    socket->connectToHost(AS_IP,6666);
        if(socket->waitForConnected(3000))
        {
            QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
            QVector<QString> pac_show_vec=package_message_noencrypt(65,4,ctoasSend,socket);
            QString show_det="客户发送给AS的获取票据许可票据请求报文：  IDc:"+QString::number(idc)+"   IDtgs:"+QString::number(ctoas.IDtgs)+"   TS1:"+ctoas.TS1;
            pac_show_vec[7]=show_det;
            send_pack_show_des(pac_show_vec);
        }
        else
        {
            socket->disconnectFromHost();
            qDebug()<<"连接失败";
            socket->waitForDisconnected();
        }

}

void MainWindow::AS_handel()
{
    //获取到了message,解析message中的内容
     //AStoC* astoc=(AStoC*)recmes.data.data();
     Message pac_show_mes=recv_mes;
     QBuffer as_ticketbuf(&recv_mes.data);
     as_ticketbuf.open(QIODevice::ReadOnly);
     QDataStream as_ticketin(&as_ticketbuf);
     char *asencrypt;char* ticket_encrypt;
     as_ticketin>>asencrypt>>ticket_encrypt;

     //****查一下本人的密钥

     switch(g_allshare.IDc)
     {
      case 201900:
         g_allshare.mykey_n=Ckey1_n;
         g_allshare.mykey_d=Ckey1_d;
         break;
     case 201901:
         g_allshare.mykey_n=Ckey2_n;
         g_allshare.mykey_d=Ckey2_d;
        break;
     case 201902:
         g_allshare.mykey_n=Ckey3_n;
         g_allshare.mykey_d=Ckey3_d;
        break;
     }

     //****astoc 解密
     char* asdecrypt=decrypt(asencrypt,g_allshare.mykey_n,g_allshare.mykey_d);
     QByteArray asdecrypt_data(asdecrypt);
     asdecrypt_data.remove(0,1);
     asdecrypt_data=QByteArray::fromHex(asdecrypt_data);

     //****qbytearr change astoc
     QBuffer asbuf(&asdecrypt_data);
     asbuf.open(QIODevice::ReadOnly);
     QDataStream asin(&asbuf);
     AStoC astocnew;
     asin>>astocnew.Key_c_tgs>>astocnew.IDtgs>>astocnew.TS2>>astocnew.lifetime;

     QString show_data=QString("AS发送给C的报文：\n Ktgs为：%1 \n IDtgs为：%2 TS2为：%3 Lifetime2为：%4 \n Tickect_tgs为：%5").arg(QString(astocnew.Key_c_tgs.toHex())).arg(astocnew.IDtgs)
                  .arg(astocnew.TS2).arg(astocnew.lifetime).arg(QString(QByteArray(ticket_encrypt).toHex()));
     recv_pack_show(pac_show_mes,show_data,"0",QByteArray(g_allshare.mykey_d).toHex());

     g_allshare.Key_c_tgs=astocnew.Key_c_tgs;
     socket->disconnectFromHost();
     socket->waitForDisconnected();

     /*连接TGS，与TGS发消息*/
     //**** Authenticator初始化和加密
     Authenticator auth;
     auth.IDc=g_allshare.IDc;
     QTime t3time=QTime::currentTime();
     auth.TS=t3time.toString("h:m:s.z");
     QByteArray authbyte;
     QBuffer authbuffer(&authbyte);
     authbuffer.open(QIODevice::WriteOnly);
     QDataStream authout(&authbuffer);
     authout<<auth.IDc<<auth.TS;
     authbuffer.close();
     QByteArray authbyte_en=DES_en(g_allshare.Key_c_tgs,authbyte);
     QString show_det= QString("C发送给TGS的发送服务许可票据请求报文：IDv为%1 \n Ticket_tgs为：%2 \n Authenticator为 %3 \n 具体有IDc:%4  TS3:%5").arg(g_allshare.IDv)
                   .arg(QString(QByteArray(ticket_encrypt).toHex())).arg(QString(QByteArray(authbyte_en).toHex())).arg(auth.IDc).arg(auth.TS);
     //**** 包装报文
     QByteArray ctotgsSend;
     QBuffer ctotgsbuffer(&ctotgsSend);
     ctotgsbuffer.open(QIODevice::WriteOnly);
     QDataStream ctotgsout(&ctotgsbuffer);
     ctotgsout<<g_allshare.IDv<<ticket_encrypt<<authbyte_en;
     ctotgsbuffer.close();

     socket=new QTcpSocket;
         socket->connectToHost(TGS_IP,6667);
         if(socket->waitForConnected(3000))
         {
             QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
             QVector<QString> pac_show_vec=package_message_noencrypt(65,5,ctotgsSend,socket);
             //QString show_det="请求商品图片";
             pac_show_vec[7]=show_det;
             send_pack_show_des(pac_show_vec);
         }
         else
         {
             socket->disconnectFromHost();
             qDebug()<<"连接失败";
             socket->waitForDisconnected();
         }

}

void MainWindow::TGS_handel()
{
    //获取到了message,解析message中的内容
     //des解密
     Message pac_show_mes=recv_mes;
     QByteArray rectgs=DES_de(g_allshare.Key_c_tgs,recv_mes.data);
     QBuffer tgstocbuf(&rectgs);
     tgstocbuf.open(QIODevice::ReadOnly);
     QDataStream tgstocin(&tgstocbuf);
     TGStoC tgstoc;
     tgstocin>>tgstoc.IDv>>tgstoc.Kcv>>tgstoc.TS4>>tgstoc.m_ticket_v;

     g_allshare.Key_c_v=tgstoc.Kcv;

     socket->disconnectFromHost();
     socket->waitForDisconnected();

     QString show_data=QString("TGS发送给C的报文：IDv为： %1 \nKcv为： %2 \nTS4为： %3\nTicket_v为：%4").arg(tgstoc.IDv)
             .arg(QString(tgstoc.Kcv.toHex())).arg(tgstoc.TS4).arg(QString(QByteArray(tgstoc.m_ticket_v).toHex()));
     recv_pack_show(pac_show_mes,show_data,"0",g_allshare.Key_c_tgs);

     /*连接V，与V发消息*/
     //**** Authenticator初始化和加密
     Authenticator auth;
     auth.IDc=g_allshare.IDc;
     QTime t5time=QTime::currentTime();
     auth.TS=t5time.toString("h:m:s.z");
     QByteArray authbyte;
     QBuffer authbuffer(&authbyte);
     authbuffer.open(QIODevice::WriteOnly);
     QDataStream authout(&authbuffer);
     authout<<auth.IDc<<auth.TS;
     authbuffer.close();
     QByteArray authbyte_en=DES_en(g_allshare.Key_c_v,authbyte);

     QString show_det=QString("C发送给V的报文： Ticket_v为： %1\nAuthenticator为: %2 具体有IDc:%3  TS5:%4").arg(QString(QByteArray(tgstoc.m_ticket_v).toHex()))
             .arg(QString(authbyte_en.toHex())).arg(auth.IDc).arg(auth.TS);
     //**** 包装报文
     QByteArray ctotgsSend;
     QBuffer ctotgsbuffer(&ctotgsSend);
     ctotgsbuffer.open(QIODevice::WriteOnly);
     QDataStream ctotgsout(&ctotgsbuffer);
     ctotgsout<<tgstoc.m_ticket_v<<authbyte_en;
     ctotgsbuffer.close();

     socket=new QTcpSocket;
         socket->connectToHost(Server_IP,6668);
         if(socket->waitForConnected(3000))
         {
             QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
             QVector<QString> pac_show_vec=package_message_noencrypt(65,6,ctotgsSend,socket);
             //QString show_det="请求商品图片";
             pac_show_vec[7]=show_det;
             send_pack_show_des(pac_show_vec);
         }
         else
         {
             socket->disconnectFromHost();
             qDebug()<<"连接失败";
             socket->waitForDisconnected();
         }
      g_allshare.socket=socket;

}

void MainWindow::C_handel()
{
    //获取到了message,解析message中的内容
     //des解密
    Message pac_show_mes=recv_mes;
     QByteArray recv=DES_de(g_allshare.Key_c_v,recv_mes.data);
     QBuffer vtocbuf(&recv);
     vtocbuf.open(QIODevice::ReadOnly);
     QDataStream vtocin(&vtocbuf);
     VtoC vtoc;
     vtocin>>vtoc.TS5;
     p->d=g_allshare.mykey_d;
     p->n=g_allshare.mykey_n;
     DES_key=g_allshare.Key_c_v;
     QString sql = "SELECT * FROM USER WHERE (uid=" + id + " AND upass=\"" + pwd + "\")";
     QByteArray qbt_sql = sql.toUtf8();
     //qbt_sql = QByteArray::fromHex(qbt_sql);

     QString show_data=QString("V发送给C的报文： TS5+1为：%1").arg(vtoc.TS5);
     recv_pack_show(pac_show_mes,show_data,"0",g_allshare.Key_c_v);

     //QMessageBox::information(NULL, "", "Kerberos认证成功");
     noticebox("Kerberos认证成功");

     QByteArray data;
     QBuffer buffer(&data);
     buffer.open(QIODevice::WriteOnly);
     QDataStream out(&buffer);
     out << qbt_sql;
     buffer.close();
     QVector<QString> pac_show_vec = package_message_DES(65, 14, data, socket, DES_key, p->n, p->d);//数字签名DES加密
     QString show_det="登录请求 SELECT * FROM user WHERE (uid=" + id + " AND upass=\"" + pwd + "\")";
     pac_show_vec[7]=show_det;
     send_pack_show_des(pac_show_vec);




     //socket->disconnectFromHost();
     //socket->waitForDisconnected();
}

void MainWindow::on_pushButton_clicked()//创建新的商品页
{
    QPixmap pixmap=QPixmap(":/picture/good_change.png");
        pixmap=pixmap.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->pushButton->setIconSize(QSize(40,40));
        ui->pushButton->setIcon(pixmap);

   pixmap=QPixmap(":/picture/cart.png");
        pixmap=pixmap.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->pushButton_2->setIconSize(QSize(40,40));
        ui->pushButton_2->setIcon(pixmap);

   pixmap=QPixmap(":/picture/user_info.png");
         pixmap=pixmap.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
         ui->pushButton_3->setIconSize(QSize(40,40));
         ui->pushButton_3->setIcon(pixmap);

    if(!wd)adbuf=wgood->add_buffer;
    if(adbuf.empty())adbuf.push_back(tmp_buffer);
    wgood=new WGood(id,this,adbuf);
    ui->scrollArea->setWidget(wgood);
    wd=0;

    connect(wgood,SIGNAL(Refresh_goods_list(int)),this,SLOT(Refresh_goods(int)));//刷新商品页
    connect(wgood,SIGNAL(Get_goods_number(int)),this,SLOT(To_page(int)));//跳转页面
    connect(wgood,SIGNAL(Search_goods_list(QString)),this,SLOT(Search_goods(QString)));//搜索查询
    connect(wgood,SIGNAL(Search_species_list(int)),this,SLOT(Search_species(int)));//种类查询
    connect(wgood,SIGNAL(Change_view_mode(int)),this,SLOT(Change_goods_view_mode(int)));//更改显示模式
    connect(wgood,SIGNAL(addcart(int,QVector<QVector<QString>>)),this,SLOT(Addcart_send(int,QVector<QVector<QString>>)));//加购物车

    g_loc[goods_view_mode-1]-=6;
    if(goods_view_mode==1)//默认
        Get_goods(g_loc[goods_view_mode-1],g_loc[goods_view_mode-1]+6);
    else if(goods_view_mode==2)//搜索框搜索
    {
        Search_goods(search_sql,1);
    }
    else//种类区分
    {
        Search_species(species_n,1);
    }
}

void MainWindow::on_pushButton_2_clicked()//创建新的购物车页
{
    QPixmap pixmap=QPixmap(":/picture/good.png");
        pixmap=pixmap.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->pushButton->setIconSize(QSize(40,40));
        ui->pushButton->setIcon(pixmap);

   pixmap=QPixmap(":/picture/cart_change.png");
        pixmap=pixmap.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->pushButton_2->setIconSize(QSize(40,40));
        ui->pushButton_2->setIcon(pixmap);

   pixmap=QPixmap(":/picture/user_info.png");
         pixmap=pixmap.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
         ui->pushButton_3->setIconSize(QSize(40,40));
         ui->pushButton_3->setIcon(pixmap);


    if(!wd)adbuf=wgood->add_buffer;
    wtrolley = new WTrolley(id,this);
    ui->scrollArea->setWidget(wtrolley);
    wd=1;

    connect(wtrolley,SIGNAL(Refresh_goods_list_t(int)),this,SLOT(Refresh_goods_t(int)));//刷新购物车页
    connect(wtrolley,SIGNAL(Change_view_mode_t(int)),this,SLOT(Change_goods_view_mode_t(int)));//更购物车页改查看模式
    connect(wtrolley,SIGNAL(Get_goods_number_t(int)),this,SLOT(To_page_t(int)));//购物车跳转页面
    connect(wtrolley,SIGNAL(Search_goods_list_t(QString)),this,SLOT(Search_goods_t(QString)));//购物车搜索查询
    connect(wtrolley,SIGNAL(Search_species_list_t(int)),this,SLOT(Search_species_t(int)));//购物车种类查询
    connect(wtrolley,SIGNAL(Update_trolley_num(QString)),this,SLOT(Update_trolley_number(QString)));//更改购物车内商品的数量
    connect(wtrolley,SIGNAL(Inquire_all_trolley()),this,SLOT(Inquire_all_trolley_goods()));//查询整个购物车

    t_loc[trolley_view_mode-1]-=6;
    if(trolley_view_mode==1)//默认
        Get_goods_t(t_loc[trolley_view_mode-1],t_loc[trolley_view_mode-1]+6);
    else if(trolley_view_mode==2)//搜索框搜索
    {
        Search_goods_t(search_sqll,1);
    }
    else//种类区分
    {
        Search_species_t(species_nn,1);
    }
}

void MainWindow::on_pushButton_3_clicked()//创建新的用户主页
{
    QPixmap pixmap=QPixmap(":/picture/good.png");
        pixmap=pixmap.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->pushButton->setIconSize(QSize(40,40));
        ui->pushButton->setIcon(pixmap);

   pixmap=QPixmap(":/picture/cart.png");
        pixmap=pixmap.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->pushButton_2->setIconSize(QSize(40,40));
        ui->pushButton_2->setIcon(pixmap);

   pixmap=QPixmap(":/picture/user_info_change.png");
         pixmap=pixmap.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
         ui->pushButton_3->setIconSize(QSize(40,40));
         ui->pushButton_3->setIcon(pixmap);


    if(!wd)adbuf=wgood->add_buffer;
    myuser=new WUser(g_allshare,this,this);
    //myuser->show();
    connect(myuser,SIGNAL(Send_to_wsk_recv(Message,QString,QByteArray,QByteArray )),this,SLOT(Recv_pack_show(Message,QString,QByteArray,QByteArray)));
    ui->scrollArea->setWidget(myuser);
    wd=2;
}

void MainWindow::on_pushButton_4_clicked()//抓包
{
    capture_window->show();
}

void MainWindow::Login_check(QString uid, QString upass)//登录请求
{
    id = uid;
    pwd = upass;

    Kerberos(id.toInt());
    }

void MainWindow::Login_confirm()//登录确认【收】
{
    //收包处理

    Message pac_show_mes=recv_mes;
    recv_mes.data=DES_de(DES_key,recv_mes.data);
    //res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key,recv_mes.sign);
    //不要数字签名的话这一行去掉，DES的解密，res为message类型，res.data为message的data部分
    sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,p->e);
    //不要数字签名的话这一行去掉
    bool login;
    QByteArray img;
    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadOnly);
    QDataStream in(&buf);
    in>>login>>img;
    buf.close();
    QString login_state;
    if(login) login_state="登录反馈：登录成功";
    else login_state="登陆反馈：登录失败";
    QString show_data=login_state;
    recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),DES_key);

    if(login)
    {
        //qDebug()<<"login ok";
        ui->label_2->setText("");

        QImage head_img=getImage(img);
        QPixmap pix_img=QPixmap::fromImage(head_img);

        QPixmap fitpixmap_userIcon=pix_img.scaled(200, 200, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        fitpixmap_userIcon = PixmapToRound(fitpixmap_userIcon,100);
        ui->label_head->setPixmap(fitpixmap_userIcon);

        //ui->label_head->setPixmap(QPixmap::fromImage(head_img));
        ui->label_head->setScaledContents(true);//铺满label
        this->show();
        wlogin->hide();
        Get_goods(g_loc[goods_view_mode-1],g_loc[goods_view_mode-1]+6);
        wgood->id=id;
        //Notice_box(this,"登录成功");
    }
    else
    {
        qDebug()<<"login failed";
    }

}

void MainWindow::Change_head(QImage img)//更改首页头像
{
    QPixmap pix_img=QPixmap::fromImage(img);
    QPixmap fitpixmap_userIcon=pix_img.scaled(200, 200, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    fitpixmap_userIcon = PixmapToRound(fitpixmap_userIcon,100);
    ui->label_head->setScaledContents(true);//铺满label
    ui->label_head->setPixmap(fitpixmap_userIcon);
}

void MainWindow::send_pack_show_des(QVector<QString> need_data)
{
    QStandardItem *item;
    for(int j=0;j<6;j++)
    {
        item=new QStandardItem(need_data[j]);
        capture_window->model->setItem(capture_window->now_row,j,item);
    }
    QVector<QString> detail_data;
    detail_data.push_back(need_data[6]);
    detail_data.push_back(need_data[7]);
    detail_data.push_back(need_data[8]);
    capture_window->detailed_info.push_back(detail_data);
    capture_window->now_row++;
    capture_window->update_tableview();
}

void MainWindow::recv_pack_show(Message mes,QString data,QByteArray sign,QByteArray key)
{
    QVector<QString> recv_show_pac;
    QString dst_ip="";
    QString dst_src="";
    for(int i=0;i<4;i++)
    {
        //qDebug()<<(unsigned int)mes.des_ip[i];
        dst_ip+=QString::number((unsigned int)mes.des_ip[i]);
        dst_src+=QString::number((unsigned int)mes.des_src[i]);
        if(i!=3)
        {
            dst_ip+=".";
            dst_src+=".";
        }
    }
    recv_show_pac.push_back(dst_ip);
    recv_show_pac.push_back(dst_src);
    recv_show_pac.push_back(QString::number(int(recv_mes.message_type)));
    recv_show_pac.push_back(QString::number(int(recv_mes.field_type)));

    if(sign!="0")
         recv_show_pac.push_back(sign.toHex());
    else
        recv_show_pac.push_back(sign);
    recv_show_pac.push_back("nothing");
    recv_show_pac.push_back(mes.data.toHex());
    recv_show_pac.push_back(data);

    if(key!="0")
         recv_show_pac.push_back(key.toHex());
    else
        recv_show_pac.push_back(key);


    send_pack_show_des(recv_show_pac);
}

void MainWindow::Send_pack_show(QVector<QString> data)
{
    send_pack_show_des(data);
}

void MainWindow::Recv_pack_show(Message mes,QString data,QByteArray sign,QByteArray key)
{
    recv_pack_show(mes,data,sign,key);
}

//******************************商品页******************************//

void MainWindow::Get_goods(int start,int end)//请求商品表
{
    QString sql = "SELECT * FROM GOOD LIMIT "+QString::number(start)+","+QString::number(end)+";";
    QByteArray qbt_sql = sql.toUtf8();
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << qbt_sql;
    buffer.close();
    QVector<QString> pac_show_vec = package_message_DES(65, 15, data, socket, DES_key, p->n, p->d);//数字签名DES加密
    QString show_det="请求商品表\n"+sql;
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
    g_loc[goods_view_mode-1]+=6;
}

void MainWindow::Show_goods()//***显示商品信息***【收】
{
    //收包处理
    Message pac_show_mes=recv_mes;
    recv_mes.data=DES_de(DES_key,recv_mes.data);
    //res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key,recv_mes.sign);
    //不要数字签名的话这一行去掉，DES的解密，res为message类型，res.data为message的data部分
    sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,p->e);
    //不要数字签名的话这一行去掉

    QVector<QVector<QString>> goods_infm;
    QVector<QByteArray> goods_img;
    int goods_num;
    QByteArray recv_img;
    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadWrite);
    QDataStream in(&buf);
    buf.seek(1);
    in>>goods_num>>goods_infm>>goods_img;//本组商品数量、商品信息、商品图片
    buf.close();

    QString data_list="获取所请求的一页商品信息\n本页商品数量："+QString::number(goods_num)+"\n";
    for(int i=0;i<goods_infm.count();i++)
    {
        data_list=data_list+"名称："+goods_infm[i][0]+" 库存："+goods_infm[i][1]+" 原价："+goods_infm[i][2]
                +" 现价："+goods_infm[i][3]+" 类型："+spc[goods_infm[i][4].toInt()-1]+"\n";
    }

    QString show_data=data_list;
    recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),DES_key);
    int page_num=g_loc[goods_view_mode-1];
    emit Transport_goods(g_loc[goods_view_mode-1],goods_num,goods_infm,goods_img);
}

void MainWindow::Refresh_goods(int offset)//页面刷新/转换
{
    qDebug()<<"Refresh";
    g_loc[goods_view_mode-1]-=offset;
    if(goods_view_mode==1)//默认
    {
        Get_goods(g_loc[goods_view_mode-1],g_loc[goods_view_mode-1]+6);
    }
    else if(goods_view_mode==2)//搜索框搜索
    {
        Search_goods(search_sql,1);
    }
    else//种类区分
    {
        Search_species(species_n,1);
    }
}

void MainWindow::To_page(int n)//跳转到指定页
{
    if(!(n>0))
    {
        //QMessageBox::information(this,"notice"," 输入页码需要为正整数");
        noticebox("页码需要为正整数");
        return;
    }
    g_tar_loc[goods_view_mode-1]=6*n;
    QVector<QString> sql;
    if(goods_view_mode==1)//默认
    {
        sql.push_back("SELECT COUNT(*) FROM GOOD");
        sql.push_back("SELECT * FROM GOOD LIMIT "+QString::number(6*(n-1))+","+QString::number(6*n));
    }
    else if(goods_view_mode==2)//搜索框搜索
    {
        sql.push_back("SELECT COUNT(*) FROM Good WHERE Gname LIKE \"%"+search_sql+"%\"");
        sql.push_back("SELECT * FROM Good WHERE Gname LIKE \"%"+search_sql+"%\""
                      +" LIMIT "+QString::number(6*(n-1))+","+QString::number(6*n));
    }
    else//种类区分
    {
        sql.push_back("SELECT COUNT(*) FROM GOOD WHERE GTYPE="+QString::number(goods_view_mode-2));
        sql.push_back("SELECT * FROM GOOD WHERE GTYPE="+QString::number(goods_view_mode-2)
                      +" LIMIT "+QString::number(6*(n-1))+","+QString::number(6*n));
    }

    //QByteArray qbt_sql = sql.toUtf8();
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out<<sql<<n;
    buffer.close();
    QVector<QString> pac_show_vec = package_message_DES(65, 26, data, socket, DES_key, p->n, p->d);//数字签名DES加密
    QString show_det="发起页面跳转请求：转到第"+QString::number(n)+"页\n"+sql[0]+"\n"+sql[1];
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
}

void MainWindow::Page_handel()//页面跳转处理【收】
{
    //收包处理
    Message pac_show_mes=recv_mes;
    recv_mes.data=DES_de(DES_key,recv_mes.data);
    //res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key,recv_mes.sign);
    //不要数字签名的话这一行去掉，DES的解密，res为message类型，res.data为message的data部分
    sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,p->e);
    //不要数字签名的话这一行去掉

    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadWrite);
    QDataStream in(&buf);
    //buf.seek(1);
    int iqur,page_ok;
    in>>iqur>>page_ok;
    if(iqur)//商品数量查询成功
    {
        if(page_ok)//目标页码可跳转
        {
            g_loc[goods_view_mode-1]=g_tar_loc[goods_view_mode-1];
            g_tar_loc[goods_view_mode-1]=-1;
            int goods_num;
            QVector<QVector<QString>> goods_infm;
            QVector<QByteArray> goods_img;
            in>>goods_num>>goods_infm>>goods_img;
            //改为goods_loc
//            emit Transport_goods(goods_locate,goods_num,goods_infm,goods_img);//发送信号使商品页进行显示
            emit Transport_goods(g_loc[goods_view_mode-1],goods_num,goods_infm,goods_img);

        }
        else//目标页码不可跳转
        {
            int max_page;
            in>>max_page;
            QString m_page="最大页码： "+QString::number(max_page);
            //QMessageBox::information(this,"notice",m_page);
            noticebox(m_page);
        }
    }
    else//商品数量查询失败
    {
       qDebug()<<"inquire goods_num failed";
    }
    buf.close();
    QString show_data;
    if(page_ok) show_data="页面跳转反馈：成功";
    else show_data="页面跳转反馈：失败";
    recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),DES_key);
}

void MainWindow::Search_goods(QString sql,int mode)//查询指定商品
{
    if(!mode)
        g_loc[goods_view_mode-1]=0;
    search_sql=sql;
    sql="SELECT * FROM Good WHERE Gname LIKE \"%"+sql+"%\""
            +" LIMIT "+QString::number(g_loc[goods_view_mode-1])+","+QString::number(g_loc[goods_view_mode-1]+6);
    QByteArray qbt_sql = sql.toUtf8();
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << qbt_sql;
    buffer.close();
    QVector<QString> pac_show_vec = package_message_DES(65, 19, data, socket, DES_key, p->n, p->d);//数字签名DES加密
    QString show_det="查询名称中包含“"+search_sql+"”的商品\n"+sql;
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
    g_loc[goods_view_mode-1]+=6;
}

void MainWindow::Search_species(int n,int mode)//查询不同种商品
{
     if(!mode)
         g_loc[goods_view_mode-1]=0;
     species_n=n;
     QString species_n=QString::number(n);
     QString sql="SELECT * FROM Good WHERE Gtype="+species_n
                +" LIMIT "+QString::number(g_loc[goods_view_mode-1])+","+QString::number(g_loc[goods_view_mode-1]+6);
     QByteArray qbt_sql = sql.toUtf8();
     QByteArray data;
     QBuffer buffer(&data);
     buffer.open(QIODevice::WriteOnly);
     QDataStream out(&buffer);
     out << qbt_sql;
     buffer.close();
     QVector<QString> pac_show_vec = package_message_DES(65, 20, data, socket, DES_key, p->n, p->d);//数字签名DES加密
     QString show_det="查询["+spc[n-1]+"]类商品\n"+sql;
     pac_show_vec[7]=show_det;
     send_pack_show_des(pac_show_vec);
     g_loc[goods_view_mode-1]+=6;
}

void MainWindow::Change_goods_view_mode(int n)//更改显示模式
{
    goods_view_mode=n;
}

void MainWindow::Addcart_send(int idc,QVector<QVector<QString>>send)//加入购物车
{
    QByteArray infSend;
    QBuffer infbuffer(&infSend);
    infbuffer.open(QIODevice::WriteOnly);
    QDataStream infout(&infbuffer);
    infout<<idc<<send;
    infbuffer.close();
    QVector<QString> pac_show_vec=package_message_DES(65,9,infSend,socket,DES_key,p->n,p->d);
    QString show_det="将以下商品加入购物车：\n";
    for(int i=0;i<send.count();i++)
    {
        show_det=show_det+send[i][1]+" 件【"+send[i][0]+"】;\n";
    }
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
}

void MainWindow::addcart_recive()//加购物车反馈【收】
{
    //****des解密
    Message pac_show_mes=recv_mes;
    recv_mes.data=DES_de(DES_key,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,p->e);      //不要数字签名的话这一行去掉

    //****解析报文
    bool success;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>success;
    recbuffer.close();
    QString show_data;
    if(success) show_data="加购物车反馈：成功";
    else show_data="加购物车反馈：失败";
    recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),DES_key);
    if(success)
    {
        //QMessageBox::information(this,"notice","success");
        noticebox("成功加入购物车");
        wgood->add_buffer.clear();
        Refresh_goods(6);
        int a=0;
    }
    else //QMessageBox::information(this,"notice","failed");
        noticebox("加购出错");

    //add_buffer.clear();

    //Refresh_goods(12);
}


//*****//*****//*****//*****//*****ADD*****//*****//*****//*****////////////////////////////////////////////////

void MainWindow::Goods_order_pay(QByteArray data)//发送直接购买的商品信息
{
    QVector<QString> pac_show_vec = package_message_DES(65,10,data,socket,DES_key,p->n,p->d);

    QByteArray data_show=data;
    QBuffer buffer(&data_show);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);
    QVector<QVector<QString>> under_order;
    QVector<QString> select_sql,update_sql,record_sql;
    QString delete_sql;
    QString show_det="在商品页下单购买商品：\n";
    in>>under_order>>select_sql>>update_sql>>record_sql>>delete_sql;
    for(int i=0;i<under_order.count();i++)
    {
        show_det=show_det+"购买"+QString(under_order[i][1])+"件【"+under_order[i][0]+"】\n";
    }
    for(int i=0;i<select_sql.count();i++)
    {
        show_det=show_det+select_sql[i]+"\n"+update_sql[i]+"\n"+record_sql[i]+"\n";
    }
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
}

void MainWindow::Goods_order_handle()//商品页购买商品反馈处理【收】
{
    Message pac_show_mes=recv_mes;
    bool sucess;
       recv_mes.data=DES_de(DES_key,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
       recv_mes.sign=DES_de(DES_key,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
       bool judge=sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,p->e);                  //不要数字签名的话这一行去掉
       QBuffer buffer(&recv_mes.data);
       buffer.open(QIODevice::ReadOnly);
       QDataStream in(&buffer);
       in>>sucess;


       if(sucess==false)
       {
           wgood->add_buffer.clear();
           in>>wgood->add_buffer;
           //QMessageBox::question(this,"错误","下单失败，可能商品已经被其他人买走，已经帮你更新最新的订单",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
           noticebox("商品已售罄");
       }
       else
       {
           //QMessageBox::question(this,"成功","下单成功",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
           noticebox("购买成功");
           wgood->add_buffer.clear();
           //adbuf=wgood->add_buffer;
       }
       Refresh_goods(6);

       QString show_data;
       if(sucess) show_data="商品页购买商品反馈：成功";
       else show_data="商品页购买商品反馈：失败";
       recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),DES_key);

       buffer.close();
}

void MainWindow::Trolley_order_pay(QByteArray send)//购物车下单
{
    QVector<QString> pac_show_vec = package_message_DES(65,11,send,socket,DES_key,p->n,p->d);//数字签名DES加密
    QString show_det="刷新购物车信息";
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
}

void MainWindow::Trolley_order_handle()//购物车下单反馈【收】
{
    Message pac_show_mes=recv_mes;
    bool sucess;
    recv_mes.data=DES_de(DES_key,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(DES_key,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,p->e);                  //不要数字签名的话这一行去掉
    QBuffer buffer(&recv_mes.data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);
    in>>sucess;
    QString show_data;
    if(sucess) show_data="购物车下单反馈：成功";
    else show_data="购物车下单反馈：失败";
    recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),DES_key);

    if(sucess==false)//如果下单失败，提醒用户，并且更新缓存，购物车这里不用更新缓存，应该是更新购物车表，并且根据购物车表更新缓存
    {
        //under_order.clear();
        //in>>under_order;
        //QMessageBox::question(this,"错误","下单失败，可能商品已经被其他人买走，已经帮你更新最新的订单",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
        noticebox("商品已售罄");
    }
    else//提醒用户成功下单
    {
        //QMessageBox::question(this,"成功","下单成功",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
        noticebox("购买成功");
    }
    Refresh_goods_t(6);
    buffer.close();
}

void MainWindow::Inquire_all_trolley_goods()//查询整个购物车表
{
    QVector<QString> sql;
    QString sql_1="SELECT COUNT(*) FROM  cart WHERE uid="+id;
    QString sql_2="SELECT * FROM  cart WHERE uid="+id;
    sql.push_back(sql_1);
    sql.push_back(sql_2);
    //QByteArray qbt_sql = sql.toUtf8();
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << sql;
    buffer.close();
    QVector<QString> pac_show_vec = package_message_DES(65, 28, data, socket, DES_key, p->n, p->d);//数字签名DES加密
    QString show_det="请求ID为"+id+"的用户的完整购物车信息\n"+sql[0]+"\n"+sql[1];
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
}

void MainWindow::Trolley_all_handle()//查询整个购物车的反馈【收】
{
    //收包处理
    Message pac_show_mes=recv_mes;
    recv_mes.data=DES_de(DES_key,recv_mes.data);
    recv_mes.sign=DES_de(DES_key,recv_mes.sign);
    sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,p->e);

    QVector<QVector<QString>> trolley_infm;
    int trolley_num;
    QByteArray recv_img;
    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadWrite);
    QDataStream in(&buf);
    buf.seek(1);
    in>>trolley_num>>trolley_infm;//本组商品数量、商品信息、商品图片
    buf.close();
    QString show_data="查询当前用户的完整购物车信息：\n当前购物车内商品数量："+QString::number(trolley_num)+"\n";
    for(int i=0;i<trolley_infm.count();i++)
    {
        show_data=show_data+"名称："+trolley_infm[i][0]+"数量："+trolley_infm[i][1]
                +"原价："+trolley_infm[i][2]+"现价："+trolley_infm[i][3]+"\n";
    }
    recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),DES_key);
    emit Send_trolley_buffer(trolley_infm);
}

//*****//*****//*****//*****//*****DDA*****//*****//*****//*****///////////////////////////////////////////////


//******************************购物车页******************************//

void MainWindow::Get_goods_t(int start,int end)//请求购物车表
{
    QString sql = "SELECT cart.Gname,good.Gnum,cart.Gprice,cart.Gdiscount,cart.Snum,good.Gtype,good.Gimage FROM cart,good WHERE cart.Gname=good.Gname AND cart.uid="+id+" LIMIT "+QString::number(start)+","+QString::number(end)+";";
    QByteArray qbt_sql = sql.toUtf8();
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << qbt_sql;
    buffer.close();
    QVector<QString> pac_show_vec = package_message_DES(65, 16, data, socket, DES_key, p->n, p->d);//数字签名DES加密
    QString show_det="请求ID为"+id+"的用户的购物车内第"+QString::number(start+1)+"个到第"+QString::number(start+6)+"个商品的信息\n"+sql;
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
    t_loc[trolley_view_mode-1]+=6;
}

void MainWindow::Show_goods_t()//***显示购物车信息***【收】
{
    //收包处理
    Message pac_show_mes=recv_mes;
    recv_mes.data=DES_de(DES_key,recv_mes.data);
    recv_mes.sign=DES_de(DES_key,recv_mes.sign);
    sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,p->e);

    QVector<QVector<QString>> trolley_infm;
    QVector<QByteArray> trolley_img;
    int trolley_num;
    QByteArray recv_img;
    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadWrite);
    QDataStream in(&buf);
    buf.seek(1);
    in>>trolley_num>>trolley_infm>>trolley_img;//本组商品数量、商品信息、商品图片
    int page_num=t_loc[trolley_view_mode-1];
    buf.close();

    QString data_list="获取所请求的一页商品信息\n本页商品数量："+QString::number(trolley_num)+"\n";
    for(int i=0;i<trolley_infm.count();i++)//名、库、原、现、数、类
    {
        data_list=data_list+"名称："+trolley_infm[i][0]+" 库存："+trolley_infm[i][1]+" 原价："+trolley_infm[i][2]
                +" 现价："+trolley_infm[i][3]+" 数量："+trolley_infm[i][4]+" 类型："+spc[trolley_infm[i][5].toInt()-1]+"\n";
    }
    QString show_data=data_list;
    recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),DES_key);
    emit Transport_trolley(t_loc[trolley_view_mode-1],trolley_num,trolley_infm,trolley_img);
}

void MainWindow::Refresh_goods_t(int offset)//刷新购物车表
{
    qDebug()<<"Refresh";
    t_loc[trolley_view_mode-1]-=offset;
    if(trolley_view_mode==1)//默认
    {
        Get_goods_t(t_loc[trolley_view_mode-1],t_loc[trolley_view_mode-1]+6);
    }
    else if(trolley_view_mode==2)//搜索框搜索
    {
        Search_goods_t(search_sqll,1);
    }
    else//种类区分
    {
        Search_species_t(species_nn,1);
    }
}

void MainWindow::To_page_t(int n)//购物车页面跳转
{
    if(!(n>0))
    {
        //QMessageBox::information(this,"notice"," 输入页码需要为正整数");
        noticebox("页码需要为正整数");
        return;
    }
    t_tar_loc[trolley_view_mode-1]=6*n;
    QVector<QString> sql;
    if(trolley_view_mode==1)//默认
    {
        sql.push_back("SELECT count(*) FROM good WHERE good.Gname IN ( SELECT good.Gname FROM cart,good WHERE cart.Gname = good.Gname AND cart.uid="+id+");");
        sql.push_back("SELECT * FROM (SELECT cart.Gname,good.Gnum,cart.Gprice,cart.Gdiscount,cart.Snum,good.Gtype,good.Gimage FROM cart,good WHERE cart.Gname = good.Gname AND cart.uid="+id+") AS A LIMIT "+QString::number(6*(n-1))+","+QString::number(6*n));
    }
    else if(trolley_view_mode==2)//搜索框搜索
    {
        sql.push_back("SELECT COUNT(*) FROM Cart WHERE Gname LIKE \'%"+search_sqll+"%\'");
        sql.push_back("SELECT * FROM (SELECT cart.Gname,good.Gnum,cart.Gprice,cart.Gdiscount,cart.Snum,good.Gtype,good.Gimage "
                      "FROM cart,good WHERE cart.Gname = good.Gname AND cart.uid="+id+" AND good.Gname LIKE \'%"+search_sqll+"%\') AS A"
                      +" LIMIT "+QString::number(6*(n-1))+",6");
    }
    else//种类区分
    {
        sql.push_back("SELECT COUNT(*) FROM (SELECT cart.Gname,good.Gnum,cart.Gprice,cart.Gdiscount,cart.Snum,good.Gtype,good.Gimage "
                      "FROM cart,good WHERE cart.Gname=good.Gname AND cart.uid="+id+" AND Gtype="+QString::number(trolley_view_mode-2)+") AS A");
        sql.push_back("SELECT * FROM (SELECT cart.Gname,good.Gnum,cart.Gprice,cart.Gdiscount,cart.Snum,good.Gtype,good.Gimage "
                      "FROM cart,good WHERE cart.Gname = good.Gname AND cart.uid="+id+" AND good.Gtype="+QString::number(trolley_view_mode-2)
                      +") AS A LIMIT "+QString::number(6*(n-1))+",6");
    }

    //QByteArray qbt_sql = sql.toUtf8();
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out<<sql<<n;
    buffer.close();
    QVector<QString> pac_show_vec = package_message_DES(65, 27, data, socket, DES_key, p->n, p->d);//数字签名DES加密
    QString show_det="请求ID为"+id+"的用户的购物车的第"+QString::number(n)+"页\n"+sql[0]+"\n"+sql[1];
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
}

void MainWindow::Page_handel_t()//购物车处理翻页
{
    //收包处理
    Message pac_show_mes=recv_mes;
    recv_mes.data=DES_de(DES_key,recv_mes.data);
    recv_mes.sign=DES_de(DES_key,recv_mes.sign);
    sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,p->e);

    QBuffer buf(&recv_mes.data);
    buf.open(QIODevice::ReadWrite);
    QDataStream in(&buf);
    //buf.seek(1);
    int iqur,page_ok;
    in>>iqur>>page_ok;

    if(iqur)//商品数量查询成功
    {
        if(page_ok)//目标页码可跳转
        {
            t_loc[trolley_view_mode-1]=t_tar_loc[trolley_view_mode-1];
            t_tar_loc[trolley_view_mode-1]=-1;
            int goods_num;
            QVector<QVector<QString>> goods_infm;
            QVector<QByteArray> goods_img;
            in>>goods_num>>goods_infm>>goods_img;
            //改为goods_loc
//            emit Transport_goods(goods_locate,goods_num,goods_infm,goods_img);//发送信号使商品页进行显示
            emit Transport_trolley(t_loc[trolley_view_mode-1],goods_num,goods_infm,goods_img);

        }
        else//目标页码不可跳转
        {
            int max_page;
            in>>max_page;
            QString m_page="最大页码："+QString::number(max_page);
            //QMessageBox::information(this,"notice",m_page);
            noticebox(m_page);
        }
    }
    else//商品数量查询失败
    {
       qDebug()<<"inquire goods_num failed";
    }
    QString show_data;
    if(page_ok) show_data="购物车翻页反馈：成功";
    else show_data="购物车翻页反馈：失败";
    recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),DES_key);
}

void MainWindow::Search_species_t(int n,int mode)//购物车种类查询
{
    if(!mode)
        t_loc[trolley_view_mode-1]=0;
    species_nn=n;
    QString species_nn=QString::number(n);
    QString sql="SELECT * FROM (SELECT cart.Gname,good.Gnum,cart.Gprice,cart.Gdiscount,cart.Snum,good.Gtype,good.Gimage "
                "FROM cart,good WHERE cart.Gname = good.Gname AND cart.uid="+id+" AND good.Gtype="+species_nn+") AS A"
                +" LIMIT "+QString::number(t_loc[trolley_view_mode-1])+","+QString::number(t_loc[trolley_view_mode-1]+6);
    QByteArray qbt_sql = sql.toUtf8();
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << qbt_sql;
    buffer.close();
    QVector<QString> pac_show_vec = package_message_DES(65, 23, data, socket, DES_key, p->n, p->d);//数字签名DES加密
    QString show_det="请求用户购物车内["+spc[n-1]+"]类商品信息\n"+sql;
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
    t_loc[trolley_view_mode-1]+=6;
}

void MainWindow::Change_goods_view_mode_t(int n)//更改购物车表显示模式
{
    trolley_view_mode=n;
}

void MainWindow::Search_goods_t(QString sql,int mode)//购物车搜索框查询
{
    if(!mode)
        t_loc[trolley_view_mode-1]=0;
    search_sqll=sql;
    sql="SELECT * FROM (SELECT cart.Gname,good.Gnum,cart.Gprice,cart.Gdiscount,cart.Snum,good.Gtype,good.Gimage "
        "FROM cart,good WHERE cart.Gname = good.Gname AND cart.uid="+id+" AND good.Gname LIKE \'%"+sql+"%\') AS A"
        +" LIMIT "+QString::number(t_loc[trolley_view_mode-1])+","+QString::number(t_loc[trolley_view_mode-1]+6);
    QByteArray qbt_sql = sql.toUtf8();
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << qbt_sql;
    buffer.close();
    QVector<QString> pac_show_vec = package_message_DES(65, 22, data, socket, DES_key, p->n, p->d);//数字签名DES加密
    QString show_det="搜索用户ID为"+id+"的用户的购物车内名称包含“"+search_sqll+"”的商品";
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
    t_loc[trolley_view_mode-1]+=6;
}

void MainWindow::Update_trolley_number(QString sql)//更改购物车内商品数量、清理购物车
{
    QByteArray qbt_sql = sql.toUtf8();
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << qbt_sql;
    buffer.close();
    QVector<QString> pac_show_vec = package_message_DES(65, 7, data, socket, DES_key, p->n, p->d);//数字签名DES加密
    QString show_det="刷新购物车信息";
    pac_show_vec[7]=show_det;
    send_pack_show_des(pac_show_vec);
}

void MainWindow::Set_style()
{
    this->setWindowTitle("Shop");

    const QString label_head_SheetStyle = "border-radius: 35px;";
    ui->label_head->setStyleSheet(label_head_SheetStyle);
    ui->pushButton_4->setStyleSheet("border-radius:25px;");
    notice->setStyleSheet(ss.notice);
    notice->setAlignment(Qt::AlignCenter);
    notice->hide();

    //this->setWindowFlag(Qt::FramelessWindowHint);//删除顶部导航栏

    this->setStyleSheet("QMainWindow{"
                        "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(255, 247, 248), stop:1 rgb(255, 254, 245));"
                        "}");
    //this->setStyleSheet("border-radius:15px;");

    //*****主窗口圆角*****//
    //    this->setWindowFlags(Qt::FramelessWindowHint);//新建无边框窗口,可以把边框完全去掉
    //    QBitmap bmp(this->size());
    //    bmp.fill();
    //    QPainter p(&bmp);
    //    p.setPen(Qt::NoPen);
    //    p.setBrush(Qt::black);
    //    p.drawRoundedRect(bmp.rect(),15,15);//像素为10的圆角
    //    setMask(bmp);


}

void MainWindow::noticebox(QString ntc)
{
    notice->setText(ntc);
    notice->show();
    notice->setGeometry(255,200,180,40);
    QTimer *timer=new QTimer;
    timer->singleShot(1500,this,SLOT(Hide_notice()));
    delete timer;
}

void MainWindow::Hide_notice()
{
    notice->hide();
}
