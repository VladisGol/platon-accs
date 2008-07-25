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

#include "associatedextraattribute.h"


using namespace platon;

AssociatedExtraAttribute::AssociatedExtraAttribute(ExtraAttribute* InEA, Hypotesis* InHyp)
{
        if(InHyp==NULL) throw "������ �������� ���������������� �������������� ��� ������� ����������";
        this->OwnerHypotesis=InHyp;
        this->EA=InEA;
}


std::string AssociatedExtraAttribute::GetVisibleValue()const
{
//��������� ������� ������� �������� ��������������, �����-�� ��� ��������� � recordset-�
//����� SQL ������
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
        	        case ft_RB:
                        {
                	        return this->GetStringValue();
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
                        	throw("��������� ��� ������ �� ��������������");
	        }
}

std::string AssociatedExtraAttribute::GetStringValue()const
{
//��������� ���������� ��������
        std::string ProcReturnValue="";
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("MEANING",ProcReturnValue);
        return ProcReturnValue;
}
int AssociatedExtraAttribute::GetIntValue()const
{
//��������� �������������� ��������
        int ProcReturnValue=0;
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("MEANING",(int32_t*)&ProcReturnValue);
        return ProcReturnValue;
}
bool AssociatedExtraAttribute::GetBoolValue()const
{
//��������� �������� ��������
        bool ProcReturnValue=false;
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("MEANING",ProcReturnValue);
        return ProcReturnValue;
}
float AssociatedExtraAttribute::GetFloatValue() const
{
//��������� ������������� ��������
        float ProcReturnValue=0;
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("MEANING",ProcReturnValue);
        return ProcReturnValue;

}
IBPP::Timestamp AssociatedExtraAttribute::GetDateTimeValue()const
{
//��������� �������� ����-�������
        IBPP::Timestamp ProcReturnValue(1900,01,01);
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("MEANING",ProcReturnValue);
        return ProcReturnValue;

}
RoutineInOut AssociatedExtraAttribute::GetDLLValue()const
{
//��������� �������� DLL
        RoutineInOut struVal;
        struVal.Key="";
        struVal.VisibleValue="";

        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
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
//��������� �������� LNK
        if(this->EA->Multilnk==true) throw ("�������� ��� ������� ���� ������ �� ��������������");

        LNK_Value ProcReturnValue;
        ProcReturnValue.LinkTo=0;
        ProcReturnValue.Ratio=1;


        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Execute();

        if(Statement->Fetch())
        {
                Statement->Get("KEYVALUE",(int32_t*)&ProcReturnValue.LinkTo);
                Statement->Get("RATIO",&ProcReturnValue.Ratio);
        }

        return ProcReturnValue;

}
LNK_Value AssociatedExtraAttribute::GetLink2PValue()const
{
        return GetLink2HValue();
}

long AssociatedExtraAttribute::GetRefBookValue()const
{
//��������� �������������� �����������
        long ProcReturnValue=0;
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();

        Statement->Prepare("Select * from "+this->EA->NameStoredProc()+"(?) where ID_LINK =?;");
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("KEYVALUE",(int32_t*)&ProcReturnValue);
        return ProcReturnValue;
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
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
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
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Set(3,(int32_t)In);     //MEANING
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
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
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
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
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
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
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
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Set(3,In.VisibleValue);
        Statement->Set(4,In.Key);
        Statement->Set(5,this->EA->DTValue);
        Statement->Execute();
        OwnerHypotesis->CommitProcedure();
}
void AssociatedExtraAttribute::SetLink2HValue(LNK_Value In)
{
        if(this->EA->Multilnk==true) throw ("�������� ��� ������� ���� ������ �� ��������������");

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
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Set(3,(int32_t)In.LinkTo); //MEANING
        Statement->Set(4,In.Ratio);     //RATIO
        Statement->Set(5,this->EA->DTValue);
        Statement->Execute();
        OwnerHypotesis->CommitProcedure();
}
void AssociatedExtraAttribute::SetLink2PValue(LNK_Value In)
{
        SetLink2HValue(In);
}


void AssociatedExtraAttribute::SetRefBookValue(long In)
{
        IBPP::Statement Statement=IBPP::StatementFactory(this->OwnerHypotesis->HostEidos->DB, this->OwnerHypotesis->TransactionIBPP);
        if(!this->OwnerHypotesis->TransactionIBPP->Started()) this->OwnerHypotesis->TransactionIBPP->Start();
        /*
        1  ID_EA_HEADER INTEGER,
        2  ID_LINK INTEGER,
        3  MEANING INTEGER,
        4  ID_RB INTEGER,
        5  DATE_OF_CHANGE TIMESTAMP)
        */
        Statement->Prepare("Execute procedure SET_EA_RB(?,?,?,?,?)");
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Set(3,(int32_t)In);
        Statement->Set(4,(int32_t)this->EA->ID_RB_Describer);
        Statement->Set(5,this->EA->DTValue);
        Statement->Execute();
        OwnerHypotesis->CommitProcedure();
}

long AssociatedExtraAttribute::SetMultiLNKValue(LNK_Value In, long ID_inlist)
{
// ������� �������� �������� ��������� ��� ���������� ��� �������������� ������������� ������
        long ProcReturnValue=0;

        if(this->EA->Multilnk==false) throw ("�������� ��� ������� ���� ������ �� ��������������");

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
        Statement->Set(1,(int32_t)this->EA->GetEAID());
        Statement->Set(2,(int32_t)OwnerHypotesis->GetID());
        Statement->Set(3,(int32_t)In.LinkTo);           //MEANING
        Statement->Set(4,In.Ratio);                     //RATIO
        Statement->Set(5,(int32_t)ID_inlist);           //ID_INLIST
        Statement->Execute();
        if(Statement->Fetch()) Statement->Get("ID_OUT",(int32_t*)&ProcReturnValue);
        OwnerHypotesis->CommitProcedure();
        return ProcReturnValue;
}
/*
void DLLExtraAttrib::LaunchDLLProc(RoutineInOut* InValueStru)
{
//��������� ��������� ��������� ����������� � �������������� � ���������� �������� � ���� ���������
	RoutineInOut OutValueStru;
//        void(__stdcall * DLLRoutine)(RoutineInOut*,ExtraAttribute*,TDM*);

        return ;

	TLocateOptions Opts;
        Opts.Clear();
        Variant locvalues[2];
        locvalues[0] = this->DLL_FileName.c_str();
        locvalues[1] = this->DLL_ProcName.c_str();

        if(DM->MemTableEh_Plugins->Locate("DLL_name;Name", VarArrayOf(locvalues, 1),Opts))
        {

                DLL_HINSTANCE * MyDLLhinst=((DLL_HINSTANCE *)DM->DLL_Hints->Items[DM->MemTableEh_Plugins->FieldByName("ListID")->AsInteger]);
                DLLRoutine =(void (__stdcall *)(RoutineInOut*,ExtraAttribute*,TDM*)) GetProcAddress(MyDLLhinst->DLL_HINSTANCE,this->DLL_ProcName.c_str());

                if(DLLRoutine!=NULL)
                {
	                try
        	        {
	                        DLLRoutine(InValueStru,this,DM);
	                }
        	        catch(...)
                	{
                		std::cerr << "���� ������ ������������ ���������";
	                }
                }
                else
                {
                	std::cerr << "��� ������� "+this->DLL_ProcName+" � ���������� "+this->DLL_FileName+" ����������� ����� ���������";
                }
        }����� ���������� ��� ���������� ������ DM
}*/




