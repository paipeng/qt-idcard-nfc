#ifndef SQLITEENGINE_H
#define SQLITEENGINE_H

#include <QObject>
#include "idcard.h"
class QSqlDatabase;

class SqliteEngine : public QObject
{
    Q_OBJECT
public:
    explicit SqliteEngine(QObject *parent = nullptr);
    ~SqliteEngine();

public:
    void initDB();
    void close();

    int insert(IdCard &idCard);
    QList<IdCard> query();
    IdCard getIdCardById(long id);
    IdCard updateChipUID(IdCard idCard, QString chipUID);
    IdCard getIdCardBySerialNumber(QString serialNumber);
    IdCard getIdCardByChipUID(QString chipUID);
    IdCard updatePassPhoto(IdCard idCard, QString passPhoto, QString passPhotoWebP);

signals:
private:
    QSqlDatabase *database;

};

#endif // SQLITEENGINE_H
