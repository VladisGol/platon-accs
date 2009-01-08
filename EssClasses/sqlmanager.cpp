/*
software core of accounting system "Platon".
Copyright (C) 2005-2007 Borisenkov S., Golovyrin V.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation version 2.1 of the License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
Contacts: e-mail vladisgol@rambler.ru

���� ������� ����� "������".
�������� ��������� �������� ���������� ������ ������������� � ��������� ��������� ������������, 2005-2007�.
������ ���������� �������� ��������� ����������� ������������. �� ������ �������������� �� �/��� ��������������
� ������������ � ��������� ������ 2.1 ����������� ������������ �������� ������������� ���������� GNU,
�������������� Free Software Foundation.
�� �������������� ��� ��������� � ������� �� ��, ��� ��� ����� ��� ��������,
������ �� ������������� �� ��� ������� ��������, � ��� ����� �������� ��������� ��������� ��� ������� �
����������� ��� ������������� � ���������� �����.
��� ��������� ����� ��������� ���������� ������������ �� ����������� ������������ ��������� �������������
���������� GNU.
��������� � �������� ��������� �� ������ �� e-mail vladisgol@rambler.ru
*/
#include "sqlmanager.h"

namespace platon
{
	SQLManager::SQLManager(Eidos* Ei)
	{
			DB=Ei->DB;
			EidosID=Ei->GetID();
			Initialize();
	}
	SQLManager::~SQLManager()
	{
			AttributesList.clear();
	}
	void SQLManager::Initialize()
	{
			LocalTR=IBPP::TransactionFactory(this->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
			LocalST=IBPP::StatementFactory(this->DB, LocalTR);
	}
	void SQLManager::Clear()
	{
			AttributesList.clear();
	}
	void SQLManager::AddFieldVisibleValue(std::string FieldName)
	{
			if(!LocalTR->Started())LocalTR->Start();
			LocalST->Prepare("select * from GET_EIDOSEXTRAATTRIB_LIST(0) where fieldname ='"+FieldName+"';");
			LocalST->Execute();
			IsFetched=LocalST->Fetch();
			if(IsFetched)
			{
					HeaderEADescriber NewRec;

					LocalST->Get("FIELDNAME",NewRec.FieldName);
					LocalST->Get("NAMESTOREDPROC",NewRec.StoredProcedureName);
					NewRec.AliasName="t_"+NewRec.FieldName;
					NewRec.FieldForRetVal="MEANING";
					LocalST->Get("ID",(int32_t*)&NewRec.EAID);
					LocalST->Get("FIELDTYPE",NewRec.FieldType);
					LocalST->Get("CAPTION",NewRec.Caption);

					AttributesList.push_back(NewRec);

			}
			else
			{
					throw "��� ������ �������� � ����";
			}
	}
	void SQLManager::AddFieldVisibleValue(ExtraAttribute* EA)
	{
		HeaderEADescriber NewRec;
		NewRec.FieldName=EA->GetEAFieldName();
		NewRec.StoredProcedureName=EA->NameStoredProc();
		NewRec.AliasName="t_"+NewRec.FieldName;
		NewRec.FieldForRetVal="MEANING";
		NewRec.EAID=EA->GetEAID();
		NewRec.FieldType=EA->type;
		NewRec.Caption=EA->GetEACaption();

		AttributesList.push_back(NewRec);
	}
	void SQLManager::AddFieldKeyValue(std::string FieldName)
	{
			if(!LocalTR->Started())LocalTR->Start();
			LocalST->Prepare("select * from GET_EIDOSEXTRAATTRIB_LIST(0) where fieldname ='"+FieldName+"';");
			LocalST->Execute();
			IsFetched=LocalST->Fetch();
			if(IsFetched)
			{
					HeaderEADescriber NewRec;

					LocalST->Get("FIELDNAME",NewRec.FieldName);
					LocalST->Get("NAMESTOREDPROC",NewRec.StoredProcedureName);
					NewRec.AliasName="t_"+NewRec.FieldName;
					NewRec.FieldForRetVal="KEYVALUE";
					LocalST->Get("ID",(int32_t*)&NewRec.EAID);
					LocalST->Get("FIELDTYPE",NewRec.FieldType);
					LocalST->Get("CAPTION",NewRec.Caption);
					AttributesList.push_back(NewRec);
			}
			else
			{
					throw "��� ������ �������� � ����";
			}
	}
	void SQLManager::AddFieldKeyValue(ExtraAttribute* EA)
	{
			HeaderEADescriber NewRec;
			NewRec.FieldName=EA->GetEAFieldName();
			NewRec.StoredProcedureName=EA->NameStoredProc();
			NewRec.AliasName="t_"+NewRec.FieldName;
			NewRec.FieldForRetVal="KEYVALUE";
			NewRec.EAID=EA->GetEAID();
			NewRec.FieldType=EA->type;
			NewRec.Caption=EA->GetEACaption();
			AttributesList.push_back(NewRec);
			EA->GetEACaption();
	}
	HypotesisSQLManager::HypotesisSQLManager(Eidos* Ei):SQLManager(Ei){;}
	const std::string HypotesisSQLManager::SQLString()
	{
	//��������� ��������� SQL ���������� ��� ����������� � GRID ������� ������ ��������� ��� �������� ������ ���
		std::string SqlString_select="";	//���������� ��� ������������ ������ ����������� �����
		std::string SqlString_from=" FROM GET_HYPOTESIS_LIST("+ToString(EidosID)+") ";	//���������� ��� ������������ ���������� ����������

		for(unsigned int i=0; i<AttributesList.size();i++)	//��������������� ��� ���� ���������� ���������
		{
					SqlString_select.append(" ,"+AttributesList[i].AliasName+"."+AttributesList[i].FieldForRetVal+" AS "+AttributesList[i].FieldName);
					SqlString_from.append(" LEFT JOIN "+AttributesList[i].StoredProcedureName+"("+ToString(AttributesList[i].EAID)+") ");
					SqlString_from.append(AttributesList[i].AliasName+" ON ("+AttributesList[i].StoredProcedureName+".id_link=GET_HYPOTESIS_LIST.id)");
		}
	std::string SqlString="SELECT GET_HYPOTESIS_LIST.*"+SqlString_select+SqlString_from+" WHERE ID>0;";
		return SqlString;
	}
	HypotesisPragmaSQLManager::HypotesisPragmaSQLManager(Eidos* Ei):SQLManager(Ei){;}
	const std::string HypotesisPragmaSQLManager::SQLString()
	{
	//��������� ��������� SQL ���������� ��� ����������� � GRID ������� ������ ��������� ��� �������� ������ ���
		std::string SqlString_select="";	//���������� ��� ������������ ������ ����������� �����
		std::string SqlString_from=" FROM GET_PRAGMA_WITH_HIPOTESIS_LIST("+ToString(EidosID)+") ";	//���������� ��� ������������ ���������� ����������

		for(unsigned int i=0; i<AttributesList.size();i++)	//��������������� ��� ���� ���������� ���������
		{
					SqlString_select.append(" ,"+AttributesList[i].AliasName+"."+AttributesList[i].FieldForRetVal+" AS "+AttributesList[i].FieldName);
					SqlString_from.append(" LEFT JOIN "+AttributesList[i].StoredProcedureName+"("+ToString(AttributesList[i].EAID)+") ");
					SqlString_from.append(AttributesList[i].AliasName+" ON ("+AttributesList[i].StoredProcedureName+".id_link=GET_PRAGMA_WITH_HIPOTESIS_LIST.id)");
		}
	std::string SqlString="SELECT GET_PRAGMA_WITH_HIPOTESIS_LIST.*"+SqlString_select+SqlString_from+" WHERE ID>0;";
		return SqlString;
	}

	PragmaSQLManager::PragmaSQLManager(Hypotesis* Hi):SQLManager(Hi->HostEidos)
	{
		HypotesisID=Hi->GetID();
	}
	const std::string PragmaSQLManager::SQLString()
	{
	//��������� ��������� SQL ���������� ��� ����������� � GRID ������� ������ ��������� ��� �������� ������ ���
		std::string SqlString_select="";	//���������� ��� ������������ ������ ����������� �����
		std::string SqlString_from=" FROM GET_PRAGMA_LIST("+ToString(EidosID)+","+ToString(HypotesisID)+") ";	//���������� ��� ������������ ���������� ����������

		for(unsigned int i=0; i<AttributesList.size();i++)	//��������������� ��� ���� ���������� ���������
		{
					SqlString_select.append(" ,"+AttributesList[i].AliasName+"."+AttributesList[i].FieldForRetVal+" AS "+AttributesList[i].FieldName);
					SqlString_from.append(" LEFT JOIN "+AttributesList[i].StoredProcedureName+"("+ToString(AttributesList[i].EAID)+") ");
					SqlString_from.append(AttributesList[i].AliasName+" ON ("+AttributesList[i].StoredProcedureName+".id_link=GET_PRAGMA_LIST.id)");
		}
	std::string SqlString="SELECT GET_PRAGMA_LIST.*"+SqlString_select+SqlString_from+" WHERE ID>0;";
		return SqlString;
	}

}



