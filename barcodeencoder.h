#ifndef BARCODEENCODER_H
#define BARCODEENCODER_H

#include <QObject>


#include "BarcodeFormat.h"
#include "BitMatrix.h"
#include "MultiFormatWriter.h"
#include "TextUtfEncoding.h"
#include "CharacterSetECI.h"


using namespace ZXing;
class BarcodeEncoder : public QObject
{
    Q_OBJECT
public:
    explicit BarcodeEncoder(QObject *parent = nullptr);

signals:

};

#endif // BARCODEENCODER_H
