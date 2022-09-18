#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QBuffer>
#include <DES.h>
#include <string>
#include <iostream>
#include <QVector>
#include <QApplication>
#include <QBitmap>
#include <QPainter>
#include <QMessageBox>
#include <QLabel>
#include "homepage.h"
#include "wlogin.h"
#include "wgood.h"
#include "wtrolley.h"
#include "wuser.h"
#include "network.h"
#include "imagehandle.h"
#include "kerberos.h"
#include "package_show.h"
#include "interface.h"
using namespace std;

extern QVector<QVector<QString>> add_buffer;

struct test {
    int a;
    QByteArray data;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    QTcpSocket* socket = new QTcpSocket();
    key_pair* p = new key_pair();//锟皆硷拷锟斤拷锟斤拷钥
    QByteArray m_buffer;
    Message recv_mes;
    Pubdata g_allshare;
    QByteArray DES_key;
    void message_handel();//锟斤拷锟斤拷锟秸碉拷锟侥憋拷锟侥ｏ拷锟斤拷执锟斤拷锟斤拷应锟侥诧拷锟斤拷
    //QMainWindow* getMainWindow();
    QVector<QVector<QString>> pack_show_data;//抓包函数，每一行按顺序分别是，目的IP,源IP,报文类型，字段类型，数字签名，补充说明，报文内容（加密），解密内容（解密），密钥；
    long pack_show_num=0;//记录抓包报文的总数
    //void send_pack_show_des(QVector<QString> need_data);
    package_show *capture_window=new package_show();
    void recv_pack_show(Message mes,QString data,QByteArray sign="0",QByteArray key="0");
    void send_pack_show_des(QVector<QString> need_data);
    void Set_style();
    void noticebox(QString);

private slots:

    void Login_check(QString, QString);//锟斤拷录
    void Login_confirm();//锟斤拷锟斤拷锟斤拷录锟斤拷锟斤拷
    void Change_head(QImage);//锟斤拷锟斤拷锟斤拷页头锟斤拷

    void Get_goods(int start,int end);//锟斤拷询锟斤拷品锟斤拷指锟斤拷位锟斤拷
    void Show_goods();//锟斤拷示锟斤拷品锟斤拷询锟斤拷锟斤拷
    void Refresh_goods(int);//刷锟斤拷锟斤拷品锟斤拷
    void To_page(int);//页锟斤拷锟斤拷转
    void Page_handel();//锟斤拷锟斤拷锟斤拷页
    void Search_species(int n,int mode=0);//锟斤拷锟斤拷锟斤拷询
    void Change_goods_view_mode(int);//锟斤拷锟斤拷锟斤拷品锟斤拷锟斤拷示模式
    void Search_goods(QString,int mode=0);//锟斤拷锟斤拷锟斤拷锟斤拷询
    void Addcart_send(int,QVector<QVector<QString>>);//锟斤拷锟诫购锟斤车
    void addcart_recive();//锟斤拷锟诫购锟斤车锟斤拷锟斤拷锟斤拷锟斤拷

    //*****//*****//*****//*****//*****ADD*****//*****//*****//*****////////////////////////////////////////////////

    //报文:67-3;67-11

    void Goods_order_pay(QByteArray);//商品页直接付款
    void Goods_order_handle();//商品页下单反馈
    void Trolley_order_pay(QByteArray);//购物车下单
    void Trolley_order_handle();//购物车下单反馈
    void Inquire_all_trolley_goods();//查询整个购物车表
    void Trolley_all_handle();//查询整个购物车的反馈



    //*****//*****//*****//*****//*****DDA*****//*****//*****//*****////////////////////////////////////////////////

    void Get_goods_t(int start,int end);//锟斤拷询锟斤拷锟斤车锟斤拷指锟斤拷位锟斤拷
    void Show_goods_t();//锟斤拷示锟斤拷锟斤车锟斤拷询锟斤拷锟斤拷
    void Refresh_goods_t(int);//刷锟铰癸拷锟斤车锟斤拷
    void To_page_t(int);//锟斤拷锟斤车页锟斤拷锟斤拷转
    void Page_handel_t();//锟斤拷锟斤车锟斤拷锟斤拷锟斤拷页
    void Search_species_t(int n,int mode=0);//锟斤拷锟斤车锟斤拷锟斤拷锟斤拷询
    void Change_goods_view_mode_t(int);//锟斤拷锟侥癸拷锟斤车锟斤拷锟斤拷示模式
    void Search_goods_t(QString,int mode=0);//锟斤拷锟斤车锟斤拷锟斤拷锟斤拷锟斤拷询
    void Update_trolley_number(QString);//锟斤拷锟侥癸拷锟斤车锟斤拷锟斤拷品锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤车

    void Send_pack_show(QVector<QString>);
    void Recv_pack_show(Message mes,QString data,QByteArray sign,QByteArray key);




    void socket_Read_Data();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    //keberos
    void Kerberos(int idc);
    void AS_handel();
    void TGS_handel();
    void C_handel();


    void on_pushButton_4_clicked();
    void Hide_notice();

    //void Notice_box(QMainWindow* pmain, QString);




signals:
    void Transport_goods(int,int,QVector<QVector<QString>>,QVector<QByteArray>);
    void Control_page_turn(int,int);

    void Transport_trolley(int,int,QVector<QVector<QString>>,QVector<QByteArray>);
    void Control_page_turn_t(int,int);

    void Send_trolley_buffer(QVector<QVector<QString>>);



private:
    QString id, pwd;
    Ui::MainWindow* ui;
    Homepage* home;
    WLogin* wlogin = new WLogin(this);
    //WGood* wgood= new WGood(id,this);
    WGood* wgood;
    WTrolley* wtrolley;
    WUser* myuser;
    QImage* head_picture;
    QImage* good_btn_pic;
    QVector<QVector<QString>> adbuf;
    QVector<QString> tmp_buffer;
    Style_sheet ss;
    QLabel *notice;

    int goods_view_mode=1;//1.默锟较ｏ拷2.锟斤拷锟斤拷锟斤拷3-8.锟斤拷锟斤拷锟斤拷锟斤拷
    //int trolley_view_mode=1;//1.默锟较ｏ拷2.锟斤拷锟斤拷锟斤拷3-8.锟斤拷锟斤拷锟斤拷锟斤拷
    int g_loc[8]={0,0,0,0,0,0,0,0};//锟斤拷品页-锟斤拷品锟斤拷锟捷匡拷锟斤拷锟斤拷
    int g_tar_loc[8]={-1,-1,-1,-1,-1,-1,-1,-1};//锟斤拷品页-锟斤拷品锟斤拷示目锟斤拷锟斤拷锟捷匡拷锟斤拷锟斤拷
    QString search_sql;//锟斤拷品页-锟较次诧拷询锟斤拷锟斤拷锟斤拷
    int species_n;//锟斤拷品页-锟较次诧拷询锟斤拷锟斤拷锟斤拷

    int trolley_view_mode=1;//1.默锟较ｏ拷2.锟斤拷锟斤拷锟斤拷3.锟斤拷锟斤拷锟斤拷锟斤拷
    int t_loc[8]={6,0,0,0,0,0,0,0};//锟斤拷锟斤车页-锟斤拷品锟斤拷锟捷匡拷锟斤拷锟斤拷
    int t_tar_loc[8]={-1,-1,-1,-1,-1,-1,-1,-1};//锟斤拷锟斤车页-锟斤拷品锟斤拷示目锟斤拷锟斤拷锟捷匡拷锟斤拷锟斤拷
    QString search_sqll;//锟斤拷锟斤车页-锟较次诧拷询锟斤拷锟斤拷锟斤拷
    int species_nn;//锟斤拷锟斤车页-锟较次诧拷询锟斤拷锟斤拷锟斤拷

    int wd=0;//当前窗口类型：1、商品页；2、购物车；3、个人主页
    QString spc[6]={"水果生鲜","零食饮料","日常用品","美妆服饰","科技数码","家用电器"};//商品种类

};
#endif // MAINWINDOW_H
