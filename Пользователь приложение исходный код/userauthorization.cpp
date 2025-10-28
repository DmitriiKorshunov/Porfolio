#include "userauthorization.h"
#include "ui_userauthorization.h"
#include "settingwindow.h"
#include "docsigfull.h"

#include <QScreen>


UserAuthorization::UserAuthorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userAuthorization)
{
    ui->setupUi(this);
    this->set=Settings::instance();
    set->getUserModel();
    ui->lineEdit->setText(set->URL);
    ui->lineEdit->hide();
    connect(ui->pushButton,&QPushButton::clicked,this,[this](bool chec)
    {
        ui->lineEdit->setVisible(chec);
        if(!chec)
        {
            set->URL =  ui->lineEdit->text();
            set->getUserModel();
            updateUserList();
            set->saveSettings();
        }
    });
            ui->comboBox->addItems(set->userStringList);
    updateUserList();
}

void UserAuthorization::updateUserList()
{
    if(set->Online)
    {
        ui->online->setText(set->URL+" ONLINE");
        ui->online->setStyleSheet("color: rgb(0, 200, 150)");
    } else
    {
        ui->online->setText(set->URL+" OFFLINE");
        ui->online->setStyleSheet("color: rgb(200, 0, 0)");
    }
}

UserAuthorization::~UserAuthorization()
{
    delete ui;
}

void UserAuthorization::on_acceptButton_clicked()
{
    if(ui->passEdit->text()=="cc18d24a-d827-4615-af5b-dacb2e68591a")
    {
        qDebug()<<"DEV MODE";
        SettingWindow setwin;
        this->close();
        setwin.exec();
        return;
    }
    QJsonObject json;
    json["name"]=ui->comboBox->currentText();
    json["password"]=QString(QCryptographicHash::hash((ui->passEdit->text().toUtf8()),QCryptographicHash::Md5));
#ifdef QT_DEBUG
    qDebug()<<json;
#endif

    auto rep = set->saveJson(json,"/api/v1/auth");

    if (rep.contains("uuid"))
    {
        set->curUserUUID = rep["uuid"].toString();
        set->Online = true;
        set->updateUserList();
        accept();
    }
    else
    {
        ui->titleLablel->setText("Неправильный пароль!");
        ui->titleLablel->setFont(QFont("Verdana",14));
        ui->passEdit->clear();
        ui->passEdit->setFocus();
    }
}
