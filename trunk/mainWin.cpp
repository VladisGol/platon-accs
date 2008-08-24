#include "mainWin.h"

mainWin::mainWin(QWidget *parent)
    : QMainWindow(parent)

{
	setupUi(this);
//	QObject::connect(GridButton, SIGNAL(clicked()), this, SLOT(GoGrid()));

	IBPP::Database MyDB = IBPP::DatabaseFactory("vladisgol",
	                                        "platon",
	                                        "sysdba",
	                                        "u+3LS2Tc",
	                                        "",//Role
	                                        "WIN1251",//codepage
	                                        "DEFAULT CHARACTER SET WIN1251");//Доп параметры
		MyDB->Connect();

		platon::iterEidos* MyEidosIter= new  platon::iterEidos(MyDB,"ALL");

		LocalEidos=new platon::Eidos(MyDB,9);
		platon::HypotesisMemModel* MyModel=new platon::HypotesisMemModel(LocalEidos, this);

		//HypotesisModel* MyModel=new HypotesisModel(MyEidos, this);
		//PragmaModel* MyModel=new PragmaModel(MyEidos, this);
		//platon::Hypotesis* MyHypotes= new platon::Hypotesis(MyEidos, 6757);
		//platon::PragmaMemModel* MyModel=new platon::PragmaMemModel(MyHypotes, this);
		tableViewHypotesis->setModel(MyModel);
		LocalHypotesis=NULL;
		//QObject::connect(tableViewHypotesis, SIGNAL(activated(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		QObject::connect(tableViewHypotesis, SIGNAL(clicked(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
		//QObject::connect(tableViewHypotesis, SIGNAL(entered(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));

}
void mainWin::SetHypotesysView()
{
	long id_eidos;

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
