#ifndef MYPROJECT_H
#define MYPROJECT_H

#include <QWidget>
#include <QCamera>
#include <QCameraInfo>
#include <QGraphicsVideoItem>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QSqlTableModel>
#include <QTextToSpeech>

#include "capturethread.h"
#include "login.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MyProject; }
QT_END_NAMESPACE

class MyProject : public QWidget
{
    Q_OBJECT

public:
    MyProject(QWidget *parent = nullptr);
    ~MyProject();
    //重写定时器的事件   虚函数 子类重写父类的虚函数
    virtual void timerEvent(QTimerEvent *e);
    void plateRecognition(const QImage &preview);
    void connectMyDB();

private slots:
    void captureCamera1(int id,const QImage &preview);
    void captureCamera2(int id,const QImage &preview);
    void on_pushButton_clicked();

signals:
    void emitStartThread(const QImage &preview);
private:
    Ui::MyProject *ui;
    QCamera *ca1;//摄像头对象
    QCamera *ca2;//摄像头对象
    QCameraViewfinder *w1;//显示窗口
    QCameraViewfinder *w2;//显示窗口
    QStringList cameraList;
    QCameraImageCapture *cap1;//截屏对象
    QCameraImageCapture *cap2;
    int timerId;//定时器id

    CaptureThread *capThread;
    QThread *thread;

    //窗口
    Login *loginWidget;
    QSqlTableModel *model;
    QTextToSpeech *tts;

};
#endif // MYPROJECT_H
