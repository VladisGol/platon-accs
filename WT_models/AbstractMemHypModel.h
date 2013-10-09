/*
 * AbstractMemHypModel.h
 *
 *  Created on: 07.08.2008
 *      Author: slava
 */

#ifndef ABSTRACTMEMHYPMODEL_H_
#define ABSTRACTMEMHYPMODEL_H_

#include <vector>
#include <map>
#include "EssentialClass.h"
#include <Wt/WAbstractTableModel>
#include <Wt/WDate>

using namespace Wt;
namespace platon
{
class AbstractMemHypModel : public WAbstractTableModel
{
public:
    AbstractMemHypModel(WObject * parent);
	virtual ~AbstractMemHypModel();

    int rowCount(const WModelIndex &parent = WModelIndex())const;
    int columnCount(const WModelIndex & index) const;
    virtual boost::any data(const WModelIndex &index, int role) const;
    pIterator* RecordsIter;
    void ReadToBuffer()const {;};								// Процедура считывает в буфер все записи
    WModelIndex GetWModelIndexByID(long ID_in);				// Функция возвращает значение индекса модели по идентификатору записи

protected:
    mutable std::vector <long> * FieldsID;					//Вектор для хранения ID записи по которой можно будет найти записи в векторе FieldsInModel
    mutable std::map<std::pair<int,int>,boost::any>* ValuesInModel;	//Вектор для хранения значений полей в виде QMap <long,QVariant>

	mutable int NumCol;										//Число полей в запросе (из числа экстраатрибутов)
	mutable int LastRequestedReccount;						//Число записей в запросе

	int ReservedColumns;									//Зарезервированное число полей
    virtual boost::any GetDataFromBuffer(const WModelIndex &index) const;


	Eidos* ForEidos;										//Ссылка на Eidos, по которому идет обработка

	virtual ExtraAttribute* getEAFromEidos(int i) const {return NULL;};			//Функция возвращает ссылку на экстраатрибут по номеру
};

}

#endif /* ABSTRACTMEMHYPMODEL_H_ */
