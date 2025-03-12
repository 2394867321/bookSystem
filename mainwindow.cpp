#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bookinfo.h"
#include "stuinfo.h"
#include"recordinfo.h"
#include"find.h"
#include <QtSql>
#include<QtWidgets>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent, QSqlDatabase *db)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(db)
    , queryModel(nullptr)
    , flag(all_stu)
    , block(new QButtonGroup(this))
{
    ui->setupUi(this);
    initializeUI();
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete db;
    delete ui;
}

void MainWindow::initializeUI()
{
    block->addButton(ui->radioButton_over_time, over_time);
    block->addButton(ui->radioButton_book_stu, select_stu);
    block->addButton(ui->radioButton_all_stu, all_stu);
    ui->radioButton_all_stu->setChecked(true);
    
    createBookPanel();
    createStuPanel();
}

void MainWindow::setupConnections()
{
    connect(ui->pushButton_book_register, &QPushButton::clicked, this, &MainWindow::registerBookInfo);
    connect(ui->pushButton_book_edit, &QPushButton::clicked, this, &MainWindow::editBookInfo);
    connect(ui->pushButton_find_book,SIGNAL(clicked()),this,SLOT(findBookInfo()));
    connect(ui->pushButton_stu_edit,SIGNAL(clicked()),this,SLOT(editStuInfo()));
    connect(ui->pushButton_stu_register,SIGNAL(clicked()),this,SLOT(registerStuInfo()));
    connect(ui->pushButton_stu_find,SIGNAL(clicked()),this,SLOT(findStuInfo()));
    connect(ui->pushButton_find_record,SIGNAL(clicked()),this,SLOT(findRecord()));
    connect(ui->radioButton_over_time,SIGNAL(toggled(bool)),this,SLOT(setOverUpdateStuView()));
    connect(ui->radioButton_all_stu,SIGNAL(toggled(bool)),this,SLOT(setAllUpdateStuView()));
    connect(ui->radioButton_book_stu,SIGNAL(toggled(bool)),this,SLOT(setBookUpdateStuView()));
    connect(ui->pushButton_all_record,SIGNAL(clicked()),this,SLOT(allRecord()));
    connect(ui->pushButton_show_all,SIGNAL(clicked()),this,SLOT(all_show()));
    ui->tableView_book->setCurrentIndex(bookModel->index(0, 0));
}

void MainWindow::createBookPanel()
{
    /*QSqlQuery query(*db);
    query.exec("SELECT * FROM BOOK");
    if(!query.isActive()){
        QMessageBox::warning(this,tr("Database Error"),query.lastError().text());
    }
    while(query.next()){
        QString title=query.value(0).toString();
        qDebug()<<title;
    }*/
    bookModel = new QSqlRelationalTableModel(this);
    bookModel->setTable("BOOK");
    /*bookModel->select();
    for(int i=0;i<bookModel->rowCount();i++){
        QSqlRecord record=bookModel->record(i);
        QString t=record.value("BOOK_NAME").toString();
        qDebug()<<t;
    }*/
    bookModel->setHeaderData(Book_Id, Qt::Horizontal, tr("ISBN"));
    bookModel->setHeaderData(Book_Name, Qt::Horizontal, ("BOOK_NAME"));
    bookModel->setHeaderData(Book_Author, Qt::Horizontal, tr("AUTHOUR"));
    bookModel->setHeaderData(Book_Pub, Qt::Horizontal, tr("PUBLISH"));
    bookModel->setHeaderData(Book_Price, Qt::Horizontal, tr("PRICE"));
    bookModel->setHeaderData(Book_Record, Qt::Horizontal, tr("RECORD"));
    bookModel->select();
    ui->tableView_book->setModel(bookModel);
    ui->tableView_book->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->tableView_book->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_book->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->tableView_book->setColumnHidden(Book_Id, true);
    ui->tableView_book->resizeColumnsToContents();
    ui->tableView_book->horizontalHeader()->setStretchLastSection(true);
    connect(ui->tableView_book->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex &,
                                     const QModelIndex &)),
            this, SLOT(updateStuView()));
}

void MainWindow::createStuPanel()
{
    stuModel = new QSqlRelationalTableModel(this);
    stuModel->setTable("BOOK_USER");
    /*bookModel->select();
    for(int i=0;i<bookModel->rowCount();i++){
        QSqlRecord record=bookModel->record(i);
        QString t=record.value("BOOK_NAME").toString();
        qDebug()<<t;
    }*/
    stuModel->setHeaderData(User_uid, Qt::Horizontal, tr("Id"));
    stuModel->setHeaderData(User_name, Qt::Horizontal, tr("Name"));
    stuModel->setHeaderData(User_sex, Qt::Horizontal, tr("Publish"));
    stuModel->setHeaderData(User_pro, Qt::Horizontal, tr("Pro"));
    stuModel->setHeaderData(User_type, Qt::Horizontal, tr("Type"));
    stuModel->setHeaderData(User_rank, Qt::Horizontal, tr("Rank"));
    //stuModel->select();
    ui->tableView_stu->setModel(stuModel);
    updateViewTable();
}
void MainWindow::registerBookInfo(){
    int row = bookModel->rowCount();
    bookModel->insertRow(row);
    QModelIndex index = bookModel->index(row, Book_Name);
    qDebug()<<index.row();
    ui->tableView_book->setCurrentIndex(index);
    ui->tableView_book->edit(index);
}
void MainWindow::editBookInfo()
{
    int bookId = -1;
    QModelIndex index = ui->tableView_book->currentIndex();
    if (index.isValid()) {
         QSqlRecord record = bookModel->record(index.row());
         bookId = record.value(Book_Id).toInt();
     }

    bookInfo form(bookId, this);
    form.exec();
    updateBookView();
}

void MainWindow::findBookInfo()
{
    Find l(0);
    if(l.exec()){
        qDebug()<<QString("%1 ='%2'").arg(l.getContext(),l.getText());
        bookModel->setFilter(QString("%1 ='%2'").arg(l.getContext(),l.getText()));
        ui->tableView_book->update();
    }
}

void MainWindow::editStuInfo()
{
    int stuId = -1;
    QModelIndex index = ui->tableView_stu->currentIndex();
    if (index.isValid()) {
        QSqlRecord record;
        if(flag==all_stu)
            record = stuModel->record(index.row());
        else if(flag==select_stu)
            record = queryModel->record(index.row());
         stuId = record.value(User_uid).toInt();
     }

    stuInfo form(stuId, this);
    form.exec();
    updateStuView();
}

void MainWindow::registerStuInfo()
{
    int row = stuModel->rowCount();
    stuModel->insertRow(row);
    QModelIndex index = stuModel->index(row, Book_Name);
    qDebug()<<index.row();
    ui->tableView_stu->setCurrentIndex(index);
    ui->tableView_stu->edit(index);
}

void MainWindow::findStuInfo()
{
    Find l(1);
    if(l.exec()){
        stuModel->setFilter(QString("%1 = '%2'").arg(l.getContext(),l.getText()));
        ui->tableView_stu->update();
    }
}

void MainWindow::findRecord()
{

    QString record_b="\0";
    QModelIndex index = ui->tableView_stu->currentIndex();
    if (index.isValid()) {
         QSqlRecord record;
         if(flag==all_stu)
         {
             record = stuModel->record(index.row());
             record_b = record.value(Book_Id).toString();
         }
         else if(flag==select_stu)
         {
             record = queryModel->record(index.row());
             record_b = record.value(Book_Id).toString();
         }
         else
         {
             record=queryModel->record(index.row());
             record_b = record.value(1).toString();
         }
     }
    recordInfo form(record_b,nullptr);
    form.exec();
}

void MainWindow::setOverUpdateStuView()
{
    flag=over_time;
    ui->pushButton_stu_find->setEnabled(false);
    ui->pushButton_stu_register->setEnabled(false);
    ui->pushButton_stu_edit->setEnabled(false);
    updateStuView();
}

void MainWindow::setAllUpdateStuView()
{
    flag=all_stu;
    ui->pushButton_stu_find->setEnabled(true);
    ui->pushButton_stu_register->setEnabled(true);
    ui->pushButton_stu_edit->setEnabled(true);
    updateStuView();
}

void MainWindow::setBookUpdateStuView()
{
    flag=select_stu;
    ui->pushButton_stu_find->setEnabled(false);
    ui->pushButton_stu_register->setEnabled(false);
    ui->pushButton_stu_edit->setEnabled(true);
    updateStuView();
}

void MainWindow::updateStuView()
{
    switch(flag) {
        case select_stu:
            updateSelectedStuView();
            break;
        case all_stu:
            updateAllStuView();
            break;
        case over_time:
            updateOverTimeStuView();
            break;
    }
    updateViewTable();
    ui->tableView_stu->horizontalHeader()->setVisible(bookModel->rowCount() > 0);
}

void MainWindow::updateSelectedStuView()
{
    QModelIndex index = ui->tableView_book->currentIndex();
    if (index.isValid()&&flag==select_stu) {
        QSqlRecord record = bookModel->record(index.row());
        QString id = record.value("ISBN").toString();
        if(queryModel==nullptr)
            queryModel=new QSqlQueryModel;
        queryModel->setQuery(QString(
                     "SELECT BOOK_USER.USER_UID,USER_NAME,USER_SEX,USER_PRO,USER_TYPE,USER_RANK FROM BOOK_USER,BORROW WHERE BOOK_USER.USER_UID=BORROW.USER_UID AND ISBN='%1'").arg(id));
        queryModel->setHeaderData(User_uid, Qt::Horizontal, tr("Id"));
        queryModel->setHeaderData(User_name, Qt::Horizontal, tr("Name"));
        queryModel->setHeaderData(User_sex, Qt::Horizontal, tr("Publish"));
        queryModel->setHeaderData(User_pro, Qt::Horizontal, tr("Pro"));
        queryModel->setHeaderData(User_type, Qt::Horizontal, tr("Type"));
        queryModel->setHeaderData(User_rank, Qt::Horizontal, tr("Rank"));
        ui->tableView_stu->setModel(queryModel);
        }
}

void MainWindow::updateAllStuView()
{
    stuModel->select();
    ui->tableView_stu->setModel(stuModel);
}

void MainWindow::updateOverTimeStuView()
{
    if(queryModel==nullptr)
        queryModel=new QSqlQueryModel;
    qDebug()<<QString("select BOOK_NAME,BORROW.USER_UID,USER_NAME,TO_NUMBER(TO_DATE(SYSDATE)-DATE_TETURN_LIMIT) from BOOK,BOOK_USER,BORROW where BOOK.ISBN=BORROW.ISBN AND BOOK_USER.USER_UID=BORROW.USER_UID AND TO_NUMBER(DATE_TETURN_LIMIT-TO_DATE(SYSDATE))<0;");
    queryModel->setQuery(QString("select BOOK_NAME,BORROW.USER_UID,USER_NAME,TO_NUMBER(TO_DATE(SYSDATE)-DATE_TETURN_LIMIT) from BOOK,BOOK_USER,BORROW where BOOK.ISBN=BORROW.ISBN AND BOOK_USER.USER_UID=BORROW.USER_UID AND TO_NUMBER(DATE_TETURN_LIMIT-TO_DATE(SYSDATE))<0"));
    queryModel->setHeaderData(User_uid, Qt::Horizontal, tr("BOOK_NAME"));
    queryModel->setHeaderData(User_name, Qt::Horizontal, tr("USER_UID"));
    queryModel->setHeaderData(User_sex, Qt::Horizontal, tr("USER_NAME"));
    queryModel->setHeaderData(User_pro, Qt::Horizontal, tr("OVER_DAY"));
    ui->tableView_stu->setModel(queryModel);
}

void MainWindow::updateBookView()
{
    bookModel->select();
    ui->tableView_book->horizontalHeader()->setVisible(bookModel->rowCount() > 0);
}

void MainWindow::allRecord(){
    QString record="\0";
    recordInfo form(record,nullptr);
    form.exec();
}
void MainWindow::updateViewTable(){
    ui->tableView_stu->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->tableView_stu->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_stu->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->tableView_book->setColumnHidden(Book_Id, true);
    ui->tableView_stu->resizeColumnsToContents();
    ui->tableView_stu->horizontalHeader()->setStretchLastSection(true);
}
void MainWindow::all_show(){
    bookModel->setFilter("");
    stuModel->setFilter("");
    ui->tableView_book->update();
    ui->tableView_stu->update();
}
