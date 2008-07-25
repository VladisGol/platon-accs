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

#include "actclass.h"


using namespace platon;


ACTClass::ACTClass(IBPP::Database MyDB, long ID_IN)
		:Eidos(MyDB, ID_IN)
{
	//������������� ����������� � �������� ��������� �������� �������������
        //������� ������
	;
}

ACTClass::ACTClass(IBPP::Database MyDB)
		:Eidos(MyDB)
{
	//������������� ����������� � �������� ��������� �������� �������������
        //������� ������
        Species="ACT";
}
ACTType* ACTClass::GetACTType(long ACTID)
{
//��������� ������� ������ ���� OBJType ������������� � �������� ������ �������� �����
//�� ��������� �������������� ������������ � ���������
	ACTType*TmpPtr= new ACTType(this,ACTID);
        return TmpPtr;
}

ACTType* ACTClass::AddACTType()
{
//��������� ������� ������ ���� OBJType ������������� � �������� ������ �������� �����
//�� ��������� �������������� ������������ � ���������
	ACTType*TmpPtr= new ACTType(this,"����� ������� ��������");
        return TmpPtr;
}

std::string ACTClass::GetClassSQL()
{
	return "select * from GET_EIDOS_LIST('ACT') where ID>1;";
}

