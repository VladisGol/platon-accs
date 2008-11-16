#include "HypotesisEditForm.h"

namespace platon
{
HypotesisEditForm::HypotesisEditForm(QWidget * parent, IBPP::Database InDB, long ID_Hypotesys): AbstarctHipEditForm(parent)
{
	this->DB=InDB;
	long EidosID,HypotesysID;
	Hypotesis::GetEidosHypotesisIDS(DB, ID_Hypotesys,EidosID,HypotesysID);
	if(EidosID>0)	//��������� ������� �� ����� ������� �������� � ���������
	{
		LocalEidos= new Eidos(DB, EidosID);
		LocalHypotesis=new Hypotesis(LocalEidos,HypotesysID);
		LocalHypotesis->Autocommited=false;	//���� ��������� �������������� ����������
		FormFillFrames();
	}
	else throw "������ �� ������� ��������� ������������� �� ��������� �� ����������";
}
HypotesisEditForm::HypotesisEditForm(QWidget * parent, Hypotesis* InHip): AbstarctHipEditForm(parent)
{
	LocalHypotesis=InHip;
	LocalEidos=InHip->HostEidos;
	this->DB=InHip->HostEidos->DB;
	FormFillFrames();
}
HypotesisEditForm::~HypotesisEditForm()
{
	delete LocalEidos;
	delete LocalHypotesis;
}
}
