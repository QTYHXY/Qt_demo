#include "mydatabase.h"
#include "ui_mydatabase.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QHBoxLayout>
#include <QItemSelectionModel>
MyDatabase::MyDatabase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyDatabase)
{
    ui->setupUi(this);

    //初始化模型
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderItem(0,new QStandardItem("序号"));
    model->setHorizontalHeaderItem(1,new QStandardItem("车牌"));
    model->setHorizontalHeaderItem(2,new QStandardItem("进入时间"));
    model->setHorizontalHeaderItem(3,new QStandardItem("离开时间"));
    model->setHorizontalHeaderItem(4,new QStandardItem("进入时照片路径"));
    model->setHorizontalHeaderItem(5,new QStandardItem("离开时照片路径"));
    model->setHorizontalHeaderItem(6,new QStandardItem("费用"));
    //绑定模型和表格
    ui->tableView->setModel(model);
    //1.添加数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    //2.指定数据库文件路径
    db.setDatabaseName("car.db");
    //3.打开数据库
    if(!db.open())
    {
        qDebug()<<"打开失败";
    }
    this->setStyleSheet("QLabel { width: 200px; height: 50px;font-size: 25px; }"
                        "QLineEdit { width: 200px; height: 50px;font-size: 25px; }"
                        "QPushButton{height:50px;}"
                        );
    QObject::connect(ui->lineEdit,&QLineEdit::textChanged,ui->selectBtn,[=](){
        ui->selectBtn->setEnabled(true);
    });

    // 创建选择模型
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model, this);

    // 将选择模型与数据模型关联
    ui->tableView->setSelectionModel(selectionModel);

    // 监听选择变化
    QObject::connect(selectionModel, &QItemSelectionModel::selectionChanged, model,[&](const QItemSelection &selected, const QItemSelection &deselected) {
        // 处理选择变化，输出选中的行号和数据
        for (const QModelIndex &index : selected.indexes()) {
            //qDebug() << "Selected column:" << index.column();
            if(index.column()==4||index.column()==5)
            {
                QVariant selectedData = model->data(index);
                // 打印内容
                qDebug() << "Selected Content:" << selectedData.toString();
                // 创建QImage对象并加载图片文件
                QImage image;
                image.load(selectedData.toString()); // 替换为您的图片路径

                if (image.isNull()) {
                    // 图片加载失败
                    QMessageBox::warning(this,"提示","打开失败");
                    return -1;
                }

                // 创建QPixmap对象并将QImage作为参数传入
                QPixmap pixmap = QPixmap::fromImage(image);
                label=new QLabel;
                label->setWindowTitle("照片显示");
                // 创建QLabel控件并设置Pixmap作为显示内容
                label->setPixmap(pixmap);

                // 调整标签的大小以适应图片大小
                label->resize(pixmap.size());

                // 显示标签
                label->show();
            }
        }
        return 0;
    });
}

MyDatabase::~MyDatabase()
{
    db.close();
    delete ui;
}

void MyDatabase::on_refreshBtn_clicked()
{
    quint32 count = 0;
    model->clear();
    model->setHorizontalHeaderItem(0,new QStandardItem("序号"));
    model->setHorizontalHeaderItem(1,new QStandardItem("车牌"));
    model->setHorizontalHeaderItem(2,new QStandardItem("进入时间"));
    model->setHorizontalHeaderItem(3,new QStandardItem("离开时间"));
    model->setHorizontalHeaderItem(4,new QStandardItem("进入时照片路径"));
    model->setHorizontalHeaderItem(5,new QStandardItem("离开时照片路径"));
    model->setHorizontalHeaderItem(6,new QStandardItem("费用"));
    QString sql = QString("select * from carInfo;");

    //执行sql
    QSqlQuery query;
    if(!query.exec(sql)){
        QMessageBox::information(this,"提示","查询数据失败!");
        return;
    }
    //获取查询结果
    while(query.next()){
        model->setItem(count,0,new QStandardItem(query.value(0).toString()));
        model->setItem(count,1,new QStandardItem(query.value(1).toString()));
        model->setItem(count,2,new QStandardItem(query.value(2).toString()));
        model->setItem(count,3,new QStandardItem(query.value(3).toString()));
        model->setItem(count,4,new QStandardItem(query.value(4).toString()));
        model->setItem(count,5,new QStandardItem(query.value(5).toString()));
        model->setItem(count,6,new QStandardItem(query.value(6).toString()));
        count++;
    }
}

void MyDatabase::on_insertBtn_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("添加内容");
    dialog->setFixedSize(400, 400); // 设置固定大小

    QLabel *carNumberLabel = new QLabel("车牌", dialog);
    QLineEdit *carNumberEdit = new QLineEdit(dialog);
    QHBoxLayout *lyh1=new QHBoxLayout;
    lyh1->addWidget(carNumberLabel);
    lyh1->addWidget(carNumberEdit);

    QLabel *entryTimeLabel = new QLabel("进入时间", dialog);
    QLineEdit *entryTimeEdit = new QLineEdit(dialog);
    QHBoxLayout *lyh2=new QHBoxLayout;
    lyh2->addWidget(entryTimeLabel);
    lyh2->addWidget(entryTimeEdit);

    QLabel *departTimeLabel = new QLabel("离开时间", dialog);
    QLineEdit *departTimeEdit = new QLineEdit(dialog);
    QHBoxLayout *lyh3=new QHBoxLayout;
    lyh3->addWidget(departTimeLabel);
    lyh3->addWidget(departTimeEdit);

    QPushButton *sureBtn = new QPushButton("确定",dialog);
    QPushButton *cancelBtn = new QPushButton("取消",dialog);
    QHBoxLayout *lyh4=new QHBoxLayout;
    lyh4->addWidget(sureBtn);
    lyh4->addWidget(cancelBtn);


    QVBoxLayout *lyt = new QVBoxLayout;

    lyt->addLayout(lyh1);
    lyt->addLayout(lyh2);
    lyt->addLayout(lyh3);
    lyt->addLayout(lyh4);
    dialog->setLayout(lyt);

    dialog->show();

    connect(sureBtn, &QPushButton::clicked, this,[=]() {
        QString sql = QString("insert into carInfo(carNumber,entryTime,departureTime) values('%1','%2','%3'); ")
                .arg(carNumberEdit->text())
                .arg(entryTimeEdit->text())
                .arg(departTimeEdit->text());

        //执行sql
        QSqlQuery query;
        if(query.exec(sql)){
            QMessageBox::information(this,"提示","插入成功!");
            return;
        }else{
            QMessageBox::information(this,"提示","插入失败!");
        }
        on_refreshBtn_clicked();
    });

    connect(cancelBtn, &QPushButton::clicked, this,[=]() {
        // 处理取消按钮点击
        dialog->close();
    });

}

void MyDatabase::on_changeBtn_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("修改内容");
    dialog->setFixedSize(500, 400); // 设置固定大小

    QLabel *idLabel = new QLabel("要修改数据的序号", dialog);
    QLineEdit *idEdit = new QLineEdit(dialog);
    QHBoxLayout *lyh=new QHBoxLayout;
    lyh->addWidget(idLabel);
    lyh->addWidget(idEdit);

    QLabel *carNumberLabel = new QLabel("将修改车牌名为", dialog);
    QLineEdit *carNumberEdit = new QLineEdit(dialog);
    QHBoxLayout *lyh1=new QHBoxLayout;
    lyh1->addWidget(carNumberLabel);
    lyh1->addWidget(carNumberEdit);

    QLabel *entryTimeLabel = new QLabel("将修改进入时间为", dialog);
    QLineEdit *entryTimeEdit = new QLineEdit(dialog);
    QHBoxLayout *lyh2=new QHBoxLayout;
    lyh2->addWidget(entryTimeLabel);
    lyh2->addWidget(entryTimeEdit);

    QLabel *departTimeLabel = new QLabel("将修改离开时间为", dialog);
    QLineEdit *departTimeEdit = new QLineEdit(dialog);
    QHBoxLayout *lyh3=new QHBoxLayout;
    lyh3->addWidget(departTimeLabel);
    lyh3->addWidget(departTimeEdit);

    QLabel *chargeLabel = new QLabel("修改费用为", dialog);
    QLineEdit *chargeEdit = new QLineEdit(dialog);
    QHBoxLayout *lyh4=new QHBoxLayout;
    lyh4->addWidget(chargeLabel);
    lyh4->addWidget(chargeEdit);

    QPushButton *sureBtn = new QPushButton("确定",dialog);
    QPushButton *cancelBtn = new QPushButton("取消",dialog);
    QHBoxLayout *lyh5=new QHBoxLayout;
    lyh5->addWidget(sureBtn);
    lyh5->addWidget(cancelBtn);


    QVBoxLayout *lyt = new QVBoxLayout;

    lyt->addLayout(lyh);
    lyt->addLayout(lyh1);
    lyt->addLayout(lyh2);
    lyt->addLayout(lyh3);
    lyt->addLayout(lyh4);
    lyt->addLayout(lyh5);
    dialog->setLayout(lyt);

    dialog->show();

    connect(sureBtn, &QPushButton::clicked, this,[=]() {
        QString sql = QString("update carInfo set carNumber = '%1',entryTime = '%2',departureTime = '%3',charge ='%4' where id =%5;")
                .arg(carNumberEdit->text())
                .arg(entryTimeEdit->text())
                .arg(departTimeEdit->text())
                .arg(chargeEdit->text())
                .arg(idEdit->text().toInt());

        //执行sql
        QSqlQuery query;
        if(!query.exec(sql)){
            QMessageBox::information(this,"提示","修改失败!");
            return;
        }else{
            QMessageBox::information(this,"提示","修改成功");
        }
        on_refreshBtn_clicked();
    });

    connect(cancelBtn, &QPushButton::clicked, this,[=]() {
        // 处理取消按钮点击
        dialog->close();
    });

}

void MyDatabase::on_deleteBtn_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("修改内容");
    dialog->setFixedSize(400, 400); // 设置固定大小

    QLabel *idLabel = new QLabel("要删除数据的序号", dialog);
    QLineEdit *idEdit = new QLineEdit(dialog);
    QHBoxLayout *lyh=new QHBoxLayout;
    lyh->addWidget(idLabel);
    lyh->addWidget(idEdit);


    QPushButton *sureBtn = new QPushButton("确定",dialog);
    QPushButton *cancelBtn = new QPushButton("取消",dialog);
    QHBoxLayout *lyh5=new QHBoxLayout;
    lyh5->addWidget(sureBtn);
    lyh5->addWidget(cancelBtn);

    QVBoxLayout *lyt = new QVBoxLayout;

    lyt->addLayout(lyh);
    lyt->addLayout(lyh5);


    dialog->setLayout(lyt);

    dialog->show();

    connect(sureBtn, &QPushButton::clicked, this,[=]() {
        QString sql = QString("delete from carInfo where id =%1;").arg(idEdit->text().toInt());

        //执行sql
        QSqlQuery query;
        if(!query.exec(sql)){
            QMessageBox::information(this,"提示","删除失败!");
            return;
        }else{
            QMessageBox::information(this,"提示","删除成功");
        }
        on_refreshBtn_clicked();
    });

    connect(cancelBtn, &QPushButton::clicked, this,[=]() {
        // 处理取消按钮点击
        dialog->close();
    });
}

void MyDatabase::on_selectBtn_clicked()
{
    QString selectCondition = ui->lineEdit->text();
    quint32 count = 0;

    QString sql = QString("select * from carInfo where carNumber = '%1';").arg(selectCondition);

    //执行sql
    QSqlQuery query;
    if(!query.exec(sql)){
        QMessageBox::information(this,"提示","查询数据失败!");
        return;
    }
    model->clear();
    //获取查询结果
    while(query.next()){
        model->setItem(count,0,new QStandardItem(query.value(0).toString()));
        model->setItem(count,1,new QStandardItem(query.value(1).toString()));
        model->setItem(count,2,new QStandardItem(query.value(2).toString()));
        model->setItem(count,3,new QStandardItem(query.value(3).toString()));
        model->setItem(count,4,new QStandardItem(query.value(4).toString()));
        model->setItem(count,5,new QStandardItem(query.value(5).toString()));
        model->setItem(count,6,new QStandardItem(query.value(6).toString()));
        count++;
    }
}

void MyDatabase::on_selectDateBtn_clicked()
{
    QString tempDate=ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");

    quint32 count = 0;

    QString sql = QString("select * from carInfo  where entryTime like '%1%';").arg(tempDate);

    //执行sql
    QSqlQuery query;
    if(!query.exec(sql)){
        QMessageBox::information(this,"提示","查询数据失败!");
        return;
    }
    model->clear();
    //获取查询结果
    while(query.next()){
        model->setItem(count,0,new QStandardItem(query.value(0).toString()));
        model->setItem(count,1,new QStandardItem(query.value(1).toString()));
        model->setItem(count,2,new QStandardItem(query.value(2).toString()));
        model->setItem(count,3,new QStandardItem(query.value(3).toString()));
        model->setItem(count,4,new QStandardItem(query.value(4).toString()));
        model->setItem(count,5,new QStandardItem(query.value(5).toString()));
        model->setItem(count,6,new QStandardItem(query.value(6).toString()));
        count++;
    }
}

void MyDatabase::on_selectTimeBtn_clicked()
{
    QDateTime startTime = ui->startTimeEdit->dateTime();
    QDateTime endTime = ui->endTimeEdit->dateTime();
    //qDebug()<<startTime;
    //qDebug()<<endTime;
    //qint64 seconds = startTime.secsTo(endTime); // 计算秒数差
    //qint64 minutes = seconds / 60;
    //qDebug()<<minutes;

    quint32 count = 0;

    QString sql = QString("select * from carInfo  where entryTime between '%1' and '%2';")
            .arg(startTime.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(endTime.toString("yyyy-MM-dd hh:mm:ss"));

    //执行sql
    QSqlQuery query;
    if(!query.exec(sql)){
        QMessageBox::information(this,"提示","查询数据失败!");
        return;
    }
    model->clear();
    //获取查询结果
    model->setHorizontalHeaderItem(0,new QStandardItem("序号"));
    model->setHorizontalHeaderItem(1,new QStandardItem("车牌"));
    model->setHorizontalHeaderItem(2,new QStandardItem("进入时间"));
    model->setHorizontalHeaderItem(3,new QStandardItem("离开时间"));
    model->setHorizontalHeaderItem(4,new QStandardItem("进入时照片路径"));
    model->setHorizontalHeaderItem(5,new QStandardItem("离开时照片路径"));
    model->setHorizontalHeaderItem(6,new QStandardItem("费用"));
    while(query.next()){
        model->setItem(count,0,new QStandardItem(query.value(0).toString()));
        model->setItem(count,1,new QStandardItem(query.value(1).toString()));
        model->setItem(count,2,new QStandardItem(query.value(2).toString()));
        model->setItem(count,3,new QStandardItem(query.value(3).toString()));
        model->setItem(count,4,new QStandardItem(query.value(4).toString()));
        model->setItem(count,5,new QStandardItem(query.value(5).toString()));
        model->setItem(count,6,new QStandardItem(query.value(6).toString()));
        count++;
    }
}
