#include "find.h"
#include "ui_find.h"

Find::Find(int flag,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Find)
{
    ui->setupUi(this);
    if(!flag)
        ui->comboBox->addItems(str_b);
    else
        ui->comboBox->addItems(str_u);
}

Find::~Find()
{
    delete ui;
}
QString Find::getText(){
    return ui->lineEdit->text();
}
QString Find::getContext(){
    return ui->comboBox->currentText();
}
