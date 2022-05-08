#ifndef BARCODEDECODER_H
#define BARCODEDECODER_H

#if QT_VERSION>=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include <QImage>
#include "ReadBarcode.h"

using namespace ZXing;

class BarcodeDecoder: public QThread
{
    Q_OBJECT
public:
    BarcodeDecoder();
    ~BarcodeDecoder();

public slots:
    void stop();
    void decode(const QImage& image);
    void setImage(const QImage &image);

signals:
    Q_SIGNAL void updateBarcodeDecodeResult(int decodeState);

private:
    QMutex m_mutex;
    bool m_stop;
    bool decoding;

    void run();
    QImage image;

public:
    Results decodeResults;
};

#endif // BARCODEDECODER_H
