/*
 * HypotesysMemModel.cpp
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */

#include "HypotesisMemModel.h"

namespace platon
{
HypotesisMemModel::HypotesisMemModel(Eidos* InEidos, WObject * parent)
					:AbstractMemHypModel(parent)
{
	setObjectName("HypotesisMemModel");
	ForEidos =InEidos;

	NumCol=InEidos->HypotesisSQL->AttributesList.size();					//Получаем количество полей в запросе
	ReservedColumns=1;														//Одно зарезервированное поле ID

    Id_records = new std::vector <long>;										//Выделяем необходимую память
    FieldsInModel= new std::vector <std::map<long,boost::any>*>;
    for(int i=0;i<NumCol;i++) FieldsInModel->push_back(new std::map<long,boost::any>);

	KeyIterator=new iterHypotesis(ForEidos);								//Выставляем итератор ключей записей
	ReadToBuffer();															//Считываем значения в буфер
}


ExtraAttribute* HypotesisMemModel::getEAFromEidos(int i) const
{
	std::string FieldName = this->ForEidos->HypotesisSQL->AttributesList[i].FieldName;
	return ForEidos->GetEAByFieldName(FieldName);
}

std::string HypotesisMemModel::getSQLstringforEA(ExtraAttribute*MyEA) const
{
    std::string SQLString;
    SQLString="select GET_HYPOTESIS_LIST.ID id, id_link, meaning from GET_HYPOTESIS_LIST("+std::string(boost::lexical_cast<std::string>(ForEidos->GetID()))+") inner join ";
    SQLString=SQLString + std::string(MyEA->NameStoredProc())+"("+ std::string(boost::lexical_cast<std::string>(MyEA->GetEAID()))+")";
    SQLString=SQLString+" on GET_HYPOTESIS_LIST.ID="+std::string(MyEA->NameStoredProc())+".ID_LINK;";
	return SQLString;
}

boost::any HypotesisMemModel::headerData(int section, Wt::Orientation orientation,int role) const
{
	//Процедура выводит значения надписей столбцов и строк

    if(role == Wt::DecorationRole)	//Возвращаем иконки
        return boost::any();

    if (role != Wt::DisplayRole)
             return boost::any();

    if (orientation==Wt::Horizontal)
	{
		if(section==0)
			return "ID";
		else
		{
            return this->ForEidos->HypotesisSQL->AttributesList[section-ReservedColumns].Caption;
		}
	}
	else
	{
        return boost::lexical_cast<std::string>(section);
	}
}
}

