#include "DialogchoicePragma.h"
namespace platon
{

ChoicePragma_Dialog::ChoicePragma_Dialog(QWidget * parent, platon::Eidos* InEidos, long ID_in): QDialog(parent)
{
    if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("ChoicePragma_Dialog"));
    this->resize(434, 348);

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    TableView= new QTableView(this);
    Model=new HypPragmaMemModel(InEidos,this);
    TableView->setModel(Model);

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

}
void ChoicePragma_Dialog::ExitWithAccept()
{
	//Выход с возвратом значения выбранного объекта

	int row=this->TableView->currentIndex().row();
	int col=0;
	Out_value=QVariant(TableView->model()->data(Model->index(row,col,QModelIndex()))).toInt();

	this->accept();
}
bool ChoicePragma_Dialog::find(long ID_searchfor)
{
	QModelIndex founded_item= Model->GetQModelIndexByID(ID_searchfor);
	if(founded_item.row()==0) return false;
	this->TableView->setCurrentIndex(founded_item);
	return true;
}
}
