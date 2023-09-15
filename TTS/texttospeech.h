#ifndef TEXTTOSPEECH_H
#define TEXTTOSPEECH_H

#include <QWidget>
#include <QTextToSpeech>
QT_BEGIN_NAMESPACE
namespace Ui { class TextToSpeech; }
QT_END_NAMESPACE

class TextToSpeech : public QWidget
{
    Q_OBJECT

public:
    TextToSpeech(QWidget *parent = nullptr);
    ~TextToSpeech();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TextToSpeech *ui;
    QTextToSpeech *tts;
};
#endif // TEXTTOSPEECH_H
