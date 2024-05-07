/********************************************************************************
** Form generated from reading UI file 'serialwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIALWINDOW_H
#define UI_SERIALWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_serialwindow
{
public:
    QComboBox *comboBox;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QComboBox *portBox;
    QPushButton *mt_stopBt;
    QPushButton *mc_startBt;
    QPushButton *enter_bt;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *Kp_lineEdit;
    QLineEdit *Ki_lineEdit;
    QLineEdit *Kc_lineEdit;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *mc_pidset;
    QLineEdit *Speed_lineEdit;
    QLabel *label_6;
    QPushButton *mc_speed_Bt;
    QLabel *label_7;
    QLineEdit *Speed_lineEdit_2;
    QLabel *label_8;
    QLineEdit *Speed_lineEdit_3;
    QPushButton *mc_speed_Bt_2;
    QLabel *LED_Label;
    QLineEdit *id_lineEdit;
    QLineEdit *iq_lineEdit;
    QLabel *label_5;
    QLabel *label_4;

    void setupUi(QWidget *serialwindow)
    {
        if (serialwindow->objectName().isEmpty())
            serialwindow->setObjectName("serialwindow");
        serialwindow->resize(624, 404);
        comboBox = new QComboBox(serialwindow);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(300, 10, 71, 21));
        pushButton = new QPushButton(serialwindow);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(300, 70, 75, 23));
        textEdit = new QTextEdit(serialwindow);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(0, 0, 281, 141));
        portBox = new QComboBox(serialwindow);
        portBox->setObjectName("portBox");
        portBox->setGeometry(QRect(300, 40, 71, 21));
        mt_stopBt = new QPushButton(serialwindow);
        mt_stopBt->setObjectName("mt_stopBt");
        mt_stopBt->setGeometry(QRect(490, 340, 80, 18));
        mc_startBt = new QPushButton(serialwindow);
        mc_startBt->setObjectName("mc_startBt");
        mc_startBt->setGeometry(QRect(390, 340, 80, 18));
        enter_bt = new QPushButton(serialwindow);
        enter_bt->setObjectName("enter_bt");
        enter_bt->setGeometry(QRect(230, 170, 51, 21));
        layoutWidget = new QWidget(serialwindow);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(150, 210, 61, 92));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Kp_lineEdit = new QLineEdit(layoutWidget);
        Kp_lineEdit->setObjectName("Kp_lineEdit");

        verticalLayout->addWidget(Kp_lineEdit);

        Ki_lineEdit = new QLineEdit(layoutWidget);
        Ki_lineEdit->setObjectName("Ki_lineEdit");

        verticalLayout->addWidget(Ki_lineEdit);

        Kc_lineEdit = new QLineEdit(layoutWidget);
        Kc_lineEdit->setObjectName("Kc_lineEdit");

        verticalLayout->addWidget(Kc_lineEdit);

        layoutWidget1 = new QWidget(serialwindow);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(100, 210, 51, 91));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget1);
        label->setObjectName("label");

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);

        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName("label_3");

        verticalLayout_2->addWidget(label_3);

        mc_pidset = new QPushButton(serialwindow);
        mc_pidset->setObjectName("mc_pidset");
        mc_pidset->setGeometry(QRect(230, 210, 61, 31));
        Speed_lineEdit = new QLineEdit(serialwindow);
        Speed_lineEdit->setObjectName("Speed_lineEdit");
        Speed_lineEdit->setGeometry(QRect(400, 170, 51, 21));
        label_6 = new QLabel(serialwindow);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(358, 170, 41, 21));
        mc_speed_Bt = new QPushButton(serialwindow);
        mc_speed_Bt->setObjectName("mc_speed_Bt");
        mc_speed_Bt->setGeometry(QRect(470, 170, 41, 21));
        label_7 = new QLabel(serialwindow);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(360, 220, 41, 21));
        Speed_lineEdit_2 = new QLineEdit(serialwindow);
        Speed_lineEdit_2->setObjectName("Speed_lineEdit_2");
        Speed_lineEdit_2->setGeometry(QRect(400, 220, 51, 21));
        label_8 = new QLabel(serialwindow);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(360, 250, 41, 21));
        Speed_lineEdit_3 = new QLineEdit(serialwindow);
        Speed_lineEdit_3->setObjectName("Speed_lineEdit_3");
        Speed_lineEdit_3->setGeometry(QRect(400, 250, 51, 21));
        mc_speed_Bt_2 = new QPushButton(serialwindow);
        mc_speed_Bt_2->setObjectName("mc_speed_Bt_2");
        mc_speed_Bt_2->setGeometry(QRect(470, 250, 41, 21));
        LED_Label = new QLabel(serialwindow);
        LED_Label->setObjectName("LED_Label");
        LED_Label->setGeometry(QRect(300, 330, 51, 31));
        id_lineEdit = new QLineEdit(serialwindow);
        id_lineEdit->setObjectName("id_lineEdit");
        id_lineEdit->setGeometry(QRect(100, 170, 59, 22));
        iq_lineEdit = new QLineEdit(serialwindow);
        iq_lineEdit->setObjectName("iq_lineEdit");
        iq_lineEdit->setGeometry(QRect(160, 170, 59, 22));
        label_5 = new QLabel(serialwindow);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(110, 150, 29, 22));
        label_4 = new QLabel(serialwindow);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(170, 150, 29, 29));

        retranslateUi(serialwindow);

        QMetaObject::connectSlotsByName(serialwindow);
    } // setupUi

    void retranslateUi(QWidget *serialwindow)
    {
        serialwindow->setWindowTitle(QCoreApplication::translate("serialwindow", "\344\270\262\345\217\243\350\260\203\350\257\225\347\225\214\351\235\242", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("serialwindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("serialwindow", "9600", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("serialwindow", "115200", nullptr));

        pushButton->setText(QCoreApplication::translate("serialwindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        mt_stopBt->setText(QCoreApplication::translate("serialwindow", "\347\224\265\346\234\272\345\201\234\346\255\242", nullptr));
        mc_startBt->setText(QCoreApplication::translate("serialwindow", "\347\224\265\346\234\272\345\220\257\345\212\250", nullptr));
        enter_bt->setText(QCoreApplication::translate("serialwindow", "\347\241\256\350\256\244", nullptr));
        label->setText(QCoreApplication::translate("serialwindow", "\347\224\265\346\265\201\347\216\257Kp", nullptr));
        label_2->setText(QCoreApplication::translate("serialwindow", "\347\224\265\346\265\201\347\216\257Ki", nullptr));
        label_3->setText(QCoreApplication::translate("serialwindow", "\347\224\265\346\265\201\347\216\257Kc", nullptr));
        mc_pidset->setText(QCoreApplication::translate("serialwindow", "PID\345\217\202\346\225\260\347\241\256\350\256\244", nullptr));
        label_6->setText(QCoreApplication::translate("serialwindow", "\347\233\256\346\240\207\351\200\237\345\272\246", nullptr));
        mc_speed_Bt->setText(QCoreApplication::translate("serialwindow", "\347\241\256\350\256\244", nullptr));
        label_7->setText(QCoreApplication::translate("serialwindow", "\351\200\237\345\272\246Kp", nullptr));
        label_8->setText(QCoreApplication::translate("serialwindow", "\351\200\237\345\272\246Ki", nullptr));
        mc_speed_Bt_2->setText(QCoreApplication::translate("serialwindow", "\347\241\256\350\256\244", nullptr));
        LED_Label->setText(QCoreApplication::translate("serialwindow", "TextLabel", nullptr));
        label_5->setText(QCoreApplication::translate("serialwindow", "tar id", nullptr));
        label_4->setText(QCoreApplication::translate("serialwindow", "tar iq", nullptr));
    } // retranslateUi

};

namespace Ui {
    class serialwindow: public Ui_serialwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALWINDOW_H
