
#include "globalfunction.h"

int CT[64][64][64];

Mat table_recognize(Mat input){

    Mat result(input);
     int rows,cols;
     rows=input.rows;
     cols=input.cols;
     int Ch1,Ch2,Ch3;
     for(int i=0;i<rows;i++)
     {
         for(int j=0;j<cols;j++)
         {
             Ch1=input.at<Vec3b>(i, j)[0];
             Ch2=input.at<Vec3b>(i, j)[1];
             Ch3=input.at<Vec3b>(i, j)[2];
             Ch1=Ch1/4;
             Ch2=Ch2/4;
             Ch3=Ch3/4;
             if(CT[Ch1][Ch2][Ch3]==1)
             {
                 result.at<Vec3b>(i, j) = Vec3b(0,255,0);
             }
             else if(CT[Ch1][Ch2][Ch3]==2)
             {
                 result.at<Vec3b>(i, j) = Vec3b(255,255,255);
             }
             else if(CT[Ch1][Ch2][Ch3]==3)
             {
                 result.at<Vec3b>(i, j) = Vec3b(255,0,0);
             }
             else
             {
                 result.at<Vec3b>(i, j) = Vec3b(0,0,0);
             }
         }
     }
     GaussianBlur(result, result, Size(5, 5), 0, 0);
     medianBlur(result, result, 5);
//     Mat element_10_rect = getStructuringElement(MORPH_RECT, Size(10, 10));
//     Mat element_5_rect = getStructuringElement(MORPH_RECT, Size(5, 5));
//     Mat element_10_ellipse = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
//     Mat element_5_ellipse = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
//     erode(result,result,element_5_rect);
//     dilate(result, result, element_10_rect);
//     erode(result,result,element_5_rect);
//     dilate(result, result, element_10_rect);
//     dilate(result, result, element_10_ellipse);
//     dilate(result, result, element_5_ellipse);
//     dilate(result, result, element_10_ellipse);
//     erode(result, result, element_5_ellipse);
//     morphologyEx(result, result, MORPH_OPEN, element_5_rect);

   return result;
}
