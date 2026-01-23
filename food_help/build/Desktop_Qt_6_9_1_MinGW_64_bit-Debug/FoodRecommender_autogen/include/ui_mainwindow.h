/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *topButtons;
    QPushButton *generateDayButton;
    QSpacerItem *horizontalSpacerTop;
    QHBoxLayout *bottomButtons;
    QPushButton *addDishButton;
    QPushButton *showAllButton;
    QSpacerItem *horizontalSpacerBottom;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        topButtons = new QHBoxLayout();
        topButtons->setObjectName("topButtons");
        generateDayButton = new QPushButton(centralwidget);
        generateDayButton->setObjectName("generateDayButton");
        generateDayButton->setMinimumHeight(50);

        topButtons->addWidget(generateDayButton);

        horizontalSpacerTop = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topButtons->addItem(horizontalSpacerTop);


        verticalLayout->addLayout(topButtons);

        bottomButtons = new QHBoxLayout();
        bottomButtons->setObjectName("bottomButtons");
        addDishButton = new QPushButton(centralwidget);
        addDishButton->setObjectName("addDishButton");

        bottomButtons->addWidget(addDishButton);

        showAllButton = new QPushButton(centralwidget);
        showAllButton->setObjectName("showAllButton");

        bottomButtons->addWidget(showAllButton);

        horizontalSpacerBottom = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomButtons->addItem(horizontalSpacerBottom);


        verticalLayout->addLayout(bottomButtons);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\237\320\273\320\260\320\275\320\270\321\200\320\276\320\262\321\211\320\270\320\272 \320\261\320\273\321\216\320\264", nullptr));
        generateDayButton->setText(QCoreApplication::translate("MainWindow", "\320\241\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\264\320\265\320\275\321\214", nullptr));
        addDishButton->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\201\320\262\320\276\321\221 \320\261\320\273\321\216\320\264\320\276", nullptr));
        showAllButton->setText(QCoreApplication::translate("MainWindow", "\320\222\321\201\320\265 \320\261\320\273\321\216\320\264\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
