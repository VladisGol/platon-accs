#include "EAOneFrame.h"

EA_OneFrame::EA_OneFrame(QWidget *parent)
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
}

EA_OneFrame::~EA_OneFrame()
{
	delete gridLayout;
	delete horizontalLayout;
	delete label;
	delete frame;
	delete toolButton;
}
