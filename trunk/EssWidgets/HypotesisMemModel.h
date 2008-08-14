/*
 * HypotesisMemModel.h
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */
#ifndef HYPOTESISMEMMODEL_H_
#define HYPOTESISMEMMODEL_H_

#include "AbstractMemHypModel.h"
#include "EssentialClass.h"



namespace platon
{

class HypotesisMemModel: public AbstractMemHypModel
{
public:
	HypotesisMemModel(Eidos*, QObject * parent);
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
protected:
	virtual ExtraAttribute* getEAFromEidos(int i) const ;		//Процедура возвращает ссылку на экстраатрибут по номеру
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const;	//Функция возвращает заполненную SQL строку для получения 1 экстраатрибута
};

}

#endif /* HYPOTESISMEMMODEL_H_ */
