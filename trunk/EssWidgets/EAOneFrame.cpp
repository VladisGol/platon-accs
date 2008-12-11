#include "EAOneFrame.h"
namespace platon
{

EA_OneFrame::EA_OneFrame(QWidget *parent, AssociatedExtraAttribute* InEAA)
	:QFrame(parent)
{
    LNKButton=NULL;
    HronologyButton=NULL;
    DLLButton=NULL;

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

    //Разбираемся с типом экстраатрибута
    this->EAA=InEAA;
    fillVisibleWidget();
    //Присоединяем обработку сигалов к слотам
    if(LNKButton!=NULL)   		QObject::connect(LNKButton, SIGNAL(clicked()), this, SLOT(LNKClick()));
    if(HronologyButton!=NULL)	QObject::connect(HronologyButton, SIGNAL(clicked()), this, SLOT(HronologyClick()));
    if(DLLButton!=NULL)			QObject::connect(DLLButton, SIGNAL(clicked()), this, SLOT(CallDllRoutine()));

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
			((QDateTimeEdit*)EditableWidget)->setDisplayFormat("dd.MM.yyyy HH:mm");
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
		    icon1.addPixmap(QPixmap(QString::fromUtf8((":/PICS/exec.png"))), QIcon::Normal, QIcon::Off);
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
		    icon1.addPixmap(QPixmap(QString::fromUtf8(":/PICS/chain_nicu_buculei_01.png")), QIcon::Normal, QIcon::Off);
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
		icon2.addPixmap(QPixmap(QString::fromUtf8(":/PICS/clock.png")), QIcon::Normal, QIcon::Off);
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

void EA_OneFrame::LNKClick()
{
	long ID_Eidos, ID_Hypotesis;

	if(EAA->EA->LNK_HypID>0)	//Если определен уровень к которому следует отнести текущий
	{
		if(EAA->EA->type==platon::ft_LinkHypotesis)	//Ссылка указывает на объект Hypotesis
			platon::Hypotesis::GetEidosHypotesisIDS(EAA->OwnerHypotesis->HostEidos->DB,EAA->EA->LNK_HypID,ID_Eidos, ID_Hypotesis);
		if(EAA->EA->type==platon::ft_LinkPragma)	//Ссылка указывает на объект Pragma
			platon::Pragma::GetEidosHypotesisIDS(EAA->OwnerHypotesis->HostEidos->DB,EAA->EA->LNK_HypID,ID_Eidos, ID_Hypotesis);

		if(ID_Eidos==0)
		{
			QString Species="ALL";
			if(QString::fromStdString(EAA->EA->LNK_species)!="") Species=QString::fromStdString(EAA->EA->LNK_species);

			ChoiceEidos_Dialog* Localdialog=new ChoiceEidos_Dialog(this,EAA->OwnerHypotesis->HostEidos->DB,Species,0);
			Localdialog->exec();
			ID_Eidos=Localdialog->Out_value;
			if(ID_Eidos==0) return;	//Ничего не выбрано
			if(EAA->EA->type==platon::ft_LinkHypotesis)
			{
				platon::Eidos *localEidos=new platon::Eidos(EAA->OwnerHypotesis->HostEidos->DB,ID_Eidos);
				ChoiceHypotesis_Dialog* HypotesisDialog=new ChoiceHypotesis_Dialog(this,localEidos,0);
				HypotesisDialog->exec();
				delete localEidos;
			}
			if(EAA->EA->type==platon::ft_LinkPragma)
			{
				platon::Eidos *localEidos=new platon::Eidos(EAA->OwnerHypotesis->HostEidos->DB,ID_Eidos);
				ChoicePragma_Dialog* PragmaDialog=new ChoicePragma_Dialog(this,localEidos,0);
				PragmaDialog->exec();
				delete localEidos;
			}
		}
	}
	else	//Уровень не предопределен, значит следует в любом случае получать подтверждение пользователя
	{
		if(EAA->EA->type==platon::ft_LinkHypotesis)	//Ссылка указывает на объект Hypotesis
			platon::Hypotesis::GetEidosHypotesisIDS(EAA->OwnerHypotesis->HostEidos->DB,EAA->GetLink2HValue().LinkTo,ID_Eidos, ID_Hypotesis);
		if(EAA->EA->type==platon::ft_LinkPragma)	//Ссылка указывает на объект Pragma
			platon::Pragma::GetEidosHypotesisIDS(EAA->OwnerHypotesis->HostEidos->DB,EAA->GetLink2PValue().LinkTo,ID_Eidos, ID_Hypotesis);

		QString Species="ALL";
		if(ID_Eidos>0)
			Species.fromStdString(platon::GetEidosSpecies(EAA->OwnerHypotesis->HostEidos->DB,ID_Eidos));

		ChoiceEidos_Dialog* Localdialog=new ChoiceEidos_Dialog(this,EAA->OwnerHypotesis->HostEidos->DB,Species,ID_Eidos);
		Localdialog->exec();
		ID_Eidos=Localdialog->Out_value;
		if(ID_Eidos==0) return;	//Ничего не выбрано
		//Далее вызывается форма для выбора из числа объектов
		if(EAA->EA->type==platon::ft_LinkHypotesis)
		{
			platon::Eidos *localEidos=new platon::Eidos(EAA->OwnerHypotesis->HostEidos->DB,ID_Eidos);
			ChoiceHypotesis_Dialog* HypotesisDialog=new ChoiceHypotesis_Dialog(this,localEidos,ID_Hypotesis);
			HypotesisDialog->exec();
			QMessageBox::information(this,"Imitation",QString::number(HypotesisDialog->Out_value));
			delete localEidos;
		}
		if(EAA->EA->type==platon::ft_LinkPragma)
		{
			platon::Eidos *localEidos=new platon::Eidos(EAA->OwnerHypotesis->HostEidos->DB,ID_Eidos);
			ChoicePragma_Dialog* PragmaDialog=new ChoicePragma_Dialog(this,localEidos,EAA->GetLink2PValue().LinkTo);
			PragmaDialog->exec();
			QMessageBox::information(this,"Imitation",QString::number(PragmaDialog->Out_value));
			delete localEidos;
		}

	}
}

void EA_OneFrame::HronologyClick()
{
	DialogEditHronologyEA* HronologyDialog=new DialogEditHronologyEA(this,EAA);
	HronologyDialog->exec();
}

void EA_OneFrame::CallDllRoutine()
{
	//Функция вызывает процедуру из динамической библиотеки
	QMessageBox::information(this,"Imitation","Called procedure");
}

}
