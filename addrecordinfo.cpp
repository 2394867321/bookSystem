#include "addrecordinfo.h"
#include "ui_addrecordinfo.h"
#include<QtSql>
#include<QtWidgets>
AddRecordInfo::AddRecordInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRecordInfo)
{
    ui->setupUi(this);
    /*tableModel=new QSqlRelationalTableModel(this);
    tableModel->setTable("BORROW");
    tableModel->setRelation(Book_Name,QSqlRelation("BOOK","ISBN","BOOK_NAME"));
    tableModel->setRelation(Stu_Name,QSqlRelation("BOOK_USER","USER_UID","USER_NAME"));
    tableModel->select();
    QSqlTableModel *relationModel_b =tableModel->relationModel(Book_Name);
    QSqlTableModel *relationModel_u =tableModel->relationModel(Stu_Name);
    ui->comboBox_book_name->setModel(relationModel_b);
    ui->comboBox_book_name->setModelColumn(relationModel_b->fieldIndex("BOOK_NAME"));
    ui->comboBox_user_name->setModel(relationModel_u);
    ui->comboBox_user_name->setModelColumn(relationModel_u->fieldIndex("USER_NAME"));*/
    tableModel_b=new QSqlQueryModel(this);
    tableModel_b->setQuery("SELECT BOOK_NAME,ISBN FROM BOOK");
    tableModel_u=new QSqlQueryModel(this);
    tableModel_u->setQuery("SELECT USER_NAME,USER_UID FROM BOOK_USER");
    ui->comboBox_book_name->setModel(tableModel_b);
    ui->comboBox_book_name->setModelColumn(0);
    ui->comboBox_user_name->setModel(tableModel_u);
    ui->comboBox_user_name->setModelColumn(0);
    //QSqlRecord record=relationModel_b->record(0);
    //qDebug()<<record.value(relationModel_b->fieldIndex("BOOK_NAME")).toString();
    mapper_b = new QDataWidgetMapper(this);
    mapper_b->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper_b->setModel(tableModel_b);
    mapper_b->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper_b->addMapping(ui->comboBox_book_name, 0);
    mapper_u = new QDataWidgetMapper(this);
    mapper_u->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper_u->setModel(tableModel_u);
    mapper_u->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper_u->addMapping(ui->comboBox_user_name, 0);
    mapper_u->toFirst();
    mapper_b->toFirst();
    /*mapper= new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->comboBox_book_name, Book_Name);
    mapper->addMapping(ui->comboBox_user_name,Stu_Name);
    mapper->addMapping(ui->dateEdit_borrow,Borrow_time);
    mapper->addMapping(ui->dateEdit_repay,Return_time);
    int row = mapper->currentIndex();
    tableModel->insertRow(row);
    mapper->setCurrentIndex(row);*/
    QDateTime curDateTime=QDateTime::currentDateTime();
    ui->dateEdit_borrow->setDate(curDateTime.date());
    ui->dateEdit_repay->setDate(curDateTime.addMonths(2).date());
    connect(ui->pushButton_add,SIGNAL(clicked()),this,SLOT(addData()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(cancel()));
}

AddRecordInfo::~AddRecordInfo()
{
    delete ui;
}
void AddRecordInfo::addData(){

    QSqlRecord record_b=tableModel_b->record(ui->comboBox_book_name->currentIndex());
    QSqlRecord record_u=tableModel_u->record(ui->comboBox_user_name->currentIndex());
    QString str_b=record_b.value(1).toString();
    QString str_u=record_u.value(1).toString();
    QString str_bo_y=QString::number(ui->dateEdit_borrow->date().year());
    QString str_bo_m=QString::number(ui->dateEdit_borrow->date().month());
    QString str_bo_d=QString::number(ui->dateEdit_borrow->date().day());
    QString str_r_y=QString::number(ui->dateEdit_repay->date().year());
    QString str_r_m=QString::number(ui->dateEdit_repay->date().month());
    QString str_r_d=QString::number(ui->dateEdit_repay->date().day());
    //qDebug()<<ui->dateEdit_borrow->date().year();
    QSqlQuery tmp(QString("INSERT INTO BORROW VALUES('%1','%2',to_date('%3-%4-%5','YYYY-MM-DD'),to_date('%6-%7-%8','YYYY-MM-DD'))").arg(str_b,str_u,str_bo_y,str_bo_m,str_bo_d,str_r_y,str_r_m,str_bo_d));
    if(!tmp.isActive()){
        QMessageBox::warning(this,tr("DataBase Error"),tmp.lastError().text());
    }
    qDebug()<<QString("INSERT INTO BORROW VALUES('%1','%2',to_date('%3-%4-%5','YYYY-MM-DD'),to_date('%6-%7-%8','YYYY-MM-DD'))").arg(str_b,str_u,str_bo_y,str_bo_m,str_bo_d,str_r_y,str_r_m,str_r_d);
    //mapper->submit();
    accept();
}
void AddRecordInfo::cancel(){
    reject();
}

