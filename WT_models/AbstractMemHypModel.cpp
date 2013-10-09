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
    delete ValuesInModel;
    delete FieldsID;
    delete RecordsIter;

}

int AbstractMemHypModel::columnCount(const WModelIndex & index) const
{
	//Процедура возвращает число столбцов (полей)
	return this->NumCol+ReservedColumns;
}

int AbstractMemHypModel::rowCount(const WModelIndex &parent) const
{
	//Процедура возвращает число строк
    return LastRequestedReccount;
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



boost::any AbstractMemHypModel::GetDataFromBuffer(const WModelIndex &index)const
{
//Функция возвращает значение с индексом, переданном в параметре
	int row=index.row();
	int col=index.column();

    if(col==0) return boost::any(this->FieldsID->at(row));

    std::pair<int,int> koordinates=std::pair<int,int>(row,col);
    boost::any AnValue;
    if(ValuesInModel->find(koordinates)!=ValuesInModel->end())
        AnValue=ValuesInModel->at(std::pair<int,int>(row,col));

    return AnValue;
}
WModelIndex AbstractMemHypModel::GetWModelIndexByID(long ID_in)
{    
    return this->index(FieldsID->at(ID_in),0,WModelIndex());
}

}
