#include "color_thersholding_program.h"
#include "selectfunction.h"
#include <QApplication>
#include "global.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    selectFunction hw;
    hw.show();

    return a.exec();
}
