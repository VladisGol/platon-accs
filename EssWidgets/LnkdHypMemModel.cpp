/*
 * HypotesysMemModel.cpp
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */

#include "LnkdHypMemModel.h"


namespace platon
{

LnkdHypMemModel::LnkdHypMemModel(Eidos* InEidos,int IDin ,QObject * parent)
					:HypotesisMemModel(parent)
{
	setObjectName("LnkdHypMemModel");
	ForEidos =InEidos;

	NumCol=InEidos->HypotesisSQL->AttributesList.size();					//Получаем количество полей в запросе
	ReservedColumns=1;														//Одно зарезервированное поле ID

	Id_records = new QVector <long>;										//Выделяем необходимую память
	FieldsInModel= new QVector <QMap<long,QVariant>*>;
	for(int i=0;i<NumCol;i++) FieldsInModel->append(new QMap<long,QVariant>);

	KeyIterator= new iterLNKS_Hyp(ForEidos->DB);							//Выставляем итератор ключей записей
	((iterLNKS_Hyp*)KeyIterator)->MasterChanged(ForEidos->GetID(),IDin);

	ID_in=IDin;

	ReadToBuffer();															//Считываем значения в буфер
}

QString LnkdHypMemModel::getSQLstringforEA(ExtraAttribute*MyEA) const
{
	QString SQLString;
	SQLString="select ID_HYPOTESIS ID, id_link, meaning from GET_LINKED_HYPLIST("+QString::number(this->ID_in)+") inner join ";
	SQLString=SQLString+QString::fromStdString(MyEA->NameStoredProc())+"("+QString::number(MyEA->GetEAID())+")";
	SQLString=SQLString+" on GET_LINKED_HYPLIST.ID_HYPOTESIS="+QString::fromStdString(MyEA->NameStoredProc())+".ID_LINK";
	SQLString=SQLString+" WHERE ID_EIDOS="+QString::number(this->ForEidos->GetID())+";";
	return SQLString;
}


}

