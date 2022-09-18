#include "vipregist.h"
#include "ui_vipregist.h"
#include"network.h"
#include <QMessageBox>
#include<QBuffer>
VIPregist::VIPregist(Pubdata alldata,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VIPregist)
{
    ui->setupUi(this);
    g_alldata=alldata;
    this->setWindowTitle("VIP");
    this->setStyleSheet("QDialog{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(255, 247, 248), stop:1 rgb(255, 254, 245))}");
    ui->radioButton->setChecked(true);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("开通"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("取消"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setFixedSize(80,30);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setFixedSize(80,30);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(ss.color_btn_style);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setStyleSheet(ss.write_btn_style);
    ui->label->setStyleSheet(ss.general_txt);
    ui->label_3->setStyleSheet(ss.general_txt);
    ui->label_2->setStyleSheet(ss.general_txt);
    ui->radioButton->setStyleSheet(ss.general_txt);
    ui->radioButton_2->setStyleSheet(ss.general_txt);
    ui->radioButton_3->setStyleSheet(ss.general_txt);
    ui->label_4->setStyleSheet("border-radius: 10px;font:12px '微软雅黑';color:#fae60a;background-color:#141400;");


}

VIPregist::~VIPregist()
{
    delete ui;
}

void VIPregist::on_radioButton_clicked()
{
    ui->radioButton->setChecked(true);
    ui->radioButton_2->setChecked(false);
    ui->radioButton_3->setChecked(false);
}


void VIPregist::on_radioButton_2_clicked()
{
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(true);
    ui->radioButton_3->setChecked(false);
}


void VIPregist::on_buttonBox_accepted()
{
    int type;
    if(ui->radioButton->isChecked()) type=1;
    if(ui->radioButton_2->isChecked()) type=2;
    if(ui->radioButton_3->isChecked()) type=3;
    QByteArray infSend;
    QBuffer infbuffer(&infSend);
    infbuffer.open(QIODevice::WriteOnly);
    QDataStream infout(&infbuffer);
    infout<<g_alldata.IDc<<g_alldata.uname<<type;
    infbuffer.close();
    package_message_DES(65,12,infSend,g_alldata.socket,g_alldata.Key_c_v,g_alldata.mykey_n,g_alldata.mykey_d);

    //QString("用户向服务器端发送VIP申请报文：\n 用户ID：%1 用户名：%2 会员类型：%3").arg(g_alldata.IDc).arg(g_alldata.uname).arg(type);

}


void VIPregist::on_radioButton_3_clicked()
{
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(false);
    ui->radioButton_3->setChecked(true);
}



