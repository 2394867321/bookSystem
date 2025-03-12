#ifndef FIND_H
#define FIND_H

#include <QDialog>

namespace Ui {
class Find;
}

class Find : public QDialog
{
    Q_OBJECT

public:
    explicit Find(int flag,QWidget *parent = nullptr);
    ~Find();
    QString getText();
    QString getContext();
private:
    Ui::Find *ui;
    QStringList str_b={"ISBN","BOOK_NAME","BOOK_PUB","BOOK_PRICE","BOOK_AUTHOR","BOOK_RECORD"};
    QStringList str_u={"USER_UID","USER_NAME","USER_TYPE","USER_SEX","USER_PRO","USER_RANK"};
};

#endif // FIND_H
