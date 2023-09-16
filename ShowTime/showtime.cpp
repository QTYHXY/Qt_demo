#include "showtime.h"
#include "ui_showtime.h"
#include  <QDateTime>
#include <QString>
#include <QMessageBox>
Showtime::Showtime(QWidget *parent)
    : QWidget(parent,Qt::WindowStaysOnTopHint)
    , ui(new Ui::Showtime)
{
    ui->setupUi(this);
    flag=false;
}

Showtime::~Showtime()
{
    delete ui;
}

void Showtime::on_startBtn_clicked()
{
    if(flag==false)
    {
        timer_id = startTimer(1000);
        flag=true;
    }
    else
    {
         QMessageBox::warning(this,"提示","已启用");
    }
}

void Showtime::on_stopBtn_clicked()
{
    if(flag)
    {
        killTimer(timer_id);
        flag=false;
    }
    else
    {

        QMessageBox::warning(this,"提示","已停止");
    }
}

void Showtime::timerEvent(QTimerEvent *event)
{
    QDateTime dt = QDateTime::currentDateTime();
    QString str = dt.toString("yyyy-MM-dd MMMd号 ddd hh:mm:ss");
    ui->label->setText(str);
}
