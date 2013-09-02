#include "DialogchoicePragma.h"
namespace platon
{

ChoicePragma_Dialog::ChoicePragma_Dialog(QWidget * parent, platon::Eidos* InEidos, long ID_in): QDialog(parent)
{
    if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("ChoicePragma_Dialog"));

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;

    TableView= new QTableView(this);
    Model=new HypPragmaMemModel(InEidos,this);
    SFProxyModel= new QSortFilterProxyModel(this);
    SFProxyModel->setSourceModel(Model);
    TableView->setModel(SFProxyModel);
    TableView->setSortingEnabled(true);
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

    if(this->find(ID_in)) Out_value=ID_in; else Out_value=0;

    this->setWindowTitle(tr("Укажите экземпляр"));
    SetViewID(DTL->ViewIDs);
    ReadFormWidgetsAppearance();

}
void ChoicePragma_Dialog::ExitWithAccept()
{
	//Выход с возвратом значения выбранного объекта

	int row=this->TableView->currentIndex().row();
	Out_value=QVariant(TableView->model()->data(TableView->model()->index(row,0,QModelIndex()))).toInt();

	WriteFormWidgetsAppearance();
	this->accept();
}
bool ChoicePragma_Dialog::find(long ID_searchfor)
{
	QModelIndex founded_item= SFProxyModel->mapFromSource(Model->GetQModelIndexByID(ID_searchfor));
	if(founded_item.row()==0) return false;
	this->TableView->setCurrentIndex(founded_item);
	return true;
}
void ChoicePragma_Dialog::ReadFormWidgetsAppearance()
{
	//Процедура считывает из DbETC параметры элементов формы и устанавливает их значения

    DTL->ETC_OpenKey(QString("FormsAppearance\\"+this->objectName()));
	int w=800,h=700;
    if(DTL->ETC_ParamExists("width")) w=DTL->ETC_ReadInteger("width");
    if(DTL->ETC_ParamExists("height")) h=DTL->ETC_ReadInteger("height");
	this->resize (w,h);
    DTL->ETC_CloseKey();
}

void ChoicePragma_Dialog::WriteFormWidgetsAppearance()
{
	//Процедура записывает в DbETC параметры элементов формы
    DTL->ETC_OpenKey(QString("FormsAppearance\\"+this->objectName()));
    DTL->ETC_WriteInteger("width", this->width());
    DTL->ETC_WriteInteger("height", this->height());
    DTL->ETC_CloseKey();
}
void ChoicePragma_Dialog::SetViewID(bool IsView)
{
	//Процедура скрывает или открывает поля идентификаторов объектов
	if(!IsView)	//Отметка снята
	{
		for (int i=0;i<TableView->model()->columnCount(QModelIndex());i++)
		{
			QString FieldCaption = TableView->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				TableView->hideColumn(i);
		}
	}
	else								//Отметка в меню установлена
	{
		for (int i=0;i<TableView->model()->columnCount(QModelIndex());i++)
		{
			QString FieldCaption = TableView->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
			if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
				TableView->showColumn(i);
		}
	}
}

}
