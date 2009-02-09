#include "PragmaEditForm.h"

namespace platon
{
PragmaEditForm::PragmaEditForm(QWidget * parent, long ID_Pragma): AbstarctHipEditForm(parent)
{
	DataClass* DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;

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
//	this->DB=InPr->HostEidos->DB;
	DataClass* DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;

	FormFillFrames();
}
PragmaEditForm::~PragmaEditForm()
{
	delete LocalEidos;
	delete LocalHypotesis;
	delete LocalHostHypotesis;
}

}
