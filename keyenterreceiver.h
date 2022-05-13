#ifndef KEYENTERRECEIVER_H
#define KEYENTERRECEIVER_H

#include <QObject>

class KeyEnterReceiver : public QObject
{
    Q_OBJECT
public:
    explicit KeyEnterReceiver(QObject *parent = nullptr);
protected:
    bool eventFilter(QObject* obj, QEvent* event);
signals:

private:
    bool shift;
    bool ctrl;
    bool alt;
};

#endif // KEYENTERRECEIVER_H
