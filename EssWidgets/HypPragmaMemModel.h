/*
 * PragmaMemModel.h
 *
 *  Created on: 14.08.2008
 *      Author: slava
 */

#ifndef HYPPRAGMAMEMMODEL_H_
#define HYPPRAGMAMEMMODEL_H_

#include "AbstractMemHypModel.h"
#include "EssentialClass.h"

namespace platon
{

class HypPragmaMemModel: public platon::AbstractMemHypModel
{
public:
	HypPragmaMemModel(Hypotesis* InHyp, QObject * parent);
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
protected:
	Hypotesis* MyHyp;
	virtual ExtraAttribute* getEAFromEidos(int i) const ;		//Процедура возвращает ссылку на экстраатрибут по номеру
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const;	//Функция возвращает заполненную SQL строку для получения 1 экстраатрибута

};

}

#endif /* HYPPRAGMAMEMMODEL_H_ */
