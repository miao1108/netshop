#ifndef MODIFYINF_H
#define MODIFYINF_H
#include"kerberos.h"
#include"network.h"
#include "interface.h"
#include <QDialog>

namespace Ui {
class modifyinf;
}

class modifyinf : public QDialog
{
    Q_OBJECT

public:
    explicit modifyinf(Pubdata alldata,QImage img,QWidget *parent = nullptr);
    void deal_aftermod(Message recv_mes);
    bool ifmodsuccess=false;
    bool ifimgchange=false;
    QImage currentimg;
    QImage beforeimg;
    Pubdata g_alldata;
    void noticebox(QString ntc);
    ~modifyinf();

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void Hide_notice();



signals:
    void Change_mwd_head(QImage);
    void Send_to_wsk1(QVector<QString>);
    void Send_to_wsk_recv1(Message,QString,QByteArray,QByteArray );
    void send_notice(QString);

private:

    Ui::modifyinf *ui;
    Style_sheet ss;
    QLabel *notice;
};

#endif // MODIFYINF_H
