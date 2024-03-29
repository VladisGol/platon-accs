#include "qeidostreewidget.h"


QEidosTreeWidget::QEidosTreeWidget(QWidget* parent)
				:QTreeWidget(parent)
{
	Species="ALL";
}

void QEidosTreeWidget::AttachToDB(IBPP::Database InDB)
{
    DTL=platon::GetDataModule(this);
	this->MyDB=InDB;
	MyEidosIter= new  platon::iterEidos(MyDB,Species.toStdString());

	//Настраиваем виджет
	this->clear();
	this->setColumnCount (3);	//Name,ID,ID_Parent
	this->headerItem()->setText(0,tr("Наименование"));
	this->headerItem()->setText(1,tr("ID"));
	this->headerItem()->setText(2,tr("ID_PARENT"));

    DTL->ETC_OpenKey(QString("WidgetAppearance\\QEidosTreeWidget\\"+this->parent()->objectName ()));
    if(DTL->ETC_ParamExists("Column_Name_width")) this->setColumnWidth ( 0,DTL->ETC_ReadInteger("Column_Name_width"));
    if(DTL->ETC_ParamExists("Column_ID_width")) this->setColumnWidth ( 1,DTL->ETC_ReadInteger("Column_ID_width"));
    if(DTL->ETC_ParamExists("Column_ID_PARENT_width")) this->setColumnWidth ( 2,DTL->ETC_ReadInteger("Column_ID_PARENT_width"));
    DTL->ETC_CloseKey();

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
}

QEidosTreeWidget::~QEidosTreeWidget()
{
	SaveAppearance();
};


void QEidosTreeWidget::SetSpecies(const QString InSpecies)
{
	Species=InSpecies;
}

int QEidosTreeWidget::GetEidosID()
{
	//Функция возвращает текущее значение идентификатора эйдоса
	return this->currentItem()->text(1).toLong();
}

bool QEidosTreeWidget::findNMakeCurrent(int ID_searchfor)
{

	QList<QTreeWidgetItem *> FoundedItem = this->findItems (QString::number(ID_searchfor), Qt::MatchExactly | Qt::MatchRecursive,1 );
    if(FoundedItem.count()>0)      //Найден искомый элемент
    {
    	this->setCurrentItem(FoundedItem.at(0));	//Найденный элемент выводим текущим
    	return true;
    }
    else
    	return false;

}


void QEidosTreeWidget::SaveAppearance()
{
//Процедура сохраняет значения внешнего вида виджета
    DTL->ETC_OpenKey(QString("WidgetAppearance\\QEidosTreeWidget\\"+this->parent()->objectName ()));
    DTL->ETC_WriteInteger("Column_Name_width", this->columnWidth (0));
    DTL->ETC_WriteInteger("Column_ID_width", this->columnWidth (1));
    DTL->ETC_WriteInteger("Column_ID_PARENT_width", this->columnWidth (2));
    DTL->ETC_CloseKey();
}
