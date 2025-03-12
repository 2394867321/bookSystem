#include "stuinfo.h"
#include "ui_stuinfo.h"
#include <QtSql>
#include <QtWidgets>
stuInfo::stuInfo(int id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stuInfo)
{
    ui->setupUi(this);
    tableModel = new QSqlRelationalTableModel(this);
    tableModel->setTable("BOOK_USER");
   //tableModel->setSort(Employee_Name, Qt::AscendingOrder);
    tableModel->select();
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->lineEdit_stu_name, User_name);
    mapper->addMapping(ui->lineEdit_class, User_type);
    mapper->addMapping(ui->lineEdit_rank, User_rank);
    mapper->addMapping(ui->lineEdit_stu_id, User_uid);
    mapper->addMapping(ui->lineEdit_unit,User_pro);
    mapper->addMapping(ui->comboBox_sex,User_sex);
    if (id != -1) {
         for (int row = 0; row < tableModel->rowCount(); ++row) {
         QSqlRecord record = tableModel->record(row);
         if (record.value(User_uid).toInt() == id) {
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
    connect(ui->pushButton_new, SIGNAL(clicked()), this, SLOT(addStu()));
    connect(ui->pushButton_delete, SIGNAL(clicked()),this, SLOT(deleteStu()));
    if (id == -1) {
            ui->pushButton_next->setFocus();
        } else {
            ui->lineEdit_stu_name->setFocus();
        }
}

stuInfo::~stuInfo()
{
    delete ui;
}

void stuInfo::done(int result)
{
    mapper->submit();
    QDialog::done(result);
}
void stuInfo::addStu(){
    int row = mapper->currentIndex();
    mapper->submit();
    tableModel->insertRow(row);
    mapper->setCurrentIndex(row);
    ui->lineEdit_stu_id->clear();
    ui->lineEdit_stu_name->clear();
    ui->lineEdit_rank->clear();
    ui->lineEdit_unit->clear();
    ui->lineEdit_class->clear();
    ui->lineEdit_stu_id->setFocus();
}
void stuInfo::deleteStu(){
    int row = mapper->currentIndex();
    tableModel->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}
