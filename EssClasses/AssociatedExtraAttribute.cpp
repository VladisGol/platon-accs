/*
software core of accounting system "Platon".
Copyright (C) 2005-2010 Borisenkov S., Golovyrin V.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation version 2.1 of the License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License aint with this library; if not, write to the Free Software
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

#include "associatedextraattribute.h"


using namespace platon;

AssociatedExtraAttribute::AssociatedExtraAttribute(ExtraAttribute* InEA, Hypotesis* InHyp)
{
        if(InHyp==NULL) throw "Ошибка создания ассоциированного экстраатрибута без объекта ассоциации";
        this->OwnerHypotesis=InHyp;
        this->EA=InEA;
}


std::string AssociatedExtraAttribute::GetVisibleValue()const
{
//Процедура выводит видимое значение экстраатрибута, такое-же как выводится в recordset-е
//через SQL запрос
               	switch (this->EA->type)
	        {
        		case ft_String:
                                return this->GetStringValue();
        	        case ft_Integer:
                                return ToString(this->GetIntValue());
	              	case ft_Boolean:
                                return ToString(this->GetBoolValue());
        	        case ft_Float:
                                return ToString(this->GetFloatValue());
        	        case ft_DateTime:
                        {
                                IBPP::Timestamp MyTs=this->GetDateTimeValue();
                                return ToString(MyTs.Year())+"-"+ToString(MyTs.Month())+"-"+ToString(MyTs.Day());
                        }
	                case ft_DLL:
                        {
                                RoutineInOut MylocalIOR=this->GetDLLValue();
	                        return MylocalIOR.VisibleValue;
                        }
                        case ft_LinkHypotesis:
                        {
                                if(this->EA->Multilnk==false)
                                        return this->GetStringValue();
                                else
                                        return "";
                        }
                        case ft_LinkPragma:
                        {
                                if(this->EA->Multilnk==false)
                                        return this->GetStringValue();
                                else
                                        return "";
                        }
                        case ft_Security:
                                return ToString(this->GetIntValue());
                	default:
                        	throw("Указанный тип данных не поддерживается");
	        }
}

std::string AssociatedExtraAttribute::GetStringValue()const
{
//Получение строкового значения
        std::string ProcReturnValue="";
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("MEANING",ProcReturnValue);
        return ProcReturnValue;
}
int AssociatedExtraAttribute::GetIntValue()const
{
//Получение целочисленного значения
        int ProcReturnValue=0;
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("MEANING",ProcReturnValue);
        return ProcReturnValue;
}
bool AssociatedExtraAttribute::GetBoolValue()const
{
//Получение булевого значения
        bool ProcReturnValue=false;
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("MEANING",ProcReturnValue);
        return ProcReturnValue;
}
float AssociatedExtraAttribute::GetFloatValue() const
{
//Получение вещественного значения
        float ProcReturnValue=0;
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("MEANING",ProcReturnValue);
        return ProcReturnValue;

}
IBPP::Timestamp AssociatedExtraAttribute::GetDateTimeValue()const
{
//Получение значения даты-времени
        IBPP::Timestamp ProcReturnValue(1900,01,01);
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("MEANING",ProcReturnValue);
        return ProcReturnValue;

}
RoutineInOut AssociatedExtraAttribute::GetDLLValue()const
{
//Получение значения DLL
        RoutineInOut struVal;
        struVal.Key="";
        struVal.VisibleValue="";

        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Execute();

        if(Statement->Fetch())
        {
        	Statement->Get("MEANING",struVal.VisibleValue);
                Statement->Get("KEYVALUE",struVal.Key);
        }

        return struVal;
}
LNK_Value AssociatedExtraAttribute::GetLink2HValue()const
{
//Получение значения LNK
        if(this->EA->Multilnk==true) throw ("Операция для данного типа данных не поддерживается");

        LNK_Value ProcReturnValue;
        ProcReturnValue.LinkTo=0;
        ProcReturnValue.Ratio=1;


        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Execute();

        if(Statement->Fetch())
        {
                Statement->Get("KEYVALUE",ProcReturnValue.LinkTo);
                Statement->Get("RATIO",&ProcReturnValue.Ratio);
        }

        return ProcReturnValue;

}
LNK_Value AssociatedExtraAttribute::GetLink2PValue()const
{
        return GetLink2HValue();
}

void AssociatedExtraAttribute::SetStringValue(std::string In)
{
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();
        /*
        1 ID_EA_HEADER INTEGER,
        2 ID_LINK INTEGER,
        3 MEANING,
        4 DATE_OF_CHANGE TIMESTAMP)
        */
        Statement->Prepare("Execute procedure SET_EA_SIMPLE_STR(?,?,?,?)");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Set(3,In);     //MEANING
        Statement->Set(4,this->EA->DTValue);
        Statement->Execute();
        OwnerHypotesis->CommitProcedure();
}
void AssociatedExtraAttribute::SetIntValue(int In)
{
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();
        /*
        1 ID_EA_HEADER INTEGER,
        2 ID_LINK INTEGER,
        3 MEANING,
        4 DATE_OF_CHANGE TIMESTAMP)
        */
        Statement->Prepare("Execute procedure SET_EA_SIMPLE_INT(?,?,?,?)");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Set(3,In);     //MEANING
        Statement->Set(4,this->EA->DTValue);
        Statement->Execute();
        OwnerHypotesis->CommitProcedure();
}
void AssociatedExtraAttribute::SetBoolValue(bool In)
{
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();
        /*
        1 ID_EA_HEADER INTEGER,
        2 ID_LINK INTEGER,
        3 MEANING,
        4 DATE_OF_CHANGE TIMESTAMP)
        */
        Statement->Prepare("Execute procedure SET_EA_SIMPLE_INT(?,?,?,?)");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Set(3,In);     //MEANING
        Statement->Set(4,this->EA->DTValue);
        Statement->Execute();
        OwnerHypotesis->CommitProcedure();
}
void AssociatedExtraAttribute::SetFloatValue(float In)
{
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();
        /*
        1 ID_EA_HEADER INTEGER,
        2 ID_LINK INTEGER,
        3 MEANING,
        4 DATE_OF_CHANGE TIMESTAMP)
        */
        Statement->Prepare("Execute procedure SET_EA_SIMPLE_NUM(?,?,?,?)");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Set(3,In);     //MEANING
        Statement->Set(4,this->EA->DTValue);
        Statement->Execute();
        OwnerHypotesis->CommitProcedure();
}
void AssociatedExtraAttribute::SetDateTimeValue(IBPP::Timestamp In)
{
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();
        /*
        1 ID_EA_HEADER INTEGER,
        2 ID_LINK INTEGER,
        3 MEANING,
        4 DATE_OF_CHANGE TIMESTAMP)
        */
        Statement->Prepare("Execute procedure SET_EA_SIMPLE_DATE(?,?,?,?)");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Set(3,In);     //MEANING
        Statement->Set(4,this->EA->DTValue);
        Statement->Execute();
        OwnerHypotesis->CommitProcedure();
}
void AssociatedExtraAttribute::SetDLLValue(RoutineInOut In)
{
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();
        /*
        1  ID_EA_HEADER INTEGER,
        2  ID_LINK INTEGER,
        3  MEANING VARCHAR(255),
        4  KEYVALUE VARCHAR(255),
        5  DATE_OF_CHANGE TIMESTAMP)
        */
        Statement->Prepare("Execute procedure SET_EA_DLL(?,?,?,?,?)");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Set(3,In.VisibleValue);
        Statement->Set(4,In.Key);
        Statement->Set(5,this->EA->DTValue);
        Statement->Execute();
        OwnerHypotesis->CommitProcedure();
}
void AssociatedExtraAttribute::SetLink2HValue(LNK_Value In)
{
        if(this->EA->Multilnk==true) throw ("Операция для данного типа данных не поддерживается");

        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();
        /*
        1 ID_EA_HEADER INTEGER,
        2 ID_LINK INTEGER,
        3 MEANING,
        4 RATIO,
        5 DATE_OF_CHANGE TIMESTAMP)
        */
        Statement->Prepare("Execute procedure SET_EA_LNK(?,?,?,?,?)");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Set(3,In.LinkTo); //MEANING
        Statement->Set(4,In.Ratio);     //RATIO
        Statement->Set(5,this->EA->DTValue);
        Statement->Execute();
        OwnerHypotesis->CommitProcedure();
}
void AssociatedExtraAttribute::SetLink2PValue(LNK_Value In)
{
        SetLink2HValue(In);
}

int AssociatedExtraAttribute::SetMultiLNKValue(LNK_Value In, int ID_inlist)
{
// Функция вызывает хранимую процедуру для добавления или редактирования множественной ссылки
        int ProcReturnValue=0;

        if(this->EA->Multilnk==false) throw ("Операция для данного типа данных не поддерживается");

        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        /*
        PROCEDURE SET_EA_MULTILNK (
        1 ID_EA_HEADER INTEGER,
        2 ID_LINK INTEGER,
        3 KEYVALUE INTEGER,
        4 RATIO FLOAT,
        5 ID_INLIST INTEGER)

        RETURNS (
        ID_OUT INTEGER)
        */
        Statement->Prepare("Execute procedure SET_EA_MULTILNK(?,?,?,?,?)");
        Statement->Set(1,this->EA->GetEAID());
        Statement->Set(2,OwnerHypotesis->GetID());
        Statement->Set(3,In.LinkTo);
        Statement->Set(4,In.Ratio);                     //RATIO
        Statement->Set(5,ID_inlist);           //ID_INLIST
        Statement->Execute();
        Statement->Get("ID_OUT",ProcReturnValue);

        OwnerHypotesis->CommitProcedure();
        return ProcReturnValue;
}
void AssociatedExtraAttribute::DeleteMultiLNKValue(int ID_inlist)
{
	//Функция удаляет одно вхождение многострочной ссылки, идентификатор которого передан в параметре
    IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
    if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();
    /*
	1  ID_EA_HEADER INTEGER,
	2  ID_LINE
	*/
	Statement->Prepare("Execute procedure DEL_EA_MULTILNK_LINE(?,?);");
	Statement->Set(1,this->EA->GetEAID());
	Statement->Set(2,ID_inlist);
	Statement->Execute();

	OwnerHypotesis->CommitProcedure();
	return;
}
