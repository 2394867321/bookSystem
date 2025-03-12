#ifndef ADDRECORDINFO_H
#define ADDRECORDINFO_H

#include <QDialog>

namespace Ui {
class AddRecordInfo;
}
enum{
    Book_Name=0,
    Stu_Name=1,
    Borrow_time=2,
    Return_time=3
};
class QSqlRelationalTableModel;
class QSqlQueryModel;
class QDataWidgetMapper;
class AddRecordInfo : public QDialog
{
    Q_OBJECT

public:
    explicit AddRecordInfo(QWidget *parent = nullptr);
    ~AddRecordInfo();
private:
    Ui::AddRecordInfo *ui;
    QSqlRelationalTableModel* tableModel;
    QSqlQueryModel* tableModel_b;
    QSqlQueryModel* tableModel_u;
    QDataWidgetMapper* mapper_b;
    QDataWidgetMapper* mapper_u;
    QDataWidgetMapper* mapper;
private slots:
    void addData();
    void cancel();
};

#endif // ADDRECORDINFO_H
