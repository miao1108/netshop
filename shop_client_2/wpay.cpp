#include "wpay.h"
#include "ui_wpay.h"

WPay::WPay(QString uid,QVector<QVector<QString>> buffer,int mode,QMainWindow *p,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WPay)
{
    ui->setupUi(this);
    id=uid;
    pay_mode=mode;
    prt=p;
    under_order=buffer;
    Set_pay_style();
    //显示已选商品
    Show_order();
    if(mode)
    {
        //connect(this,SIGNAL(Trolley_order(QByteArray)),prt,SLOT(Trolley_order_pay(QByteArray)));//商品页直接购买
        connect(this,SIGNAL(Change_trolley_pay()),prt,SLOT(Change_trolley_pay_show()));
    }
    if(mode)
        emit Change_trolley_pay();
}

WPay::~WPay()
{
    delete ui;
}

//确认支付
void WPay::on_pushButton_clicked()
{
    if(pay_mode)//购物车下单
    {
        QVector<QString> select_sql;
        QVector<QString> update_sql;
        QVector<QString> record_sql;
        QVector<QString> delete_sql;
        QString sel_sql="select Gnum from good where Gname='%1'";
        QString upd_sql="update good set Gnum=Gnum - %1 where Gname='%2'";
        QString inv_sql="insert into sale(Mid,Gname,Anum,Atime) values(%1,'%2',%3,'%4')";
        QString del_cart=QString("delete from cart where uid=%1").arg(id);
        //这三行是提供sql语句模板，逻辑是，先查询商品表，比对数量是否正确，然后再更新商品表数量，最后添加销售记录，如果有购物车表的话，需要再加一条语句，就是删除购物车表项

        QDateTime dateTime= QDateTime::currentDateTime();
        QString date_str = dateTime .toString("yyyy-MM-dd hh:mm:ss");

        //这下面是根据缓存生成对应的sql语句
        for(int i=0;i<under_order.size();i++)
        {
            QString sel=sel_sql.arg(under_order[i][0]);
            QString upd=upd_sql.arg(under_order[i][1]).arg(under_order[i][0]);
            QString inv=inv_sql.arg(id).arg(under_order[i][0]).arg(under_order[i][1]).arg(date_str);
            //QString del=del_sql.arg(under_order[i][0]).arg(id);
            select_sql.push_back(sel);
            update_sql.push_back(upd);
            record_sql.push_back(inv);
           // delete_sql.push_back(del);
        }
        QByteArray send_data;
        QBuffer buffer(&send_data);
        buffer.open(QIODevice::WriteOnly);
        QDataStream out(&buffer);
        //发送缓存加sql语句
        out<<under_order<<select_sql<<update_sql<<record_sql<<del_cart;
        buffer.close();
        emit Trolley_order(send_data);

        //QVector<QString> pac_show_vec = package_message_DES(i,y,send_data,socket,DES_key,rsa_to_server->n,rsa_to_server->d);//数字签名DES加密
        //QVector<QString> pac_show_vec = package_message_DES(65,11,send_data,DES_key,p->n,p->d);//数字签名DES加密

        this->close();
    }
    else//商品页下单
    {
        QVector<QString> select_sql;
        QVector<QString> update_sql;
        QVector<QString> record_sql;
        QString sel_sql="select Gnum from good where Gname='%1'";
        QString upd_sql="update good set Gnum=Gnum - %1 where Gname='%2'";
        QString inv_sql="insert into sale(Mid,Gname,Anum,Atime) values(%1,'%2',%3,'%4')";

        QDateTime dateTime= QDateTime::currentDateTime();
        QString date_str = dateTime .toString("yyyy-MM-dd hh:mm:ss");
        for(int i=0;i<under_order.size();i++)
        {
           QString sel=sel_sql.arg(under_order[i][0]);
           QString upd=upd_sql.arg(under_order[i][1]).arg(under_order[i][0]);
           QString inv=inv_sql.arg(id).arg(under_order[i][0]).arg(under_order[i][1]).arg(date_str);
           select_sql.push_back(sel);
           update_sql.push_back(upd);
           record_sql.push_back(inv);
        }

        QByteArray send_data;
        QBuffer buffer(&send_data);
        buffer.open(QIODevice::WriteOnly);
        QDataStream out(&buffer);
        out<<under_order<<select_sql<<update_sql<<record_sql;
        buffer.close();

        emit Goods_order(send_data);
        this->close();
    }
//       QVector<QString> pac_show_vec = package_message_DES(i,y,send_data,socket,DES_key,rsa_to_server->n,rsa_to_server->d);//数字签名DES加密
//       QVector<QString> pac_show_vec = package_message_DES(65,10,send_data,socket,DES_key,rsa_to_server->n,rsa_to_server->d);//数字签名DES加密
}

//关闭窗口
void WPay::on_pushButton_2_clicked()
{
    this->close();
}

//显示已选商品
void WPay::Show_order()
{
    res->clear();
    QStandardItem *item;
    //QStandardItemModel *res=new QStandardItemModel();
    int num=0,price=0,row=-1,col=0;
    for(int i=0;i<under_order.size();i++)//商品
    {
        if(under_order[i][1]=="0")
            continue;
        else
        {
            row++;
            for(int j=0;j<under_order[i].size();j++)//属性
            {
                 if(j==2) continue;
                 if(j==1)num=under_order[i][j].toDouble();
                 if(j==3)price=under_order[i][j].toDouble();
                 item=new QStandardItem(under_order[i][j]);
                 res->setItem(row,col,item);
                 res->item(row,col)->setTextAlignment(Qt::AlignCenter);
                 col++;
            }
            col=0;
        }
        money+=(num*price);
    }
    res->setColumnCount(3);
    res->setHeaderData(0,Qt::Horizontal,QString("商品名"));
    res->setHeaderData(1,Qt::Horizontal,QString("数量"));
    res->setHeaderData(2,Qt::Horizontal,QString("单价"));
    ui->tableView->setModel(res);
    ui->pushButton->setText("支付"+QString::number(money,'f',2)+"元");
    //ui->label_4->setText(QString::number(money));

}

void WPay::Set_pay_style()
{
    this->setWindowTitle("支付");
    this->setStyleSheet("QMainWindow{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(255, 247, 248), stop:1 rgb(255, 254, 245))}");

    ui->label_3->setStyleSheet("font: 15px '汉仪正圆-65W';color:#94592f");
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自动调整列宽
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section, QTableCornerButton::section {padding: 1px;border: none;background-color:#ff8469;color:#ffffff;font:12px '微软雅黑'}");
    ui->tableView->setStyleSheet("QTableView::item::selected{""background:rgba(233, 121, 101, 255);}");
    ui->pushButton->setStyleSheet(ss.color_btn_style);
    ui->pushButton_2->setStyleSheet(ss.write_btn_style);


}
