#ifndef MODIFYPSW_H
#define MODIFYPSW_H
#include "kerberos.h"
#include "network.h"
#include "interface.h"
#include <QDialog>
#include <QPushButton>

namespace Ui {
class modifypsw;
}

class modifypsw : public QDialog
{
    Q_OBJECT

public:
    explicit modifypsw(Pubdata alldata,QWidget *parent = nullptr);
    void deal_aftermod(Message recv_mes);
    Pubdata g_alldata;
    void noticebox(QString ntc);
    ~modifypsw();

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

    void Hide_notice();

signals:
    void Send_to_wsk2(QVector<QString>);
    void Send_to_wsk_recv2(Message,QString,QByteArray,QByteArray );
    void send_notice(QString);

private:
    Ui::modifypsw *ui;
    Style_sheet ss;
    QLabel *notice;
};

#endif // MODIFYPSW_H
