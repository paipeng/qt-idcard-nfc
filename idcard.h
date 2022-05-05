#ifndef IDCARD_H
#define IDCARD_H

#include <QObject>

class IdCard : public QObject
{
    Q_OBJECT
public:
    explicit IdCard(QObject *parent = nullptr);
    IdCard(long id, QString name);
    IdCard(const IdCard &other);
    IdCard& operator=(const IdCard &other);
    bool operator==(const IdCard &other) const;

    void setName(QString name);
    QString getName();
    void setId(long id);
    long getId();

signals:

private:
    long id;
    QString name;
};

#endif // IDCARD_H
