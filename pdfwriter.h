#ifndef PDFWRITER_H
#define PDFWRITER_H

#include <QObject>
#include "idcard.h"

class PDFWriter : public QObject
{
    Q_OBJECT
public:
    explicit PDFWriter(QObject *parent = nullptr);
    int test(QString fileName);
    int generateIdCard(const IdCard &idCard, QString fileName);
signals:

};

#endif // PDFWRITER_H
