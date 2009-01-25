#include "LinksExplorer.h"

namespace platon
{
LinksExplorer::LinksExplorer(QWidget * parent, IBPP::Database InDB, long ID_in, QString InSpecies): QMainWindow(parent)
{
	setupUi(this);

	this->DB=InDB;

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
	QObject::connect(PEidosTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem* ,int)), this, SLOT(SetPGridView(QTreeWidgetItem*,int)));

//	QObject::connect(action_Cancel, SIGNAL(activated()), this, SLOT(ExitByCancel()));
//	QObject::connect(action_saveNexit, SIGNAL(activated()), this, SLOT(ExitWithSave()));

    PaintingEidos(this->HEidosTreeWidget,IEidosH);
	PaintingEidos(this->PEidosTreeWidget,IEidosP);

	ReadFormWidgetsAppearance();

}

void LinksExplorer::ExitWithSave()
{
	WriteFormWidgetsAppearance();
	this->close();
}
void LinksExplorer::ExitByCancel()
{
	this->close();
}
LinksExplorer::~LinksExplorer()
{
	if(LocalEidosH!=NULL) delete LocalEidosH;
	if(LocalEidosP!=NULL) delete LocalEidosP;
	delete LocalHypotesis;
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
				long idparent =	OneItem->text(2).toInt();
				if(idparent<=0) break;
				OneItem=FindEidosByID(EidosTreeWidget, idparent);
			}
			iter->Next();
		}
	}
}
QTreeWidgetItem * LinksExplorer::FindEidosByID(QTreeWidget* EidosTreeWidget, long ID)
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
	long id_eidos=CurItem->text(1).toLong();

	keep4delete=LocalEidosH;

	LocalEidosH=new platon::Eidos(DB,id_eidos);

	platon::LnkdHypMemModel* MyModel=new platon::LnkdHypMemModel(LocalEidosH,IDFor, this);
	tableView_Hyp->setModel(MyModel);

	if(keep4delete!=NULL) delete keep4delete;
}

void LinksExplorer::SetPGridView(QTreeWidgetItem*CurItem , int Column)
{
	platon::Eidos* keep4delete;
	long id_eidos=CurItem->text(1).toLong();

	keep4delete=LocalEidosP;

	LocalEidosP=new platon::Eidos(DB,id_eidos);

	platon::LnkdHypPragmaMemModel* MyModel=new platon::LnkdHypPragmaMemModel(LocalEidosP,IDFor, this);
	tableView_Pragma->setModel(MyModel);

	if(keep4delete!=NULL) delete keep4delete;
}

void LinksExplorer::ReadFormWidgetsAppearance()
{
	//Процедура считывает из DbETC параметры элементов формы и устанавливает их значения
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);

	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName()).toStdString(),true,-1);
	int w=800,h=700;
	if(MyETC->ParamExists("width")) w=MyETC->ReadInteger("width");
	if(MyETC->ParamExists("height")) h=MyETC->ReadInteger("height");
	this->resize (w,h);

	MyETC->CloseKey();
	delete MyETC;
}

void LinksExplorer::WriteFormWidgetsAppearance()
{
	//Процедура записывает в DbETC параметры элементов формы
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName()).toStdString(),true,-1);
	MyETC->WriteInteger("width", this->width());
	MyETC->WriteInteger("height", this->height());

	MyETC->CloseKey();
	delete MyETC;
}



}
