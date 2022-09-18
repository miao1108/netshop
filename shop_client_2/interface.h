#ifndef INTERFACE_H
#define INTERFACE_H

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QMainWindow>
#include <QLabel>
#include <QTime>
#include <QTimer>
#include <QString>

class Style_sheet
{
public:
    QString color_btn_style;

    QString write_btn_style;

    QString line_edit_style;

    QString good_name;

    QString good_price;

    QString good_discountprice;

    QString good_inventory;

    QString general_txt;

    QString good_num;

    QString notice;

    Style_sheet();
};


QPixmap PixmapToRound(QPixmap &src, int radius);
QPixmap getRoundRectPixmap(QPixmap srcPixMap,const QSize & size, int radius);
QPixmap Roundpix(const QPixmap& img_in, int radius);

QPixmap scaledPixmap(const QPixmap &src, int width, int height);
QPixmap generatePixmap(const QPixmap &src, const int &radius);

//消息提示
//void Notice_box(QString qstr);
void Notice_box(QMainWindow *pmain,QString nts);

#endif // INTERFACE_H

