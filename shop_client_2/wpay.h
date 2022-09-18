#ifndef WPAY_H
#define WPAY_H

#include <QMainWindow>
#include <string>
#include <QTime>
#include <QBuffer>
#include <QStandardItem>
#include "interface.h"
using namespace std;

//bool trolley_pay;

namespace Ui {
class WPay;
}

class WPay : public QMainWindow
{
    Q_OBJECT

public:
    explicit WPay(QString uid,QVector<QVector<QString>> buffer,int mode,QMainWindow *p,QWidget *parent = nullptr);
    ~WPay();
    void Set_pay_style();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


signals:
    void Goods_order(QByteArray);
    void Trolley_order(QByteArray);
    void Change_trolley_pay();

private:
    Ui::WPay *ui;
    QString id;
    int pay_mode;
    QMainWindow *prt;
    QVector<QVector<QString>> under_order;
    double money=0;
    QStandardItemModel *res=new QStandardItemModel();
    Style_sheet ss;
    void Show_order();

};

#endif // WPAY_H
