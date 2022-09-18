#include "wireshark.h"
#include "ui_wireshark.h"

Wireshark::Wireshark(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Wireshark)
{
    ui->setupUi(this);
}

Wireshark::~Wireshark()
{
    delete ui;
}
