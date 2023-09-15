#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include <QWidget>
#include <QCamera>
#include <QCameraInfo>
#include <QGraphicsVideoItem>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QMutex>
#include <QReadWriteLock>
class CaptureThread : public QThread
{
    Q_OBJECT
public:
    explicit CaptureThread(QObject *parent=0);
    ~CaptureThread();
    bool selectCar(QString carNumber);
signals:
    void emitResult(QString str);
    void emitFlag(QString str);
    void emitComplete();
public slots:
    void dealCapture(const QImage &preview);
protected:
    void run() override;
private:
    QCameraImageCapture *cap1;//截屏对象
    QCameraImageCapture *cap2;
    QSqlDatabase db;
    QStandardItemModel *model;
    QReadWriteLock  mutex;
    int   i;
    QString entryTimeTemp;
    int charge;
    static int count;
    QList<QString> tempList;
    bool flag;
    int isComplete;
    QList<QDateTime> timeList;
    int isId;
};

#endif // CAPTURETHREAD_H
