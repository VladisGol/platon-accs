#include "PragmaEditForm.h"

namespace platon
{
PragmaEditForm::PragmaEditForm(QWidget * parent, long ID_Pragma): AbstarctHipEditForm(parent)
{
	MyDCl=platon::GetDataModule(this);
	this->DB=MyDCl->DB;

	long EidosID, HypotesysID;

	Pragma::GetEidosHypotesisIDS(DB, ID_Pragma,EidosID,HypotesysID);
	if(EidosID>0)	//��������� ������� �� ����� ����� �������� � ���������
	{
		LocalEidos= new Eidos(DB, EidosID);
		LocalHostHypotesis=new Hypotesis(LocalEidos,HypotesysID);;
		LocalHypotesis=new Pragma(LocalHostHypotesis,ID_Pragma);
		LocalHypotesis->Autocommited=false;	//���� ��������� �������������� ����������
		FormFillFrames();
		dbEtcBranchName=QString("EidosID=")+QString::number(LocalEidos->GetID())+QString(":pragma");
		ReadFormWidgetsAppearance();
	}
	else throw "������ �� ������� ��������� ������������� �� ��������� �� ����������";
}
PragmaEditForm::PragmaEditForm(QWidget * parent, Pragma* InPr): AbstarctHipEditForm(parent)
{
	LocalHypotesis=InPr;
	LocalHostHypotesis=InPr->HostHypotesis;
	LocalEidos=InPr->HostEidos;
	MyDCl=platon::GetDataModule(this);
	this->DB=MyDCl->DB;
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
