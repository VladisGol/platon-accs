/*
 * HypotesysMemModel.cpp
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */

#include "HypotesisMemModel.h"


namespace platon
{

HypotesisMemModel::HypotesisMemModel(Eidos* InEidos, QObject * parent)
					:AbstractMemHypModel(parent)
{
	setObjectName("HypotesisMemModel");
	ForEidos =InEidos;

	NumCol=InEidos->HypotesisSQL->AttributesList.size();					//�������� ���������� ����� � �������
	ReservedColumns=1;														//���� ����������������� ���� ID

	Id_records = new QVector <long>;										//�������� ����������� ������
	FieldsInModel= new QVector <QMap<long,QVariant>*>;
	for(int i=0;i<NumCol;i++) FieldsInModel->append(new QMap<long,QVariant>);

	KeyIterator=new iterHypotesis(ForEidos);								//���������� �������� ������ �������
	ReadToBuffer();															//��������� �������� � �����
}


ExtraAttribute* HypotesisMemModel::getEAFromEidos(int i) const
{
	std::string FieldName = this->ForEidos->HypotesisSQL->AttributesList[i].FieldName;
	return ForEidos->GetEAByFieldName(FieldName);
}

QString HypotesisMemModel::getSQLstringforEA(ExtraAttribute*MyEA) const
{
	QString SQLString;
	SQLString="select GET_HYPOTESIS_LIST.ID id, id_link, meaning from GET_HYPOTESIS_LIST("+QString::number(ForEidos->GetID())+") inner join ";
	SQLString=SQLString+QString::fromStdString(MyEA->NameStoredProc())+"("+QString::number(MyEA->GetEAID())+")";
	SQLString=SQLString+" on GET_HYPOTESIS_LIST.ID="+QString::fromStdString(MyEA->NameStoredProc())+".ID_LINK;";
	return SQLString;
}

QVariant HypotesisMemModel::headerData(int section, Qt::Orientation orientation,int role) const
{
	//��������� ������� �������� �������� �������� � �����

	if (role != Qt::DisplayRole)
	         return QVariant();

	if (orientation==Qt::Horizontal)
	{
		if(section==0)
			return "ID";
		else
			return tr(this->ForEidos->HypotesisSQL->AttributesList[section-ReservedColumns].Caption.c_str());
	}
	else
	{
		return QString::number(section);
	}
}
}

