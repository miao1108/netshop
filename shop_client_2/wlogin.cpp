#include "wlogin.h"
#include "ui_wlogin.h"
#include <QDebug>

WLogin::WLogin(QMainWindow *p, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WLogin)
{
    ui->setupUi(this);
    uii=ui;
    p_mwd=p;
    this->setWindowTitle("Shop");
    ui->label_error->hide();
    ui->lineEdit_2->setEchoMode(QLineEdit::EchoMode::Password);
    ui->lineEdit->setStyleSheet("font: 25 12pt '微软雅黑';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色 "padding-left:20px;"       //内边距-字体缩进
                                    "background-color: rgb(255, 255, 255);" //背景颜色
                                    "border:2px solid #d9a7c7;border-radius:15px;");//边框粗细-颜色-圆角设置
    ui->lineEdit_2->setStyleSheet("font: 25 12pt '微软雅黑';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色 "padding-left:20px;"       //内边距-字体缩进
                                    "background-color: rgb(255, 255, 255);" //背景颜色
                                    "border:2px solid #d9a7c7;border-radius:15px;");//边框粗细-颜色-圆角设置
    this->setStyleSheet("QMainWindow{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #d9a7c7, stop:1 #fffcdc)};");
    ui->pushButton->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑';color: rgb(255,255,255);background-color: #ffb8b9;"
                                    "border: none;border-radius:15px;}"
                                    "QPushButton:hover{background-color: #ffb8b9;}"//hover
                                    "QPushButton:pressed{background-color: #ff9499;}");//pressed
    ui->label_4->setStyleSheet("border:2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(217, 167, 199, 255), stop:1 rgba(255, 255, 255, 255));image:url(:/picture/header.png)};");
    ui->label->setStyleSheet("color: #878787;font: 25 10pt '微软雅黑 Light';background: transparent;");
    ui->label_2->setStyleSheet("color:#878787;font: 25 10pt '微软雅黑 Light';background: transparent;");
    ui->label_3->setStyleSheet("background: transparent;");
    //qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(217, 167, 199, 255), stop:1 rgba(255, 255, 255, 255))
}

WLogin::~WLogin()
{
    delete ui;
}

void WLogin::on_pushButton_clicked()
{
    id=ui->lineEdit->text();
    pwd=ui->lineEdit_2->text();

    QByteArray pswd;
    pswd.append(pwd);
    QByteArray hash = QCryptographicHash::hash(pswd, QCryptographicHash::Md5);
    QString strMD5 = hash.toHex();
    pwd=strMD5;
    Send_log_id();
    //p_mwd->show();
    //this->hide();
}

void WLogin::Send_log_id()
{
    emit Send_uid_to_mwd(id,pwd);
}


