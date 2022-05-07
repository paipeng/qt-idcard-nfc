#include "barcodeencoder.h"

#include "BitMatrix.h"
#include "MultiFormatWriter.h"
#include "TextUtfEncoding.h"
#include "ReadBarcode.h"

#include <QDebug>


using namespace ZXing;
using namespace TextUtfEncoding;

static std::string GetExtension(const std::string& path)
{
    auto fileNameStart = path.find_last_of("/\\");
    auto fileName = fileNameStart == std::string::npos ? path : path.substr(fileNameStart + 1);
    auto extStart = fileName.find_last_of('.');
    auto ext = extStart == std::string::npos ? "" : fileName.substr(extStart + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), [](char c) { return std::tolower(c); });
    return ext;
}

BarcodeEncoder::BarcodeEncoder(QObject *parent) : QObject(parent)
{

}

int BarcodeEncoder::encode(QString data) {
    BarcodeFormat format;
    CharacterSet encoding = CharacterSet::UTF8;
    format = BarcodeFormat::QRCode;

    int width = 80, height = 80;
    int margin = 0;
    int eccLevel = -1;
    QString filePath = "C:\\pngsuite\\qrcode.bmp";
    try {
        auto writer = MultiFormatWriter(format).setMargin(margin).setEncoding(encoding).setEccLevel(eccLevel);
        auto bitmap = ToMatrix<uint8_t>(writer.encode(TextUtfEncoding::FromUtf8(data.toStdString()), width, height));

        qDebug() << "qrcode size: " << bitmap.width() << "-" << bitmap.height();

        QImage image(bitmap.width(),bitmap.height(), QImage::Format_ARGB32);

        for(int i=0;i<bitmap.height();++i){
            for(int j=0;j<bitmap.width();++j) {
                //qDebug() << bitmap.get(j,i) << " -";
                image.setPixel(j, i, qRgb(bitmap.get(j,i), bitmap.get(j,i), bitmap.get(j,i)));
            }
        }

        image.save(filePath);
#if 0
        auto ext = GetExtension(filePath.toStdString());
        int success = 0;

        if (ext == "" || ext == "png") {
            success = stbi_write_png(filePath, bitmap.width(), bitmap.height(), 1, bitmap.data(), 0);
        } else if (ext == "jpg" || ext == "jpeg") {
            success = stbi_write_jpg(filePath, bitmap.width(), bitmap.height(), 1, bitmap.data(), 0);
        }
        if (!success) {
            qDebug() << "Failed to write image: " << filePath;
            return -1;
        }

#endif
    } catch (const std::exception& e) {
        qDebug() << e.what();
        return -1;
    }
    return 0;
}

QImage BarcodeEncoder::encodeToImage(const QString &data) {
    BarcodeFormat format;
    CharacterSet encoding = CharacterSet::UTF8;
    format = BarcodeFormat::QRCode;

    int width = 80, height = 80;
    int margin = 0;
    int eccLevel = -1;
    try {
        auto writer = MultiFormatWriter(format).setMargin(margin).setEncoding(encoding).setEccLevel(eccLevel);
        auto bitmap = ToMatrix<uint8_t>(writer.encode(TextUtfEncoding::FromUtf8(data.toStdString()), width, height));

        qDebug() << "qrcode size: " << bitmap.width() << "-" << bitmap.height();

        QImage image(bitmap.width(),bitmap.height(), QImage::Format_ARGB32);

        for(int i=0;i<bitmap.height();++i){
            for(int j=0;j<bitmap.width();++j) {
                //qDebug() << bitmap.get(j,i) << " -";
                image.setPixel(j, i, qRgb(bitmap.get(j,i), bitmap.get(j,i), bitmap.get(j,i)));
            }
        }
        return image;
    } catch (const std::exception& e) {
        qDebug() << e.what();
        return QImage();
    }
}

void BarcodeEncoder::decode(const QImage& image) {
    DecodeHints hints;
    hints.setEanAddOnSymbol(EanAddOnSymbol::Read);

    ImageView imageView{image.bits(), image.width(), image.height(), ImageFormat::RGBX};

    Results decodeResults = ReadBarcodes(imageView, hints);

    // if we did not find anything, insert a dummy to produce some output for each file
    int ret = 0;
    if (decodeResults.empty())
        ret = -1;

}
