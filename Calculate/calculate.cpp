#include "calculate.h"
#include "ui_calculate.h"

Calculate::Calculate(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Calculate)
{
    ui->setupUi(this);
    //初始化自定义成员
    num = temp = 0;
    op = oper_null;
    flag = true;
}

Calculate::~Calculate()
{
    delete ui;
}


void Calculate::on_pushButton_1_clicked()
{
    if(!flag){
        num = 0;
        flag = true;
    }

    if(num<100000000)
        num = num*10+1;
    //显示
    ui->lcdNumber->display(num);
}

void Calculate::on_pushButton_2_clicked()
{
    if(!flag){
        num = 0;
        flag = true;
    }

    if(num<100000000)
        num = num*10+2;

    //显示
    ui->lcdNumber->display(num);
}

void Calculate::on_pushButton_3_clicked()
{
    if(!flag){
        num = 0;
        flag = true;
    }

    if(num<100000000)
        num = num*10+3;
    //显示
    ui->lcdNumber->display(num);
}

void Calculate::on_pushButton_4_clicked()
{
    if(!flag){
        num = 0;
        flag = true;
    }

    if(num<100000000)
        num = num*10+4;
    //显示
    ui->lcdNumber->display(num);
}

void Calculate::on_pushButton_5_clicked()
{
    if(!flag){
        num = 0;
        flag = true;
    }

    if(num<100000000)
        num = num*10+5;
    //显示
    ui->lcdNumber->display(num);
}

void Calculate::on_pushButton_6_clicked()
{
    if(!flag){
        num = 0;
        flag = true;
    }

    if(num<100000000)
        num = num*10+6;
    //显示
    ui->lcdNumber->display(num);
}

void Calculate::on_pushButton_7_clicked()
{
    if(!flag){
        num = 0;
        flag = true;
    }

    if(num<100000000)
        num = num*10+7;
    //显示
    ui->lcdNumber->display(num);
}

void Calculate::on_pushButton_8_clicked()
{
    if(!flag){
        num = 0;
        flag = true;
    }

    if(num<100000000)
        num = num*10+8;
    //显示
    ui->lcdNumber->display(num);
}

void Calculate::on_pushButton_9_clicked()
{
    if(!flag){
        num = 0;
        flag = true;
    }

    if(num<100000000)
        num = num*10+9;
    //显示
    ui->lcdNumber->display(num);
}

void Calculate::on_pushButton_0_clicked()
{
    if(!flag){
        num = 0;
        flag = true;
    }

    if(num<100000000)
        num = num*10;
    //显示
    ui->lcdNumber->display(num);
}

//清零
void Calculate::on_pushButton_clear_clicked()
{
    num = temp = 0;
    op = oper_null;

    ui->lcdNumber->display(num);
}

//=
void Calculate::on_pushButton_equal_clicked()
{
    switch (op) {
        case oper_add:
            num = temp+num;
            break;
        case oper_sub:
            num = temp-num;
            break;
        case oper_mul:
            num = temp*num;
            break;
        case oper_div:
            if(num==0){
                num = 0;
                temp = 0;
                op = oper_null;
                ui->lcdNumber->display("Err");
                return;
            }
            num = temp/num;
            break;
        default:
            return;
    }

    //第一操作数清0
    temp = 0;
    //运算符清空
    op = oper_null;
    //标志
    flag = false;

    if(num>999999999||num<-99999999){
        ui->lcdNumber->display("Err");
        num = 0;
    }
    else{
        //显示
        ui->lcdNumber->display(num);
    }
}

//+
void Calculate::on_pushButton_add_clicked()
{
    //如果前面输入过运算符
    if(op!=oper_null){
        //计算
        on_pushButton_equal_clicked();
    }

    //记录加法
    op = oper_add;
    //把num放入第一操作数
    temp = num;
    //num清0
    num = 0;
}

void Calculate::on_pushButton_sub_clicked()
{
    //如果前面输入过运算符
    if(op!=oper_null){
        //计算
        on_pushButton_equal_clicked();
    }

    //记录减法
    op = oper_sub;
    //把num放入第一操作数
    temp = num;
    //num清0
    num = 0;
}

void Calculate::on_pushButton_mul_clicked()
{
    //如果前面输入过运算符
    if(op!=oper_null){
        //计算
        on_pushButton_equal_clicked();
    }

    //记录乘法
    op = oper_mul;
    //把num放入第一操作数
    temp = num;
    //num清0
    num = 0;
}

void Calculate::on_pushButton_div_clicked()
{
    //如果前面输入过运算符
    if(op!=oper_null){
        //计算
        on_pushButton_equal_clicked();
    }

    //记录除法
    op = oper_div;
    //把num放入第一操作数
    temp = num;
    //num清0
    num = 0;
}
