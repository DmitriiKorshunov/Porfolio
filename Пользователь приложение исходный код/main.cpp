
#include "userauthorization.h"
#include "settings.h"
#include "docsigfull.h"
#include "QMessageBox"
Settings* Settings::m_instanse;



#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //    qDebug()<<argv;
    //    Settings set;
    QString path = argv[0];
    auto set = Settings::instance();
    path.replace("\\","/");
    path = path.left(path.lastIndexOf("/"));
    QDir temp(path+"/tempDoc/");
    if(!temp.exists())
        temp.mkdir(path+"/tempDoc/");

    set->currentPath = path+"/";
    set->updateSettings();

    //            // Окно ввода пароля
    UserAuthorization UsAu;
    auto res = UsAu.exec();

    if(res == QDialog::Accepted)
    {
        DocSigFull window;
        window.show();
        a.exec();
    }
    QDir delTemp(path+"/tempDoc/");
    qDebug()<<"Deleting temp files form: "<<delTemp.path();
    foreach (QString path,delTemp.entryList(QDir::Files))
    {
        qDebug()<<path+" "<<QFile::remove(delTemp.path()+"/"+path);
    }

    return 1;


}
