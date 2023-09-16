#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableView>
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //添加数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("car.db");

    if(!db.open()){
        QMessageBox::warning(this,"提示",QString("打开数据库失败:%1").arg(db.lastError().text()));
        this->close();
    }
    //创建表格
    QString sql("create table if not exists user(id int unique,password ntext);");
    //执行sql语句
    QSqlQuery query;
    if(!query.exec(sql)){
        QMessageBox::warning(this,"提示",QString("建表失败:%1").arg(query.lastError().text()));
        this->close();
    }

    //初始化模型
    model = new QSqlTableModel(this);
    //绑定表格
    model->setTable("user");
    //设置手动提交
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QObject::connect(ui->passwordEdit,&QLineEdit::textChanged,ui->loginBtn,[=](){
        ui->loginBtn->setEnabled(true);
    });
}
Login::~Login()
{
    db.close();
    delete ui;
}

void Login::on_loginBtn_clicked()
{
    if(ui->passwordEdit->text()=="12345")
    {
        this->close();
        mydb = new MyDatabase;
        mydb->show();
    }
    else
    {
        QMessageBox::warning(this,"提示","验证失败");
    }
}

void Login::on_changeBtn_clicked()
{
    QWidget *w = new QWidget;
    w->resize(400,400);
    QTableView *tableView = new QTableView(this);


    //显示
    tableView->setModel(model);
    model->select();

    QPushButton *selectBtn = new QPushButton("增加",w);
    QPushButton *deleteBtn = new QPushButton("删除",w);
    QPushButton *sureBtn = new QPushButton("确定",w);
    QPushButton *cancelBtn = new QPushButton("取消",w);
    QPushButton *refreshBtn = new QPushButton("刷新",w);

    QHBoxLayout *lyh=new QHBoxLayout;
    lyh->addWidget(selectBtn);
    lyh->addWidget(deleteBtn);
    lyh->addWidget(sureBtn);
    lyh->addWidget(cancelBtn);
    lyh->addWidget(refreshBtn);
    QVBoxLayout *lyt = new QVBoxLayout;

    lyt->addWidget(tableView);
    lyt->addLayout(lyh);

    connect(selectBtn, &QPushButton::clicked, this,[=]() {
        qDebug()<<"增加";
        //获取当前行数
        int row = model->rowCount();
        qDebug()<<"row"<<row;
        model->insertRow(row);//最后一行插入

        int id = 1;
        model->setData(model->index(row,0),id);

        model->submitAll();
    });
    connect(deleteBtn, &QPushButton::clicked, this,[=]() {
        qDebug()<<"删除";
        //从表格上得到选中的行
        int delRow = tableView->currentIndex().row();
        qDebug()<<delRow;
        //删除改行
        model->removeRow(delRow);

        //再次询问是否删除
        if(QMessageBox::No==QMessageBox::information(w,"删除","确定删除本行吗?",QMessageBox::Yes|QMessageBox::No)){
            //撤销
            model->revertAll();
        }
        else{
            model->submitAll();
        }
    });
    connect(sureBtn, &QPushButton::clicked, this,[=]() {
        qDebug()<<"确定";
        model->submitAll();
    });
    connect(cancelBtn, &QPushButton::clicked, this,[=]() {
        qDebug()<<"取消";
        model->revertAll();
    });
    connect(refreshBtn, &QPushButton::clicked, this,[=]() {
        qDebug()<<"刷新";
        model->select();
    });
    w->setLayout(lyt);
    w->show();
}
