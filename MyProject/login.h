#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <mydatabase.h>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_loginBtn_clicked();

    void on_changeBtn_clicked();

private:
    Ui::Login *ui;
    MyDatabase *mydb;
    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif // LOGIN_H
