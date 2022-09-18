#ifndef VIPREGIST_H
#define VIPREGIST_H
#include"kerberos.h"
#include <QDialog>
#include <QPushButton>
#include "interface.h"

namespace Ui {
class VIPregist;
}

class VIPregist : public QDialog
{
    Q_OBJECT

public:
    explicit VIPregist(Pubdata alldata,QWidget *parent = nullptr);
    Pubdata g_alldata;
    ~VIPregist();

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_buttonBox_accepted();

    void on_radioButton_3_clicked();

private:
    Ui::VIPregist *ui;
    Style_sheet ss;
};

#endif // VIPREGIST_H
