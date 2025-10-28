#ifndef ADDDOC_H
#define ADDDOC_H

#include <QDialog>
#include <QtWidgets>
#include <user.h>
#include <QVector>
#include <doc.h>
#include <settings.h>

namespace Ui {
class AddDoc;
}

class EditDoc : public QDialog
{
    Q_OBJECT

public:
    explicit EditDoc(Doc *doc= nullptr,QWidget *parent = nullptr);
    void editDoc(Doc *doc);
    void makeDocFromJsonFile(QString path);
    ~EditDoc();
protected:
    virtual void dragEnterEvent(QDragEnterEvent *) override;
    virtual void dropEvent(QDropEvent *) override;

private slots:

    void on_backButton_clicked();

    void on_acceptButton_clicked();

    void on_openFile_clicked();

    void on_numInp_returnPressed();

    void on_dateInp_returnPressed();

    void on_sender_returnPressed();

    void on_numExt_returnPressed();

private:
    Doc *doc;
    Settings *set;
    void updateSettings();
    Ui::AddDoc *ui;
    QJsonObject json;
    bool newDoc=false;
};

#endif // ADDDOC_H
