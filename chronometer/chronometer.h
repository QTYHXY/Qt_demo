#ifndef CHRONOMETER_H
#define CHRONOMETER_H

#include <QWidget>
#include <QTimer>
#include <QTime>
QT_BEGIN_NAMESPACE
namespace Ui { class Chronometer; }
QT_END_NAMESPACE

class Chronometer : public QWidget
{
    Q_OBJECT

public:
    Chronometer(QWidget *parent = nullptr);
    ~Chronometer();
    virtual void timerEvent(QTimerEvent *event);
private slots:
    void on_startBtn_clicked();

    void on_recordBtn_clicked();

    void on_resetBtn_clicked();

    void on_stopBtn_clicked();

private:
    Ui::Chronometer *ui;
    int timerId;//定时器id
    QTime time;
    bool flag;
};
#endif // CHRONOMETER_H
