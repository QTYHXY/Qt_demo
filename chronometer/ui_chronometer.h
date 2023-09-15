/********************************************************************************
** Form generated from reading UI file 'chronometer.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHRONOMETER_H
#define UI_CHRONOMETER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Chronometer
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *startBtn;
    QPushButton *recordBtn;
    QPushButton *stopBtn;
    QPushButton *resetBtn;

    void setupUi(QWidget *Chronometer)
    {
        if (Chronometer->objectName().isEmpty())
            Chronometer->setObjectName(QString::fromUtf8("Chronometer"));
        Chronometer->resize(400, 300);
        verticalLayout_2 = new QVBoxLayout(Chronometer);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(Chronometer);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 80));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(22);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listWidget = new QListWidget(Chronometer);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout->addWidget(listWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        startBtn = new QPushButton(Chronometer);
        startBtn->setObjectName(QString::fromUtf8("startBtn"));
        startBtn->setMinimumSize(QSize(50, 40));

        verticalLayout->addWidget(startBtn);

        recordBtn = new QPushButton(Chronometer);
        recordBtn->setObjectName(QString::fromUtf8("recordBtn"));
        recordBtn->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(recordBtn);

        stopBtn = new QPushButton(Chronometer);
        stopBtn->setObjectName(QString::fromUtf8("stopBtn"));
        stopBtn->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(stopBtn);

        resetBtn = new QPushButton(Chronometer);
        resetBtn->setObjectName(QString::fromUtf8("resetBtn"));
        resetBtn->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(resetBtn);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(Chronometer);

        QMetaObject::connectSlotsByName(Chronometer);
    } // setupUi

    void retranslateUi(QWidget *Chronometer)
    {
        Chronometer->setWindowTitle(QCoreApplication::translate("Chronometer", "Chronometer", nullptr));
        label->setText(QCoreApplication::translate("Chronometer", "\345\210\235\345\247\213\345\214\226", nullptr));
        startBtn->setText(QCoreApplication::translate("Chronometer", "\345\274\200\345\247\213", nullptr));
        recordBtn->setText(QCoreApplication::translate("Chronometer", "\350\256\260\345\275\225", nullptr));
        stopBtn->setText(QCoreApplication::translate("Chronometer", "\346\232\202\345\201\234", nullptr));
        resetBtn->setText(QCoreApplication::translate("Chronometer", "\346\270\205\351\233\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Chronometer: public Ui_Chronometer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHRONOMETER_H
