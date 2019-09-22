#include "colorfilter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Colorfilter w;
    w.show();

    return a.exec();
}
