#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>
#include <QVector>
#include <QStringList>
#include <user.h>
#include <doc.h>
#include "settingwindow.h"
#include "docsig.h"
#include "settings.h"


#include "QDataStream"
#include "QtNetwork/QNetworkAccessManager"
#include "QNetworkReply"
#include "QJsonDocument"
#include "QJsonArray"




namespace Ui {
class MainMenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT

public:
    Settings set;
    explicit MainMenu(QWidget *parent = nullptr);
    bool exit ();
    void updateUserList();
    void readUsersFromServer();
    void saveDoc();


    ~MainMenu();

private slots:
    void on_setButton_clicked();

    void on_MainMenu_rejected();

    void on_passEdit_returnPressed();

    void on_acceptButton_clicked();

    void on_replyUpdateUsers();

    void on_updateButton_clicked();

    void on_makeOfflineDoc_clicked();

private:
    void updateUser(const QJsonObject &json);
    QVector<User> userList;
    QVector<Doc> docList;
    User * curUser;
    Ui::MainMenu *ui;
    QString serverPath;
    bool closeFlag = false;


    QNetworkAccessManager qnam;
    QNetworkReply *reply;



};

#endif // MAINMENU_H
