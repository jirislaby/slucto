#include "slucto.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SlUcto w;
    w.show();

    return a.exec();
}
