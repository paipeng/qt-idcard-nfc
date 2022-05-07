QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    barcodeencoder.cpp \
    idcard.cpp \
    main.cpp \
    mainwindow.cpp \
    pdfwriter.cpp \
    sqliteengine.cpp

HEADERS += \
    barcodeencoder.h \
    idcard.h \
    mainwindow.h \
    pdfwriter.h \
    sqliteengine.h

# decoder/reader
HEADERS += \
    zxing/BarcodeFormat.h \
    zxing/BitHacks.h \
    zxing/ByteArray.h \
    zxing/DecodeHints.h \
    zxing/DecodeStatus.h \
    zxing/Flags.h \
    zxing/GTIN.h \
    zxing/ImageView.h \
    zxing/Point.h \
    zxing/Quadrilateral.h \
    zxing/ReadBarcode.h \
    zxing/Result.h \
    zxing/StructuredAppend.h \
    zxing/TextUtfEncoding.h \
    zxing/ZXContainerAlgorithms.h

# encoder/writer
HEADERS += \
    zxing/Matrix.h \
    zxing/BitMatrix.h \
    zxing/CharacterSet.h \
    zxing/CharacterSetECI.h \
    zxing/ZXConfig.h \
    zxing/MultiFormatWriter.h

# libharu
HEADERS += \
    libharu/hpdf.h \
    libharu/hpdf_config.h \
    libharu/hpdf_version.h \
    libharu/hpdf_consts.h \
    libharu/hpdf_doc.h \
    libharu/hpdf_error.h \
    libharu/hpdf_pdfa.h \
    libharu/hpdf_catalog.h \
    libharu/hpdf_objects.h \
    libharu/hpdf_encoder.h \
    libharu/hpdf_streams.h \
    libharu/hpdf_list.h \
    libharu/hpdf_types.h \
    libharu/hpdf_mmgr.h \
    libharu/hpdf_encrypt.h \
    libharu/hpdf_image.h \
    libharu/hpdf_pages.h \
    libharu/hpdf_gstate.h \
    libharu/hpdf_font.h \
    libharu/hpdf_fontdef.h \
    libharu/hpdf_ext_gstate.h \
    libharu/hpdf_outline.h



FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    qt-idcard-nfc_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/x64/release/ -lZXing -llibhpdf -llibpng16 -lzlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/x64/debug/ -lZXing -llibhpdfd -llibpng16d -lzlibd
else:unix: LIBS += -L$$PWD/libs/x64/ -lZXing -llibhpdf -llibpng16 -lzlib

INCLUDEPATH += $$PWD/libs/x64
DEPENDPATH += $$PWD/libs/x64
INCLUDEPATH += $$PWD/zxing
INCLUDEPATH += $$PWD/libharu

