#ifndef DOCSIG_H
#define DOCSIG_H

#include <QDialog>
#include <QTreeWidget>
#include <QVector>
#include "user.h"
#include "doc.h"
#include "adddoc.h"
#include "settings.h"
#include <QDateTime>

namespace Ui {
class DocSig;
}

class DocSig : public QDialog
{
    Q_OBJECT

public:
    explicit DocSig(QWidget *parent = nullptr);
    bool exit ();

    void updateUserListBox();
    void updateDocListBox();
    void updateSettings(Settings* set);

    ~DocSig();

private slots:
    void on_addDocButton_clicked();

    void on_DocSig_rejected();

    void on_treeWidget_doubleClicked(const QModelIndex &index);

    void on_showButton_clicked();

    void on_treeWidget_itemSelectionChanged();

    void on_addPatButton_clicked();

    void on_addToAr_clicked();

    void on_checkBox_clicked();

    void on_changeModButton_clicked();

    void on_sigFlagButton_clicked();

    void on_updateButton_clicked();

    void on_delPattern_clicked();

private:
    void readMode();
    void writeMode();
    void signedMode();
    void readedMode();

    QVector<User> *userList;
    QVector<Doc> *docList;
    User *curUser;
    bool closeFlag = false;
    bool modeFlag = false;
    bool cherArc;
    bool flagShowAll=false;
    QDateTime time=QDateTime::currentDateTime();
    QString* serverPath;
    QPushButton *addToAr;
    QCheckBox *check;
    Settings *set;
    Ui::DocSig *ui;
};

#endif // DOCSIG_H
