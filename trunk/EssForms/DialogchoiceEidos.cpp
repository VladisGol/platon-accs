#include "DialogchoiceEidos.h"

ChoiceEidos_Dialog::ChoiceEidos_Dialog(QWidget * parent,IBPP::Database InDB, long ID_in): QDialog(parent)
{
	Out_value=ID_in;

    if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("Dialog"));
    this->resize(434, 348);

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    treeWidget = new QEidosTreeWidget (this);
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

}
void ChoiceEidos_Dialog::ExitWithAccept()
{
	//Выход с возвратом значения выбранного объекта
	Out_value=this->treeWidget->currentItem()->text(1).toLong();
	this->accept();
}

