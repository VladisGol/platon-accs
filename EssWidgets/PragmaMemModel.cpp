/*
 * PragmaMemModel.cpp
 *
 *  Created on: 14.08.2008
 *      Author: slava
 */

#include "PragmaMemModel.h"


namespace platon
{

PragmaMemModel::PragmaMemModel(Hypotesis* InHyp, QObject * parent)
					:AbstractMemHypModel(parent)
{
	// TODO Auto-generated constructor stub
	setObjectName("PragmaMemModel");

	ForEidos =InHyp->HostEidos;

	MyHyp=InHyp;

	NumCol=InHyp->HostEidos->PragmaSQL->AttributesList.size();				//Получаем количество полей в запросе
	ReservedColumns=1;														//Одно зарезервированное поле ID

	Id_records = new QVector <long>;										//Выделяем необходимую память
	FieldsInModel= new QVector <QMap<long,QVariant>*>;
	for(int i=0;i<NumCol;i++) FieldsInModel->append(new QMap<long,QVariant>);
	KeyIterator=new iterPragma(MyHyp);										//Выставляем итератор ключей записей
	ReadToBuffer();															//Считываем значения в буфер

}

ExtraAttribute* PragmaMemModel::getEAFromEidos(int i) const
{
	std::string FieldName = this->ForEidos->PragmaSQL->AttributesList[i].FieldName;
	return ForEidos->GetEAByFieldName(FieldName);
}

QString PragmaMemModel::getSQLstringforEA(ExtraAttribute*MyEA) const
{
	QString SQLString;
	SQLString="select GET_PRAGMA_LIST.ID id, id_link, meaning from GET_PRAGMA_LIST("+QString::number(ForEidos->GetID())+", "+QString::number(MyHyp->GetID())+") inner join ";
	SQLString=SQLString+QString::fromStdString(MyEA->NameStoredProc())+"("+QString::number(MyEA->GetEAID())+")";
	SQLString=SQLString+" on GET_PRAGMA_LIST.ID="+QString::fromStdString(MyEA->NameStoredProc())+".ID_LINK;";
	return SQLString;
}

QVariant PragmaMemModel::headerData(int section, Qt::Orientation orientation,int role) const
{
	//Процедура выводит значения надписей столбцов и строк

	if (role != Qt::DisplayRole)
	         return QVariant();

	if (orientation==Qt::Horizontal)
	{
		if(section==0)
			return "ID";
		else
			return tr(this->ForEidos->PragmaSQL->AttributesList[section-ReservedColumns].Caption.c_str());
	}
	else
	{
		return QString::number(section);
	}
}


}
