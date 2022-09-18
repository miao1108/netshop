#ifndef WTROLLEY_H
#define WTROLLEY_H

#include <QMainWindow>
#include <string>
#include <QPushButton>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include "imagehandle.h"
#include "wpay.h"
#include "interface.h"
using namespace std;
using namespace std;



namespace Ui {
class WTrolley;
}

class WTrolley : public QMainWindow
{
    Q_OBJECT

public:
    explicit WTrolley(QString uid,QMainWindow *p,QWidget *parent = nullptr);
    ~WTrolley();
    QString id;
    QVector<QVector<QString>> add_buffer_t;//商品页加购物车的缓存:商品名、加购数量、原价、现价
    void Set_trolley_style();

private slots:
    void Show_trolley_1(int,int,QVector<QVector<QString>>,QVector<QByteArray>);

    void Control_page_turn_btn_t(int,int);//翻页使能控制

    void Get_buffer(QVector<QVector<QString>>);

    void Change_trolley_pay_show();

    void on_pushButton_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void add_num();

    void sub_num();

    void on_pushButton_24_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_add_to_trolley_clicked();

    void on_pushButton_purchase_clicked();

signals:
    void Change_view_mode_t(int);
    void Refresh_goods_list_t(int);
    void Get_goods_number_t(int);
    void Search_goods_list_t(QString,int=1);
    void Search_species_list_t(int,int=1);
    void Control_page_turn_t(int,int);
    void Update_trolley_num(QString);
    void Inquire_all_trolley();

private:
    Ui::WTrolley *ui;
    QMainWindow *pmwd;
    WPay* wpay;//=new WPay(id,add_buffer_t,0);
    QVector<QString> tmp_infm;
    QVector<QVector<QString>> show_infm;//正在显示的商品的数据
    QPushButton *add_btn[6],*sub_btn[6];
    bool trolley_pay=true;
    Style_sheet ss;
};

#endif // WTROLLEY_H
