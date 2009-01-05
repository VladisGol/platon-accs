/*
 * HypotesysMemModel.cpp
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */

#include "LnkdHypMemModel.h"


namespace platon
{

LnkdHypMemModel::LnkdHypMemModel(Eidos* InEidos,long IDin ,QObject * parent)
					:HypotesisMemModel(parent)
{
	setObjectName("LnkdHypMemModel");
	ForEidos =InEidos;

	NumCol=InEidos->HypotesisSQL->AttributesList.size();					//�������� ���������� ����� � �������
	ReservedColumns=1;														//���� ����������������� ���� ID

	Id_records = new QVector <long>;										//�������� ����������� ������
	FieldsInModel= new QVector <QMap<long,QVariant>*>;
	for(int i=0;i<NumCol;i++) FieldsInModel->append(new QMap<long,QVariant>);

	KeyIterator= new iterLNKS_Hyp(ForEidos->DB);							//���������� �������� ������ �������
	((iterLNKS_Hyp*)KeyIterator)->MasterChanged(ForEidos->GetID(),IDin);

	ID_in=IDin;

	ReadToBuffer();															//��������� �������� � �����
}

QString LnkdHypMemModel::getSQLstringforEA(ExtraAttribute*MyEA) const
{
	QString SQLString;
	SQLString="select ID_HYPOTESIS ID, id_link, meaning from GET_LINKED_HYPLIST("+QString::number(this->ID_in)+") inner join ";
	SQLString=SQLString+QString::fromStdString(MyEA->NameStoredProc())+"("+QString::number(MyEA->GetEAID())+")";
	SQLString=SQLString+" on GET_LINKED_HYPLIST.ID_HYPOTESIS="+QString::fromStdString(MyEA->NameStoredProc())+".ID_LINK";
	SQLString=SQLString+" WHERE ID_EIDOS="+QString::number(this->ForEidos->GetID())+";";
	return SQLString;
}


}

