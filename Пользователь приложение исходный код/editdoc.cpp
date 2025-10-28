#include "editdoc.h"
#include "ui_adddoc.h"

#include <QDateTime>
#include <QFile>

EditDoc::EditDoc(Doc *doc,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDoc)
{
    setAcceptDrops(true);
    ui->setupUi(this);
    ui->numInp->setFocus();
    this->doc = doc;
    this->set=Settings::instance();
    updateSettings();
    //this->QDialog::setStyleSheet("QDialog{border-image: url(:/new/prefix1/1.jpg)}");
}

void EditDoc::updateSettings()
{
    ui->userListWidget->setModel(set->getUserModel());
    if(doc == nullptr)
    {
        this->doc = new Doc();
        newDoc=true;
        ui->titleLabel->setText("Добавление нового файла");
    }
    else
    {
        ui->userListWidget->clearSelection();
        json=doc->makeJson();
        ui->titleLabel->setText(json["name"].toString());
        if (json.contains("output_date") && json["output_date"].isString())
            ui->dateInp->setText(json["output_date"].toString());
        if (json.contains("number_output") && json["number_output"].isString())
            ui->numInp->setText(json["number_output"].toString());
        if (json.contains("sender") && json["sender"].isString())
            ui->sender->setText(json["sender"].toString());
        if (json.contains("in_date") && json["in_date"].isString())
            ui->dateExt->setText(json["in_date"].toString());
        if (json.contains("number_input") && json["number_input"].isString())
            ui->numExt->setText(json["number_input"].toString());
        if (json.contains("user_address") && json["user_address"].isArray())
        {
            QJsonArray Jarr = (json["user_address"].toArray());
            auto selModel = ui->userListWidget->selectionModel();
            auto userModel = set->getUserModel();

            for(int iArr=0;iArr<Jarr.size();iArr++)
            {
                selModel->select(userModel->indexByUUID(Jarr[iArr].toString()),QItemSelectionModel::Select);
            }
        }
    }
}


void EditDoc::makeDocFromJsonFile(QString path)
{
    QJsonObject json;
    json = set->readJsonFromFile(path);
    if (json.contains("output_date") && json["output_date"].isString())
        ui->dateInp->setText(json["output_date"].toString());
    if (json.contains("number_output") && json["number_output"].isString())
        ui->numInp->setText(json["number_output"].toString());
    if (json.contains("sender") && json["sender"].isString())
        ui->sender->setText(json["sender"].toString());
    if (json.contains("in_date") && json["in_date"].isString())
        ui->dateExt->setText(json["in_date"].toString());
    if (json.contains("number_input") && json["number_input"].isString())
        ui->numExt->setText(json["number_input"].toString());
    if (json.contains("path") && json["path"].isString())
        ui->filePath->setText(json["path"].toString());
}

void EditDoc::dragEnterEvent(QDragEnterEvent *pe)
{
    pe->acceptProposedAction();
}

void EditDoc::dropEvent(QDropEvent *pe)
{
    QString tempPath=pe->mimeData()->urls().first().toString();
    if (tempPath.right(4)==".pdf")
    {
        ui->filePath->setText(pe->mimeData()->urls().first().toString());
    }
    else
    {
        QMessageBox::warning(0,"Добавление файла","Поддерживаются только PDF - файлы");
    }
}


EditDoc::~EditDoc()
{
    delete ui;
}


void EditDoc::on_backButton_clicked()
{
    this->close();
}

void EditDoc::on_acceptButton_clicked()
{
    QJsonArray selectedUsers;
    QStringList users;
    QString log;
    QString path;

    auto selModel = ui->userListWidget->selectionModel();
    auto userModel = set->getUserModel();

    for(auto index:selModel->selectedIndexes())
    {
        auto user = userModel->userByIndex(index);
        if(user != Q_NULLPTR)
        {
            selectedUsers.append(user->getUuid());
            users.append(user->getName());
        }
    }

    json["output_date"] = ui->dateInp->text();
    json["number_output"] = ui->numInp->text();
    json["sender"] = ui->sender->text();
    json["in_date"] = ui->dateExt->text();
    json["number_input"] = ui->numExt->text();
    json["user_address"] = selectedUsers;

    if (newDoc)
    {
        // добавление нового файла
        json["date_upload"]=QDate::currentDate().toString("dd/MM/yyyy");
        if (!users.isEmpty()&& !ui->filePath->text().isEmpty())
        {
            path=ui->filePath->text();
            QString newName;
            QStringList list;
            list=path.split("/",QString::KeepEmptyParts);
            json["name"]=list.last();
            json["owner"]=set->curUser->getUuid();
            if (path.startsWith("file:///"))
                path=path.right(path.count()-8);
            // тут должно быть перекидывание файла на сервер
            json["path"] = path;

            log.clear();
            log.append(QDateTime::currentDateTime().toString("dd/MM/yy hh:mm "));
            log.append("добавил пользователь - ");
            log.append(set->curUser->getPostPosition());
            log.append("\n Адресован: ");
            log.append(users.join(","));
            log.append(".");

        }
        else
        {
            if (users.isEmpty())
                QMessageBox::warning(0,"Добавление файла","Выберете адресатов для добавления файла!");
            if (ui->filePath->text().isEmpty())
                QMessageBox::warning(0,"Добавление файла","Выберете путь к файлу!");
            return;
        }

    }
    else
    {
        log.clear();
        log.append(QDateTime::currentDateTime().toString("dd/MM/yy hh:mm "));
        log.append("пользователь - ");
        log.append(set->curUser->getPostPosition());
        log.append(" внес изменения в документ.");
        if (ui->filePath->text()!="")
        {
            path=ui->filePath->text();
            QString newName;
            QStringList list;
            list=path.split("/",QString::KeepEmptyParts);
            json["name"]=list.last();
            if (path.startsWith("file:///"))
                path=path.right(path.count()-8);
            json["path"]=path;
            // тут должно быть перекидывание файла на сервер
        }
    }
    doc=new Doc(json);
    doc->addLog(log);
    QJsonObject jsonForUpload;

    jsonForUpload["docs"]=doc->makeJson();
    jsonForUpload["uuid_user"]=set->curUser->getUuid();
    set->saveJsonToFile(jsonForUpload,"c:/doc1.json");
    QJsonObject reply;
    reply = set->uploadFile(jsonForUpload);
    if(reply.contains("process"))
    {
        QMessageBox::information(0,"Добавление файла","Добавление файла прошло успешно: \n"+reply["process"].toString());
    }
    else
    {
        QMessageBox::warning(0,"Добавление файла","При добавлении файла произошла ошибка: \n"+reply["error"].toString());
    }
    this->close();
}

void EditDoc::on_openFile_clicked()
{
    QString path;
    path=(QFileDialog::getOpenFileName(0,"Выберете файл для добавления",set->lastPath,"*.pdf"));
    ui->filePath->setText(path);
    if(!path.isEmpty())
    {
        QStringList list;
        list=path.split("/",QString::KeepEmptyParts);
        list.removeLast();
        set->lastPath=list.join("/").append("/");
    }
}

void EditDoc::on_numInp_returnPressed()
{
    ui->dateInp->setFocus();
}

void EditDoc::on_dateInp_returnPressed()
{
    ui->sender->setFocus();
}

void EditDoc::on_sender_returnPressed()
{
    ui->numExt->setFocus();
}

void EditDoc::on_numExt_returnPressed()
{
    ui->dateExt->setFocus();
}
