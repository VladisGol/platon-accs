/********************************************************************************
** Form generated from reading ui file 'HypotesisEditForm.ui'
**
** Created: Fri Nov 7 21:22:00 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_HYPOTESISEDITFORM_H
#define UI_HYPOTESISEDITFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HypotesisWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *HypotesisWindow)
    {
    if (HypotesisWindow->objectName().isEmpty())
        HypotesisWindow->setObjectName(QString::fromUtf8("HypotesisWindow"));
    HypotesisWindow->resize(800, 600);
    centralwidget = new QWidget(HypotesisWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    scrollArea = new QScrollArea(centralwidget);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 778, 529));
    gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

    scrollArea->setWidget(scrollAreaWidgetContents);

    gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

    HypotesisWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(HypotesisWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 800, 28));
    HypotesisWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(HypotesisWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    HypotesisWindow->setStatusBar(statusbar);

    retranslateUi(HypotesisWindow);

    QMetaObject::connectSlotsByName(HypotesisWindow);
    } // setupUi

    void retranslateUi(QMainWindow *HypotesisWindow)
    {
    HypotesisWindow->setWindowTitle(QApplication::translate("HypotesisWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(HypotesisWindow);
    } // retranslateUi

};

namespace Ui {
    class HypotesisWindow: public Ui_HypotesisWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HYPOTESISEDITFORM_H
