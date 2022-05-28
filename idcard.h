#ifndef IDCARD_H
#define IDCARD_H

#include <QObject>
#include <QDate>

#define DATE_FORMAT "yyyy-MM-dd"

class IdCard : public QObject
{
    Q_OBJECT
public:
    explicit IdCard(QObject *parent = nullptr);
    IdCard(long id, QString name, const QDate& expireDate);
    IdCard(long id, QString name);
    IdCard(QString name, const QDate &expireDate);
    IdCard(const IdCard &other);
    IdCard();
    //IdCard& operator=(const IdCard &other);
    bool operator==(const IdCard &other) const;

    void setName(QString name);
    const QString& getName() const;
    void setId(long id);
    long getId();
    void setExpireDate(QDate date);
    const QDate getExpireDate() const;

    void setCompany(QString company);
    QString getCompany() const;

    const QString &getSerialNumber() const;
    void setSerialNumber(const QString &newSerialNumber);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    void toString();
    const QString &getChipUID() const;
    void setChipUID(const QString &newChipUID);

    void setPassPhoto(QString passPhoto);
    QString getPassPhoto();

    void setPassPhotoWebP(QString passPhotoWebP);
    QString getPassPhotoWebP();
signals:

private:
    long id;

    QString name;
    QString company;
    QString serialNumber;
    QDate expireDate;
    QString chipUID;
    QString passPhoto;
    QString passPhotoWebP;
};

#endif // IDCARD_H
