#ifndef SELECTFUNCTION_H
#define SELECTFUNCTION_H

#include <QMainWindow>
#include "color_thersholding_program.h"
#include "objectdetect.h"

namespace Ui {
class selectFunction;
}

class selectFunction : public QMainWindow
{
    Q_OBJECT

public:
    explicit selectFunction(QWidget *parent = 0);
    ~selectFunction();

public slots:
    void start_CT();
    void start_OF();

private:
    Ui::selectFunction *ui_sf;
    Color_Thersholding_Program* CT;
    objectDetect* OD;
};

#endif // SELECTFUNCTION_H
