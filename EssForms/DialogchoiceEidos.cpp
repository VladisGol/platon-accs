#include "DialogchoiceEidos.h"
namespace platon
{


ChoiceEidos_Dialog::ChoiceEidos_Dialog(QWidget * parent,QString Species, long ID_in): QDialog(parent)
{
    if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("Dialog_Eidos"));

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

	DataClass* DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;

    treeWidget = new QEidosTreeWidget (this);
    treeWidget->SetSpecies(Species);
    treeWidget->AttachToDB(DB);

    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

    gridLayout->addWidget(treeWidget, 0, 0, 1, 1);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    gridLayout->addWidget(buttonBox, 1, 0, 1, 1);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ExitWithAccept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    if(this->find(ID_in)) Out_value=ID_in; else Out_value=0;

    this->setWindowTitle(tr("Укажите класс"));
    ReadFormWidgetsAppearance();

}
void ChoiceEidos_Dialog::ExitWithAccept()
{
	//Выход с возвратом значения выбранного объекта
	Out_value=this->treeWidget->GetEidosID();
	WriteFormWidgetsAppearance();
	this->accept();
}
bool ChoiceEidos_Dialog::find(long ID_searchfor)
{
	return treeWidget->findNMakeCurrent(ID_searchfor);
}
void ChoiceEidos_Dialog::ReadFormWidgetsAppearance()
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

void ChoiceEidos_Dialog::WriteFormWidgetsAppearance()
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
