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

    DTL=platon::GetDataModule(this);
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

    DTL->ETC_OpenKey(QString("FormsAppearance\\"+this->objectName()));
	int w=800,h=700;
	bool ch=false;
    if(DTL->ETC_ParamExists("width")) w=DTL->ETC_ReadInteger("width");
    if(DTL->ETC_ParamExists("height")) h=DTL->ETC_ReadInteger("height");
    if(DTL->ETC_ParamExists("checkBoxState")) ch=DTL->ETC_ReadBool("checkBoxState");
	this->resize (w,h);
	checkBox->setChecked(ch);
    DTL->ETC_CloseKey();
}

void ChoiceEidos_Dialog::WriteFormWidgetsAppearance()
{
	//Процедура записывает в DbETC параметры элементов формы
    DTL->ETC_OpenKey(QString("FormsAppearance\\"+this->objectName()));
    DTL->ETC_WriteInteger("width", this->width());
    DTL->ETC_WriteInteger("height", this->height());
    DTL->ETC_WriteBool("checkBoxState",checkBox->isChecked());
    DTL->ETC_CloseKey();
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
