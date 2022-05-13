QT       += core gui
QT       += sql

QT += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    barcodeencoder.cpp \
    barcodedecoder.cpp \
    common-util.cpp \
    idcard.cpp \
    main.cpp \
    mainwindow.cpp \
    pdfwriter.cpp \
    sqliteengine.cpp

HEADERS += \
    barcodeencoder.h \
    barcodedecoder.h \
    common-util.h \
    idcard.h \
    mainwindow.h \
    pdfwriter.h \
    sqliteengine.h

# decoder/reader
HEADERS += \
    include/zxing/BarcodeFormat.h \
    include/zxing/BitHacks.h \
    include/zxing/ByteArray.h \
    include/zxing/DecodeHints.h \
    include/zxing/DecodeStatus.h \
    include/zxing/Flags.h \
    include/zxing/GTIN.h \
    include/zxing/ImageView.h \
    include/zxing/Point.h \
    include/zxing/Quadrilateral.h \
    include/zxing/ReadBarcode.h \
    include/zxing/Result.h \
    include/zxing/StructuredAppend.h \
    include/zxing/TextUtfEncoding.h \
    include/zxing/ZXContainerAlgorithms.h

# encoder/writer
HEADERS += \
    include/zxing/Matrix.h \
    include/zxing/BitMatrix.h \
    include/zxing/CharacterSet.h \
    include/zxing/CharacterSetECI.h \
    include/zxing/ZXConfig.h \
    include/zxing/MultiFormatWriter.h

# libharu
HEADERS += \
    include/libharu/hpdf.h \
    include/libharu/hpdf_config.h \
    include/libharu/hpdf_version.h \
    include/libharu/hpdf_consts.h \
    include/libharu/hpdf_doc.h \
    include/libharu/hpdf_error.h \
    include/libharu/hpdf_pdfa.h \
    include/libharu/hpdf_catalog.h \
    include/libharu/hpdf_objects.h \
    include/libharu/hpdf_encoder.h \
    include/libharu/hpdf_streams.h \
    include/libharu/hpdf_list.h \
    include/libharu/hpdf_types.h \
    include/libharu/hpdf_mmgr.h \
    include/libharu/hpdf_encrypt.h \
    include/libharu/hpdf_image.h \
    include/libharu/hpdf_pages.h \
    include/libharu/hpdf_gstate.h \
    include/libharu/hpdf_font.h \
    include/libharu/hpdf_fontdef.h \
    include/libharu/hpdf_ext_gstate.h \
    include/libharu/hpdf_outline.h


HEADERS += \
    include/cpcamera/cpcamera.h \
    include/cpcamera/cp-camera_global.h \
    include/cpcamera/cpcameraviewfinder.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    qt-idcard-nfc_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

DEFINES += CPCAMERA_LIBRARY

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/x64/release/ -lZXing -llibhpdf -llibpng16 -lzlib -lcp-camera
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/x64/debug/ -lZXing -llibhpdfd -llibpng16d -lzlibd -lcp-camera
else:unix: LIBS += -L$$PWD/libs/x64/ -lZXing -llibhpdf -llibpng16 -lzlib -lcp-camera

INCLUDEPATH += $$PWD/libs/x64
DEPENDPATH += $$PWD/libs/x64
INCLUDEPATH += $$PWD/include/zxing
INCLUDEPATH += $$PWD/include/libharu
INCLUDEPATH += $$PWD/include/cpcamera


#DEFINES += CP_SMARTCARD_LIBRARY
INCLUDEPATH += $$PWD/../qt-smartcard

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../qt-smartcard/release/ -lqt-smartcard
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../qt-smartcard/debug/ -lqt-smartcard
else:unix: LIBS += -L$$PWD/libs/x64/ -lqt-smartcard


