#include "EAOneFrame.h"
namespace platon
{

EA_OneFrame::EA_OneFrame(QWidget *parent, AssociatedExtraAttribute* InEAA)
	:QFrame(parent)
{
    if (this->objectName().isEmpty())
    	this->setObjectName("EA_OneFrame");
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Plain);
    this->setContentsMargins(0,0,0,5);
    topLayout = new QHBoxLayout(this);// new QGridLayout(this);
    topLayout->setObjectName("gridLayout");
    topLayout->setContentsMargins(0,0,0,0);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("horizontalLayout");
    horizontalLayout->setContentsMargins(5,0,0,0);
    label = new QLabel(this);
    label->setObjectName("label");
    horizontalLayout->addWidget(label);

    frame = new QFrame(this);
    frame->setObjectName("frame");
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Plain);
    frame->setContentsMargins(0,0,0,0);
    gridLayoutInFrame = new QGridLayout(frame);
    gridLayoutInFrame->setContentsMargins(0,0,0,0);
    gridLayoutInFrame->setObjectName("gridLayoutInFrame");

    horizontalLayout->addWidget(frame);

    topLayout->addLayout(horizontalLayout);//, 0, 0, 1, 1);

    //DllCalling=new QAction(this);
    //QObject::connect(DllCalling, SIGNAL(activated()), this, SLOT(CallDllRoutine())));

    //Разбираемся с типом экстраатрибута
    this->EAA=InEAA;
    fillVisibleWidget();
}

EA_OneFrame::~EA_OneFrame()
{
	/*delete topLayout;
	delete horizontalLayout;
	delete label;
	delete frame;
	delete EditableWidget;*/
	;
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
			EditableWidget=new QSpinBox(frame);
			gridLayoutInFrame->addWidget(EditableWidget);
			((QSpinBox*)EditableWidget)->setMinimum(-65536/2);
			((QSpinBox*)EditableWidget)->setMaximum(65536/2);

 			((QSpinBox*)EditableWidget)->setValue(EAA->GetIntValue());
			KeepValue=((QSpinBox*)EditableWidget)->value();
			break;

		}
		case platon::ft_Boolean:
		{
			EditableWidget=new QCheckBox(frame);
			gridLayoutInFrame->addWidget(EditableWidget);

 			((QCheckBox*)EditableWidget)->setChecked(EAA->GetBoolValue());
			KeepValue=((QCheckBox*)EditableWidget)->checkState();
			break;

		}
		case platon::ft_Float:
		{
			EditableWidget=new QDoubleSpinBox(frame);
			gridLayoutInFrame->addWidget(EditableWidget);

 			((QDoubleSpinBox*)EditableWidget)->setValue(EAA->GetFloatValue());
			KeepValue=((QDoubleSpinBox*)EditableWidget)->value();
			break;
		}
		case platon::ft_DateTime:
		{

			EditableWidget=new QDateTimeEdit(frame);
			((QDateTimeEdit*)EditableWidget)->setDisplayFormat("dd.MM.yyyy H:mm");
			((QDateTimeEdit*)EditableWidget)->setCalendarPopup(true);
			((QDateEdit*)EditableWidget)->calendarWidget()->setFirstDayOfWeek(Qt::Monday);
			gridLayoutInFrame->addWidget(EditableWidget);

 			((QDateTimeEdit*)EditableWidget)->setDateTime(IBPPTimestamp2QDateTime(EAA->GetDateTimeValue()));
			KeepValue=((QDateTimeEdit*)EditableWidget)->dateTime();
			break;
		}
		case platon::ft_RB:
		case platon::ft_DLL:
		{
			DLLButton = new QToolButton(this);
			DLLButton->setObjectName("DLLButton");
		    QIcon icon1;
		    icon1.addPixmap(QPixmap(QString::fromUtf8("images/exec.png")), QIcon::Normal, QIcon::Off);
		    DLLButton->setIcon(icon1);
		    horizontalLayout->addWidget(DLLButton);
			EditableWidget=new QLineEdit(frame);
			gridLayoutInFrame->addWidget(EditableWidget);

 			((QLineEdit*)EditableWidget)->setText(tr(EAA->GetVisibleValue().c_str()));
			KeepValue=((QLineEdit*)EditableWidget)->text();
			//toolButton->addAction(DllCalling);

		    break;
		}
		case platon::ft_LinkHypotesis:
		case platon::ft_LinkPragma:
		{
			EditableWidget=new QLineEdit(frame);
			gridLayoutInFrame->addWidget(EditableWidget);

 			((QLineEdit*)EditableWidget)->setText(tr(EAA->GetVisibleValue().c_str()));
			KeepValue=((QLineEdit*)EditableWidget)->text();

			LNKButton = new QToolButton(this);
			LNKButton->setObjectName("LNKButton");
		    QIcon icon1;
		    icon1.addPixmap(QPixmap(QString::fromUtf8("images/chain_nicu_buculei_01.png")), QIcon::Normal, QIcon::Off);
		    LNKButton->setIcon(icon1);

		    horizontalLayout->addWidget(LNKButton);

			break;
		}
		default:
			throw("Указанный тип данных не поддерживается");
	}
	// Добавляем кнопку хронологического вывода по атрибутам
	if(EAA->EA->Temporality==true)
	{
		HronologyButton = new QToolButton(this);
		HronologyButton->setObjectName("HronologyButton");
		QIcon icon2;
		icon2.addPixmap(QPixmap(QString::fromUtf8("images/clock.png")), QIcon::Normal, QIcon::Off);
		HronologyButton->setIcon(icon2);
		horizontalLayout->addWidget(HronologyButton);
	}
	// Изменяем начертание шрифта для отображения обязательных для заполнения атрибутов
	if(EAA->EA->Required==true)
	{
		QFont font=label->font();
		font.setBold(true);
		font.setWeight(75);
		label->setFont(font);
	}
	//Изменяем шрифт для блокированный значений фреймов
	if(EAA->EA->Locked==true)
	{
		EditableWidget->setEnabled(false);
		QFont font=label->font();
		font.setItalic(true);
		label->setFont(font);

	}

}

void EA_OneFrame::Save()
{
	//Процедура записывает измененное значение фрейма
	if(EAA->EA->Locked==false)	//Только для незакрытых значений фреймов
	{
		EAA->EA->DTValue=platon::QDateTime2IBPPTimestamp(QDateTime::currentDateTime());	//Изменение времени на системное для темпоральных значений
		switch(EAA->EA->type)
		{
			case platon::ft_String:
			{
				if (KeepValue!=((QLineEdit*)EditableWidget)->text())
					EAA->SetStringValue(((QLineEdit*)EditableWidget)->text().toStdString());
				break;
			}
			case platon::ft_Integer:
			{
				if(KeepValue!=((QSpinBox*)EditableWidget)->value())
					EAA->SetIntValue(((QSpinBox*)EditableWidget)->value());
				break;

			}
			case platon::ft_Boolean:
			{
				if(KeepValue!=((QCheckBox*)EditableWidget)->checkState())
					EAA->SetBoolValue(((QCheckBox*)EditableWidget)->checkState());
				break;

			}
			case platon::ft_Float:
			{
				if(KeepValue!=((QDoubleSpinBox*)EditableWidget)->value())
					EAA->SetFloatValue(((QDoubleSpinBox*)EditableWidget)->value());
				break;
			}
			case platon::ft_DateTime:
			{
				if(KeepValue!=((QDateTimeEdit*)EditableWidget)->dateTime())
					EAA->SetDateTimeValue(QDateTime2IBPPTimestamp(((QDateTimeEdit*)EditableWidget)->dateTime()));
				break;
			}
			case platon::ft_RB:
			case platon::ft_DLL:
			case platon::ft_LinkHypotesis:
			case platon::ft_LinkPragma:
			{
				if(KeepValue!=((QLineEdit*)EditableWidget)->text())
					;
				break;
			}
			default:
				throw("Указанный тип данных не поддерживается");

		}
	}
}
void EA_OneFrame::CallDllRoutine()
{
	//Функция вызывает процедуру из динамической библиотеки
	QMessageBox::information(this,"Imitation","Called procedure");
}

QDateTime IBPPTimestamp2QDateTime(IBPP::Timestamp InVal)
{
//Функция переводит дату время из формата IBPP::Timestamp в формат QDateTime
	QString year=QString::number(InVal.Year());
	QString month=QString("0"+QString::number(InVal.Month())).right(2);
	QString day=QString("0"+QString::number(InVal.Day())).right(2);
	QDateTime LocAlValue=QDateTime::fromString(year+month+day,"yyyyMMdd");
	LocAlValue.addSecs(InVal.Hours()*3600+InVal.Minutes()*60+InVal.Seconds());
	return LocAlValue;
}

IBPP::Timestamp QDateTime2IBPPTimestamp(QDateTime InVal)
{
//Функция переводит дату время из формата QDateTime в формат IBPP::Timestamp

	int y, mo, d, h, mi, s;
	y=QString(InVal.toString("yyyy")).toInt();
	mo=QString(InVal.toString("MM")).toInt();
	d=QString(InVal.toString("dd")).toInt();
	h=QString(InVal.toString("hh")).toInt();
	mi=QString(InVal.toString("mm")).toInt();
	s=QString(InVal.toString("ss")).toInt();
	return IBPP::Timestamp(y, mo, d, h, mi, s);
}
}
