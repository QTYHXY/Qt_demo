#include "myproject.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MyProject w;
    w.show();
    return a.exec();
}
