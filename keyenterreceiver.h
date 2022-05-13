#ifndef KEYENTERRECEIVER_H
#define KEYENTERRECEIVER_H

#include <QObject>
#include <QKeyEvent>

class KeyEnterReceiver : public QObject
{
    Q_OBJECT
public:
    explicit KeyEnterReceiver(QObject *parent = nullptr);
protected:
    bool eventFilter(QObject* obj, QEvent* event);
signals:
    Q_SIGNAL void keyEvent(int keyCode, bool shift, bool ctrl, bool alt);
private:
    bool shift;
    bool ctrl;
    bool alt;
};

#endif // KEYENTERRECEIVER_H
