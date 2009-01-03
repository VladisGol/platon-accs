/*
 * HypotesysMemModel.cpp
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */

#include "LnkdHypMemModel.h"


namespace platon
{

LnkdHypMemModel::LnkdHypMemModel(Eidos* InEidos,iterLNKS_Hyp* LnkdIterator ,QObject * parent)
					:HypotesisMemModel(parent)
{
	setObjectName("LnkdHypMemModel");
	ForEidos =InEidos;

	NumCol=InEidos->HypotesisSQL->AttributesList.size();					//�������� ���������� ����� � �������
	ReservedColumns=1;														//���� ����������������� ���� ID

	Id_records = new QVector <long>;										//�������� ����������� ������
	FieldsInModel= new QVector <QMap<long,QVariant>*>;
	for(int i=0;i<NumCol;i++) FieldsInModel->append(new QMap<long,QVariant>);

	KeyIterator=LnkdIterator;												//���������� �������� ������ �������
	iterEidosID=LnkdIterator->EidosID;
	iterID_in=LnkdIterator->ID_in;

	ReadToBuffer();															//��������� �������� � �����
}

QString LnkdHypMemModel::getSQLstringforEA(ExtraAttribute*MyEA) const
{
	QString SQLString;
	SQLString="select ID_HYPOTESIS ID, id_link, meaning from GET_LINKED_HYPLIST("+QString::number(this->iterID_in)+") inner join ";
	SQLString=SQLString+QString::fromStdString(MyEA->NameStoredProc())+"("+QString::number(MyEA->GetEAID())+")";
	SQLString=SQLString+" on GET_LINKED_HYPLIST.ID_HYPOTESIS="+QString::fromStdString(MyEA->NameStoredProc())+".ID_LINK";
	SQLString=SQLString+" WHERE ID_EIDOS="+QString::number(this->iterEidosID)+";";
	return SQLString;
}


}

