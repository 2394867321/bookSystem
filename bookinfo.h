#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QDialog>

namespace Ui {
class bookInfo;
}
class QSqlRelationalTableModel;
class QDataWidgetMapper;
class QSqlDatabase;
class QSqlQueryModel;
enum {
    Book_Id = 0,
    Book_Name = 1,
    Book_Author =2,
    Book_Price =3,
    Book_Pub=4,
    Book_Record=5
};
class bookInfo : public QDialog
{
    Q_OBJECT

public:
    explicit bookInfo(int id,QWidget *parent = nullptr);
    ~bookInfo();
    void done(int result);

private slots:
    void addBook();
    void deleteBook();

private:
    QSqlRelationalTableModel *tableModel;
    QDataWidgetMapper *mapper;
    Ui::bookInfo *ui;
    QSqlDatabase *db;
    //QSqlQueryModel *tableModel;
};

#endif // BOOKINFO_H
