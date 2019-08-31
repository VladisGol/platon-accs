/*
 * HypotesysMemModel.cpp
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */

#include "LnkdPragmaMemModel.h"


namespace platon
{

LnkdHypPragmaMemModel::LnkdHypPragmaMemModel(Eidos* InEidos,int IDin ,QObject * parent)
					:HypPragmaMemModel(parent)
{
	setObjectName("LnkdHypPragmaMemModel");
	ForEidos =InEidos;

	NumCol=ForEidos->PragmaSQL->AttributesList.size();						//Получаем количество полей в запросе
	ReservedColumns=2;														//Одно зарезервированное поле ID

	Id_records = new QVector <long>;										//Выделяем необходимую память
	FieldsInModel= new QVector <QMap<long,QVariant>*>;
	for(int i=0;i<NumCol+1;i++) FieldsInModel->append(new QMap<long,QVariant>);

	KeyIterator= new iterLNKS_Pragma(ForEidos->DB);							//Выставляем итератор ключей записей
	((iterLNKS_Pragma*)KeyIterator)->MasterChanged(ForEidos->GetID(),IDin);

	ID_in=IDin;

	ReadToBuffer();															//Считываем значения в буфер
}

QString LnkdHypPragmaMemModel::getSQLstringforEA(ExtraAttribute*MyEA) const
{
	QString SQLString;
	SQLString="select ID_PRAGMA ID, id_link, meaning from GET_LINKED_PRAGMALIST("+QString::number(this->ID_in)+") inner join ";
	SQLString=SQLString+QString::fromStdString(MyEA->NameStoredProc())+"("+QString::number(MyEA->GetEAID())+")";
	SQLString=SQLString+" on GET_LINKED_PRAGMALIST.ID_PRAGMA="+QString::fromStdString(MyEA->NameStoredProc())+".ID_LINK";
	SQLString=SQLString+" WHERE ID_EIDOS="+QString::number(this->ForEidos->GetID())+";";
	return SQLString;
}

}

