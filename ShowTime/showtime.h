#ifndef SHOWTIME_H
#define SHOWTIME_H

#include <QWidget>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class Showtime; }
QT_END_NAMESPACE

class Showtime : public QWidget
{
    Q_OBJECT

public:
    Showtime(QWidget *parent = nullptr);
    ~Showtime();
    virtual void timerEvent(QTimerEvent *event);
private slots:
    void on_startBtn_clicked();

    void on_stopBtn_clicked();

private:
    Ui::Showtime *ui;
      int timer_id;//定时器id
      bool flag;
};
#endif // SHOWTIME_H
