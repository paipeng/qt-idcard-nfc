#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sqliteengine.h"
#include "cpcamera.h"
#include "barcodedecoder.h"

#define CP_SMARTCARD_LIBRARY 0
#include "nfc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, CameraInterface, SmartCardInterface
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void initDB();


private slots:
    void insert();
    void query();
    void initTableView();
    void print();
    void scan();
    void readNFC();
    void initCameras();
    void startCamera1();
    void updateBarcodeDecodeResult(int decodeState);
    void tableViewClicked(QModelIndex index);
    void clear();

private:
    void updateIdCardChipUID(QString chipUID);
    IdCard getSelectedIdCard() const;
    void updateInputTextField(IdCard idCard);

private:
    void cameraState(int cameraId, int state) override;
    void processCapturedImage(int cameraId, const QImage& img) override;
    void cameraReadyForCapture(int cameraId, bool ready) override;

private:
    void getDeviceList(QStringList deviceNameList) override;
    void updateStatusBarMessage(QString message) override;
    void receiveResponse(unsigned char* data, int data_len) override;
    void addLog(unsigned char* data, int data_len, int direction) override;
    void addLog2(QString text, int state) override;

private:
    Ui::MainWindow *ui;
    SqliteEngine * sqliteEngine;
    CPCamera camera1;
    BarcodeDecoder barcodeDecoder;
    QElapsedTimer timer;

    NFC nfc;
};
#endif // MAINWINDOW_H
