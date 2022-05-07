#include "idcard.h"

#include <QDebug>
#include <QJsonObject>

IdCard::IdCard(QObject *parent) : QObject(parent), id(0) {

}

IdCard::IdCard(): IdCard(NULL) {

}

IdCard::IdCard(long id, QString name) {
    setId(id);
    setName(name);
}

IdCard::IdCard(long id, QString name, const QDate& expireDate):IdCard(NULL) {
    setId(id);
    setName(name);
    setExpireDate(expireDate);
}

IdCard::IdCard(QString name, const QDate& expireDate):IdCard(NULL) {
    setName(name);
    setExpireDate(expireDate);
}

IdCard::IdCard(const IdCard &other):IdCard(NULL) {
    setId(other.id);
    setName(other.name);
    setExpireDate(other.expireDate);
    setCompany(other.company);
    setSerialNumber(other.serialNumber);
}

void IdCard::setName(QString name) {
    this->name = name;
}

const QString &IdCard::getName() const{
    return this->name;
}

void IdCard::setId(long id) {
    this->id = id;
}

long IdCard::getId() {
    return this->id;
}

void IdCard::setExpireDate(QDate date) {
    //date.
    //qDebug() << "setExpireDate:" << date;
    //this->expireDate = date.toString("yyyy-MM-dd");//QDate::fromString(date.toString("yyyy-MM-dd"), "yyyy-MM-dd");
    //qDebug() << "setExpireDate:" << expireDate;
    //this->expireDate = date;
    this->expireDate = date;//QDate::fromString(date.toString("yyyy-MM-dd"), "yyyy-MM-dd");
}

const QDate IdCard::getExpireDate() const {
    //qDebug() << "getExpireDate: " << &expireDate;
    return expireDate;//QDate::fromString(expireDate, "yyyy-MM-dd");;
}

void IdCard::setCompany(QString company) {
    this->company = company;
}
QString IdCard::getCompany() const {
    return this->company;
}

const QString &IdCard::getSerialNumber() const
{
    return serialNumber;
}

void IdCard::setSerialNumber(const QString &newSerialNumber)
{
    serialNumber = newSerialNumber;
}

void IdCard::read(const QJsonObject &json) {
    if (json.contains("name") && json["name"].isString())
        name = json["name"].toString();

    if (json.contains("company") && json["company"].isString())
        company = json["company"].toString();

    if (json.contains("serialNumber") && json["serialNumber"].isString())
        serialNumber = json["serialNumber"].toString();
    if (json.contains("id") && json["id"].isDouble())
        id = json["id"].toInt();
    if (json.contains("expireDate") && json["expireDate"].isString())
        this->expireDate = QDate::fromString(json["expireDate"].toString(), DATE_FORMAT);
        //mClassType = ClassType(json["classType"].toInt());
}

void IdCard::write(QJsonObject &json) const {
    json["id"] = (int)id;//QJsonValue::fromVariant(QVariant::fromValue(id));
    json["name"] = name;
    json["company"] = company;
    json["expireDate"] = expireDate.toString(DATE_FORMAT);
    json["serialNumber"] = serialNumber;
}

void IdCard::toString() {

}
bool IdCard::operator==(const IdCard &other) const{
    return this->id == other.id && this->name != other.name;
};
