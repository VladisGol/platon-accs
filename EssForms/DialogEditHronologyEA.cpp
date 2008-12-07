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

    TableView= new QTableView(this);
    Model=new HronologyEaMemModel(OneAEA,this);
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

    this->setWindowTitle(tr("Временной ряд изменения значений атрибута"));

}
void DialogEditHronologyEA::ExitWithAccept()
{
	//Выход с возвратом значения выбранного объекта

	int row=this->TableView->currentIndex().row();
	int col=0;
	Out_value=QVariant(TableView->model()->data(Model->index(row,col,QModelIndex()))).toInt();

	this->accept();
}

}
