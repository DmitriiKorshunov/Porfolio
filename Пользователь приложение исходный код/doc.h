#ifndef DOC_H
#define DOC_H
#include "user.h"

#include <QListWidget>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QDateTime>
#include <QTreeWidget>
#include <QJsonArray>
#include "QtNetwork/QNetworkAccessManager"

class Doc
{
public:
    bool flagErrorPdf = false;
    bool dontShow = false;
    bool wasUpdate = false;

    Doc();
    Doc(const QJsonObject json);

    QString retPath();
    QString retName();
    QString retUuid();
    QString retName(bool flag);
    QString retAddressee();
    QString retAddressee(QVector <User> userList);
    QString retDateUpdate();
    QString retRez(int id);
    QString retRez(QString id);

    QList<QTreeWidgetItem*> retFlags(QVector <User> userList);
    bool hasRez(QString id);
    QString retRezAut(int id);
    int retRezCount();
    bool retAddresseeFlag();
    QString retSpecialMark();

    void addRez(QString rez,QString id);
    void addLog(QString log);
    void addAddressee(QString address);
    void addToArc(QString date,QString num);
    void addSpecialMark(QString mark);
    void updateTime();
    QVector <QString> delEmpty(QVector <QString> temp);

    void setFlag(QString id);
    void setFlagRead(QString id);
    void setFlagReceive(QString id);


    bool checkAddressee(QString);
    bool checkOwner(QString sender);
    bool cheakAddFlag(QString id);
    bool cheakReadFlag(QString id);
    bool cheakReceiveFlag(QString id);
    bool hasSpecialMark();

    static QVector <QString> toVec (QJsonArray Jarr);
    QJsonArray toArray(QVector<QString> vec);

    bool  isMod();

    QJsonObject makeJson();
    QJsonObject delSlash(QJsonObject);
    QDateTime retLastMod();
    bool isArc();
    bool showDoc(QString uuid,bool showAll);
private :
    QString uuid; // имя
    QString name; // имя
    QString path; // путь
    QVector <QString> log; // лог
    QVector <QString> addressee; // адресаты
    QJsonObject resolution;
    QJsonObject logging;
    QVector <QString> specialMarks; // специальные отметки (контроль, срочно)
    QVector <QString> addresseeFlag; // флаг адресатов
    QVector <QString> readFlag;  // флаг прочтения
    QVector <QString> receiveFlag; //флаг получения
    QString numExt;// входящий номер ВАМТО
    QString dateExt;// дата входящего документа ВАМТО
    QString sender;// отправитель
    QString numLoc;// номер внутренний
    QString dateLoc;// дата регистрации внутреннего документа
    QString owner;// инициатор документа
    QString dateUpdate;// дата добавления документа
    bool arFlag; // Флаг архивного документа
    bool flagMakeOnePDF=true;

    bool executionMark = false;
    QString executionMarkNumber;
    QString executionMarkDate;

};

#endif // DOC_H
