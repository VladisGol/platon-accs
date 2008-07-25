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

#include "acttype.h"


using namespace platon;

ACTType::ACTType()
		:Hypotesis()
{
}

ACTType::ACTType(Eidos* MyACTClass,std::string NameType)
		:Hypotesis(MyACTClass,NameType)
{
}

ACTType::ACTType(Eidos* MyACTClass, long ID_Type)
		:Hypotesis(MyACTClass,ID_Type)
{
}


ACTCopy* ACTType::GetACTCopy(long ACTID)
{
//��������� ������� ������ ���� ACTCopy ������������� � �������� ������ �������� �����
//�� ��������� �������������� ������������ � ���������
	ACTCopy* TmpPtr= new ACTCopy(this,ACTID);
        return TmpPtr;
}

ACTCopy* ACTType::AddACTCopy(long OBJID)
{
//��������� ������� ������ ���� ACTCopy ������������� � �������� ������ �������� �����
//�� ��������� �������������� ������������ � ���������
	ACTCopy* TmpPtr= new ACTCopy(this,this->HostEidos->DT,OBJID);
        return TmpPtr;
}


