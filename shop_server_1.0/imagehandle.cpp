#include"imagehandle.h"
#include <QDebug>
QByteArray getImageData(const QImage &image)
{
    QImage photo;
    photo=image.scaled(400, 400).scaled(100,100,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QByteArray imageData;
    QBuffer buffer(&imageData);
    photo.save(&buffer, "jpg");
    //qDebug()<<"//////////////////////////////////////////////////////////////////////";
    //qDebug()<<imageData.length();
    //imageData = imageData.toBase64();
    //qDebug()<<"//////////////////////////////////////////////////////////////////////";
    //qDebug()<<imageData.length();
    return imageData;
}
QImage getImage(QByteArray data)
{
    QImage res;
    //res.loadFromData(QByteArray::fromBase64(data));
    res.loadFromData(data);
    return res;
}
