/*
 * PragmaMemModel.cpp
 *
 *  Created on: 14.08.2008
 *      Author: slava
 */

#include "HypPragmaMemModel.h"


namespace platon
{

HypPragmaMemModel::HypPragmaMemModel(Eidos* InEidos, QObject * parent)
					:AbstractMemHypModel(parent)
{
	// TODO Auto-generated constructor stub
	setObjectName("HypPragmaMemModel");

	ForEidos =InEidos;

	NumCol=ForEidos->PragmaSQL->AttributesList.size();						//Получаем количество полей в запросе
	ReservedColumns=2;														//Одно зарезервированное поле ID

	Id_records = new QVector <long>;										//Выделяем необходимую память
	FieldsInModel= new QVector <QMap<long,QVariant>*>;
	for(int i=0;i<NumCol+1;i++) FieldsInModel->append(new QMap<long,QVariant>);
	KeyIterator=new iterHypPragma(ForEidos);								//Выставляем итератор ключей записей
	ReadToBuffer();															//Считываем значения в буфер

}

ExtraAttribute* HypPragmaMemModel::getEAFromEidos(int i) const
{
	std::string FieldName = this->ForEidos->PragmaSQL->AttributesList[i].FieldName;
	return ForEidos->GetEAByFieldName(FieldName);
}

QString HypPragmaMemModel::getSQLstringforEA(ExtraAttribute*MyEA) const
{
	QString SQLString;
	SQLString="select GET_PRAGMA_WITH_HIPOTESIS_LIST.ID id, id_link, meaning from GET_PRAGMA_WITH_HIPOTESIS_LIST("+QString::number(ForEidos->GetID())+") inner join ";
	SQLString=SQLString+QString::fromStdString(MyEA->NameStoredProc())+"("+QString::number(MyEA->GetEAID())+")";
	SQLString=SQLString+" on GET_PRAGMA_WITH_HIPOTESIS_LIST.ID="+QString::fromStdString(MyEA->NameStoredProc())+".ID_LINK;";
	return SQLString;
}

QVariant HypPragmaMemModel::headerData(int section, Qt::Orientation orientation,int role) const
{
	//Процедура выводит значения надписей столбцов и строк
	if(role == Qt::DecorationRole)	//Возвращаем иконки
		return this->GetHeaderIcon(section, orientation);

	if (role != Qt::DisplayRole)
	         return QVariant();

	if (orientation==Qt::Horizontal)
	{
		if(section==0)
			return tr("ID");
		if(section==1)
			return tr("Тип");
		else
			return tr(this->ForEidos->PragmaSQL->AttributesList[section-ReservedColumns].Caption.c_str());
	}
	else
	{
		return QString::number(section);
	}
}
void HypPragmaMemModel::ReadToBuffer() const
{
	//Процедура считывает в буфер по полям записи из БД
	//Заполняем значения в массиве идентификаторов записей

	Id_records->clear();
	KeyIterator->First();
	LastRequestedReccount=0;
	QMap<long,QVariant>* NameMap = FieldsInModel->at(0);//Нулевое поле - наименование Hypotesis

	while(KeyIterator->Fetched())
	{
		int idrecord=KeyIterator->GetID();
		QVariant Name_Val=tr(((iterHypPragma*)KeyIterator)->GetTitle().c_str());
		Id_records->append(idrecord);
		NameMap->insert(idrecord,Name_Val);
		LastRequestedReccount++;
		KeyIterator->Next();
	}

	//Считываем поля экстраатрибутов
	for(int i=1;i<NumCol+1;i++)
	{
		GetOneFieldInBuffer(i-1, FieldsInModel->at(i));
	}
}


}
