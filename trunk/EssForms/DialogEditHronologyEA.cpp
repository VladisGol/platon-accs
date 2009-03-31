#include "DialogEditHronologyEA.h"
namespace platon
{

DialogEditHronologyEA::DialogEditHronologyEA(QWidget * parent,AssociatedExtraAttribute* OneAEA): QDialog(parent)
{
    if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("DialogEditHronologyEA"));
    this->resize(434, 348);

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

	DataClass* DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;

	TableView= new QTableView(this);
    Model=new HronologyEaMemModel(OneAEA,this);
    TableView->setModel(Model);
    TableView->resizeColumnsToContents();

    TableView->setObjectName(QString::fromUtf8("TableView"));

    gridLayout->addWidget(TableView, 0, 0, 1, 1);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    gridLayout->addWidget(buttonBox, 1, 0, 1, 1);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ExitWithAccept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    this->setWindowTitle(tr("Временной ряд изменения значений атрибута"));
    ReadFormWidgetsAppearance();

}
void DialogEditHronologyEA::ExitWithAccept()
{
	//Выход с возвратом значения выбранного объекта

	int row=this->TableView->currentIndex().row();
	int col=0;
	Out_value=QVariant(TableView->model()->data(Model->index(row,col,QModelIndex()))).toInt();
	WriteFormWidgetsAppearance();
	this->accept();
}

void DialogEditHronologyEA::ReadFormWidgetsAppearance()
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

void DialogEditHronologyEA::WriteFormWidgetsAppearance()
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
