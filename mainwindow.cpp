#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableView>
#include <QStandardItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    sqliteEngine = new SqliteEngine();
    //sqliteEngine->initDB();
    initTableView();
    query();
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

    idCard.setCompany(ui->companyLineEdit->text());
    idCard.setSerialNumber(ui->serialNumberLineEdit->text());

    qDebug() << "insert: " << idCard.getExpireDate();

    sqliteEngine->insert(idCard);
}

void MainWindow::query() {
    QStandardItemModel *model;

    model = (QStandardItemModel*)ui->tableView->model();
    if (model == NULL) {
        model = new QStandardItemModel();
    } else {
        model->clear();
    }
    model->setColumnCount(5);
    model->setHeaderData(0,Qt::Horizontal,tr("id"));
    model->setHeaderData(1,Qt::Horizontal,tr("serial_number"));
    model->setHeaderData(2,Qt::Horizontal,tr("name"));
    model->setHeaderData(3,Qt::Horizontal,tr("company"));
    model->setHeaderData(4,Qt::Horizontal,tr("expire"));

    const QList<IdCard> idCards = sqliteEngine->query();
    QStandardItem * item = NULL;
    qDebug() << "idCards count: " << idCards.size();
    int index = 0;

    QJsonArray jsonArray;

    foreach(IdCard idCard, idCards ) {
        QJsonObject json;
        idCard.write(json);
        qDebug() << "json: " << json;
        jsonArray.append(json);

        QJsonDocument document;
        document.setObject( json );
        QByteArray bytes = document.toJson( QJsonDocument::Indented );
        QString path = "C:\\pngsuite\\idcard.json";
        QFile file( path );
        if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
        {
            QTextStream iStream( &file );
            iStream.setCodec( "utf-8" );
            iStream << bytes;
            file.close();
        }
        else
        {
            qDebug() << "file open failed: " << path << endl;
        }


        if( file.open( QIODevice::ReadOnly ) )
        {
            QByteArray bytes = file.readAll();
            file.close();

            QJsonParseError jsonError;
            QJsonDocument document = QJsonDocument::fromJson( bytes, &jsonError );
            if( jsonError.error != QJsonParseError::NoError )
            {
                qDebug() << "fromJson failed: " << jsonError.errorString() << endl;
                return ;
            }
            if( document.isObject() )
            {
                QJsonObject jsonObj = document.object();
                //...
                IdCard tt(NULL);
                tt.read(json);
                qDebug() << "json -> object: " << tt.getExpireDate();
            }
        }





        //int index = idCards.indexOf(idCard);
        qDebug() << index << "  -- " << idCard.getId() << " " << idCard.getName() << " " << idCard.getExpireDate() << " " << idCard.getCompany();
        item = new QStandardItem(QString("%1").arg(idCard.getId()));
        model->setItem(index, 0, item);

        item = new QStandardItem(QString("%1").arg(idCard.getSerialNumber()));
        model->setItem(index, 1, item);
        item = new QStandardItem(QString("%1").arg(idCard.getName()));
        model->setItem(index, 2, item);
        item = new QStandardItem(QString("%1").arg(idCard.getCompany()));
        model->setItem(index, 3, item);
        item = new QStandardItem(QString("%1").arg(idCard.getExpireDate().toString(DATE_FORMAT)));
        model->setItem(index, 4, item);
        //break;
        index ++;
    }


    QJsonDocument document;
    document.setArray(jsonArray);
    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QString path = "C:\\pngsuite\\idcards.json";
    QFile file( path );
    if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
    {
        QTextStream iStream( &file );
        iStream.setCodec( "utf-8" );
        iStream << bytes;
        file.close();
    }
    else
    {
        qDebug() << "file open failed: " << path << endl;
    }



    //model->setItem(0, 0, new QStandardItem(QString("%1").arg(1212)));


    ui->tableView->setModel(model);
    //ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    //ui->tableView->setColumnWidth(0,101);
    //ui->tableView->setColumnWidth(1,102);
    //ui->tableView->setShowGrid(true);
    //ui->tableView->show();
}

void MainWindow::initTableView() {
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setHeaderData(0,Qt::Horizontal,tr("id"));
    model->setHeaderData(1,Qt::Horizontal,tr("name"));
    model->setHeaderData(2,Qt::Horizontal,tr("expire"));


    model->setItem(0, 0, new QStandardItem(QString("%1").arg(1212)));
    ui->tableView->setModel(model);

    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //设置列宽不可变
    //ui->tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    //ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0,101);
    ui->tableView->setColumnWidth(1,102);

    ui->tableView->setShowGrid(true);

}

void MainWindow::print() {
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QModelIndexList selection = select->selectedRows();
    qDebug() << "print " << selection;

    for(int i=0; i< selection.count(); i++)
    {
        QModelIndex index = selection.at(i);
        qDebug() << index.row();
    }
}
