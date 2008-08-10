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

#ifndef ACTCopyH
#define ACTCopyH

#include "hypotesis.h"
#include "pragma.h"
#include "rescopy.h"
#include <string>

namespace platon
{

//����� ����� - ������ ����� ����������� ��������� ���� �������� � ����������� �� ���
//��������, ��������� ������������� �������� ��������� ��������� ��� �������������
class ACTCopy : public Pragma
{
private:
public:
	ACTCopy(Hypotesis* MyType,IBPP::Timestamp DateCreation);
        ACTCopy(Hypotesis* MyType,IBPP::Timestamp DateCreation, long OBJID);
        ACTCopy(Hypotesis* MyType, long ID_Copy);
        RESCopy* AddSpentRESCopy(RESCopy* OneItem);
};

}
#endif

