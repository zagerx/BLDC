/********************************************************************************
** Form generated from reading UI file 'motordebug.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOTORDEBUG_H
#define UI_MOTORDEBUG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_motordebug
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *Kp_lineEdit;
    QLineEdit *Ki_lineEdit;
    QLineEdit *Kc_lineEdit;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *paramenterBT;
    QPushButton *start_bt;
    QLineEdit *tarlineEdit;
    QLabel *label_5;
    QPushButton *stop_bt;

    void setupUi(QWidget *motordebug)
    {
        if (motordebug->objectName().isEmpty())
            motordebug->setObjectName("motordebug");
        motordebug->resize(654, 350);
        layoutWidget = new QWidget(motordebug);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(70, 80, 61, 92));
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

        layoutWidget_2 = new QWidget(motordebug);
        layoutWidget_2->setObjectName("layoutWidget_2");
        layoutWidget_2->setGeometry(QRect(20, 80, 51, 91));
        verticalLayout_2 = new QVBoxLayout(layoutWidget_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget_2);
        label->setObjectName("label");

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(layoutWidget_2);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);

        label_3 = new QLabel(layoutWidget_2);
        label_3->setObjectName("label_3");

        verticalLayout_2->addWidget(label_3);

        paramenterBT = new QPushButton(motordebug);
        paramenterBT->setObjectName("paramenterBT");
        paramenterBT->setGeometry(QRect(150, 80, 121, 31));
        start_bt = new QPushButton(motordebug);
        start_bt->setObjectName("start_bt");
        start_bt->setGeometry(QRect(140, 190, 51, 21));
        tarlineEdit = new QLineEdit(motordebug);
        tarlineEdit->setObjectName("tarlineEdit");
        tarlineEdit->setGeometry(QRect(10, 192, 59, 20));
        label_5 = new QLabel(motordebug);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 170, 31, 21));
        stop_bt = new QPushButton(motordebug);
        stop_bt->setObjectName("stop_bt");
        stop_bt->setGeometry(QRect(210, 190, 51, 21));

        retranslateUi(motordebug);

        QMetaObject::connectSlotsByName(motordebug);
    } // setupUi

    void retranslateUi(QWidget *motordebug)
    {
        motordebug->setWindowTitle(QCoreApplication::translate("motordebug", "Form", nullptr));
        label->setText(QCoreApplication::translate("motordebug", "\347\224\265\346\265\201\347\216\257Kp", nullptr));
        label_2->setText(QCoreApplication::translate("motordebug", "\347\224\265\346\265\201\347\216\257Ki", nullptr));
        label_3->setText(QCoreApplication::translate("motordebug", "\347\224\265\346\265\201\347\216\257Kc", nullptr));
        paramenterBT->setText(QCoreApplication::translate("motordebug", "PID\345\217\202\346\225\260\347\241\256\350\256\244", nullptr));
        start_bt->setText(QCoreApplication::translate("motordebug", "\345\220\257\345\212\250", nullptr));
        label_5->setText(QCoreApplication::translate("motordebug", "tar id", nullptr));
        stop_bt->setText(QCoreApplication::translate("motordebug", "\345\201\234\346\255\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class motordebug: public Ui_motordebug {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOTORDEBUG_H
