#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableView>
#include <QStandardItem>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    sqliteEngine = new SqliteEngine();
    //sqliteEngine->initDB();
    initTableView();
}

MainWindow::~MainWindow() {
    delete sqliteEngine;
    delete ui;
}

void MainWindow::insert() {
    qDebug() << "insert: " << ui->nameLineEdit->text();

    ui->expireDateEdit->date();
    ;
    const QDate date = ui->expireDateEdit->date();
    IdCard idCard(ui->nameLineEdit->text(), date);


    qDebug() << "insert: " << idCard.getExpireDate();

    sqliteEngine->insert(idCard);
}

void MainWindow::query() {
    QStandardItemModel *model = new QStandardItemModel();

    model->setColumnCount(3);
    model->setHeaderData(0,Qt::Horizontal,tr("id"));
    model->setHeaderData(1,Qt::Horizontal,tr("name"));
    model->setHeaderData(2,Qt::Horizontal,tr("expire"));

    const QList<IdCard> idCards = sqliteEngine->query();
    QStandardItem * item = NULL;
    foreach(IdCard idCard, idCards ) {
        int index = idCards.indexOf(idCard);
        qDebug() << idCard.getId() << " " << idCard.getName() << " " << idCard.getExpireDate();
        item = new QStandardItem(QString("%1").arg(idCard.getId()));
        model->setItem(index, 0, item);
        item = new QStandardItem(QString("%1").arg(idCard.getName()));
        model->setItem(index, 1, item);
        item = new QStandardItem(QString("%1").arg(idCard.getExpireDate().toString("yyyy-MM-dd")));
        model->setItem(index, 2, item);
    }

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView->setColumnWidth(0,101);
    ui->tableView->setColumnWidth(1,102);
    ui->tableView->setShowGrid(true);
    ui->tableView->show();
}

void MainWindow::initTableView() {
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setHeaderData(0,Qt::Horizontal,tr("id"));
    model->setHeaderData(1,Qt::Horizontal,tr("name"));
    model->setHeaderData(2,Qt::Horizontal,tr("expire"));


    ui->tableView->setModel(model);

    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //设置列宽不可变
    //ui->tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    //ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0,101);
    ui->tableView->setColumnWidth(1,102);

    ui->tableView->setShowGrid(true);

}
