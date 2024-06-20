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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_serialwindow
{
public:
    QComboBox *comboBox;
    QPushButton *enseriBt;
    QTextEdit *textEdit;
    QComboBox *portBox;
    QPushButton *mt_stopBt;
    QPushButton *mc_startBt;
    QLabel *LED_Label;
    QPushButton *debug_bt;
    QPushButton *normal_bt;

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
        enseriBt = new QPushButton(serialwindow);
        enseriBt->setObjectName("enseriBt");
        enseriBt->setGeometry(QRect(300, 70, 75, 23));
        textEdit = new QTextEdit(serialwindow);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(0, 0, 281, 141));
        portBox = new QComboBox(serialwindow);
        portBox->setObjectName("portBox");
        portBox->setGeometry(QRect(300, 40, 71, 21));
        mt_stopBt = new QPushButton(serialwindow);
        mt_stopBt->setObjectName("mt_stopBt");
        mt_stopBt->setGeometry(QRect(400, 220, 80, 18));
        mc_startBt = new QPushButton(serialwindow);
        mc_startBt->setObjectName("mc_startBt");
        mc_startBt->setGeometry(QRect(300, 220, 80, 18));
        LED_Label = new QLabel(serialwindow);
        LED_Label->setObjectName("LED_Label");
        LED_Label->setGeometry(QRect(250, 280, 51, 31));
        debug_bt = new QPushButton(serialwindow);
        debug_bt->setObjectName("debug_bt");
        debug_bt->setGeometry(QRect(150, 170, 91, 21));
        normal_bt = new QPushButton(serialwindow);
        normal_bt->setObjectName("normal_bt");
        normal_bt->setGeometry(QRect(340, 170, 91, 21));

        retranslateUi(serialwindow);

        QMetaObject::connectSlotsByName(serialwindow);
    } // setupUi

    void retranslateUi(QWidget *serialwindow)
    {
        serialwindow->setWindowTitle(QCoreApplication::translate("serialwindow", "\344\270\262\345\217\243\350\260\203\350\257\225\347\225\214\351\235\242", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("serialwindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("serialwindow", "9600", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("serialwindow", "115200", nullptr));

        enseriBt->setText(QCoreApplication::translate("serialwindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        mt_stopBt->setText(QCoreApplication::translate("serialwindow", "\347\224\265\346\234\272\345\201\234\346\255\242", nullptr));
        mc_startBt->setText(QCoreApplication::translate("serialwindow", "\347\224\265\346\234\272\345\220\257\345\212\250", nullptr));
        LED_Label->setText(QCoreApplication::translate("serialwindow", "TextLabel", nullptr));
        debug_bt->setText(QCoreApplication::translate("serialwindow", "DEBUG_Mode", nullptr));
        normal_bt->setText(QCoreApplication::translate("serialwindow", "Normal_Mode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class serialwindow: public Ui_serialwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALWINDOW_H
