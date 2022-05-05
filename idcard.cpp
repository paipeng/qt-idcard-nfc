#include "idcard.h"

#include <QDebug>


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
}

void IdCard::setName(QString name) {
    this->name = name;
}

QString IdCard::getName() {
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

const QDate IdCard::getExpireDate() {
    //qDebug() << "getExpireDate: " << &expireDate;
    return expireDate;//QDate::fromString(expireDate, "yyyy-MM-dd");;
}

void IdCard::setCompany(QString company) {
    this->company = company;
}
QString IdCard::getCompany() {
    return this->company;
}

bool IdCard::operator==(const IdCard &other) const{
    return this->id == other.id && this->name != other.name;
};
