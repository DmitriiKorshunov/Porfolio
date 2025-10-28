#include "settings.h"
#include "doc.h"
#include "user.h"


#include <QDir>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QMessageBox>

Settings::Settings()
{
    currentPath="//OPNIR-SERVER/iSED/ElDoc/";
    rezPatterns.append("СРОЧНО");
    rezPatterns.append("К УЧЕТУ");
    rezPatterns.append("В ДЕЛО");
    rezPatterns.append("К СВЕДЕНИЮ");
    rezPatterns.append("К ИСПОЛНЕНИЮ");
    rezPatterns.append("НЕ ВОЗРАЖАЮ");

    lastPath="C:/";

    URL="http://78.140.248.150:200";

}

Settings::~Settings()
{

}

Settings *Settings::instance()
{
    if(m_instanse == Q_NULLPTR)
        m_instanse = new Settings();
    return m_instanse;
}

void Settings::showMessage(QJsonObject json)
{
    if(json.contains("process"))
    {
        QJsonObject jsonTemp;
        QString temp;
        jsonTemp = json["process"].toObject();
        foreach (QString key,jsonTemp.keys())
            temp = temp +key+": "+(jsonTemp[key].isString()?
                                       jsonTemp[key].toString():
                                       QString::number(jsonTemp[key].toInt()))+"\n";
        QMessageBox::information(0,"Обновление данных","Обновление данных прошло успешно: \n"+temp);
    }
    else
        QMessageBox::warning(0,"Обновление данных","При обновлении данных произошла ошибка: \n"+ json["error"].toString());
}

void Settings::updateSettings()
{
    // Загрузка настроек
    QJsonObject json;
    qDebug()<<currentPath+"settings_doc.json";
    json = readJsonFromFile(currentPath+"settings_doc.json");
    if (json.contains("URL"))
        URL = json["URL"].toString();
    if (json.contains("URL"))
        URL = json["URL"].toString();
    if (json.contains("lastPath"))
        lastPath = json["lastPath"].toString();
    if (json.contains("rezPatterns") && json["rezPatterns"].isArray())
    {
        QJsonArray arr = json["rezPatterns"].toArray();
        rezPatterns.clear();
        for(int i = 0;i<arr.count();i++)
        {
            rezPatterns.append(arr[i].toString());
        }
    }

}

void Settings::saveSettings()
{
    // Сохранение настроек
    QJsonObject json;
    QJsonArray arr;
    foreach(QString pattern,rezPatterns)
        arr.append(pattern);
    json["rezPatterns"] = arr;
    json["URL"] = URL;
    json["lastPath"] = lastPath;

    saveJsonToFile(json,currentPath+"settings_doc.json");
}

UserModel *Settings::getUserModel()
{
    if(m_userModel == Q_NULLPTR)
    {
        m_userModel = new UserModel(this);
        m_userModel->setUserList(getUserList());
    }
    return m_userModel;
}

void Settings::updateUserList()
{
    if(m_userModel == Q_NULLPTR)
        m_userModel = new UserModel(this);
    userStringList.clear();
    m_userModel->setUserList(getUserList());
}

QList<User *> *Settings::getUserList()
{

    auto userList = new QList<User*>;
    QJsonObject json;
    json=readJson("/api/v1/get/user/all");
    Online = true;
    if (json.contains("error") || json["users"].toArray().count()==0)
    {
        json=readJsonFromFile(currentPath+"Users.json");
        qDebug()<< "Users read from file";
        Online = false;
    }

    if (json.contains("users") && json["users"].isArray())
    {
        userList->clear();
        QJsonArray array=json["users"].toArray();
        for(int i=0;i<array.size();i++)
        {
            auto user = new User(array[i].toObject());
            userList->append(user);
            //            qDebug()<<(user->getUuid() == curUserUUID)<<user->getName();
            userStringList.append(user->getName());
            if(user->getUuid() == curUserUUID)
                curUser = user;
        }
        qDebug()<<"Users got: "<<userList->count();
    }
    std::sort(userList->begin(),userList->end());
    return userList;
}

void Settings::saveUsersList(QList<User *> *users)
{
    QJsonObject json=saveJson(makeUsersJson(users),"/api/v1/post/users");
    saveJsonToFile(makeUsersJson(users),currentPath+"Users.json");
    showMessage(json);
}

QJsonObject Settings::makeUsersJson(QList<User *> *users)
{
    QJsonObject json;
    QJsonArray jarr;
    for(auto user:*users)
    {
        jarr.append(user->makeJson());
    }
    json["users"]=jarr;
    return json;
}

void Settings::updateDocList()
{
    docList = new QVector<Doc>;
    docListBase = new QVector<Doc>;
    docListForUpload = new QVector<Doc>;
    QJsonObject json;
    json=readJson("/api/v1/get/doc/all");

    if (json.contains("error"))
    {
        json=readJsonFromFile(currentPath+"/Docs.json");
    }

    if (json.contains("docs") && json["docs"].isArray())
    {
        QJsonArray array=json["docs"].toArray();
        for(int i=0;i<array.size();i++)
        {
            Doc doc(array[i].toObject());
            Doc docBase(array[i].toObject());
            docList->append(doc);
            docListBase->append(docBase);
        }

        qDebug()<<"Docs got from server: "<<docList->count();
    }

}

void Settings::saveDocList()
{
    QJsonObject json = saveJson(makeDocsJson(),"/api/v1/post/docs");
    saveJsonToFile(makeDocsJson(true),currentPath+"/Docs.json");
    showMessage(json);
}

QJsonObject Settings::makeDocsJson(bool all)
{
    if(docList == Q_NULLPTR)
        return QJsonObject();

    QJsonObject json;
    QJsonArray jarr;
    if(all)
    {
        for(QVector<Doc>::iterator doc=docList->begin();doc!=docList->end();doc++)
        {
            jarr.append(doc->makeJson());
        }
        for(QVector<Doc>::iterator doc=docListForUpload->begin();doc!=docListForUpload->end();doc++)
        {
            jarr.append(doc->makeJson());
        }
    }
    else
    {
        for(QVector<Doc>::iterator doc=docList->begin();doc!=docList->end();doc++)
        {
            if(doc->isMod())
                docListForUpload->append(*doc);
        }

        for(QVector<Doc>::iterator doc=docListForUpload->begin();doc!=docListForUpload->end();doc++)
        {
            jarr.append(doc->makeJson());
        }
    }

    json["docs"]=jarr;
    json["uuid_user"]=curUser->getUuid();
    json["docs_count"] = jarr.count();
    return json;
}

bool Settings::getFile(QString uuidDoc)
{

    QNetworkAccessManager qnam;
    QNetworkReply* reply=qnam.get(QNetworkRequest(QUrl(URL+"/api/v1/download/doc/"+uuidDoc)));
    QTimer timer;
    timer.setSingleShot(true);

    pgd = new QProgressDialog();
    pgd->setLabelText("Загрузка файла с сервера");
    pgd->setMinimumDuration(100);

    connect(reply, &QNetworkReply::downloadProgress, this,&Settings::slotProgressBar);
    QEventLoop loop;
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(pgd,&QProgressDialog::canceled, &loop, &QEventLoop::quit);

    timer.start(30000);   // 10 secs. timeout
    loop.exec();

    if(timer.isActive())
    {
        timer.stop();
        if(reply->error() == QNetworkReply::NoError)
        {
            // Success
            QByteArray trg=reply->readAll();
            if (trg=="{\"process\":\"file not available\"}")
            {
                qDebug()<<trg;
                return false;
            }

            QFile file(currentPath+"/tempDoc/"+uuidDoc+".pdf");
            file.open(QFile::Truncate|QFile::WriteOnly);
            if(file.isOpen())
            {
                qDebug()<<"PDF file is open for writing";
            }
            else
            {
                qDebug()<<"PDF file is NOT open for writing";
                return false;
            }
            file.write(trg);
            file.close();
            return true;

        }
        else
        {
            // handle error
            QString error = reply->errorString();
            qDebug()<<error;
            return false;
        }
    }
    else
    {
        reply->abort();
        return false;
    }
}

QJsonObject Settings::uploadFile(QJsonObject json)
{
    QJsonObject jsonDoc;
    jsonDoc=json["docs"].toObject();
    QString path = jsonDoc["path"].toString();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart jsonPart;
    jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"files\";filename=\" json\""));
    jsonPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    jsonPart.setBody(QJsonDocument(json).toJson());
    multiPart->append(jsonPart);

    if(!path.isEmpty())
    {
        QHttpPart pdfPart;
        pdfPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"files\";filename=\""+path+"\""));
        pdfPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/pdf"));
        QFile *file = new QFile(path);
        file->open(QIODevice::ReadOnly);
        pdfPart.setBodyDevice(file);
        file->setParent(multiPart); // we cannot delete the file now, so delete it with the
        multiPart->append(pdfPart);
    }

    QTimer timer;
    qDebug()<<multiPart;
    QUrl url(URL+"/api/v1/upload/doc");
    QNetworkRequest request(url);
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(request, multiPart);
    multiPart->setParent(reply);

    pgd = new QProgressDialog();
    pgd->setLabelText("Загрузка файла на сервер");
    pgd->setMinimumDuration(0);
    pgd->setAutoClose(true);

    connect(reply, &QNetworkReply::uploadProgress, this,&Settings::slotProgressBar);

    timer.setSingleShot(true);

    QEventLoop loop;
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(30000);   // 10 secs. timeout
    loop.exec();
    pgd->close();

    if(timer.isActive())
    {
        timer.stop();
        if(reply->error() == QNetworkReply::NoError)
        {
            // Успешно
            QByteArray buffer = reply->readAll();
            QJsonDocument jsonReply(QJsonDocument::fromJson(buffer));
            QJsonObject result(jsonReply.object());

            qDebug()<<buffer;
            reply->deleteLater();
            return result;
        }
        else
        {
            // Ошибка
            QString error = reply->errorString();
            qDebug()<< "reply->errorString() " << error;
            reply->deleteLater();
            QJsonDocument jsonReply(QJsonDocument::fromJson(reply->readAll()));
            return jsonReply.object();
        }
    }
    else
    {
        reply->abort();
        reply->deleteLater();
        QJsonObject result;
        result["error"] = "Time out";
        return result;
    }

}

QJsonObject Settings::saveJson(QJsonObject json, QString urlPath)
{

    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    QNetworkRequest request(QUrl(URL+urlPath));
    //QNetworkRequest request(QUrl("http://192.168.0.59:69"+urlPath));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonDocument data(json);
    QNetworkReply* reply = mgr->post(request,data.toJson());


    QTimer timer;
    timer.setSingleShot(true);

    QEventLoop loop;
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(1000);   // 10 secs. timeout

    loop.exec();

    if(timer.isActive())
    {
        timer.stop();
        if(reply->error() == QNetworkReply::NoError)
        {
            // Успешно
            QByteArray buffer = reply->readAll();
            qDebug()<<"Reply: "<<buffer;
            QJsonDocument jsonReply(QJsonDocument::fromJson(buffer));
            return jsonReply.object();
        }
        else
        {
            // Ошибка
            QString error = reply->errorString();
            qDebug()<< "reply->errorString() " << error;
            QJsonObject jsonReply;
            jsonReply["error"] = error;
        }
    }
    else
    {
        reply->abort();
    }
    reply->deleteLater();
    QJsonObject jsonReply;
    jsonReply["error"] = "error";
    return jsonReply;
}

void Settings::saveJsonToFile(const QJsonObject json, QString path)
{
    QFile saveFile(path);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qDebug()<<"Couldn't save file: "<<path ;
        return;
    }
    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
    qDebug()<<"Saved to file: "<<path;
}

QJsonObject Settings::readJson(QString urlPath)
{
    QNetworkAccessManager qnam;
    QNetworkReply* reply=qnam.get(QNetworkRequest(QUrl(URL+urlPath)));
    //QNetworkReply* reply=qnam.get(QNetworkRequest(QUrl("http://192.168.0.58:8000"+urlPath)));
    QTimer timer;
    timer.setSingleShot(true);

    QEventLoop loop;
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(1000);   // 10 secs. timeout
    loop.exec();

    if(timer.isActive())
    {
        timer.stop();
        if(reply->error() == QNetworkReply::NoError)
        {
            // Success
            this->Online=true;
            QByteArray trg=reply->readAll();
            QJsonDocument jsonDoc(QJsonDocument::fromJson(trg));
            QJsonObject json(jsonDoc.object());
            return  json;
        }
        else
        {
            // handle error
            QString error = reply->errorString();
            qDebug()<<error;
            QJsonObject json;
            json["error"]=error;
            this->Online=false;
            return  json;
        }
    }
    else
    {
        this->Online=false;
        reply->abort();
        qDebug()<<"timeout getting request";
    }
    reply->deleteLater();
    QJsonObject json;
    json["error"]="error getting from server";
    return  json;
}

QJsonObject Settings::readJsonFromFile(QString path)
{
    QFile loadFile(path);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qDebug()<<"Cant open file: "<<path;
        QJsonObject jerror;
        jerror["error"]="error reading from file";
        return jerror;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject json(loadDoc.object());
    qDebug()<<"Opened from file: "<<path;
    loadFile.close();
    return json;
}

void Settings::slotProgressBar(qint64 received, qint64 total)
{
    //    qDebug()<<100*received/total;
    if (total == 0)
        return;
    pgd->setValue(100*received/total);
}
