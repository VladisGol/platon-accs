#include "mainWin.h"

mainWin::mainWin(QWidget *parent)
    : QMainWindow(parent)

{
		QTextCodec * codec = QTextCodec::codecForName("CP1251");
		QTextCodec::setCodecForTr(codec);
		QTextCodec::setCodecForCStrings(codec);
		//QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));


		setupUi(this);

		MyDB = IBPP::DatabaseFactory("vladisgol",
	                                        "platon",
	                                        "sysdba",
	                                        "u+3LS2Tc",
	                                        "",//Role
	                                        "WIN1251",//codepage
	                                        "DEFAULT CHARACTER SET WIN1251");//Доп параметры
		MyDB->Connect();

		//platon::iterEidos* MyEidosIter= new  platon::iterEidos(MyDB,"ALL");
		EidosTreeWidget->SetSpecies("ALL");
		EidosTreeWidget->AttachToDB(MyDB);

		LocalEidos=NULL;
		LocalHypotesis=NULL;

		//QObject::connect(tableViewHypotesis, SIGNAL(activated(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		QObject::connect(tableViewHypotesis, SIGNAL(clicked(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		//QObject::connect(tableViewHypotesis, SIGNAL(entered(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		QObject::connect(EidosTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(SetHypotesysView(QTreeWidgetItem*, int)));
//		QObject::connect(EidosTreeWidget, SIGNAL(itemEntered(QTreeWidgetItem*,int)), EidosTreeWidget, SLOT(expandAll()));

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


void mainWin::GoGrid()
{
	return ;
}

void mainWin::Go()
{

	return ;
}
