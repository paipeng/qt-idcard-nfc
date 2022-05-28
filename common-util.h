#ifndef COMMONUTIL_H
#define COMMONUTIL_H

#include "idcard.h"

IdCard convertStringToIdCard(QString text);
QString convertIdCardToString(const IdCard &idCard);
bool compareIdCards(IdCard idCard1, IdCard idCard2);
QByteArray readFile(const QString &filepath);
#endif // COMMONUTIL_H
