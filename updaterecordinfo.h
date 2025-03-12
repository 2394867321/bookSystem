#ifndef UPDATERECORDINFO_H
#define UPDATERECORDINFO_H

#include <QDialog>

namespace Ui {
class updateRecordInfo;
}
class QDataWidgetMapper;
class QSqlRelationalTableModel;
enum{
    Book_R_Name=0,
    Stu_R_Id=1,
    Stu_R_Name=2,
    Borrow_Date=3,
    Return_Date=4,
    Book_R_Id=5
};
class updateRecordInfo : public QDialog
{
    Q_OBJECT

public:
    explicit updateRecordInfo(QString stu,QString book,QWidget *parent = nullptr);
    ~updateRecordInfo();
    //void done(int result);
private:
    Ui::updateRecordInfo *ui;
    QDataWidgetMapper *mapper;
    QSqlRelationalTableModel *tableModel;
private slots:
    void updateData();
};

#endif // UPDATERECORDINFO_H
