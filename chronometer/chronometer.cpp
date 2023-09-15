#include "chronometer.h"
#include "ui_chronometer.h"

#include <QString>
#include<QTime>
#include <QDebug>
#include <QMessageBox>
Chronometer::Chronometer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chronometer)
{
    ui->setupUi(this);
    time.setHMS(0,0,0);
    flag=true;
}

Chronometer::~Chronometer()
{
    killTimer(timerId);
    flag=true;
    delete ui;
}

void Chronometer::on_startBtn_clicked()
{
    if(flag)
    {
        timerId = startTimer(10);
        flag=false;
    }
    else
    {
        QMessageBox::warning(this,"提示","已经启动");
    }
}

void Chronometer::on_recordBtn_clicked()
{
        QString str = ui->label->text();
        ui->listWidget->addItem(str);
}

void Chronometer::on_resetBtn_clicked()
{

        time.setHMS(0,0,0);
        QString str = time.toString("hh:mm:ss.zzz");
        ui->label->setText(str);
        ui->listWidget->clear();
}

void Chronometer::timerEvent(QTimerEvent *event)
{
    time=time.addMSecs(10);
    QString str = time.toString("hh:mm:ss.zzz");
    ui->label->setText(str);
}

void Chronometer::on_stopBtn_clicked()
{
    if(!flag)
    {
        killTimer(timerId);
        flag=true;
    }
    else
    {
        QMessageBox::warning(this,"提示","已经暂停");
    }
}
