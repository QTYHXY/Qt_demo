#include "myproject.h"
#include "ui_myproject.h"
#include <QDebug>

#include <QApplication>
#include <opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextToSpeech>

#include <easypr.h>
#include <easypr/util/switch.hpp>


using namespace cv;
using namespace easypr;

MyProject::MyProject(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyProject)
{
    ui->setupUi(this);
    //设置定时器
    timerId = this->startTimer(700);
    //获取系统可用的摄像头
    QList<QCameraInfo> infos = QCameraInfo::availableCameras();
    //打印
    foreach(QCameraInfo info,infos){
        qDebug()<<info.description()<<info.deviceName();
        cameraList<<info.deviceName();
    }
    //绑定数据库
    connectMyDB();
    //构造摄像头对象
    ca1 = new QCamera(cameraList[0].toUtf8(),this);
    ca2 = new QCamera(cameraList[1].toUtf8(),this);
    //构造截屏对象
    cap1 = new QCameraImageCapture(ca1,this);
    cap2 = new QCameraImageCapture(ca2,this);
    //构造显示窗口
    w1 = new QCameraViewfinder(ui->camera1);
    w2 = new QCameraViewfinder(ui->camera2);
    //调整大小
    w1->resize(ui->camera1->size());
    w2->resize(ui->camera2->size());
    //设置摄像头的显示窗口
    ca1->setViewfinder(w1);
    ca2->setViewfinder(w2);
    w1->show();
    w2->show();

    //启动摄像头
    ca1->start();
    ca2->start();

    //连接截屏完成信号
   QObject::connect(cap1,&QCameraImageCapture::imageCaptured,this,&MyProject::captureCamera1);
    QObject::connect(cap2,&QCameraImageCapture::imageCaptured,this,&MyProject::captureCamera2);

    //动态分配空间
    capThread=new CaptureThread(this);
    //创建子线程
    //thread = new QThread(this);
    //把自定义线程加入到子线程中
    //capThread->moveToThread(thread);
    connect(this,&MyProject::emitStartThread,capThread,&CaptureThread::dealCapture);

    //设置显示车牌
    QObject::connect(capThread,&CaptureThread::emitResult,this,[=](QString str){
        //ui->plateNumber->setText(str);
    });
    QObject::connect(capThread,&CaptureThread::emitFlag,this,[=](QString str)
    {
        ui->flagLabel->setText(str);
        if(!str.isEmpty()){
            tts->say(str);
        }
    });

    QStringList engines= QTextToSpeech::availableEngines();//可用引擎
    qDebug()<<engines;
    tts=new QTextToSpeech(engines[0], this);//构造QTextToSpeech对象需要传入引擎
    //capThread->start();
}

MyProject::~MyProject()
{
    delete ui;
    //thread->terminate();
}

void MyProject::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == this->timerId)
    {
        cap1->capture("C:/Users/qty/Desktop/project/MyProject/captureImage/1.jpg");
        cap2->capture("C:/Users/qty/Desktop/project/MyProject/captureImage/2.jpg");
    }
}

void MyProject::connectMyDB()
{

}

void MyProject::captureCamera1(int id,const QImage &preview)
{
    //将QImage转换成QPixmap
    QPixmap map = QPixmap::fromImage(preview);
    //显示
    map = map.scaled(ui->label1->size());
    ui->label1->setPixmap(map);
    emit emitStartThread(preview);
    QObject::connect(capThread,&CaptureThread::emitComplete,this,[&]{
       qDebug()<<"识别完成";
    });
}

void MyProject::captureCamera2(int id,const QImage &preview)
{
    //将QImage转换成QPixmap
    QPixmap map = QPixmap::fromImage(preview);

    //显示
    map = map.scaled(ui->label2->size());
    ui->label2->setPixmap(map);
    emit emitStartThread(preview);
    QObject::connect(capThread,&CaptureThread::emitComplete,this,[&]{
       qDebug()<<"识别完成";
    });
}


void MyProject::on_pushButton_clicked()
{
    loginWidget = new Login;
    loginWidget->show();
}
