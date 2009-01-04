/*
 * HypotesysMemModel.cpp
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */

#include "LnkdPragmaMemModel.h"


namespace platon
{

LnkdHypPragmaMemModel::LnkdHypPragmaMemModel(Eidos* InEidos,iterLNKS_Hyp* LnkdIterator ,QObject * parent)
					:HypPragmaMemModel(parent)
{
	setObjectName("LnkdHypPragmaMemModel");
	ForEidos =InEidos;

	NumCol=ForEidos->PragmaSQL->AttributesList.size();				//Получаем количество полей в запросе
	ReservedColumns=2;														//Одно зарезервированное поле ID

	Id_records = new QVector <long>;										//Выделяем необходимую память
	FieldsInModel= new QVector <QMap<long,QVariant>*>;
	for(int i=0;i<NumCol+1;i++) FieldsInModel->append(new QMap<long,QVariant>);

	KeyIterator=LnkdIterator;												//Выставляем итератор ключей записей
	iterEidosID=LnkdIterator->EidosID;
	iterID_in=LnkdIterator->ID_in;

	ReadToBuffer();															//Считываем значения в буфер
}

QString LnkdHypPragmaMemModel::getSQLstringforEA(ExtraAttribute*MyEA) const
{
	QString SQLString;
	SQLString="select ID_PRAGMA ID, id_link, meaning from GET_LINKED_PRAGMALIST("+QString::number(this->iterID_in)+") inner join ";
	SQLString=SQLString+QString::fromStdString(MyEA->NameStoredProc())+"("+QString::number(MyEA->GetEAID())+")";
	SQLString=SQLString+" on GET_LINKED_PRAGMALIST.ID_PRAGMA="+QString::fromStdString(MyEA->NameStoredProc())+".ID_LINK";
	SQLString=SQLString+" WHERE ID_EIDOS="+QString::number(this->iterEidosID)+";";
	return SQLString;
}


}

