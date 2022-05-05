#include "idcard.h"
#if 1
IdCard::IdCard(QObject *parent) : QObject(parent)
{

}
#endif

IdCard::IdCard(long id, QString name):IdCard(NULL) {
    setId(id);
    setName(name);
}
#if 1
IdCard::IdCard(const IdCard &other):IdCard(NULL) {

    setId(other.id);
    setName(other.name);
}
#endif

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
bool IdCard::operator==(const IdCard &other) const{
    return this->id == other.id;
};
