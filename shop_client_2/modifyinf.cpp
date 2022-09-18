#include "modifyinf.h"
#include "ui_modifyinf.h"
#include"DES.h"
#include"imagehandle.h"
#include<QBuffer>
#include<QFileDialog>
#include <QMessageBox>
static char Vkey_n[]="a42ec64809f7e777bae17b10d33bf5df4bb605d6124cda58fbe0d7322342bd3af61810e2ff62b33b0bcf09c5dd155d2a6e2fabd4ee655f78ad2cf19b0ada671893a843fe307e6eb9df63be5853dc97a48aeaf19996b05e471fb14bd0ee2a9f3d0d521c158d627855397900c218b78a6561228cf3064054de70a2b772e53d4f";
extern int e;
modifyinf::modifyinf(Pubdata alldata,QImage img,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifyinf)
{
    ui->setupUi(this);
    g_alldata=alldata;
    currentimg=img;
    beforeimg=currentimg;
    notice=new QLabel(this);

    this->setWindowTitle("修改个人信息");
    this->setStyleSheet("QDialog{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(255, 247, 248), stop:1 rgb(255, 254, 245))}");
    ui->pushButton->setStyleSheet(ss.color_btn_style);
    ui->lineEdit->setStyleSheet(ss.line_edit_style);
    ui->lineEdit_2->setStyleSheet(ss.line_edit_style);
    ui->label_2->setStyleSheet(ss.general_txt);
    ui->label_3->setStyleSheet(ss.general_txt);

    ui->label->setPixmap(QPixmap::fromImage(img.scaled(ui->label->size())));
    ui->lineEdit->setText(g_alldata.uname);
    ui->lineEdit_2->setText(g_alldata.upho);
    notice->setStyleSheet(ss.notice);
    notice->setAlignment(Qt::AlignCenter);
    notice->hide();
}


modifyinf::~modifyinf()
{
    delete ui;
}

void modifyinf::on_buttonBox_accepted()
{

}


void modifyinf::on_pushButton_clicked()
{
    QString txt=QString("update user set uname='%1',uphe='%2' where uid=%3").arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text()).arg(g_alldata.IDc);
    QByteArray infSend;
    QBuffer infbuffer(&infSend);
    infbuffer.open(QIODevice::WriteOnly);
    QDataStream infout(&infbuffer);
    infout<<ifimgchange;
    if(ifimgchange)infout<<ui->lineEdit->text()<<ui->lineEdit_2->text()<<getImageData(currentimg)<<g_alldata.IDc;
    else infout<<txt;
    infbuffer.close();
    QVector<QString> pac_show_vec=package_message_DES(65,8,infSend,g_alldata.socket,g_alldata.Key_c_v,g_alldata.mykey_n,g_alldata.mykey_d);

    QString txt1;
    if(ifimgchange){
        txt1=QString("修改后的用户名：%1 修改后的电话号码：%2 \n修改后的头像： %3 \n用户的id: %4").arg(ui->lineEdit->text())
                .arg(ui->lineEdit_2->text()).arg(QString(getImageData(currentimg).toHex())).arg(g_alldata.IDc);
    }
    else txt1=txt;
    QString show_det=txt1;
    pac_show_vec[7]=show_det;
    emit Send_to_wsk1(pac_show_vec);
    //send_pack_show_des(pac_show_vec);
}

void modifyinf::deal_aftermod(Message recv_mes){
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
        ifmodsuccess=true;
        g_alldata.uname=ui->lineEdit->text();
        g_alldata.upho=ui->lineEdit_2->text();
        //修改主窗口头像
        emit Change_mwd_head(currentimg);
        this->close();
   }
    else {
        //QMessageBox::information(this,"提示","修改错误");
        //noticebox("修改失败");
        emit send_notice("修改失败");
        ui->lineEdit->setText(g_alldata.uname);
        ui->lineEdit_2->setText(g_alldata.upho);
        currentimg=beforeimg;
        ui->label->setPixmap(QPixmap::fromImage(currentimg));
    }
    ifimgchange=false;

    QString show_data=QString("服务器发送给用户的修改信息反馈结果：%1").arg(QString::number(success));
    emit Send_to_wsk_recv1(pac_show_mes,show_data,recv_mes.sign.toHex(),g_alldata.Key_c_v);
    //recv_pack_show(pac_show_mes,show_data,recv_mes.sign.toHex(),g_alldata.Key_c_v);
}

void modifyinf::on_pushButton_2_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("选择图像"),"",tr("Images (*.jpg)"));
        if(filename.isEmpty())
            return;
        else
        {
            QImage img;
            if(!(img.load(filename))) //加载图像
            {
                //QMessageBox::information(this, tr("打开图像失败"),tr("打开图像失败!"));
                noticebox("打开图像失败");
                //emit send_notice("打开图像失败");
                return;
            }
            ifimgchange=true;
            beforeimg=currentimg;
            currentimg=img;
            ui->label->setPixmap(QPixmap::fromImage(img.scaled(ui->label->size())));
            ui->label->setScaledContents(true);//铺满label
        }
}

void modifyinf::noticebox(QString ntc)
{
    notice->setText(ntc);
    notice->show();
    notice->setGeometry(90,130,200,40);
    QTimer *timer=new QTimer;
    timer->singleShot(1500,this,SLOT(Hide_notice()));
    delete timer;
}

void modifyinf::Hide_notice()
{
    notice->hide();
}
