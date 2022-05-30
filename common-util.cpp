
#include "common-util.h"
#include <QtDebug>
#include <QDate>
#include <QFile>

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
    QString data = QString("姓名: %1\n单位: %2\n证卡编号: %3\n过期日期: %4\n芯片序号: %5\n照片尺寸: %6").arg(
                idCard.getName(),
                idCard.getCompany(),
                idCard.getSerialNumber(),
                idCard.getExpireDate().toString(DATE_FORMAT),
                idCard.getChipUID(),
                QString(idCard.getPassPhotoSize())
                );
    return data;
}

bool compareIdCards(IdCard idCard1, IdCard idCard2) {
    qDebug() << "compareIdCards";
    if (idCard1.getSerialNumber() != idCard2.getSerialNumber()) {
        qDebug() << "serialNumber diff";
        return false;
    }
    if (idCard1.getChipUID() != idCard2.getChipUID()) {
        qDebug() << "chiUid diff";
        return false;
    }
    if (idCard1.getName() != idCard2.getName()) {
        qDebug() << "name diff";
        return false;
    }
    if (idCard1.getCompany() != idCard2.getCompany()) {
        qDebug() << "company diff";
        return false;
    }
    if (idCard1.getExpireDate() != idCard2.getExpireDate()) {
        qDebug() << "expire date diff";
        return false;
    }
    return true;
}


QByteArray readFile(const QString &filepath) {
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
        return QByteArray();
    QByteArray data = file.readAll();
    return data;
}

