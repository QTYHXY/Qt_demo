#include "myvideoplayer.h"
#include "ui_myvideoplayer.h"
#include <QMouseEvent>
#include <QLabel>
#include <QSlider>
#include <QFileDialog>
#include <iostream>
#include <QMessageBox>
MyVideoPlayer::MyVideoPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyVideoPlayer)
{
    ui->setupUi(this);
    player=new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    count =0;
    index=0;
    //给视频播放窗口按照过滤器
    videoWidget->installEventFilter(this);
    //添加到部件管理器
    ui->verticalLayout->addWidget(videoWidget);
    //设置视频播放窗口
    player->setVideoOutput(videoWidget);

    connect(ui->volumeSlider,&QSlider::valueChanged,ui->volumeLabel,[=](){
        ui->volumeLabel->setText(QString("音量：%1").arg(ui->volumeSlider->value()));
    });
    connect(ui->timeSlider,&QSlider::valueChanged,ui->timeLabel,[=](){
        //        qint64 currentTime = player->position();
        ui->timeLabel->setText(QString("%1 : %2").arg("60").arg("3"));
    });

    // 连接 Slider 的 sliderMoved() 信号与槽函数
    connect(ui->timeSlider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);
    //连接 Slider 的 valueChanged() 信号与槽函数
    connect(ui->volumeSlider,&QSlider::valueChanged,player,&QMediaPlayer::setVolume);

    //周期改变
    QObject::connect(player,&QMediaPlayer::durationChanged,this,[&](qint64 duration){
        //修改滑动条的最大值
        ui->timeSlider->setRange(0,duration);
        //显示周期
        videoDuration = QString("%1:%2").arg(duration/1000/60, 2, 10, QChar('0')).arg(duration/1000%60, 2, 10, QChar('0'));
        ui->timeLabel->setText(QString("00:00/%1").arg(videoDuration));
    });

    //进度改变
    QObject::connect(player,&QMediaPlayer::positionChanged,this,[&](qint64 position){
        //更新滑动条位置
        ui->timeSlider->setValue(position);
        //显示进度
        ui->timeLabel->setText(QString("%1:%2/%3").arg(position/1000/60, 2, 10, QChar('0')).
                               arg(position/1000%60, 2, 10, QChar('0')).arg(videoDuration));
    });

    QObject::connect(player, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            qDebug()<<"播放完成";
            int row = ui->listWidget->currentRow();
            if(row==ui->listWidget->count()-1)
                row = 0;
            else
                row++;

            ui->listWidget->setCurrentRow(row);
            on_playBtn_clicked();
        }
    });
}

MyVideoPlayer::~MyVideoPlayer()
{
    delete ui;
}

bool MyVideoPlayer::eventFilter(QObject *watched, QEvent *event)
{
    //按键事件
    if(event->type()==QEvent::KeyPress){
        QKeyEvent *keyEevent=dynamic_cast<QKeyEvent *>(event);
        if(keyEevent->key()==Qt::Key_Escape){
            //恢复播放窗口位子窗口
            videoWidget->setWindowFlags(Qt::SubWindow);
            //退出全屏
            videoWidget->showNormal();
        }
    }
}

void MyVideoPlayer::on_playBtn_clicked()
{
    index = ui->listWidget->currentRow();
    ui->listWidget->setCurrentRow(index);
    QString currentName = names[index];

    qDebug()<<currentName<<index;
    //设置文件
    player->setMedia(QUrl::fromLocalFile(currentName));
    player->setVolume(ui->volumeSlider->value());
    player->play();
}

void MyVideoPlayer::on_fullBtn_clicked()
{
    videoWidget->setWindowFlags(Qt::Window);
    videoWidget->showFullScreen();
}

void MyVideoPlayer::on_addBtn_clicked()
{
    names.append(QFileDialog::getOpenFileNames(this,"添加文件","..","MP4 Files(*.MP4);;AVI Files(*.avi)"));
    ui->listWidget->clear();
    foreach(const QString name,names){
        QStringList splitName = name.split("/");
        ui->listWidget->addItems(QStringList()<<splitName.last() );
        qDebug()<<splitName.last();
    }
}

void MyVideoPlayer::on_stopBtn_clicked()
{
    count+=1;
    if(count %2 !=0){
        player->pause();
        ui->stopBtn->setText("继续");
    }else{
        player->play();
        ui->stopBtn->setText("暂停");
    }
}

void MyVideoPlayer::on_exitBtn_clicked()
{
    QCoreApplication::quit();
}

void MyVideoPlayer::on_deleteBtn_clicked()
{
    index = ui->listWidget->currentRow();
    ui->listWidget->setCurrentRow(index);
    QString currentMedia = player->media().canonicalUrl().toString();
    qDebug()<<"delete    "<<currentMedia.split("/").last();
    if(names[index].split("/").last()==currentMedia.split("/").last())
    {
        QMessageBox::warning(this,"提示","该视频正在播放");
        return;
    }
    names.removeAt(index);
    ui->listWidget->takeItem(index);




}

void MyVideoPlayer::on_prevBtn_clicked()
{
    int sumCount =ui->listWidget->count();
    int prevCount = ui->listWidget->currentRow();
    prevCount--;
    qDebug()<<sumCount<<prevCount;
    if(prevCount<0){
        prevCount=sumCount-1;
    }
    ui->listWidget->setCurrentRow(prevCount);

    on_playBtn_clicked();

}

void MyVideoPlayer::on_nextBtn_clicked()
{
    int sumCount =ui->listWidget->count();
    int nextCount = ui->listWidget->currentRow();
    nextCount++;
    qDebug()<<sumCount<<nextCount;
    if(nextCount>sumCount-1){
        nextCount=0;
    }
    ui->listWidget->setCurrentRow(nextCount);

    on_playBtn_clicked();
}
