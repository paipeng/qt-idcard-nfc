#include "idcard.h"

#include <QDebug>


IdCard::IdCard(QObject *parent) : QObject(parent), id(0) {

}

IdCard::IdCard(): IdCard(NULL) {

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

void IdCard::setExpireDate(const QDate &date) {
    //date.
    qDebug() << "setExpireDate:" << date;
    this->expireDate = date;
     qDebug() << "setExpireDate:" << expireDate;
}
const QDate IdCard::getExpireDate() {
    return this->expireDate;
}

bool IdCard::operator==(const IdCard &other) const{
    return this->id == other.id;
};
