#include "homepage.h"
#include "ui_homepage.h"

Homepage::Homepage(string uid,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
    id=uid;
    ui->statusbar->hide();//隐藏状态栏
    delete ui->statusbar;//删除状态栏


    //图片显示

    QString filename(":/new/prefix1/head");
    if(! ( head_picture->load(filename) ) ) //加载图像
    {
        QMessageBox::information(this,tr("打开图像失败"),tr("打开图像失败"));
        delete head_picture;
        return;
    }
    ui->label_head->setPixmap(QPixmap::fromImage(*head_picture));

    good_btn_pic->load(":/new/prefix2/btn");
//    ui->pushButton->

}

Homepage::~Homepage()
{
    delete ui;
}

void Homepage::on_pushButton_clicked()
{
    //wgood=new WGood(id);
    //ui->scrollArea->setWidget(wgood);
}

void Homepage::on_pushButton_2_clicked()
{
    //wtrollry=new WTrolley(id);
    //ui->scrollArea->setWidget(wtrollry);
}

void Homepage::on_pushButton_3_clicked()
{
    //wuser=new WUser(id);
    //ui->scrollArea->setWidget(wuser);
}
