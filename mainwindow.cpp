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

#include "qdevicewatcher.h"
#include "clickablelabel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), camera1(0, this), nfc(this), fm1208(this), keyEnterReceiver(this) {
    ui->setupUi(this);
    sqliteEngine = new SqliteEngine();
    //sqliteEngine->initDB();
    initTableView();
    load();

    QObject::connect(&barcodeDecoder, &BarcodeDecoder::updateBarcodeDecodeResult, this, &MainWindow::updateBarcodeDecodeResult);
    barcodeDecoder.start();
    barcodeDecoder.setPriority(QThread::LowestPriority);

    initCameras();

    //BarcodeEncoder barcodeEncoder;
    //barcodeEncoder.encode("TEST1234");


    //PDFWriter pdfWriter;
    //pdfWriter.test("C:\\pngsuite\\code.pdf");
#if USE_NFC
    nfc.openDevice();
#else
    fm1208.openDevice();
#endif

    QObject::connect(&keyEnterReceiver, &KeyEnterReceiver::keyEvent, this, &MainWindow::keyEvent);

    this->installEventFilter(&keyEnterReceiver);


    watcher = new QDeviceWatcher;
    watcher->appendEventReceiver(this);
    connect(watcher,
            SIGNAL(deviceAdded(QString)),
            this,
            SLOT(slotDeviceAdded(QString)),
            Qt::DirectConnection);
    connect(watcher,
            SIGNAL(deviceChanged(QString)),
            this,
            SLOT(slotDeviceChanged(QString)),
            Qt::DirectConnection);
    connect(watcher,
            SIGNAL(deviceRemoved(QString)),
            this,
            SLOT(slotDeviceRemoved(QString)),
            Qt::DirectConnection);

    toggleWatch();

    QObject::connect(ui->photoLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
}

MainWindow::~MainWindow() {
    delete sqliteEngine;
    delete ui;
    delete watcher;
}

void MainWindow::insert() {
    qDebug() << "insert: " << ui->nameLineEdit->text();

    if (ui->nameLineEdit->text().size() > 0 && ui->companyLineEdit->text().size() > 0 &&
            ui->serialNumberLineEdit->text().size() > 0) {
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
            load();
        }
    }
}

void MainWindow::load() {
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

void MainWindow::query() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("query_idcard"),
                                        tr("query_data"), QLineEdit::Normal,
                                        "", &ok);
    // QDir::home().dirName()
    if (ok && !text.isEmpty()) {
        qDebug() << "text: " << text;
        QStringList str = text.split(" ");
        if (str.size() == 2) {
            IdCard idCard = sqliteEngine->getIdCardBySerialNumber(str[0]);
            if (idCard.getId() > 0) {
                updateInputTextField(idCard);
            }
        }
    }
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
        try {
            if (pdfWriter.generateIdCard(idCard, fileName) == 0) {
                int ret = QMessageBox::information(this, tr("idcard_print_title"),
                                         tr("idcard_print_success"), QMessageBox::Ok, QMessageBox::Close);
                switch (ret) {
                   case QMessageBox::Ok:
                       // TODO open pdf
                        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
                       break;
                   default:
                       // should never be reached
                       break;
                 }
            } else {
               QMessageBox::critical(this, tr("idcard_print_title"), tr("idcard_print_error"), QMessageBox::Ok);
            }

        } catch(...) {
           qDebug() << "generateIdCard error";
        }
    } else {
        QMessageBox::critical(this, tr("idcard_print_title"), tr("no_idcard_in_tableview_selected"), QMessageBox::Ok);
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
        if (idCard.getChipUID().size() <= 0) {
            IdCard idCard2 = sqliteEngine->updateChipUID(idCard, chipUID);
            updateInputTextField(idCard2);
            QMessageBox::information(this, tr("idcard_write_chip_uid_title"), tr("idcard_write_chip_uid"), QMessageBox::Ok);
            load();
        } else {
            // do nothing
            QMessageBox::critical(this, tr("idcard_write_chip_uid_title"), tr("invalid_idcard_in_tableview_selected"), QMessageBox::Ok);

        }
    } else {
        QMessageBox::critical(this, tr("idcard_write_chip_uid_title"), tr("no_idcard_in_tableview_selected"), QMessageBox::Ok);
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
        //updateIdCardChipUID(response);
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

    //qDebug() << "addLog: " << text;
    //ui->logTextEdit->append(text);
}

void MainWindow::addLog2(QString text, int state) {
    //qDebug() << "addLog2: " << text << " state: " << state;
}


int MainWindow::connectSmartCardDevice() {
    qDebug() << "connectSmartCardDevice: " << ui->deviceComboBox->currentIndex();
#if USE_NFC
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
        return 0;
    } else {
        return -1;
    }
#else
    if (fm1208.connectDevice(ui->deviceComboBox->currentIndex()) == 0) {
        switch (fm1208.getProtocolType()) {
            case SCARD_PROTOCOL_T0:
                ui->statusbar->showMessage(QString("SCardConnect ok Protocol Type = T0"));
                break;
            case SCARD_PROTOCOL_T1:
                ui->statusbar->showMessage(QString("SCardConnect ok Protocol Type = T1"));
                break;
            default:
                QString msg;
                msg.sprintf("SCardConnect ok Protocol Type: %d", fm1208.getProtocolType());
                ui->statusbar->showMessage(msg);
                break;
        }
        return 0;
    } else {
        return -1;
    }
#endif
}

void MainWindow::readNFC() {
#if USE_NFC
    qDebug() << "readNFC connect: " << nfc.isDeviceConnected();
    nfc.disconnectDevice();
    if (nfc.isDeviceConnected()) {
        int errorCode = nfc.readUID();
        qDebug() << "readUID: " << errorCode;
        if (errorCode == 0x9000) {
            // read ndef data
            int payload_type;
            unsigned char* data = NULL;
            int data_len = 0;
            data = nfc.readNDEFText(&payload_type, &data_len);
            if (data != NULL) {
                qDebug() << "readNDEFText: " << data_len;
            }
        }
    } else {
        if (connectSmartCardDevice() == 0) {
            int errorCode = nfc.readUID();
            qDebug() << "readUID: " << errorCode;
            if (errorCode == 0x9000) {
                // convert data -> hex
                unsigned char * data2 = nfc.getResponseBuffer();
                unsigned long data_len2 = nfc.getResponseLength();
                if (data_len2 > 2) {
                    QString response;
                    for (int i = 0; i < data_len2-2; i++) {
                        QString hex;
                        hex.sprintf("%02X", data2[i]);
                        response.append(hex);
                    }
                    IdCard idCard3 = sqliteEngine->getIdCardByChipUID(response);
                    if (idCard3.getId() <= 0) {
                        updateIdCardChipUID(response);
                    } else {
                        qDebug() << "chip uid already registered!";
                    }
                }
                // read ndef data
                int payload_type;
                unsigned char* data = NULL;
                int data_len = 0;
                data = nfc.readNDEFText(&payload_type, &data_len);
                qDebug() << "readNDEFText: " << " data_len: " << data_len;
                if (data != NULL) {
                    //QString text(std::string((char*)data));
                    std::string str((char*)data, data_len-3);
                    QString text = QString::fromStdString(str);
                    free(data);
                    qDebug() << "ndef: " << text;
                    IdCard idCard = convertStringToIdCard(text);
                    qDebug() << "name: " << idCard.getName();
                    IdCard idCardDB = sqliteEngine->getIdCardBySerialNumber(idCard.getSerialNumber());
                    if (compareIdCards(idCard, idCardDB)) {
                        updateInputTextField(idCardDB);
                    } else {
                        QMessageBox::critical(this, tr("idcard_read_chip_title"), tr("idcard_chip_uid_not_found"), QMessageBox::Ok);

                    }
                }
            }
            //updateDeviceState(1);
        }
    }
#else
    qDebug() << "readNFC connect: " << fm1208.isDeviceConnected();
    fm1208.disconnectDevice();
    if (fm1208.isDeviceConnected()) {
        fm1208.disconnectDevice();
    }
    if (connectSmartCardDevice() == 0) {
        int errorCode = fm1208.readUID();
        qDebug() << "readUID: " << errorCode;
        if (errorCode == 0x9000) {
            // convert data -> hex
            unsigned char * data2 = fm1208.getResponseBuffer();
            unsigned long data_len2 = fm1208.getResponseLength();
            if (data_len2 > 2) {
                QString response;
                for (int i = 0; i < data_len2-2; i++) {
                    QString hex;
                    hex.sprintf("%02X", data2[i]);
                    response.append(hex);
                }
                IdCard idCard3 = sqliteEngine->getIdCardByChipUID(response);
                if (idCard3.getId() <= 0) {
                    updateIdCardChipUID(response);
                } else {
                    qDebug() << "chip uid already registered!";
                }
            }
            // read ndef data
            short fileId = 0x01;
            unsigned char* data = fm1208.readFileData(fileId, 0x400);
            qDebug() << "readNDEFText: " << " data_len: " << strlen((char*)data);
            if (data != NULL) {
                //QString text(std::string((char*)data));
                int dl = 0;
                for (int i = 0; i < strlen((char*)data); i++) {
                    dl = i;
                    if (data[i] == 0x00) {
                        qDebug() << "found 0x00 " << i;

                        break;
                    }
                }
                qDebug() << "readNDEFText: " << " data_len: " << dl;
                std::string str((char*)data, dl+1);
                QString text = QString::fromStdString(str);
                free(data);
                qDebug() << "ndef: " << text;
                IdCard idCard = convertStringToIdCard(text);
                qDebug() << "name: " << idCard.getName();
                IdCard idCardDB = sqliteEngine->getIdCardBySerialNumber(idCard.getSerialNumber());
                if (compareIdCards(idCard, idCardDB)) {
                    updateInputTextField(idCardDB);
                } else {
                    QMessageBox::critical(this, tr("idcard_read_chip_title"), tr("idcard_chip_uid_not_found"), QMessageBox::Ok);
                }
                //free(data);
            }
        }
        //updateDeviceState(1);
    }
#endif
}

void MainWindow::writeNFC() {
    qDebug() << "writeNFC";
#if USE_NFC
    nfc.disconnectDevice();
    if (!nfc.isDeviceConnected()) {
        connectSmartCardDevice();
    }
    if (nfc.isDeviceConnected()) {
#else
    fm1208.disconnectDevice();
    if (!fm1208.isDeviceConnected()) {
        connectSmartCardDevice();
    }
    if (fm1208.isDeviceConnected()) {
#endif
        bool ok;
        QString text = QInputDialog::getText(this, tr("write_nfc_idcard"),
                                            tr("write_nfc_data"), QLineEdit::Normal,
                                            "", &ok);
        // QDir::home().dirName()
        if (ok && !text.isEmpty()) {
            qDebug() << "text: " << text;
            QStringList str = text.split(" ");
            if (str.size() == 2) {
                IdCard idCard = sqliteEngine->getIdCardBySerialNumber(str[0]);
                // check chip uid
                if (idCard.getId() > 0) {
#if USE_NFC
                    int errorCode = nfc.readUID();
#else
                    int errorCode = fm1208.readUID();

#endif
                    qDebug() << "readUID: " << errorCode;
                    if (errorCode == 0x9000) {
                        // convert data -> hex
#if USE_NFC
                        unsigned char * data2 = nfc.getResponseBuffer();
                        unsigned long data_len2 = nfc.getResponseLength();
#else
                        unsigned char * data2 = fm1208.getResponseBuffer();
                        unsigned long data_len2 = fm1208.getResponseLength();
#endif
                        if (data_len2 > 2) {
                            QString response;
                            for (int i = 0; i < data_len2-2; i++) {
                                QString hex;
                                hex.sprintf("%02X", data2[i]);
                                response.append(hex);
                            }

                            if (idCard.getChipUID() == str[1] && str[1] == response) {
                                updateInputTextField(idCard);


                                //IdCard idCard = getSelectedIdCard();
                                // convert IdCard object to text data
                                const QString data = convertIdCardToString(idCard);
                                qDebug() << "write NDEF data: " << data;
#if USE_NFC
                                nfc.writeNDEFText(data.toStdString().data(), (int)strlen(data.toStdString().data()));
#else
                                fm1208.writeFileData(0x01, 0x400, (unsigned char*)data.toStdString().data(), (int)strlen(data.toStdString().data()));

#endif
                                QMessageBox::information(this, tr("idcard_write_nfc_title"), tr("idcard_write_nfc_success"), QMessageBox::Ok);

                            } else {
                                QMessageBox::critical(this, tr("idcard_write_nfc_title"), tr("idcard_chip_uid_invalid"), QMessageBox::Ok);

                            }
                        } else {
                            QMessageBox::critical(this, tr("idcard_write_nfc_title"), tr("idcard_get_smartcard_uid_error"), QMessageBox::Ok);

                        }
                    } else {
                        QMessageBox::critical(this, tr("idcard_write_nfc_title"), tr("idcard_no_smartcard_found"), QMessageBox::Ok);
                    }


                } else {
                    QMessageBox::critical(this, tr("idcard_write_nfc_title"), tr("idcard_data_invalid"), QMessageBox::Ok);
                }
            } else {
                QMessageBox::critical(this, tr("idcard_write_nfc_title"), tr("qrcode_invalid"), QMessageBox::Ok);
            }
        }
    } else {
        QMessageBox::critical(this, tr("idcard_write_nfc_title"), tr("nfc_device_not_connected"), QMessageBox::Ok);
    }

}

void MainWindow::keyEvent(int keyCode, bool shift, bool ctrl, bool alt) {
    Q_UNUSED(keyCode);
    qDebug() << "keyEvent: " << keyCode << " shift: " << shift << " ctrl: " << ctrl << " alt: " << alt;
}


void MainWindow::toggleWatch() {
    if (watcher->isRunning()) {
        if (!watcher->stop()) {
            QMessageBox::warning(0, tr("WARNGING"), tr("Failed to stop"));
        }
    } else {
        if (!watcher->start()) {
            QMessageBox::warning(0, tr("WARNGING"), tr("Failed to start"));
        }
    }
}

void MainWindow::showDetail(bool show) {
}

void MainWindow::slotDeviceAdded(const QString &dev) {
    qDebug("tid=%#x: add %s", (quintptr) QThread::currentThreadId(), qPrintable(dev));
}

void MainWindow::slotDeviceRemoved(const QString &dev) {
    qDebug("tid=%#x: remove %s", (quintptr) QThread::currentThreadId(), qPrintable(dev));
}

void MainWindow::slotDeviceChanged(const QString &dev) {
    qDebug("tid=%#x: change %s", (quintptr) QThread::currentThreadId(), qPrintable(dev));
    // TODO check USB (camera, smartcard reader, qrcode scanner, printer)
}

void MainWindow::labelClicked() {
    qDebug() << "labelClicked";

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("open_image"), "/Users/paipeng/Documents", tr("image_file_format"));
    qDebug() << "selected file: " << fileName;

    QImage image = QImage(fileName);
    QPixmap pixmap = QPixmap::fromImage(image);

    int w = ui->photoLabel->width();
    int h = ui->photoLabel->height();

    ui->photoLabel->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));

}
