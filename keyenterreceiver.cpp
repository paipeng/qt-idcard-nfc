#include "keyenterreceiver.h"

#include <QKeyEvent>
#include <QDebug>
KeyEnterReceiver::KeyEnterReceiver(QObject *parent) : QObject(parent), shift(false), ctrl(false), alt(false)
{

}

bool KeyEnterReceiver::eventFilter(QObject* obj, QEvent* event) {
    //qDebug() << "eventFilter: " << event->type();
    if (event->type()==QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        qDebug() << "key pressed: " << key->key();
        if ( (key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return) ) {
            //Enter or return was pressed
        } else if (key->key()==Qt::Key_Shift) {
            shift = true;
        } else if (key->key()==Qt::Key_Control) {
            ctrl = true;
        } else if (key->key()==Qt::Key_Alt) {
            alt = true;
        } else {
            return QObject::eventFilter(obj, event);
        }
        return true;
    } else if (event->type()==QEvent::KeyRelease) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        qDebug() << "key released: " << key->key();
        if (key->key()==Qt::Key_Shift) {
            shift = false;
        } else if (key->key()==Qt::Key_Control) {
            ctrl = false;
        } else if (key->key()==Qt::Key_Alt) {
            alt = false;
        }
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}
