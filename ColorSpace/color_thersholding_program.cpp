#include "color_thersholding_program.h"
#include "ui_color_thersholding_program.h"
#include "global.h"

/*                                                                               Attention!!!
 * before use this program in debug mode, confirm that the opencv include and lib path in ./ColorSpace.pro is properly changed
 *
 * currently in author's edition, you need to change the path "/usr/local/opencv2" to your installation prefix (for example "/usr/local")
 *
 * if you get this program, please cha"nge this tips in order to tell later users which path to change
 *
 * current opencv path:"/usr/local/opencv2"
*/


/* ver-0.80a
 *
 * This is the very first version of the program
 *
 * the main function of this program is showVideo()
 *
 * I will try to apply PCA to thresholding parameter later
 * Later maybe forever... I will apply Hough transform to Y color space
 *
 * There also needs a Hough Transformation to find the football
 * Well... the fxxking Hough transformation and canny seems uable to work...
 *
 * The default value of Y color-space is the parameter I test,
 * please test in other conditions to get a more precis parameter.
 *
 * In addition also need to find some line, to constrain the robot's movement
 *
 * The blur parameter debugging will crash the program, so I let it sleep
 *
 * I will try to find the solution considering the circumstance
 * well, actually, the blurring I applied is already enough for using
 *
 * I will try to read the global program later to find out what parameter we need to output
 *
 * coding by Lciwxzr
*/




/* ver-0.90a
 * newFeature:
 * -add save color buttons
 * -delete some unused functions
 * If use other color table files
 * change the global variables(name of files ) right above
 *
 * Updated by Lciwxzr
*/


/* ver-0.90b
 *
 * -add test module to program
 * test module is used to test wether the table get is useful for our use
 *
 * However, I faced with a problem that all the data will be only saved in Y color spaces
 * I'm trying to debug it and the formal version will be released at that time : )
 *
 * file of table is stored at the txt file at the same directory
 *
 * usage:
 * first set the threshold at a reasonable value (depending on your usage)
 * second click the save_*(color) button to save the threshold to an array
 * if the output is not so good, click reset
 * if the color is good, click save file
 * to test other color, it's recommended to reset and test the new color alone, when thresholding completed, load file and save again
 *
 * the program seems to be a little weird because it's effect is only good when facing white color
 *
 * Updated by Lciwxzr
*/


/* ver-1.00a
 *
 * Changed the data structure of picture.
 * fix bug of only saving Y Spaces.
 *
 * enjoy the color filtering : )
 *
*/


/*ver-1.20a
 *
 * -add Pause function.
 * -add in-program camera change option.
 *
 *
 *
 * enjoy : )
 *
*/


/* ver-1.50
 * this version is possibly final version of this programme.
 * if there will be next version, It may be build with release build, which may be free from condition conflicts.
 *
 * -add color space choose option.
 * -add gray image in each chhannel to assist the thersholding.
 * -exception treated more properly
 *
 * the previous update log is formatted.
 *
*/
/* ver-1.50a
 * -bug that "show!" cannot be clicked when thresholding fixed
 *
*/

Color_Thersholding_Program::Color_Thersholding_Program(QWidget *parent) :
    QMainWindow(parent),
    ui_ct(new Ui::Color_Thersholding_Program)
{
//    color_table_file="ColourTable_YCrCb.txt";
    log_number=1;
    flag = -1 ;
    flag_selection = -1;
    timer = new QTimer();
    starter_flag = 0;
    reset_flag=0;
    // a timer, if computer's ability is limited, change it higher
//    timer->setInterval(1);
    ui_ct->setupUi(this);
    // start the main function timely
    connect(timer, SIGNAL(timeout()), this, SLOT(showVideo()));
    connect(ui_ct->Slider_Ch1_min, SIGNAL(valueChanged(int)), this, SLOT(setCh1BoxMin()));
    connect(ui_ct->Slider_Ch1_max, SIGNAL(valueChanged(int)), this, SLOT(setCh1BoxMax()));
    connect(ui_ct->Slider_Ch2_min, SIGNAL(valueChanged(int)), this, SLOT(setCh2BoxMin()));
    connect(ui_ct->Slider_Ch2_max, SIGNAL(valueChanged(int)), this, SLOT(setCh2BoxMax()));
    connect(ui_ct->Slider_Ch3_min, SIGNAL(valueChanged(int)), this, SLOT(setCh3BoxMin()));
    connect(ui_ct->Slider_Ch3_max, SIGNAL(valueChanged(int)), this, SLOT(setCh3BoxMax()));
    connect(ui_ct->Box_Ch1_min, SIGNAL(valueChanged(int)), this, SLOT(setCh1SliderMin()));
    connect(ui_ct->Box_Ch1_max, SIGNAL(valueChanged(int)), this, SLOT(setCh1SliderMax()));
    connect(ui_ct->Box_Ch2_min, SIGNAL(valueChanged(int)), this, SLOT(setCh2SliderMin()));
    connect(ui_ct->Box_Ch2_max, SIGNAL(valueChanged(int)), this, SLOT(setCh2SliderMax()));
    connect(ui_ct->Box_Ch3_min, SIGNAL(valueChanged(int)), this, SLOT(setCh3SliderMin()));
    connect(ui_ct->Box_Ch3_max, SIGNAL(valueChanged(int)), this, SLOT(setCh3SliderMax()));

    // the 12 functions is used for comfortable Slider and Box experience

    connect(ui_ct->button_loadFile, SIGNAL(clicked()), this, SLOT(loadTable()));
    connect(ui_ct->button_saveFile, SIGNAL(clicked()), this, SLOT(saveTable()));
    connect(ui_ct->button_resetTable, SIGNAL(clicked()), this, SLOT(reset_table()));
    connect(ui_ct->button_saveGreen, SIGNAL(clicked()), this, SLOT(save_green()));
    connect(ui_ct->button_saveWhite, SIGNAL(clicked()), this, SLOT(save_white()));
    connect(ui_ct->button_saveFigure, SIGNAL(clicked()), this, SLOT(save_figure()));
    connect(ui_ct->button_start, SIGNAL(clicked()), this, SLOT(start_color_filtering()));
    connect(ui_ct->button_pause, SIGNAL(clicked()), this, SLOT(pause_color_filtering()));

    // the 6 functions are for editing the table

    connect(ui_ct->changeto_YCrCb, SIGNAL(clicked()), this, SLOT(change_colorspace_YCrCb()));
    connect(ui_ct->changeto_HSV, SIGNAL(clicked()), this, SLOT(change_colorspace_HSV()));
    connect(ui_ct->changeto_BGR, SIGNAL(clicked()), this, SLOT(change_colorspace_BGR()));

    // the three function are for change the channel method

//    timer->start();
}
/*
// the program is used for thresholding YCrCb color space
// change parameter in cvtColor for other spaces
// variables' name can remain or... changed : )  so fxxking many names to change

// if not test with usb-connected camera, change it to 0
// CvCapture* capture = cvCaptureFromCAM(0);
// mostly the tips above will be useless in the version later than ver-1.50
*/


void Color_Thersholding_Program::showVideo(){


    Mat image_src = cvQueryFrame(capture);
    if(image_src.rows==0||image_src.cols==0){
        QMessageBox::critical(NULL, "Warning", "Not a valid camera! Please select another one!");
        log_content.sprintf("Warning : Not a valid camera! Please select another one!\n");
        ui_ct->notification->textCursor().insertText(log_content);
        qDebug("Warning : Not a valid camera! Please select another one!");
        ui_ct->notification->moveCursor(QTextCursor::End);
        pause_color_filtering();
        return;
    }
    starter_flag = 1;
    Mat image_origin(image_src);

//    seems like the blurring used here is non-sense
//    GaussianBlur(image_src, image_src, Size(5, 5), 0);
//    medianBlur(image_src, image_src, 3);
//    blur(image_src, image_src, Size(3,3), Point(-1,-1));


    // change the size of the picture to let it be fully displayed on QT
    // full-size image function will be out after I let a Scroll bar in
    // but it seems like a huge work ; )
    // so it can be long to complete it
    // or I will not complete it
    Size img_Size(320, 240);
    cv::resize(image_src, image_src, img_Size);
    cv::resize(image_origin, image_origin, img_Size);

    Mat image_ChA;
    Mat image_Ch1, image_Ch2, image_Ch3;
    if(flag!=-1){
        cvtColor(image_src, image_ChA, flag);
    }
    // If gonna find other color space number as reference, change the flag to enum ( CV_BGR2HSV for example )and ctrl into it's definition.
    else{
        image_ChA = image_src;
    }
//        CV_BGR2HSV     =40,
//        CV_RGB2HSV = 41
//        CV_BGR2YCrCb   =36,
//        CV_BGR2RGB     =4,
    vector<Mat> channels;
    split(image_ChA, channels);
    // the 0,1,2 means the channels in the uchar* data
    image_Ch1 = channels.at(0);
    image_Ch2 = channels.at(1);
    image_Ch3 = channels.at(2);
    ui_ct->video_Ch1_GRAY->setPixmap
            (QPixmap::fromImage(MAT2QIMAGE(image_Ch1)));
    ui_ct->video_Ch2_GRAY->setPixmap
            (QPixmap::fromImage(MAT2QIMAGE(image_Ch2)));
    ui_ct->video_Ch3_GRAY->setPixmap
            (QPixmap::fromImage(MAT2QIMAGE(image_Ch3)));
    // thresholding a single channel
    Mat result_Ch1 = recognize(image_Ch1, 0);
    Mat result_Ch2 = recognize(image_Ch2, 1);
    Mat result_Ch3 = recognize(image_Ch3, 2);

    Mat result_global;
    bitwise_and(result_Ch1, result_Ch2, result_global);
    bitwise_and(result_global, result_Ch3, result_global);
    Mat result_image_table = table_recognize(image_ChA);
//    GaussianBlur(result_global, result_global, Size(5, 5), 0, 0);
//    medianBlur(result_global, result_global, 5);

    // Show image in QT
    ui_ct->video_original->setPixmap
            (QPixmap::fromImage(MAT2QIMAGE(image_origin)));
    ui_ct->video_Ch1->setPixmap
            (QPixmap::fromImage(MAT2QIMAGE(result_Ch1)));
    ui_ct->video_Ch2->setPixmap
            (QPixmap::fromImage(MAT2QIMAGE(result_Ch2)));
    ui_ct->video_Ch3->setPixmap
            (QPixmap::fromImage(MAT2QIMAGE(result_Ch3)));
    ui_ct->video_global->setPixmap
            (QPixmap::fromImage(MAT2QIMAGE(result_global)));
    ui_ct->video_Table_Filtered->setPixmap
            (QPixmap::fromImage(MAT2QIMAGE(result_image_table)));

//    cvReleaseCapture(&capture);
}

// parameter label means the channel to be recognized
// The channels has already be splited
//This function is used for gray-scaled image's thresholding
Mat Color_Thersholding_Program::recognize(Mat src, int label){
    double min,max;
    switch (label){
        case 0 : {
        min = ui_ct->Box_Ch1_min->value()/4;
        max = ui_ct->Box_Ch1_max->value()/4;
        break;
    }
        case 1 : {
        min = ui_ct->Box_Ch2_min->value()/4;
        max = ui_ct->Box_Ch2_max->value()/4;
        break;
    }
        case 2 : {
        min = ui_ct->Box_Ch3_min->value()/4;
        max = ui_ct->Box_Ch3_max->value()/4;
        break;
    }
    }

    Mat result;
    result = src;
    // thresholding, the pixel value between min and max will be set to 255
    // otherwise 0
    inRange(src, min*4, max*4, result);
// change the methods of blurring, get different result!
    GaussianBlur(result, result, Size(5, 5), 0, 0);
    medianBlur(result, result, 5);
//    blur(result, result, Size(3,3), Point(-1,-1));


//    HoughCircles(result, circles, CV_HOUGH_GRADIENT, 1, result.rows/16, 100, 300, 10, 300);
//    for( size_t i = 0; i<circles.size(); i++ )
//        {
//             Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//             int radius = cvRound(circles[i][2]);
//             //绘制圆心
////             circle(result, center, 3, Scalar(0,255,0), -1, 8, 0 );
//             //绘制圆的边
//             circle(result, center, radius, Scalar(0,0,255), 3, 8, 0 );
//        }
//    return MAT2QIMAGE(result);
    return result;
}
// mat.at<uchar>(row, column, channel) = val;

// Green to 1, White to 2, Figure to 3
// The following three functions is used to save color threshold to array CT
void Color_Thersholding_Program::save_white(){
    //        CV_BGR2HSV     =40,
    //        CV_RGB2HSV = 41
    //        CV_BGR2YCrCb   =36,
    //        CV_BGR2RGB     =4,
    for(int ch1=ui_ct->Box_Ch1_min->value()/4; ch1<=ui_ct->Box_Ch1_max->value()/4; ch1++){
        for(int ch2=ui_ct->Box_Ch2_min->value()/4; ch2<=ui_ct->Box_Ch2_max->value()/4; ch2++){
            for(int ch3=ui_ct->Box_Ch3_min->value()/4; ch3<=ui_ct->Box_Ch3_max->value()/4; ch3++){
                CT[ch1][ch2][ch3] = 2;
            }
        }
    }
}

void Color_Thersholding_Program::save_green(){
    for(int ch1=ui_ct->Box_Ch1_min->value()/4; ch1<=ui_ct->Box_Ch1_max->value()/4; ch1++){
        for(int ch2=ui_ct->Box_Ch2_min->value()/4; ch2<=ui_ct->Box_Ch2_max->value()/4; ch2++){
            for(int ch3=ui_ct->Box_Ch3_min->value()/4; ch3<=ui_ct->Box_Ch3_max->value()/4; ch3++){
                CT[ch1][ch2][ch3] = 1;
            }
        }
    }
}

void Color_Thersholding_Program::save_figure(){
    for(int ch1=ui_ct->Box_Ch1_min->value()/4; ch1<=ui_ct->Box_Ch1_max->value()/4; ch1++){
        for(int ch2=ui_ct->Box_Ch2_min->value()/4; ch2<=ui_ct->Box_Ch2_max->value()/4; ch2++){
            for(int ch3=ui_ct->Box_Ch3_min->value()/4; ch3<=ui_ct->Box_Ch3_max->value()/4; ch3++){
                CT[ch1][ch2][ch3] = 3;
            }
        }
    }
}

void Color_Thersholding_Program::reset_table(){
    if(reset_flag==1)
    {
    // confirm the action
      switch( QMessageBox::warning(NULL, "Warning",QString::fromLocal8Bit("Confirming to RESET the Table?"),
                    QMessageBox::Yes | QMessageBox::Default,
                    QMessageBox::No | QMessageBox::Escape ))
      {
        case QMessageBox::Yes:
          break;
        case QMessageBox::No:
          return;
       }

    // this function is used to reset (zero) the array CT
    for(int i=0; i<64; i++){
        for(int j=0; j<64; j++){
            for(int k=0; k<64; k++)
                CT[i][j][k]=0;
        }
    }
     QMessageBox::about(NULL, "Notice", "Reset table successfully!");
    }
}
// Following two functions are used to save or load array CT from file, file is defined by the color space the program is
bool Color_Thersholding_Program::saveTable(){
    if(flag_selection==-1){
        QMessageBox::critical(NULL, "Error", "Color space not selected! Please select one!");
        log_content.sprintf("Warning : Color space not selected! Please select one!\n");
        ui_ct->notification->textCursor().insertText(log_content);
        qDebug("Warning : Color space not selected!");
        ui_ct->notification->moveCursor(QTextCursor::End);
    }
    else{
    {
    // confirm the action
      switch( QMessageBox::warning(NULL, "Warning",QString::fromLocal8Bit("Confirming to SAVE the Table?"),
                    QMessageBox::Yes | QMessageBox::Default,
                    QMessageBox::No | QMessageBox::Escape ))
      {
        case QMessageBox::Yes:
          break;
        case QMessageBox::No:
          return 0;
       }
    }
    FILE *file;
    if((file=fopen(color_table_file,"w"))==NULL)
    {
        QMessageBox::warning(NULL, "Warning", "Open the color_table_file failed!");
        log_content.sprintf("%d. Open the color_table_file failed.\n",log_number++);
        ui_ct->notification->textCursor().insertText(log_content);
        qDebug("Open the color_table_file failed.");
        ui_ct->notification->moveCursor(QTextCursor::End);
        return 0;
    }

        if((fwrite(CT,64*sizeof(int),64*64,file))==-1)
           {
               QMessageBox::warning(NULL, "Warning", "Write the file failed!");
               log_content.sprintf("%d. Write the file failed.\n",log_number++);
               ui_ct->notification->textCursor().insertText(log_content);
               qDebug("Write the file failed.");
               ui_ct->notification->moveCursor(QTextCursor::End);
               return 0;
           }
        else
            QMessageBox::about(NULL, "Notice", "Write the file succeed!");
        log_content.sprintf("%d. Write the file succeed.\n",log_number++);
        ui_ct->notification->textCursor().insertText(log_content);
        qDebug("Write the file succeed.");
        ui_ct->notification->moveCursor(QTextCursor::End);

    fclose(file);
    log_content.sprintf("%d. Color_table_file saved.\n",log_number++);
    ui_ct->notification->textCursor().insertText(log_content);
    qDebug("Color_table_file saved.");
    ui_ct->notification->moveCursor(QTextCursor::End);
    return 1;
    }
}

bool Color_Thersholding_Program::loadTable(){
    if(flag_selection==-1){
        QMessageBox::critical(NULL, "Error", "Color space not selected! Please select one!");
        log_content.sprintf("Warning : Color space not selected! Please select one!\n");
        ui_ct->notification->textCursor().insertText(log_content);
        qDebug("Warning : Color space not selected!");
        ui_ct->notification->moveCursor(QTextCursor::End);
    }
    else{
    {
    // confirm the action
      switch( QMessageBox::warning(NULL, "Warning",QString::fromLocal8Bit("Confirming to LOAD the Table?"),
                    QMessageBox::Yes | QMessageBox::Default,
                    QMessageBox::No | QMessageBox::Escape ))
      {
        case QMessageBox::Yes:
          break;
        case QMessageBox::No:
          return 0;
       }
    }
    FILE *file;
    if((file=fopen(color_table_file,"r"))==NULL)
    {
        QMessageBox::warning(NULL, "Warning", "Open the file failed!");
        log_content.sprintf("%d. Open the file failed.\n",log_number++);
        ui_ct->notification->textCursor().insertText(log_content);
        qDebug("Open the file failed.");
        ui_ct->notification->moveCursor(QTextCursor::End);
        return 0;
    }

    if((fread(CT,64*sizeof(int),64*64,file))==-1)
    {
        QMessageBox::warning(NULL, "Warning", "Read the file failed!");
        log_content.sprintf("%d. Read the file failed.\n",log_number++);
        ui_ct->notification->textCursor().insertText(log_content);
        qDebug("read the file failed.");
        ui_ct->notification->moveCursor(QTextCursor::End);
        return 0;
    }
    else
        QMessageBox::about(NULL, "Notice", "Read the file successfully!");
        log_content.sprintf("%d. Read the file successfully.\n",log_number++);
        ui_ct->notification->textCursor().insertText(log_content);
        qDebug("read the file successfully.");
        ui_ct->notification->moveCursor(QTextCursor::End);
        for(int x1=0;x1<64;x1++)
                   {
                   for(int y1=0;y1<64;y1++)
                       {
                       for(int z1=0;z1<64;z1++)
                       {
                       rewind(file);
    //		        					fp_position=ftell(fp_HSV);
    //		        					cout<<"重置指针位置"<<fp_position<<endl;
                       fseek(file,4*long(x1*64*64+y1*64+z1),0);
                           fread(&CT[x1][y1][z1],sizeof(int), 1,file) ;
                   //cout<<"colour="<<HSV[x][y][z]+0<<endl;
                       }
                       }
                   }
    fclose(file);
    return 1;
    }

}



Color_Thersholding_Program::~Color_Thersholding_Program()
{
    delete ui_ct;
}
// Function used for display image/video in QT
// It's copied, please don't change this code
QImage Color_Thersholding_Program::MAT2QIMAGE(Mat cvImg){
    QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {

        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }

    return qImg;
}
// ignore the functions below please >_<

void Color_Thersholding_Program::setCh1BoxMin(){
    ui_ct->Box_Ch1_min->setValue(ui_ct->Slider_Ch1_min->value());
}
void Color_Thersholding_Program::setCh1BoxMax(){
    ui_ct->Box_Ch1_max->setValue(ui_ct->Slider_Ch1_max->value());
}
void Color_Thersholding_Program::setCh2BoxMin(){
    ui_ct->Box_Ch2_min->setValue(ui_ct->Slider_Ch2_min->value());
}
void Color_Thersholding_Program::setCh2BoxMax(){
    ui_ct->Box_Ch2_max->setValue(ui_ct->Slider_Ch2_max->value());
}
void Color_Thersholding_Program::setCh3BoxMin(){
    ui_ct->Box_Ch3_min->setValue(ui_ct->Slider_Ch3_min->value());
}
void Color_Thersholding_Program::setCh3BoxMax(){
    ui_ct->Box_Ch3_max->setValue(ui_ct->Slider_Ch3_max->value());
}
void Color_Thersholding_Program::setCh1SliderMin(){
    ui_ct->Slider_Ch1_min->setValue(ui_ct->Box_Ch1_min->value());
}
void Color_Thersholding_Program::setCh1SliderMax(){
    ui_ct->Slider_Ch1_max->setValue(ui_ct->Box_Ch1_max->value());
}
void Color_Thersholding_Program::setCh2SliderMin(){
    ui_ct->Slider_Ch2_min->setValue(ui_ct->Box_Ch2_min->value());
}
void Color_Thersholding_Program::setCh2SliderMax(){
    ui_ct->Slider_Ch2_max->setValue(ui_ct->Box_Ch2_max->value());
}
void Color_Thersholding_Program::setCh3SliderMin(){
    ui_ct->Slider_Ch3_min->setValue(ui_ct->Box_Ch3_min->value());
}
void Color_Thersholding_Program::setCh3SliderMax(){
    ui_ct->Slider_Ch3_max->setValue(ui_ct->Box_Ch3_max->value());
}

// ignore complete, these slot functions are only used to link the scroller and the spinbox

void Color_Thersholding_Program::start_color_filtering(){
// this function is used to start the color color filtering
    if(flag_selection==-1){
        QMessageBox::critical(NULL, "Error", "Color space not selected! Please select one!");
        log_content.sprintf("Warning : Color space not selected! Please select one!\n");
        ui_ct->notification->textCursor().insertText(log_content);
        qDebug("Warning : Color space not selected!");
        ui_ct->notification->moveCursor(QTextCursor::End);
    }
    else{
//        cvReleaseCapture(&capture);
        if(starter_flag==1){
            cvReleaseCapture(&capture);
        }
        capture = cvCaptureFromCAM(ui_ct->spinbox_camera->value());
        timer->start(int(1000/ui_ct->spinbox_fps->value()));
        log_content.sprintf("Start!\n");
        ui_ct->notification->textCursor().insertText(log_content);
        qDebug("Start!");
        ui_ct->notification->moveCursor(QTextCursor::End);
    }
}
void Color_Thersholding_Program::pause_color_filtering(){
    timer->stop();
    cvReleaseCapture(&capture);
    starter_flag = 0;
    log_content.sprintf("Stop!\n");
    ui_ct->notification->textCursor().insertText(log_content);
    qDebug("Stop!");
}

void Color_Thersholding_Program::change_colorspace_YCrCb(){
    {
    switch( QMessageBox::question(NULL, "Notice",QString::fromLocal8Bit("Changing to YCrCb Color Space?"),
                  QMessageBox::Yes | QMessageBox::Default,
                  QMessageBox::No | QMessageBox::Escape ))
    {
      case QMessageBox::Yes:
        break;
      case QMessageBox::No:
        return;
     }
  }
    ui_ct->Title->setText("Color Filter Find ---YCrCb");
    ui_ct->label_Ch1->setText("Y");
    ui_ct->label_Ch2->setText("Cr");
    ui_ct->label_Ch3->setText("Cb");
    ui_ct->Color_1->setText("Y");
    ui_ct->Color_2->setText("Cr");
    ui_ct->Color_3->setText("Cb");
    ui_ct->video_Ch1->setText("video Y space");
    ui_ct->video_Ch1_GRAY->setText("video Y space (GRAY)");
    ui_ct->video_Ch2->setText("video Cr space");
    ui_ct->video_Ch2_GRAY->setText("video Cr space (GRAY)");
    ui_ct->video_Ch3->setText("video Cb space");
    ui_ct->video_Ch3_GRAY->setText("video Cb space (GRAY)");
    ui_ct->video_global->setText("Video transformed");
    ui_ct->video_Table_Filtered->setText("video table");
    ui_ct->video_original->setText("video origin");
    log_content.sprintf("%d. Change to YCrCb color space.\n",log_number++);
    ui_ct->notification->textCursor().insertText(log_content);
    qDebug("Change to YCrCb color space.");
    ui_ct->notification->moveCursor(QTextCursor::End);
    reset_table();
    color_table_file="../resources/ColourTable_YCrCb.txt";
    flag = 36 ;  // BGR2YCrCb
    flag_selection = 1;
    reset_flag=1;

}
void Color_Thersholding_Program::change_colorspace_HSV(){
    {
    switch( QMessageBox::question(NULL, "Notice",QString::fromLocal8Bit("Changing to HSV Color Space?"),
                  QMessageBox::Yes | QMessageBox::Default,
                  QMessageBox::No | QMessageBox::Escape ))
    {
      case QMessageBox::Yes:
        break;
      case QMessageBox::No:
        return;
     }
  }
    ui_ct->Title->setText("Color Filter Find --- HSV");
    ui_ct->label_Ch1->setText("H");
    ui_ct->label_Ch2->setText("S");
    ui_ct->label_Ch3->setText("V");
    ui_ct->Color_1->setText("H");
    ui_ct->Color_2->setText("S");
    ui_ct->Color_3->setText("V");
    ui_ct->video_Ch1->setText("video H space");
    ui_ct->video_Ch1_GRAY->setText("video H space (GRAY)");
    ui_ct->video_Ch2->setText("video S space");
    ui_ct->video_Ch2_GRAY->setText("video S space (GRAY)");
    ui_ct->video_Ch3->setText("video V space");
    ui_ct->video_Ch3_GRAY->setText("video V space (GRAY)");
    ui_ct->video_global->setText("Video transformed");
    ui_ct->video_Table_Filtered->setText("video table");
    ui_ct->video_original->setText("video origin");
    log_content.sprintf("%d. Change to HSV color space.\n",log_number++);
    ui_ct->notification->textCursor().insertText(log_content);
    qDebug("Change to HSV color space.");
    ui_ct->notification->moveCursor(QTextCursor::End);
    reset_table();
    color_table_file="../resources/ColourTable_HSV.txt";
//    flag = 40 ;  // BGR2HSV
    flag = 41; //RGB2HSV
    flag_selection = 1;
    reset_flag=1;

}
void Color_Thersholding_Program::change_colorspace_BGR(){
    {
    switch( QMessageBox::question(NULL, "Notice",QString::fromLocal8Bit("Changing to BGR Color Space?"),
                  QMessageBox::Yes | QMessageBox::Default,
                  QMessageBox::No | QMessageBox::Escape ))
    {
      case QMessageBox::Yes:
        break;
      case QMessageBox::No:
        return;
     }
  }
    ui_ct->Title->setText("Color Filter Find --- BGR");
    ui_ct->label_Ch1->setText("B");
    ui_ct->label_Ch2->setText("G");
    ui_ct->label_Ch3->setText("R");
    ui_ct->Color_1->setText("B");
    ui_ct->Color_2->setText("G");
    ui_ct->Color_3->setText("R");
    ui_ct->video_Ch1->setText("video B space");
    ui_ct->video_Ch1_GRAY->setText("video B space (GRAY)");
    ui_ct->video_Ch2->setText("video G space");
    ui_ct->video_Ch2_GRAY->setText("video G space (GRAY)");
    ui_ct->video_Ch3->setText("video R space");
    ui_ct->video_Ch3_GRAY->setText("video R space (GRAY)");
    ui_ct->video_global->setText("Video transformed");
    ui_ct->video_Table_Filtered->setText("video table");
    ui_ct->video_original->setText("video origin");
    log_content.sprintf("%d. Change to BGR color space.\n",log_number++);
    ui_ct->notification->textCursor().insertText(log_content);
    qDebug("Change to BGR color space.");
    ui_ct->notification->moveCursor(QTextCursor::End);
    reset_table();
    color_table_file="../resources/ColourTable_RGB.txt";
    flag = -1 ;  // Not converting Color space
    flag_selection = 1;
    reset_flag=1;
}


void Color_Thersholding_Program::closeEvent(QCloseEvent *event){
    int choose;
    choose= QMessageBox::question(this, tr("Quit?"),
                                   QString(tr("Confirm to quit?")),
                                   QMessageBox::Yes | QMessageBox::No);

    if (choose== QMessageBox::No){
          event->ignore();
    }
    else if (choose== QMessageBox::Yes){
          event->accept();
          pause_color_filtering();
    }
}
