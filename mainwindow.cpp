#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDB();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initDB() {
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("idcard.db");
        database.setUserName("idcard");
        database.setPassword("123456");
    }



    if (!database.open()) {
        qDebug() << "Error: Failed to connect database." << database.lastError();
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

    database.close();
}
