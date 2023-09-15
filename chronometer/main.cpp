#include "chronometer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Chronometer w;
    w.show();
    return a.exec();
}
