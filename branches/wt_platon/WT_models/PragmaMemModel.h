/*
 * PragmaMemModel.h
 *
 *  Created on: 14.08.2008
 *      Author: slava
 */

#ifndef PRAGMAMEMMODEL_H_
#define PRAGMAMEMMODEL_H_

#include "AbstractMemHypModel.h"
#include "EssentialClass.h"

namespace platon
{

class PragmaMemModel: public platon::AbstractMemHypModel
{
public:
    PragmaMemModel(Hypotesis* InHyp, WObject * parent);
    boost::any headerData(int section, Wt::Orientation orientation,int role = Wt::DisplayRole) const;
    int  GetColumnNumberByFieldName(std::string FieldName);
protected:
	Hypotesis* MyHyp;
	virtual ExtraAttribute* getEAFromEidos(int i) const ;		//Процедура возвращает ссылку на экстраатрибут по номеру
    virtual std::string getSQLstringforEA(ExtraAttribute*MyEA) const;	//Функция возвращает заполненную SQL строку для получения 1 экстраатрибута

};

}

#endif /* PRAGMAMEMMODEL_H_ */
