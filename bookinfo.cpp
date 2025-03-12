#include "bookinfo.h"
#include "ui_bookinfo.h"
#include <QtSql>
#include <QtWidgets>
bookInfo::bookInfo(int id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookInfo)
{
    ui->setupUi(this);
    tableModel = new QSqlRelationalTableModel(this);
    tableModel->setTable("BOOK");
   //tableModel->setSort(Employee_Name, Qt::AscendingOrder);
    tableModel->select();
    //tableModel=new QSqlQueryModel;
    //tableModel->setQuery("select BOOK_NAME,BOOK_AUTHOR,BOOK_PUB,BOOK_PRICE,BOOK_RECORD from BOOK WHERE")
           // mapper->
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->lineEdit_ISBN,Book_Id);
    mapper->addMapping(ui->lineEdit_book_name, Book_Name);
    mapper->addMapping(ui->lineEdit_author, Book_Author);
    mapper->addMapping(ui->lineEdit_publish, Book_Pub);
    mapper->addMapping(ui->doubleSpinBox_price, Book_Price);
    mapper->addMapping(ui->textEdit_abstract,Book_Record);
    if (id != -1) {
         for (int row = 0; row < tableModel->rowCount(); ++row) {
         QSqlRecord record = tableModel->record(row);
         if (record.value(Book_Id).toInt() == id) {
                mapper->setCurrentIndex(row);
                break;
            }
         }
    }
    else{
            mapper->toFirst();
        }

        connect(ui->pushButton_first, SIGNAL(clicked()), mapper, SLOT(toFirst()));
        connect(ui->pushButton_prior, SIGNAL(clicked()),mapper, SLOT(toPrevious()));
        connect(ui->pushButton_next, SIGNAL(clicked()), mapper, SLOT(toNext()));
        connect(ui->pushButton_last, SIGNAL(clicked()), mapper, SLOT(toLast()));
       // connect(ui->pushButton_new, SIGNAL(clicked()), this, SLOT(addBook()));
        connect(ui->pushButton_delete, SIGNAL(clicked()),this, SLOT(deleteBook()));
        //connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));*/
    if (id == -1) {
            ui->pushButton_next->setFocus();
        } else {
            ui->lineEdit_book_name->setFocus();
        }
}

bookInfo::~bookInfo()
{
    delete ui;
}
void bookInfo::done(int result)
{
    if(!mapper->submit())
        qDebug()<<"submit Error";
    QDialog::done(result);
}
void bookInfo::addBook(){
    int row = mapper->currentIndex();
    mapper->submit();
    tableModel->insertRow(row);
    mapper->setCurrentIndex(row);
    ui->lineEdit_book_name->clear();
    ui->lineEdit_author->clear();
    ui->lineEdit_publish->clear();
    ui->textEdit_abstract->clear();
    ui->lineEdit_book_name->setFocus();
}
void bookInfo::deleteBook(){
    int row = mapper->currentIndex();
    tableModel->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}
