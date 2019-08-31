/*
 * HypotesisMemModel.h
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */
#ifndef LNKDHYPPRAGMAMEMMODEL_H_
#define LNKDHYPPRAGMAMEMMODEL_H_

#include "HypPragmaMemModel.h"
#include "EssentialClass.h"



namespace platon
{

class LnkdHypPragmaMemModel: public HypPragmaMemModel
{
	int ID_in;
public:
	LnkdHypPragmaMemModel(Eidos* InEidos,int IDIn ,QObject * parent);
protected:
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const;	//Функция возвращает заполненную SQL строку для получения 1 экстраатрибута

};
}

#endif
