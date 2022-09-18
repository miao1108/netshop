#ifndef WUSER_H
#define WUSER_H
#include <QTcpSocket>
#include <QMainWindow>
#include <string>
#include "modifyinf.h"
#include "modifypsw.h"
#include "vipregist.h"
#include "network.h"
#include "kerberos.h"
#include "interface.h"
using namespace std;

namespace Ui {
class WUser;
}

class WUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit WUser(Pubdata alldata,QMainWindow *p,QWidget *parent = nullptr);
    void deal_initi(Message recv_mes);
    modifyinf * modifyinfs;
    modifypsw * modifypsws;
    VIPregist * vipregists;
    bool vip;
    void VIP_handel(Message recv_mes);

    void Set_user_style();
    ~WUser();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void Transmit_img(QImage);

    void on_pushButton_clicked();

    void noticebox(QString ntc);

    void Hide_notice();

    void Change_head(QImage);

signals:
    void Transmit_img_to_mwd(QImage);
    void Send_to_wsk(QVector<QString>);
    void Send_to_wsk_recv(Message,QString,QByteArray,QByteArray );
    void Send_pack_show_des_usr(QVector<QString> need_data);

private:
    //QString id;
    QImage img;
    Pubdata g_alldata;
    Ui::WUser *ui;
    QMainWindow *pmwd;
    Style_sheet ss;
    QLabel *notice;
    const QString write_button_style="QPushButton{"
                                     "background-color:rgb(255,255,255);"
                                     "border-radius: 13px; "
                                     "border:1px solid rgba(255,152,95,255);"
                                     "color:qlineargradient(spread:pad, x1:0, y1:0.034, x2:0.730337, y2:0.756045, stop:0 rgba(255, 181, 79, 255), stop:1 rgba(255, 133, 111, 255));"
                                     "font: bold 13px;"
                                     "font-family: 汉仪正圆-45W;}"
                                   "QPushButton:pressed {background-color: #dfdfdf;}";

};

#endif // WUSER_H
