#include "objectdetect.h"
#include "ui_objectdetect.h"
#include "global.h"

objectDetect::objectDetect(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::objectDetect)
{
    ui->setupUi(this);
}

objectDetect::~objectDetect()
{
    delete ui;
}

void objectDetect::objectDetectMain(){

}
