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
    QString create_sql = "create table idcard (id int primary key, name varchar(30), expire int)";
    sql_query.prepare(create_sql);
    if(!sql_query.exec()) {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
    } else {
        qDebug() << "Table created!";
    }
}

void SqliteEngine::insert() {
    qDebug() << "SQLite insert";
    QSqlQuery sql_query;
    QString insert_sql = "insert into idcard values (?, ?, ?)";
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(2);
    sql_query.addBindValue("Peng");
    sql_query.addBindValue(100);
    if(!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        qDebug() << "inserted!";
    }
}
void SqliteEngine::query() {
    qDebug() << "SQLite query";
    QSqlQuery sql_query;
    QString select_sql = "select id, name from idcard";
    if(!sql_query.exec(select_sql)) {
        qDebug()<<sql_query.lastError();
    } else {
        while(sql_query.next()) {
            int id = sql_query.value(0).toInt();
            QString name = sql_query.value(1).toString();
            qDebug()<<QString("id:%1    name:%2").arg(id).arg(name);
        }
    }
}
