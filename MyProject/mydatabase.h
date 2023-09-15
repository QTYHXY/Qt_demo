#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QWidget>
#include <QSqlTableModel>
#include <QPushButton>
#include <QLabel>
#include "capturethread.h"
namespace Ui {
class MyDatabase;
}

class MyDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit MyDatabase(QWidget *parent = nullptr);
    ~MyDatabase();

private slots:

    void on_refreshBtn_clicked();

    void on_insertBtn_clicked();

    void on_changeBtn_clicked();

    void on_deleteBtn_clicked();

    void on_selectBtn_clicked();

    void on_selectDateBtn_clicked();

    void on_selectTimeBtn_clicked();

private:
    Ui::MyDatabase *ui;
    QSqlDatabase db;
    QStandardItemModel *model;
    int id;
    QLabel *label;
};

#endif // MYDATABASE_H
