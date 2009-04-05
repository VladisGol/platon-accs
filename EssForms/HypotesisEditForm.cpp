#include "HypotesisEditForm.h"

namespace platon
{
HypotesisEditForm::HypotesisEditForm(QWidget * parent, long ID_Hypotesys): AbstarctHipEditForm(parent)
{
	this->setObjectName("HypotesisEditForm");
	MyDCl=platon::GetDataModule(this);
	this->DB=MyDCl->DB;

	long EidosID,HypotesysID;
	Hypotesis::GetEidosHypotesisIDS(DB, ID_Hypotesys,EidosID,HypotesysID);
	if(EidosID>0)	//Проверяем найдены ли среди гипотез заданная в параметре
	{
		LocalEidos= new Eidos(DB, EidosID);
		LocalHypotesis=new Hypotesis(LocalEidos,HypotesysID);
		LocalHypotesis->Autocommited=false;	//Сами управляем подтверждением транзакции
		FormFillFrames();
		dbEtcBranchName=QString("EidosID=")+QString::number(LocalEidos->GetID())+QString(":hypotesis");
		ReadFormWidgetsAppearance();
		FormActionsTune();
	}
	else throw "Объект на который указывает идентификатор из параметра не существует";
}
HypotesisEditForm::HypotesisEditForm(QWidget * parent, Hypotesis* InHip): AbstarctHipEditForm(parent)
{
	this->setObjectName("HypotesisEditForm");
	LocalHypotesis=InHip;
	LocalEidos=InHip->HostEidos;
	MyDCl=platon::GetDataModule(this);
	this->DB=MyDCl->DB;
	LocalHypotesis->Autocommited=false;	//Сами управляем подтверждением транзакции
	FormFillFrames();
	dbEtcBranchName=QString("EidosID=")+QString::number(LocalEidos->GetID())+QString(":hypotesis");
	ReadFormWidgetsAppearance();
	FormActionsTune();

}
HypotesisEditForm::~HypotesisEditForm()
{
	delete LocalEidos;
	delete LocalHypotesis;
}
}
