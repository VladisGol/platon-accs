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
	PragmaMemModel(Hypotesis* InHyp, QObject * parent);
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
    int  GetColumnNumberByFieldName(QString FieldName);
protected:
	Hypotesis* MyHyp;
	virtual ExtraAttribute* getEAFromEidos(int i) const ;		//Процедура возвращает ссылку на экстраатрибут по номеру
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const;	//Функция возвращает заполненную SQL строку для получения 1 экстраатрибута

};

}

#endif /* PRAGMAMEMMODEL_H_ */
