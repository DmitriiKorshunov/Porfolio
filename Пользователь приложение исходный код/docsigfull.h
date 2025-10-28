#ifndef DOCSIGFULL_H
#define DOCSIGFULL_H

#include <QWidget>
#include <QTreeWidget>
#include <QVector>
#include "user.h"
#include "doc.h"
#include "editdoc.h"
#include "settings.h"
#include <QDateTime>

namespace Ui {
class DocSigFull;
}

class DocSigFull : public QWidget
{
    Q_OBJECT

public:
    explicit DocSigFull(QWidget *parent = nullptr);

    void updateUserListBox();
    void updateDocListBox();
    QString getSelectedUsers();
    ~DocSigFull();

private slots:
    void on_addDocButton_clicked();

    void on_treeWidget_doubleClicked(const QModelIndex &index);

    void on_showButton_clicked();

    void on_treeWidget_itemSelectionChanged();

    void on_addPatButton_clicked();

    void on_changeModButton_clicked();

    void on_sigFlagButton_clicked();

    void on_updateButton_clicked();

    void on_delPattern_clicked();

    void on_addToArc_clicked();

    void on_editDocButton_clicked();

    void on_showArcDocs_clicked();

    void on_delDoc_clicked();

    void on_editUsersBTN_clicked();

private:
    void updateSettings();

    void readMode();
    void writeMode();
    void signedMode();
    void readedMode();
    Doc* selectedDoc();

    QVector<Doc> *docList = Q_NULLPTR;
    bool modeFlag = false;
    bool cherArc = false;
    bool flagShowAll=false;
    bool flagShowReady=false;
    QDateTime time=QDateTime::currentDateTime();
    QString* serverPath;
    QPushButton *addToAr = Q_NULLPTR;;
    QCheckBox *check = Q_NULLPTR;;
    Settings *set = Q_NULLPTR;;
    Ui::DocSigFull *ui;
};

#endif // DOCSIGFULL_H
