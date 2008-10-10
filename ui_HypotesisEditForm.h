/********************************************************************************
** Form generated from reading ui file 'HypotesisEditForm.ui'
**
** Created: Fri Oct 10 22:40:10 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_HYPOTESISEDITFORM_H
#define UI_HYPOTESISEDITFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HypotesisEditForm
{
public:
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;

    void setupUi(QDialog *HypotesisEditForm)
    {
    if (HypotesisEditForm->objectName().isEmpty())
        HypotesisEditForm->setObjectName(QString::fromUtf8("HypotesisEditForm"));
    HypotesisEditForm->resize(788, 516);
    gridLayout = new QGridLayout(HypotesisEditForm);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    scrollArea = new QScrollArea(HypotesisEditForm);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 766, 494));
    gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

    scrollArea->setWidget(scrollAreaWidgetContents);

    gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


    retranslateUi(HypotesisEditForm);

    QMetaObject::connectSlotsByName(HypotesisEditForm);
    } // setupUi

    void retranslateUi(QDialog *HypotesisEditForm)
    {
    HypotesisEditForm->setWindowTitle(QApplication::translate("HypotesisEditForm", "Dialog", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(HypotesisEditForm);
    } // retranslateUi

};

namespace Ui {
    class HypotesisEditForm: public Ui_HypotesisEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HYPOTESISEDITFORM_H
