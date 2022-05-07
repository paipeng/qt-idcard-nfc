#include "pdfwriter.h"

#include "hpdf.h"


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

PDFWriter::PDFWriter(QObject *parent) : QObject(parent)
{

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
