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

#include "pragma.h"


using namespace platon;

Pragma::Pragma(Hypotesis* MyHypotesis,IBPP::Timestamp DateCreation)
{
//����������� ������ ����� ��� �������� ������ �������� � ��. � �������� ����������
//���������� �������� ����, � ����� ������������� ������ � �������� ����������� �������
        BelongIdent=ExtraAttribute::_thePragma;
        Autocommited=true;
        this->HostEidos=MyHypotesis->HostEidos;
        this->HostHypotesis=MyHypotesis;
        Initialize();
        this->ID=0;             //����� �������
        this->ID=Save();        //���������� ��� � ���������� ������������� �������
        CopyExtraatributesFromHostEidos();
        this->GetEAByFieldName("PragmaDate")->SetDateTimeValue(DateCreation); //������� ���� �������� � ������������ �������������
        PragmaSQL= new PragmaSQLManager(this);
}
Pragma::Pragma(Hypotesis* MyHypotesis, long ID_Pragma)
{
//����������� ������ ����� ��� ���������� �� ���� ��� ���������� �������. � �������� ���������
//������������ ������������� ������� ����, ����������� � ��
        BelongIdent=ExtraAttribute::_thePragma;
        Autocommited=true;

        this->HostEidos=MyHypotesis->HostEidos;
      	this->HostHypotesis=MyHypotesis;

      	Initialize();

        IBPP::Statement LocalST=IBPP::StatementFactory(this->HostEidos->DB, TransactionIBPP);
        if(!TransactionIBPP->Started()) TransactionIBPP->Start();

        long ID_returned;
        LocalST->Prepare("EXECUTE PROCEDURE GET_PRAGMA(?);");
        LocalST->Set(1,(int32_t)ID_Pragma);
        LocalST->Execute();
        LocalST->Get("ID",(int32_t*)&ID_returned);
        CommitProcedure();

        //��������� ��������� �� ������������� ������������ ������� � ������������
        if(ID_returned==ID_Pragma)
        {
        	//�������� �������� ����������� � �� ��� ���������� �������
                this->ID=ID_returned;
                CopyExtraatributesFromHostEidos();
        }
        else
        {
        	throw "������ �� ������� ��������� ������������� �� ����������";
        }
        PragmaSQL= new PragmaSQLManager(this);
}

long Pragma::Save()
{
//��������� ��������� �������� ���������(�� ���������������) � ������� ����������� �������� �����
        IBPP::Statement LocalST=IBPP::StatementFactory(this->HostEidos->DB, TransactionIBPP);
        if(!TransactionIBPP->Started()) TransactionIBPP->Start();

        long ID_returned;
        LocalST->Prepare("EXECUTE PROCEDURE SET_PRAGMA(?,?,?);");
        LocalST->Set(1,(int32_t)this->ID);
        LocalST->Set(2,(int32_t)HostEidos->GetID());
        LocalST->Set(3,(int32_t)HostHypotesis->GetID());
        LocalST->Execute();
        LocalST->Get("ID_OUT",(int32_t*)&ID_returned);
        CommitProcedure();
        return ID_returned;
}

void Pragma::GetEidosHypotesisIDS(IBPP::Database MyDB, long ID_IN,long &ID_Eidos, long &ID_Hypotesis)
{
//����������� ��������� ������� ��������� ���������� �� ������� ���� Hypotesis �� ���� ������
//��� �������� ������ �������. ������������ ID Eidos � ��� ������������� � �������� ����������
//�������� ���� �������� � ID_Hypotesis

        IBPP::Transaction LocalTr=IBPP::TransactionFactory(MyDB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement LocalST=IBPP::StatementFactory(MyDB, LocalTr);
        LocalTr->Start();

        long ID_retHyp;
        long ID_retEidos;
        long ID;
        LocalST->Prepare("EXECUTE PROCEDURE GET_PRAGMA(?);");
        LocalST->Set(1,(int32_t)ID_IN);
        LocalST->Execute();
        LocalST->Get("ID_HYPOTESIS",(int32_t*)&ID_retHyp);
        LocalST->Get("ID_EIDOS",(int32_t*)&ID_retEidos);
        LocalST->Get("ID",(int32_t*)&ID);
        LocalTr->Commit();

        if(ID==ID_IN)
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

std::string Pragma::GetLinkedPragmaSQL(long IDLinkEA)const
{
//��������� ���������� ������ SQL ��� ������ ������ �������������� Pragma �� ��������������, ������������� ��������
//������� � ��������� (�� �����, � ������������� � ��)
//
        return "select * from GET_LINKED_PRAGMA_LIST("+ToString(IDLinkEA)+","+ToString(this->GetID())+");";
}

std::string Pragma::GetLinkedPragmaSQL(std::string LinkEAName)const
{
//��������� ���������� ������ SQL ��� ������ ������ �������������� Pragma �� ��������������, ��� ��������
//�������� � ���������

        return GetLinkedPragmaSQL(Get_EAID_ByName(this->HostEidos->DB,LinkEAName));
}

std::string Pragma::GetPragmaName()
{
//������� ���������� ��� ������ � ������� - ��� �������� + ���� ���������
        IBPP::Statement LocalST=IBPP::StatementFactory(this->HostEidos->DB, TransactionIBPP);
        if(!TransactionIBPP->Started()) TransactionIBPP->Start();

        std::string val_returned;
        LocalST->Prepare("select get_pragma_name_list.meaning TITLE from get_pragma_name_list where get_pragma_name_list.id=?;");
        LocalST->Set(1,(int32_t)this->ID);
        LocalST->Execute();
        if(LocalST->Fetch()) LocalST->Get("TITLE",val_returned);
        CommitProcedure();
        return val_returned;
}
