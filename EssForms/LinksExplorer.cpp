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

	QObject::connect(EidosTreeView_Hyp, SIGNAL(activated(QModelIndex)), this, SLOT(SetHGridView(QModelIndex)));
	QObject::connect(EidosTreeView_Pragma, SIGNAL(activated(QModelIndex)), this, SLOT(SetPGridView(QModelIndex)));

	QObject::connect(action_Close, SIGNAL(activated()), this, SLOT(close()));

	EidosMemModel* MyEidosModel=new EidosMemModel(InDB,QString("ALL"),(QObject*)this);

	EidosTreeView_Pragma->setModel(MyEidosModel);

	SFProxyModelH= new QSortFilterProxyModel(this);
	SFProxyModelP= new QSortFilterProxyModel(this);

	SFProxyModelH-> setSourceModel(MyEidosModel);
	SFProxyModelH-> setDynamicSortFilter(true);
	EidosTreeView_Hyp->setModel(SFProxyModelH);
	EidosTreeView_Hyp->setSortingEnabled (true);

//	SFProxyModelH->setFilterKeyColumn (1);
//	SFProxyModelH->setFilterRegExp("9");


	SFProxyModelP-> setSourceModel(MyEidosModel);
	EidosTreeView_Pragma->setModel(SFProxyModelP);
	EidosTreeView_Pragma->setSortingEnabled (true);
	UnBoldTreeView(EidosTreeView_Hyp, EidosTreeView_Hyp->rootIndex());
}

LinksExplorer::~LinksExplorer()
{
	if(LocalEidosH!=NULL) delete LocalEidosH;
	if(LocalEidosP!=NULL) delete LocalEidosP;

}
void LinksExplorer::UnBoldTreeView(QTreeView *TreeView,const QModelIndex& TopLevel)
{
	for(int i=0;i<TreeView->model()->rowCount(TopLevel);i++)
	{
		QModelIndex currIndex=TreeView->model()->index(i,0,TopLevel);

		QIcon icon1;
		icon1.addPixmap(QPixmap(QString::fromUtf8((":/PICS/exec.png"))), QIcon::Normal, QIcon::Off);

		TreeView->model()->setData(currIndex,icon1,Qt::DecorationRole);

		std::cout<<currIndex.row()<<" x "<<currIndex.column()<<"\n";
		std::cout<<currIndex.data(0).toString().toStdString()<<"\n";

		//QFont fn=
		//std::cout<<TreeView->indexWidget() isWidgetType()


		//fn.setBold(true);
		//TreeView->indexWidget(currIndex)->setFont(fn);

		if(TreeView->model()->hasChildren(currIndex))
			UnBoldTreeView(TreeView,currIndex);

	}

	/*QFont fn=TreeView->indexWidget(TopLevel)->font();
	fn.setBold(true);
	TreeView->indexWidget(TopLevel)->setFont(fn);
	if(TopLevel.child(0,0).isValid())
		UnBoldTreeView(TreeView,TopLevel.child(0,0));
	else
	{
		if(TopLevel.sibling(TopLevel.row()+1,TopLevel.column()).isValid())
			UnBoldTreeView(TreeView,TopLevel.sibling(TopLevel.row()+1,TopLevel.column()));
	}
	*/
}
void LinksExplorer::PaintingEidos(QTreeView *TreeView,pIterator * iter)
{
//Процедура раскрашивает элементы эйдосов
	//Сначала все элементы устанавливаем в выключенное состояние

	UnBoldTreeView(TreeView, TreeView->rootIndex());
	/*
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
	*/
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

void LinksExplorer::SetHGridView(const QModelIndex& CurItem)
{

	platon::Eidos* keep4delete=LocalEidosH;
	long id_eidos=CurItem.sibling(CurItem.row(),1).data(Qt::DisplayRole).toInt();

	LocalEidosH=new platon::Eidos(DB,id_eidos);

	platon::LnkdHypMemModel* MyModel=new platon::LnkdHypMemModel(LocalEidosH,IDFor, this);
	tableView_Hyp->setModel(MyModel);

	if(keep4delete!=NULL) delete keep4delete;
}

void LinksExplorer::SetPGridView(const QModelIndex& CurItem)
{
	platon::Eidos* keep4delete=LocalEidosP;
	long id_eidos=CurItem.sibling(CurItem.row(),1).data(Qt::DisplayRole).toInt();

	LocalEidosP=new platon::Eidos(DB,id_eidos);

	platon::LnkdHypPragmaMemModel* MyModel=new platon::LnkdHypPragmaMemModel(LocalEidosP,IDFor, this);
	tableView_Pragma->setModel(MyModel);

	if(keep4delete!=NULL) delete keep4delete;
}

}
