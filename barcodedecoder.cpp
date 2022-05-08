#include "barcodedecoder.h"


#include "TextUtfEncoding.h"
#include "GTIN.h"

using namespace TextUtfEncoding;

BarcodeDecoder::BarcodeDecoder() :m_stop(false), decoding(false) {

}


BarcodeDecoder::~BarcodeDecoder() {
    requestInterruption();
    m_stop = true;
    //exit(0);
    quit();
    wait();
}


void BarcodeDecoder::run() {
    qDebug()<<"From worker thread: "<<currentThreadId();
    while (1) {
        {
        QMutexLocker locker(&m_mutex);
        if (m_stop)
            break;
        }
        if (decoding) {
            decode(image);
            decoding = false;
        }
        //msleep(500);
    }
    qDebug()<<"From worker thread ended: "<<currentThreadId();
}


void BarcodeDecoder::setImage(const QImage &image) {
    this->image = image;
    this->decoding = true;
}

void BarcodeDecoder::stop() {
    qDebug()<<"Thread::stop called from main thread: "<<currentThreadId();
    QMutexLocker locker(&m_mutex);
    m_stop=true;
}

void BarcodeDecoder::decode(const QImage& image) {
    DecodeHints hints;
    hints.setEanAddOnSymbol(EanAddOnSymbol::Read);

    ImageView imageView{image.bits(), image.width(), image.height(), ImageFormat::RGBX};
    decodeResults = ReadBarcodes(imageView, hints);

    // if we did not find anything, insert a dummy to produce some output for each file
    int ret = 0;
    if (decodeResults.empty())
        ret = -1;

    emit updateBarcodeDecodeResult(ret);
}

