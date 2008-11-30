#include "DialogchoiceHypotesis.h"
namespace platon
{

ChoiceHypotesis_Dialog::ChoiceHypotesis_Dialog(QWidget * parent, platon::Eidos* InEidos, long ID_in): QDialog(parent)
{
    if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("Dialog"));
    this->resize(434, 348);

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    TableView= new QTableView(this);
    Model=new HypotesisMemModel(InEidos,this);
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

    this->setWindowTitle(tr("Укажите тип"));

}
void ChoiceHypotesis_Dialog::ExitWithAccept()
{
	//Выход с возвратом значения выбранного объекта

	int row=this->TableView->currentIndex().row();
	int col=0;
	Out_value=QVariant(TableView->model()->data(Model->index(row,col,QModelIndex()))).toInt();

	this->accept();
}
bool ChoiceHypotesis_Dialog::find(long ID_searchfor)
{
	/*
	//QList<QTreeWidgetItem *> FoundedItem = this->treeWidget->findItems (QString::number(ID_searchfor), Qt::MatchExactly | Qt::MatchRecursive,1 );
    if(FoundedItem.count()>0)      //Найден искомый элемент
    {
    	//this->treeWidget->setCurrentItem(FoundedItem.at(0));	//Найденный элемент выводим текущим
    	return true;
    }
    else*/
    	return false;

}
}
