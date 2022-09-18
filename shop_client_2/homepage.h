#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <QMessageBox>
#include <string>
#include "wgood.h"
#include "wtrolley.h"
#include "wuser.h"

using namespace std;

namespace Ui {
class Homepage;
}

class Homepage : public QMainWindow
{
    Q_OBJECT

public:
    explicit Homepage(string uid,QWidget *parent = nullptr);
    ~Homepage();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    string id;
    Ui::Homepage *ui;
    WGood* wgood;
    WTrolley* wtrollry;
    WUser* wuser;
    QImage* head_picture=new QImage;
    QImage* good_btn_pic=new QImage;
};

#endif // HOMEPAGE_H
