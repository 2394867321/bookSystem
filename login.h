#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "initdb.h"
namespace Ui {
class Login;
}
class QSqlDatabase;
class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    QSqlDatabase* getDB();
    QString getUID();
    bool getID();
private:
    Ui::Login *ui;
    QSqlDatabase *db;
    int identity_login;
    QString uid;
private slots:
    void on_lineEdit_user_textChanged();
    void createLink();
    void setId(bool Id);
};

#endif // LOGIN_H
