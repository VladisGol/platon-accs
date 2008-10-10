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
    this->setFrameShadow(QFrame::Raised);
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout->addWidget(label);

    frame = new QFrame(this);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    horizontalLayout->addWidget(frame);

    toolButton = new QToolButton(this);
    toolButton->setObjectName(QString::fromUtf8("toolButton"));

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
}
