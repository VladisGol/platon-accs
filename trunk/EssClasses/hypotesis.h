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
#ifndef HYPOTESISH
#define HYPOTESISH

#include "eidos.h"
#include "associatedextraattribute.h"
#include "sqlmanager.h"

#include <vector>
#include <string>

#include "ibpp.h"

namespace platon
{
class Eidos;
class PragmaSQLManager;
class AssociatedExtraAttribute;

//����� ����� - ������ ����� ����������� ��������� ���� �������� � ����������� �� ���
//��������, ��������� ������������� �������� ��������� ��������� ��� �������������
class Hypotesis
{
protected:
	long ID;
	void Initialize();
    void CopyExtraatributesFromHostEidos();
    int BelongIdent;
    void CleanExtraAttributes();
public:
	Eidos* HostEidos;
    Hypotesis(){return;};
	Hypotesis(Eidos* MyObjClass,std::string NameType);
    Hypotesis(Eidos* MyObjClass, long ID_Type);
	~Hypotesis();
	long GetID()const;
    std::vector <AssociatedExtraAttribute*> Attributes;
	int NumEAByFieldName(std::string FieldName)const;
    int NumEAByID(long FieldID)const;
    AssociatedExtraAttribute* GetEAByFieldName(std::string FieldName);
    AssociatedExtraAttribute* GetEAByFieldID(long FieldID);
    AssociatedExtraAttribute* GetEAByNum(int NumEA);
    long Save();

    void SetHypotesName(std::string value);
    std::string GetHypotesName();

   	void CommitProcedure();
    bool Autocommited;
   	void FillEASQLProp();
    static void GetEidosHypotesisIDS(IBPP::Database MyDB, long ID_IN,long &ID_Eidos, long &ID_Hypotesis);
    IBPP::Transaction TransactionIBPP;
    PragmaSQLManager* PragmaSQL;
};
}

#endif
