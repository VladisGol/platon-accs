/*
 * AbstractMemHypModel.cpp
 *
 *  Created on: 07.08.2008
 *      Author: slava
 */

#include "AbstractMemHypModel.h"

namespace platon
{
AbstractMemHypModel::AbstractMemHypModel(WObject * parent)
            :WAbstractTableModel(parent)
{
	;
}
AbstractMemHypModel::~AbstractMemHypModel()
{
	// TODO Auto-generated destructor stub
    for(unsigned int i=0; i<FieldsInModel->size();i++)
	{
		FieldsInModel->at(i)->clear();
		delete FieldsInModel->at(i);
	}
	FieldsInModel->clear();
	delete FieldsInModel;
	delete Id_records;
	delete KeyIterator;

}

int AbstractMemHypModel::columnCount(const WModelIndex & index) const
{
	//Процедура возвращает число столбцов (полей)
	return this->NumCol+ReservedColumns;
}

int AbstractMemHypModel::rowCount(const WModelIndex &parent) const
{
	//Процедура возвращает число строк
	return Id_records->size();
}

boost::any AbstractMemHypModel::data(const WModelIndex &index, int role) const
{
	 	if (!index.isValid())
             return boost::any();

	     if (index.row() >= LastRequestedReccount)
             return boost::any();

         if (role == Wt::DisplayRole)
             return GetDataFromBuffer(index);
	     else
             return boost::any();
}

void AbstractMemHypModel::ReadToBuffer() const
{
	//Процедура считывает в буфер по полям записи из БД
	//Заполняем значения в массиве идентификаторов записей

	Id_records->clear();
	KeyIterator->First();
	LastRequestedReccount=0;
	while(KeyIterator->Fetched())
	{
        Id_records->push_back(KeyIterator->GetID());
		LastRequestedReccount++;
		KeyIterator->Next();
	}

	//Считываем поля экстраатрибутов
	for(int i=0;i<NumCol;i++)
	{
		GetOneFieldInBuffer(i, FieldsInModel->at(i));
	}
}

void AbstractMemHypModel::GetOneFieldInBuffer(int FieldNumber, std::map<long,boost::any>* OneMap) const
{
	//Процедура заполняет QMap переданный в параметре значениями поля

	OneMap->clear();
	iterHypotesis* MyIterator=new iterHypotesis(ForEidos);
	ExtraAttribute* MyEA;
	long Key;

	MyEA=getEAFromEidos(FieldNumber);	//Получаем экстраатрибут

    MyIterator->SQL_string=getSQLstringforEA(MyEA);
	MyIterator->First();

	while(MyIterator->Fetched())
	{
            boost::any OneValue;

			switch(MyEA->type)
			{
				case platon::ft_String:
				{
					std::string tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
                    OneValue=WString(tmpValue,Wt::UTF8);
					Key=MyIterator->GetID();
					break;
				}
				case platon::ft_Integer:
				{
					int tmpValue;
					MyIterator->LocalST->Get("MEANING",(int32_t*)&tmpValue);
					OneValue=tmpValue;
					Key=MyIterator->GetID();
					break;
				}
				case platon::ft_Boolean:
				{
					bool tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
					OneValue=tmpValue;
					Key=MyIterator->GetID();
					break;
				}
				case platon::ft_Float:
				{
					float tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
					OneValue=tmpValue;
					Key=MyIterator->GetID();
					break;
				}
				case platon::ft_DateTime:
				{
					IBPP::Timestamp MyTs;
					MyIterator->LocalST->Get("MEANING",MyTs);
					Key=MyIterator->GetID();
                    Wt::WDate LocalDate = Wt::WDate(MyTs.Year(),MyTs.Month(),MyTs.Day());
                    OneValue=LocalDate.toString();
					break;
				}
				case platon::ft_DLL:
				case platon::ft_LinkHypotesis:
				case platon::ft_LinkPragma:
				{
					std::string tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
                    OneValue=WString(tmpValue,Wt::UTF8);
					Key=MyIterator->GetID();

					break;
				}
				default:
					throw("Указанный тип данных не поддерживается");

			}
            OneMap->insert(std::pair<long, boost::any>(Key,OneValue));
			MyIterator->Next();
	}
}
boost::any AbstractMemHypModel::GetDataFromBuffer(const WModelIndex &index)const
{
//Функция возвращает значение с индексом, переданном в параметре
	int row=index.row();
	int col=index.column();

    if(col==0) return boost::any(Id_records->at(row));

    std::map<long,boost::any>* OneMap=FieldsInModel->at(col-1);
    long key=Id_records->at(row);
    //
    if(OneMap->find(key)!=OneMap->end())
    {
        boost::any ForReturn = OneMap->at(key);  //OneMap->Value(Key)
        return ForReturn;
    }
    else
        return boost::any();
}
WModelIndex AbstractMemHypModel::GetWModelIndexByID(long ID_in)
{    
    return this->index(Id_records->at(ID_in),0,WModelIndex());
}

}
