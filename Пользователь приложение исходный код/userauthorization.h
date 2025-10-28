#ifndef USERAUTHORIZATION_H
#define USERAUTHORIZATION_H

#include "settings.h"
#include <QDialog>

namespace Ui {
class userAuthorization;
}

class UserAuthorization : public QDialog
{
    Q_OBJECT

public:
    explicit UserAuthorization(QWidget *parent = nullptr);
    void updateUserList();

    ~UserAuthorization();

private slots:
    void on_acceptButton_clicked();
private:
    Ui::userAuthorization *ui;
    Settings *set;
};

#endif // USERAUTHORIZATION_H
