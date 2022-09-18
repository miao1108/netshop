#include "wtrolley.h"
#include "ui_wtrolley.h"

WTrolley::WTrolley(QString uid,QMainWindow *p, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WTrolley)
{
    ui->setupUi(this);
    id=uid;
    pmwd=p;
    Set_trolley_style();
    tmp_infm.push_back("###");
    add_buffer_t.clear();
    //add_buffer_t.push_back(tmp_infm);
    //ui->statusbar->hide();//隐藏状态栏
    connect(pmwd,SIGNAL(Transport_trolley(int,int,QVector<QVector<QString>>,QVector<QByteArray>)),
            this,SLOT(Show_trolley_1(int,int,QVector<QVector<QString>>,QVector<QByteArray>)));
    connect(pmwd,SIGNAL(Control_page_turn_t(int,int)),this,SLOT(Control_page_turn_btn_t(int,int)));
    connect(this,SIGNAL(Control_page_turn_t(int,int)),this,SLOT(Control_page_turn_btn_t(int,int)));

    //connect(wpay,SIGNAL(Change_trolley_pay()),this,SLOT(Change_trolley_pay_show()));

    QPushButton *btn1[6]={ui->pushButton_12,ui->pushButton_22,ui->pushButton_32,ui->pushButton_42,ui->pushButton_52,ui->pushButton_62};
    QPushButton *btn2[6]={ui->pushButton_11,ui->pushButton_21,ui->pushButton_31,ui->pushButton_41,ui->pushButton_51,ui->pushButton_61};
    for(int i=0;i<6;i++)
    {
        add_btn[i]=btn1[i];
        sub_btn[i]=btn2[i];
        connect(add_btn[i],SIGNAL(clicked(bool)),SLOT(add_num()));
        connect(sub_btn[i],SIGNAL(clicked(bool)),SLOT(sub_num()));
    }
}

WTrolley::~WTrolley()
{
    delete ui;
}

//void WTrolley::Show_trolley_1(int,int,QVector<QVector<QString>>,QVector<QByteArray>)
//{

//}

//显示商品
void WTrolley::Show_trolley_1(int loc,int goods_num,QVector<QVector<QString>> goods_infm,QVector<QByteArray> goods_img)//显示商品
{
    //记录当页信息
    show_infm=goods_infm;
    //页码显示和按钮
    emit Control_page_turn_t(loc,goods_num);
    //搜索框、输入页码清空
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    //前n个商品进行显示
    for(int i=0;i<6;i++)
    {
        QLabel *label = this->findChild<QLabel *>("label_"+QString::number(i+1));
        //label->setStyleSheet("QLabel{background-color:rgb(255,255,255);}");
    }
    for(int i=0;i<goods_num;i++)//每个商品
    {
        QLabel *label_name = this->findChild<QLabel *>("label_"+QString::number(i+1)+"_0");

//        QVector<QString> temp;
//        temp.push_back(goods_infm[i][0]);
//        temp.push_back(goods_infm[i][4]);
//        temp.push_back(goods_infm[i][2]);
//        temp.push_back(goods_infm[i][3]);
//        add_buffer_t.push_back(temp);

        for(int j=0;j<6;j++)//每种属性:0名、1库、2原、3现、4数、5种
        {
            //显示信息的控件
            QLabel *label = this->findChild<QLabel *>("label_"+QString::number(i+1)+"_"+QString::number(j));
            if(j==5)//“库存件数”
            {
                label->raise();
                break;
            }
            //显示信息
            QString value;
            if(j<5) value=goods_infm[i][j];
            if(0)//显示完整信息
            {
//            if(j==1) value="库存 "+value+" 件";
//            else if(j==2) value=" "+value+" ";
//            else if(j==3) value="¥ "+value;
            ////////////////////////名、存、原、现、数
//            else if(j==4)
//            {
//                //value="0";
//                for (int p=0; p<add_buffer_t.count(); p++)
//                {
//                     if(add_buffer_t[p][0]==label_name->text())
//                     {
//                         value=add_buffer_t[p][1];
//                         break;
//                     }
//                     else
//                         value="0";
//                }
//            }
            ////////////////////////
            }

            for(int k=0;k<2;k++)
            {
                QPushButton *btn=this->findChild<QPushButton *>("pushButton_"+QString::number(i+1)+QString::number(k+1));
                btn->raise();
            }
            if(value=="")value="0";
            label->setText(value);
            label->raise();
        }
        //显示图片
        //QImage img=getImage(goods_img[i]);
        //QLabel *label_img = this->findChild<QLabel *>("label"+QString::number(i+1));
        //label_img->setPixmap(QPixmap::fromImage(img));

        QLabel *label_img = this->findChild<QLabel *>("label"+QString::number(i+1));
        QImage head_img=getImage(goods_img[i]);
        QPixmap pix_img=QPixmap::fromImage(head_img);

        pix_img=getRoundRectPixmap(pix_img, pix_img.size(), 10);
        label_img->setPixmap(pix_img);

        label_img->setScaledContents(true);//铺满label
        label_img->raise();
    }
    //后6-n个商品显示空白
    for(int i=0;i<6-goods_num;i++)
    {
        QLabel *label = this->findChild<QLabel *>("label_"+QString::number(goods_num+i+1));
        label->raise();
    }
    int a=0;
}

//刷新
void WTrolley::on_pushButton_clicked()
{
    QPalette patlette;
    patlette.setColor(QPalette::ButtonText,  QColor(136, 136, 136));
    ui->pushButton_3->setPalette(patlette);
    ui->pushButton_4->setPalette(patlette);
    ui->pushButton_5->setPalette(patlette);
    ui->pushButton_6->setPalette(patlette);
    ui->pushButton_7->setPalette(patlette);
    ui->pushButton_8->setPalette(patlette);
    emit Change_view_mode_t(1);
    emit Refresh_goods_list_t(6);
}

//跳转到指定页
void WTrolley::on_pushButton_25_clicked()
{
    QString page=ui->lineEdit_2->text();
    int n=page.toInt();//获取目标页码
    //if(!(n>0))
    //{
        //QMessageBox::information(this,"notice"," 输入页码需要为正整数");
        ui->lineEdit_2->clear();
        //return;
    //}
    emit Get_goods_number_t(n);
}

//搜索
void WTrolley::on_pushButton_2_clicked()
{
    QPalette patlette;
    patlette.setColor(QPalette::ButtonText,  QColor(136, 136, 136));
    ui->pushButton_3->setPalette(patlette);
    ui->pushButton_4->setPalette(patlette);
    ui->pushButton_5->setPalette(patlette);
    ui->pushButton_6->setPalette(patlette);
    ui->pushButton_7->setPalette(patlette);
    ui->pushButton_8->setPalette(patlette);
    emit Change_view_mode_t(2);
    QString gname=ui->lineEdit->text();
    if(gname=="") return;
    emit Search_goods_list_t(gname);
}

//种类1
void WTrolley::on_pushButton_3_clicked()
{
    QPalette patlette;
    patlette.setColor(QPalette::ButtonText, QColor(247,121,125));  // 设置button 文本颜色红色
    ui->pushButton_3->setPalette(patlette);
    patlette.setColor(QPalette::ButtonText,  QColor(136, 136, 136));
    ui->pushButton_4->setPalette(patlette);
    ui->pushButton_5->setPalette(patlette);
    ui->pushButton_6->setPalette(patlette);
    ui->pushButton_7->setPalette(patlette);
    ui->pushButton_8->setPalette(patlette);
    emit Change_view_mode_t(3);
    emit Search_species_list_t(1);
}

//种类2
void WTrolley::on_pushButton_4_clicked()
{
    QPalette patlette;
    patlette.setColor(QPalette::ButtonText, QColor(247,121,125));  // 设置button 文本颜色红色
    ui->pushButton_4->setPalette(patlette);
    patlette.setColor(QPalette::ButtonText,  QColor(136, 136, 136));
    ui->pushButton_3->setPalette(patlette);
    ui->pushButton_5->setPalette(patlette);
    ui->pushButton_6->setPalette(patlette);
    ui->pushButton_7->setPalette(patlette);
    ui->pushButton_8->setPalette(patlette);
    emit Change_view_mode_t(4);
    emit Search_species_list_t(2);
}

//种类3
void WTrolley::on_pushButton_5_clicked()
{
    QPalette patlette;
    patlette.setColor(QPalette::ButtonText, QColor(247,121,125));  // 设置button 文本颜色红色
    ui->pushButton_5->setPalette(patlette);
    patlette.setColor(QPalette::ButtonText,  QColor(136, 136, 136));
    ui->pushButton_4->setPalette(patlette);
    ui->pushButton_3->setPalette(patlette);
    ui->pushButton_6->setPalette(patlette);
    ui->pushButton_7->setPalette(patlette);
    ui->pushButton_8->setPalette(patlette);
    emit Change_view_mode_t(5);
    emit Search_species_list_t(3);
}

//种类4
void WTrolley::on_pushButton_6_clicked()
{
    QPalette patlette;
    patlette.setColor(QPalette::ButtonText, QColor(247,121,125));  // 设置button 文本颜色红色
    ui->pushButton_6->setPalette(patlette);
    patlette.setColor(QPalette::ButtonText,  QColor(136, 136, 136));
    ui->pushButton_4->setPalette(patlette);
    ui->pushButton_5->setPalette(patlette);
    ui->pushButton_3->setPalette(patlette);
    ui->pushButton_7->setPalette(patlette);
    ui->pushButton_8->setPalette(patlette);
    emit Change_view_mode_t(6);
    emit Search_species_list_t(4);
}

//种类5
void WTrolley::on_pushButton_8_clicked()
{
    QPalette patlette;
    patlette.setColor(QPalette::ButtonText, QColor(247,121,125));  // 设置button 文本颜色红色
    ui->pushButton_8->setPalette(patlette);
    patlette.setColor(QPalette::ButtonText,  QColor(136, 136, 136));
    ui->pushButton_4->setPalette(patlette);
    ui->pushButton_5->setPalette(patlette);
    ui->pushButton_6->setPalette(patlette);
    ui->pushButton_3->setPalette(patlette);
    ui->pushButton_7->setPalette(patlette);
    emit Change_view_mode_t(8);
    emit Search_species_list_t(6);
}

//种类6
void WTrolley::on_pushButton_7_clicked()
{
    QPalette patlette;
    patlette.setColor(QPalette::ButtonText, QColor(247,121,125));  // 设置button 文本颜色红色
    ui->pushButton_7->setPalette(patlette);
    patlette.setColor(QPalette::ButtonText,  QColor(136, 136, 136));
    ui->pushButton_4->setPalette(patlette);
    ui->pushButton_5->setPalette(patlette);
    ui->pushButton_6->setPalette(patlette);
    ui->pushButton_8->setPalette(patlette);
    ui->pushButton_3->setPalette(patlette);
    emit Change_view_mode_t(7);
    emit Search_species_list_t(5);
}

//增加数量
void WTrolley::add_num()
{
    int i=0;
    QPushButton *pbtn=qobject_cast<QPushButton *>(sender());
    for(i=0;i<6;i++)
    {
        if(add_btn[i]==pbtn)
            break;
    }
    QLabel *label_name = this->findChild<QLabel *>("label_"+QString::number(i+1)+"_0");
    QLabel *label_repertory= this->findChild<QLabel *>("label_"+QString::number(i+1)+"_1");
    QLabel *label_price = this->findChild<QLabel *>("label_"+QString::number(i+1)+"_2");
    QLabel *label_discount = this->findChild<QLabel *>("label_"+QString::number(i+1)+"_3");
    QLabel *label_num = this->findChild<QLabel *>("label_"+QString::number(i+1)+"_4");

    int n=label_num->text().toInt();
    if((label_repertory->text()).toInt()>n)n++;
    label_num->setText(QString::number(n));
    //修改购物车商品的加购数量
    QString sql="UPDATE Cart SET Snum="+QString::number(n)+" WHERE Uid="+id+" AND Gname=\'"+label_name->text()+"\'";
    emit Update_trolley_num(sql);

    int find=0;
    for (int j=0; j<add_buffer_t.count(); j++)//
    {
         if(add_buffer_t[j][0]==label_name->text())//
         {
             find=1;
             add_buffer_t[j][1]=QString::number(n);//
             //修改购物车商品的加购数量
             //QString sql="UPDATE Cart SET Snum="+QString::number(n)+" WHERE Uid="+id+" AND Gname=\'"+label_name->text()+"\'";
             //emit Update_trolley_num(sql);
             break;
         }
    }
    if(find==0&&n>0)
    {
        QVector<QString> temp;
        temp<<label_name->text()<<label_num->text()<<label_price->text()<<label_discount->text();
        add_buffer_t.push_back(temp);//
        //添加到购物车
    }
    int a=0;
}

//减少数量
void WTrolley::sub_num()
{
    int i=0;
    QPushButton *pbtn=qobject_cast<QPushButton *>(sender());
    for(i=0;i<6;i++)
    {
        if(sub_btn[i]==pbtn)
            break;
    }
    QLabel *label_name = this->findChild<QLabel *>("label_"+QString::number(i+1)+"_0");
    QLabel *label_price = this->findChild<QLabel *>("label_"+QString::number(i+1)+"_2");
    QLabel *label_discount = this->findChild<QLabel *>("label_"+QString::number(i+1)+"_3");
    QLabel *label_num = this->findChild<QLabel *>("label_"+QString::number(i+1)+"_4");
    int n=label_num->text().toInt();
    if(n>0)n--;
    label_num->setText(QString::number(n));
    //修改购物车商品的加购数量
    QString sql="UPDATE Cart SET Snum="+QString::number(n)+" WHERE Uid="+id+" AND Gname=\'"+label_name->text()+"\'";
    emit Update_trolley_num(sql);

    for (int j=0; j<add_buffer_t.count(); j++)//
    {
         if(add_buffer_t[j][0]==label_name->text())//
         {
             add_buffer_t[j][1]=QString::number(n);//
             //修改购物车商品的加购数量
             //QString sql="UPDATE Cart SET Snum="+QString::number(n)+" WHERE Uid="+id+"AND Gname=\'"+label_name->text()+"\'";
             //emit Update_trolley_num(sql);
             break;
         }
    }
}

//控制页码和加减按钮使能
void WTrolley::Control_page_turn_btn_t(int goods_loc,int goods_num)
{
    //qDebug()<<goods_loc;
    int page=goods_loc/6;
    if(goods_loc%6) page++;
    ui->label_page->setText(QString::number(page));

    if(goods_loc<=6)//控制左翻
        ui->pushButton_24->setEnabled(false);
    else
        ui->pushButton_24->setEnabled(true);

    if(goods_num<6)//控制右翻
        ui->pushButton_23->setEnabled(false);
    else
        ui->pushButton_23->setEnabled(true);
}

//左翻
void WTrolley::on_pushButton_24_clicked()
{
    emit Refresh_goods_list_t(12);
    ui->pushButton_24->setEnabled(true);
}

//右翻
void WTrolley::on_pushButton_23_clicked()
{
    emit Refresh_goods_list_t(0);
    ui->pushButton_24->setEnabled(true);
}

//清理购物车
void WTrolley::on_pushButton_add_to_trolley_clicked()
{
    //label_1_0, 4
    QString sql="DELETE FROM Cart WHERE uid="+id+" AND Snum=0";
    emit Update_trolley_num(sql);
}

//购物车下单
void WTrolley::on_pushButton_purchase_clicked()
{
    connect(pmwd,SIGNAL(Send_trolley_buffer(QVector<QVector<QString>>)),
            this,SLOT(Get_buffer(QVector<QVector<QString>>)));
    emit Inquire_all_trolley();

//    wpay=new WPay(id,add_buffer_t,1);
//    connect(wpay,SIGNAL(Trolley_order(QByteArray)),pmwd,SLOT(Trolley_order_pay(QByteArray)));//商品页直接购买
//    wpay->show();
}

//更新缓存，弹出付款界面
void WTrolley::Get_buffer(QVector<QVector<QString>> new_buf)
{
    connect(pmwd,SIGNAL(Send_trolley_buffer(QVector<QVector<QString>>)),
            this,SLOT(Get_buffer(QVector<QVector<QString>>)));
    add_buffer_t=new_buf;
    wpay=new WPay(id,add_buffer_t,1,this);
    connect(wpay,SIGNAL(Trolley_order(QByteArray)),pmwd,SLOT(Trolley_order_pay(QByteArray)));//商品页直接购买
    //connect(wpay,SIGNAL(Change_trolley_pay()),this,SLOT(Change_trolley_pay_show()));
    wpay->show();
    int a=1;
}

void WTrolley::Change_trolley_pay_show()
{
    trolley_pay=false;
}

void WTrolley::Set_trolley_style()
{
    const QString good_style="QLabel{"
                                    "background-color:rgb(255,255,255);"
                                    "border-radius: 10px;"
                                    "border:none;}";//1px solid #a2a2a2;}";

    const QString img_style="QLabel{"
                                    "border-radius: 10px;"
                                    "border:1px solid #ffb70f;}";

    const QString as_btn_style="QPushButton{"
                                    "border-radius: 10px;"
                                    "background-color: rgb(255, 255, 255);"
                                    "border:1px solid #dfdfdf;}"
                               "QPushButton:pressed {"
                                    "background-color: #dfdfdf;}";

    for(int i=0;i<6;i++)
    {
        QLabel *label_good = this->findChild<QLabel *>("label_"+QString::number(i+1));
        QGraphicsDropShadowEffect *shadow_1=new QGraphicsDropShadowEffect();
        label_good->setStyleSheet(good_style);
        shadow_1->setBlurRadius(10);
        shadow_1->setColor(QColor(0,0,0,50));
        shadow_1->setOffset(0,0);
        label_good->setStyleSheet(good_style);
        label_good->setGraphicsEffect(shadow_1);
        QLabel *label_img = this->findChild<QLabel *>("label"+QString::number(i+1));
        label_img->setStyleSheet(img_style);
        QPushButton *psbtn_sub_btn=this->findChild<QPushButton *>("pushButton_"+QString::number(i+1)+"1");
        QPushButton *psbtn_add_btn=this->findChild<QPushButton *>("pushButton_"+QString::number(i+1)+"2");
        psbtn_sub_btn->setStyleSheet(as_btn_style);
        psbtn_add_btn->setStyleSheet(as_btn_style);

        for(int j=0;j<6;j++)
        {
            QLabel *label_txt= this->findChild<QLabel *>("label_"+QString::number(i+1)+"_"+QString::number(j));
            if(j==0) label_txt->setStyleSheet(ss.good_name);
            else if(j==2) label_txt->setStyleSheet(ss.good_price);
            else if(j==3) label_txt->setStyleSheet(ss.good_discountprice);
            else if(j==4) label_txt->setStyleSheet(ss.good_num);
            else  label_txt->setStyleSheet(ss.good_inventory);
        }

        QPushButton *spices_btn=this->findChild<QPushButton *>("pushButton_"+QString::number(i+3));
        QPalette patlette;
        patlette.setColor(QPalette::ButtonText, QColor(136, 136, 136));  // 设置button 文本颜色红色
        spices_btn->setPalette(patlette);
    }

    this->setStyleSheet("QMainWindow{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(255, 247, 248), stop:1 rgb(255, 254, 245))}");

    ui->pushButton_purchase->setStyleSheet(ss.color_btn_style);
    ui->pushButton_add_to_trolley->setStyleSheet(ss.color_btn_style);
    ui->pushButton_25->setStyleSheet(ss.write_btn_style);
    ui->pushButton->setStyleSheet("QPushButton{image:url(\":/picture/update.png\");border-radius:10px;}"
                                  "QPushButton:pressed{image:url(\":/picture/update_clicked.png\");}");
    ui->pushButton_2->setStyleSheet("QPushButton{image:url(\":/picture/search.png\");border-radius:10px;}"
                                    "QPushButton:pressed{image:url(\":/picture/search_clicked.png\");}");
    ui->pushButton_23->setStyleSheet("QPushButton{border-radius:10px;image:url(\":/picture/right_change.png\");background-color:qlineargradient(spread:reflect, x1:0.0168539, y1:0, x2:0.478, y2:0.493955, stop:0 rgba(238, 222, 220, 105), stop:1 rgba(255, 255, 255, 255));}"
                                     "QPushButton:pressed{border-radius:10px;image:url(\":/picture/right_clicked.png\")}");
    ui->pushButton_24->setStyleSheet("QPushButton{border-radius:10px;image:url(\":/picture/left_change.png\");background-color:qlineargradient(spread:reflect, x1:0.0168539, y1:0, x2:0.478, y2:0.493955, stop:0 rgba(238, 222, 220, 105), stop:1 rgba(255, 255, 255, 255));}"
                                     "QPushButton:pressed{border-radius:10px;image:url(\":/picture/left_clicked.png\")}");
    ui->lineEdit->setStyleSheet("border-radius:12px;background-color:rgb(255, 255, 255);border:1px solid qlineargradient(spread:pad, x1:0, y1:0.034, x2:0.73, y2:0.756, stop:0 rgba(255, 179, 79, 100), stop:1 rgba(255, 133, 111, 100));");
    ui->lineEdit_2->setStyleSheet("border-radius: 13px;border:1px solid rgba(255,152,95,255);color:rgb(20,20,20);font: bold 12px;font-family: Segoe UI Historic;");
    ui->label_page->setStyleSheet("color:rgb(20,20,20);font: bold 12px;font-family: Segoe UI Historic;");

}
