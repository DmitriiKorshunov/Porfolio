#ifndef SETTINGS_H
#define SETTINGS_H

#include "doc.h"
#include "user.h"
#include "usermodel.h"

#include <QString>
#include <QStringList>
#include <QFile>
#include <QDataStream>
#include <QDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QProgressDialog>



class Settings: public QObject
{
     Q_OBJECT
public:
    static Settings *instance();
    void showMessage(QJsonObject json);

    void updateSettings();
    void saveSettings();

    UserModel* getUserModel();
    void updateUserList();

    void updateDocList();
    void saveDocList();
    QJsonObject makeDocsJson(bool all = false);

    bool getFile(QString uuidDoc);
    QJsonObject uploadFile(QJsonObject json);

    QJsonObject saveJson(QJsonObject json,QString urlPath);
    void saveJsonToFile(const QJsonObject json, QString path);

    QJsonObject readJson(QString urlPath);
    QJsonObject readJsonFromFile(QString path);

    // Листы ввода
    QStringList rezPatterns;
    QStringList spesialMarks;
    QStringList userStringList;

    // Переменные
    QString currentPath;// Путь к серверу
    QString lastPath;
    User* curUser = Q_NULLPTR;
    QString curUserUUID;
    QVector<Doc>* docList = Q_NULLPTR;
    QVector<Doc>* docListBase = Q_NULLPTR;
    QVector<Doc>* docListForUpload = Q_NULLPTR;
    QString URL;
    QProgressDialog *pgd = Q_NULLPTR;
    bool Online=false;

public slots:

    void slotProgressBar(qint64,qint64);

private:

    Settings();
    ~Settings();
    Settings(const Settings&);                 // Prevent copy-construction
    Settings& operator=(const Settings&);
    static Settings* m_instanse;

    UserModel* m_userModel = Q_NULLPTR;
    QList<User*> *getUserList();
    void saveUsersList(QList<User*> *users);
    QJsonObject makeUsersJson(QList<User*> *users);
};

#endif // SETTINGS_H
