#ifndef CPCAMERAVIEWFINDER_H
#define CPCAMERAVIEWFINDER_H

#include "cp-camera_global.h"
#include <QCameraViewfinder>
#include <QPainter>

class CPCAMERA_EXPORT CPCameraViewfinder : public QCameraViewfinder {
    Q_OBJECT
public:
    CPCameraViewfinder(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // CPCAMERAVIEWFINDER_H
