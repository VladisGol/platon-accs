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

    FieldsID = new std::vector <long>;										//Выделяем необходимую память
    ValuesInModel= new std::map<std::pair<int,int>,boost::any>;
    RecordsIter=new iterPragma(MyHyp);										//Выставляем итератор ключей записей
	ReadToBuffer();															//Считываем значения в буфер

}

ExtraAttribute* PragmaMemModel::getEAFromEidos(int i) const
{
    long FieldID = this->ForEidos->PragmaSQL->AttributesList[i].EAID;
    return ForEidos->GetEAByID(FieldID);
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
            return this->getEAFromEidos(section-ReservedColumns)->GetEACaption();
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
void PragmaMemModel::ReadToBuffer() const
{
    //Процедура считывает в буфер по полям записи из БД

    RecordsIter->First();
    LastRequestedReccount=0;
    while(RecordsIter->Fetched())
    {
        platon::Pragma* OnePr = new platon::Pragma(MyHyp,RecordsIter->GetID());
        for(unsigned int i=0; i<NumCol+ReservedColumns;i++)
        {
            boost::any OneString;
            if(i==0)
                OneString = OnePr->GetID();
            else
                OneString = Wt::WString(OnePr->GetEAByFieldID(this->getEAFromEidos(i-ReservedColumns)->GetEAID())->GetVisibleValue(),Wt::UTF8);
            std::pair<int,int> koordinates=std::pair<int,int>(LastRequestedReccount,i-ReservedColumns);
            ValuesInModel->insert(std::pair<std::pair<int,int>,boost::any>(koordinates,OneString));
            std::cout<<"LastRequestedReccount =" + boost::lexical_cast<std::string>(LastRequestedReccount);
        }
        delete OnePr;
        LastRequestedReccount++;
        RecordsIter->Next();
    }

}

}
