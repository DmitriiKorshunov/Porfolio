#include "doc.h"
#include "settings.h"

#include <QEventLoop>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QList>
#include <QNetworkReply>
#include <QPainter>
#include <QPdfWriter>
#include <QStringList>
#include <QTextDocumentWriter>
#include <QTimer>

Doc::Doc()
{

}

Doc::Doc(const QJsonObject json)
{
    QJsonObject js;
    js=json;
    if (json.contains("owner") && json["owner"].isString())
        this->owner = json["owner"].toString();
    if (json.contains("uuid_doc") && json["uuid_doc"].isString())
        this->uuid = json["uuid_doc"].toString();
    if (json.contains("name") && json["name"].isString())
        this->name = json["name"].toString();
    if (json.contains("path") && json["path"].isString())
        this->path = json["path"].toString();
    if (json.contains("log"))
        this->logging=delSlash(json["log"].toObject());
    if (json.contains("user_address") && json["user_address"].isArray())
        this->addressee  =toVec(json["user_address"].toArray());
    if (json.contains("resolution"))
        resolution=delSlash(json["resolution"].toObject());
    if (json.contains("special_notes") && json["special_notes"].isArray())
        this->specialMarks  =toVec(json["special_notes"].toArray());
    if (json.contains("address_flag") && json["address_flag"].isArray())
        this->addresseeFlag  =toVec(json["address_flag"].toArray());
    if (json.contains("read_flag") && json["read_flag"].isArray())
        this->readFlag  =toVec(json["read_flag"].toArray());
    if (json.contains("receive_flag") && json["receive_flag"].isArray())
        this->receiveFlag  =toVec(json["receive_flag"].toArray());
    if (json.contains("number_input") && json["number_input"].isString())
        this->numExt = json["number_input"].toString();
    if (json.contains("in_date") && json["in_date"].isString())
        this->dateExt = json["in_date"].toString();
    if (json.contains("sender") && json["sender"].isString())
        this->sender = json["sender"].toString();
    if (json.contains("number_output") && json["number_output"].isString())
        this->numLoc = json["number_output"].toString();
    if (json.contains("output_date") && json["output_date"].isString())
        this->dateLoc = json["output_date"].toString();
    if (json.contains("date_upload") && json["date_upload"].isString())
        this->dateUpdate=(json["date_upload"].toString());
    if (json.contains("archive_flag") && json["archive_flag"].isBool())
        this->arFlag=(json["archive_flag"].toBool());
    else {
        this->arFlag = false;
    }
    //qDebug()<<uuid<<name<<json["archive_flag"].toBool()<<this->arFlag;
}

QString Doc::retPath()
{
    //Возвращает  полный путь к файлу
    return path;
}

QString Doc::retName()
{
    //Возвращает имя файла
    return name;
}

QString Doc::retUuid()
{
    return uuid;
}

QString Doc::retName(bool flag)
{
    if (flag)
    {
        QStringList list;
        list=path.split("/",QString::KeepEmptyParts);
        return list.last();
    }
    else
        return name;
}

QString Doc::retAddressee()
{
    QVector<QString>::iterator it;
    QString Addresses;
    Addresses.clear();
    for(it=addressee.begin();it!=addressee.end();){
        //qDebug()<<(*it);
        Addresses.append(*it);
        if(++it==addressee.end())
            Addresses.append(".");
        else Addresses.append(", ");
    }
    return Addresses;

}

QString Doc::retAddressee(QVector<User> userList)
{
    QVector<QString>::iterator it;
    QVector<User>::iterator itUser;
    QString Addresses;
    Addresses.clear();
    for(it=addressee.begin();it!=addressee.end();)
    {
        //qDebug()<<(*it);
        for (itUser=userList.begin();itUser!=userList.end();itUser++)
        {
            if(itUser->getUuid()==*it)
            {
                Addresses.append(itUser->getPostPosition());
                break;
            }
        }

        if(++it==addressee.end())
            Addresses.append(".");
        else Addresses.append(", ");
    }
    return Addresses;
}

QString Doc::retDateUpdate()
{
    return dateUpdate;
}

QString Doc::retRez(int id)
{
    return resolution[resolution.keys()[id]].toString();
}

QString Doc::retRez(QString id)
{
    if(resolution.contains(id))
    {
        return  resolution[id].toString();
    }
    else
    {
        return "";
    }

}

QList<QTreeWidgetItem *> Doc::retFlags(QVector<User> userList)
{
    QList<QTreeWidgetItem *> items;

    QVector<QString>::iterator it;
    QVector<User>::iterator itUser;
    for(it=addressee.begin();it!=addressee.end();it++)
    {
        bool flagFind=false;
        for (itUser=userList.begin();itUser!=userList.end()&&!flagFind;itUser++)
        {
            if(itUser->getUuid()==*it)
            {
                QTreeWidgetItem* addresseItem=new QTreeWidgetItem();
                addresseItem->setText(0,itUser->getPostPosition());
                // Ознакомление
                QColor gr(0, 200, 150);
                QColor rd(255,0,100);
                if(cheakAddFlag(itUser->getUuid()))
                {
                    addresseItem->setText(1,"Есть отметка об ознакомлении");
                    addresseItem->setForeground(1,gr);
                }
                else
                {
                    addresseItem->setText(1,"Нет отметки об ознакомлении");
                    addresseItem->setForeground(1,rd);
                }

                // Прочтение
                if(cheakReadFlag(itUser->getUuid()))
                {
                    addresseItem->setText(2,"Прочитан");
                    addresseItem->setForeground(2,gr);
                }
                else
                {
                    addresseItem->setText(2,"Не прочитан");
                    addresseItem->setForeground(2,rd);
                }

                // Получение
                if(cheakReceiveFlag(itUser->getUuid()))
                {
                    addresseItem->setText(3,"Получен");
                    addresseItem->setForeground(3,gr);
                }
                else
                {
                    addresseItem->setText(3,"Не получен");
                    addresseItem->setForeground(3,rd);
                }
                addresseItem->setTextAlignment(0,Qt::AlignCenter);
                addresseItem->setTextAlignment(1,Qt::AlignCenter);
                addresseItem->setTextAlignment(2,Qt::AlignCenter);
                addresseItem->setTextAlignment(3,Qt::AlignCenter);



                items.append(addresseItem);
                flagFind=true;
            }
        }

    }

    return items;
}

bool Doc::hasRez(QString id)
{
    return resolution.contains(id);
}

QString Doc::retRezAut(int id)
{
    return resolution.keys()[id];
    //return rezolutionId[id];
}

int Doc::retRezCount()
{
    return resolution.count();
}

bool Doc::retAddresseeFlag()
{
    QVector<QString>::iterator it1;
    QVector<QString>::iterator it2;
    QVector<bool> flagFind(addressee.size(),false);
    int i=0;
    for(it1=addressee.begin();it1!=addressee.end();it1++,i++){
        for(it2=addresseeFlag.begin();it2!=addresseeFlag.end();it2++)
        {
            if(*it1==*it2)
            {
                flagFind[i]=true;
                break;
            }

        }
    }
    for (i=0;i<flagFind.count();i++)
    {
        if(!flagFind[i])
            return false;
    }
    return true;
}

QString Doc::retSpecialMark()
{
    QVector <QString>::iterator itSpecialMarks;
    QString temp;
    for(itSpecialMarks=specialMarks.begin();itSpecialMarks!=specialMarks.end();itSpecialMarks++)
    {
        if (!temp.isEmpty())
            temp.append(", ");
        temp.append(*itSpecialMarks);
    }
    temp.append(".");
    return temp;
}

bool Doc::cheakAddFlag(QString id)
{
    QVector<QString>::iterator it;
    for(it=addresseeFlag.begin();it!=addresseeFlag.end();it++){
        if (it->data()==id)
        {
            return true;
        }
    }
    return false;
}

bool Doc::cheakReadFlag(QString id)
{
    QVector<QString>::iterator it;
    for(it=readFlag.begin();it!=readFlag.end();it++)
    {
        if (it->data()==id)
        {
            return true;
        }
    }
    return false;
}

bool Doc::cheakReceiveFlag(QString id)
{
    QVector<QString>::iterator it;
    for(it=receiveFlag.begin();it!=receiveFlag.end();it++){
        if (it->data()==id)
        {
            return true;
        }
    }
    return false;
}

bool Doc::hasSpecialMark()
{
    QVector<QString>::iterator it;
    int i=0;
    if (specialMarks.count()==1)
    {
        if (specialMarks.first()=="")
        {
            specialMarks.remove(0);
            return false;
        }
        else
            return true;
    }
    else
    {

        for(it=specialMarks.begin();it!=specialMarks.end();it++,i++)
        {
            if(*it=="")
                specialMarks.remove(i);
        }
        if (specialMarks.isEmpty())
            return false;
        else
            return true;
    }
}

void Doc::addRez(QString rez,QString id)
{
    resolution[id] = rez;
    wasUpdate = true;
}

void Doc::addLog(QString log)
{
    // Добавляет лог файл
    logging[QString::number(QDateTime::currentMSecsSinceEpoch())]=log.replace("\\n"," ");
    wasUpdate = true;
}

bool Doc::checkAddressee(QString currentUser)
{
    QVector<QString>::iterator it;
    for(it=addressee.begin();it!=addressee.end();it++){
        if( it==currentUser)
            return true;
    }
    return false;
}

bool Doc::checkOwner(QString sender)
{
    if(this->owner==sender)
    {
        return true;

    }
    else
    {
        return false;
    }
}


void Doc::addAddressee(QString address)
{
    //Добавляет список адресатов
    if (addressee.count()==1 && addressee[0]=="")
        addressee.clear();
    addressee.append(address);
    wasUpdate = true;
}

QJsonArray Doc::toArray(QVector<QString> vec)
{
    QJsonArray arr;
    QVector<QString>::iterator it;
    for(it=vec.begin();it!=vec.end();it++)
    {
        arr.append(*it);
    }
    return arr;
}

QVector<QString> Doc::toVec(QJsonArray Jarr)
{
    QVector<QString> vs;
    for(int iArr=0;iArr<Jarr.size();iArr++)
    {
        vs.append(Jarr[iArr].toString());
    }
    return vs;
}


bool Doc::isMod()
{
    return wasUpdate;
}

QJsonObject Doc::makeJson()
{
    QJsonObject json;

    json["owner"]=this->owner;
    json["uuid_doc"]=this->uuid;
    json["name"]=this->name;
    json["path"]=this->path;
    json["log"]=this->logging;
    json["user_address"]=toArray(this->addressee);
    json["resolution"]=resolution;
    json["special_notes"]=toArray(delEmpty(this->specialMarks));
    json["address_flag"]=toArray(delEmpty(this->addresseeFlag));
    json["read_flag"]=toArray(delEmpty(this->readFlag));
    json["receive_flag"]=toArray(delEmpty(this->receiveFlag));
    json["number_input"]=this->numExt;
    json["in_date"]=this->dateExt;
    json["sender"]=this->sender;
    json["number_output"]=this->numLoc;
    json["output_date"]=this->dateLoc;
    json["date_upload"]=this->dateUpdate;
    json["archive_flag"]=this->arFlag;
    return json;
}

QJsonObject Doc::delSlash(QJsonObject items)
{
    QJsonObject json;
    foreach(QString key,items.keys())
    {
        QString temp;
        temp=items[key].toString();
        temp.replace("\\n","\n");
        json[key]=temp;
    }
    return json;
}

void Doc::setFlag(QString id)
{
    // Устанавливает флаг прочтения документа
    QVector<QString>::iterator it;
    bool flagFind=false;
    for (it=addresseeFlag.begin();it<addresseeFlag.end(); it++)
    {
        if(it->data()==id)
        {
            flagFind=true;
        }

    }
    if (!flagFind)
    {
        addresseeFlag.append(id);
        wasUpdate = true;
    }
}

void Doc::setFlagRead(QString id)
{
    // Устанавливает флаг прочтения документа
    QVector<QString>::iterator it;
    bool flagFind=false;
    for (it=readFlag.begin();it<readFlag.end(); it++)
    {
        if(it->data()==id)
        {
            flagFind=true;
        }

    }
    if (!flagFind)
    {
        readFlag.append(id);
        wasUpdate = true;
    }

}

void Doc::setFlagReceive(QString id)
{
    // Устанавливает флаг прочтения документа

    QVector<QString>::iterator it;
    bool flagFind=false;
    for (it=receiveFlag.begin();it<receiveFlag.end(); it++)
    {
        if(it->data()==id)
        {
            flagFind=true;
        }

    }
    if (!flagFind)
    {
        receiveFlag.append(id);
        wasUpdate = true;
    }

}



void Doc::addToArc(QString date,QString num)
{
    arFlag=true;
    executionMark = true;
    executionMarkDate = date;
    executionMarkNumber = num;
    wasUpdate = true;
}

void Doc::addSpecialMark(QString mark)
{
    this->specialMarks.append(mark);
}

QVector<QString> Doc::delEmpty(QVector<QString> temp)
{
    if(temp.count()>1 && temp[0]=="")
        temp.pop_front();
    return temp;
}

bool Doc::isArc()
{
    return arFlag;
}

bool Doc::showDoc(QString uuid, bool showAll)
{
    if (dontShow && !showAll)
        return false;
    if(uuid=="ff846147-2705-4790-9294-aa41a0ad2e6f"||uuid=="0f98eefe-edda-4d0c-98c4-d9a486222cc7")
        return true;
    if(this->checkAddressee(uuid) && !this->cheakAddFlag(uuid))
        return true;
    if (this->checkAddressee(uuid) && showAll)
        return true;
    if(this->checkOwner(uuid)&& showAll)
        return true;
    return false;
}
