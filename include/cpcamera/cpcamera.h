#ifndef CPCAMERA_H
#define CPCAMERA_H

#include "cp-camera_global.h"


#if QT_VERSION>=0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include <QMediaService>
#include <QMediaRecorder>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QMediaMetaData>

#include <QCamera>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QScopedPointer>

#include <QMessageBox>
#include <QPalette>

#include <QtWidgets>
#include <QDebug>
#include <QPixmap>


#include <QPixmap>

class CameraInterface {
public:
    virtual ~CameraInterface() {}
    virtual void cameraState(int cameraId, int state) = 0;
    virtual void processCapturedImage(int cameraId, const QImage& img) = 0;
    virtual void cameraReadyForCapture(int cameraId, bool ready) = 0;
};

class CPCAMERA_EXPORT CPCamera : public QObject
{
public:
    CPCamera(int cameraId, CameraInterface* cameraInterface);
    ~CPCamera();
public:
    void setCamera(const QCameraInfo &cameraInfo, QCameraViewfinder *surface);
    static const QList<QCameraInfo> getAvailableCamersInfos();
    void startCamera();
    void stopCamera();

    void record();
    void pause();
    void stop();
    void setMuted(bool);

    void toggleLock();
    void takeImage();
private:

    void displayCaptureError(int, QCameraImageCapture::Error, const QString &errorString);

    void configureCaptureSettings();
    void configureVideoSettings();
    void configureImageSettings();

    void displayRecorderError();
    void displayCameraError();

    void updateCameraDevice(QAction *action);

    void updateCameraState(QCamera::State);
    void updateCaptureMode();
    void updateRecorderState(QMediaRecorder::State state);
    void setExposureCompensation(int index);

    void updateRecordTime();

    void processCapturedImage(int requestId, const QImage &img);
    void updateLockStatus(QCamera::LockStatus, QCamera::LockChangeReason);

    void displayViewfinder();
    void displayCapturedImage();

    void readyForCapture(bool ready);
    void imageSaved(int id, const QString &fileName);
private:
    QScopedPointer<QCamera> m_camera;
    QScopedPointer<QCameraImageCapture> m_imageCapture;
    QScopedPointer<QMediaRecorder> m_mediaRecorder;

    QImageEncoderSettings m_imageSettings;
    QAudioEncoderSettings m_audioSettings;
    QVideoEncoderSettings m_videoSettings;
    QString m_videoContainerFormat;
    bool m_isCapturingImage = false;
    bool m_applicationExiting = false;
    bool m_isCameraStart = false;
    bool m_saveImage = false;
    int cameraId;

    CameraInterface* cameraInterface;

};

#endif // CPCAMERA_H
