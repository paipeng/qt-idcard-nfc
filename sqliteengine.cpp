#include "sqliteengine.h"


#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

static QSqlDatabase temp;
SqliteEngine::SqliteEngine(QObject *parent) : QObject(parent) {
#if 1
    temp = QSqlDatabase::addDatabase("QSQLITE");
    database = &temp;
#else
    database = new QSqlDatabase();
    database->registerSqlDriver("QSQLITE", NULL);

#endif
    qDebug() << database->drivers();
    //database->addDatabase("QSQLITE", "SQLITE");
    initDB();
}

SqliteEngine::~SqliteEngine() {
    close();
    //delete database;
}

void SqliteEngine::close() {
    //database->close();
    temp.close();
}


void SqliteEngine::initDB() {

    database->setDatabaseName("idcard.db");
    database->setUserName("idcard");
    database->setPassword("123456");

    if (!database->open()) {
        qDebug() << "Error: Failed to connect database." << database->lastError();
    } else {
        // do something
    }

    QSqlQuery sql_query;
    QString create_sql = "create table idcard (\
                            id INTEGER primary key AUTOINCREMENT, \
                            serial_number varchar(32) not null, \
                            name varchar(30) not null, \
                            company varchar(64) not null, \
                            chip_uid varchar(16), \
                            expire date not null)";
    sql_query.prepare(create_sql);
    if(!sql_query.exec()) {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
    } else {
        qDebug() << "Table created!";
    }
}

int SqliteEngine::insert(IdCard& idCard) {
    qDebug() << "SQLite insert: " << idCard.getExpireDate() << " name: " << idCard.getName() << " " << idCard.getCompany();
    QSqlQuery sql_query;
    QString insert_sql = "insert into idcard(name, company, serial_number, expire) values (:name, :company, :serialNumber, :expire)";
    sql_query.prepare(insert_sql);
    sql_query.bindValue(":name", idCard.getName());
    sql_query.bindValue(":company", idCard.getCompany());
    sql_query.bindValue(":expire", idCard.getExpireDate());
    sql_query.bindValue(":serialNumber", idCard.getSerialNumber());

    if(!sql_query.exec()) {
        qDebug() << sql_query.lastError();
        qDebug() << sql_query.lastQuery();
        return sql_query.lastError().number();
    } else {
        qDebug() << "inserted!";
        qDebug() << sql_query.lastQuery();
        return 0;
    }
}

QList<IdCard> SqliteEngine::query() {
    qDebug() << "SQLite query";
    QList<IdCard> idCards;
    QSqlQuery sql_query;
    QString select_sql = "select id, name, company, expire, serial_number, chip_uid from idcard";
    if(!sql_query.exec(select_sql)) {
        qDebug() << sql_query.lastError();
        return idCards;
    } else {
        while(sql_query.next()) {
            long id = sql_query.value(0).toLongLong();
            QString name = sql_query.value(1).toString();
            QString company = sql_query.value(2).toString();
            QString expireDate = sql_query.value(3).toString();
            QDate date = QDate::fromString(expireDate, DATE_FORMAT);
            QString serialNumber = sql_query.value(4).toString();
            QString chipUID = sql_query.value(5).toString();
            qDebug() << "chipUID: " << chipUID;
            IdCard idCard(id, name, date);
            idCard.setCompany(company);
            idCard.setSerialNumber(serialNumber);
            idCard.setChipUID(chipUID);
            idCards.append(idCard);
        }
        return idCards;
    }
}

IdCard SqliteEngine::getIdCardById(long id) {
    qDebug() << "getIdCardById: " << id;
    QSqlQuery sql_query;
    QString select_sql = "select id, name, company, expire, serial_number, chip_uid from idcard where id =:id";
    sql_query.prepare(select_sql);
    sql_query.bindValue(":id", (int)id);
    if(!sql_query.exec()) {
        qDebug() << sql_query.lastError();
        return IdCard(NULL);
    } else {
        while(sql_query.next()) {
            long id = sql_query.value(0).toLongLong();
            QString name = sql_query.value(1).toString();
            QString company = sql_query.value(2).toString();
            QString expireDate = sql_query.value(3).toString();
            QDate date = QDate::fromString(expireDate, DATE_FORMAT);
            QString serialNumber = sql_query.value(4).toString();

            QString chipUID = sql_query.value(5).toString();

            IdCard idCard(id, name, date);
            idCard.setCompany(company);
            idCard.setSerialNumber(serialNumber);
            idCard.setChipUID(chipUID);
            return idCard;
        }
        return IdCard(NULL);
    }
}

IdCard SqliteEngine::getIdCardBySerialNumber(QString serialNumber) {
    qDebug() << "getIdCardBySerialNumber: " << serialNumber;
    QSqlQuery sql_query;
    QString select_sql = "select id, name, company, expire, serial_number, chip_uid from idcard where serial_number =:serialNumber";
    sql_query.prepare(select_sql);
    sql_query.bindValue(":serialNumber", serialNumber);
    if(!sql_query.exec()) {
        qDebug() << sql_query.lastError();
        return IdCard(NULL);
    } else {
        while(sql_query.next()) {
            long id = sql_query.value(0).toLongLong();
            QString name = sql_query.value(1).toString();
            QString company = sql_query.value(2).toString();
            QString expireDate = sql_query.value(3).toString();
            QDate date = QDate::fromString(expireDate, DATE_FORMAT);
            QString serialNumber = sql_query.value(4).toString();

            QString chipUID = sql_query.value(5).toString();

            IdCard idCard(id, name, date);
            idCard.setCompany(company);
            idCard.setSerialNumber(serialNumber);
            idCard.setChipUID(chipUID);
            return idCard;
        }
        return IdCard(NULL);
    }
}

IdCard SqliteEngine::getIdCardByChipUID(QString chipUID) {
    qDebug() << "getIdCardByChipUID: " << chipUID;
    QSqlQuery sql_query;
    QString select_sql = "select id, name, company, expire, serial_number, chip_uid from idcard where chip_uid =:chipUID";
    sql_query.prepare(select_sql);
    sql_query.bindValue(":chipUID", chipUID);
    if(!sql_query.exec()) {
        qDebug() << sql_query.lastError();
        return IdCard(NULL);
    } else {
        while(sql_query.next()) {
            long id = sql_query.value(0).toLongLong();
            QString name = sql_query.value(1).toString();
            QString company = sql_query.value(2).toString();
            QString expireDate = sql_query.value(3).toString();
            QDate date = QDate::fromString(expireDate, DATE_FORMAT);
            QString serialNumber = sql_query.value(4).toString();

            QString chipUID = sql_query.value(5).toString();

            IdCard idCard(id, name, date);
            idCard.setCompany(company);
            idCard.setSerialNumber(serialNumber);
            idCard.setChipUID(chipUID);
            return idCard;
        }
        return IdCard(NULL);
    }
}

IdCard SqliteEngine::updateChipUID(IdCard idCard, QString chipUID) {
    qDebug() << "updateChipUID: " << chipUID;
    QSqlQuery sql_query;
    QString select_sql = "update idcard set chip_uid=:chipUID where id =:id";
    sql_query.prepare(select_sql);
    qDebug() << "1";
    sql_query.bindValue(":chipUID", chipUID);
    qDebug() << "2";
    sql_query.bindValue(":id", (int)idCard.getId());
    qDebug() << "3";
    qDebug() << "before update " << sql_query.lastQuery();
    if(!sql_query.exec()) {
        qDebug() << "sqlite error: " << sql_query.lastError();
        return IdCard(NULL);
    } else {
        qDebug() << "updateChipUID success " << sql_query.lastQuery();
        idCard.setChipUID(chipUID);
        return idCard;
    }
}
