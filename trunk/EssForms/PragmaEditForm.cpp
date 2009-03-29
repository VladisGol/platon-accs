#include "PragmaEditForm.h"

namespace platon
{
PragmaEditForm::PragmaEditForm(QWidget * parent, long ID_Pragma): AbstarctHipEditForm(parent)
{
	this->setObjectName("PragmaEditForm");
	MyDCl=platon::GetDataModule(this);
	this->DB=MyDCl->DB;

	long EidosID, HypotesysID;

	Pragma::GetEidosHypotesisIDS(DB, ID_Pragma,EidosID,HypotesysID);
	if(EidosID>0)	//Проверяем найдены ли среди прагм заданная в параметре
	{
		LocalEidos= new Eidos(DB, EidosID);
		LocalHostHypotesis=new Hypotesis(LocalEidos,HypotesysID);;
		LocalHypotesis=new Pragma(LocalHostHypotesis,ID_Pragma);
		LocalHypotesis->Autocommited=false;	//Сами управляем подтверждением транзакции
		FormFillFrames();
		dbEtcBranchName=QString("EidosID=")+QString::number(LocalEidos->GetID())+QString(":pragma");
		ReadFormWidgetsAppearance();
	}
	else throw "Объект на который указывает идентификатор из параметра не существует";
}
PragmaEditForm::PragmaEditForm(QWidget * parent, Pragma* InPr): AbstarctHipEditForm(parent)
{
	this->setObjectName("PragmaEditForm");
	LocalHypotesis=InPr;
	LocalHostHypotesis=InPr->HostHypotesis;
	LocalEidos=InPr->HostEidos;
	MyDCl=platon::GetDataModule(this);
	this->DB=MyDCl->DB;
	LocalHypotesis->Autocommited=false;	//Сами управляем подтверждением транзакции
	FormFillFrames();
	dbEtcBranchName=QString("EidosID=")+QString::number(LocalEidos->GetID())+QString(":pragma");
	ReadFormWidgetsAppearance();

}
PragmaEditForm::~PragmaEditForm()
{
	delete LocalEidos;
	delete LocalHypotesis;
	delete LocalHostHypotesis;
}

}
