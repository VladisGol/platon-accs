/********************************************************************************
** Form generated from reading ui file 'EAOneFrame.ui'
**
** Created: Thu Oct 9 21:43:28 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EAONEFRAME_H
#define UI_EAONEFRAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QFrame *frame;
    QToolButton *toolButton;

    void setupUi(QFrame *Form)
    {
    if (Form->objectName().isEmpty())
        Form->setObjectName(QString::fromUtf8("Form"));
    Form->resize(847, 48);
    Form->setFrameShape(QFrame::StyledPanel);
    Form->setFrameShadow(QFrame::Raised);
    gridLayout = new QGridLayout(Form);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label = new QLabel(Form);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout->addWidget(label);

    frame = new QFrame(Form);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    horizontalLayout->addWidget(frame);

    toolButton = new QToolButton(Form);
    toolButton->setObjectName(QString::fromUtf8("toolButton"));

    horizontalLayout->addWidget(toolButton);


    gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


    retranslateUi(Form);

    QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QFrame *Form)
    {
    Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
    toolButton->setText(QApplication::translate("Form", "...", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EAONEFRAME_H
