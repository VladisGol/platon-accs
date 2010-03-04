#include "mainWin.h"
#include <QMessageBox>

mainWin::mainWin(QWidget *parent)
    : QMainWindow(parent)

{
		QTextCodec * codec = QTextCodec::codecForName("UTF-8"); 	//Устанавливаем кодировщик
		QTextCodec::setCodecForTr(codec);
		QTextCodec::setCodecForCStrings(codec);

		setupUi(this);								//Загружаем элементы формы

		this->MyDCl=new platon::DataClass(this);	//Создаем молуль данных приложения
		if(this->MyDCl->DB->Connected())			//Если соединение с базой данной при создании модуля прошло успешно
		{
			LocalEidos=NULL;
			LocalHypotesis=NULL;

			QObject::connect(EidosTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem* ,int)), this, SLOT(SetHypotesysView(QTreeWidgetItem*,int)));
			QObject::connect(EidosTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem* ,int)), this, SLOT(SetHypotesysView(QTreeWidgetItem*,int)));

			QObject::connect(tableViewHypotesis, SIGNAL(activated(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
			QObject::connect(tableViewHypotesis, SIGNAL(clicked(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));

			//Привязываем элементы управления к событиям
			QObject::connect(comboBox_Species, SIGNAL(currentIndexChanged(int)), this, SLOT(SetEidosView(int)));
			QObject::connect(action_edit, SIGNAL(activated()), this, SLOT(EditItem()));
			QObject::connect(action_add, SIGNAL(activated()), this, SLOT(AddItem()));
			QObject::connect(action_del, SIGNAL(activated()), this, SLOT(DeleteItem()));
			QObject::connect(action_refresh, SIGNAL(activated()), this, SLOT(RefreshViews()));
			QObject::connect(action_links, SIGNAL(activated()), this, SLOT(Showlinks()));
			QObject::connect(action_quit, SIGNAL(activated()), this, SLOT(CloseForm()));
			QObject::connect(action_AddFilter, SIGNAL(activated()), this, SLOT(AddFilter()));
			QObject::connect(action_RemoveFilter, SIGNAL(activated()), this, SLOT(RemoveFilter()));
			QObject::connect(action_View_IDs, SIGNAL(activated()), this, SLOT(ViewID_Activated()));

			SetEidosView(0);

			this->EidosTreeWidget->installEventFilter(this);
			this->tableViewHypotesis->installEventFilter(this);
			this->tableViewPragma->installEventFilter(this);
			CurrentObjectLevel=0;

			//Программное время
			this->MyDCl->ProgramDateTime = QDateTime::currentDateTime();
			DTBaseShifter=new QTimer(this);
			connect(DTBaseShifter, SIGNAL(timeout()), this, SLOT(BaseTimeShift()));
			BaseTimeShift();
			DTBaseShifter->start(10*1000);	//Устанавливаем время обновления

			//Выставляем прокси модели для возможности сортировки и фильтрования
			SFProxyModelH= new QSortFilterProxyModel(this);
			SFProxyModelP= new QSortFilterProxyModel(this);

			tableViewPragma->setModel(SFProxyModelP);
			tableViewPragma->setSortingEnabled(true);
			tableViewHypotesis->setModel(SFProxyModelH);
			tableViewHypotesis->setSortingEnabled(true);

			//Устанавливаем размеры виджетов на форме
			ReadFormWidgetsAppearance();
			DisableAllActions();
			//Инициализируем иконку фильтра
		    icon_filter.addPixmap(QPixmap(QString::fromUtf8((":/PICS/filter2.png"))), QIcon::Normal, QIcon::Off);
		}
}
void mainWin::DisableAllActions()
{
	//Процедура отключает любые действия на форме
	this->action_add->setEnabled(false);
	this->action_del->setEnabled(false);
	this->action_edit->setEnabled(false);
	this->action_links->setEnabled(false);
	this->action_refresh->setEnabled(false);
	this->action_AddFilter->setEnabled(false);
	this->action_RemoveFilter->setEnabled(false);
}

bool mainWin::eventFilter(QObject *obj, QEvent *event)
{

	if (obj == EidosTreeWidget)
	{
		if(event->type()==QEvent::FocusIn)
		{
			DisableAllActions();//Пока в форме не должны обрабатываться объекты Eidos
		}
	}
	if (obj == tableViewHypotesis)
	{
		if(event->type()==QEvent::FocusIn)
		{
			DisableAllActions();
			this->action_add->setEnabled(true);
			this->action_del->setEnabled(true);
			this->action_edit->setEnabled(true);
			this->action_links->setEnabled(true);
			this->action_refresh->setEnabled(true);
			this->action_AddFilter->setEnabled(true);
			if(SFProxyModelH->filterRegExp()!=QRegExp("")) this->action_RemoveFilter->setEnabled(true);
			CurrentObjectLevel=Level_Hypotesis;
		}
	}
	if (obj == tableViewPragma)
	{
		if(event->type()==QEvent::FocusIn)
		{
			DisableAllActions();
			QString Species=QString::fromStdString(LocalEidos->GetEidosSpecies());
			if(Species=="OBJ" || Species=="RES")
			{
				this->action_add->setEnabled(true);
				this->action_del->setEnabled(true);
				this->action_edit->setEnabled(true);
				this->action_links->setEnabled(true);
				this->action_refresh->setEnabled(true);
				this->action_AddFilter->setEnabled(true);
				if(SFProxyModelP->filterRegExp()!=QRegExp("")) this->action_RemoveFilter->setEnabled(true);
			}
			else
			{
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
		{
			EidosTreeWidget->SetSpecies("ALL");
			break;
		}
	case 1:
		{
			EidosTreeWidget->SetSpecies("OBJ");
			break;
		}
	case 2:
		{
			EidosTreeWidget->SetSpecies("ACT");
			break;
		}
	case 3:
		{
			EidosTreeWidget->SetSpecies("RES");
			break;
		}
	case 4:
		{
			EidosTreeWidget->SetSpecies("NSI");
		}
	}
	EidosTreeWidget->AttachToDB(this->MyDCl->DB);
}

void mainWin::SetHypotesysView(QTreeWidgetItem*CurItem , int Column)
{
	platon::Eidos* keep4delete=LocalEidos;


	long id_eidos;
	//Проверяем не впервые ли запущена программа
	if(CurItem==NULL || CurItem==0)
		id_eidos=1;	//Выводим root
	else
		id_eidos=CurItem->text(1).toLong();


	LocalEidos=new platon::Eidos(this->MyDCl->DB,id_eidos);
	platon::HypotesisMemModel* MyModel=new platon::HypotesisMemModel(LocalEidos, this);
	SFProxyModelH->setSourceModel(MyModel);
	this->tableViewHypotesis->resizeColumnsToContents();

	//Прячем фрейм для прагмы в случае если объект входит в ветку нормативно-справочной информации
	if(QString::fromStdString(LocalEidos->GetEidosSpecies())=="NSI")
		this->tableViewPragma->setEnabled(false);
	else
		this->tableViewPragma->setEnabled(true);

	//Инициируем смену значений списка прагм
	if(tableViewHypotesis->model()->rowCount()>0)
		SetPragmaView(tableViewHypotesis->model()->index(0,0,QModelIndex()));
	else
		SFProxyModelP->setSourceModel(NULL);

	if(keep4delete!=NULL) delete keep4delete;
	ViewID_Activated();	//Устанавливаем видимость ID
}

void mainWin::SetPragmaView(const QModelIndex & HypModelindex)
{
	platon::Hypotesis* keep4delete=LocalHypotesis;
	int row=tableViewHypotesis->currentIndex().row();
	if(row<0) row=0;	//не выбран элемент, значит выбираем самый первый
	long id_hypotesys=QVariant(tableViewHypotesis->model()->data(tableViewHypotesis->model()->index(row,0,QModelIndex()))).toInt();

	LocalHypotesis=new platon::Hypotesis(this->LocalEidos, id_hypotesys);
	platon::PragmaMemModel* MyModel=new platon::PragmaMemModel(LocalHypotesis, this);
	SFProxyModelP->setSourceModel(MyModel);
	this->tableViewPragma->resizeColumnsToContents();
	if(keep4delete!=NULL) delete keep4delete;
}

void mainWin::EditItem()
{
	if(CurrentObjectLevel==Level_Hypotesis)
	{
		int myrow=tableViewHypotesis->currentIndex().row();
		if(myrow>=0)	//Из существующих элементов в таблице (если таблица пустая или элемент не выбран, то myrow=-1)
		{
			long id_hypotesys=QVariant(tableViewHypotesis->model()->data(tableViewHypotesis->model()->index(myrow,0,QModelIndex()))).toInt();
			platon::HypotesisEditForm * md=new platon::HypotesisEditForm(this,id_hypotesys);
			md->setWindowTitle(tr("Редактирование объекта \"Тип\""));
			md->show();
		}
	}
	if(CurrentObjectLevel==Level_Pragma)
	{
		int myrow=tableViewPragma->currentIndex().row();
		if(myrow>=0)	//Из существующих элементов в таблице (если таблица пустая или элемент не выбран, то myrow=-1)
		{
			long id_pragma=QVariant(tableViewPragma->model()->data(tableViewPragma->model()->index(myrow,0,QModelIndex()))).toInt();
			platon::PragmaEditForm * md=new platon::PragmaEditForm(this,id_pragma);
			md->setWindowTitle(tr("Редактирование объекта \"Экземпляр\""));
			md->show();
		}
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
		if(myrow==-1) myrow=0;	//Это на случай если сразу перешагнули в окно Pragma без выбора bp Hypotesis
		long id_hypotesys=QVariant(tableViewHypotesis->model()->data(tableViewHypotesis->model()->index(myrow,0,QModelIndex()))).toInt();
		if(id_hypotesys>0)
		{
			formHypotesis=new platon::Hypotesis(formEidos,id_hypotesys);
			platon::Pragma*formPragma;

			//Создаем экземпляры объектов для формы
			if(Species=="OBJ") formPragma= ((platon::OBJType*)formHypotesis)->AddOBJCopy();
			if(Species=="ACT")
				{
					; //Необходимо выбрать с каким объектом проводится действие
				}
			if(Species=="RES") formPragma= ((platon::RESType*)formHypotesis)->AddRESCopy(); //Создание ресурса, который размещается "на складе"
			if(Species=="NSI")	;//Не может быть экземпляров на указанной ветви
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
		platon::LinksExplorer* LnkForm=new platon::LinksExplorer(this,id_hypotesys,"ALL");
		LnkForm->show();
	}
	if(CurrentObjectLevel==Level_Pragma)
	{
		int myrow=tableViewPragma->currentIndex().row();
		long id_hypotesys=QVariant(tableViewPragma->model()->data(tableViewPragma->model()->index(myrow,0,QModelIndex()))).toInt();
		platon::LinksExplorer* LnkForm=new platon::LinksExplorer(this,id_hypotesys,"ALL");
		LnkForm->show();
	}
}

void mainWin::RefreshViews()
{
//Процедура обновляет значения в окнах отображения гипотез и прагм
	if(CurrentObjectLevel==Level_Hypotesis) RefreshHView();
	if(CurrentObjectLevel==Level_Pragma) RefreshPView();

}
void mainWin::RefreshHView()
{
	SetHypotesysView(this->EidosTreeWidget->currentItem(),0);
}
void mainWin::RefreshPView()
{
	SetPragmaView(tableViewHypotesis->currentIndex());
}


void mainWin::BaseTimeShift()
{
//Процедура - слот производит перевод текущего системного времени в базе данных для корректировки выборок по темпоральным значениям
	this->MyDCl->SetProgramDateTime();
	this->statusbar->showMessage(tr("Программное время ")+MyDCl->ProgramDateTime.toString(tr("dd.MMMM.yyyy hh:mm:ss")));
}
void mainWin::CloseForm()
{
	//Слот закрытия формы. Перед закрытием формы сохраняем параметры формы
	WriteFormWidgetsAppearance();
	this->close();
}
void mainWin::AddFilter()
{
	QString FieldCaption;
	int col;

	if(CurrentObjectLevel==Level_Hypotesis)
	{
		col=tableViewHypotesis->currentIndex().column();
		FieldCaption = tableViewHypotesis->model()->headerData(col,Qt::Horizontal,Qt::DisplayRole).toString();
	}
	if(CurrentObjectLevel==Level_Pragma)
	{
		col=tableViewPragma->currentIndex().column();
		FieldCaption = tableViewPragma->model()->headerData(col,Qt::Horizontal,Qt::DisplayRole).toString();
	}
	//Получим текущее значение фильтра для отображения
	QString CurrentFilterPattern="";
	if(SFProxyModelH->filterRegExp()!=QRegExp("") && SFProxyModelH->filterKeyColumn()==col)
		CurrentFilterPattern=SFProxyModelH->filterRegExp().pattern ();
	if(SFProxyModelP->filterRegExp()!=QRegExp("") && SFProxyModelP->filterKeyColumn()==col)
		CurrentFilterPattern=SFProxyModelP->filterRegExp().pattern ();

	//Запросим в диалоговом окне строку фильрации
	bool ok;
    QString textExp = QInputDialog::getText(this, tr("Введите условие фильтра"),	tr("Для поля:")+FieldCaption,QLineEdit::Normal,CurrentFilterPattern,&ok);
    //Установим значение фильтра если пользователь ввел значение фильрации и нажал кнопку Ok в диалоге
	if (ok && !textExp.isEmpty())
	{
		if(CurrentObjectLevel==Level_Hypotesis)
		{
			SFProxyModelH->setFilterRegExp(QRegExp(textExp));
			SFProxyModelH->setFilterKeyColumn(col);
			tableViewHypotesis->setToolTip(tr("Условие фильтра для поля ") +FieldCaption+":"+textExp);

			platon::AbstractMemHypModel* CurModel=(platon::AbstractMemHypModel*)SFProxyModelH->sourceModel();
			CurModel->setHeaderIcon(col, Qt::Horizontal,icon_filter);

		}
		if(CurrentObjectLevel==Level_Pragma)
		{
			SFProxyModelP->setFilterRegExp(QRegExp(textExp));
			SFProxyModelP->setFilterKeyColumn(col);
			tableViewPragma->setToolTip(tr("Условие фильтра для поля ") +FieldCaption+":"+textExp);

			platon::AbstractMemHypModel* CurModel=(platon::AbstractMemHypModel*)SFProxyModelP->sourceModel();
			CurModel->setHeaderIcon(col,Qt::Horizontal,icon_filter);

		}
	}
}
void mainWin::RemoveFilter()
{
	if(CurrentObjectLevel==Level_Hypotesis)
	{
		platon::AbstractMemHypModel* CurModel=(platon::AbstractMemHypModel*)SFProxyModelH->sourceModel();
		CurModel->RemoveHeaderIcon(SFProxyModelH->filterKeyColumn(),Qt::Horizontal);

		SFProxyModelH->setFilterRegExp(QRegExp(""));
		SFProxyModelH->setFilterKeyColumn(-1);
		tableViewHypotesis->setToolTip("");
	}
	if(CurrentObjectLevel==Level_Pragma)
	{
		platon::AbstractMemHypModel* CurModel=(platon::AbstractMemHypModel*)SFProxyModelP->sourceModel();
		CurModel->RemoveHeaderIcon(SFProxyModelP->filterKeyColumn(),Qt::Horizontal);

		SFProxyModelP->setFilterRegExp(QRegExp(""));
		SFProxyModelP->setFilterKeyColumn(-1);
		tableViewPragma->setToolTip("");
	}
	this->action_RemoveFilter->setEnabled(false);
}
void mainWin::ReadFormWidgetsAppearance()
{
	//Процедура считывает из DbETC параметры элементов формы и устанавливает их значения
	platon::DbEtc* MyETC=new platon::DbEtc(this->MyDCl->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName ()).toStdString(),true,-1);
	int w=874,h=744;
	if(MyETC->ParamExists("width")) w=MyETC->ReadInteger("width");
	if(MyETC->ParamExists("height")) h=MyETC->ReadInteger("height");
	this->resize (w,h);

	QList<int> vals;
	if(MyETC->ParamExists("splitter_e\\0")) vals.append(MyETC->ReadInteger("splitter_e\\0")); else vals.append(350);
	if(MyETC->ParamExists("splitter_e\\1")) vals.append(MyETC->ReadInteger("splitter_e\\1")); else vals.append(762);
	splitter_e->setSizes(vals);
	vals.clear();
	if(MyETC->ParamExists("splitter_hp\\0")) vals.append(MyETC->ReadInteger("splitter_hp\\0")); else vals.append(361);
	if(MyETC->ParamExists("splitter_hp\\1")) vals.append(MyETC->ReadInteger("splitter_hp\\1")); else vals.append(362);
	splitter_hp->setSizes(vals);

	if(MyETC->ParamExists("comboBox_Species")) this->comboBox_Species->setCurrentIndex(MyETC->ReadInteger("comboBox_Species"));
	if(MyETC->ParamExists("EidosID")) this->EidosTreeWidget->findNMakeCurrent(MyETC->ReadInteger("EidosID"));
	SetHypotesysView(this->EidosTreeWidget->currentItem(),0);

	if(MyETC->ParamExists("action_View_IDs")) this->action_View_IDs->setChecked(MyETC->ReadBool("action_View_IDs"));
	ViewID_Activated();


	MyETC->CloseKey();
	delete MyETC;

}
void mainWin::WriteFormWidgetsAppearance()
{
	//Процедура записывает в DbETC параметры элементов формы
	platon::DbEtc* MyETC=new platon::DbEtc(this->MyDCl->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName ()).toStdString(),true,-1);
	MyETC->WriteInteger("width", this->width());
	MyETC->WriteInteger("height", this->height());

	QList<int> vals = this->splitter_e->sizes();
	MyETC->WriteInteger("splitter_e\\0", vals.at(0));
	MyETC->WriteInteger("splitter_e\\1", vals.at(1));
	vals=this->splitter_hp->sizes();
	MyETC->WriteInteger("splitter_hp\\0", vals.at(0));
	MyETC->WriteInteger("splitter_hp\\1", vals.at(1));

	MyETC->WriteInteger("comboBox_Species", this->comboBox_Species->currentIndex());
	MyETC->WriteInteger("EidosID",this->EidosTreeWidget->GetEidosID());
	MyETC->WriteBool("action_View_IDs",this->action_View_IDs->isChecked());

	MyETC->CloseKey();
	delete MyETC;

}
void mainWin::ViewID_Activated()
{
	//Слот для изменения отображения идентификаторов объектов в программе
	this->MyDCl->ViewIDs =action_View_IDs->isChecked();//Устанавливаем значение переменной в Датамодуле
	if(!this->MyDCl->ViewIDs)	//Отметка снята
	{

		for (int i=0;i<tableViewHypotesis->model()->columnCount(QModelIndex());i++)
		{
			QString FieldCaption = tableViewHypotesis->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				tableViewHypotesis->hideColumn(i);
		}
		for (int i=0;i<tableViewPragma->model()->columnCount(QModelIndex());i++)
			{
				QString FieldCaption = tableViewPragma->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
				if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
					tableViewPragma->hideColumn(i);
			}
		for (int i=0;i<EidosTreeWidget->columnCount();i++)
			{
				QString FieldCaption = EidosTreeWidget->headerItem()->text(i);
				if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
					EidosTreeWidget->hideColumn(i);
			}
	}
	else								//Отметка в меню установлена
	{
		for (int i=0;i<tableViewHypotesis->model()->columnCount(QModelIndex());i++)
		{
			QString FieldCaption = tableViewHypotesis->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				tableViewHypotesis->showColumn(i);
		}
		for (int i=0;i<tableViewPragma->model()->columnCount(QModelIndex());i++)
			{
				QString FieldCaption = tableViewPragma->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
				if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
					tableViewPragma->showColumn(i);
			}
		for (int i=0;i<EidosTreeWidget->columnCount();i++)
			{
				QString FieldCaption = EidosTreeWidget->headerItem()->text(i);
				if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
					EidosTreeWidget->showColumn(i);
			}
	}
}


