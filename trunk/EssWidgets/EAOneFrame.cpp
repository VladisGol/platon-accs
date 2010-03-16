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
			if(EAA->EA->LNK_NeedList)
			{
				if(EAA->EA->type==platon::ft_LinkPragma)
				{
					platon::Eidos* ListEidos=new platon::Eidos(EAA->OwnerHypotesis->HostEidos->DB,EAA->EA->LNK_EidosID);
					EditableWidget=new LnkHypPragmaComboBox(ListEidos, frame);	//Меняем на типизированное значение выбора из списка
					((LnkHypPragmaComboBox*)EditableWidget)->SetCurrentIndexByID(EAA->GetLink2PValue().LinkTo);
		 			delete ListEidos;

		 			KeepValue=QVariant::fromValue(EAA->GetLink2PValue().LinkTo);

				}
				else
				{
					platon::Eidos* ListEidos=new platon::Eidos(EAA->OwnerHypotesis->HostEidos->DB,EAA->EA->LNK_EidosID);
					EditableWidget=new LnkHypComboBox(ListEidos, frame);
		 			((LnkHypComboBox*)EditableWidget)->SetCurrentIndexByID(EAA->GetLink2HValue().LinkTo);
		 			delete ListEidos;

		 			KeepValue=QVariant::fromValue(EAA->GetLink2HValue().LinkTo);
				}
				gridLayoutInFrame->addWidget(EditableWidget);
			}
			else
			{
				EditableWidget=new QLineEdit(frame);

	 			((QLineEdit*)EditableWidget)->setText(tr(EAA->GetVisibleValue().c_str()));
				KeepValue=((QLineEdit*)EditableWidget)->text();

				gridLayoutInFrame->addWidget(EditableWidget);

				LNKButton = new QToolButton(this);
				LNKButton->setObjectName("LNKButton");
				QIcon icon1;
				icon1.addPixmap(QPixmap(QString::fromUtf8(":/PICS/chain_nicu_buculei_01.png")), QIcon::Normal, QIcon::Off);
				LNKButton->setIcon(icon1);

				horizontalLayout->addWidget(LNKButton);
			}
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
			case platon::ft_DLL:
				//Данные значения обрабатываются в процедурах вызова кнопок
				break;

			case platon::ft_LinkHypotesis:
			case platon::ft_LinkPragma:
			{
				if(EAA->EA->LNK_NeedList)
				{
					if(EAA->EA->type==platon::ft_LinkPragma)
					{
						long ID=((LnkHypPragmaComboBox*)EditableWidget)->GetCurrentID();
						if(KeepValue.toInt()!=ID)
						{
							platon::LNK_Value localLNKVal;
							localLNKVal.LinkTo=ID;
							localLNKVal.Ratio=EAA->GetLink2PValue().Ratio;
							if(localLNKVal.LinkTo>0) EAA->SetLink2PValue(localLNKVal);
						}
						break;
					}
					else
					{
						long ID=((LnkHypComboBox*)EditableWidget)->GetCurrentID();
						if(KeepValue.toInt()!=ID)
						{
							platon::LNK_Value localLNKVal;
							localLNKVal.LinkTo=ID;
							localLNKVal.Ratio=EAA->GetLink2HValue().Ratio;
							if(localLNKVal.LinkTo>0) EAA->SetLink2HValue(localLNKVal);
						}
						break;
					}
				}
				// Иначе обработка проходит в процедурах вызова кнопок
				break;
			}
			default:
				throw("Указанный тип данных не поддерживается");

		}
		MyDCl->SetProgramDateTime();
	}
}

void EA_OneFrame::LNKClick()
{
	long ID_Eidos, ID_Hyp;
	if(EAA->EA->type==platon::ft_LinkHypotesis)
	{
		if(EAA->EA->LNK_EidosID==0)	//Выводим список эйдосов
		{
			QString spec=QString::fromStdString(EAA->EA->LNK_species);
			if(spec=="")spec="ALL";
			platon::Hypotesis::GetEidosHypotesisIDS(EAA->OwnerHypotesis->HostEidos->DB,EAA->GetLink2HValue().LinkTo,ID_Eidos, ID_Hyp);

			ChoiceEidos_Dialog* Localdialog=new ChoiceEidos_Dialog(this,spec,ID_Eidos);
			Localdialog->exec();
			ID_Eidos=Localdialog->Out_value;
		}
		else ID_Eidos=EAA->EA->LNK_EidosID;
		if(ID_Eidos==0)	return;

		//Выводим список гипотез для выбора
		platon::Eidos *localEidos=new platon::Eidos(EAA->OwnerHypotesis->HostEidos->DB,ID_Eidos);
		ChoiceHypotesis_Dialog* HypotesisDialog=new ChoiceHypotesis_Dialog(this,localEidos,EAA->GetLink2HValue().LinkTo);
		HypotesisDialog->exec();
		platon::LNK_Value localLNKVal;
		localLNKVal.LinkTo=HypotesisDialog->Out_value;
		localLNKVal.Ratio=EAA->GetLink2HValue().Ratio;
		if(localLNKVal.LinkTo>0) EAA->SetLink2HValue(localLNKVal);
		delete localEidos;
		((QLineEdit*)EditableWidget)->setText(tr(EAA->GetVisibleValue().c_str()));
	}
	if(EAA->EA->type==platon::ft_LinkPragma)
	{
		if(EAA->EA->LNK_EidosID==0)	//Выводим список эйдосов
		{
			QString spec=QString::fromStdString(EAA->EA->LNK_species);
			if(spec=="")spec="ALL";
			platon::Pragma::GetEidosHypotesisIDS(EAA->OwnerHypotesis->HostEidos->DB,EAA->GetLink2PValue().LinkTo,ID_Eidos, ID_Hyp);

			ChoiceEidos_Dialog* Localdialog=new ChoiceEidos_Dialog(this,spec,ID_Eidos);
			Localdialog->exec();
			ID_Eidos=Localdialog->Out_value;
		}
		else ID_Eidos=EAA->EA->LNK_EidosID;
		if(ID_Eidos==0)	return;

		//Выводим список прагм с наименованиями гипотез для выбора
		platon::Eidos *localEidos=new platon::Eidos(EAA->OwnerHypotesis->HostEidos->DB,ID_Eidos);
		ChoicePragma_Dialog* PragmaDialog=new ChoicePragma_Dialog(this,localEidos,EAA->GetLink2PValue().LinkTo);
		PragmaDialog->exec();
		platon::LNK_Value localLNKVal;
		localLNKVal.LinkTo=PragmaDialog->Out_value;
		localLNKVal.Ratio=EAA->GetLink2HValue().Ratio;

		if(localLNKVal.LinkTo>0) EAA->SetLink2PValue(localLNKVal);
		delete localEidos;
		((QLineEdit*)EditableWidget)->setText(tr(EAA->GetVisibleValue().c_str()));
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
	QLibrary* CalledLib= this->MyDCl->GetLibByName(QString::fromStdString(this->EAA->EA->DLL_FileName));
	if(CalledLib==NULL)
	{
		QString Message=tr("Требуемая процедура '")+QString::fromStdString(this->EAA->EA->DLL_FileName)+tr("' не найдена");
		QMessageBox::information(this,tr("Внимание"),Message);
	}
	else
	{
		//Вызов процедуры из библиотеки. Можно будет описать после разработки процедуры
	}

}

}
