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
	virtual ExtraAttribute* getEAFromEidos(int i) const ;		//��������� ���������� ������ �� ������������� �� ������
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const;	//������� ���������� ����������� SQL ������ ��� ��������� 1 ��������������

};

}

#endif /* HYPPRAGMAMEMMODEL_H_ */
