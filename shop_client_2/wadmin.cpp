#include "wadmin.h"
#include "ui_wadmin.h"

WAdmin::WAdmin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WAdmin)
{
    ui->setupUi(this);
}

WAdmin::~WAdmin()
{
    delete ui;
}
