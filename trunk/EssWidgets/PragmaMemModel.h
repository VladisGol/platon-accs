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
	virtual ExtraAttribute* getEAFromEidos(int i) const ;		//��������� ���������� ������ �� ������������� �� ������
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const;	//������� ���������� ����������� SQL ������ ��� ��������� 1 ��������������

};

}

#endif /* PRAGMAMEMMODEL_H_ */
