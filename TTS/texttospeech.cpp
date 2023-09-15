#include "texttospeech.h"
#include "ui_texttospeech.h"
#include <QDebug>
TextToSpeech::TextToSpeech(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextToSpeech)
{
    ui->setupUi(this);
    //tts = new QTextToSpeech(this);
    QStringList engines= QTextToSpeech::availableEngines();//可用引擎
    qDebug()<<engines;
    tts=new QTextToSpeech(engines[0], this);//构造QTextToSpeech对象需要传入引擎

}

TextToSpeech::~TextToSpeech()
{
    delete ui;
}


void TextToSpeech::on_pushButton_clicked()
{
    QString str = ui->textEdit->toPlainText();
    if(!str.isEmpty()){
        tts->say(str);
    }
}
