#include "abstracthip_editform.h"
#include "mainWin.h"

namespace platon
{
AbstarctHipEditForm::AbstarctHipEditForm(QWidget * parent): QMainWindow(parent)
{
	setupUi(this);

	QObject::connect(action_Cancel, SIGNAL(activated()), this, SLOT(ExitByCancel()));
	QObject::connect(action_saveNexit, SIGNAL(activated()), this, SLOT(ExitWithSave()));
	QObject::connect(action_AddAction, SIGNAL(activated()), this, SLOT(DoAddAction()));
	QObject::connect(action_WriteOffRes, SIGNAL(activated()), this, SLOT(DoWriteOffRes()));
	QObject::connect(action_OpenType, SIGNAL(activated()), this, SLOT(DoOpenType()));

}
void AbstarctHipEditForm::FormActionsTune()
{
	//��������� ��������� �������� ����� � ����������� �� ���� �������������� �������

	this->action_AddAction->setVisible(false);
	this->action_WriteOffRes->setVisible(false);
	this->action_OpenType->setVisible(false);

	QString Species= QString::fromStdString(this->LocalEidos->GetEidosSpecies());

	if(this->objectName()=="HypotesisEditForm")
	{
		;
	}

	if(this->objectName()=="PragmaEditForm")
	{
		this->action_OpenType->setVisible(true);
		if (Species=="OBJ")
		{
			this->action_AddAction->setVisible(true);
		}
		if (Species=="ACT")
		{
			this->action_WriteOffRes->setVisible(true);
		}
	}
}

void AbstarctHipEditForm::DoAddAction()
{
	//��������� ��������� �������� ��� �������� �������
	ChoiceEidos_Dialog* Localdialog=new ChoiceEidos_Dialog(this,"ACT",0);
	Localdialog->exec();
	long ID_Eidos=Localdialog->Out_value;

	if(ID_Eidos==0)	return;

	//������� ������ ������� ��� ������
	platon::Eidos *localEidos=new platon::Eidos(this->DB,ID_Eidos);
	ChoiceHypotesis_Dialog* HypotesisDialog=new ChoiceHypotesis_Dialog(this,localEidos,0);
	HypotesisDialog->exec();
	long ID_ActType=HypotesisDialog->Out_value;

	if(ID_ActType==0)
	{
		delete localEidos;
		return;
	}

	platon::ACTType * localActType= new platon::ACTType(localEidos,ID_ActType);
	localActType->Autocommited=false;
	platon::ACTCopy * newActioncopy=localActType->AddACTCopy(((platon::Pragma*)this->LocalHypotesis)->GetID());

	platon::PragmaEditForm * md=new platon::PragmaEditForm(this,newActioncopy);
	md->setWindowTitle(tr("�������������� ��������"));
	md->show();
	//�������� newActioncopy localActType � localEidos - ������� � ����������� ����� PragmaEditForm
}
void AbstarctHipEditForm::DoWriteOffRes()
{
	//��������� ��������� ������ �� ���������� ��������
		ChoiceEidos_Dialog* Localdialog=new ChoiceEidos_Dialog(this,"RES",0);
		Localdialog->exec();
		long ID_Eidos=Localdialog->Out_value;

		if(ID_Eidos==0)	return;

		//������� ������ ����� ��� ������
		platon::Eidos *localEidos=new platon::Eidos(this->DB,ID_Eidos);
		ChoicePragma_Dialog* PragmaDialog=new ChoicePragma_Dialog(this,localEidos,0);

		int filteredColumn=((PragmaMemModel*)PragmaDialog->Model)->GetColumnNumberByFieldName("LNK_ForkedRES");
		PragmaDialog->SFProxyModel->setFilterFixedString("");	//������������� ������ ��� ������� ���������� �����
		PragmaDialog->SFProxyModel->setFilterKeyColumn(filteredColumn);
		PragmaDialog->exec();
		long ID_RESCopy=PragmaDialog->Out_value;

		if(ID_RESCopy==0)
		{
			delete localEidos;
			return;
		}

		long ideidos,idhyp;
		platon::Pragma::GetEidosHypotesisIDS(this->DB,ID_RESCopy,ideidos,idhyp);

		platon::RESType * localResType= new platon::RESType(localEidos,idhyp);
		localResType->Autocommited=false;
		platon::RESCopy * Rescopy=new platon::RESCopy(localResType,ID_RESCopy);
		platon::RESCopy * forkedResCopy=Rescopy->Fork(((platon::Pragma*)this->LocalHypotesis)->GetID());
		delete Rescopy;

		platon::PragmaEditForm * md=new platon::PragmaEditForm(this,forkedResCopy);
		md->setWindowTitle(tr("�������������� ������� ��������������� �������"));
		md->show();
		//�������� forkedResCopy localResType � localEidos - ������� � ����������� ����� PragmaEditForm;
}
void AbstarctHipEditForm::DoOpenType()
{
//��������� ��������� �������� Hypotesis ��� �������� ������� pragma;

	platon::HypotesisEditForm * md=new platon::HypotesisEditForm(this,((platon::Pragma*)this->LocalHypotesis)->HostHypotesis->GetID());
	md->setWindowTitle(tr("�������������� ������� \"���\""));
	md->show();
}



void AbstarctHipEditForm::ExitWithSave()
{
	if(this->LocalHypotesis->TransactionIBPP->Started()==false) this->LocalHypotesis->TransactionIBPP->Start();

	QList<EA_OneFrame*> AllFrames =this->findChildren<EA_OneFrame*>(QRegExp("EA_Frame*"));
	for(int i=0;i<AllFrames.count();i++)
		AllFrames.at(i)->Save();

	this->LocalHypotesis->TransactionIBPP->Commit();

	WriteFormWidgetsAppearance();

	//����� ��������� �������� �� ������ �� ��������������� MainWin
	if(this->parent()->objectName()=="MainWindow")
	{
		if(this->objectName()=="HypotesisEditForm")
			((mainWin *)this->parent())->RefreshHView();
		if(this->objectName()=="PragmaEditForm")
			((mainWin *)this->parent())->RefreshPView();
	}
	this->close();
}
void AbstarctHipEditForm::ExitByCancel()
{
	this->LocalHypotesis->TransactionIBPP->Rollback();
	this->close();
}

void AbstarctHipEditForm::FormFillFrames()
{
	unsigned int ElementsNumber=LocalHypotesis->Attributes.size()-1;
	for(unsigned int i =0; i<=ElementsNumber;i++)
	{
		AssociatedExtraAttribute* tmpAttrib =(AssociatedExtraAttribute*)LocalHypotesis->Attributes[ElementsNumber-i];	//������������� � ���������� �� �������
		if(tmpAttrib->EA->type!=platon::ft_Security && tmpAttrib->EA->Visible)
		{
			EA_OneFrame* my =new EA_OneFrame(this,tmpAttrib);
			my->setObjectName("EA_Frame"+QString::number(i));
			my->MyDCl=this->MyDCl;				//���������� ����� ������ ������
			verticalLayout->insertWidget(-1,my); //������� � ����� �����
		}
	}
}

void AbstarctHipEditForm::ReadFormWidgetsAppearance()
{
	//��������� ��������� �� DbETC ��������� ��������� ����� � ������������� �� ��������
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);

	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName()+"\\"+dbEtcBranchName).toStdString(),true,-1);
	int w=800,h=700;
	if(MyETC->ParamExists("width")) w=MyETC->ReadInteger("width");
	if(MyETC->ParamExists("height")) h=MyETC->ReadInteger("height");
	this->resize (w,h);

	MyETC->CloseKey();
	delete MyETC;
}

void AbstarctHipEditForm::WriteFormWidgetsAppearance()
{
	//��������� ���������� � DbETC ��������� ��������� �����
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName()+"\\"+dbEtcBranchName).toStdString(),true,-1);
	MyETC->WriteInteger("width", this->width());
	MyETC->WriteInteger("height", this->height());

	MyETC->CloseKey();
	delete MyETC;
}

}
