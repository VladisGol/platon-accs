#include "mainWin.h"
#include <QtGui>
#include "EssentialClass.h"
#include "ibpp.h"
#include "HypotesisMemModel.h"
//#include "phipotesystablemodel.h"
//#include "hypotesismodel.h"
#include "pragmamodel.h"
#include "HypotesisMemModel.h"
#include "PragmaMemModel.h"




mainWin::mainWin(QWidget *parent)
    : QMainWindow(parent)

{
	setupUi(this);
//	QObject::connect(GoButton, SIGNAL(clicked()), this, SLOT(Go()));
//	QObject::connect(GridButton, SIGNAL(clicked()), this, SLOT(GoGrid()));


	IBPP::Database MyDB = IBPP::DatabaseFactory("localhost",
	                                        "platon",
	                                        "sysdba",
	                                        "u+3LS2Tc",
	                                        "",//Role
	                                        "WIN1251",//codepage
	                                        "DEFAULT CHARACTER SET WIN1251");//Доп параметры
		MyDB->Connect();


		platon::iterEidos* MyEidosIter= new  platon::iterEidos(MyDB,"ALL");

		platon::Eidos* MyEidos=new platon::Eidos(MyDB,9);
		platon::HypotesisMemModel* MyModel=new platon::HypotesisMemModel(MyEidos, this);
		//HypotesisModel* MyModel=new HypotesisModel(MyEidos, this);
		//PragmaModel* MyModel=new PragmaModel(MyEidos, this);
		//platon::Hypotesis* MyHypotes= new platon::Hypotesis(MyEidos, 6757);
		//platon::PragmaMemModel* MyModel=new platon::PragmaMemModel(MyHypotes, this);
		tableView->setModel(MyModel);



}

void mainWin::GoGrid()
{
	return ;
}

void mainWin::Go()
{

	return ;
}
