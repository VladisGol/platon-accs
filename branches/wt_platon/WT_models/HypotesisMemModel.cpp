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

    FieldsID = new std::vector <long>;										//Выделяем необходимую память

    for(int i=0;i<NumCol;i++) FieldsID->push_back(InEidos->HypotesisSQL->AttributesList.at(i).EAID);
    ValuesInModel= new std::map<std::pair<int,int>,boost::any>;
    RecordsIter=new iterHypotesis(ForEidos);								//Выставляем итератор ключей записей
	ReadToBuffer();															//Считываем значения в буфер
}


ExtraAttribute* HypotesisMemModel::getEAFromEidos(int i) const
{
    long FieldID = this->ForEidos->HypotesisSQL->AttributesList[i].EAID;
    return ForEidos->GetEAByID(FieldID);
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
            return getEAFromEidos(section-ReservedColumns)->GetEACaption();
		}
	}
	else
	{
        return boost::lexical_cast<std::string>(section);
	}
}
void HypotesisMemModel::ReadToBuffer() const
{
    //Процедура считывает в буфер по полям записи из БД

    RecordsIter->First();
    LastRequestedReccount=0;
    while(RecordsIter->Fetched())
    {
        platon::Hypotesis* OneHyp = new platon::Hypotesis(this->ForEidos,RecordsIter->GetID());
        for(unsigned int i=0; i<NumCol+ReservedColumns;i++)
        {
            boost::any OneString;
            if(i==0)
                OneString = OneHyp->GetID();
            else
                OneString = Wt::WString(OneHyp->GetEAByFieldID(this->getEAFromEidos(i-ReservedColumns)->GetEAID())->GetVisibleValue(),Wt::UTF8);

            std::pair<int,int> koordinates=std::pair<int,int>(LastRequestedReccount,i);
            ValuesInModel->insert(std::pair<std::pair<int,int>,boost::any>(koordinates,OneString));

        }
        delete OneHyp;
        LastRequestedReccount++;
        RecordsIter->Next();
    }

}
}

