/********************************************************************************
** Form generated from reading UI file 'pidwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PIDWINDOW_H
#define UI_PIDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_pidwindow
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *pidwindow)
    {
        if (pidwindow->objectName().isEmpty())
            pidwindow->setObjectName("pidwindow");
        pidwindow->resize(400, 300);
        pushButton = new QPushButton(pidwindow);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(60, 110, 80, 18));

        retranslateUi(pidwindow);

        QMetaObject::connectSlotsByName(pidwindow);
    } // setupUi

    void retranslateUi(QWidget *pidwindow)
    {
        pidwindow->setWindowTitle(QCoreApplication::translate("pidwindow", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("pidwindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class pidwindow: public Ui_pidwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PIDWINDOW_H
