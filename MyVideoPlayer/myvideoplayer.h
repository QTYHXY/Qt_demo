#ifndef MYVIDEOPLAYER_H
#define MYVIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class MyVideoPlayer; }
QT_END_NAMESPACE

class MyVideoPlayer : public QWidget
{
    Q_OBJECT

public:
    MyVideoPlayer(QWidget *parent = nullptr);
    ~MyVideoPlayer();
    //重写事件过滤器处理函数
    virtual bool eventFilter(QObject *watched,QEvent *event);
private slots:
    void on_playBtn_clicked();

    void on_fullBtn_clicked();

    void on_addBtn_clicked();

    void on_stopBtn_clicked();

    void on_exitBtn_clicked();

    void on_deleteBtn_clicked();

    void on_prevBtn_clicked();

    void on_nextBtn_clicked();

private:
    Ui::MyVideoPlayer *ui;
    QLabel *myTable;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QStringList names;
    int count;
    int index;
    QString videoDuration;
};
#endif // MYVIDEOPLAYER_H
