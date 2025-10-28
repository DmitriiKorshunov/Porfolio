#ifndef EDITINGUSER_H
#define EDITINGUSER_H

#include "settings.h"

#include <QDialog>
#include <doc.h>
#include <user.h>

namespace Ui {
class editingUser;
}

class editingUser : public QDialog,private User
{
    Q_OBJECT

public:
    explicit editingUser(QWidget *parent = nullptr);
    ~editingUser();

private slots:
    void on_listWidget_itemSelectionChanged();

    void on_saveChanges_clicked();

    void on_deleteFromServer_clicked();

    void on_addSign_clicked();

    void on_pushButton_clicked();

private:
    Ui::editingUser *ui;
    Settings *set;
    QJsonObject user;
};

#endif // EDITINGUSER_H
