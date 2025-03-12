#ifndef RECORDINFO_H
#define RECORDINFO_H

#include <QDialog>

namespace Ui {
class recordInfo;
}
class QSqlRelationalTableModel;
class QSqlQueryModel;
class QDataWidgetMapper;
class recordInfo : public QDialog
{
    Q_OBJECT

public:
    explicit recordInfo(QString id,QWidget *parent = nullptr);
    ~recordInfo();
private:
    Ui::recordInfo *ui;
    void createCurrentPanel(QString id);
    //QSqlRelationalTableModel *tableModel;
    QSqlQueryModel* tableModel;
    QDataWidgetMapper *mapper;
    QString id;
private slots:
    void updateData();
    void addData();
    void deleteData();
};

#endif // RECORDINFO_H
