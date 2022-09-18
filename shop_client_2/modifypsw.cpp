#include "modifypsw.h"
#include "ui_modifypsw.h"
#include"DES.h"
#include<QBuffer>
#include <QMessageBox>
static char Vkey_n[]="a42ec64809f7e777bae17b10d33bf5df4bb605d6124cda58fbe0d7322342bd3af61810e2ff62b33b0bcf09c5dd155d2a6e2fabd4ee655f78ad2cf19b0ada671893a843fe307e6eb9df63be5853dc97a48aeaf19996b05e471fb14bd0ee2a9f3d0d521c158d627855397900c218b78a6561228cf3064054de70a2b772e53d4f";
extern int e;
modifypsw::modifypsw(Pubdata alldata,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifypsw)
{
    ui->setupUi(this);
    notice=new QLabel(this);

    this->setWindowTitle("修改密码");
    this->setStyleSheet("QDialog{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(255, 247, 248), stop:1 rgb(255, 254, 245))}");
    ui->lineEdit->setStyleSheet(ss.line_edit_style);
    ui->lineEdit_2->setStyleSheet(ss.line_edit_style);
    ui->label->setStyleSheet(ss.general_txt);
    ui->label_2->setStyleSheet(ss.general_txt);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("确认修改"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("取消"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setFixedSize(80,30);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setFixedSize(80,30);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(ss.color_btn_style);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setStyleSheet(ss.write_btn_style);

    ui->lineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    ui->lineEdit_2->setEchoMode(QLineEdit::EchoMode::Password);
    g_alldata=alldata;

}
void modifypsw::deal_aftermod(Message recv_mes){
    //****des解密
    Message pac_show_mes=recv_mes;
    recv_mes.data=DES_de(g_alldata.Key_c_v,recv_mes.data);//res的data存放了数字签名和des的加密后的整体，所以解密的时候需要分开，直接这么调用就可以了
    recv_mes.sign=DES_de(g_alldata.Key_c_v,recv_mes.sign);   //不要数字签名这块去掉，DES的解密，res为message类型，res.data为message的data部分
    bool judge=sign_judge(recv_mes.data,recv_mes.sign,Vkey_n,e);      //不要数字签名的话这一行去掉

    //****解析报文
    bool success;
    QBuffer recbuffer(&recv_mes.data);
    recbuffer.open(QIODevice::ReadOnly);
    QDataStream recin(&recbuffer);
    recin>>success;

    if(success){
        //QMessageBox::information(this,"提示","修改成功");
        //noticebox("修改成功");
        emit send_notice("修改成功");
    }
    else //QMessageBox::information(this,"提示","密码错误");
        //noticebox("修改失败");
        emit send_notice("修改失败");

    QString show_data=QString("服务端向用户发送修改密码的结果：\n%1").arg(QString::number(success));
    emit Send_to_wsk_recv2(pac_show_mes,show_data,recv_mes.sign.toHex(),g_alldata.Key_c_v);
    //recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),g_alldata.Key_c_v);
}

modifypsw::~modifypsw()
{
    delete ui;
}

void modifypsw::on_pushButton_clicked()
{

}


void modifypsw::on_buttonBox_accepted()
{
    QString txtfind=QString("select upass from user where uid=%1").arg(g_alldata.IDc);
    QByteArray infSend;
    QBuffer infbuffer(&infSend);
    infbuffer.open(QIODevice::WriteOnly);
    QDataStream infout(&infbuffer);
    infout<<txtfind<<g_alldata.IDc<<ui->lineEdit->text()<<ui->lineEdit_2->text();
    infbuffer.close();
    QVector<QString> pac_show_vec=package_message_DES(65,25,infSend,g_alldata.socket,g_alldata.Key_c_v,g_alldata.mykey_n,g_alldata.mykey_d);
    QString show_det=QString("用户向服务端发送修改密码的请求报文：\n%1 用户id为：%2 原密码为：%3 新密码为： %4").arg(txtfind).arg(g_alldata.IDc).arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text());
    pac_show_vec[7]=show_det;
    emit Send_to_wsk2(pac_show_vec);
    //send_pack_show_des(pac_show_vec);
}

void modifypsw::noticebox(QString ntc)
{
    notice->setText(ntc);
    notice->show();
    notice->setGeometry(275,200,140,40);
    QTimer *timer=new QTimer;
    timer->singleShot(1500,this,SLOT(Hide_notice()));
    delete timer;
}

void modifypsw::Hide_notice()
{
    notice->hide();
}

