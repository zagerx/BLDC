#ifndef PIDWINDOW_H
#define PIDWINDOW_H

#include <QWidget>

namespace Ui {
class pidwindow;
}

class pidwindow : public QWidget
{
    Q_OBJECT

public:
    explicit pidwindow(QWidget *parent = nullptr);
    ~pidwindow();

private:
    Ui::pidwindow *ui;
};

#endif // PIDWINDOW_H
