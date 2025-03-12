#ifndef MAINWINDOW_USER_H
#define MAINWINDOW_USER_H

#include <QMainWindow>

namespace Ui {
class MainWindow_User;
}
enum{
    over_time_record=0,
    all_record=1,
    select_record=2
};
class QSqlDatabase;
class QSqlQueryModel;
class QButtonGroup;
class MainWindow_User : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_User(QString uid,QWidget *parent = nullptr);
    ~MainWindow_User();

private:
    Ui::MainWindow_User *ui;
    QSqlDatabase *db;
    QSqlQueryModel* tableModel;
    QSqlQueryModel* tableModel_b;
    QButtonGroup *block;
    int flag;
    QString uid;
    void createBookPanel();
    void createRecordPanel();
    void updateTable();
    //void updateViewTable();
private slots:
    void borrowInfo();
    void findBookInfo();
    void returnInfo();
    void conInfo();
    void setOverUpdateStuView();
    void setAllUpdateStuView();
    void setBookUpdateStuView();
    void updateViewTable();
    void all_show();
};

#endif // MAINWINDOW_USER_H
