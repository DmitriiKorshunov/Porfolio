#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>
#include <QVector>
#include "settings.h"


namespace Ui {
class SettingWindow;
}

class SettingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();
    bool exit ();

private slots:
    void on_addNewUser_clicked();
    void on_changeUser_clicked();

private:
    Ui::SettingWindow *ui;
    bool closeFlag=false;
    Settings * set;
};

#endif // SETTINGWINDOW_H
