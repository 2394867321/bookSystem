#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
enum{
    over_time=0,
    all_stu=1,
    select_stu=2
};
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QSqlDatabase;
class QSqlRelationalTableModel;
class QSqlQueryModel;
class QButtonGroup;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,QSqlDatabase *db=nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase *db;
    QSqlRelationalTableModel* bookModel;
    QSqlRelationalTableModel* stuModel;
    QSqlQueryModel* queryModel;
    QButtonGroup *block;
    int flag;
    void createBookPanel();
    void createStuPanel();
    void updateViewTable();
    void updateTable();
private slots:
    void registerBookInfo();
    void editBookInfo();
    void findBookInfo();
    void editStuInfo();
    void registerStuInfo();
    void findStuInfo();
    void findRecord();
    void setOverUpdateStuView();
    void setAllUpdateStuView();
    void setBookUpdateStuView();
    void updateStuView();
    void updateBookView();
    void allRecord();
    void all_show();
};
#endif // MAINWINDOW_H
