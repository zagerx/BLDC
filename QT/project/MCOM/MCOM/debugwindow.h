#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H
#include <QWidget>



namespace Ui {
class motordebug;
}

class motordebug : public QWidget
{
    Q_OBJECT

public:
    explicit motordebug(QWidget *parent = nullptr);
    ~motordebug();

private:
    Ui::motordebug *ui;
protected:
void closeEvent(QCloseEvent *event) override;
signals:
    void dataReady(const QString &data); // 当数据准备好时发射的信号
private slots:
    void on_param_enter_bt_clicked();
    void on_start_bt_clicked();
    void on_stop_bt_clicked();
};

#endif // DEBUGWINDOW_H
