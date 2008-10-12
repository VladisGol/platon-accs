#include "EAOneFrame.h"
namespace platon
{

EA_OneFrame::EA_OneFrame(QWidget *parent, AssociatedExtraAttribute* InEAA)
	:QFrame(parent)
{
    if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("EA_OneFrame"));
    this->resize(847, 48);
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Plain);
    this->setContentsMargins(0,0,0,1);
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName("gridLayout");
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("horizontalLayout");
    label = new QLabel(this);
    label->setObjectName("label");

    horizontalLayout->addWidget(label);

    frame = new QFrame(this);
    frame->setObjectName("frame");
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Plain);
    frame->setContentsMargins(0,0,0,0);
    gridLayoutInFrame = new QGridLayout(frame);
    gridLayoutInFrame->setObjectName("gridLayoutInFrame");

    horizontalLayout->addWidget(frame);

    toolButton = new QToolButton(this);
    toolButton->setObjectName("toolButton");

    horizontalLayout->addWidget(toolButton);
    gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

    //Разбираемся с типом экстраатрибута
    this->EAA=InEAA;
    fillVisibleWidget();
}

EA_OneFrame::~EA_OneFrame()
{
	delete gridLayout;
	delete horizontalLayout;
	delete label;
	delete frame;
	delete toolButton;
}

void EA_OneFrame::fillVisibleWidget()
{
	label->setText(tr(EAA->EA->GetEACaption().c_str()));
	switch(EAA->EA->type)
	{
		case platon::ft_String:
		{
			EditableWidget=new QLineEdit(frame);
			gridLayoutInFrame->addWidget(EditableWidget);

 			((QLineEdit*)EditableWidget)->setText(tr(EAA->GetStringValue().c_str()));
			KeepValue=((QLineEdit*)EditableWidget)->text();
			break;
		}
		case platon::ft_Integer:
		{
			break;
		}
		case platon::ft_Boolean:
		{
			break;
		}
		case platon::ft_Float:
		{
			break;
		}
		case platon::ft_DateTime:
		{
			/*
			IBPP::Timestamp MyTs;
			MyIterator->LocalST->Get("MEANING",MyTs);
			Key=MyIterator->GetID();

			QString year=QString::number(MyTs.Year());
			QString month=QString("0"+QString::number(MyTs.Month())).right(2);
			QString day=QString("0"+QString::number(MyTs.Day())).right(2);
			QDateTime LocAlValue=QDateTime::fromString(year+month+day,"yyyyMMdd");
			LocAlValue.addSecs(MyTs.Hours()*3600+MyTs.Minutes()*60+MyTs.Seconds());

			OneValue=LocAlValue;*/
			break;
		}
		case platon::ft_RB:
		case platon::ft_DLL:
		case platon::ft_LinkHypotesis:
		case platon::ft_LinkPragma:
		{

			break;
		}
		default:
			throw("Указанный тип данных не поддерживается");

	}

}

/*

#ifndef EAONEFRAMELH4050_H
#define EAONEFRAMELH4050_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QDateTimeEdit *dateTimeEdit;
    QToolButton *toolButton;

    void setupUi(QFrame *Form)
    {
    if (Form->objectName().isEmpty())
        Form->setObjectName(QString::fromUtf8("Form"));
    Form->resize(862, 68);
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
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    frame->setSizePolicy(sizePolicy);
    QFont font;
    font.setKerning(false);
    frame->setFont(font);
    frame->setFrameShape(QFrame::NoFrame);
    frame->setFrameShadow(QFrame::Plain);
    frame->setLineWidth(0);
    frame->setMidLineWidth(0);
    gridLayout_2 = new QGridLayout(frame);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    dateTimeEdit = new QDateTimeEdit(frame);
    dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));
    dateTimeEdit->setCalendarPopup(true);

    verticalLayout->addWidget(dateTimeEdit);


    gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


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

#endif // EAONEFRAMELH4050_H

 */


}
