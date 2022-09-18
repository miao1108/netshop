#include "wuser.h"
#include "ui_wuser.h"
#include"DES.h"
#include"imagehandle.h"
#include <QMessageBox>
#include<QBuffer>
#include<QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
static char Vkey_n[]="a42ec64809f7e777bae17b10d33bf5df4bb605d6124cda58fbe0d7322342bd3af61810e2ff62b33b0bcf09c5dd155d2a6e2fabd4ee655f78ad2cf19b0ada671893a843fe307e6eb9df63be5853dc97a48aeaf19996b05e471fb14bd0ee2a9f3d0d521c158d627855397900c218b78a6561228cf3064054de70a2b772e53d4f";
extern int e;
WUser::WUser(Pubdata alldata,QMainWindow *p,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WUser)
{
    ui->setupUi(this);
    pmwd=p;
    g_alldata=alldata;
    notice=new QLabel(this);
    //ui->statusbar->hide();//隐藏状态栏
    Set_user_style();

    QString txtinf=QString("select uname,uphe,upic from user where uid=%1").arg(g_alldata.IDc);
    QString txtorder=QString("select gname,anum,atime from sale where mid=%1").arg(g_alldata.IDc);
    QString txtvip=QString("select endtime from vip where uid=%1").arg(g_alldata.IDc);
    QByteArray infSend;
    QBuffer infbuffer(&infSend);
    infbuffer.open(QIODevice::WriteOnly);
    QDataStream infout(&infbuffer);
    infout<<txtinf<<txtorder<<txtvip;
    infbuffer.close();
    QVector<QString> pac_show_vec=package_message_DES(65,24,infSend,g_alldata.socket,g_alldata.Key_c_v,g_alldata.mykey_n,g_alldata.mykey_d);
    QString show_det=QString("这是个人信息页面发送的初始化信息数据请求报文，内容有：三条查询语句：%1\n%2\n%3").arg(txtinf).arg(txtorder).arg(txtvip);
    pac_show_vec[7]=show_det;
    connect(this,SIGNAL(Send_to_wsk(QVector<QString>)),pmwd,SLOT(Send_pack_show(QVector<QString>)));
    emit Send_to_wsk(pac_show_vec);
    //send_pack_show_des(pac_show_vec);
    ui->label_8->setText(QString("%1").arg(g_alldata.IDc));
    //QString("这是个人信息页面发送的初始化信息数据请求报文，内容有：三条查询语句：%1\n%2\n%3").arg(txtinf).arg(txtorder).arg(txtvip);
    //QString show_det="刷新购物车信息";
    //pac_show_vec[7]=show_det;
    //send_pack_show_des(pac_show_vec);
}

WUser::~WUser()
{
    delete ui;
}

void WUser::deal_initi(Message recv_mes){
    //****des解密
    Message pac_show_mes=recv_mes;
    recv_mes.data=DES_de(g_alldata.Key_c_v,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(g_alldata.Key_c_v,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,e);      //不要数字签名的话这一行去掉

    //****解析报文
    QString uname,uphe,datestring;
    QByteArray upic;
    int order_size;
    QVector<QVector<QString>> orders;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>uname>>uphe>>upic>>vip;
    if(vip)recin>>datestring;
    recin>>order_size;
    if(order_size!=0)recin>>orders;

    g_alldata.uname=uname;
    g_alldata.upho=uphe;

    //****还原图片
    img=getImage(upic);

    //***显示vip
    if(vip){
        ui->label_2->setStyleSheet("font: 15px '汉仪正圆-75W';color:qlineargradient(spread:pad, x1:1, y1:1, x2:0.287, y2:0.335227, stop:0.22905 rgba(255, 47, 0, 255), stop:1 rgba(255, 255, 139, 231));");
        ui->label_6->setStyleSheet("border-radius: 8px;font:11px '微软雅黑';color:#fae60a;background-color:#141400;");
        ui->label_6->setText(QString(QString("vip ")+datestring+QString(" 到期")));//到时候设置奢华的字体！！！
        QImage vipimage=QImage("://picture/VIP.png");
        QMatrix matrix;
        matrix.rotate(315);
        vipimage = vipimage.transformed(matrix); //Img是一个图片，此时图片旋转270度
        QPixmap pix_img=QPixmap::fromImage(vipimage);
        QPixmap fitpixmap_userIcon=pix_img.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        fitpixmap_userIcon = PixmapToRound(fitpixmap_userIcon,50);
        ui->label_10->setScaledContents(true);//铺满label
        ui->label_10->setPixmap(fitpixmap_userIcon);
    }
    else{
         ui->label_2->setStyleSheet("font: 15px '汉仪正圆-75W';color:#444444;");
         ui->label_6->setVisible(false);
    }

    //***显示ui
     ui->label_2->setText(uname);
     ui->label_9->setText(uphe);

     QPixmap pix_img=QPixmap::fromImage(img);
     QPixmap fitpixmap_userIcon=pix_img.scaled(160, 160, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
     fitpixmap_userIcon = PixmapToRound(fitpixmap_userIcon,80);
     ui->label->setPixmap(fitpixmap_userIcon);

     //****显示订单
     if(order_size!=0){
         // ui->label_7->setStyleSheet("background:transparent");
           ui->label_7->setVisible(false);
         QStandardItem *item;
         QStandardItemModel *ordertable=new QStandardItemModel();
         ordertable->setColumnCount(3);
         ordertable->setHeaderData(0,Qt::Horizontal,QString("商品名"));
         ordertable->setHeaderData(1,Qt::Horizontal,QString("数量"));
         ordertable->setHeaderData(2,Qt::Horizontal,QString("购买时间"));
         for(int i=0;i<orders.size();++i){
             for(int j=0;j<orders[i].size();++j){
                 item=new QStandardItem(orders[i][j]);
                 ordertable->setItem(i,j,item);
                 ordertable->item(i, j)->setTextAlignment(Qt::AlignCenter);
             }
         }

         ui->tableView->verticalHeader()->hide();//隐藏行号方法

         ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section, QTableCornerButton::section {padding: 1px;border: none;background-color:#fd9f9a;color:#fde1df;}");

         ui->tableView->setStyleSheet("QTableView::item::selected{"
                                      "background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 0, stop: 0 #ffe1d7, stop: 0.25 #fcffe6, stop: 0.5 #f8ffcd, stop: 0.75 #fcffe6);color:rgb(0,0,0);}");

         ui->tableView->setShowGrid(false);//隐藏单元格线
         ui->tableView->setModel(ordertable);
         //设置列宽不可变

         ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);

         ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);

         ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);

         ui->tableView->setColumnWidth(0,187);

         ui->tableView->setColumnWidth(1,93);

         ui->tableView->setColumnWidth(2,281);

     }

     QString txt1=QString("这是服务器端发送给用户的个人信息界面的初始化结果\n 用户名：%1 电话：%2 是否vip%3 \n图片：%4\n订单：\n").arg(uname).arg(uphe).arg(QString::number(vip)).arg(QString(upic.toHex()));
     for(int i=0;i<orders.size();++i){
         txt1+=QString("%1 %2 %3\n").arg(orders[i][0]).arg(orders[i][1]).arg(orders[i][2]);
     }
     QString show_data=txt1;
     emit Send_to_wsk_recv(pac_show_mes,show_data,recv_mes.sign.toHex(),g_alldata.Key_c_v);
     //recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),g_alldata.Key_c_v);
}

void WUser::on_pushButton_2_clicked()
{
    modifyinfs=new modifyinf(g_alldata,img,this);
    connect(modifyinfs,SIGNAL(Change_mwd_head(QImage)),pmwd,SLOT(Change_head(QImage)));
    connect(modifyinfs,SIGNAL(Change_mwd_head(QImage)),this,SLOT(Change_head(QImage)));
    connect(modifyinfs,SIGNAL(Send_to_wsk1(QVector<QString>)),pmwd,SLOT(Send_pack_show(QVector<QString>)));
    connect(modifyinfs,SIGNAL(Send_to_wsk_recv1(Message,QString,QByteArray,QByteArray )),pmwd,SLOT(Recv_pack_show(Message,QString,QByteArray,QByteArray)));
    connect(modifyinfs,SIGNAL(send_notice(QString)),this,SLOT(noticebox(QString)));
    //modifyinfs->setModal(false);
    modifyinfs->show();
    if(modifyinfs->exec()==QDialog::Rejected&&modifyinfs->ifmodsuccess==true){
        g_alldata.uname=modifyinfs->g_alldata.uname;
        g_alldata.upho=modifyinfs->g_alldata.upho;
        img=modifyinfs->currentimg;
        ui->label_2->setText(g_alldata.uname);
        ui->label_9->setText(g_alldata.upho);

        //QPixmap pix_img=QPixmap::fromImage(modifyinfs->currentimg.scaled(ui->label->size()));
        //QPixmap fitpixmap_userIcon=pix_img.scaled(200, 0, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        //fitpixmap_userIcon = PixmapToRound(fitpixmap_userIcon,200);
        //QImage head_img=getImage(goods_img[i]);
        QPixmap pix_img=QPixmap::fromImage(modifyinfs->currentimg.scaled(ui->label->size()));
        pix_img=getRoundRectPixmap(pix_img, pix_img.size(), 40);
        //label_img->setPixmap(pix_img);
        ui->label->setScaledContents(true);//铺满label
        ui->label->setPixmap(pix_img);
        //ui->label->setPixmap(QPixmap::fromImage(modifyinfs->currentimg.scaled(ui->label->size())));
    }

}

void WUser::on_pushButton_3_clicked()
{
    modifypsws=new modifypsw(g_alldata);
    connect(modifypsws,SIGNAL(Send_to_wsk2(QVector<QString>)),pmwd,SLOT(Send_pack_show(QVector<QString>)));
    connect(modifypsws,SIGNAL(Send_to_wsk_recv2(Message,QString,QByteArray,QByteArray )),pmwd,SLOT(Recv_pack_show(Message,QString,QByteArray,QByteArray)));
    connect(modifypsws,SIGNAL(send_notice(QString)),this,SLOT(noticebox(QString)));
    modifypsws->show();
}

void WUser::Transmit_img(QImage img)
{
    emit Transmit_img_to_mwd(img);
}

void WUser::on_pushButton_clicked()
{
    if(vip){
        //QMessageBox::information(this,"提示","您已经是尊贵的VIP用户了");
        noticebox("您已经是尊贵的VIP用户了");
        return;
    }
    vipregists=new VIPregist(g_alldata);
    vipregists->show();
    if(vipregists->exec()==QDialog::Accepted){

    }
}

void WUser::VIP_handel(Message recv_mes){
    //****des解密
    Message pac_show_mes=recv_mes;
    recv_mes.data=DES_de(g_alldata.Key_c_v,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(g_alldata.Key_c_v,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,e);      //不要数字签名的话这一行去掉

    //****解析报文
    bool success;
    QString datestring;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>success>>datestring;
    if(success){
    vip=true;
    //*****ADD*****//
    ui->label_2->setStyleSheet("font: 15px '汉仪正圆-75W';color:qlineargradient(spread:pad, x1:1, y1:1, x2:0.287, y2:0.335227, stop:0.22905 rgba(255, 47, 0, 255), stop:1 rgba(255, 255, 139, 231));");
    ui->label_6->setStyleSheet("border-radius: 8px;font:11px '微软雅黑';color:#fae60a;background-color:#141400;");
    ui->label_6->setText(QString(QString("vip ")+datestring+QString(" 到期")));//到时候设置奢华的字体！！！
    QImage vipimage=QImage("://picture/VIP.png");
    QMatrix matrix;
    matrix.rotate(315);
    vipimage = vipimage.transformed(matrix); //Img是一个图片，此时图片旋转270度
    QPixmap pix_img=QPixmap::fromImage(vipimage);
    QPixmap fitpixmap_userIcon=pix_img.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    fitpixmap_userIcon = PixmapToRound(fitpixmap_userIcon,50);
    ui->label_10->setScaledContents(true);//铺满label
    ui->label_10->setPixmap(fitpixmap_userIcon);
    noticebox("恭喜您成为尊敬的VIP用户");
    //*****DDA*****//
    //ui->label_6->setText(QString("vip(")+datestring+QString("到期)"));
    ui->label_6->setVisible(true);
    }
    QString show_data=QString("服务器端发送给用户的VIP申请结果：%1 %2").arg(QString::number(success)).arg(datestring);

    emit Send_to_wsk_recv(pac_show_mes,show_data,recv_mes.sign.toHex(),g_alldata.Key_c_v);
    //recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),g_alldata.Key_c_v);
}

void WUser::Set_user_style()
{
    this->setStyleSheet("QMainWindow{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(255, 247, 248), stop:1 rgb(255, 254, 245))}");
    ui->pushButton->setStyleSheet(write_button_style);
    ui->pushButton_2->setStyleSheet(write_button_style);
    ui->pushButton_3->setStyleSheet(write_button_style);
    ui->label_3->setStyleSheet(ss.general_txt);
    ui->label_4->setStyleSheet(ss.general_txt);
    ui->label_8->setStyleSheet(ss.general_txt);
    ui->label_9->setStyleSheet(ss.general_txt);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->label->setScaledContents(true);//铺满label
    ui->label->setStyleSheet("min-width: 80px; min-height: 80px;max-width:80px; max-height: 80px;border-radius: 40px;  border:none;");
    ui->label_10->setStyleSheet("min-width: 60px; min-height: 60px;max-width:60px; max-height: 60px;border-radius: 30px;  border:none;");
    notice->setStyleSheet(ss.notice);
    notice->setAlignment(Qt::AlignCenter);
    notice->hide();
}

void WUser::noticebox(QString ntc)
{
    notice->setText(ntc);
    notice->raise();
    notice->show();
    notice->setGeometry(145,200,220,40);
    QTimer *timer=new QTimer;
    timer->singleShot(1500,this,SLOT(Hide_notice()));
    delete timer;
}

void WUser::Hide_notice()
{
    notice->hide();
}

void WUser::Change_head(QImage img)
{
    //QPixmap pix_img=QPixmap::fromImage(img);
    //QPixmap fitpixmap_userIcon=pix_img.scaled(200, 130, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
   // fitpixmap_userIcon = PixmapToRound(fitpixmap_userIcon,200);
    //ui->label->setScaledContents(true);//铺满label
   // ui->label->setPixmap(fitpixmap_userIcon);
}
