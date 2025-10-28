#include "docsig.h"
#include "ui_docsig.h"

#include <QDesktopServices>
#include <QMessageBox>

DocSig::DocSig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocSig)
{
    ui->setupUi(this);

    readMode();
    //this->QDialog::setStyleSheet("QDialog{border-image: url(:/new/prefix1/1.jpg)}");
}

bool DocSig::exit()
{
    // Функция возврата выхода из окна
    return closeFlag;
}


void DocSig::updateUserListBox()
{
    QVector<User>::iterator it;
    QStringList::iterator itCategory;
    QList<QTreeWidgetItem*> categoryItems;


    ui->userListWidget->clear();
    for(itCategory=set->userCategory.begin();itCategory!=set->userCategory.end();itCategory++)
    {
        if(itCategory->right(3)!="НИО")
        {
            QTreeWidgetItem* temp=new QTreeWidgetItem();
            temp->setText(0,*itCategory);

            for(it=userList->begin();it!=userList->end();it++)
            {
                if(it->retCategory()==temp->text(0))
                {
                    QTreeWidgetItem* tempUser=new QTreeWidgetItem();
                    tempUser->setText(0,it->retId());
                    temp->addChild(tempUser);
                }
            }

            categoryItems.append(temp);

        } else
        {
            QTreeWidgetItem* temp=new QTreeWidgetItem();
            temp->setText(0,*itCategory);
            for(it=userList->begin();it!=userList->end();it++)
            {
                if(it->retCategory()==temp->text(0))
                {
                    QTreeWidgetItem* tempUser=new QTreeWidgetItem();
                    tempUser->setText(0,it->retId());
                    temp->addChild(tempUser);
                }
            }
            categoryItems.last()->addChild(temp);
        }


    }

    ui->userListWidget->addTopLevelItems(categoryItems);

    //ui->userListWidget->addItems(lst);
}

void DocSig::updateDocListBox()
{
    ui->treeWidget->clear();
    if (docList->size()>0){
        ui->treeWidget->clear();
        QVector<Doc>::iterator it;
        QTreeWidgetItem *temp;
        it=docList->begin();
        for(;it!=docList->end();it++)
        {
            if((it->checkAddressee(curUser->retUuid()) && !it->cheakAddFlag(curUser->retUuid()))
                    || curUser->retId()=="Администратор"||curUser->retId()=="Разработчик"
                    ||(it->checkAddressee(curUser->retUuid()) &&flagShowAll)
                    ||(it->checkOwner(curUser->retUuid())&&flagShowAll))
            {

                it->setFlagReceive(curUser->retUuid());
                if (cherArc||!it->isArc())
                {
                    temp=new QTreeWidgetItem();
                    temp->setText(0,it->retName());
                    temp->setText(2,it->retDateUpdate());
                    temp->setTextAlignment(2,Qt::AlignCenter);
                    // Специальные метки

                    if (it->hasSpecialMark())
                    {

                        temp->setText(1,it->retSpecialMark());
                        temp->setForeground(0,QColor(Qt::red));
                        temp->setForeground(1,QColor(Qt::red));
                    }

                    //

                    if (it->checkOwner(curUser->retUuid()))
                    {

                        temp->setText(3,"Вы - отправитель документа");
                        temp->setForeground(0,QColor(Qt::darkCyan));
                        temp->setForeground(3,QColor(Qt::darkCyan));
                    }

                    // Уже обработан
                    if (it->cheakAddFlag(curUser->retUuid()))
                    {
                        temp->setText(3,"Документ уже обработан");
                        temp->setForeground(0,QColor(Qt::darkYellow));
                    }

                    // все обработали документ
                    if (it->retAddresseeFlag())
                    {
                        temp->setForeground(0,QColor(Qt::green));
                        temp->setText(3,"Документ готов к архивации");
                        temp->setForeground(3,QColor(Qt::green));
                        if (it->isArc())
                            temp->setForeground(0,QColor(Qt::blue));
                    }

                    // Архивный
                    if (it->isArc())
                    {
                        temp->setText(3,"Архивный");
                        temp->setForeground(0,QColor(Qt::magenta));
                        temp->setForeground(3,QColor(Qt::magenta));
                    }

                    temp->addChildren(it->retFlags(*userList));
                    ui->treeWidget->addTopLevelItem(temp);
                }
            }
        }
    }
}

void DocSig::updateSettings(Settings *set)
{
    this->set=set;
    this->serverPath=&set->serverPath;

    this->curUser=set->curUser;
    if (curUser->retId()=="Администратор"||curUser->retId()=="Разработчик")
    {
        addToAr=new QPushButton("Перенести документ в архив");
        check = new QCheckBox ("Показывать документы в архиве");
        ui->gridLayout->addWidget(addToAr,9,0,Qt::AlignLeft);
        ui->gridLayout->addWidget(check,9,1,Qt::AlignLeft);
        connect(addToAr,SIGNAL(clicked()),SLOT(on_addToAr_clicked()));
        connect(check,SIGNAL(clicked()),SLOT(on_checkBox_clicked()));
        ui->line->setHidden(true);
    }
    ui->userName->setText(this->curUser->retName());

    // обновление списка пользователей
    this->userList=set->userList;

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


    QScreen* sqr=QGuiApplication::screens().first();
    this->setGeometry((sqr->geometry().width()/2-set->dsx/2),(sqr->geometry().height()/2-set->dsy/2),set->dsx,set->dsy);
}


DocSig::~DocSig()
{
    delete ui;
}

void DocSig::on_addDocButton_clicked()
{
    // открытие окна добавления документа
    this->hide();
    AddDoc docwin;
    docwin.updateSettings(set);
    do {
        docwin.updateUserListBox();
        docwin.show();
        docwin.exec();
    } while(!docwin.exit());
    this->show();
    this->setFocus();
}

void DocSig::on_DocSig_rejected()
{
    // Обновление флага закрытия

    set->dsx=this->geometry().width();

    set->dsy=this->geometry().height();

    closeFlag=true;
}

void DocSig::on_treeWidget_doubleClicked(const QModelIndex &index)
{

    qDebug()<<index.row();
    QVector<Doc>::iterator it;
    bool flagFind=false;

    for (it=docList->begin();it<docList->end();it++)
    {
        if (ui->treeWidget->currentItem()->text(0)==it->retName())
        {
            flagFind=true;
            break;
        }
    }

    if (flagFind)
    {
        if (!it->isArc())
        {
            if(set->Online)
                it->makePDF(*userList);
            //qDebug()<<it->retPath();

            if( !QDesktopServices::openUrl (QUrl::fromLocalFile("C:/Eldoc/doc/"+it->retName(true))))
            {
                QMessageBox *msg= new QMessageBox(QMessageBox::Warning,"Создание PDF файла","При создании PDF - файла произошла ошибка,открыть исходный файл с сервера?",QMessageBox::Yes|QMessageBox::No);
                int n=msg->exec();
                if(n==QMessageBox::Yes)
                    QDesktopServices::openUrl (QUrl::fromLocalFile(it->retPath()));
            }
            it->setFlagRead(curUser->retUuid());
        }
        else
        {
            QMessageBox *msg= new QMessageBox(QMessageBox::Warning,"Открытие файла","Данный файл архивный,открыть с сервера?",QMessageBox::Yes|QMessageBox::No);
            int n=msg->exec();
            if(n==QMessageBox::Yes)
                QDesktopServices::openUrl (QUrl::fromLocalFile(it->retPath()));
        }
    }
}

void DocSig::on_showButton_clicked()
{
    flagShowAll=!flagShowAll;
    if (flagShowAll)
    {
        ui->rezComboBox->setEnabled(false);
        ui->addPatButton->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->sigFlagButton->setEnabled(false);
        ui->changeModButton->setEnabled(false);
        updateDocListBox();
        ui->showButton->setText("Текущие документы");

    }
    else
    {
        ui->rezComboBox->setEnabled(true);
        ui->addPatButton->setEnabled(true);
        ui->label_5->setEnabled(true);
        ui->sigFlagButton->setEnabled(true);
        ui->changeModButton->setEnabled(true);
        updateDocListBox();
        ui->showButton->setText("Все документы");
    }

}

void DocSig::on_treeWidget_itemSelectionChanged()
{

    QVector<Doc>::iterator it;
    bool flagFind=false;

    for (it=docList->begin();it<docList->end();it++)
    {
        if (ui->treeWidget->currentItem()->text(0)==it->retName())
        {
            flagFind=true;
            break;
        }
    }

    if (flagFind)
    {
        //qDebug()<<it->retLog();
        ui->userListWidget->clearSelection();

        readMode();
        ui->rezTree->clear();
        QVector<User>::iterator itUs;
        bool flagRez=false;
        for(itUs=userList->begin();itUs!=userList->end();itUs++)
        {

            if (it->hasRez(itUs->retUuid()))
            {
                QTreeWidgetItem* temp=new QTreeWidgetItem();
                QTreeWidgetItem* tempChild=new QTreeWidgetItem();
                temp->setText(0,itUs->retId());
                tempChild->setText(0,it->retRez(itUs->retUuid()));
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

void DocSig::readMode()

{
    if (!flagShowAll)
    {
        modeFlag=false;
        ui->changeModButton->setText("Режим редактирования");


        //    QTreeWidgetItem *disk=new QTreeWidgetItem;
        //    disk->setText(0,"it->retRez()");
        //    wid->itemAt(0,0)->insertChild(0,disk);

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
                if(it->retRez().isEmpty())
                {
                    ui->rezEdit->setPlainText("Резолюции для этого документа еще нет");
                }
                else
                {
                    //qDebug()<<it->retRez();
                    ui->rezEdit->setPlainText(it->retRez());
                }
            }

        } else ui->rezEdit->setPlainText("Режим чтения");
    }

}

void DocSig::writeMode()
{
    if (!flagShowAll)
    {
        QDateTime time=QDateTime::currentDateTime();
        ui->changeModButton->setText("Режим чтения");
        modeFlag=true;
        ui->sigFlagButton->setText("Добавить резолюцию");
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



void DocSig::on_addPatButton_clicked()
{
    ui->rezEdit->append(ui->rezComboBox->currentText());
    QStringList::iterator it;
    bool flag=false;
    for(it=set->rezPatterns.begin();it!=set->rezPatterns.end();it++)
    {
        if (it->data()==ui->rezComboBox->currentText())
        {
            flag=true;
        }
    }

    if (!flag)
    {
        set->rezPatterns.append(ui->rezComboBox->currentText());
        ui->rezComboBox->clear();
        ui->rezComboBox->addItems(set->rezPatterns);

    }

    ui->rezComboBox->setCurrentIndex(-1);
}

void DocSig::signedMode()
{
    if (!ui->treeWidget->selectedItems().isEmpty())
    {
        QVector<Doc>::iterator itDoc;
        bool flagFind=false;
        itDoc=docList->begin();
        for (;itDoc<docList->end();itDoc++)
        {
            if (ui->treeWidget->currentItem()->text(0)==itDoc->retName())
            {
                flagFind=true;
                break;
            }
        }
        if(flagFind)
        {
            QDateTime time=QDateTime::currentDateTime();
            QList<QTreeWidgetItem*>  ar=ui->userListWidget->selectedItems();
            QString log,tempRez;
            QList<QString>::iterator itSpecialMarks;
            tempRez=ui->rezEdit->toPlainText();
            tempRez.append("\n\n"+curUser->retNumber()+time.toString("\ndd/MM/yy hh:mm"));
            ui->rezEdit-> clear();
            log.clear();
            log.append(time.toString("dd/MM/yy hh:mm "));
            log.append(curUser->retId());
            log.append(" - добавил резолюцию");




            for(itSpecialMarks=set->spesialMarks.begin();itSpecialMarks!=set->spesialMarks.end();itSpecialMarks++)
            {

                if(tempRez.indexOf(*itSpecialMarks)!=-1)
                {
                    qDebug()<<*itSpecialMarks;
                    itDoc->addSpecialMark(*itSpecialMarks);
                }
            }

            if (!ar.isEmpty())
            {
                log.append("\nАдресована: ");
                ui->rezEdit->append("\nАдресована: ");


                QVector <User> ::iterator it;
                QStringList::iterator itCategory;


                QList<QTreeWidgetItem*> ar=ui->userListWidget->selectedItems();
                QVector <QString> tempString;
                for(int i=0;i<ar.count();i++)
                {
                    bool flagCategory=false;
                    for(itCategory=set->userCategory.begin();itCategory!=set->userCategory.end();itCategory++)
                    {
                        if (*itCategory==ar[i]->text(0))
                        {
                            flagCategory=true;
                            for(it=userList->begin();it!=userList->end();it++)
                            {
                                if(it->retCategory()==ar[i]->text(0))
                                {

                                    if (!itDoc->checkAddressee(it->retUuid()))
                                        itDoc->addAddressee(it->retUuid());

                                    if (log.right(12)!="Адресована: ")
                                        log.append(", ");

                                    log.append(it->retId());

                                    ui->rezEdit->append(it->retId());
                                }
                            }
                        }
                    }

                    if(!flagCategory)
                    {
                        for(it=userList->begin();it!=userList->end();it++)
                        {
                            if(it->retId()==ar[i]->text(0)&&tempString.indexOf(it->retUuid())==-1)
                            {
                                if (!itDoc->checkAddressee(it->retUuid()))
                                    itDoc->addAddressee(it->retUuid());
                                ui->rezEdit->append(it->retId());
                                if (log.right(12)!="Адресована: ")
                                    log.append(", ");
                                log.append(it->retId());


                            }
                        }

                    }
                    log.append(".");
                    //qDebug()<<ar[i]->text();
                }

                ui->rezEdit->append("________\n");

            }

            ui->rezEdit->append(tempRez);
            itDoc->addRez(ui->rezEdit->toPlainText(),curUser->retUuid());
            itDoc->addLog(log);

            if (!itDoc->cheakAddFlag(curUser->retUuid()))
            {
                itDoc->setFlag(curUser->retUuid());
            }

            on_treeWidget_itemSelectionChanged();
            updateDocListBox();
            readMode();
        }

    }

}


void DocSig::readedMode()
{
    if (!ui->treeWidget->selectedItems().isEmpty())
    {
        QList<QTreeWidgetItem*> ar=ui->userListWidget->selectedItems();
        QString log;
        QVector<Doc>::iterator it;

        log.clear();
        log.append(time.toString("dd/MM/yy hh:mm "));
        log.append(curUser->retId());
        log.append(" - добавил отметку о прочтении");


        bool flagFind=false;
        it=docList->begin();
        for (;it<docList->end();it++)
        {
            if (ui->treeWidget->currentItem()->text(0)==it->retName())
            {
                flagFind=true;
                break;
            }
        }
        if(flagFind)
        {
            if (!it->cheakAddFlag(curUser->retUuid()))
            {
                it->addLog(log);
                it->setFlag(curUser->retUuid());
            }
            on_treeWidget_itemSelectionChanged();
            updateDocListBox();
        }

    }
}

void DocSig::on_addToAr_clicked()
{
    if(!ui->treeWidget->selectedItems().isEmpty())
    {


        QVector<Doc>::iterator it;
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
        if(flagFind)
        {
            if(!it->isArc())
            {
                bool bOK=false;
                QString date=QInputDialog::getText(0,"Добавление в архив","Дата исходящего документа",QLineEdit::Normal,QDate::currentDate().toString("dd.MM.yyyy"),&bOK);
                if(bOK)
                {
                     bOK=false;
                    QString numb=QInputDialog::getText(0,"Добавление в архив","Номер исходящего документа",QLineEdit::Normal,"",&bOK);
                    if(bOK)
                    {

                        it->addToArc(*userList,date,numb);
                    }
                }
            }
            else
            {
                ui->userName->setText("Уже архивирован");
            }
        }
    }
    else ui->userName->setText("Выберете файл!");
    updateDocListBox();
}

void DocSig::on_checkBox_clicked()
{
    cherArc=!cherArc;
    updateDocListBox();
}

void DocSig::on_changeModButton_clicked()
{

    if(!modeFlag)
    {
        writeMode();
    }
    else
    {
        readMode();
    }
}

void DocSig::on_sigFlagButton_clicked()
{
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


void DocSig::on_updateButton_clicked()
{
    QVector<Doc> tempUserList;
    tempUserList=Doc::updateDocList(&set->Online,set->URL);
    if(set->Online)
    {
        ui->date->setText(time.toString("ddd, dd MMM yyyy")+"       ONLINE ");
        ui->date->setStyleSheet("color: rgb(0, 200, 150)");
    } else
    {
        ui->date->setText(time.toString("ddd, dd MMM yyyy")+"       OFFLINE ");
        ui->date->setStyleSheet("color: rgb(200, 0, 0)");
    }
    set->docList=&tempUserList;
    updateDocListBox();
}


void DocSig::on_delPattern_clicked()
{
    QStringList::iterator it;
    int i =0;
    for(it=set->rezPatterns.begin();it!=set->rezPatterns.end();it++,i++)
    {
        if (it->data()==ui->rezComboBox->currentText())
        {
            set->rezPatterns.removeAt(i);
            break;
        }
    }
    ui->rezComboBox->clear();
    ui->rezComboBox->addItems(set->rezPatterns);
    ui->rezComboBox->setCurrentIndex(-1);
}
