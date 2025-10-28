#include "editinguser.h"
#include "ui_editinguser.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QNetworkReply>
#include <QTimer>

editingUser::editingUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editingUser)
{
    ui->setupUi(this);
    this->set=Settings::instance();
//    ui->category->addItems(set->userCategory);
}

editingUser::~editingUser()
{
    delete ui;
}

void editingUser::on_listWidget_itemSelectionChanged()
{
//    QList<QListWidgetItem*> ar=ui->listWidget->selectedItems();
//    QVector<User>::iterator it;
//    QStringList::iterator itCategory;

//    it=userList->begin();
//    for (;it<userList->end();it++)
//    {
//        if (ar[0]->text()==it->retId())
//        {
//            user = it->makeJson();

//            QPixmap pix;
//            pix.loadFromData(it->retSign());
//            ui->singLabel->setPixmap( pix.scaled(150,150,Qt::KeepAspectRatio));

//            pix.loadFromData(it->retPhoto());
//            ui->photoLabel->setPixmap( pix.scaled(150,150,Qt::KeepAspectRatio));

//            ui->nameEdit->setText(it->retName());
//            ui->idEdit->setText(it->retId());
//            ui->rezEnd->setText(it->retResolution());
//            ui->category->setCurrentText(it->retCategory());
//            ui->number->setText(QString::number(it->retNumber()));
//            break;
//        }

//    }


}

void editingUser::on_saveChanges_clicked()
{
//    QList<QListWidgetItem*> ar=ui->listWidget->selectedItems();
//    if (!ar.isEmpty())
//    {
//        QVector<User>::iterator it;

//        it=userList->begin();
//        for (int i=0;it<userList->end();it++,i++)
//        {
//            if (ar[0]->text()==it->retId())
//            {
//                user["name"]=ui->nameEdit->text();
//                user["post_position"]=ui->idEdit->text();
//                user["password"]=ui->passEdit->text();
//                user["end_resolution"]= ui->rezEnd->toPlainText();
//                user["category"]=ui->category->currentText();
//                User userTemp(user);
//                userList->append(userTemp);
//                userList->remove(i);
//                break;
//            }

//        }
//        set->saveUserList();
//        this->hide();
//    }

}


void editingUser::on_deleteFromServer_clicked()
{
//    QList<QListWidgetItem*> ar=ui->listWidget->selectedItems();
//    if (!ar.isEmpty())
//    {
//        QVector<User>::iterator it;
//        it=userList->begin();
//        for (int i=0;it<userList->end();it++,i++)
//        {
//            if (ar[0]->text()==it->retId())
//            {
//                set->readJson("/api/v1/delete/user/"+it->retUuid());
//                userList->remove(i);
//                break;
//                this->close();
//            }

//        }
//        //updateForm();
//        this->close();
//    }
}



void editingUser::on_addSign_clicked()
{

    QString path;
    path=(QFileDialog::getOpenFileName(0,"Выберите файл для добавления",set->lastPath,"*.png"));
    if(!path.isEmpty())
    {
//        QVector<User>::iterator it;
//        it=userList->begin();
//        QList<QListWidgetItem*> ar=ui->listWidget->selectedItems();
//        if (!ar.isEmpty())
//        {

//            for (int i=0;it<userList->end();it++,i++)
//            {
//                if (ar[0]->text()==it->retId())
//                {
//                    QFile loadFile(path);
//                    if (!loadFile.open(QIODevice::ReadOnly)) {
//                        qDebug()<<"Cant open file: "<<path;
//                        return;
//                    }

//                    user["sign_path"]= QString(loadFile.readAll().toBase64());
//                    loadFile.close();
//                    break;
//                }

//            }
//        }
    }
}

void editingUser::on_pushButton_clicked()
{
    QString path;
    path=(QFileDialog::getOpenFileName(0,"Выберите файл для добавления",set->lastPath));
    if(!path.isEmpty())
    {
//        QVector<User>::iterator it;
//        it=userList->begin();
//        QList<QListWidgetItem*> ar=ui->listWidget->selectedItems();
//        if (!ar.isEmpty())
//        {

//            for (int i=0;it<userList->end();it++,i++)
//            {
//                if (ar[0]->text()==it->retId())
//                {
//                    QFile loadFile(path);
//                    if (!loadFile.open(QIODevice::ReadOnly)) {
//                        qDebug()<<"Cant open file: "<<path;
//                        return;
//                    }

//                    user["photo"]= QString(loadFile.readAll());
//                    loadFile.close();
//                    break;
//                }

//            }
//        }
    }
}
