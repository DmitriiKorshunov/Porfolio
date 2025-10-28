#include "makeuser.h"
#include "ui_makeuser.h"
#include <QMessageBox>

MakeUser::MakeUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MakeUser)
{
    ui->setupUi(this);
    this->set=Settings::instance();
    //this->QDialog::setStyleSheet("QDialog{border-image: url(:/new/prefix1/1.jpg)}");
    ui->category->clear();
    connect(ui->saveButton,&QPushButton::clicked,this,&MakeUser::saveUser);
    //    ui->category->addItems(set->userCategory);
}

MakeUser::~MakeUser()
{
    delete ui;
}

void MakeUser::on_nameEdit_returnPressed()
{
    ui->idEdit->setFocus();
}

void MakeUser::on_idEdit_returnPressed()
{
    ui->passEdit->setFocus();
}

void MakeUser::on_passEdit_returnPressed()
{
    ui->saveButton->setFocus();
}

void MakeUser::on_MakeUser_rejected()
{
    this->hide();
}

void MakeUser::saveUser()
{
    if (!ui->nameEdit->text().isEmpty() && !ui->passEdit->text().isEmpty())
    {
        QJsonObject json;
        json["name"]=ui->nameEdit->text();
        json["password"]=QString(QCryptographicHash::hash((ui->passEdit->text().toUtf8()),QCryptographicHash::Md5));
        json["end_resolution"]=ui->rezEnd->toPlainText();
        json["category"]=ui->category->currentText();
        json["post_position"]=ui->idEdit->text();
        json["sender"]=set->curUserUUID;
        QJsonObject reply=set->saveJson(json,"/api/v1/post/users");
        set->showMessage(reply);
#ifdef QT_DEBUG
        QJsonObject temp;
        QJsonArray jarr;
        jarr.append(json);
        temp["users"]=jarr;
        set->saveJsonToFile(temp,set->currentPath+"Users.json");
#endif
        set->updateUserList();
        this->hide();
    }
    else
    {
        QMessageBox::information(0,"Ошибка создания пользователя","Имя пользователя или пароль не заполнены!");
    }
}
