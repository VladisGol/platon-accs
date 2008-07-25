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


#include "actcopy.h"


using namespace platon;

ACTCopy::ACTCopy(Hypotesis* MyType,IBPP::Timestamp DateCreation)
	:Pragma(MyType,DateCreation)
{
}
ACTCopy::ACTCopy(Hypotesis* MyType, long ID_Copy)
	:Pragma(MyType, ID_Copy)
{
}
ACTCopy::ACTCopy(Hypotesis* MyType,IBPP::Timestamp DateCreation, long OBJ_ID)
	:Pragma(MyType,DateCreation)
{
//�������� ������������� � ������� ����� � ���������� 1
        LNK_Value LNK;
        LNK.LinkTo=OBJ_ID;
        LNK.Ratio=1;


        this->GetEAByFieldName("LnkOBJ")->SetLink2PValue(LNK);
}



RESCopy* ACTCopy::AddSpentRESCopy(RESCopy* OneItem)
{
//��������� ������� ����� ������� ����������� � ���������, ����������
//����������� ��������� ������� � �������� ���������� ��������

        return OneItem->Fork(this->GetID());
}

