#include "texttospeech.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextToSpeech w;
    w.show();
    return a.exec();
}
