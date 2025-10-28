#include "editinguser.h"
#include "makeuser.h"
#include "settingwindow.h"
#include "ui_settingwindow.h"

#include <QFileDialog>

SettingWindow::SettingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);
    this->set=Settings::instance();
}

SettingWindow::~SettingWindow()
{
    delete ui;
}


void SettingWindow::on_addNewUser_clicked()
{
    MakeUser m;
    m.show();
    this->close();
    m.exec();
}


void SettingWindow::on_changeUser_clicked()
{
    editingUser m;;
    m.show();
    this->close();
    m.exec();
}






