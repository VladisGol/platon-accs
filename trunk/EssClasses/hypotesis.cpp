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

#include "hypotesis.h"

using namespace platon;

Hypotesis::Hypotesis(Eidos* MyEidos,std::string NameHypotesis)
{
//����������� ������ ����� ��� �������� ������ �������� � ��. � �������� ����������
//���������� �������� ����, � ����� ������������� ������ � �������� ����������� �������
        BelongIdent=ExtraAttribute::_theHypotesis;
        this->HostEidos=MyEidos;
        Autocommited=false;
        Initialize();
        this->ID=0;             //����� �������
        this->ID=Save();        //���������� � ���������� ������������� �������
        CopyExtraatributesFromHostEidos();
        this->GetEAByFieldName("HypName")->SetStringValue(NameHypotesis);   //������� ��� � ������������ �������������
        PragmaSQL= new PragmaSQLManager(this);
}

Hypotesis::Hypotesis(Eidos* MyEidos, long ID_Hypotesis)
{
//����������� ������ ����� ��� ���������� �� ���� ��� ���������� �������. � �������� ���������
//������������ ������������� ������� ����, ����������� � ��
	//��������� ������ ������, ���������� � ��
        BelongIdent=ExtraAttribute::_theHypotesis;
        this->HostEidos=MyEidos;
        Autocommited=false;
        Initialize();

        IBPP::Statement LocalST=IBPP::StatementFactory(this->HostEidos->DB, TransactionIBPP);
        if(!TransactionIBPP->Started()) TransactionIBPP->Start();

        long ID_returned;
        LocalST->Prepare("EXECUTE PROCEDURE GET_HYPOTESIS(?);");
        LocalST->Set(1,(int32_t)ID_Hypotesis);
        LocalST->Execute();
        LocalST->Get("ID",(int32_t*)&ID_returned);
        CommitProcedure();

        //��������� ��������� �� ������������� ������������ ������� � ������������
        if(ID_returned==ID_Hypotesis)
        {
        	//�������� �������� ����������� � �� ��� ���������� �������
                this->ID=ID_returned;
                CopyExtraatributesFromHostEidos();
                PragmaSQL= new PragmaSQLManager(this);
        }
        else
        {
        	throw "������ �� ������� ��������� ������������� �� ����������";
        }
}

void Hypotesis::Initialize()
{
//��������� �������������� ���������� ������ OBJType ���������� �� ���� ����� �� �������������
//������������� ������������

        Attributes.clear();
        TransactionIBPP=IBPP::TransactionFactory(this->HostEidos->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
}

Hypotesis::~Hypotesis()
{
//���������� ������ ������� ��� ������� ����������� � ��������� ������
        CleanExtraAttributes();
        Attributes.clear();
        delete PragmaSQL;
}

long Hypotesis::GetID()const
{
//������ �������� ���� ID
	return ID;
}

void Hypotesis::CopyExtraatributesFromHostEidos()
{
//��������� �������� ������ �� ������� ��������������� � ������ �������, �������������� �� ��
//���, ������� ��������� � ������� ������, ������������� �������� � ����������
		ExtraAttribute* tmpAttrib;
        Attributes.clear();
        for(unsigned int i=0;i<this->HostEidos->Attributes.size();i++)
        {
                tmpAttrib=(ExtraAttribute*)this->HostEidos->Attributes[i];
                if(tmpAttrib->belongTo==BelongIdent)
                {
                        AssociatedExtraAttribute* MyAttrib = new AssociatedExtraAttribute(tmpAttrib,this);
                        Attributes.push_back(MyAttrib);
                }
        }
}

int Hypotesis::NumEAByFieldName(std::string FieldName)const
{
//������� ������� � ���������� �� ����� �������� ��� ����� � ������ ���� ������ �� ������, ������������ -1
        AssociatedExtraAttribute* tmpAttrib;
        int for_return=-1;
        for(unsigned int i=0;i < this->Attributes.size();i++)
        {
                tmpAttrib=(AssociatedExtraAttribute*)this->Attributes[i];
                if(ToUpper(tmpAttrib->EA->GetEAFieldName()) == ToUpper(FieldName))
                {
                	for_return=i;
                	break;
                }
        }
        return for_return;
}

int Hypotesis::NumEAByID(long FieldID)const
{
//������� ������� � ���������� �� ����� �������� ��� ����� � ������ ���� ������ �� ������, ������������ -1
        AssociatedExtraAttribute* tmpAttrib;
        int for_return=-1;
        for(unsigned int i=0;i<this->Attributes.size();i++)
        {
                tmpAttrib=(AssociatedExtraAttribute*)this->Attributes[i];
                if(tmpAttrib->EA->GetEAID() == FieldID)
                {
                	for_return=i;
                	break;
                }
        }
        return for_return;
}



AssociatedExtraAttribute* Hypotesis::GetEAByFieldName(std::string FieldName)
{
//��������� ���������� ������ �� ������������� �� ����� ���� ��������������

        AssociatedExtraAttribute* tmpAttrib=NULL;
        int Number = NumEAByFieldName(FieldName);
        if(Number>-1) tmpAttrib=(AssociatedExtraAttribute*)this->Attributes[Number];
        return tmpAttrib;
}

AssociatedExtraAttribute* Hypotesis::GetEAByFieldID(long FieldID)
{
//��������� ���������� ������ �� ������������� �� ����� ���� ��������������

        AssociatedExtraAttribute* tmpAttrib=NULL;
        int Number = NumEAByID(FieldID);
        if(Number>-1) tmpAttrib=(AssociatedExtraAttribute*)this->Attributes[Number];
        return tmpAttrib;
}

AssociatedExtraAttribute* Hypotesis::GetEAByNum(int Number)
{
//��������� ���������� ������ �� ������������� �� ������ �������������� � ������
        AssociatedExtraAttribute* tmpAttrib=NULL;
        if(Number>-1) tmpAttrib=(AssociatedExtraAttribute*)this->Attributes[Number];
        return tmpAttrib;
}

long Hypotesis::Save()
{
//��������� ��������� �������� ���������(�� ���������������) � ������� ����� ��������
        IBPP::Statement LocalST=IBPP::StatementFactory(this->HostEidos->DB, TransactionIBPP);
        if(!TransactionIBPP->Started()) TransactionIBPP->Start();

        long ID_returned;
        LocalST->Prepare("EXECUTE PROCEDURE SET_HYPOTESIS(?,?);");
        LocalST->Set(1,(int32_t)this->ID);
        LocalST->Set(2,(int32_t)HostEidos->GetID());
        LocalST->Execute();
        LocalST->Get("ID_OUT",(int32_t*)&ID_returned);
        CommitProcedure();
        return ID_returned;
}


void Hypotesis::SetHypotesName(std::string value)
{
        this->GetEAByFieldName("HypName")->SetStringValue(value);   //������� ��� � ������������ �������������
        Save();
}
std::string Hypotesis::GetHypotesName()
{
        return this->GetEAByFieldName("HypName")->GetStringValue();
}

void Hypotesis::CommitProcedure()
{
//��������� ��������� ���������� � ������ ���� ���������� ������ Autocommited=true
        if(Autocommited==true)
        {
                TransactionIBPP->Commit();
        }
}


void Hypotesis::GetEidosHypotesisIDS(IBPP::Database MyDB, long ID_IN,long &ID_Eidos, long &ID_Hypotesis)
{
//����������� ��������� ������� ��������� ���������� �� ������� ���� Hypotesis �� ���� ������
//��� �������� ������ �������. ������������ ID Eidos � ��� ������������� � �������� ����������
//�������� ���� �������� � ID_Hypotesis

        IBPP::Transaction LocalTr=IBPP::TransactionFactory(MyDB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement LocalST=IBPP::StatementFactory(MyDB, LocalTr);
        LocalTr->Start();

        long ID_retHyp;
        long ID_retEidos;
        LocalST->Prepare("EXECUTE PROCEDURE GET_HYPOTESIS(?);");
        LocalST->Set(1,(int32_t)ID_IN);
        LocalST->Execute();
        LocalST->Get("ID",(int32_t*)&ID_retHyp);
        LocalST->Get("ID_EIDOS",(int32_t*)&ID_retEidos);
        LocalTr->Commit();

        if(ID_retHyp==ID_IN)
        {
        	//�������� �������� ����������� � �� ��� ���������� �������
                ID_Eidos=ID_retEidos;
                ID_Hypotesis=ID_retHyp;
        }
        else
        {
                //���������� ��� ����
                ID_Eidos=0;
                ID_Hypotesis=0;
        }
}

void Hypotesis::CleanExtraAttributes()
{
//��������� ������� ��� ��������� � ������������ ������� �������� �� ������
	for(unsigned int i =0;i<Attributes.size();i++) delete ((AssociatedExtraAttribute*)Attributes[i]);
        Attributes.clear();
}

void Hypotesis::FillEASQLProp()
{
//��������� ��������� ������ ��������������� SQL ����� ��� ������
        PragmaSQL->Clear();
        ExtraAttribute* OneAttrib;	//��������� �� ������� ������� ������ ��������������� ��������
        for(int i=this->HostEidos->Attributes.size()-1;i>=0;i--)	//��������������� ��� ���� �������������� ���������
        {
                OneAttrib=(ExtraAttribute*)this->HostEidos->Attributes[i];
                if(OneAttrib->belongTo==ExtraAttribute::_thePragma && OneAttrib->Visible)
                {
                        PragmaSQL->AddFieldVisibleValue(OneAttrib);
                }
        }
}

