#include "LinksExplorer.h"

namespace platon
{
LinksExplorer::LinksExplorer(QWidget * parent, IBPP::Database InDB, long ID_Hypotesys): QMainWindow(parent)
{
	setupUi(this);

	this->DB=InDB;
	long EidosID,HypotesysID;
	Hypotesis::GetEidosHypotesisIDS(DB, ID_Hypotesys,EidosID,HypotesysID);
	if(EidosID>0)	//Проверяем найдены ли среди гипотез заданная в параметре
	{
		LH=new LinkedHypotesys(DB,ID_Hypotesys);

		EidosTreeWidget->SetSpecies("ALL");
		EidosTreeWidget->AttachToDB(DB);

		PaintingEidos();
	}
	else throw "Объект на который указывает идентификатор из параметра не существует";


	QObject::connect(EidosTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem* ,int)), this, SLOT(SetHypotesysView(QTreeWidgetItem*,int)));
//	QObject::connect(action_Cancel, SIGNAL(activated()), this, SLOT(ExitByCancel()));
//	QObject::connect(action_saveNexit, SIGNAL(activated()), this, SLOT(ExitWithSave()));
}

void LinksExplorer::ExitWithSave()
{
	this->LocalHypotesis->TransactionIBPP->Commit();
	this->close();
}
void LinksExplorer::ExitByCancel()
{
	this->LocalHypotesis->TransactionIBPP->Rollback();
	this->close();
}
LinksExplorer::~LinksExplorer()
{
	delete LocalEidos;
	delete LocalHypotesis;
}
void LinksExplorer::PaintingEidos()
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
	LH->LEidos->First();
	while(LH->LEidos->Fetched())
	{
		QTreeWidgetItem * OneItem=FindEidosByID(LH->LEidos->GetID());

		QFont fn=OneItem->font(0);
		fn.setBold(true);
		OneItem->setFont(0,fn);

		while(true)
		{
			OneItem->setExpanded(true);
			long idparent =	OneItem->text(2).toInt();
			if(idparent<=0) break;
			OneItem=FindEidosByID(idparent);
		}
		LH->LEidos->Next();
	}
}
QTreeWidgetItem * LinksExplorer::FindEidosByID(long ID)
{
	QList<QTreeWidgetItem *> FoundedItem = EidosTreeWidget->findItems (QString::number(ID), Qt::MatchExactly | Qt::MatchRecursive,1 );
	if(FoundedItem.count()>0)      //Найден искомый элемент
	{
		return FoundedItem.at(0);
	}
	return NULL;
}

void LinksExplorer::SetHypotesysView(QTreeWidgetItem*CurItem , int Column)
{
	platon::LnkdHypMemModel* keep4delete=NULL;
	long id_eidos=CurItem->text(1).toLong();

	if(LocalEidos!=NULL)
	{
		delete LocalEidos;
		keep4delete=(platon::LnkdHypMemModel*)tableView_Hyp->model();
	}

	LocalEidos=new platon::Eidos(DB,id_eidos);
	LH->LHyp->MasterChanged(id_eidos,LH->IDfor);
	platon::LnkdHypMemModel* MyModel=new platon::LnkdHypMemModel(LocalEidos,LH->LHyp, this);
	tableView_Hyp->setModel(MyModel);
	if(keep4delete!=NULL)
		delete keep4delete;
}

}
