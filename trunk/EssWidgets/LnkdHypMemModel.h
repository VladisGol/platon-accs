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
	long ID_in;
public:
	LnkdHypMemModel(Eidos* InEidos,long IDIn ,QObject * parent);
protected:
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const;	//������� ���������� ����������� SQL ������ ��� ��������� 1 ��������������

};
}

#endif
