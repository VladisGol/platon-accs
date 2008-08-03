#include "firstFormapp.h"
#include <QtGui>
#include "EssentialClass.h"
#include "ibpp.h"
#include "phipotesystablemodel.h"
#include "hypotesismodel.h"
#include "pragmamodel.h"




firstformAPP::firstformAPP(QWidget *parent)
    : QWidget(parent)

{
	setupUi(this);
	QObject::connect(GoButton, SIGNAL(clicked()), this, SLOT(Go()));
	QObject::connect(GridButton, SIGNAL(clicked()), this, SLOT(GoGrid()));

}

void firstformAPP::GoGrid()
{
	;
}

void firstformAPP::Go()
{
    IBPP::Database MyDB = IBPP::DatabaseFactory("localhost",
                                        "platon",
                                        "sysdba",
                                        "u+3LS2Tc",
                                        "",//Role
                                        "WIN1251",//codepage
                                        "DEFAULT CHARACTER SET WIN1251");//��� ���������
	MyDB->Connect();


	platon::iterEidos* MyEidosIter= new  platon::iterEidos(MyDB,"ALL");

	platon::Eidos* MyEidos=new platon::Eidos(MyDB,9);
	HypotesisModel* MyModel=new HypotesisModel(MyEidos, this);
	//PragmaModel* MyModel=new PragmaModel(MyEidos, this);

	tableView->setModel(MyModel);

	return ;
}
