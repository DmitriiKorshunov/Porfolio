#include "docsigfull.h"
#include "settingwindow.h"
#include "ui_docsigfull.h"

DocSigFull::DocSigFull(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DocSigFull)
{
    ui->setupUi(this);
    this->set=Settings::instance();
    set->updateDocList();
    this->updateSettings();
    ui->treeWidget->setHeaderHidden(false);
    ui->updateButton->setVisible(false);
    ui->updateButton->setEnabled(false);
}

void DocSigFull::updateDocListBox()
{
    if(set->docList == Q_NULLPTR || set->docListForUpload == Q_NULLPTR || set->curUser ==Q_NULLPTR)
        return;
    qDebug()<<set->docList->count()<<set->docListForUpload->count();
    ui->treeWidget->clear();
    if (docList->size()>0){
        ui->treeWidget->clear();
        QTreeWidgetItem *temp;
        for(auto &it:*set->docList)
        {
            if(it.showDoc(set->curUser->getUuid(),flagShowAll))
            {

                it.setFlagReceive(set->curUser->getUuid());
                if (cherArc||!it.isArc())
                {
                    temp=new QTreeWidgetItem();
                    temp->setText(0,it.retName());
                    temp->setData(2,0,QDate::fromString(it.retDateUpdate(),"dd/MM/yyyy"));
                    temp->setData(4,0,it.retUuid());
                    //temp->setText(2,QDate::fromString(it->retDateUpdate(),"dd/MM/yy"));
                    // Специальные метки

                    if (it.hasSpecialMark())
                    {

                        temp->setText(1,it.retSpecialMark());
                        temp->setForeground(0,QColor(Qt::red));
                        temp->setForeground(1,QColor(Qt::red));
                    }

                    //

                    if (it.checkOwner(set->curUser->getUuid()))
                    {

                        temp->setText(3,"Вы - отправитель документа");
                        temp->setForeground(0,QColor(Qt::darkCyan));
                        temp->setForeground(3,QColor(Qt::darkCyan));
                    }

                    // Уже обработан
                    if (it.cheakAddFlag(set->curUser->getUuid()))
                    {
                        temp->setText(3,"Документ уже обработан");
                        temp->setForeground(0,QColor(Qt::darkYellow));
                    }

                    // все обработали документ
                    if (it.retAddresseeFlag())
                    {
                        temp->setForeground(0,QColor(Qt::green));
                        temp->setText(3,"Документ готов к архивации");
                        temp->setForeground(3,QColor(Qt::green));
                        if (it.isArc())
                            temp->setForeground(0,QColor(Qt::blue));
                    }

                    // Архивный
                    if (it.isArc())
                    {
                        temp->setText(3,"Архивный");
                        temp->setForeground(0,QColor(Qt::magenta));
                        temp->setForeground(3,QColor(Qt::magenta));
                    }

//                    temp->addChildren(it->retFlags(*userList));

                    temp->setTextAlignment(1,Qt::AlignCenter);
                    temp->setTextAlignment(2,Qt::AlignCenter);
                    temp->setTextAlignment(3,Qt::AlignCenter);
                    ui->treeWidget->addTopLevelItem(temp);
                }
            }
        }
    }
}



QString DocSigFull::getSelectedUsers()
{
    QStringList users;
    auto selModel = ui->userListWidget->selectionModel();
    auto userModel = set->getUserModel();

    for(auto index:selModel->selectedIndexes())
    {
        auto user = userModel->userByIndex(index);
        users.append(user->getName());
    }

    return users.join(',').append(".");
}

DocSigFull::~DocSigFull()
{
    set->saveDocList();
    delete ui;
}

void DocSigFull::on_addDocButton_clicked()
{
    // открытие окна добавления документа
    EditDoc docwin;
    docwin.exec();
    this->updateDocListBox();
}

void DocSigFull::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    qDebug()<<index.row();
    QVector<Doc>::iterator it;
    bool flagFind=false;
    if(docList == Q_NULLPTR)
        return;

    for (it=docList->begin();it<docList->end();it++)
    {
        if (ui->treeWidget->currentItem()->text(4)==it->retUuid())
        {
            flagFind=true;
            break;
        }
    }

    if (flagFind)
    {

        if(!QFile::exists(set->currentPath+"/tempDoc/"+it->retUuid()+".pdf"))
        {
            if(!set->getFile(it->retUuid()))
                QMessageBox::warning(0,"Открытие файла","При загрузке файла произошла ошибка, повторите снова!");
        }
        QDesktopServices::openUrl (QUrl::fromLocalFile(set->currentPath+"/tempDoc/"+it->retUuid()+".pdf"));
        if(set->curUser != Q_NULLPTR)
        it->setFlagRead(set->curUser->getUuid());
    }
}

void DocSigFull::on_showButton_clicked()
{
    flagShowReady=!flagShowReady;
    if(flagShowReady)
    {
        docList=set->docListForUpload;
        flagShowAll = true;
        updateDocListBox();
        ui->showButton->setText("Текущие документы");
        ui->sigFlagButton->setText("Показать все документы");
        ui->changeModButton->setText("Отредактировать документ");
    }
    else
    {
        docList=set->docList;
        flagShowAll = false;
        updateDocListBox();
        ui->showButton->setText("Рассмотренные документы");
        ui->sigFlagButton->setEnabled(true);
        ui->changeModButton->setEnabled(true);
        readMode();
    }
}

void DocSigFull::on_treeWidget_itemSelectionChanged()
{
    Doc *doc=selectedDoc();

    if (doc!=nullptr)
    {
        //qDebug()<<it->retLog();
        ui->userListWidget->clearSelection();
        readMode();
        ui->rezTree->clear();
        bool flagRez=false;
        auto users = set->getUserModel()->getUserList();
        for(auto user:*users)
        {
            if (doc->hasRez(user->getUser()->getUuid()))
            {
                QTreeWidgetItem* temp=new QTreeWidgetItem();
                QTreeWidgetItem* tempChild=new QTreeWidgetItem();
                temp->setText(0,user->getUser()->getPostPosition());
                tempChild->setText(0,doc->retRez(user->getUser()->getUuid()));
                //qDebug()<<doc->retRez(itUs->retUuid());
                temp->addChild(tempChild);
                ui->rezTree->addTopLevelItem(temp);
                flagRez=true;
            }
        }
        if(!flagRez)
        {
            QTreeWidgetItem* temp=new QTreeWidgetItem();
            temp->setText(0,"Резолюций для этого документа пока нет!");
            ui->rezTree->addTopLevelItem(temp);
        }
    }
}

void DocSigFull::on_addPatButton_clicked()
{
    ui->rezEdit->append(ui->rezComboBox->currentText());
    QStringList::iterator it;
    bool flag=false;
    flag= set->rezPatterns.contains(ui->rezComboBox->currentText());

    if (!flag)
    {
        set->rezPatterns.append(ui->rezComboBox->currentText());
        ui->rezComboBox->clear();
        ui->rezComboBox->addItems(set->rezPatterns);

    }

    ui->rezComboBox->setCurrentIndex(-1);
}


void DocSigFull::on_changeModButton_clicked()
{
    if (flagShowReady)
    {
        QVector<Doc>::iterator it, itBase;
        it=selectedDoc();
        if (it!=nullptr)
        {
            for (itBase=set->docListBase->begin();itBase<set->docListBase->end();itBase++)
            {
                if (itBase->retUuid()==it->retUuid())
                {
                    Doc doc;
                    doc=*itBase;
                    set->docList->append(doc);
                    set->docListForUpload->erase(it);
                    break;
                }
            }
        }
        updateDocListBox();
    }
    else{
        if(!modeFlag)
        {
            writeMode();
        }
        else
        {
            readMode();
        }
    }
}

void DocSigFull::on_sigFlagButton_clicked()
{
    if (flagShowReady)
    {
        docList=set->docList;
        updateDocListBox();
        ui->sigFlagButton->setEnabled(false);
        ui->changeModButton->setEnabled(false);
    }
    else {

        if(!modeFlag)
        {
            // режим чтения
            readedMode();

        }
        else
        {
            // режим редактирования
            signedMode();

        }
    }
}

void DocSigFull::on_updateButton_clicked()
{
    set->saveSettings();
    set->saveDocList();
    set->updateDocList();
    this->updateSettings();
}

void DocSigFull::on_delPattern_clicked()
{
    set->rezPatterns.removeOne(ui->rezComboBox->currentText());
    ui->rezComboBox->clear();
    ui->rezComboBox->addItems(set->rezPatterns);
    ui->rezComboBox->setCurrentIndex(-1);
}

void DocSigFull::readMode()
{
    if (!flagShowAll)
    {
        modeFlag=false;
        ui->changeModButton->setText("Режим редактирования");

        ui->rezEdit->hide();
        ui->rezTree->show();

        ui->rezComboBox->setEnabled(false);
        ui->addPatButton->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->delPattern->setEnabled(false);
        ui->sigFlagButton->setText("Отметка о прочтении");

        ui->rezComboBox->setHidden(true);
        ui->addPatButton->setHidden(true);
        ui->label_5->setHidden(true);
        ui->delPattern->setHidden(true);

        ui->rezEdit->setReadOnly(true);
        if (!ui->treeWidget->selectedItems().isEmpty())
        {
            QVector<Doc>::iterator it;
            if(docList != Q_NULLPTR)
              {
                it=docList->begin();
                bool flagFind=false;
                for (;it<docList->end();it++)
                {

                    if (ui->treeWidget->currentItem()->text(0)==it->retName())
                    {
                        flagFind=true;
                        break;
                    }
                }
                if (flagFind)
                {
                    if(it->retRezCount()==0)
                    {
                        ui->rezEdit->setPlainText("Резолюции для этого документа еще нет");
                    }
                }
            }

        } else ui->rezEdit->setPlainText("Режим чтения");
    }
}

void DocSigFull::writeMode()
{
    if (!flagShowAll)
    {
        QDateTime time=QDateTime::currentDateTime();
        ui->changeModButton->setText("Режим чтения");
        modeFlag=true;
        ui->sigFlagButton->setText("Добавить комментарий");
        ui->rezComboBox->clear();
        ui->rezComboBox->addItems(set->rezPatterns);

        ui->rezTree->hide();
        ui->rezEdit->show();


        ui->delPattern->setEnabled(true);
        ui->rezComboBox->setEnabled(true);
        ui->addPatButton->setEnabled(true);
        ui->label_5->setEnabled(true);

        ui->rezComboBox->setHidden(false);
        ui->addPatButton->setHidden(false);
        ui->label_5->setHidden(false);
        ui->delPattern->setHidden(false);

        ui->rezComboBox->setCurrentIndex(-1);

        ui->rezEdit->setReadOnly(false);
        ui->rezEdit->clear();
    }
}

void DocSigFull::signedMode()
{
    if(set->curUser == Q_NULLPTR)
        return;

    if (!ui->treeWidget->selectedItems().isEmpty())
    {
        Doc *doc=selectedDoc();

        if (doc!=nullptr)
        {
            QDateTime time=QDateTime::currentDateTime();
            QString log,tempRez;

            // Добавление резолюции
            tempRez.clear();
            tempRez.append(getSelectedUsers());
            tempRez.append("\n_____________\n");
            tempRez.append(ui->rezEdit->toPlainText());
            tempRez.append("\n\n"+set->curUser->getResolution()+time.toString("\ndd/MM/yy hh:mm"));
            doc->addRez(tempRez,set->curUser->getUuid());
            ui->rezEdit-> clear();

            auto selModel = ui->userListWidget->selectionModel();
            auto userModel = set->getUserModel();

            for(auto index:selModel->selectedIndexes())
            {
                auto user = userModel->userByIndex(index);
                if(user != Q_NULLPTR)
                    doc->addAddressee(user->getUuid());
            }


            // Добавление лога
            log.clear();
            log.append(time.toString("dd/MM/yy hh:mm "));
            log.append(set->curUser->getPostPosition());
            log.append(" - добавил резолюцию");
            doc->addLog(log);


            for(QList<QString>::iterator itSpecialMarks=set->spesialMarks.begin();itSpecialMarks!=set->spesialMarks.end();itSpecialMarks++)
            {
                if(tempRez.indexOf(*itSpecialMarks)!=-1)
                {
                    qDebug()<<*itSpecialMarks;
                    doc->addSpecialMark(*itSpecialMarks);
                }
            }



            if (!doc->cheakAddFlag(set->curUser->getUuid()))
            {
                doc->setFlag(set->curUser->getUuid());
            }

            doc->dontShow=true;
            set->docListForUpload->append(*doc);;
            set->docList->erase(doc);

            on_treeWidget_itemSelectionChanged();
            updateDocListBox();
            readMode();
        }

    }

}

void DocSigFull::readedMode()
{
    if(set->curUser == Q_NULLPTR)
        return;
    if (!ui->treeWidget->selectedItems().isEmpty())
    {
        Doc *doc=selectedDoc();
        if( doc ==Q_NULLPTR)
            return;

        qDebug()<<doc->retName()<<doc->retUuid();
        if (doc!=nullptr)
        {
            //            qDebug()<<doc->retUuid();
            if (!doc->cheakAddFlag(set->curUser->getUuid()))
            {
                QString log;
                log.clear();
                log.append(time.toString("dd/MM/yy hh:mm "));
                log.append(set->curUser->getPostPosition());
                log.append(" - добавил отметку о прочтении");

                doc->addLog(log);
                doc->setFlag(set->curUser->getUuid());
                qDebug()<<doc->retName()<<doc->retUuid();
                doc->dontShow=true;
                set->docListForUpload->append(*doc);;
                qDebug()<<set->docList->count()<<set->docListForUpload->count();
                set->docList->erase(doc);
            }
            updateDocListBox();
            on_treeWidget_itemSelectionChanged();
        }

    }
}

Doc* DocSigFull::selectedDoc()
{
    if(docList == Q_NULLPTR)
        return nullptr;
    QVector<Doc>::iterator it;
    if (!ui->treeWidget->selectedItems().isEmpty())
        for (it=docList->begin();it<docList->end();it++)
        {
            if (ui->treeWidget->currentItem()->text(4)==it->retUuid())
            {
                return it;
            }
        }
    return nullptr;
}

void DocSigFull::on_addToArc_clicked()
{
    if(!ui->treeWidget->selectedItems().isEmpty())
    {
        Doc *doc=selectedDoc();
        if (doc!=nullptr)
        {
            if(!doc->isArc())
            {
                bool bOK=false;
                QString date=QInputDialog::getText(0,"Добавление в архив","Дата исходящего документа",QLineEdit::Normal,QDate::currentDate().toString("dd.MM.yyyy"),&bOK);
                if(bOK)
                {
                    bOK=false;
                    QString numb=QInputDialog::getText(0,"Добавление в архив","Номер исходящего документа",QLineEdit::Normal,"",&bOK);
                    if(bOK)
                    {
                        doc->addToArc(date,numb);
                    }
                }
            }
            else
            {
                ui->userName->setText("Уже архивирован");
            }
        }
    }
    else ui->userName->setText("Выберите файл!");
    updateDocListBox();
}

void DocSigFull::on_editDocButton_clicked()
{

    if (selectedDoc()!=nullptr)
    {
        EditDoc docwin(selectedDoc());
        docwin.exec();
        this->updateDocListBox();
    }
}

void DocSigFull::on_showArcDocs_clicked()
{
    cherArc=!cherArc;
    if (cherArc)
        ui->showArcDocs->setText("Скрыть документы в архиве");
    else
        ui->showArcDocs->setText("Показать документы в архиве");
    updateDocListBox();
}

void DocSigFull::on_delDoc_clicked()
{

    bool bOK=false;
    QString numb=QInputDialog::getText(0,"Удаление документа","Введите числа от 1 до 6",QLineEdit::Normal,"",&bOK);
    if(bOK)
    {
        if (numb=="123456")
        {
            if(!ui->treeWidget->selectedItems().isEmpty())
            {
                Doc *doc=selectedDoc();
                if(doc == Q_NULLPTR)
                    return;
                qDebug()<<set->readJson("/api/v1/delete/doc/"+doc->retUuid());
                set->updateDocList();
                updateSettings();
                updateDocListBox();
            }
        }
    }
}

void DocSigFull::on_editUsersBTN_clicked()
{
    SettingWindow setwin;
    setwin.exec();
    this->updateDocListBox();
}

void DocSigFull::updateSettings()
{
    this->serverPath=&set->currentPath;
    ui->editUsersBTN->setVisible(false);
    ui->userListWidget->setModel(set->getUserModel());

    if(set->curUser != Q_NULLPTR)
    {
        if (!(set->curUser->getCategory()=="Администратор"||set->curUser->getCategory()=="Разработчик"))
        {
            ui->addToArc->setVisible(false);
            ui->showArcDocs->setVisible(false);
            ui->editDocButton->setVisible(false);
            ui->delDoc->setVisible(false);
            ui->treeWidget->setColumnHidden(4,true);

            ui->addToArc->setEnabled(false);
            ui->showArcDocs->setEnabled(false);
            ui->editDocButton->setEnabled(false);
            ui->delDoc->setEnabled(false);
        }

        if(set->curUser->getCategory()=="Разработчик")
        {
            ui->editUsersBTN->setVisible(true);
        }
        ui->userName->setText(this->set->curUser->getName());
    }
    // обновление списка пользователей
    set->updateUserList();

    // Обновление списка документоа
    this->docList=set->docList;
    if(set->Online)
    {
        ui->date->setText(time.toString("ddd, dd MMM yyyy")+"       ONLINE ");
        ui->date->setStyleSheet("color: rgb(0, 200, 150)");
    } else
    {
        ui->date->setText(time.toString("ddd, dd MMM yyyy")+"       OFFLINE ");
        ui->date->setStyleSheet("color: rgb(200, 0, 0)");
    }
    cherArc=false;

    updateDocListBox();
    readMode();
}
