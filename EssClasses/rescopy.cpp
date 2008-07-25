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

#include "rescopy.h"
#include "restype.h"



using namespace platon;

RESCopy::RESCopy(Hypotesis* MyType,IBPP::Timestamp DateCreation)
	:Pragma(MyType,DateCreation)
{
}
RESCopy::RESCopy(Hypotesis* MyType, long ID_Copy)
	:Pragma(MyType, ID_Copy)
{
}


RESCopy* RESCopy::Fork(long ID_ACTCopy_BelongFor)
{
//��������� ������� ����� �������� ������� ���������� �������
//������� � ����������� ������ ��� ��������������, ����� ����
//� �� LNK_ACTBelongFor ���������� �������� ���������� � ���������
// - ������������� ���������� �������� �� ������� ����������� ����������� ������
// � �� LNK_ForkedRES ���������� ������������� ���������� ������� �� ��������
//��������� ����������.
//�������� �� Quantity ������������� �������� ������� �� Remainder ������������
//���������� �������.
//��� ��� ��������� ��������������� ���������, ���� �������� ������ �������� �� ������� ��������
//������� ���������� ������ �� ����������� ��������� �������

        RESCopy* FosteredRESCopy=((RESType*)this->HostHypotesis)->AddRESCopy();


        RESCopy*Source=this;
        RESCopy*Destination=FosteredRESCopy;

        for(unsigned int i=0;i<Source->Attributes.size();i++)
        {
       	        //�� ������� ��������� ��� �������� ���������������
              	switch (((AssociatedExtraAttribute*)Source->Attributes[i])->EA->type)
                {
                        case ft_String:
                                Destination->GetEAByNum(i)->SetStringValue(Source->GetEAByNum(i)->GetStringValue());
                                break;
                        case ft_Integer:
                                Destination->GetEAByNum(i)->SetIntValue(Source->GetEAByNum(i)->GetIntValue());
                                break;
                        case ft_Float:
                                Destination->GetEAByNum(i)->SetFloatValue(Source->GetEAByNum(i)->GetFloatValue());
                                break;
                        case ft_Boolean:
                                Destination->GetEAByNum(i)->SetBoolValue(Source->GetEAByNum(i)->GetBoolValue());
                                break;
                        case ft_DateTime:
                                Destination->GetEAByNum(i)->SetDateTimeValue(Source->GetEAByNum(i)->GetDateTimeValue());
                                break;
                        case ft_RB:
                                Destination->GetEAByNum(i)->SetRefBookValue(Source->GetEAByNum(i)->GetRefBookValue());
                                break;
                        case ft_DLL:
                                Destination->GetEAByNum(i)->SetDLLValue(Source->GetEAByNum(i)->GetDLLValue());
                                break;
                        case ft_LinkHypotesis:
                                Destination->GetEAByNum(i)->SetLink2HValue(Source->GetEAByNum(i)->GetLink2HValue());
                                break;
                        case ft_LinkPragma:
                                Destination->GetEAByNum(i)->SetLink2PValue(Source->GetEAByNum(i)->GetLink2PValue());
                                break;
                        case ft_Security:
                                break;
                        default:
                                throw("��������� ��� ������ �� ��������������");
                }
        }

        FosteredRESCopy->GetEAByFieldName("PragmaDate")->SetDateTimeValue(this->HostEidos->DT);

        LNK_Value LNK;

        LNK.LinkTo=ID_ACTCopy_BelongFor;
        LNK.Ratio=1;
        FosteredRESCopy->GetEAByFieldName("LNK_ACTBelongFor")->SetLink2PValue(LNK);

        LNK.LinkTo=this->GetID();
        LNK.Ratio=1;
        FosteredRESCopy->GetEAByFieldName("LNK_ForkedRES")->SetLink2PValue(LNK);

        FosteredRESCopy->GetEAByFieldName("Quantity")->SetFloatValue(this->GetEAByFieldName("Remainder")->GetFloatValue());
        return FosteredRESCopy;
}
