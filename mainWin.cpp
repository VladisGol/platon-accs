#include "mainWin.h"

mainWin::mainWin(QWidget *parent)
    : QMainWindow(parent)

{
		QTextCodec * codec = QTextCodec::codecForName("CP1251");
		QTextCodec::setCodecForTr(codec);
		QTextCodec::setCodecForCStrings(codec);
		//QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));


		setupUi(this);

#ifdef IBPP_WINDOWS
                MyDB = IBPP::DatabaseFactory("vladisgol",
                                                "platon",
                                                "sysdba",
                                                "masterkey",
                                                "",//Role
                                                "WIN1251",//codepage
                                                "DEFAULT CHARACTER SET WIN1251");//Доп параметры
#else
                MyDB = IBPP::DatabaseFactory("vladisgol",
                                                "platon",
                                                "sysdba",
                                                "ymIyiAdV",
                                                "",//Role
                                                "WIN1251",//codepage
                                                "DEFAULT CHARACTER SET WIN1251");//Доп параметры
#endif
		MyDB->Connect();

		//platon::iterEidos* MyEidosIter= new  platon::iterEidos(MyDB,"ALL");

		LocalEidos=NULL;
		LocalHypotesis=NULL;

		QObject::connect(EidosTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem* ,int)), this, SLOT(SetHypotesysView(QTreeWidgetItem*,int)));
		QObject::connect(tableViewHypotesis, SIGNAL(activated(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));

		QObject::connect(tableViewHypotesis, SIGNAL(pressed(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		QObject::connect(tableViewHypotesis, SIGNAL(clicked(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		//QObject::connect(tableViewHypotesis, SIGNAL(clicked(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		QObject::connect(tableViewHypotesis, SIGNAL(entered(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));

		//Привязываем элементы управления к событиям
		QObject::connect(comboBox_Species, SIGNAL(currentIndexChanged(int)), this, SLOT(SetEidosView(int)));
		QObject::connect(action_edit, SIGNAL(activated()), this, SLOT(EditItem()));
		QObject::connect(action_add, SIGNAL(activated()), this, SLOT(AddItem()));
		QObject::connect(action_del, SIGNAL(activated()), this, SLOT(DeleteItem()));
		QObject::connect(action_refresh, SIGNAL(activated()), this, SLOT(RefreshViews()));
		QObject::connect(action_links, SIGNAL(activated()), this, SLOT(Showlinks()));

		SetEidosView(0);

		this->EidosTreeWidget->installEventFilter(this);
		this->tableViewHypotesis->installEventFilter(this);
		this->tableViewPragma->installEventFilter(this);
		CurrentObjectLevel=0;

		//Программное время
		this->ProgramDateTime = QDateTime::currentDateTime();
		DTBaseShifter=new QTimer(this);
		connect(DTBaseShifter, SIGNAL(timeout()), this, SLOT(BaseTimeShift()));
		BaseTimeShift();
		DTBaseShifter->start(10*1000);	//Устанавливаем время обновления
}

bool mainWin::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == EidosTreeWidget)
	{
		if(event->type()==QEvent::FocusIn)
		{
			this->action_add->setEnabled(false);
			this->action_del->setEnabled(false);
			this->action_edit->setEnabled(false);
			this->action_links->setEnabled(false);
		}
	}
	if (obj == tableViewHypotesis)
	{
		if(event->type()==QEvent::FocusIn)
		{
			this->action_add->setEnabled(true);
			this->action_del->setEnabled(true);
			this->action_edit->setEnabled(true);
			this->action_links->setEnabled(true);
			CurrentObjectLevel=Level_Hypotesis;
		}
	}
	if (obj == tableViewPragma)
	{
		if(event->type()==QEvent::FocusIn)
		{
			QString Species=QString::fromStdString(LocalEidos->GetEidosSpecies());
			if(Species=="OBJ")
			{
				this->action_add->setEnabled(true);
				this->action_del->setEnabled(true);
				this->action_edit->setEnabled(true);
				this->action_links->setEnabled(true);
			}
			else
			{
				this->action_add->setEnabled(false);
				this->action_del->setEnabled(true);
				this->action_edit->setEnabled(true);
				this->action_links->setEnabled(true);
			}
			CurrentObjectLevel=Level_Pragma;
		}
	}

	return false;
}


void mainWin::SetEidosView(int Row)
{
	switch (Row)
	{
	case 0:
		EidosTreeWidget->SetSpecies("ALL");
		break;
	case 1:
		EidosTreeWidget->SetSpecies("OBJ");
		break;
	case 2:
		EidosTreeWidget->SetSpecies("ACT");
		break;
	case 3:
		EidosTreeWidget->SetSpecies("RES");
		break;
	case 4:
		EidosTreeWidget->SetSpecies("NSI");
	}

	EidosTreeWidget->AttachToDB(MyDB);
}

void mainWin::SetHypotesysView(QTreeWidgetItem*CurItem , int Column)
{
	platon::HypotesisMemModel* keep4delete=NULL;

	long id_eidos=CurItem->text(1).toLong();
	//long id_eidos=QVariant(CurItem->data(2,Qt::DisplayRole)).toInt();

	if(LocalEidos!=NULL)
	{
		delete LocalEidos;
		keep4delete=(platon::HypotesisMemModel*)tableViewHypotesis->model();
	}

	LocalEidos=new platon::Eidos(MyDB,id_eidos);
	platon::HypotesisMemModel* MyModel=new platon::HypotesisMemModel(LocalEidos, this);
	tableViewHypotesis->setModel(MyModel);
	if(keep4delete!=NULL)
		delete keep4delete;

	if(tableViewHypotesis->model()->rowCount()>0)
		SetPragmaView(tableViewHypotesis->model()->index(0,0,QModelIndex()));
	else
		tableViewPragma->setModel(NULL);


}

void mainWin::SetPragmaView(const QModelIndex & HypModelindex)
{
	platon::PragmaMemModel* keep4delete=NULL;
	int row=HypModelindex.row();
	int col=0;
	long id_hypotesys=QVariant(tableViewHypotesis->model()->data(tableViewHypotesis->model()->index(row,col))).toInt();
	if(LocalHypotesis!=NULL)
	{
		delete LocalHypotesis;
		keep4delete=(platon::PragmaMemModel*)tableViewPragma->model();
	}
	LocalHypotesis=new platon::Hypotesis(this->LocalEidos, id_hypotesys);
	platon::PragmaMemModel* MyModel=new platon::PragmaMemModel(LocalHypotesis, this);
	tableViewPragma->setModel(MyModel);
	if(keep4delete!=NULL)
		delete keep4delete;
}

void mainWin::EditItem()
{
	if(CurrentObjectLevel==Level_Hypotesis)
	{
		int myrow=tableViewHypotesis->currentIndex().row();
		long id_hypotesys=QVariant(tableViewHypotesis->model()->data(tableViewHypotesis->model()->index(myrow,0,QModelIndex()))).toInt();
		platon::HypotesisEditForm * md=new platon::HypotesisEditForm(this,MyDB,id_hypotesys);
		md->setWindowTitle(tr("Редактирование объекта \"Тип\""));
		md->show();
	}
	if(CurrentObjectLevel==Level_Pragma)
	{
		int myrow=tableViewPragma->currentIndex().row();
		long id_pragma=QVariant(tableViewPragma->model()->data(tableViewPragma->model()->index(myrow,0,QModelIndex()))).toInt();
		platon::PragmaEditForm * md=new platon::PragmaEditForm(this,MyDB,id_pragma);
		md->setWindowTitle(tr("Редактирование объекта \"Экземпляр\""));
		md->show();
	}
	return ;
}
void mainWin::AddItem()
{
	QString Species=QString::fromStdString(LocalEidos->GetEidosSpecies());
	platon::Eidos* formEidos=new platon::Eidos(LocalEidos->DB,LocalEidos->GetID());
	platon::Hypotesis* formHypotesis;

	if(CurrentObjectLevel==Level_Hypotesis)
	{
		//Создаем экземпляры объектов для формы

		if(Species=="OBJ") formHypotesis= ((platon::OBJClass*)formEidos)->AddOBJType();
		if(Species=="ACT") formHypotesis= ((platon::ACTClass*)formEidos)->AddACTType();
		if(Species=="RES") formHypotesis= ((platon::RESClass*)formEidos)->AddRESType();
		if(Species=="NSI") formHypotesis=new platon::Hypotesis(formEidos,QString(tr("Новый тип нормативно-справочной информации")).toStdString());

		platon::HypotesisEditForm * md=new platon::HypotesisEditForm(this,formHypotesis);
		md->setWindowTitle(tr("Создание объекта \"Тип\""));
		md->show();
	}
	if(CurrentObjectLevel==Level_Pragma)
	{
		int myrow=tableViewHypotesis->currentIndex().row();
		long id_hypotesys=QVariant(tableViewHypotesis->model()->data(tableViewHypotesis->model()->index(myrow,0,QModelIndex()))).toInt();
		if(id_hypotesys>0)
		{
			formHypotesis=new platon::Hypotesis(formEidos,id_hypotesys);
			platon::Pragma*formPragma;

			//Создаем экземпляры объектов для формы
			if(Species=="OBJ") formPragma= ((platon::OBJType*)formHypotesis)->AddOBJCopy();
			//if(Species=="ACT") formHypotesis= ((platon::ACTType*)formHypotesis)->AddACTCopy(); //Необходимо выбрать на какой объект списываем
			//if(Species=="RES") formHypotesis= ((platon::RESType*)formHypotesis)->AddRESCopy(); //Необходимо выбрать на какое действие списываем
			//if(Species=="NSI") formPragma = new platon::Pragma(formHypotesis,platon::QDateTime2IBPPTimestamp(QDateTime::currentDateTime()));
			platon::PragmaEditForm * md=new platon::PragmaEditForm(this,formPragma);
			md->setWindowTitle(tr("Создание объекта \"Экземпляр\""));
			md->show();
		}
	}
	return ;
}
void mainWin::DeleteItem()
{
}
void mainWin::Showlinks()
{
	if(CurrentObjectLevel==Level_Hypotesis)
	{
		int myrow=tableViewHypotesis->currentIndex().row();
		long id_hypotesys=QVariant(tableViewHypotesis->model()->data(tableViewHypotesis->model()->index(myrow,0,QModelIndex()))).toInt();
		platon::LinksExplorer* LnkForm=new platon::LinksExplorer(this,MyDB,id_hypotesys,"ALL");
		LnkForm->show();
	}
	if(CurrentObjectLevel==Level_Pragma)
	{
		int myrow=tableViewPragma->currentIndex().row();
		long id_hypotesys=QVariant(tableViewPragma->model()->data(tableViewPragma->model()->index(myrow,0,QModelIndex()))).toInt();
		platon::LinksExplorer* LnkForm=new platon::LinksExplorer(this,MyDB,id_hypotesys,"ALL");
		LnkForm->show();
	}
}

void mainWin::RefreshViews()
{
//Процедура обновляет значения в окнах отображения гипотез и прагм
	if(CurrentObjectLevel==Level_Hypotesis) ((platon::AbstractMemHypModel*)this->tableViewHypotesis->model())->ReadToBuffer();
	if(CurrentObjectLevel==Level_Pragma) 	((platon::AbstractMemHypModel*)this->tableViewPragma->model())->ReadToBuffer();
}
void mainWin::BaseTimeShift()
{
//Процедура - слот производит перевод текущего системного времени в базе данных для корректировки выборок по темпоральным значениям
	//Проверяем попадает ли установка даты времени в диапазо +-5 минут от системного времени
	if(QDateTime::currentDateTime() < this->ProgramDateTime.addSecs(5*60) && QDateTime::currentDateTime().addSecs(5*60) > this->ProgramDateTime)
	{
		ProgramDateTime= QDateTime::currentDateTime();
	}
	platon::SetTimestampTemporalCompareFor(MyDB, platon::QDateTime2IBPPTimestamp(ProgramDateTime));
	this->statusbar->showMessage(tr("Программное время ")+ProgramDateTime.toString(tr("dd.MMMM.yyyy mm:ss")));
}
