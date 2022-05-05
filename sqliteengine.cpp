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
    QString create_sql = "create table idcard (id INTEGER primary key AUTOINCREMENT, name varchar(30) not null, expire date not null)";
    sql_query.prepare(create_sql);
    if(!sql_query.exec()) {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
    } else {
        qDebug() << "Table created!";
    }
}

void SqliteEngine::insert(IdCard& idCard) {
    qDebug() << "SQLite insert: " << idCard.getExpireDate() << " name: " << idCard.getName();
    QSqlQuery sql_query;
    QString insert_sql = "insert into idcard(name, expire) values (:name, :expire)";
    sql_query.prepare(insert_sql);
    sql_query.bindValue(":name", idCard.getName());
    sql_query.bindValue(":expire", idCard.getExpireDate());

    if(!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        qDebug() << "inserted!";
    }
    qDebug() << sql_query.lastQuery();
}
QList<IdCard> SqliteEngine::query() {
    qDebug() << "SQLite query";
    QList<IdCard> idCards;
    QSqlQuery sql_query;
    QString select_sql = "select id, name from idcard";
    if(!sql_query.exec(select_sql)) {
        qDebug()<<sql_query.lastError();
        return idCards;
    } else {

        while(sql_query.next()) {
            long id = sql_query.value(0).toLongLong();
            QString name = sql_query.value(1).toString();
            QString expireDate = sql_query.value(2).toString();
            QDate date = QDate::currentDate();
            IdCard idCard(id, name, date);
            //IdCard idcard;
            //qDebug()<<QString("id:%1    name:%2").arg(id).arg(name);
            idCards.append(idCard);
            qDebug() << idCard.getId() << " >> " << idCard.getName();
        }
        return idCards;
    }
}
