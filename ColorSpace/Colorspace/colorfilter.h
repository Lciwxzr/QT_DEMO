#ifndef COLORFILTER_H
#define COLORFILTER_H

#include <QMainWindow>

namespace Ui {
class Colorfilter;
}

class Colorfilter : public QMainWindow
{
    Q_OBJECT

public:
    explicit Colorfilter(QWidget *parent = nullptr);
    ~Colorfilter();

private:
    Ui::Colorfilter *ui;
};

#endif // COLORFILTER_H
