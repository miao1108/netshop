#ifndef PACKAGE_SHOW_H
#define PACKAGE_SHOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QScrollArea>
#include "interface.h"
namespace Ui {
class package_show;
}

class package_show : public QMainWindow
{
    Q_OBJECT

public:
    explicit package_show(QWidget *parent = nullptr);
    ~package_show();
    QStandardItemModel *model;
    int now_row=0;
    int now_col=0;
    QVector<QVector<QString>> detailed_info;
    void update_tableview();
private slots:
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::package_show *ui;
    Style_sheet ss;
    const QString txtedt_style="font:  100 30 14px 'Consolas';"
                               "color:rgb(40,40,0);"
                               "border:3px solid rgba(233, 121, 101, 255);"
                               "border-left-style:none;"
                               "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(255, 249, 250), stop:1 rgb(255, 254, 248));";
    const QString label_style="background: qlineargradient(spread:pad, x1:0, y1:0.034, x2:1, y2:1, stop:0 rgba(255, 181, 79, 255), stop:1 rgba(233, 121, 101, 255));"
                              "color:rgb(250,250,250);"
                              "border:3px solid rgba(233, 121, 101, 255);"
                              "border-right-style:none;";
};

#endif // PACKAGE_SHOW_H
