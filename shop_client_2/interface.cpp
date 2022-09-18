#include "interface.h"


QPixmap PixmapToRound(QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }

    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);

    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

QPixmap PixmapToRound_angle(QPixmap &src, int radius)//没有
{
    if (src.isNull()) {
        return QPixmap();
    }

    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);

    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

QPixmap getRoundRectPixmap(QPixmap srcPixMap,const QSize & size, int radius)//没用
{
    //不处理空数据或者错误数据
    if (srcPixMap.isNull()) {
        return srcPixMap;
    }

    //获取图片尺寸
    int imageWidth = size.width();
    int imageHeight = size.height();

    //处理大尺寸的图片,保证图片显示区域完整
    QPixmap newPixMap = srcPixMap.scaled(imageWidth, (imageHeight == 0 ? imageWidth : imageHeight),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPixmap destImage(imageWidth, imageHeight);
    destImage.fill(Qt::transparent);
    QPainter painter(&destImage);
    // 抗锯齿
    painter.setRenderHints(QPainter::Antialiasing, true);
    // 图片平滑处理
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    // 将图片裁剪为圆角
    QPainterPath path;
    QRect rect(0, 0, imageWidth, imageHeight);
    path.addRoundedRect(rect, radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, imageWidth, imageHeight, newPixMap);
    return destImage;
}

QPixmap Roundpix(const QPixmap& img_in, int radius)//没用
{
    if (img_in.isNull())
    {
        return QPixmap();
    }
    QSize size(img_in.size());
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(mask.rect(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(mask.rect(), radius, radius);
    QPixmap image = img_in;// .scaled(size);
    image.setMask(mask);
    return image;
}

QPixmap scaledPixmap(const QPixmap &src, int width, int height) {
    return src.scaled(width, (height == 0 ? width : height),
            Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QPixmap generatePixmap(const QPixmap &src, const int &radius) {

        // 无效图片不处理
        if (src.isNull()) {
            return src;
        }

        // 压缩图片
        QPixmap pixmap = scaledPixmap(src, radius * 2,radius * 2);

        QPixmap dest(2*radius, 2*radius);
        dest.fill(Qt::transparent);
        QPainter painter(&dest);
        // 抗锯齿 + 平滑边缘处理
        painter.setRenderHints(QPainter::Antialiasing, true);
        painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
        // 裁剪为圆角
        QPainterPath path;
        path.addEllipse(0, 0, 2*radius, 2*radius);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, 2*radius, 2*radius, pixmap);

        return dest;
    }

Style_sheet::Style_sheet()
{
    color_btn_style="QPushButton{"
                                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 rgba(255, 181, 79, 255), stop:1 rgba(255, 133, 111, 255));"
                                    "border-radius: 15px;"
                                    "color:#ffffff; "
                                    "font: bold 13px;"
                                    "font-family: 汉仪正圆-45W;}"
                    "QPushButton:hover{"
                                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 rgba(255, 181, 79, 230), stop:1 rgba(255, 133, 111, 230));"
                                    "border-radius: 15px;"
                                    "color:#ffffff; "
                                    "font: bold 13px;"
                                    "font-family: 汉仪正圆-45W;}"
                    "QPushButton:pressed {"
                                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 rgba(255, 181, 79, 200), stop:1 rgba(255, 133, 111, 200));"
                                    "border-radius: 15px;"
                                    "color:#ffffff; "
                                    "font: bold 13px;"
                                    "font-family: 汉仪正圆-45W;}";
    //qlineargradient(spread:pad, x1:0, y1:0.034, x2:0.73, y2:0.756, stop:0 rgba(245, 174, 76, 255), stop:1 rgba(245, 128, 107, 255))

    write_btn_style="QPushButton{"
                                    "background-color:rgb(255,255,255);"
                                    "border-radius: 13px; "
                                    "border:1px solid rgba(255,152,95,255);"
                                    "color:qlineargradient(spread:pad, x1:0, y1:0.034, x2:0.730337, y2:0.756045, stop:0 rgba(255, 181, 79, 255), stop:1 rgba(255, 133, 111, 255));"
                                    "font: bold 13px;"
                                    "font-family: 汉仪正圆-45W;}"
                    "QPushButton:hover{"
                                    "background-color:#fbfbfb;"
                                    "border-radius: 13px; "
                                    "border:1px solid rgba(255,152,95,255);"
                                    "color:qlineargradient(spread:pad, x1:0, y1:0.034, x2:0.730337, y2:0.756045, stop:0 rgba(255, 181, 79, 255), stop:1 rgba(255, 133, 111, 255));"
                                    "font: bold 13px;"
                                    "font-family: 汉仪正圆-45W;}"
                     "QPushButton:pressed {"
                                    "background-color:#f5f5f5;"
                                    "border-radius: 13px; "
                                    "border:1px solid rgba(255,152,95,255);"
                                    "color:qlineargradient(spread:pad, x1:0, y1:0.034, x2:0.730337, y2:0.756045, stop:0 rgba(255, 181, 79, 255), stop:1 rgba(255, 133, 111, 255));"
                                    "font: bold 13px;"
                                    "font-family: 汉仪正圆-45W;}";

    line_edit_style="border-radius:12px;"
                    "padding-left:10px;"
                    "background-color:rgb(255, 255, 255);"
                    "border:1px solid qlineargradient(spread:pad, x1:0, y1:0.034, x2:0.73, y2:0.756, stop:0 rgba(255, 179, 79, 100), stop:1 rgba(255, 133, 111, 100));";

    good_name="font:450 15px '汉仪正圆-55W';color:#444444";

    good_discountprice="font: 16px 'Arial Rounded MT Bold';color:#e15e00";

    good_price="font:italic 11px 'Calibri';color:#767676";

    good_num="font: 11px '微软雅黑';color:#545454";

    good_inventory="font: 11px '微软雅黑 Light';color:#767676";

    general_txt="font: 13px '汉仪正圆-55W';color:#545454";

    notice="background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(0, 0, 0, 150), stop:1 rgba(0, 0, 0, 150));"
           "color:rgb(255,255,255);"
           "border-radius:15px;"
           "font:15px '微软雅黑';";
}

//void Notice_box(QMainWindow *pmain,QString nts)
//{
//    QLabel *notice=new QLabel(pmain);
//    notice->setContentsMargins(pmain->frameGeometry().x()/2,pmain->frameGeometry().y()/2,pmain->frameGeometry().width()/2,pmain->frameGeometry().height()/2);
//    notice->setText(nts);
//    notice->show();
//    QTimer *timer=new QTimer;
//    timer->singleShot(5000,pmain,SLOT(notice->hide()));
//    delete timer;
//    delete notice;
//}
