#include "selectfunction.h"
#include "ui_selectfunction.h"


selectFunction::selectFunction(QWidget *parent) :
    QMainWindow(parent),
    ui_sf(new Ui::selectFunction)
{
    ui_sf->setupUi(this);
    connect(ui_sf->Start_CT, SIGNAL(clicked()), this, SLOT(start_CT()));
    connect(ui_sf->Start_OF, SIGNAL(clicked()), this, SLOT(start_OF()));
}

selectFunction::~selectFunction()
{
    delete ui_sf;
}


void selectFunction::start_CT(){
if ( CT==NULL ){
    CT = new Color_Thersholding_Program;
}
CT->show();
}
void selectFunction::start_OF(){
if(OD==NULL){
    OD = new objectDetect;
}
OD->show();
}

