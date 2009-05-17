#include "Multilinks.h"

namespace platon
{
Multilinks::Multilinks(QWidget * parent,  AssociatedExtraAttribute* InAEA): QMainWindow(parent)
{
	setupUi(this);

	DataClass* DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;

	this->AEAttrib=InAEA;

	MyIter= new iterMultilink(AEAttrib);

	QObject::connect(action_Exit, SIGNAL(activated()), this, SLOT(Exit()));

	ReadFormWidgetsAppearance();

}

void Multilinks::Exit()
{
	WriteFormWidgetsAppearance();
	this->close();
}

Multilinks::~Multilinks()
{
	;
}

void Multilinks::ReadFormWidgetsAppearance()
{
	//Процедура считывает из DbETC параметры элементов формы и устанавливает их значения
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);

	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName()).toStdString(),true,-1);
	int w=800,h=700;
	if(MyETC->ParamExists("width")) w=MyETC->ReadInteger("width");
	if(MyETC->ParamExists("height")) h=MyETC->ReadInteger("height");
	this->resize (w,h);

	MyETC->CloseKey();
	delete MyETC;
}

void Multilinks::WriteFormWidgetsAppearance()
{
	//Процедура записывает в DbETC параметры элементов формы
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName()).toStdString(),true,-1);
	MyETC->WriteInteger("width", this->width());
	MyETC->WriteInteger("height", this->height());

	MyETC->CloseKey();
	delete MyETC;
}

}
