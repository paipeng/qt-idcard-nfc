#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sqliteengine.h"
#include "cpcamera.h"
#include "barcodedecoder.h"
#include "keyenterreceiver.h"

#define CP_SMARTCARD_LIBRARY 0
#include "nfc.h"
#include "fm1208.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define USE_NFC 0

class QDeviceWatcher;

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
    void load();
    void initTableView();
    void print();
    void scan();
    void readNFC();
    void initCameras();
    void startCamera1();
    void updateBarcodeDecodeResult(int decodeState);
    void tableViewClicked(QModelIndex index);
    void clear();
    void writeNFC();
    void keyEvent(int keyCode, bool shift, bool ctrl, bool alt);


public slots:
    void toggleWatch();
    void showDetail(bool show);
    void slotDeviceAdded(const QString &dev);
    void slotDeviceRemoved(const QString &dev);
    void slotDeviceChanged(const QString &dev);
private:
    void updateIdCardChipUID(QString chipUID);
    IdCard getSelectedIdCard() const;
    void updateInputTextField(IdCard idCard);
    int connectSmartCardDevice();

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
    FM1208 fm1208;
    KeyEnterReceiver keyEnterReceiver;

    QDeviceWatcher *watcher;
};
#endif // MAINWINDOW_H
