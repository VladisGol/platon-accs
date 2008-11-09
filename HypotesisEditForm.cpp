#include "HypotesisEditForm.h"

namespace platon
{
HypotesisEditForm::HypotesisEditForm(QWidget * parent, IBPP::Database InDB, long ID_Hypotesys): QMainWindow(parent)
{
	setupUi(this);

	this->DB=InDB;
	long EidosID,HypotesysID;
	Hypotesis::GetEidosHypotesisIDS(DB, ID_Hypotesys,EidosID,HypotesysID);
	if(ID_Hypotesys==HypotesysID)	//��������� ������� �� ����� ������� �������� � ���������
	{
		LocalEidos= new Eidos(DB, EidosID);
		LocalHypotesis=new Hypotesis(LocalEidos,HypotesysID);
		LocalHypotesis->Autocommited=false;	//���� ��������� �������������� ����������

		for(int i =LocalHypotesis->Attributes.size()-1;i>=0;i--)
		{
			AssociatedExtraAttribute* tmpAttrib =(AssociatedExtraAttribute*)LocalHypotesis->Attributes[i];
			if(tmpAttrib->EA->type!=platon::ft_Security && tmpAttrib->EA->Visible)
			{
				EA_OneFrame* my =new EA_OneFrame(this,tmpAttrib);
				my->setObjectName("EA_Frame"+QString::number(i));
				verticalLayout->addWidget(my);
			}
		}
	}
	else throw "������ �� ������� ��������� ������������� �� ��������� �� ����������";

	QObject::connect(action_Cancel, SIGNAL(activated()), this, SLOT(ExitByCancel()));
	QObject::connect(action_saveNexit, SIGNAL(activated()), this, SLOT(ExitWithSave()));
}

void HypotesisEditForm::ExitWithSave()
{
	QList<EA_OneFrame*> AllFrames =this->findChildren<EA_OneFrame*>(QRegExp("EA_Frame*"));
	for(int i=0;i<AllFrames.count();i++)
		AllFrames.at(i)->Save();

	this->LocalHypotesis->TransactionIBPP->Commit();
	this->close();
}
void HypotesisEditForm::ExitByCancel()
{
	this->LocalHypotesis->TransactionIBPP->Rollback();
	this->close();
}



}
