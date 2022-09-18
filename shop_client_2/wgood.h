#ifndef WGOOD_H
#define WGOOD_H

#include <QMainWindow>
#include <string>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include "imagehandle.h"
#include "wpay.h"
#include "interface.h"
using namespace std;

//QVector<QVector<QString>> add_buffer;//商品页加购物车的缓存:商品名、加购数量、原价、现价
//QVector<QVector<QString>> add_buf;

namespace Ui {
class WGood;
}

class WGood : public QMainWindow
{
    Q_OBJECT

public:
    explicit WGood(QString uid,QMainWindow *p,QVector<QVector<QString>> buffer,QWidget *parent = nullptr);
    ~WGood();
    QString id;
    QVector<QVector<QString>> add_buffer;//商品页加购物车的缓存:商品名、加购数量、原价、现价
    void Set_good_style();


private slots:
    void Show_goods_1(int,int,QVector<QVector<QString>>,QVector<QByteArray>);//显示商品信息

    void Control_page_turn_btn(int,int);//翻页使能控制

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    //void Select_kind(QString kind);

    void on_pushButton_21_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_23_clicked();

//    void on_pushButton_left_clicked();

//    void on_pushButton_right_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void add_num();

    void sub_num();

    void on_pushButton_purchase_clicked();

    void on_pushButton_add_to_trolley_clicked();

signals:
    void Refresh_goods_list(int);
    void Get_goods_number(int);
    void Control_page_turn(int,int);
    void Search_goods_list(QString,int=1);
    void Search_species_list(int,int=1);
    void Change_view_mode(int);
    void addcart(int,QVector<QVector<QString>>);

private:
    QMainWindow *pmwd;
    Ui::WGood *ui;
    WPay* wpay;//=new WPay(id,add_buffer,0);
    QVector<QVector<QString>> show_infm;//正在显示的商品的数据
    //QVector<QVector<QString>> add_buffer;//商品页加购物车的缓存:商品名、加购数量、原价、现价
    QPushButton *add_btn[6],*sub_btn[6];
    Style_sheet ss;

};

#endif // WGOOD_H
