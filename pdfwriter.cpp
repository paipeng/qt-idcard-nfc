#include "pdfwriter.h"

#include "hpdf.h"

#include "barcodeencoder.h"

#include <QDebug>
#include <QTextCodec>

jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler  (HPDF_STATUS   error_no,
                HPDF_STATUS   detail_no,
                void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}


void
draw_image (HPDF_Doc     pdf,
            const char  *filename,
            float        x,
            float        y,
            float       dpi,
            const char  *text)
{
#ifdef __WIN32__
    const char* FILE_SEPARATOR = "\\";
#else
    const char* FILE_SEPARATOR = "/";
#endif
    char filename1[255];
    char print_text[256];

    HPDF_Page page = HPDF_GetCurrentPage (pdf);
    HPDF_Image image;

    strcpy(filename1, "C:\\pngsuite");
    strcat(filename1, FILE_SEPARATOR);
    strcat(filename1, filename);

    if (strstr(filename, ".bmp")) {
        image = HPDF_LoadBmpImageFromFile(pdf, filename1);
    } else {
        image = HPDF_LoadPngImageFromFile(pdf, filename1);
    }


    snprintf(print_text, sizeof(char) * 256, "%s (%.0f dpi)", text, dpi);
    /* Draw image to the canvas. */
    if (strstr(filename, ".bmp")) {
        HPDF_Page_DrawImage(page, image, x, y, HPDF_Image_GetWidth(image)*72.0/dpi,
            HPDF_Image_GetHeight(image)*72.0/dpi);
    }
    else {
#if 0
        HPDF_Page_DrawImage(page, image, x, y, HPDF_Image_GetWidth(image),
            HPDF_Image_GetHeight(image));
#endif
    }
    /* Print the text. */
    HPDF_Page_BeginText (page);
    HPDF_Page_SetTextLeading (page, 16);
    HPDF_Page_MoveTextPos (page, x, y);
    HPDF_Page_ShowTextNextLine (page, filename);
    HPDF_Page_ShowTextNextLine (page, print_text);
    HPDF_Page_EndText (page);
}


void
draw_rect (HPDF_Page     page,
           double        x,
           double        y,
           const char   *label)
{
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x, y - 10);
    HPDF_Page_ShowText (page, label);
    HPDF_Page_EndText (page);

    HPDF_Page_Rectangle(page, x, y - 40, 220, 25);
}

PDFWriter::PDFWriter(QObject *parent) : QObject(parent)
{

}

int PDFWriter::generateIdCard(const IdCard &idCard, QString fileName) {
    qDebug() << "generateIdCard: " << fileName;
    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;

    HPDF_Image image;

    HPDF_Destination dst;
    float dpi;
    HPDF_REAL x, y;

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
       printf ("error: cannot create PdfDoc object\n");
       return 1;
    }
    qDebug() << "2";
    /* error-handler */
    if (setjmp(env)) {
       HPDF_Free (pdf);
       return 1;
    }
    qDebug() << "3";

    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    HPDF_UseCNSFonts(pdf);
    HPDF_UseCNSEncodings(pdf);

    qDebug() << "4";
#if 0
    font = HPDF_GetFont (pdf, "Simsun", "GB-EUC-H");
#else
    //HPDF_UseUTFEncodings(pdf);
    //HPDF_SetCurrentEncoder(pdf, "UTF-8");

    /* create default-font */

    //HPDF_SetCurrentEncoder(pdf, "GB-EUC-H");

#if 1
    HPDF_UseUTFEncodings(pdf);
    HPDF_SetCurrentEncoder(pdf, "UTF-8");
    //const char* font_name = HPDF_LoadTTFontFromFile(pdf, "C:\\Users\\paipeng\\git\\libharu\\demo\\ttfont\\FZQTJW.TTF", HPDF_TRUE);
    //font = HPDF_GetFont(pdf, font_name, "UTF-8");
    const char* font_name = HPDF_LoadTTFontFromFile(pdf, "C:\\Users\\paipeng\\git\\qt-idcard-nfc\\fonts\\FZSSJW.TTF", HPDF_FALSE);
    font = HPDF_GetFont(pdf, font_name, "GB-EUC-H");

#if 0
    HPDF_UseUTFEncodings(pdf);
    HPDF_SetCurrentEncoder(pdf, "UTF-8");
    QString fontPath = "C:\\Users\\paipeng\\git\\libharu\\demo\\ttfont\\FZQTJW.TTF";
    fontPath = "C:\\\\Users\\\\paipeng\\\\git\\\\qt-idcard-nfc\\\\fonts\\\\FZDHTJW.TTF";
    qDebug() << "fontPath: " << fontPath << " -> " << fontPath.toStdString().data();
    qDebug() << "load ttf font from file";
    const char* font_name = HPDF_LoadTTFontFromFile(pdf, fontPath.toStdString().data(), HPDF_FALSE);
    std::string str(font_name);
    QString text = QString::fromStdString(str);
    qDebug() << "load ttf font success: " << text;

    font = HPDF_GetFont(pdf, font_name, "GBK-EUC-H");
    //font = HPDF_GetFont(pdf, font_name, "GB-EUC-H");
#endif
    qDebug() << "font loaded";
#else
    //font = HPDF_GetFont (pdf, "SimSun", "GB-EUC-H");
    font = HPDF_GetFont(pdf, "SimHei,BoldItalic", "GB-EUC-H");
#endif
#endif
    qDebug() << "selected font: " << font;
    /* add a new page object. */
    page = HPDF_AddPage (pdf);


    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    dst = HPDF_Page_CreateDestination (page);
    HPDF_Destination_SetXYZ (dst, 0, HPDF_Page_GetHeight (page), 1);
    HPDF_SetOpenAction(pdf, dst);


    HPDF_Page_SetRGBFill (page, 1.0, 0.0, 0.0);
    HPDF_Page_SetTextRenderingMode (page, HPDF_FILL);
    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 24);
    HPDF_Page_MoveTextPos (page, 140, HPDF_Page_GetHeight (page) - 50);
    QString title("证卡");
    //const char dt[] = {(char)0xBA, (char)0xBA, (char)0xD7, (char)0xD6, 0x00};
    //HPDF_Page_ShowText (page, dt);

    //QTextCodec *textCodec = QTextCodec::codecForName("GB18030");
    HPDF_Page_ShowText (page, title.toLocal8Bit());
    HPDF_Page_EndText (page);

#if 0
    const char* dd = "汉字测试";//title.toStdString().data();
    qDebug() << "title data len: " << strlen(dd);
    for (int i = 0; i < strlen(dd); i++) {
        QString t;
        t.sprintf("0x%02X ", dd[i] & 0xFF);

        qDebug() << t;
    }
#endif

    qDebug() << "IdCard serialnumber: " << idCard.getSerialNumber();
    // gen qrcode
    QImage qrCodeImage = BarcodeEncoder::encodeToImage(idCard.getSerialNumber() + " " + idCard.getChipUID());

    //qrCodeImage.save("C:\\pngsuite\\qrcode_test.bmp");
#if 0
    uchar* ptr = qrCodeImage.bits();
    for (int i = 0; i < qrCodeImage.height(); i++) {
        for (int j = 0; j < qrCodeImage.width(); j++) {
            qDebug() << ptr[i*qrCodeImage.width()+j];
        }
    }
#endif
    image = HPDF_LoadRawImageFromMem(pdf, qrCodeImage.bits(), qrCodeImage.width(), qrCodeImage.height(), HPDF_CS_DEVICE_RGB, 8);

    HPDF_Page_DrawImage (page, image, 150, HPDF_Page_GetHeight (page) - 110, 30, 30);//qrCodeImage.width(), qrCodeImage.height());


    // draw pass photo
    QImage passPhotoImage = QImage(idCard.getPassPhoto());
    qDebug() << "passPhotoImage: " << passPhotoImage.width() << "-" << passPhotoImage.height() << " " << passPhotoImage.bitPlaneCount();
    //image = HPDF_LoadJpegImageFromFile(pdf, idCard.getPassPhoto().toStdString().data());//pdf, passPhotoImage.bits(), passPhotoImage.width(), passPhotoImage.height(), HPDF_CS_DEVICE_RGB, 32);



    bool alpha = passPhotoImage.hasAlphaChannel();
    unsigned pixel_count = alpha ? 4 : 3;
    unsigned stride = pixel_count * passPhotoImage.width();

    uint8_t* data = new uint8_t[ stride * passPhotoImage.height() ];
    if( !data )
        return false;

    //Make sure the input is in ARGB
    if( passPhotoImage.format() != QImage::Format_RGB32 && passPhotoImage.format() != QImage::Format_ARGB32 )
        passPhotoImage = passPhotoImage.convertToFormat( QImage::Format_ARGB32 );

    for( int iy=0; iy<passPhotoImage.height(); ++iy ){
        const QRgb* in = (const QRgb*)passPhotoImage.constScanLine( iy );
        uint8_t* out = data + iy*stride;

        for( int ix=0; ix<passPhotoImage.width(); ++ix, ++in ){
            *(out++) = qRed( *in );
            *(out++) = qGreen( *in );
            *(out++) = qBlue( *in );
            if( alpha )
                *(out++) = qAlpha( *in );
        }
    }



    image = HPDF_LoadRawImageFromMem(pdf,
                                    data,
                                     passPhotoImage.width(),
                                     passPhotoImage.height(),
                                     HPDF_CS_DEVICE_RGB,
                                     8);
    delete[] data;

    int passPhotoWidth = 90;
    int passPhotoHeight = passPhotoImage.height() * passPhotoWidth/ passPhotoImage.width();
    HPDF_Page_DrawImage (page, image, 40, HPDF_Page_GetHeight (page) - passPhotoHeight - 30, passPhotoWidth, passPhotoHeight);//qrCodeImage.width(), qrCodeImage.height());




    HPDF_Page_SetFontAndSize (page, font, 10);
    HPDF_Page_SetRGBFill (page, 0.0, 0.0, 0.0);


    x = 60;
    y = HPDF_Page_GetHeight (page) - 160;
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x,  y);
    HPDF_Page_ShowText (page, QString("姓名").toLocal8Bit());
    HPDF_Page_EndText (page);
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x + 70, y);
    HPDF_Page_ShowText (page, idCard.getName().toLocal8Bit());
    HPDF_Page_EndText (page);

    y -= 20;
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x, y);
    HPDF_Page_ShowText (page, QString("单位").toLocal8Bit());
    HPDF_Page_EndText (page);
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x + 70, y);
    HPDF_Page_ShowText (page, idCard.getCompany().toLocal8Bit());
    HPDF_Page_EndText (page);

    y -= 20;
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x, y);
    HPDF_Page_ShowText (page, QString("证卡编号").toLocal8Bit());
    HPDF_Page_EndText (page);
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x + 70, y);
    HPDF_Page_ShowText (page, idCard.getSerialNumber().toLocal8Bit());
    HPDF_Page_EndText (page);

    y -= 20;
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x, y);
    HPDF_Page_ShowText (page, QString("有效日期").toLocal8Bit());
    HPDF_Page_EndText (page);
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x + 70, y);
    HPDF_Page_ShowText (page, idCard.getExpireDate().toString(DATE_FORMAT).toLocal8Bit());
    HPDF_Page_EndText (page);


    HPDF_Page_SetFontAndSize (page, font, 8);
    HPDF_Page_SetRGBFill (page, 1.0, 0.0, 0.0);
    y -= 20;
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x, y);
    HPDF_Page_ShowText (page, QString("芯片序号").toLocal8Bit());
    HPDF_Page_EndText (page);
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x + 70, y);
    HPDF_Page_ShowText (page, idCard.getChipUID().toLocal8Bit());
    HPDF_Page_EndText (page);
#if 0
    dpi = 600;
    draw_image(pdf, "code_1bit.bmp", 100, HPDF_Page_GetHeight(page) - 550, dpi,
        "1bit");

    draw_image(pdf, "code_rgb.bmp", 200, HPDF_Page_GetHeight(page) - 550, dpi,
        "rgb");
    draw_image(pdf, "code_gray.bmp", 300, HPDF_Page_GetHeight(page) - 550, dpi,
        "gray");

    dpi = 1200;
    draw_image(pdf, "code_1bit_2x.bmp", 100, HPDF_Page_GetHeight(page) - 450, dpi,
        "1bit 2x");

    draw_image(pdf, "code_rgb_2x.bmp", 200, HPDF_Page_GetHeight(page) - 450, dpi,
        "rgb 2x");
    draw_image(pdf, "code_gray_2x.bmp", 300, HPDF_Page_GetHeight(page) - 450, dpi,
        "gray 2x");
#endif

#if 0
    /* print the lines of the page. */
    HPDF_Page_SetLineWidth (page, 1);
    HPDF_Page_Rectangle (page, 20, 20, HPDF_Page_GetWidth(page) - 40,
                HPDF_Page_GetHeight (page) - 40);
    HPDF_Page_Stroke (page);

    HPDF_Page_SetLineWidth (page, 2);
    HPDF_Page_SetRGBStroke (page, 0.0, 0.0, 0.5);

    //HPDF_Page_SetLineJoin (page, HPDF_MITER_JOIN);
    draw_rect(page, 100, 140, "rect");
    HPDF_Page_Stroke (page);
#endif
    qDebug() << "save pdf to file";

    /* save the document to a file */
    HPDF_SaveToFile (pdf, fileName.toStdString().data());

    /* clean up */
    HPDF_Free (pdf);
    return 0;
}
int PDFWriter::test(QString fileName) {
    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;

    HPDF_Destination dst;
    float dpi;

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
       printf ("error: cannot create PdfDoc object\n");
       return 1;
    }

    /* error-handler */
    if (setjmp(env)) {
       HPDF_Free (pdf);
       return 1;
    }

    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    /* create default-font */
    font = HPDF_GetFont (pdf, "Helvetica", NULL);

    /* add a new page object. */
    page = HPDF_AddPage (pdf);
    #if 0
    HPDF_Page_SetWidth (page, 550);
    HPDF_Page_SetHeight (page, 650);
    #else
   HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
#endif
   double pageWidth = HPDF_Page_GetWidth(page);
   double pageHeight = HPDF_Page_GetHeight(page);
   printf("pdf A4 page size: %f-%f\n", pageWidth, pageHeight);
   dst = HPDF_Page_CreateDestination (page);
   HPDF_Destination_SetXYZ (dst, 0, HPDF_Page_GetHeight (page), 1);
   HPDF_SetOpenAction(pdf, dst);

   HPDF_Page_BeginText (page);
   HPDF_Page_SetFontAndSize (page, font, 20);
   HPDF_Page_MoveTextPos (page, 220, HPDF_Page_GetHeight (page) - 70);
   HPDF_Page_ShowText (page, "Code Test");
   HPDF_Page_EndText (page);

   HPDF_Page_SetFontAndSize (page, font, 12);

   dpi = 600;
   draw_image(pdf, "code_1bit.bmp", 100, HPDF_Page_GetHeight(page) - 550, dpi,
       "1bit");

   draw_image(pdf, "code_rgb.bmp", 200, HPDF_Page_GetHeight(page) - 550, dpi,
       "rgb");
   draw_image(pdf, "code_gray.bmp", 300, HPDF_Page_GetHeight(page) - 550, dpi,
       "gray");

   dpi = 1200;
   draw_image(pdf, "code_1bit_2x.bmp", 100, HPDF_Page_GetHeight(page) - 450, dpi,
       "1bit 2x");

   draw_image(pdf, "code_rgb_2x.bmp", 200, HPDF_Page_GetHeight(page) - 450, dpi,
       "rgb 2x");
   draw_image(pdf, "code_gray_2x.bmp", 300, HPDF_Page_GetHeight(page) - 450, dpi,
       "gray 2x");

   dpi = 600;
   draw_image(pdf, "code_1bit_2x.bmp", 100, HPDF_Page_GetHeight(page) - 350, dpi,
       "1bit 2x");

   draw_image(pdf, "code_rgb_2x.bmp", 200, HPDF_Page_GetHeight(page) - 350, dpi,
       "rgb 2x");
   draw_image(pdf, "code_gray_2x.bmp", 300, HPDF_Page_GetHeight(page) - 350, dpi,
       "gray 2x");


   dpi = 300;
   draw_image(pdf, "code_1bit.bmp", 100, HPDF_Page_GetHeight(page) - 250, dpi,
       "1bit");

   draw_image(pdf, "code_rgb.bmp", 200, HPDF_Page_GetHeight(page) - 250, dpi,
       "rgb");
   draw_image(pdf, "code_gray.bmp", 300, HPDF_Page_GetHeight(page) - 250, dpi,
       "gray");
   /* save the document to a file */
   HPDF_SaveToFile (pdf, fileName.toStdString().data());

   /* clean up */
   HPDF_Free (pdf);
   return 0;
}
