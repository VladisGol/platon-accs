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
	long iterEidosID;
	long iterID_in;
public:
	LnkdHypPragmaMemModel(Eidos* InEidos,iterLNKS_Hyp* LnkdIterator ,QObject * parent);
protected:
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const;	//������� ���������� ����������� SQL ������ ��� ��������� 1 ��������������

};
}

#endif
