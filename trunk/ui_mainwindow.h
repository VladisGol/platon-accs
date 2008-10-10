/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Fri Oct 10 22:40:10 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "qeidostreewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_quit;
    QAction *action_Hronology;
    QAction *action_edit;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_search;
    QWidget *tab_explore;
    QGridLayout *gridLayout1;
    QComboBox *comboBox_Species;
    QSplitter *splitter_2;
    QEidosTreeWidget *EidosTreeWidget;
    QSplitter *splitter;
    QTableView *tableViewHypotesis;
    QTableView *tableViewPragma;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(874, 744);
    action_quit = new QAction(MainWindow);
    action_quit->setObjectName(QString::fromUtf8("action_quit"));
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("images/exit.png")), QIcon::Normal, QIcon::Off);
    action_quit->setIcon(icon);
    action_Hronology = new QAction(MainWindow);
    action_Hronology->setObjectName(QString::fromUtf8("action_Hronology"));
    QIcon icon1;
    icon1.addPixmap(QPixmap(QString::fromUtf8("images/Tempor.png")), QIcon::Normal, QIcon::Off);
    action_Hronology->setIcon(icon1);
    action_edit = new QAction(MainWindow);
    action_edit->setObjectName(QString::fromUtf8("action_edit"));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    centralwidget->setGeometry(QRect(0, 57, 874, 664));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    tabWidget = new QTabWidget(centralwidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setCursor(QCursor(Qt::ArrowCursor));
    tabWidget->setStyleSheet(QString::fromUtf8("font: 10pt \"Albany AMT\";"));
    tabWidget->setIconSize(QSize(24, 24));
    tab_search = new QWidget();
    tab_search->setObjectName(QString::fromUtf8("tab_search"));
    tab_search->setGeometry(QRect(0, 0, 840, 420));
    tabWidget->addTab(tab_search, QString());
    tab_explore = new QWidget();
    tab_explore->setObjectName(QString::fromUtf8("tab_explore"));
    tab_explore->setGeometry(QRect(0, 0, 852, 618));
    gridLayout1 = new QGridLayout(tab_explore);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    comboBox_Species = new QComboBox(tab_explore);
    comboBox_Species->setObjectName(QString::fromUtf8("comboBox_Species"));
    comboBox_Species->setMinimumSize(QSize(0, 23));

    gridLayout1->addWidget(comboBox_Species, 0, 0, 1, 1);

    splitter_2 = new QSplitter(tab_explore);
    splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
    splitter_2->setOrientation(Qt::Horizontal);
    EidosTreeWidget = new QEidosTreeWidget(splitter_2);
    EidosTreeWidget->setObjectName(QString::fromUtf8("EidosTreeWidget"));
    EidosTreeWidget->setStyleSheet(QString::fromUtf8("font: 10pt \"Arial\";"));
    splitter_2->addWidget(EidosTreeWidget);
    splitter = new QSplitter(splitter_2);
    splitter->setObjectName(QString::fromUtf8("splitter"));
    splitter->setOrientation(Qt::Vertical);
    tableViewHypotesis = new QTableView(splitter);
    tableViewHypotesis->setObjectName(QString::fromUtf8("tableViewHypotesis"));
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(10);
    font.setBold(false);
    font.setItalic(false);
    font.setWeight(50);
    tableViewHypotesis->setFont(font);
    tableViewHypotesis->setStyleSheet(QString::fromUtf8("font: 10pt \"Arial\";"));
    splitter->addWidget(tableViewHypotesis);
    tableViewPragma = new QTableView(splitter);
    tableViewPragma->setObjectName(QString::fromUtf8("tableViewPragma"));
    splitter->addWidget(tableViewPragma);
    splitter_2->addWidget(splitter);

    gridLayout1->addWidget(splitter_2, 1, 0, 1, 1);

    tabWidget->addTab(tab_explore, QString());

    gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 874, 23));
    menu = new QMenu(menubar);
    menu->setObjectName(QString::fromUtf8("menu"));
    menu_2 = new QMenu(menubar);
    menu_2->setObjectName(QString::fromUtf8("menu_2"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    statusbar->setGeometry(QRect(0, 721, 874, 23));
    MainWindow->setStatusBar(statusbar);
    toolBar = new QToolBar(MainWindow);
    toolBar->setObjectName(QString::fromUtf8("toolBar"));
    toolBar->setGeometry(QRect(0, 23, 874, 34));
    MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

    menubar->addAction(menu->menuAction());
    menubar->addAction(menu_2->menuAction());
    menu->addAction(action_quit);
    menu_2->addAction(action_edit);
    toolBar->addSeparator();
    toolBar->addAction(action_quit);
    toolBar->addSeparator();
    toolBar->addAction(action_Hronology);
    toolBar->addAction(action_edit);

    retranslateUi(MainWindow);
    QObject::connect(action_quit, SIGNAL(activated()), MainWindow, SLOT(close()));

    tabWidget->setCurrentIndex(1);


    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    action_quit->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
    action_Hronology->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \321\205\321\200\320\276\320\275\320\276\320\273\320\276\320\263\320\270\321\216", 0, QApplication::UnicodeUTF8));
    action_edit->setText(QApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_search), QApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272", 0, QApplication::UnicodeUTF8));
    comboBox_Species->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "\320\222\321\201\320\265", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "\320\236\320\261\321\212\320\265\320\272\321\202\321\213", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "\320\224\320\265\320\271\321\201\321\202\320\262\320\270\321\217", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "\320\240\320\265\321\201\321\203\321\200\321\201\321\213", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "\320\235\320\241\320\230", 0, QApplication::UnicodeUTF8)
    );
    EidosTreeWidget->headerItem()->setText(0, QApplication::translate("MainWindow", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
    EidosTreeWidget->headerItem()->setText(1, QApplication::translate("MainWindow", "ID", 0, QApplication::UnicodeUTF8));
    EidosTreeWidget->headerItem()->setText(2, QApplication::translate("MainWindow", "Parent_ID", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_explore), QApplication::translate("MainWindow", "\320\236\320\261\321\212\320\265\320\272\321\202\320\275\320\276\320\265 \320\277\321\200\320\265\320\264\321\201\321\202\320\260\320\262\320\273\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
    menu->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
    menu_2->setTitle(QApplication::translate("MainWindow", "\320\227\320\260\320\277\320\270\321\201\321\214", 0, QApplication::UnicodeUTF8));
    toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
