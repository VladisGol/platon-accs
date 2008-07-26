#include "firstFormapp.h"
#include <QtGui>
#include "EssentialClass.h"
#include "ibpp.h"
#include "phipotesystablemodel.h"
#include "hypotesismodel.h"



firstformAPP::firstformAPP(QWidget *parent)
    : QWidget(parent)

{
	setupUi(this);
	QObject::connect(GoButton, SIGNAL(clicked()), this, SLOT(Go()));
	QObject::connect(GridButton, SIGNAL(clicked()), this, SLOT(GoGrid()));

}

void firstformAPP::GoGrid()
{
/*
	QMessageBox::warning(this, tr("My Application"),
                   tr("The document has been modified.\n"
                      "Do you want to save your changes?"),
                   QMessageBox::Save | QMessageBox::Discard
                   | QMessageBox::Cancel,
                   QMessageBox::Save);
   platon::pHipotesysTableModel* MyModel=new platon::pHipotesysTableModel(this);
	tableView->setModel(MyModel);

	IBPP::Database MyDB = IBPP::DatabaseFactory("localhost",
	                                        "platon",
	                                        "sysdba",
	                                        "u+3LS2Tc",
	                                        "",//Role
	                                        "WIN1251",//codepage
	                                        "DEFAULT CHARACTER SET WIN1251");//Доп параметры
		MyDB->Connect();


		platon::Eidos* MyEidos=new platon::Eidos(MyDB,7171);

		for(int i=1;i<1001;i++)
		{

			platon::Hypotesis* OneItem;
			OneItem=new platon::Hypotesis(MyEidos,QString("Hypotesis_"+QString::number(i)).toStdString());
			OneItem->Autocommited=true;
			this->textEdit->append(OneItem->GetHypotesName().c_str());
			OneItem->Save();

			delete OneItem;
		}*/
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
                                        "DEFAULT CHARACTER SET WIN1251");//Доп параметры
	MyDB->Connect();


	platon::iterEidos* MyEidosIter= new  platon::iterEidos(MyDB,"ALL");

	/*treeWidget->SetSpecies("ALL");
	treeWidget->AttachToDB(MyDB);*/
/*
	while(MyEidosIter->Next())
	{
		this->textEdit->append(QString("<h2>"+tr( MyEidosIter->GetTitle().c_str())+"</h2>"));
		platon::iterHypotesis* MyHypIter=new platon::iterHypotesis(MyDB,MyEidosIter->GetID());
		while(MyHypIter->Next())
		{
			this->textEdit->append("<h3>"+QString(".....")+tr(MyHypIter->GetTitle().c_str())+"</h3>");
			platon::iterPragma * MyPragmaIter= new platon::iterPragma(MyDB,MyEidosIter->GetID(),MyHypIter->GetID());
			while(MyPragmaIter->Next())
			{
				this->textEdit->append(QString("..........")+tr(MyPragmaIter->GetTitle().c_str()));
			}
			delete MyPragmaIter;
		}
		delete MyHypIter;
	}
	delete MyEidosIter;
*/
	platon::Eidos* MyEidos=new platon::Eidos(MyDB,7171);
	HypotesisModel* MyModel=new HypotesisModel(MyEidos, this);
	tableView->setModel(MyModel);



/*

	MyEidosIter= new  platon::iterEidos(MyDB,"RES");
	while(MyEidosIter->Next())
	{
		platon::Eidos* MyEidos=new platon::Eidos(MyDB, MyEidosIter->GetID());
		this->textEdit->append(QString("<h2>"+tr( MyEidos->GetName().c_str())+"</h2>"));
		platon::iterHypotesis* MyHypIter=new platon::iterHypotesis(MyEidos);
		while(MyHypIter->Next())
		{
			platon::Hypotesis * MyHypotesis= new platon::Hypotesis(MyEidos,MyHypIter->GetID());
			this->textEdit->append("<h3>"+QString(".....")+tr(MyHypotesis->GetHypotesName().c_str())+"</h3>");
			platon::iterPragma * MyPragmaIter= new platon::iterPragma(MyHypotesis);
			while(MyPragmaIter->Next())
			{
				platon::Pragma * MyPragma=new platon::Pragma(MyHypotesis,MyPragmaIter->GetID());
				this->textEdit->append(QString("..........")+tr(MyPragma->GetPragmaName().c_str()));
				delete MyPragma;
			}
			delete MyHypotesis;
			delete MyPragmaIter;
		}
		delete MyHypIter;
		delete MyEidos;
	}
	delete MyEidosIter;

*/

/*

	long HipID=MyIter->First();

	this->textEdit->append(QString("<h2>"+tr(MyEidos->GetName().c_str())+"</h2>"));
	//this->textEdit->append(QString(tr("<CENTER> <TABLE BORDER><TR>   <TD> Раз </TD>   <TD> Два </TD>   <TD> Три </TD></TR> </TABLE> </CENTER>")));


    bool IsFirst=true;
    QString Header=tr("<CENTER> <TABLE BORDER>");
	while (HipID>0)
	{
	    platon::Hypotesis* MyHyp=new platon::Hypotesis(MyEidos,HipID);

        if(IsFirst)
        {
            IsFirst=false;
            Header.append("<TR>");
            for(unsigned int i=0;i < MyHyp->Attributes.size();i++)
            {
		Header.append("<TD>"+tr(MyHyp->Attributes[i]->EA->GetEAFieldName().c_str())+"</TD>");
            }
            Header.append("</TR>");
        }

        Header.append("<TR>");
        for(unsigned int i=0;i < MyHyp->Attributes.size();i++)
        {
		Header.append("<TD>"+tr(MyHyp->Attributes[i]->GetVisibleValue().c_str())+"</TD>");
        }
        Header.append("</TR>");

	    delete MyHyp;
	    HipID=MyIter->Next();
	//break;
    }
    Header.append("</TABLE> </CENTER>");
    this->textEdit->append(Header);

    delete MyIter;
    delete MyEidos;
*/
	return ;
}
