
#include "common-util.h"
#include <QtDebug>
#include <QDate>

IdCard convertStringToIdCard(QString text) {
    IdCard idCard(NULL);
    QStringList strList = text.split("\n");
    if (strList.size() <= 1) {
        return idCard;
    }
    foreach(QString str, strList) {
        qDebug() << "str: " << str;
        QStringList strList2 = str.split(": ");
        if (strList2.size() <= 1) {
            continue;
        }
        qDebug() << "key: " << strList2[0] << " value: " << strList2[1];
        if (strList2[0] == "姓名") {
            idCard.setName(strList2[1]);
        } else if (strList2[0] == "单位") {
            idCard.setCompany(strList2[1]);
        } else if (strList2[0] == "证卡编号") {
            idCard.setSerialNumber(strList2[1]);
        } else if (strList2[0] == "过期日期") {
            idCard.setExpireDate(QDate::fromString(strList2[1], DATE_FORMAT));
        } else if (strList2[0] == "芯片序号") {
            idCard.setChipUID(strList2[1]);
        }
    }
    return idCard;
}

QString convertIdCardToString(const IdCard &idCard) {
    QString data = QString("姓名: %1\n单位: %2\n证卡编号: %3\n过期日期: %4\n芯片序号: %5").arg(
                idCard.getName(),
                idCard.getCompany(),
                idCard.getSerialNumber(),
                idCard.getExpireDate().toString(DATE_FORMAT),
                idCard.getChipUID()
                );
    return data;
}

bool compareIdCards(IdCard idCard1, IdCard idCard2) {
    if (idCard1.getSerialNumber() != idCard2.getSerialNumber()) {
        return false;
    }
    if (idCard1.getChipUID() != idCard2.getChipUID()) {
        return false;
    }
    if (idCard1.getName() != idCard2.getName()) {
        return false;
    }
    if (idCard1.getCompany() != idCard2.getCompany()) {
        return false;
    }
    if (idCard1.getExpireDate() != idCard2.getExpireDate()) {
        return false;
    }
    return true;
}
