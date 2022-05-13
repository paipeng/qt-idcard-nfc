#ifndef COMMONUTIL_H
#define COMMONUTIL_H

#include "idcard.h"

IdCard convertStringToIdCard(QString text);
QString convertIdCardToString(const IdCard &idCard);
#endif // COMMONUTIL_H
