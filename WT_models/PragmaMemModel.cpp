/*
 * PragmaMemModel.cpp
 *
 *  Created on: 14.08.2008
 *      Author: slava
 */

#include "PragmaMemModel.h"


namespace platon
{

PragmaMemModel::PragmaMemModel(Hypotesis* InHyp, WObject * parent)
					:AbstractMemHypModel(parent)
{
	// TODO Auto-generated constructor stub
	setObjectName("PragmaMemModel");
	ForEidos =InHyp->HostEidos;

	MyHyp=InHyp;

	NumCol=InHyp->HostEidos->PragmaSQL->AttributesList.size();				//Получаем количество полей в запросе
	ReservedColumns=1;														//Одно зарезервированное поле ID

    Id_records = new std::vector <long>;										//Выделяем необходимую память
    FieldsInModel= new std::vector <std::map<long,boost::any>*>;
    for(int i=0;i<NumCol;i++)
        FieldsInModel->push_back(new std::map<long,boost::any>);
	KeyIterator=new iterPragma(MyHyp);										//Выставляем итератор ключей записей
	ReadToBuffer();															//Считываем значения в буфер

}

ExtraAttribute* PragmaMemModel::getEAFromEidos(int i) const
{
	std::string FieldName = this->ForEidos->PragmaSQL->AttributesList[i].FieldName;
	return ForEidos->GetEAByFieldName(FieldName);
}

std::string PragmaMemModel::getSQLstringforEA(ExtraAttribute*MyEA) const
{
    std::string SQLString;
    SQLString="select GET_PRAGMA_LIST.ID id, id_link, meaning from GET_PRAGMA_LIST("+std::string(boost::lexical_cast<std::string>(ForEidos->GetID()))+", "+std::string(boost::lexical_cast<std::string>(MyHyp->GetID()))+") inner join ";
    SQLString=SQLString+MyEA->NameStoredProc()+"("+std::string(boost::lexical_cast<std::string>(MyEA->GetEAID()))+")";
    SQLString=SQLString+" on GET_PRAGMA_LIST.ID="+MyEA->NameStoredProc()+".ID_LINK;";
	return SQLString;
}

boost::any PragmaMemModel::headerData(int section, Wt::Orientation orientation,int role) const
{
	//Процедура выводит значения надписей столбцов и строк
    if(role == Wt::DecorationRole)	//Возвращаем иконки
        //return this->GetHeaderIcon(section, orientation);
        return boost::any();


    if (role != Wt::DisplayRole)
             return boost::any();

    if (orientation==Wt::Horizontal)
	{
		if(section==0)
			return "ID";
		else
		{
            return this->ForEidos->PragmaSQL->AttributesList[section-ReservedColumns].Caption;
		}
	}
    else
	{
        return boost::lexical_cast<std::string>(section);
	}
}

int  PragmaMemModel::GetColumnNumberByFieldName(std::string FieldName)
{
	//Процедура выводит номер столбца модели по имени, переданному в параметре
	for(int i=0;i<NumCol;i++)
	{
        std::string OneField = ForEidos->PragmaSQL->AttributesList[i].FieldName;
        if(OneField==FieldName)
		{
			return i+ReservedColumns;
		}
	}
	return -1;	//Ничего не найдено
}

}
