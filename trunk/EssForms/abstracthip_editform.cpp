#include "abstracthip_editform.h"

namespace platon
{
AbstarctHipEditForm::AbstarctHipEditForm(QWidget * parent): QMainWindow(parent)
{
	setupUi(this);

	QObject::connect(action_Cancel, SIGNAL(activated()), this, SLOT(ExitByCancel()));
	QObject::connect(action_saveNexit, SIGNAL(activated()), this, SLOT(ExitWithSave()));
}

void AbstarctHipEditForm::ExitWithSave()
{
	if(this->LocalHypotesis->TransactionIBPP->Started()==false) this->LocalHypotesis->TransactionIBPP->Start();

	QList<EA_OneFrame*> AllFrames =this->findChildren<EA_OneFrame*>(QRegExp("EA_Frame*"));
	for(int i=0;i<AllFrames.count();i++)
		AllFrames.at(i)->Save();

	this->LocalHypotesis->TransactionIBPP->Commit();

	WriteFormWidgetsAppearance();
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
		AssociatedExtraAttribute* tmpAttrib =(AssociatedExtraAttribute*)LocalHypotesis->Attributes[ElementsNumber-i];	//Разворачиваем с последнего до первого
		if(tmpAttrib->EA->type!=platon::ft_Security && tmpAttrib->EA->Visible)
		{
			EA_OneFrame* my =new EA_OneFrame(this,tmpAttrib);
			my->setObjectName("EA_Frame"+QString::number(i));
			my->MyDCl=this->MyDCl;				//Записываем адрес модуля данных
			verticalLayout->insertWidget(-1,my); //Заносим в форму фрейм
		}
	}
}

void AbstarctHipEditForm::ReadFormWidgetsAppearance()
{
	//Процедура считывает из DbETC параметры элементов формы и устанавливает их значения
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
	//Процедура записывает в DbETC параметры элементов формы
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName()+"\\"+dbEtcBranchName).toStdString(),true,-1);
	MyETC->WriteInteger("width", this->width());
	MyETC->WriteInteger("height", this->height());

	MyETC->CloseKey();
	delete MyETC;
}

}
