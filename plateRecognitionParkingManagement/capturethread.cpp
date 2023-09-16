#include "capturethread.h"

#include <QApplication>
#include <opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <easypr.h>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QDateTime>
#include <QSqlRecord>

#include <easypr/util/switch.hpp>


using namespace cv;
using namespace easypr;

CaptureThread::CaptureThread(QObject *parent):QThread(parent)
{
    i=0;
    charge=0;
    isComplete=0;
    isId=0;
    tempList<<"0"<<"0"<<"0";
    timeList<<QDateTime::currentDateTime()<<QDateTime::currentDateTime();

    //1.添加数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    //2.指定数据库文件路径
    db.setDatabaseName("car.db");
    //3.打开数据库
    if(!db.open())
    {
        qDebug()<<"打开失败";
    }
    QString sql = QString("create table if not exists carInfo(id INTEGER PRIMARY KEY ,carNumber ntext,entryTime ntext unique,departureTime ntext  ,entryImagePath ntext unique,departureImagePath ntext,charge ntext,flag int);");

    //执行sql
    QSqlQuery query;
    if(!query.exec(sql)){
        qDebug()<<"失败";
    }
    db.close();
    //qDebug()<<"-------------------------子线程"<<QThread::currentThread();
}

CaptureThread::~CaptureThread()
{
    db.close();
    //QSqlDatabase::removeDatabase(db.connectionName());
}

int CaptureThread::count =0;
void CaptureThread::dealCapture(const QImage &preview)
{
    db.open();

    //qDebug()<<"子线程"<<QThread::currentThread();
    //车牌识别
    CPlateRecognize pr;
    pr.setResultShow(false);//不展示过程
    //通过文字，颜色， 边沿识别车牌
    pr.setDetectType(PR_DETECT_CMSER|PR_DETECT_COLOR|PR_DETECT_SOBEL);
    pr.setLifemode(true);//生活模式

    vector<CPlate> plateVec;
    Mat src = Mat(preview.height(),preview.width(),CV_8UC4,(void *)preview.bits(),preview.bytesPerLine());
    //easyPR只能处理三通道图片
    cvtColor(src,src,CV_RGBA2RGB);

    //识别
    int result = pr.plateRecognize(src, plateVec);
    if(result==0&&!plateVec.empty())
    {
        CPlate plate = plateVec.at(0);
        string license = plate.getPlateStr();//车牌识别结果字符串
        qDebug()<<QString::fromLocal8Bit(license.c_str());
        emit emitResult(QString::fromLocal8Bit(license.c_str()));
        //获取车牌
        QString carNumber = QString::fromLocal8Bit(license.c_str());
        // 获取当前时间
        QDateTime currentDateTime = QDateTime::currentDateTime();
        //mutex.lockForWrite();

        qDebug()<<"离开"<<selectCar(carNumber)<<flag<<isComplete;
        if(carNumber !="蓝牌"&&carNumber !="黄牌"&&carNumber != "白牌")
        {
            qDebug()<<"count"<<count;
            if(count>=3){
                count=0;
            }
            tempList.replace(count++,carNumber);
            qDebug()<<"change tempName[0]"<<tempList.at(0);
            qDebug()<<"change tempName[1]"<<tempList.at(1);
            qDebug()<<"change tempName[2]"<<tempList.at(2);

            if(tempList.at(0)==tempList.at(1)&&tempList.at(0)==tempList.at(2)&&tempList.at(1)==tempList.at(2)) {
                count=0;
                flag=true;
                qDebug()<<"flag==true";
            }else{
                flag=false;
                qDebug()<<"flag==false";
            }
            if(!selectCar(carNumber)&&flag==true)
            {
                //找id进行修改
                QString sql2 = QString("select * from carInfo where carNumber='%1';").arg(carNumber);
                //执行sql
                QSqlQuery query2;
                if(query2.exec(sql2)){
                    while (query2.next()) {
                        if(query2.value("departureTime").toString()=="离开时间"&&query2.value("flag").toInt()!=2)
                        {
                            int id = query2.value("id").toInt();
                            isId =id;
                            qDebug() << "-----------------------------------------isid " << id;
                            break;
                        }
                    }
                }
                // 添加离开时间
                QString sql = QString("select * from carInfo where id =%1;").arg(isId);
                //执行sql
                QSqlQuery query;
                if(query.exec(sql)){
                    while (query.next()) {
                        entryTimeTemp= QString(query.value("entryTime").toString());
                        //转换格式
                        QString format = "yyyy-MM-dd hh:mm:ss";
                        QDateTime dateTime = QDateTime::fromString(entryTimeTemp, format);
                        // 计算时间差
                        qint64 seconds = dateTime.secsTo(currentDateTime); // 计算秒数差
                        if(seconds<=900){
                            charge =15;
                        }else if(seconds>900&&seconds<=1800){
                            charge=30;
                        }else if(seconds>1800&&seconds<=3600){
                            charge=60;
                        }else if(seconds>3600){
                            charge=10000;
                        }
                        if(seconds>60) {
                            emitFlag(QString("谢谢%1").arg(carNumber));
                            //拆分车牌名
                            QString tempName = carNumber.mid(3);  // 获取字符串从第三个字符开始的部分
                            QString savePath = QString("C:/Users/qty/Desktop/project/MyProject/image/departure/%1-%2.png").arg(tempName).arg( currentDateTime.toString("yyyy-MM-dd-hh-mm"));
                            if(preview.save(savePath))
                            {
                                // 准备更新语句
                                QString updateSql = QString("update carInfo set departureTime = '%1',departureImagePath='%2',charge ='%3',flag='%4' WHERE id = %5;")
                                        .arg(currentDateTime.toString("yyyy-MM-dd hh:mm:ss"))
                                        .arg(savePath)
                                        .arg(charge)
                                        .arg(2)
                                        .arg(isId);
                                // 执行更新语句
                                QSqlQuery updateQuery;
                                if (updateQuery.exec(updateSql)) {
                                    isId=0;
                                    qDebug() << "更新完成 ";
                                    tempList.replace(0,"carNumber");
                                    tempList.replace(1,"carNumber");
                                    tempList.replace(2,"carNumber");
                                    return;
                                }
                            }
                        }else{
                            return;
                        }
                    }
                }
                //sleep(1);
                QString sql3 = QString("select * from carInfo where carNumber='%1';").arg(carNumber);
                //执行sql
                QSqlQuery query3;
                if(query3.exec(sql2)){
                    while (query3.next()) {
                        int id = query3.value("id").toInt();
                        int flag= query3.value("flag").toInt();
                        QDateTime tempStart = query3.value("entryTime").toDateTime();
                        QDateTime tempEnd = query3.value("departureTime").toDateTime();
                        qDebug()<<tempStart<<tempEnd;
                        qint64 temp = tempStart.secsTo(tempEnd);
                        if(temp<60)
                        {
                            //删除不合理的数据
                            QString sql = QString("delete from carInfo where id='%1';").arg(id);
                            QSqlQuery query;
                            query.exec(sql);
                        }
                        isComplete=flag;
                        qDebug() << "id " << id;
                        qDebug() << "flag " << flag;
                    }
                }

            }
        }

        //添加到数据库
        if(carNumber !="蓝牌"&&carNumber !="黄牌"&&carNumber != "白牌")
        {
            qDebug()<<"count"<<count;
            if(count>=3){
                count=0;
            }
            tempList.replace(count++,carNumber);
            qDebug()<<" add tempName[0]"<<tempList.at(0);
            qDebug()<<"add tempName[1]"<<tempList.at(1);
            qDebug()<<"add tempName[2]"<<tempList.at(2);

            if(tempList.at(0)==tempList.at(1)&&tempList.at(0)==tempList.at(2)&&tempList.at(1)==tempList.at(2)) {
                count=0;
                flag=true;
                qDebug()<<"flag==true";
            }else{
                flag=false;
                qDebug()<<"flag==false";
            }
            qDebug()<<"添加"<<selectCar(carNumber)<<flag<<isComplete;
            if(!selectCar(carNumber)&&flag==true)
            {
                //拆分车牌名
                QString tempName = carNumber.mid(3);  // 获取字符串从第三个字符开始的部分
                QString savePath = QString("C:/Users/qty/Desktop/project/MyProject/image/enter/%1-%2.png").arg(tempName).arg( currentDateTime.toString("yyyy-MM-dd-hh-mm"));
                if(preview.save(savePath))
                {
                    //数据库添加
                    QString sql = QString("insert or ignore into carInfo(carNumber,entryTime,departureTime,entryImagePath,departureImagePath,charge,flag) values('%1','%2','%3','%4','%5','%6',%7);")
                            .arg(carNumber)
                            .arg(currentDateTime.toString("yyyy-MM-dd hh:mm:ss"))
                            .arg("离开时间")
                            .arg(savePath)
                            .arg("照片路径")
                            .arg("费用")
                            .arg(1);
                    //执行sql
                    QSqlQuery query(sql);
                    if(query.exec()){
                        emitFlag(QString("欢迎%1").arg(carNumber));
                        qDebug()<<"-----------------------------**********添加成功";
                        emitComplete();
                    }
                }
            }
        }
        //mutex.unlock();
    }

    else{
        qDebug()<<"没有识别到车牌!";
    }
    db.close();
    //QSqlDatabase::removeDatabase(db.connectionName());

}


void CaptureThread::run()
{
    qDebug()<<"子线程-----------------------------------------------"<<QThread::currentThread();
}

bool CaptureThread::selectCar(QString carNumber)
{
    QString sql = QString("select * from carInfo where carNumber='%1';").arg(carNumber);
    //执行sql
    QSqlQuery query;
    if(query.exec(sql)){
        return false;
    }
    else
    {
        return true;
    }
}
