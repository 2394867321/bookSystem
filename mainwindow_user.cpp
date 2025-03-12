#include "mainwindow_user.h"
#include "ui_mainwindow_user.h"
#include "find.h"
#include <QtSql>
#include<QtWidgets>
MainWindow_User::MainWindow_User(QString uid,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_User),
    uid(uid),
    flag(all_record)
{
    ui->setupUi(this);
    block=new QButtonGroup(this);
    block->addButton(ui->radioButton_over_time,over_time_record);
    block->addButton(ui->radioButton_book_record,select_record);
    block->addButton(ui->radioButton_all_record,all_record);
    ui->radioButton_all_record->setChecked(1);
    createBookPanel();
    createRecordPanel();
    connect(ui->pushButton_borrow,SIGNAL(clicked()),this,SLOT(borrowInfo()));
    connect(ui->pushButton_find_book,SIGNAL(clicked()),this,SLOT(findBookInfo()));
    connect(ui->pushButton_return,SIGNAL(clicked()),this,SLOT(returnInfo()));
    connect(ui->pushButton_con,SIGNAL(clicked()),this,SLOT(conInfo()));
    connect(ui->radioButton_over_time,SIGNAL(toggled(bool)),this,SLOT(setOverUpdateStuView()));
    connect(ui->radioButton_all_record,SIGNAL(toggled(bool)),this,SLOT(setAllUpdateStuView()));
    connect(ui->radioButton_book_record,SIGNAL(toggled(bool)),this,SLOT(setBookUpdateStuView()));
    connect(ui->pushButton_show_all,SIGNAL(clicked()),this,SLOT(all_show()));
    ui->tableView_book->setCurrentIndex(tableModel_b->index(0, 0));

}

MainWindow_User::~MainWindow_User()
{
    delete ui;
}
void MainWindow_User::createBookPanel()
{
    tableModel_b=new QSqlQueryModel(this);
    tableModel_b->setQuery(QString("SELECT * FROM BOOK"));
    tableModel_b->setHeaderData(0,Qt::Horizontal,tr("ISBN"));
    tableModel_b->setHeaderData(1,Qt::Horizontal,tr("BOOK_NAME"));
    tableModel_b->setHeaderData(2,Qt::Horizontal,tr("BOOK_AUTHOR"));
    tableModel_b->setHeaderData(3,Qt::Horizontal,tr("BOOK_PRICE"));
    tableModel_b->setHeaderData(4,Qt::Horizontal,tr("BOOK_PUBLISH"));
    tableModel_b->setHeaderData(5,Qt::Horizontal,tr("BOOK_RECORD"));
    ui->tableView_book->setModel(tableModel_b);
    ui->tableView_book->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_book->setSelectionBehavior(QAbstractItemView::SelectRows);
   // ui->tableView_book->setColumnHidden(5, true);
    ui->tableView_book->resizeColumnsToContents();
    ui->tableView_book->horizontalHeader()->setStretchLastSection(true);
    connect(ui->tableView_book->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex &,
                                     const QModelIndex &)),
            this, SLOT(updateViewTable()));
}

void MainWindow_User::createRecordPanel()
{
    tableModel=new QSqlQueryModel(this);
    tableModel->setQuery(QString("SELECT BOOK.BOOK_NAME,BOOK_USER.USER_UID,BOOK_USER.USER_NAME,DATE_BORROW,DATE_TETURN_LIMIT,BOOK.ISBN FROM BOOK,BOOK_USER,BORROW WHERE BORROW.USER_UID=BOOK_USER.USER_UID and BORROW.ISBN=BOOK.ISBN AND BORROW.USER_UID='%1'").arg(uid));
    tableModel->setHeaderData(0,Qt::Horizontal,tr("Book_Name"));
    tableModel->setHeaderData(1,Qt::Horizontal,tr("User_Id"));
    tableModel->setHeaderData(2,Qt::Horizontal,tr("User_Name"));
    tableModel->setHeaderData(3,Qt::Horizontal,tr("Borrow_Date"));
    tableModel->setHeaderData(4,Qt::Horizontal,tr("Return_Date"));
    ui->tableView_record->setModel(tableModel);
    ui->tableView_record->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_record->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_record->setColumnHidden(5, true);
    ui->tableView_record->resizeColumnsToContents();
    ui->tableView_record->horizontalHeader()->setStretchLastSection(true);
    updateViewTable();
}
void MainWindow_User::borrowInfo()
{
    QSqlRecord record_b=tableModel_b->record(ui->tableView_book->currentIndex().row());
    QString str_b=record_b.value(0).toString();
    QString str_bo_y=QString::number(QDate::currentDate().year());
    QString str_bo_m=QString::number(QDate::currentDate().month());
    QString str_bo_d=QString::number(QDate::currentDate().day());
    QDate r_d=QDate::currentDate().addMonths(2);
    QString str_r_y=QString::number(r_d.year());
    QString str_r_m=QString::number(r_d.month());
    QString str_r_d=QString::number(r_d.day());
    qDebug()<<QString("INSERT INTO BORROW VALUES('%1','%2',to_date('%3-%4-%5','YYYY-MM-DD'),to_date('%6-%7-%8','YYYY-MM-DD'))").arg(str_b,uid,str_bo_y,str_bo_m,str_bo_d,str_r_y,str_r_m,str_r_d);
    QSqlQuery tmp(QString("INSERT INTO BORROW VALUES('%1','%2',to_date('%3-%4-%5','YYYY-MM-DD'),to_date('%6-%7-%8','YYYY-MM-DD'))").arg(str_b,uid,str_bo_y,str_bo_m,str_bo_d,str_r_y,str_r_m,str_bo_d));
    if(!tmp.isActive()){
        QMessageBox::warning(this,tr("DataBase Error"),tmp.lastError().text());
    }
    updateViewTable();
    //qDebug()<<QString("INSERT INTO BORROW VALUES('%1','%2',to_date('%3-%4-%5','YYYY-MM-DD'),to_date('%6-%7-%8','YYYY-MM-DD'))").arg(str_b,uid,str_bo_y,str_bo_m,str_bo_d,str_r_y,str_r_m,str_r_d);
}

void MainWindow_User::findBookInfo()
{
    Find l(0);
    if(l.exec()){
        qDebug()<<QString("%1 = %2").arg(l.getContext(),l.getText());
        tableModel_b->setQuery(QString("SELECT * FROM BOOK WHERE %1 = '%2'").arg(l.getContext(),l.getText()));
        tableModel_b->setHeaderData(0,Qt::Horizontal,tr("ISBN"));
        tableModel_b->setHeaderData(1,Qt::Horizontal,tr("BOOK_NAME"));
        tableModel_b->setHeaderData(2,Qt::Horizontal,tr("BOOK_AUTHOR"));
        tableModel_b->setHeaderData(3,Qt::Horizontal,tr("BOOK_PRICE"));
        tableModel_b->setHeaderData(4,Qt::Horizontal,tr("BOOK_PUBLISH"));
        tableModel_b->setHeaderData(5,Qt::Horizontal,tr("BOOK_RECORD"));
        ui->tableView_book->setModel(tableModel_b);
        ui->tableView_book->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView_book->setSelectionBehavior(QAbstractItemView::SelectRows);
       // ui->tableView_book->setColumnHidden(5, true);
        ui->tableView_book->resizeColumnsToContents();
        ui->tableView_book->horizontalHeader()->setStretchLastSection(true);
        ui->tableView_book->update();
    }
}

void MainWindow_User::returnInfo()
{
    QModelIndex index=ui->tableView_record->currentIndex();
    QSqlRecord record=tableModel->record(index.row());
    QString str_b=record.value(5).toString();
    QString str_u=record.value(1).toString();
    qDebug()<<str_b<<str_u;
    QSqlQuery query(QString("DELETE FROM BORROW WHERE USER_UID='%1' AND ISBN='%2'").arg(str_u,str_b));
    if(!query.isActive()){
        QMessageBox::warning(this,tr("DataBase Error"),query.lastError().text());
    }
    updateViewTable();
}

void MainWindow_User::conInfo()
{
    QModelIndex index=ui->tableView_record->currentIndex();
    QSqlRecord record=tableModel->record(index.row());
    QString str_b=record.value(5).toString();
    QString str_u=record.value(1).toString();
    QDate r_d=record.value(4).toDate().addMonths(2);
    qDebug()<<r_d;
    QString str_r_y=QString::number(r_d.year());
    QString str_r_m=QString::number(r_d.month());
    QString str_r_d=QString::number(r_d.day());
    qDebug()<<QString("UPDATE BORROW SET DATE_TETURN_LIMIT=to_date('%1-%2-%3','YYYY-MM-DD') WHERE USER_UID='%4' AND ISBN='%5'").arg(str_r_y,str_r_m,str_r_d,str_u,str_b);
    QSqlQuery query(QString("UPDATE BORROW SET DATE_TETURN_LIMIT=to_date('%1-%2-%3','YYYY-MM-DD') WHERE USER_UID='%4' AND ISBN='%5'").arg(str_r_y,str_r_m,str_r_d,str_u,str_b));
    if(!query.isActive()){
        QMessageBox::warning(this,tr("DataBase Error"),query.lastError().text());
    }
    updateViewTable();
}




void MainWindow_User::setOverUpdateStuView()
{
    flag=over_time_record;
    updateViewTable();
}

void MainWindow_User::setAllUpdateStuView()
{
    flag=all_record;
    updateViewTable();
}

void MainWindow_User::setBookUpdateStuView()
{
    flag=select_record;
    updateViewTable();
}

void MainWindow_User::updateViewTable()
{
    QModelIndex index = ui->tableView_book->currentIndex();
    if (index.isValid()&&flag==select_record) {
        QSqlRecord record = tableModel_b->record(index.row());
        QString id = record.value("ISBN").toString();
        tableModel->setQuery(QString(
                     "SELECT BOOK.BOOK_NAME,BOOK_USER.USER_UID,BOOK_USER.USER_NAME,DATE_BORROW,DATE_TETURN_LIMIT,BOOK.ISBN FROM BOOK,BOOK_USER,BORROW WHERE BORROW.USER_UID=BOOK_USER.USER_UID AND BORROW.ISBN=BOOK.ISBN AND BORROW.USER_UID='%1' AND BORROW.ISBN='%2'").arg(uid,id));
        qDebug()<<QString(
                      "SELECT BOOK.BOOK_NAME,BOOK_USER.USER_UID,BOOK_USER.USER_NAME,DATE_BORROW,DATE_TETURN_LIMIT,BOOK.ISBN FROM BOOK,BOOK_USER,BORROW WHERE BORROW.USER_UID=BOOK_USER.USER_UID AND BORROW.ISBN=BOOK.ISBN AND BORROW.USER_UID='%1' AND BORROW.ISBN='%2'").arg(uid,id);
        }
    else if(flag==all_record){
        tableModel->setQuery(QString("SELECT BOOK.BOOK_NAME,BOOK_USER.USER_UID,BOOK_USER.USER_NAME,DATE_BORROW,DATE_TETURN_LIMIT,BOOK.ISBN FROM BOOK,BOOK_USER,BORROW WHERE BORROW.USER_UID=BOOK_USER.USER_UID and BORROW.ISBN=BOOK.ISBN AND BORROW.USER_UID='%1'").arg(uid));
    }
    else if(flag==over_time_record)
    {
        tableModel->setQuery(QString("select BOOK_NAME,BORROW.USER_UID,USER_NAME,TO_NUMBER(TO_DATE(SYSDATE)-DATE_TETURN_LIMIT) from BOOK,BOOK_USER,BORROW where BOOK.ISBN=BORROW.ISBN AND BOOK_USER.USER_UID=BORROW.USER_UID AND TO_NUMBER(DATE_TETURN_LIMIT-TO_DATE(SYSDATE))<0 AND BORROW.USER_UID='%1'").arg(uid));
        tableModel->setHeaderData(0, Qt::Horizontal, tr("BOOK_NAME"));
        tableModel->setHeaderData(1, Qt::Horizontal, tr("USER_UID"));
        tableModel->setHeaderData(2, Qt::Horizontal, tr("USER_NAME"));
        tableModel->setHeaderData(3, Qt::Horizontal, tr("OVER_DAY"));
    }
    updateTable();
    ui->tableView_record->update();

}
void MainWindow_User::updateTable(){
    ui->tableView_record->setColumnHidden(5, true);

}

void MainWindow_User::all_show(){
    tableModel_b->setQuery(QString("SELECT * FROM BOOK"));
    tableModel_b->setHeaderData(0,Qt::Horizontal,tr("ISBN"));
    tableModel_b->setHeaderData(1,Qt::Horizontal,tr("BOOK_NAME"));
    tableModel_b->setHeaderData(2,Qt::Horizontal,tr("BOOK_AUTHOR"));
    tableModel_b->setHeaderData(3,Qt::Horizontal,tr("BOOK_PRICE"));
    tableModel_b->setHeaderData(4,Qt::Horizontal,tr("BOOK_PUBLISH"));
    tableModel_b->setHeaderData(5,Qt::Horizontal,tr("BOOK_RECORD"));
    ui->tableView_book->setModel(tableModel_b);
    ui->tableView_book->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_book->setSelectionBehavior(QAbstractItemView::SelectRows);
   // ui->tableView_book->setColumnHidden(5, true);
    ui->tableView_book->resizeColumnsToContents();
    ui->tableView_book->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_book->update();
}

