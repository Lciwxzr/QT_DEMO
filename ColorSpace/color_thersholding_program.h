#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QtWidgets/QSpinBox>
#include <QMessageBox>
#include "global.h"
#include <QCloseEvent>
#include <QVBoxLayout>


namespace Ui {
class Color_Thersholding_Program;
}

class Color_Thersholding_Program: public QMainWindow
{
    Q_OBJECT

public:
    explicit Color_Thersholding_Program(QWidget *parent = 0);
    Mat captureCam();
    QImage MAT2QIMAGE(Mat);
    Mat recognize(Mat, int);
    void closeEvent(QCloseEvent *event);
//    void recognize(const Mat&, int);



        ~Color_Thersholding_Program();
public slots:
    void showVideo();
    void setCh1BoxMin();
    void setCh1BoxMax();
    void setCh2BoxMin();
    void setCh2BoxMax();
    void setCh3BoxMin();
    void setCh3BoxMax();
    void setCh1SliderMin();
    void setCh1SliderMax();
    void setCh2SliderMin();
    void setCh2SliderMax();
    void setCh3SliderMin();
    void setCh3SliderMax();
    void save_white();
    void save_green();
    void save_figure();
    void reset_table();
    void start_color_filtering();
    void pause_color_filtering();
    void change_colorspace_YCrCb();
    void change_colorspace_HSV();
    void change_colorspace_BGR();

    bool saveTable();
    bool loadTable();






private:
    Ui::Color_Thersholding_Program *ui_ct;
    QTimer *timer;
    CvCapture* capture;
    int flag, flag_selection;
    char *color_table_file;
    QString log_content;
    int log_number;
    int starter_flag;
    int reset_flag;
};

#endif // MAINWINDOW_H
