#include "LinksExplorer.h"

namespace platon
{
LinksExplorer::LinksExplorer(QWidget * parent, int ID_in, QString InSpecies): QMainWindow(parent)
{
	setupUi(this);

    DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;
	IsViewID=DTL->ViewIDs;

	LocalEidosH=NULL;
	LocalEidosP=NULL;

	IEidosH=new iterLNKS_HEidos(DB,ID_in);
	IEidosP=new iterLNKS_PEidos(DB,ID_in);

	IHyp= new iterLNKS_Hyp(DB);
	IPragma=new iterLNKS_Pragma(DB);

	IDFor=ID_in;

	HEidosTreeWidget->SetSpecies("ALL");
	HEidosTreeWidget->AttachToDB(DB);
	PEidosTreeWidget->SetSpecies("ALL");
	PEidosTreeWidget->AttachToDB(DB);


	QObject::connect(HEidosTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem* ,int)), this, SLOT(SetHGridView(QTreeWidgetItem*,int)));
    QObject::connect(HEidosTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem* ,int)), this, SLOT(SetHGridView(QTreeWidgetItem*,int)));
	QObject::connect(PEidosTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem* ,int)), this, SLOT(SetPGridView(QTreeWidgetItem*,int)));
    QObject::connect(PEidosTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem* ,int)), this, SLOT(SetPGridView(QTreeWidgetItem*,int)));
	QObject::connect(action_close, SIGNAL(activated()), this, SLOT(Exit()));

	QObject::connect(tableView_Hyp, SIGNAL(activated(QModelIndex)), this, SLOT(SetCorespActions()));
	QObject::connect(tableView_Pragma, SIGNAL(activated(QModelIndex)), this, SLOT(SetCorespActions()));

	QObject::connect(action_HypOpen, SIGNAL(activated()), this, SLOT(OpenHypotesis()));
	QObject::connect(action_OBJOpen, SIGNAL(activated()), this, SLOT(OpenPragma()));
	QObject::connect(action_ACTOpen, SIGNAL(activated()), this, SLOT(OpenPragma()));
	QObject::connect(action_RESOpen, SIGNAL(activated()), this, SLOT(OpenPragma()));
    QObject::connect(action_copy, SIGNAL(activated()), this, SLOT(slotCopySelectedFromView()));  //Слот копирования в буфер обмена

	this->PEidosTreeWidget->installEventFilter(this);
	this->HEidosTreeWidget->installEventFilter(this);


	PaintingEidos(this->HEidosTreeWidget,IEidosH);
	PaintingEidos(this->PEidosTreeWidget,IEidosP);

	if(IEidosH->GetRowCount()>IEidosP->GetRowCount())
		tabWidget_Prg->setCurrentIndex(0);//Активизируем вкладку со списком гипотез
	else
		tabWidget_Prg->setCurrentIndex(1);//Активизируем вкладку со списком прагм


        //Контекстные меню реализация по описанию http://www.prog.org.ru/topic_10094_0.html, спасибо "Павлик"
        ContextMenuHyp=new QMenu(this);
        ContextMenuPragma=new QMenu(this);

        ContextMenuHyp->addAction(action_HypOpen);
        ContextMenuHyp->addAction(action_OBJOpen);
        ContextMenuHyp->addAction(action_ACTOpen);
        ContextMenuHyp->addAction(action_RESOpen);
        ContextMenuHyp->addSeparator();
        ContextMenuHyp->addAction(action_copy);

        ContextMenuPragma->addAction(action_HypOpen);
        ContextMenuPragma->addAction(action_OBJOpen);
        ContextMenuPragma->addAction(action_ACTOpen);
        ContextMenuPragma->addAction(action_RESOpen);
        ContextMenuPragma->addSeparator();
        ContextMenuPragma->addAction(action_copy);

        this->tableView_Hyp->setContextMenuPolicy(Qt::CustomContextMenu);
        this->tableView_Pragma->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(this->tableView_Hyp, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotHypCntxMenu(QPoint)));
        connect(this->tableView_Pragma, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotPragmaCntxMenu(QPoint)));


	ClearCorespActions();
	SetViewID();
	ReadFormWidgetsAppearance();

}

void LinksExplorer::slotHypCntxMenu(const QPoint &point)      //Слот для реализации контекстного меню в Hypotesis
{
    ContextMenuHyp->popup(this->tableView_Hyp->mapToGlobal(point));
}
void LinksExplorer::slotPragmaCntxMenu(const QPoint &point)      //Слот для реализации контекстного меню в Pragma
{
    ContextMenuPragma->popup(this->tableView_Pragma->mapToGlobal(point));
}

void LinksExplorer::slotCopySelectedFromView()
{
    //Процедура копирует в буфер обмена текущее выделение из текущего грида в HTML формате.

    QTableView *CurrentTableView=NULL;

    if(this->tabWidget_Prg->currentIndex()==0)  //Гипотезы
        CurrentTableView=this->tableView_Hyp;
    if(this->tabWidget_Prg->currentIndex()==1)  //Прагмы
        CurrentTableView=this->tableView_Pragma;
    if(CurrentTableView==NULL)
        return;

    QModelIndexList list =  CurrentTableView->selectionModel()->selectedIndexes();

    if( list.isEmpty()) return;


    int firstRow, lastRow, rowCount;
    firstRow = list.first().row();
    lastRow = list.last().row();
    rowCount = lastRow - firstRow+1;

    QString html_string="<table>";

    for(int i = 0; i < rowCount; i++)
    {
        html_string +="<tr>";
        for(int j = i; j < list.count(); j += rowCount)
        {
            html_string += "<td>";
            html_string += CurrentTableView->model()->data(list[j], Qt::DisplayRole).toString();
            html_string += "</td>";
        }
        html_string +="</tr>";
    }
    html_string +="</table>";

    QMimeData* md= new QMimeData();
    md->setHtml(html_string);
    QApplication::clipboard()->setMimeData(md);

}

bool LinksExplorer::eventFilter(QObject *obj, QEvent *event)
{
	if(event->type()==QEvent::FocusIn)
	{
		ClearCorespActions();
	}
	return false;
}

void LinksExplorer::ClearCorespActions()
{
	action_HypOpen->setVisible(false);
	action_OBJOpen->setVisible(false);
	action_ACTOpen->setVisible(false);
	action_RESOpen->setVisible(false);
}

void LinksExplorer::SetCorespActions()
{
	ClearCorespActions();
	if(tabWidget_Prg->currentIndex()==0)
	{
		action_HypOpen->setVisible(true);
	}
	else
	{
		int id_eidos= PEidosTreeWidget->currentItem ()->text(1).toInt();
		QString Species=QString::fromStdString(platon::GetEidosSpecies(this->DB,id_eidos));
		if(Species=="OBJ")action_OBJOpen->setVisible(true);
		if(Species=="ACT")action_ACTOpen->setVisible(true);
		if(Species=="RES")action_RESOpen->setVisible(true);
	}
}

void LinksExplorer::OpenHypotesis()
{
	//Открыть окно с гипотезой
	int myrow=tableView_Hyp->currentIndex().row();
	int id_hypotesys=QVariant(tableView_Hyp->model()->data(tableView_Hyp->model()->index(myrow,0,QModelIndex()))).toInt();
	platon::HypotesisEditForm * md=new platon::HypotesisEditForm(this,id_hypotesys);
	md->setWindowTitle(tr("Редактирование объекта \"Тип\""));
        md->setAttribute(Qt::WA_ShowModal, true);
	md->show();
}
void LinksExplorer::OpenPragma()
{
	//Открыть окно с прагмой
	int myrow=tableView_Pragma->currentIndex().row();
	int id_pragma=QVariant(tableView_Pragma->model()->data(tableView_Pragma->model()->index(myrow,0,QModelIndex()))).toInt();
	platon::PragmaEditForm * md=new platon::PragmaEditForm(this,id_pragma);
	md->setWindowTitle(tr("Редактирование объекта \"Экземпляр\""));
        md->setAttribute(Qt::WA_ShowModal, true);
	md->show();
}

void LinksExplorer::Exit()
{
	WriteFormWidgetsAppearance();
	this->close();
}

LinksExplorer::~LinksExplorer()
{
        if(LocalEidosH!=NULL) delete LocalEidosH;
        if(LocalEidosP!=NULL) delete LocalEidosP;
}
void LinksExplorer::PaintingEidos(QTreeWidget* EidosTreeWidget,pIterator * iter)
{
//Процедура раскрашивает элементы эйдосов
	//Сначала все элементы устанавливаем в выключенное состояние
	QTreeWidgetItemIterator it(EidosTreeWidget, QTreeWidgetItemIterator::All);
	while (*it)
	{
		QFont fn=(*it)->font(0);
		fn.setBold(false);
		(*it)->setFont(0,fn);
		(*it)->setExpanded(false);
	    ++it;
	}
	//После чего устанавливаем в раскрытое состояние и подкрашиваем его BOLD-ом
	iter->First();
	while(iter->Fetched())
	{
		QTreeWidgetItem * OneItem=FindEidosByID(EidosTreeWidget, iter->GetID());
		if(OneItem!=NULL)					//Если найдено значение
		{
			QFont fn=OneItem->font(0);
			fn.setBold(true);
			OneItem->setFont(0,fn);			//Устанавливаем элемент в BOLD

			while(true)
			{
				OneItem->setExpanded(true);	//Раскрываем все предшествующие элементы списка
				int idparent =	OneItem->text(2).toInt();
				if(idparent<=0) break;
				OneItem=FindEidosByID(EidosTreeWidget, idparent);                                
			}			
		}
                iter->Next();
	}
}
QTreeWidgetItem * LinksExplorer::FindEidosByID(QTreeWidget* EidosTreeWidget, int ID)
{
	QList<QTreeWidgetItem *> FoundedItem = EidosTreeWidget->findItems (QString::number(ID), Qt::MatchExactly | Qt::MatchRecursive,1 );
	if(FoundedItem.count()>0)      //Найден искомый элемент
	{
		return FoundedItem.at(0);
	}
	return NULL;
}

void LinksExplorer::SetHGridView(QTreeWidgetItem*CurItem , int Column)
{
	platon::Eidos* keep4delete;
	int id_eidos=CurItem->text(1).toInt();

	keep4delete=LocalEidosH;

	LocalEidosH=new platon::Eidos(DB,id_eidos);

	platon::LnkdHypMemModel* MyModel=new platon::LnkdHypMemModel(LocalEidosH,IDFor, this);
	tableView_Hyp->setModel(MyModel);
	tableView_Hyp->resizeColumnsToContents();

	if(keep4delete!=NULL) delete keep4delete;
	SetViewID();
}

void LinksExplorer::SetPGridView(QTreeWidgetItem*CurItem , int Column)
{
	platon::Eidos* keep4delete;
	int id_eidos=CurItem->text(1).toInt();

	keep4delete=LocalEidosP;

	LocalEidosP=new platon::Eidos(DB,id_eidos);

	platon::LnkdHypPragmaMemModel* MyModel=new platon::LnkdHypPragmaMemModel(LocalEidosP,IDFor, this);
	tableView_Pragma->setModel(MyModel);
	tableView_Pragma->resizeColumnsToContents();

	if(keep4delete!=NULL) delete keep4delete;
	SetViewID();
}

void LinksExplorer::ReadFormWidgetsAppearance()
{
	//Процедура считывает из DbETC параметры элементов формы и устанавливает их значения

    DTL->ETC_OpenKey(QString("FormsAppearance\\"+this->objectName()));
	int w=800,h=700;
    if(DTL->ETC_ParamExists("width")) w=DTL->ETC_ReadInteger("width");
    if(DTL->ETC_ParamExists("height")) h=DTL->ETC_ReadInteger("height");
	this->resize (w,h);
    DTL->ETC_CloseKey();

}

void LinksExplorer::WriteFormWidgetsAppearance()
{
	//Процедура записывает в DbETC параметры элементов формы
    DTL->ETC_OpenKey(QString("FormsAppearance\\"+this->objectName()));
    DTL->ETC_WriteInteger("width", this->width());
    DTL->ETC_WriteInteger("height", this->height());
    DTL->ETC_CloseKey();

}

void LinksExplorer::SetViewID()
{
	//Процедура скрывает или открывает поля идентификаторов объектов
	if(!IsViewID)	//Отметка снята
	{
		for (int i=0;i<HEidosTreeWidget->columnCount();i++)
		{
			QString FieldCaption = HEidosTreeWidget->headerItem()->text(i);
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				HEidosTreeWidget->hideColumn(i);
		}
		for (int i=0;i<PEidosTreeWidget->columnCount();i++)
		{
			QString FieldCaption = PEidosTreeWidget->headerItem()->text(i);
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				PEidosTreeWidget->hideColumn(i);
		}
		if(tableView_Hyp->model()!=0)	//Если модель установлена
		for (int i=0;i<tableView_Hyp->model()->columnCount(QModelIndex());i++)
		{
			QString FieldCaption = tableView_Hyp->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				tableView_Hyp->hideColumn(i);
		}
		if(tableView_Pragma->model()!=0)
		for (int i=0;i<tableView_Pragma->model()->columnCount(QModelIndex());i++)
		{
			QString FieldCaption = tableView_Pragma->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				tableView_Pragma->hideColumn(i);
		}
	}
	else								//Отметка в меню установлена
	{
		for (int i=0;i<HEidosTreeWidget->columnCount();i++)
		{
			QString FieldCaption = HEidosTreeWidget->headerItem()->text(i);
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				HEidosTreeWidget->showColumn(i);
		}
		for (int i=0;i<PEidosTreeWidget->columnCount();i++)
		{
			QString FieldCaption = PEidosTreeWidget->headerItem()->text(i);
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				PEidosTreeWidget->showColumn(i);
		}
		if(tableView_Hyp->model()!=0)
		for (int i=0;i<tableView_Hyp->model()->columnCount(QModelIndex());i++)
		{
			QString FieldCaption = tableView_Hyp->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				tableView_Hyp->showColumn(i);
		}
		if(tableView_Pragma->model()!=0)
		for (int i=0;i<tableView_Pragma->model()->columnCount(QModelIndex());i++)
		{
			QString FieldCaption = tableView_Pragma->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				tableView_Pragma->showColumn(i);
		}

	}
}

}
