/*
 * HypotesisMemModel.h
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */
#ifndef LNKDHYPOTESISMEMMODEL_H_
#define LNKDHYPOTESISMEMMODEL_H_

#include "HypotesisMemModel.h"
#include "EssentialClass.h"



namespace platon
{

class LnkdHypMemModel: public HypotesisMemModel
{
	long iterEidosID;
	long iterID_in;
public:
	LnkdHypMemModel(Eidos* InEidos,iterLNKS_Hyp* LnkdIterator ,QObject * parent);
protected:
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const;	//Функция возвращает заполненную SQL строку для получения 1 экстраатрибута

};
}

#endif
