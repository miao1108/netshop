#ifndef IMAGEHANDLE_H
#define IMAGEHANDLE_H
#include<QByteArray>
#include<QImage>
#include<QBuffer>
QByteArray getImageData(const QImage &image);
QImage getImage(QByteArray data);

#endif // IMAGEHANDLE_H
