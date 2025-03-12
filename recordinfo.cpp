#include "recordinfo.h"
#include "ui_recordinfo.h"
#include "updaterecordinfo.h"
#include"addrecordinfo.h"
#include<QtSql>
#include<QtWidgets>
recordInfo::recordInfo(QString id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordInfo),
    id(id)
{
    ui->setupUi(this);
    tableModel=new QSqlQueryModel(this);
    //tableModel=new QSqlRelationalTableModel(this);
    //tableModel->setTable("BORROW");
    createCurrentPanel(id);
    //tableModel->setRelation(Stu_R_Id,QSqlRelation("BOOK_USER","USER_UID","USER_NAME"));
    //tableModel->setRelation(Book_R_Id,QSqlRelation("BOOK","ISBN","BOOK_NAME"));
    tableModel->setHeaderData(Book_R_Id,Qt::Horizontal,tr("Book_Name"));
    tableModel->setHeaderData(Stu_R_Id,Qt::Horizontal,tr("User_Id"));
    tableModel->setHeaderData(Stu_R_Name,Qt::Horizontal,tr("User_Name"));
    tableModel->setHeaderData(Borrow_Date,Qt::Horizontal,tr("Borrow_Date"));
    tableModel->setHeaderData(Return_Date,Qt::Horizontal,tr("Return_Date"));
    /*tableModel->select();
    if (!tableModel->select()) {
        QMessageBox::critical(this, "Unable to initialize Database",
                    "Error initializing database: " + tableModel->lastError().text());
    }*/
    mapper=new QDataWidgetMapper;
    mapper->setModel(tableModel);
    mapper->addMapping(ui->lineEdit_book_name,Book_R_Name);
    mapper->addMapping(ui->lineEdit_uid,Stu_R_Id);
    mapper->addMapping(ui->lineEdit_user_name,Stu_R_Name);
    mapper->addMapping(ui->dateEdit_borrow,Borrow_Date);
    mapper->addMapping(ui->dateEdit_return,Return_Date);
    mapper->toFirst();
    ui->lineEdit_uid->setEnabled(false);
    ui->lineEdit_book_name->setEnabled(false);
    ui->lineEdit_user_name->setEnabled(false);
    ui->dateEdit_borrow->setEnabled(false);
    ui->dateEdit_return->setEnabled(false);
    ui->tableView_record->setModel(tableModel);
    ui->tableView_record->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->tableView_record->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_record->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_record->resizeColumnsToContents();
    ui->tableView_record->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_record->setCurrentIndex(tableModel->index(0,0));
    ui->tableView_record->setColumnHidden(Book_R_Id,true);
    connect(ui->tableView_record->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            mapper,
            &QDataWidgetMapper::setCurrentModelIndex
            );
    connect(ui->pushButton_time,SIGNAL(clicked()),this,SLOT(updateData()));
    connect(ui->pushButton_new,SIGNAL(clicked()),this,SLOT(addData()));
    connect(ui->pushButton_delete,SIGNAL(clicked()),this,SLOT(deleteData()));
     ui->tableView_record->setCurrentIndex(tableModel->index(0, 0));
}

recordInfo::~recordInfo()
{
    delete ui;
}

void recordInfo::createCurrentPanel(QString id){
    if(id=="\0")
         tableModel->setQuery(QString("SELECT BOOK.BOOK_NAME,BOOK_USER.USER_UID,BOOK_USER.USER_NAME,DATE_BORROW,DATE_TETURN_LIMIT,BOOK.ISBN FROM BOOK,BOOK_USER,BORROW WHERE BORROW.USER_UID=BOOK_USER.USER_UID and BORROW.ISBN=BOOK.ISBN"));
    else
         tableModel->setQuery(QString("SELECT BOOK.BOOK_NAME,BOOK_USER.USER_UID,BOOK_USER.USER_NAME,DATE_BORROW,DATE_TETURN_LIMIT,BOOK.ISBN FROM BOOK,BOOK_USER,BORROW WHERE BORROW.USER_UID=BOOK_USER.USER_UID and BORROW.ISBN=BOOK.ISBN AND BORROW.USER_UID='%1'").arg(id));
}
void recordInfo::updateData(){
    QString stuId = "\0",bookId="\0";
    QModelIndex index = ui->tableView_record->currentIndex();
    if (index.isValid()) {
        QSqlRecord record;

         record = tableModel->record(index.row());
         stuId = record.value(Stu_R_Id).toString();
         bookId=record.value(Book_R_Id).toString();
     }

    updateRecordInfo form( stuId,bookId,this);
    form.exec();
    createCurrentPanel(id);
}
void recordInfo::deleteData(){
    QModelIndex index=ui->tableView_record->currentIndex();
    QSqlRecord record=tableModel->record(index.row());
    QString str_b=record.value(5).toString();
    QString str_u=record.value(1).toString();
    qDebug()<<str_b<<str_u;
    QSqlQuery query(QString("DELETE FROM BORROW WHERE USER_UID='%1' AND ISBN='%2'").arg(str_u,str_b));
    if(!query.isActive()){
        QMessageBox::warning(this,tr("DataBase Error"),query.lastError().text());
    }
    createCurrentPanel(id);
    ui->tableView_record->update();
}
void recordInfo::addData(){
    AddRecordInfo form(this);
    form.exec();
    createCurrentPanel(id);
}
