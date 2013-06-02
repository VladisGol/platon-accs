/*
software core of accounting system "Platon".
Copyright (C) 2005-2009 Borisenkov S., Golovyrin V.

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

Ядро системы учета "Платон".
Авторами программы являются Борисенков Сергей Александрович и Головырин Владислав Владимирович, 2005-2009г.
Данная библиотека является свободным программным обеспечением. Вы вправе распространять ее и/или модифицировать
в соответствии с условиями версии 2.1 Стандартной Общественной Лицензии Ограниченного Применения GNU,
опубликованной Free Software Foundation.
Мы распространяем эту программу в надежде на то, что она будет вам полезной,
однако НЕ ПРЕДОСТАВЛЯЕМ НА НЕЕ НИКАКИХ ГАРАНТИЙ, в том числе ГАРАНТИИ ТОВАРНОГО СОСТОЯНИЯ ПРИ ПРОДАЖЕ и
ПРИГОДНОСТИ ДЛЯ ИСПОЛЬЗОВАНИЯ В КОНКРЕТНЫХ ЦЕЛЯХ.
Для получения более подробной информации ознакомьтесь со Стандартной Общественной Лицензией Ограниченного
Применений GNU.
Связаться с авторами программы вы можете по e-mail vladisgol@rambler.ru
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
					throw "Нет такого атрибута в базе";
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
					throw "Нет такого атрибута в базе";
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
	//Процедура формирует SQL инструкцию для отображения в GRID таблице список атрибутов для объектов класса ТИП
		std::string SqlString_select="";	//Переменная для формирования списка именованных полей
		std::string SqlString_from=" FROM GET_HYPOTESIS_LIST("+ToString(EidosID)+") ";	//Переменная для формирования источников информации

		for(unsigned int i=0; i<AttributesList.size();i++)	//Последовательно для всех означенных атрибутов
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
	//Процедура формирует SQL инструкцию для отображения в GRID таблице список атрибутов для объектов класса ТИП
		std::string SqlString_select="";	//Переменная для формирования списка именованных полей
		std::string SqlString_from=" FROM GET_PRAGMA_WITH_HIPOTESIS_LIST("+ToString(EidosID)+") ";	//Переменная для формирования источников информации

		for(unsigned int i=0; i<AttributesList.size();i++)	//Последовательно для всех означенных атрибутов
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
	//Процедура формирует SQL инструкцию для отображения в GRID таблице список атрибутов для объектов класса ТИП
		std::string SqlString_select="";	//Переменная для формирования списка именованных полей
		std::string SqlString_from=" FROM GET_PRAGMA_LIST("+ToString(EidosID)+","+ToString(HypotesisID)+") ";	//Переменная для формирования источников информации

		for(unsigned int i=0; i<AttributesList.size();i++)	//Последовательно для всех означенных атрибутов
		{
					SqlString_select.append(" ,"+AttributesList[i].AliasName+"."+AttributesList[i].FieldForRetVal+" AS "+AttributesList[i].FieldName);
					SqlString_from.append(" LEFT JOIN "+AttributesList[i].StoredProcedureName+"("+ToString(AttributesList[i].EAID)+") ");
					SqlString_from.append(AttributesList[i].AliasName+" ON ("+AttributesList[i].StoredProcedureName+".id_link=GET_PRAGMA_LIST.id)");
		}
	std::string SqlString="SELECT GET_PRAGMA_LIST.*"+SqlString_select+SqlString_from+" WHERE ID>0;";
		return SqlString;
	}

}



