#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sqliteengine.h"
#include "cpcamera.h"
#include "barcodedecoder.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, CameraInterface
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
    void initCameras();
    void startCamera1();
    void updateBarcodeDecodeResult(int decodeState);

private:

private:
    void cameraState(int cameraId, int state) override;
    void processCapturedImage(int cameraId, const QImage& img) override;
    void cameraReadyForCapture(int cameraId, bool ready) override;

private:
    Ui::MainWindow *ui;
    SqliteEngine * sqliteEngine;
    CPCamera camera1;
    BarcodeDecoder barcodeDecoder;
    QElapsedTimer timer;
};
#endif // MAINWINDOW_H
