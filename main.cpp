#include "mainwindow.h"
#include"mainwindow_user.h"
#include "login.h"
#include <QtSql>
#include <QApplication>
//#include<QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase* db;
    Login l;
    MainWindow* w;
    MainWindow_User* w_u;
    if(l.exec())
    { 
       db=l.getDB();
       if(l.getID())
       {
           w=new MainWindow(nullptr,db);
           w->show();
       }
       else
       {
           w_u=new MainWindow_User(l.getUID());
           w_u->show();
       }
       //delete db;
    }
    else
        exit(0);

    return a.exec();
}
