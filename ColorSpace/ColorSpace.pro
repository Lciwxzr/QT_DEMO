
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorSpace
TEMPLATE = app


SOURCES += main.cpp \
    color_thersholding_program.cpp \
    selectfunction.cpp \
    objectdetect.cpp \
    globalfunction.cpp


HEADERS  += \
    global.h \
    color_thersholding_program.h \
    selectfunction.h \
    objectdetect.h \
    globalfunction.h

FORMS    += \
    color_thersholding_program.ui \
    selectfunction.ui \
    objectdetect.ui

INCLUDEPATH += /usr/local/opencv2/include/opencv \
/usr/local/opencv2/include/opencv2 \
/usr/local/opencv2/include

CONFIG(debug, debug|release): {
LIBS += -L/usr/local/opencv2/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
#-lopencv_ml \
#-lopencv_video \
#-lopencv_features2d \
#-lopencv_calib3d \
#-lopencv_objdetect \
#-lopencv_contrib \
#-lopencv_legacy \
#-lopencv_flann
}

#CONFIG(release, debug|release): {
#LIBS += -L/usr/local/opencv2/lib \
#-lopencv_core \
#-lopencv_imgproc \
#-lopencv_highgui \
#}
