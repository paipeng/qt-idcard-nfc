#ifndef IDCARD_H
#define IDCARD_H

#include <QObject>
#include <QDate>

class IdCard : public QObject
{
    Q_OBJECT
public:
    explicit IdCard(QObject *parent = nullptr);
    IdCard(long id, QString name, const QDate& expireDate);
    IdCard(QString name, const QDate &expireDate);
    IdCard(const IdCard &other);
    IdCard();
    IdCard& operator=(const IdCard &other);
    bool operator==(const IdCard &other) const;

    void setName(QString name);
    QString getName();
    void setId(long id);
    long getId();
    void setExpireDate(const QDate &date);
    const QDate getExpireDate();

signals:

private:
    long id;

    QString name;
    QDate expireDate;
};

#endif // IDCARD_H
