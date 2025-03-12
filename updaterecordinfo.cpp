#include "updaterecordinfo.h"
#include "ui_updaterecordinfo.h"
#include <QtSql>
#include<QtWidgets>
updateRecordInfo::updateRecordInfo(QString stu,QString book,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::updateRecordInfo)
{
    ui->setupUi(this);
    tableModel=new QSqlRelationalTableModel(this);
    tableModel->setTable("BORROW");
    qDebug()<<(QString("USER_UID='%1' AND ISBN='%2'").arg(stu,book));
    tableModel->setFilter(QString("USER_UID='%1' AND ISBN='%2'").arg(stu,book));
    //tableModel->setFilter(QString("ISBN='%1'").arg(book));
    tableModel->setHeaderData(2,Qt::Horizontal,tr("Borrow_Date"));
    tableModel->setHeaderData(3,Qt::Horizontal,tr("Return_Date"));
    if (!tableModel->select()) {
        QMessageBox::critical(this, "Unable to initialize Database",
                    "Error initializing database: " + tableModel->lastError().text());
    }
    QSqlRecord record=tableModel->record(0);
    qDebug()<<record.value("DATE_BORROW").toString();
    mapper=new QDataWidgetMapper(this);
    mapper->setModel(tableModel);
    mapper->addMapping(ui->dateEdit_borrow,2);
    mapper->addMapping(ui->dateEdit_repay,3);
    mapper->toFirst();
    connect(ui->pushButton_ok,SIGNAL(clicked()),this,SLOT(updateData()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(reject()));
}

updateRecordInfo::~updateRecordInfo()
{
    delete ui;
}
void updateRecordInfo::updateData(){
    mapper->submit();
    accept();
}
