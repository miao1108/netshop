#ifndef MYSOCKET_H
#define MYSOCKET_H
#include<QTime>
#include <QTcpSocket>
#include"network.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include<QBuffer>
#include"DES.h"

#include <qdebug.h>
#include <QMessageBox>
#include<QDate>
#include<QSqlTableModel>
#include <QStandardItemModel>
#include <QSqlRecord>
#include <QVector>
#include <string>
#include "imagehandle.h"
#include<QMutex>
static char TGSkey_n[]="c432a812fda6c2b150efa577acf70721de1799acf9e789d054ceca82cc3af6f211ab3cf458b4016c32fc286c6d27d05baa5eccef0c594c2cb3d6eeca57cdc901911f4a990d062e2206a323bd0a3650607a9093cec0cc6eeb47b21316d643708f89e76c4a739f027ac6f93565dd68ffe4a5480f79b9163935238d6b05113003";
static char TGSkey_d[]="26401e067dcf4a3bd9a4af24d2e2e250170590df69655bb7f320d1d8884aa2a4750f6675d8e95731aa0b01328a616dbcc14d7434d2ba822060fb71589f9fae1fc1079cbe9d0a0aafcc378cd9edd29dbb151c3df3326207f77b153dfb7cbc8dba7e7f805c3cbe8d2e97b6770b8e3c2f718b1d53ec0f4348e197a5f8f431ef19";
static char Vkey_n[]="a42ec64809f7e777bae17b10d33bf5df4bb605d6124cda58fbe0d7322342bd3af61810e2ff62b33b0bcf09c5dd155d2a6e2fabd4ee655f78ad2cf19b0ada671893a843fe307e6eb9df63be5853dc97a48aeaf19996b05e471fb14bd0ee2a9f3d0d521c158d627855397900c218b78a6561228cf3064054de70a2b772e53d4f";
static char Vkey_d[]="54ac46d84a43cedd41c51dd9ec86c4e41740d5bbb48e6c216e2d63ee589f07c68bba97a2ea40f5dc62d7b0db998c7612f6e6a100dcbded41e85600788c5d5ca2ce5ec34c63b8644ddca22fe239a93bd93857687cce033315bf1ae523f1c3a2279f81c10b85960c691d706a1c210d97ae3ee5536f41412ea6f08c763fdbf269";
extern QMutex mutexx;
extern QMutex mutexx_t;
class MYsocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MYsocket(int sockDesc,QObject *parent = nullptr);
    QTcpSocket* socket;

    key_pair * p=new key_pair();//自己的密钥
    QStandardItemModel* good_model;

    QVector<QVector<QString>> send_table;//发送给用户的数据库表格数据

    QSqlDatabase db;
    QSqlTableModel* model;
    QSqlQuery* query;

    QByteArray DES_key_to_client;//与client的DES密钥

    QByteArray m_buffer;//缓存，不要管

    Message recv_mes;//收到的报文

    void message_handel();//处理收到的报文，并执行相应的操作
    bool modify_db(QString txt);//增删改
    QSqlQuery query_db(QString txt) ;//查询
    QSqlTableModel* model_db(QString txt) ;//查询
    void sql_to_table(QString txt);//sqltablemodel转QStandardItemModel
    QByteArray g_Key_cv;
    int g_idc;
    char* g_Ckey_n;


    //**********Leeee-客户端**********//
    void V_handel();//认证处理
    void Login_Check();//登录认证
    void Inquire_goods();//查询一页商品
    void Page_jump();//查询商品总数
    void Inquire_spcf_goods();//查询指定商品（搜索框）
    void Inquire_trolley();//查询-购物车页
    void addcart_handle();//加入购物车

    //*****220520 Leeee****//
    void Page_jump_t();//购物车跳转页面
    void Update_trolley_num();//更改购物车内商品数量、清理购物车
    void Goods_order();//直接购买商品
    void Trolley_order();//购物车下单
    void Inquire_all_trolley();

    void userinf_handel();//查询个人信息
    void modinf_handel();//修改个人信息
    void modpsw_handel();//修改密码
    void VIP_handel();//申请vip


    //----------廷恺-------------//
    QVector<QByteArray> send_img_byte;
    void inquire_sql_to_manager();//打包数据给管理员
    void delete_good();
    void manager_pic();
    void insert_good_table();

private slots:
    void socket_Read_Data();

private:
    int m_sockDesc;
};

struct Ticket {
QByteArray Key_cn;//c与tgs的des密钥
int IDc;
int ID;
QString TS;
int lifetime;//hour
};
struct Authenticator{
    int IDc;
    QString TS;
};
struct CtoV{
   char* m_ticket_v;
   QByteArray authenticator;
};
struct VtoC{
   QString TS5;
};

#endif // MYSOCKET_H
