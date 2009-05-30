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
#include "piterator.h"

namespace platon
{
	pIterator::~pIterator()
	{
	}
        void pIterator::Initialize()
        {
                LocalTR=IBPP::TransactionFactory(this->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
                LocalST=IBPP::StatementFactory(this->DB, LocalTR);
                IsStarted=false;
                RowNum=0;
        }
        long pIterator::First()
        {
                if(!LocalTR->Started())LocalTR->Start();
                LocalST->Prepare(SQL_string);
                LocalST->Execute();
                IsFetched=LocalST->Fetch();
                IsStarted=true;
                RowNum=1;
                return this->GetID();
        }
        long pIterator::Next()
        {
                if(IsStarted)
                {
                        IsFetched=LocalST->Fetch();
                        RowNum++;
                        return this->GetID();
                }
                else return First();
        }

        bool pIterator::Fetched()
        {
                return IsFetched;
        }

	long pIterator::GetID()
	{
		//Процедура возвращает значение поля ID из текущей записи
        long RetVal=0;
        if(IsFetched) LocalST->Get("ID",(int32_t*)&RetVal);
		return RetVal;
	}
	long pIterator::GetRowNum()
	{
		return this->RowNum;
	}
	long pIterator::GetRowCount()
	{
		//Возвращаем число записей
		long RetVal;
		IBPP::Transaction TmpTR=IBPP::TransactionFactory(this->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
		IBPP::Statement TmpST=IBPP::StatementFactory(this->DB, TmpTR);
        TmpTR->Start();
        TmpST->Prepare(SQL_string_forreccount);
        TmpST->Execute();
        if(TmpST->Fetch()) TmpST->Get("recordscount",(int32_t*)&RetVal);
        return RetVal;
	}


	iterEidos::iterEidos(IBPP::Database inDB,std::string Species)
	{
        this->DB=inDB;
        Initialize();
        SQL_string="select id, id_parent,name TITLE from GET_EIDOS_LIST('"+Species+"');";
        SQL_string_forreccount="select count(id) recordscount from GET_EIDOS_LIST('"+Species+"');";
	}
	long iterEidos::GetParentID()
	{
		long RetVal=0;
        if(IsFetched) LocalST->Get("ID_PARENT",(int32_t*)&RetVal);
		return RetVal;
	}
	std::string iterEidos::GetTitle()
	{
		//Процедура возвращает значение наименования объекта, по которому выстраивается цикл
		//ВНИМАНИЕ, для корректного отображения поля необходимо действительное имя в базе переопределить псевдонимом TITLE
        std::string RetVal="";
        if(IsFetched) LocalST->Get("TITLE",RetVal);
		return RetVal;
	}

	iterHypotesis::iterHypotesis(Eidos* InEidos)
	{
		this->DB=InEidos->DB;
        Initialize();
        SQL_string="select get_hypotesis_name_list.id ID, get_hypotesis_name_list.meaning TITLE from get_hypotesis_name_list("+ToString(InEidos->GetID())+");";
        SQL_string_forreccount="select count(get_hypotesis_name_list.id) recordscount from get_hypotesis_name_list("+ToString(InEidos->GetID())+");";
	}
	iterHypotesis::iterHypotesis(IBPP::Database inDB,long ID_Eidos)
	{
		this->DB=inDB;
        Initialize();
        SQL_string="select get_hypotesis_name_list.id ID, get_hypotesis_name_list.meaning TITLE from get_hypotesis_name_list("+ToString(ID_Eidos)+");";
        SQL_string_forreccount="select count(get_hypotesis_name_list.id) recordscount from get_hypotesis_name_list("+ToString(ID_Eidos)+");";
	}
	std::string iterHypotesis::GetTitle()
	{
        std::string RetVal="";
        if(IsFetched) LocalST->Get("TITLE",RetVal);
		return RetVal;
	}
	iterPragma::iterPragma(Hypotesis* InHyp)
	{
        this->DB=InHyp->HostEidos->DB;
        Initialize();
        SQL_string="select get_pragma_list.id from get_pragma_list("+ToString(InHyp->HostEidos->GetID())+", "+ToString(InHyp->GetID())+");";
        SQL_string_forreccount="select count(get_pragma_list.id) recordscount from get_pragma_list("+ToString(InHyp->HostEidos->GetID())+", "+ToString(InHyp->GetID())+");";
	}

	iterPragma::iterPragma(IBPP::Database inDB,long ID_Eidos, long ID_Hypotesis)
	{
        this->DB=inDB;
        Initialize();
        SQL_string="select get_pragma_list.id from get_pragma_list("+ToString(ID_Eidos)+", "+ToString(ID_Hypotesis)+");";
        SQL_string_forreccount="select count(get_pragma_list.id) recordscount from get_pragma_list("+ToString(ID_Eidos)+", "+ToString(ID_Hypotesis)+");";
	}
	iterHypPragma::iterHypPragma(Eidos* InEidos)
	{
		this->DB=InEidos->DB;
		Initialize();
		SQL_string="SELECT GET_PRAGMA_WITH_HIPOTESIS_LIST.ID ID, GET_PRAGMA_WITH_HIPOTESIS_LIST.HYP_NAME TITLE  FROM GET_PRAGMA_WITH_HIPOTESIS_LIST("+ToString(InEidos->GetID())+");";
		SQL_string_forreccount="select count(GET_PRAGMA_WITH_HIPOTESIS_LIST.id) recordscount from GET_PRAGMA_WITH_HIPOTESIS_LIST("+ToString(InEidos->GetID())+");";
	}
	std::string iterHypPragma::GetTitle()
	{
        std::string RetVal="";
        if(IsFetched) LocalST->Get("TITLE",RetVal);
		return RetVal;
	}

	iterTemporalityListofOneEA::iterTemporalityListofOneEA(AssociatedExtraAttribute* OneAEA)
	{
		this->DB=OneAEA->OwnerHypotesis->HostEidos->DB;
		Initialize();
        SQL_string="select ID, DATE_TIME, MEANING from "+OneAEA->EA->TemporalListSPName()+"("+ToString(OneAEA->OwnerHypotesis->GetID())+","+ ToString(OneAEA->EA->GetEAID())+");";
	}
	IBPP::Timestamp iterTemporalityListofOneEA::GetGateTime()
	{
		IBPP::Timestamp RetVal;
		if(IsFetched) LocalST->Get("DATE_TIME",RetVal);
		return RetVal;
	}

	iterLNKS_HEidos::iterLNKS_HEidos(IBPP::Database inDB,long ID_in)
	{
        this->DB=inDB;
        Initialize();
        DetailIter=NULL;
		SQL_string="select ID_EIDOS ID from GET_LINKED_HYPLIST("+ToString(ID_in)+") GROUP BY ID_EIDOS;";
        SQL_string_forreccount="select count(id) recordscount from (select id_eidos id from GET_LINKED_HYPLIST("+ToString(ID_in)+") group by id_eidos)";
	}
	iterLNKS_Hyp::iterLNKS_Hyp(IBPP::Database inDB)
	{
        this->DB=inDB;
        Initialize();
		this->EidosID=0;
		this->ID_in=0;

	}
	void iterLNKS_Hyp::MasterChanged(long LEidosID,long ID_in_par)
	{
		this->EidosID=LEidosID;
		this->ID_in=ID_in_par;
		SQL_string="select ID_HYPOTESIS ID from GET_LINKED_HYPLIST("+ToString(ID_in)+") WHERE ID_EIDOS="+ToString(LEidosID)+";";
        SQL_string_forreccount="select count(ID_HYPOTESIS) recordscount from GET_LINKED_HYPLIST("+ToString(ID_in)+") WHERE ID_EIDOS="+ToString(LEidosID)+";";
	}

	iterLNKS_PEidos::iterLNKS_PEidos(IBPP::Database inDB,long ID_in)
	{
        this->DB=inDB;
        Initialize();
        DetailIter=NULL;
		SQL_string="select ID_EIDOS ID from GET_LINKED_PRAGMALIST("+ToString(ID_in)+") GROUP BY ID_EIDOS;";
        SQL_string_forreccount="select count(id) recordscount from (select id_eidos id from GET_LINKED_PRAGMALIST("+ToString(ID_in)+") group by id_eidos)";
	}
	iterLNKS_Pragma::iterLNKS_Pragma(IBPP::Database inDB)
	{
        this->DB=inDB;
        Initialize();
		this->EidosID=0;
		this->ID_in=0;
	}

	void iterLNKS_Pragma::MasterChanged(long LEidosID,long ID_in_par)
	{
		this->EidosID=LEidosID;
		this->ID_in=ID_in_par;
		SQL_string="select ID_PRAGMA ID, GET_PRAGMA_WITH_HIPOTESIS_LIST.HYP_NAME TITLE from GET_LINKED_PRAGMALIST("+ToString(ID_in)+") INNER JOIN GET_PRAGMA_WITH_HIPOTESIS_LIST("+ToString(LEidosID)+") ON (GET_PRAGMA_WITH_HIPOTESIS_LIST.ID=GET_LINKED_PRAGMALIST.ID_PRAGMA) WHERE ID_EIDOS="+ToString(LEidosID)+";";
        SQL_string_forreccount="select count(ID_PRAGMA) recordscount from GET_LINKED_PRAGMALIST("+ToString(ID_in)+") WHERE ID_EIDOS="+ToString(LEidosID)+";";
	}

	iterMultilink::iterMultilink(AssociatedExtraAttribute* OneAEA)
	{
		this->DB=OneAEA->OwnerHypotesis->HostEidos->DB;
		Initialize();
		if(OneAEA->EA->Multilnk && (OneAEA->EA->type==ft_LinkHypotesis ||OneAEA->EA->type==ft_LinkPragma ))
		{
			if(OneAEA->EA->type==ft_LinkHypotesis)
			{
				SQL_string="SELECT p.MEANING TITLE, p.ID_LINK, p.KEYVALUE, p.RATIO, p.ID ID FROM GET_EA_LIST_MULTILNKH("+ToString(OneAEA->EA->GetEAID())+","+ToString(OneAEA->OwnerHypotesis->GetID())+") p;";
				SQL_string_forreccount="SELECT count(p.ID) recordscount FROM GET_EA_LIST_MULTILNKH("+ToString(OneAEA->EA->GetEAID())+","+ToString(OneAEA->OwnerHypotesis->GetID())+") p;";
			}
			else //ft_LinkPragma
			{
				SQL_string="SELECT p.MEANING TITLE, p.ID_LINK, p.KEYVALUE, p.RATIO, p.ID ID FROM GET_EA_LIST_MULTILNKP("+ToString(OneAEA->EA->GetEAID())+","+ToString(OneAEA->OwnerHypotesis->GetID())+") p;";
				SQL_string_forreccount="SELECT count(p.ID) recordscount FROM GET_EA_LIST_MULTILNKP("+ToString(OneAEA->EA->GetEAID())+","+ToString(OneAEA->OwnerHypotesis->GetID())+") p;";
			}
		}
		else throw("Неверное использование итератора");
	}

	std::string iterMultilink::GetTitle()
	{
        std::string RetVal="";
        if(IsFetched) LocalST->Get("TITLE",RetVal);
		return RetVal;
	}
	LNK_Value iterMultilink::GetLNKValue()
	{
        LNK_Value ProcReturnValue;
        ProcReturnValue.LinkTo=0;
        ProcReturnValue.Ratio=1;
        if(IsFetched)
		{
        	LocalST->Get("KEYVALUE",(int32_t*)&ProcReturnValue.LinkTo);
        	LocalST->Get("RATIO",ProcReturnValue.Ratio);
		}
        return ProcReturnValue;
	}
}



