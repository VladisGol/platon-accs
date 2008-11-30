#include "DialogchoiceEidos.h"
namespace platon
{


ChoiceEidos_Dialog::ChoiceEidos_Dialog(QWidget * parent,IBPP::Database InDB,QString Species, long ID_in): QDialog(parent)
{
    if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("Dialog_Eidos"));
    this->resize(434, 348);

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    treeWidget = new QEidosTreeWidget (this);
    treeWidget->SetSpecies(Species);
    treeWidget->AttachToDB(InDB);

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

}
void ChoiceEidos_Dialog::ExitWithAccept()
{
	//Выход с возвратом значения выбранного объекта
	Out_value=this->treeWidget->currentItem()->text(1).toLong();
	this->accept();
}
bool ChoiceEidos_Dialog::find(long ID_searchfor)
{
	QList<QTreeWidgetItem *> FoundedItem = this->treeWidget->findItems (QString::number(ID_searchfor), Qt::MatchExactly | Qt::MatchRecursive,1 );
    if(FoundedItem.count()>0)      //Найден искомый элемент
    {
    	this->treeWidget->setCurrentItem(FoundedItem.at(0));	//Найденный элемент выводим текущим
    	return true;
    }
    else
    	return false;

}
}
