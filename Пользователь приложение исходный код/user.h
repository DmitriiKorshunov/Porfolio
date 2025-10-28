#ifndef USER_H
#define USER_H

#include "QString"
#include "QFile"
#include "QVector"
#include "QtNetwork/QNetworkAccessManager"
#include "QJsonObject"

#include "QDebug"


class User
{
private:
    QString name;// Имя
    QString id; // должность
    int number;// порядковый номер
    QString resolution;// конец резолюции
    QString category;// категория пользователя
    QString uuid;
    QByteArray photo;
    QByteArray sign;

public:
    User();
    User(const QJsonObject json);

    QString getPostPosition();
    QString getName();
    int retNumber();
    QString getResolution();
    QString getCategory();
    QString getUuid();
    QByteArray retPhoto();
    QByteArray retSign();


    QJsonObject makeJson();
    bool operator <(User us);
    void setNumber(int num);

};

#endif // USER_H
