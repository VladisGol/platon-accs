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

		LocalEidos=NULL;
		LocalHypotesis=NULL;

		QObject::connect(tableViewHypotesis, SIGNAL(activated(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		QObject::connect(tableViewHypotesis, SIGNAL(clicked(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		QObject::connect(tableViewHypotesis, SIGNAL(entered(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		QObject::connect(EidosTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(SetHypotesysView(QTreeWidgetItem*, int)));
		//QObject::connect(EidosTreeWidget, SIGNAL(itemEntered(QTreeWidgetItem*,int)), EidosTreeWidget, SLOT(expandAll()));
		QObject::connect(comboBox_Species, SIGNAL(currentIndexChanged(int)), this, SLOT(SetEidosView(int)));
		SetEidosView(0);
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
	//SetHypotesysView(EidosTreeWidget->currentItem(),0);

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
