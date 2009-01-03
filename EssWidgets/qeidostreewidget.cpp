#include "qeidostreewidget.h"


QEidosTreeWidget::QEidosTreeWidget(QWidget* parent)
				:QTreeWidget(parent)
{
	Species="ALL";
}

void QEidosTreeWidget::AttachToDB(IBPP::Database InDB)
{
	this->MyDB=InDB;
	MyEidosIter= new  platon::iterEidos(MyDB,Species.toStdString());

	//Настраиваем виджет
	this->clear();
	this->setColumnCount (3);	//Name,ID,ID_Parent
	this->headerItem()->setText(0,tr("Наименование"));
	this->headerItem()->setText(1,tr("ID"));
	this->headerItem()->setText(2,tr("ID_PARENT"));

	MyETC=new platon::DbEtc(this->MyDB);
	MyETC->OpenKey(QString("WidgetAppearance\\QEidosTreeWidget\\"+this->parent()->objectName ()).toStdString(),true,-1);
	if(MyETC->ParamExists("Column_Name_width")) this->setColumnWidth ( 0,MyETC->ReadInteger("Column_Name_width"));
	if(MyETC->ParamExists("Column_ID_width")) this->setColumnWidth ( 1,MyETC->ReadInteger("Column_ID_width"));
	if(MyETC->ParamExists("Column_ID_PARENT_width")) this->setColumnWidth ( 2,MyETC->ReadInteger("Column_ID_PARENT_width"));
	MyETC->CloseKey();

	while(MyEidosIter->Next())
	{
		//Добавляем в элемент управления отображения дерева
		QList<QTreeWidgetItem *> FoundedItem = this->findItems (QString::number(MyEidosIter->GetParentID()), Qt::MatchExactly | Qt::MatchRecursive,1 );
		if(FoundedItem.count()==0)	//Не найден, добавление в корень представления
		{
			QTreeWidgetItem *__item = new QTreeWidgetItem(this);
			__item->setText(0, tr( MyEidosIter->GetTitle().c_str()));
    		__item->setText(1,QString::number(MyEidosIter->GetID()));
			__item->setText(2,QString::number(MyEidosIter->GetParentID()));
		}
		else
		{
			QTreeWidgetItem *__item = new QTreeWidgetItem(FoundedItem.at(0));
			__item->setText(0, tr( MyEidosIter->GetTitle().c_str()));
    		__item->setText(1,QString::number(MyEidosIter->GetID()));
			__item->setText(2,QString::number(MyEidosIter->GetParentID()));
		}
	}
	delete MyEidosIter;
	delete MyETC;
}

QEidosTreeWidget::~QEidosTreeWidget()
{
	SaveAppearance();
};


void QEidosTreeWidget::SetSpecies(const QString InSpecies)
{
	Species=InSpecies;
}

void QEidosTreeWidget::SaveAppearance()
{
//Процедура сохраняет значения внешнего вида виджета
	MyETC=new platon::DbEtc(this->MyDB);
	MyETC->OpenKey(QString("WidgetAppearance\\QEidosTreeWidget\\"+this->parent()->objectName ()).toStdString(),true,-1);
	MyETC->WriteInteger("Column_Name_width", this->columnWidth (0));
	MyETC->WriteInteger("Column_ID_width", this->columnWidth (1));
	MyETC->WriteInteger("Column_ID_PARENT_width", this->columnWidth (2));
	MyETC->CloseKey();
	delete MyETC;
}
