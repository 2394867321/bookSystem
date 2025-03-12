#include "login.h"
#include "ui_login.h"
#include<QtWidgets>
#include <QtSql>
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //ui->pushButton_login->setEnabled(false);
    ui->radioButton_admin->toggle();
    ui->label_tipShow->setText(tr("Admin Login"));
    connect(ui->pushButton_login, SIGNAL(clicked()), this, SLOT(createLink()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->radioButton_admin,SIGNAL(toggled(bool)),this, SLOT(setId(bool)));
}
void Login::on_lineEdit_user_textChanged(){
    ui->pushButton_login->setEnabled(
            ui->lineEdit_user->hasAcceptableInput());
}
void Login::createLink(){
    db=new QSqlDatabase;
    *db=QSqlDatabase::addDatabase("QOCI");
    db->setDatabaseName("orcl");
    db->setHostName("0.0.0.0");
    db->setPort(1521);
    uid=ui->lineEdit_user->text();
    if(identity_login)
    {
        db->setUserName(ui->lineEdit_user->text());
        db->setPassword(ui->lineEdit_passwd->text());
    }
    else

    {
        db->setUserName("0x11111");
        db->setPassword("123456");
    }
    if (!db->open())
    {
        QMessageBox::critical(0, "Unable to initialize Database","Error initializing database: " + db->lastError().text());
        ui->label_tipShow->setText(tr("账号名或密码不正确！！！"));
        ui->lineEdit_passwd->clear();
        delete db;
    }
    else
        accept();
        //return db->lastError();
}
QSqlDatabase* Login::getDB(){
    return db;
}
void Login::setId(bool Id){
    if(Id)
        ui->label_tipShow->setText(tr("Admin Login"));
    else
        ui->label_tipShow->setText(tr("User Login"));
    identity_login=Id;
}
Login::~Login()
{
    delete ui;
}
QString Login::getUID(){
    return uid;
}
bool Login::getID(){
    return identity_login;
}
