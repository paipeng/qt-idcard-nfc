#ifndef BARCODEENCODER_H
#define BARCODEENCODER_H

#include <QObject>

#include <QImage>


class BarcodeEncoder : public QObject
{
    Q_OBJECT
public:
    explicit BarcodeEncoder(QObject *parent = nullptr);
    int encode(QString data);
    void decode(const QImage& image);
signals:
public:
};

#endif // BARCODEENCODER_H
