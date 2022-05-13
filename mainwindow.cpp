#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableView>
#include <QStandardItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QVariant>
#include <QDebug>

#include "barcodeencoder.h"
#include "pdfwriter.h"
#include "common-util.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), camera1(0, this), nfc(this) {
    ui->setupUi(this);
    sqliteEngine = new SqliteEngine();
    //sqliteEngine->initDB();
    initTableView();
    query();

    QObject::connect(&barcodeDecoder, &BarcodeDecoder::updateBarcodeDecodeResult, this, &MainWindow::updateBarcodeDecodeResult);
    barcodeDecoder.start();
    barcodeDecoder.setPriority(QThread::LowestPriority);

    initCameras();

    //BarcodeEncoder barcodeEncoder;
    //barcodeEncoder.encode("TEST1234");


    //PDFWriter pdfWriter;
    //pdfWriter.test("C:\\pngsuite\\code.pdf");

    nfc.openDevice();
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

    if (sqliteEngine->insert(idCard) == 0) {
        // insert success
        // show alert message dialog
        QMessageBox::information(this, tr("idcard_insert_title"), tr("idcard_insert_success"), QMessageBox::Ok);
        // upload table
        query();
    }
}

void MainWindow::query() {
    QStandardItemModel *model;

    model = (QStandardItemModel*)ui->tableView->model();
    if (model == NULL) {
        model = new QStandardItemModel();
    } else {
        model->clear();
    }
    model->setColumnCount(6);
    model->setHeaderData(0,Qt::Horizontal,tr("id"));
    model->setHeaderData(1,Qt::Horizontal,tr("serial_number"));
    model->setHeaderData(2,Qt::Horizontal,tr("name"));
    model->setHeaderData(3,Qt::Horizontal,tr("company"));
    model->setHeaderData(4,Qt::Horizontal,tr("expire"));
    model->setHeaderData(5,Qt::Horizontal,tr("chip_uid"));

    const QList<IdCard> idCards = sqliteEngine->query();
    QStandardItem * item = NULL;
    qDebug() << "idCards count: " << idCards.size();
    int index = 0;

    QJsonArray jsonArray;

    foreach(IdCard idCard, idCards ) {
#if 0
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
#endif




        //int index = idCards.indexOf(idCard);
        qDebug() << index << "  -- " << idCard.getId() << " " << idCard.getName() << " " << idCard.getExpireDate() << " " << idCard.getCompany()
                 << " " << idCard.getChipUID();
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
        item = new QStandardItem(QString("%1").arg(idCard.getChipUID()));
        model->setItem(index, 5, item);
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
    ui->tableView->setColumnWidth(5,180);
    //ui->tableView->setShowGrid(true);
    //ui->tableView->show();
}

void MainWindow::initTableView() {
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,tr("id"));
    model->setHeaderData(1,Qt::Horizontal,tr("name"));
    model->setHeaderData(2,Qt::Horizontal,tr("expire"));
    model->setHeaderData(3,Qt::Horizontal,tr("chip_uid"));


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
    IdCard idCard = getSelectedIdCard();
    if (idCard.getId() > 0) {
        qDebug() << "print to pdf";
        PDFWriter pdfWriter;

        QString fileName = "C:\\pngsuite\\idcard_";
        fileName += idCard.getSerialNumber();
        fileName += ".pdf";
        pdfWriter.generateIdCard(idCard, fileName);
    }
}

void MainWindow::tableViewClicked(QModelIndex index) {
    qDebug() << "tableViewClicked: " << index;
    QVariant variant = index.data();
    qDebug() << "variant: " << variant;
    if (variant.type() == QVariant::String) {
        IdCard idCard = sqliteEngine->getIdCardById(variant.toString().toInt());
        updateInputTextField(idCard);
    }
}

void MainWindow::clear() {
    qDebug() << "clear";
    ui->nameLineEdit->setText("");
    ui->expireDateEdit->setDate(QDate::fromString("2022-01-01", DATE_FORMAT));
    ui->companyLineEdit->setText("");
    ui->serialNumberLineEdit->setText("");
}
void MainWindow::scan() {
    startCamera1();
}

IdCard MainWindow::getSelectedIdCard() const {
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QModelIndexList selection = select->selectedRows();
    qDebug() << "print " << selection;

    for(int i=0; i< selection.count(); i++)
    {
        QModelIndex index = selection.at(i);
        qDebug() << "selected row index: " << index.row() << "data: " << index.data();
#if 0
        QStandardItemModel *model = (QStandardItemModel*)ui->tableView->model();
        QStandardItem *item = model->item(index.row(), 1);
        qDebug() << "item: " << item;
#endif
        QVariant variant = index.data();
        qDebug() << "variant: " << variant;
        int value = 0;
        qDebug() << "variant type: " << variant.type() << " value: " << variant.toString();
        if (variant.type() == QVariant::Int) {
            value = variant.toInt();
        } else if (variant.type() == QVariant::String) {
            value = variant.toString().toInt();
        } else if (variant.type() == QVariant::Invalid) {
            value = variant.toString().toInt();
        }
        //int value = variant.toInt();
        return sqliteEngine->getIdCardById(value);
    }
    return IdCard(NULL);
}
void MainWindow::updateIdCardChipUID(QString chipUID) {
    qDebug() << "updateIdCardChipUID: " << chipUID;
    IdCard idCard = getSelectedIdCard();
    if (idCard.getId() > 0) {
        IdCard idCard2 = sqliteEngine->updateChipUID(idCard, chipUID);
    }
}

void MainWindow::updateInputTextField(IdCard idCard) {
    qDebug() << "updateInputTextField: " << idCard.getId();
    ui->nameLineEdit->setText(idCard.getName());
    ui->expireDateEdit->setDate(idCard.getExpireDate());
    ui->companyLineEdit->setText(idCard.getCompany());
    ui->serialNumberLineEdit->setText(idCard.getSerialNumber());
    ui->chipUIDLineEdit->setText(idCard.getChipUID());
}

void MainWindow::initCameras() {
    qDebug() << "initCameras";
    const QList<QCameraInfo> availableCameras = CPCamera::getAvailableCamersInfos();

    int index = 0;
    for (const QCameraInfo &cameraInfo : availableCameras) {
        qDebug() << "cameraInfo: " << cameraInfo.description();
        //ui->camera1ComboBox->addItem(cameraInfo.description(), index);
        //ui->camera2ComboBox->addItem(cameraInfo.description(), index);
        if (index != 0) {
            camera1.setCamera(cameraInfo, ui->cpCameraViewFinder);
            break;
        }
        index ++;
    }
}

void MainWindow::startCamera1() {
    qDebug() << "startCamera1";
    camera1.startCamera();
}



void MainWindow::cameraState(int cameraId, int state) {
    qDebug() << "cameraState: " << cameraId << " state: " << state;
    if (state == 0) {
        ui->scanPushButton->setText(tr("scan"));
    } else if (state == 1) {
        ui->scanPushButton->setText(tr("scan"));
        //camera2AutoCapture = true;
    } else if (state == 2) {
        ui->scanPushButton->setText(tr("stop"));

    }

}
void MainWindow::processCapturedImage(int cameraId, const QImage& img) {
    qDebug() << "processCapturedImage: " << cameraId << " img: " << img.width() << "-" << img.height();
    timer.start();
    if (cameraId == 0) {
        barcodeDecoder.setImage(img);
    }
}
void MainWindow::cameraReadyForCapture(int cameraId, bool ready) {
    qDebug() << "cameraReadyForCapture: " << cameraId << " state: " << ready;

    if (ready) {
        camera1.takeImage();
    }
}


void MainWindow::updateBarcodeDecodeResult(int decodeState) {
    Q_UNUSED(decodeState);
    qint64 t = timer.elapsed();

    qDebug() << "updateBarcodeDecodeResult: " << decodeState << " elapsed time: " << t;

    if (decodeState == 0) {
        for (auto&& result : barcodeDecoder.decodeResults) {
            //Beep(800, 2000);
            qDebug() << "RESULT: " << result.text();
            QString text = QString::fromWCharArray(result.text().c_str());
            ui->serialNumberLineEdit->setText(text);
            break;
        }
    } else {
        ui->serialNumberLineEdit->setText(QString(""));
    }

}

void MainWindow::getDeviceList(QStringList deviceNameList) {
    qDebug() << "getDeviceList";
    int index = 0;
    for (QString device : deviceNameList) {
        qDebug() << "device: " << device << " len: " << device.length();
        if (device.length() > 0) {
            ui->deviceComboBox->addItem(device, index++);
            //qDebug() << "device--: " << device.data();
        }
    }
    if (index > 0) {
        ui->nfcPushButton->setDisabled(false);
        ui->statusbar->showMessage(QString("device Ready"));
    } else {
        QString errStr;
        errStr.sprintf("no device found %d", index);
        QMessageBox::critical(NULL, "critical", errStr, QMessageBox::Ok, QMessageBox::Ok);
    }
}

void MainWindow::updateStatusBarMessage(QString message) {

}

void MainWindow::receiveResponse(unsigned char* data, int data_len) {
    qDebug() << "receiveResponse len: " << data_len;
    if (data_len != MAX_RESPONSE && data_len > 2) {
        // convert data -> hex
        QString response;
        for (int i = 0; i < data_len-2; i++) {
            QString hex;
            hex.sprintf("%02X", data[i]);
            response.append(hex);
        }
        // show
        qDebug() << response;

        ui->chipUIDLineEdit->setText(response);

        // update idcard
        updateIdCardChipUID(response);
    }
}

void MainWindow::addLog(unsigned char* data, int data_len, int direction) {
    QString text;
    if (direction == 0) {
        text.append("<font color=\"green\">");
        text.append("C-APDU -- ");
    } else {
        text.append("<font color=\"blue\">");
        text.append("R-APDU -- ");
    }

    for (int i = 0; i < data_len; i++) {
        QString c;
        c.sprintf("0x%02X ", data[i]);
        text.append(c);
    }
    text.append(" (");
    text.append(QString::number(data_len));
    text.append(")");


    text.append("</font>");

    qDebug() << "addLog: " << text;
    //ui->logTextEdit->append(text);
}

void MainWindow::addLog2(QString text, int state) {
    qDebug() << "addLog2: " << text << " state: " << state;
}

void MainWindow::readNFC() {
    qDebug() << "connect: " << nfc.isDeviceConnected();
    if (nfc.isDeviceConnected()) {
        int errorCode = nfc.readUID();
        qDebug() << "readUID: " << errorCode;
        if (errorCode == 0x9000) {
            // read ndef data
            int payload_type;
            unsigned char* data = NULL;
            int data_len = 0;
            data = nfc.readNDEFText(&payload_type, &data_len);
            qDebug() << "readNDEFText: " << data_len;
        }
    } else {
        qDebug() << "connectDevice: " << ui->deviceComboBox->currentIndex();
        if (nfc.connectDevice(ui->deviceComboBox->currentIndex()) == 0) {
            switch (nfc.getProtocolType()) {
                case SCARD_PROTOCOL_T0:
                    ui->statusbar->showMessage(QString("SCardConnect ok Protocol Type = T0"));
                    break;
                case SCARD_PROTOCOL_T1:
                    ui->statusbar->showMessage(QString("SCardConnect ok Protocol Type = T1"));
                    break;
                default:
                    QString msg;
                    msg.sprintf("SCardConnect ok Protocol Type: %d", nfc.getProtocolType());
                    ui->statusbar->showMessage(msg);
                    break;
            }
            int errorCode = nfc.readUID();
            qDebug() << "readUID: " << errorCode;
            if (errorCode == 0x9000) {
                // read ndef data
                int payload_type;
                unsigned char* data = NULL;
                int data_len = 0;
                data = nfc.readNDEFText(&payload_type, &data_len);
                qDebug() << "readNDEFText: " << " data_len: " << data_len;
                if (data != NULL) {
                    qDebug() << "data: " << data_len;
                    for (int i = 0; i < 10; i++) {
                        QString t;
                        t.sprintf("0x%02X ", data[i]);
                        qDebug() << "data: " << t;
                    }
                    //QString text(std::string((char*)data));
                    std::string str = (char*)data;
                    QString text = QString::fromStdString(str);
                    free(data);
                    qDebug() << "ndef: " << text;
                    IdCard idCard = convertStringToIdCard(text);
                    qDebug() << "name: " << idCard.getName();
                    IdCard idCardDB = sqliteEngine->getIdCardBySerialNumber(idCard.getSerialNumber());
                    if (compareIdCards(idCard, idCardDB)) {
                        updateInputTextField(idCardDB);
                    } else {

                    }
                }
            }
            //updateDeviceState(1);
        }
    }
}



void MainWindow::writeNFC() {
    qDebug() << "writeNFC";
    if (nfc.isDeviceConnected()) {
        IdCard idCard = getSelectedIdCard();
        // convert IdCard object to text data
        const QString data = convertIdCardToString(idCard);
        qDebug() << "write NDEF data: " << data;
        nfc.writeNDEFText(data.toStdString().data(), (int)strlen(data.toStdString().data()));
    }
}

