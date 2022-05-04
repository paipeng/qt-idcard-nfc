#ifndef SQLITEENGINE_H
#define SQLITEENGINE_H

#include <QObject>
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

    void insert();
    void query();

signals:
private:
    QSqlDatabase *database;

};

#endif // SQLITEENGINE_H
