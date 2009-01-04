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
	HypPragmaMemModel(Eidos* InEidos, QObject * parent);
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    void ReadToBuffer() const;
protected:
	Hypotesis* MyHyp;
	virtual ExtraAttribute* getEAFromEidos(int i) const ;		//Процедура возвращает ссылку на экстраатрибут по номеру
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const;	//Функция возвращает заполненную SQL строку для получения 1 экстраатрибута
	HypPragmaMemModel(QObject * parent):AbstractMemHypModel(parent){;};	//Скрытый конструктор для потомков использующих собственный конструктор
};

}

#endif /* HYPPRAGMAMEMMODEL_H_ */
