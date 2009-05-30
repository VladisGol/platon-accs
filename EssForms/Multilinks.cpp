#include "Multilinks.h"

namespace platon
{
Multilinks::Multilinks(QWidget * parent,  AssociatedExtraAttribute* InAEA): QMainWindow(parent)
{
	setupUi(this);

	DataClass* DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;

	this->AEAttrib=InAEA;

	MyIter= new iterMultilink(AEAttrib);
	MyIter->LocalTR=AEAttrib->OwnerHypotesis->TransactionIBPP;
	MyIter->LocalST=IBPP::StatementFactory(this->DB, MyIter->LocalTR);

	LNKMap=new QMap<long,LNK_Value>;
	RatedLinks=false;	//Не показывать кратность ссылок

	QObject::connect(action_Exit, SIGNAL(activated()), this, SLOT(Exit()));
	QObject::connect(action_AddLink, SIGNAL(activated()), this, SLOT(AddLink()));
	QObject::connect(action_EditLink, SIGNAL(activated()), this, SLOT(EditLink()));
	QObject::connect(action_DeleteLink, SIGNAL(activated()), this, SLOT(DeleteLink()));

	FillWidgets(0,0);	//Запоняем таблицу и становимся на первый элемент с координатами row=0,col=0

	ReadFormWidgetsAppearance();

}

void Multilinks::Exit()
{
	WriteFormWidgetsAppearance();
	this->close();
}

void Multilinks::AddLink()
{
	//Добавляем новую запись ссылки
	int row=tableWidget->currentRow();
	int col=tableWidget->currentColumn();

	platon::LNK_Value EditedLNKValue;
	EditedLNKValue.LinkTo=0;
	EditedLNKValue.Ratio=1;
	EditedLNKValue= LNKChoice(EditedLNKValue);
	if(EditedLNKValue.LinkTo>0)
		this->AEAttrib->SetMultiLNKValue(EditedLNKValue,0);	//Сохраняем значение

	FillWidgets(row,col);

}

void Multilinks::EditLink()
{
	//Редактируем существующую запись ссылки
	int row=tableWidget->currentRow();
	int col=tableWidget->currentColumn();

	long idml=tableWidget->item(row, 0)->data(Qt::DisplayRole).toInt();

	platon::LNK_Value EditedLNKValue;
	EditedLNKValue= LNKChoice(LNKMap->value(idml));
	if(EditedLNKValue.LinkTo>0)
		this->AEAttrib->SetMultiLNKValue(EditedLNKValue,idml);	//Сохраняем значение

	FillWidgets(row,col);
}

void Multilinks::DeleteLink()
{
	//Удаляем ссылку
	int row=tableWidget->currentRow();
	int col=tableWidget->currentColumn();

	long idml=tableWidget->item(row, 0)->data(Qt::DisplayRole).toInt();

	QMessageBox::StandardButton reply;
	reply = QMessageBox::information(this, tr("Внимание, необратимая операция"), tr("Подтвердите удаление ссылки"),QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Ok)
		AEAttrib->DeleteMultiLNKValue(idml);

	FillWidgets(row,col);
}

Multilinks::~Multilinks()
{
	;
}
void Multilinks::FillWidgets(int prow,int pcolumn)
{
	//Процедура заполняет виджеты формы (в первую очередь список ссылок) и выставляет в списке ссылок индекс от начала
	//значение которого передано в параметрах
	tableWidget->clear();	//Полностью очищаем виджет
	//Формируем заголовок
	tableWidget->setColumnCount(4);
	tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem(tr("ID")));
	tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem(tr("ID объекта")));
	tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem(tr("Описание объекта")));
	tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem(tr("Кратность")));

	if(!RatedLinks)
		tableWidget->setColumnHidden(3,true);
	else
		tableWidget->setColumnHidden(3,false);

	//Формируем строчную часть
	//if(MyIter->LocalTR->Started())MyIter->LocalTR->Commit();	//Пробуем влезть в транзакцию
	//MyIter->LocalTR->Start();

	MyIter->First();	//Начинаем заполнение с первого элемента
	LNKMap->clear();	//Очищаем массив ссылок

	int NumberRows=MyIter->GetRowCount();
	if(NumberRows>0)
	{
		tableWidget->setRowCount(NumberRows);
		for(int row=0;row<NumberRows;row++)
		{
			LNKMap->insert(MyIter->GetID(),MyIter->GetLNKValue());	//Ссылку добавляем в массив

			tableWidget->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(row),0));
			tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(MyIter->GetID()),0));
			tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(MyIter->GetLNKValue().LinkTo),0));
			tableWidget->setItem(row, 2, new QTableWidgetItem(tr(MyIter->GetTitle().c_str()),0));
			tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(MyIter->GetLNKValue().Ratio),0));

			MyIter->Next();
		}
		tableWidget->resizeColumnsToContents();	//Выравниваем размер столбцов по их содержимому
	}

	//Позиционируем
	if(prow>NumberRows) prow=NumberRows;	//На случай вызова функции после удаления последней строки ссылки
	tableWidget->setCurrentCell (prow,pcolumn);
}

platon::LNK_Value Multilinks::LNKChoice(platon::LNK_Value InLNK)
{
	//Процедура выводит диалоговые окна для выбора конкретного значения идентификатора
	platon::LNK_Value localLNKVal=InLNK;

	long ID_Eidos, ID_Hyp;
	if(AEAttrib->EA->type==platon::ft_LinkHypotesis)
	{
		if(AEAttrib->EA->LNK_EidosID==0)	//Выводим список эйдосов
		{
			QString spec=QString::fromStdString(AEAttrib->EA->LNK_species);
			if(spec=="")spec="ALL";
			platon::Hypotesis::GetEidosHypotesisIDS(DB,InLNK.LinkTo,ID_Eidos, ID_Hyp);

			ChoiceEidos_Dialog* Localdialog=new ChoiceEidos_Dialog(this,spec,ID_Eidos);
			Localdialog->exec();
			ID_Eidos=Localdialog->Out_value;
		}
		else ID_Eidos=AEAttrib->EA->LNK_EidosID;
		if(ID_Eidos!=0)
		{
			//Выводим список гипотез для выбора
			platon::Eidos *localEidos=new platon::Eidos(DB,ID_Eidos);
			ChoiceHypotesis_Dialog* HypotesisDialog=new ChoiceHypotesis_Dialog(this,localEidos,InLNK.LinkTo);
			HypotesisDialog->exec();

			localLNKVal.LinkTo=HypotesisDialog->Out_value;

			if(!RatedLinks)
				localLNKVal.Ratio=1;
			else
			{
				bool ok;
				double d = QInputDialog::getDouble(this, tr("Введите значение"),tr("Кратность ссылки:"),InLNK.Ratio , -10000, 10000, 2, &ok);
				if (ok) localLNKVal.Ratio=d;
			}

			delete localEidos;
		}
	}
	if(AEAttrib->EA->type==platon::ft_LinkPragma)
	{
		if(AEAttrib->EA->LNK_EidosID==0)	//Выводим список эйдосов
		{
			QString spec=QString::fromStdString(AEAttrib->EA->LNK_species);
			if(spec=="")spec="ALL";
			platon::Pragma::GetEidosHypotesisIDS(DB,InLNK.LinkTo,ID_Eidos, ID_Hyp);

			ChoiceEidos_Dialog* Localdialog=new ChoiceEidos_Dialog(this,spec,ID_Eidos);
			Localdialog->exec();
			ID_Eidos=Localdialog->Out_value;
		}
		else ID_Eidos=AEAttrib->EA->LNK_EidosID;
		if(ID_Eidos!=0)
		{

			//Выводим список прагм с наименованиями гипотез для выбора
			platon::Eidos *localEidos=new platon::Eidos(DB,ID_Eidos);
			ChoicePragma_Dialog* PragmaDialog=new ChoicePragma_Dialog(this,localEidos,InLNK.LinkTo);
			PragmaDialog->exec();
			localLNKVal.LinkTo=PragmaDialog->Out_value;

			if(!RatedLinks)
				localLNKVal.Ratio=1;
			else
			{
				bool ok;
				double d = QInputDialog::getDouble(this, tr("Введите значение"),tr("Кратность ссылки:"),InLNK.Ratio , -10000, 10000, 2, &ok);
				if (ok) localLNKVal.Ratio=d;
			}

			delete localEidos;
		}
	}
	return localLNKVal;
}

void Multilinks::ReadFormWidgetsAppearance()
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

void Multilinks::WriteFormWidgetsAppearance()
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
