#include "DialogchoiceEidos.h"
namespace platon
{


ChoiceEidos_Dialog::ChoiceEidos_Dialog(QWidget * parent,QString Species, long ID_in): QDialog(parent)
{
    if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("Dialog_Eidos"));

    VBoxLayout = new QVBoxLayout(this);
    VBoxLayout->setObjectName(QString::fromUtf8("VBoxLayout"));

    checkBox = new QCheckBox(this);
    checkBox->setObjectName(QString::fromUtf8("checkBox"));
    checkBox->setText(tr("Раскрыть все уровни"));

    VBoxLayout->addWidget(checkBox);

	DataClass* DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;

    treeWidget = new QEidosTreeWidget (this);
    treeWidget->SetSpecies(Species);
    treeWidget->AttachToDB(DB);

    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

    VBoxLayout->addWidget(treeWidget);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    VBoxLayout->addWidget(buttonBox);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ExitWithAccept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QObject::connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(collapseExp(int)));

    if(this->find(ID_in)) Out_value=ID_in; else Out_value=0;

    this->setWindowTitle(tr("Укажите класс"));
    SetViewID(DTL->ViewIDs);
    ReadFormWidgetsAppearance();

}
void ChoiceEidos_Dialog::ExitWithAccept()
{
	//Выход с возвратом значения выбранного объекта
	Out_value=this->treeWidget->GetEidosID();
	WriteFormWidgetsAppearance();
	this->accept();
}
void ChoiceEidos_Dialog::collapseExp(int state)
{
	if(state==Qt::Checked) treeWidget->expandAll();
	else  treeWidget->collapseAll();
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
	bool ch=false;
	if(MyETC->ParamExists("width")) w=MyETC->ReadInteger("width");
	if(MyETC->ParamExists("height")) h=MyETC->ReadInteger("height");
	if(MyETC->ParamExists("checkBoxState")) ch=MyETC->ReadBool("checkBoxState");
	this->resize (w,h);
	checkBox->setChecked(ch);

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
	MyETC->WriteBool("checkBoxState",checkBox->isChecked());

	MyETC->CloseKey();
	delete MyETC;
}

void ChoiceEidos_Dialog::SetViewID(bool IsView)
{
	//Процедура скрывает или открывает поля идентификаторов объектов
	if(!IsView)	//Отметка снята
	{
		for (int i=0;i<treeWidget->columnCount();i++)
			{
				QString FieldCaption = treeWidget->headerItem()->text(i);
				if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
					treeWidget->hideColumn(i);
			}
	}
	else								//Отметка в меню установлена
	{
		for (int i=0;i<treeWidget->columnCount();i++)
			{
				QString FieldCaption = treeWidget->headerItem()->text(i);
				if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
					treeWidget->showColumn(i);
			}
	}
}
}