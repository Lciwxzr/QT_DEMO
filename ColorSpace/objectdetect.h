#ifndef OBJECTDETECT_H
#define OBJECTDETECT_H

#include <QMainWindow>

namespace Ui {
class objectDetect;
}

class objectDetect : public QMainWindow
{
    Q_OBJECT

public:
    explicit objectDetect(QWidget *parent = 0);
    void objectDetectMain();
    ~objectDetect();

private:
    Ui::objectDetect *ui;
};

#endif // OBJECTDETECT_H
