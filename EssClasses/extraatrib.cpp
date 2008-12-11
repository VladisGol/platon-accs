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

#include "extraatrib.h"


namespace platon
{


std::string ToUpper(const std::string val)
{
        std::locale loc;
        std::string ForReturn;
        ForReturn.clear();
        for(unsigned int i=0;i<val.size();i++)
                ForReturn.append(ToString(toupper(val.at(i))));
        return ForReturn;
}
std::string Rtrim (const std::string val)
{
        unsigned int Pointer;
        for(unsigned int i=val.size()-1;i>0;i--)
                if(val.at(i)!=' '){Pointer=i;break;}
        return val.substr(0,Pointer+1);
}


void ExtraAttribute::SetEACaption(std::string value)
{
	if(Caption != value) {
		Caption = value;
                Save();
	}
}
std::string ExtraAttribute::GetEACaption()const
{
	return Caption;
}

long ExtraAttribute::Save()
{
//��������� ���������� ���������� ������ ��������� ��������

        if(this->id >0)                //������� �� �����
        if(this->RealRecordsCount()>0) //���������� (���������) �������� ������ �������������, � �������� ��� ������� � ��
        {
                throw ("������� ��� �������� ������ � ����");
        }
        //�������� ���������� ��������� �������������� � ����� ������ ������������������ ������ �������
        if(this->id_Class<=0)
        {
                throw ("������� �� ����������� �� � ������ ������");
        }
        if(this->FieldName=="")
        {
                throw ("��� ���� �� ����� ���� ������");
        }

        switch (this->type)
        {
                case ft_String:
                case ft_Integer:
                case ft_Boolean:
                case ft_Float:
                case ft_DateTime:
       	        case ft_RB:
                case ft_DLL:
                        break;
                case ft_LinkHypotesis:
                case ft_LinkPragma:
                {
                        if(this->Multilnk==true)
                                if(this->Visible==true || this->Temporality==true||this->Required==true)
                                        throw("�������� ������� ������� �������������� ������������� �����");
                        break;
                }
                case ft_Security:
                        break;
              	default:
                       	throw("��������� ��� ������ �� ��������������");
        }

        IBPP::Transaction LocalTr=IBPP::TransactionFactory(HostEidos->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement LocalST=IBPP::StatementFactory(HostEidos->DB, LocalTr);
        LocalTr->Start();

        long ID_returned;
        /*
CREATE PROCEDURE SET_EIDOSEXTRAATTRIB (
1    id integer,
2    id_eidos integer,
3    caption varchar(255),
4    fieldtype integer,
5    id_belongfor integer,
6    id_rb_describer integer,
7    dll_filename varchar(64),
8    dll_procname varchar(64),
9    fieldname varchar(64),
10   temporality integer,
11   required integer,
12   visible integer,
13   locked integer,
14   multilnk integer,
15   lnk_species varchar(64),
16   lnk_eidosid integer,
17   lnk_hypid integer,
18   lnk_needlist integer)
returns (
    id_out integer)
        */
        LocalST->Prepare("EXECUTE PROCEDURE SET_EIDOSEXTRAATTRIB(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);");

        LocalST->Set(1,(int32_t)this->id);
        LocalST->Set(2,(int32_t)HostEidos->GetID());
        LocalST->Set(3,this->Caption);
        LocalST->Set(4,(int32_t)this->type);
        LocalST->Set(5,(int32_t)this->belongTo);
        LocalST->Set(6,(int32_t)this->ID_RB_Describer);
        LocalST->Set(7,this->DLL_FileName);
        LocalST->Set(8,this->DLL_ProcName);
        LocalST->Set(9,this->FieldName);
        LocalST->Set(10,this->Temporality);
        LocalST->Set(11,this->Required);
        LocalST->Set(12,this->Visible);
        LocalST->Set(13,this->Locked);
        LocalST->Set(14,this->Multilnk);
        LocalST->Set(15,this->LNK_species);
        LocalST->Set(16,(int32_t)this->LNK_EidosID);
        LocalST->Set(17,(int32_t)this->LNK_HypID);
        LocalST->Set(18,this->LNK_NeedList);

        LocalST->Execute();
        LocalST->Get("ID_OUT",(int32_t*)&ID_returned);
        LocalTr->Commit();
        return ID_returned;
}

ExtraAttribute::ExtraAttribute()
{
//����������� ������. �������� ��� �������� ����� ��������� �� � 0 � �������� �� ���������
	this->id =0;
        this->Caption ="����� �������";
        this->id_Class =0;
        this->type = ft_String;
        this->belongTo =_theHypotesis;
        this->ID_RB_Describer =0;
        this->DLL_FileName="";
        this->DLL_ProcName="";
        this->FieldName="";
        this->Temporality =false;
        this->Required =false;
        this->Visible=true;
        this->Locked=false;
        this->Multilnk=false;
        this->LNK_species="";
        this->LNK_EidosID=0;
        this->LNK_HypID=0;
        this->LNK_NeedList=false;
}

ExtraAttribute::~ExtraAttribute()
{
//���������� ������.
        ;
}

std::string ExtraAttribute::GetClass_NodeName()const
{
//���������� ��� ������� � �������� ��������� ������������
	return strClass_NodeName;
}

long ExtraAttribute::GetEAID()const
{
//�������� ID ��������������� ��������
        return id;
}

void ExtraAttribute::SetEAFieldName(std::string value)
{
        if(FieldName != value) {
                FieldName = value;
                Save();
        }
}
std::string ExtraAttribute::GetEAFieldName()const
{
        return FieldName;
}

std::string ExtraAttribute::NameStoredProc()const
{
//������� ���������� ��� �������� ��������� ��� ������������ SQL ������� ������ FROM ���
//�������� ���� - ������
	return this->sNameStoredProc;
}

long ExtraAttribute::RealRecordsCount(void)
{
//������� ���������� ���������� ������� ������� �������������� � ������� ��� ������� ������� �
//���������� ���������� TableName, �������� ������� ��������������� � ������� ��������
        long ObjectCount;
        IBPP::Transaction LocalTr=IBPP::TransactionFactory(HostEidos->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement LocalST=IBPP::StatementFactory(HostEidos->DB, LocalTr);
        LocalTr->Start();
        std::string SQLForRequest="select count(ID_LINK) NumRec from "+this->NameStoredProc()+"(?);";
        LocalST->Prepare(SQLForRequest);
        LocalST->Set(1,(int32_t)this->GetEAID());
        LocalST->Execute();
        if(LocalST->Fetch())LocalST->Get("NumRec",(int32_t*)&ObjectCount);
        LocalTr->Commit();
        return ObjectCount;
}


std::string ExtraAttribute::TemporalListSPName()const
{return this->sTemporalListSPName;}

}
