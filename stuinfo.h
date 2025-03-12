#ifndef STUINFO_H
#define STUINFO_H

#include <QDialog>

namespace Ui {
class stuInfo;
}
enum {
    User_uid=0,
    User_name=1,
    User_sex=2,
    User_pro=3,
    User_type=4,
    User_rank=5
};
class QSqlRelationalTableModel;
class QDataWidgetMapper;
class QSqlDatabase;
class stuInfo : public QDialog
{
    Q_OBJECT

public:
    explicit stuInfo(int id,QWidget *parent = nullptr);
    ~stuInfo();
    void done(int result);

private slots:
    void addStu();
    void deleteStu();
private:
    Ui::stuInfo *ui;
    QSqlRelationalTableModel *tableModel;
    QDataWidgetMapper *mapper;
};

#endif // STUINFO_H
