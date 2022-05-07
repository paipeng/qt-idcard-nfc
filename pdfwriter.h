#ifndef PDFWRITER_H
#define PDFWRITER_H

#include <QObject>

class PDFWriter : public QObject
{
    Q_OBJECT
public:
    explicit PDFWriter(QObject *parent = nullptr);
    int test(QString fileName);
signals:

};

#endif // PDFWRITER_H
