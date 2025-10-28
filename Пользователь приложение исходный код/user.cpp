#include "user.h"

#include <QDataStream>
#include <QEventLoop>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QTimer>


QString User::getPostPosition()
{
    // Возвращает ИД пользователя
    return id;
}

QString User::getName()
{
    // Возвращает имя пользователя
    return name;
}

int User::retNumber()
{
    return number;
}

void User::setNumber(int num)
{
    this->number=num;
}

QString User::getResolution()
{
    return resolution;
}

QString User::getCategory()
{
    return category;
}

QString User::getUuid()
{
    return uuid;
}

QByteArray User::retPhoto()
{
    return photo;
}

QByteArray User::retSign()
{
     return sign;
}

QJsonObject User::makeJson()
{
    QJsonObject json;
    json["uuid_user"]=this->uuid;
    json["name"]=this->name;
    json["count"]=QString::number(this->number);
    json["end_resolution"]=this->resolution;
    json["category"]=this->category;
    json["post_position"]=this->id;
    json["photo"]= QString(this->photo.toBase64());
    json["sign_path"]= QString(this->sign.toBase64());
    return json;
}

bool User::operator <(User us)
{
    return (this->name)<(us.name);
}

User::User()
{


}

User::User(const QJsonObject json)
{
    if (json.contains("name") && json["name"].isString())
        this->name = json["name"].toString();
    if (json.contains("count") && json["count"].isString())
        this->number = json["count"].toString().toInt();
    if (json.contains("end_resolution") && json["end_resolution"].isString())
        this->resolution = json["end_resolution"].toString();
    if (json.contains("category") && json["category"].isString())
        this->category = json["category"].toString();
    if (json.contains("uuid_user") && json["uuid_user"].isString())
        this->uuid = json["uuid_user"].toString();
    if (json.contains("post_position") && json["post_position"].isString())
        this->id = json["post_position"].toString();
    if (json.contains("photo") && json["photo"].isString())
        this->photo = QByteArray::fromBase64(json["photo"].toString().toUtf8());
    if (json.contains("sign_path") && json["sign_path"].isString())
        this->sign = QByteArray::fromBase64(json["sign_path"].toString().toUtf8());

}
