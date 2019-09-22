#include "colorfilter.h"
#include "ui_colorfilter.h"

Colorfilter::Colorfilter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Colorfilter)
{
    ui->setupUi(this);
}

Colorfilter::~Colorfilter()
{
    delete ui;
}
