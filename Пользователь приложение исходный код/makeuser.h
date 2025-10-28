#ifndef MAKEUSER_H
#define MAKEUSER_H

#include "user.h"

#include <QDialog>
#include <QVector>
#include <settings.h>

namespace Ui {
class MakeUser;
}

class MakeUser : public QDialog
{
    Q_OBJECT

public:
    explicit MakeUser(QWidget *parent = nullptr);
    ~MakeUser();

private slots:
    void on_nameEdit_returnPressed();

    void on_idEdit_returnPressed();

    void on_passEdit_returnPressed();

    void on_MakeUser_rejected();
private:
    void saveUser();
    Ui::MakeUser *ui;
    Settings* set;
};

#endif // MAKEUSER_H
