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
#ifndef EIDOSH
#define EIDOSH

#include "extraatrib.h"
#include "sqlmanager.h"
#include <vector>
#include <string>

#include "ibpp.h"

namespace platon
{

class HypotesisSQLManager;
class HypotesisPragmaSQLManager;
class ExtraAttribute;



//����� ������� �������� - �������� ����� ��� ��������������� ����������� �������� �����
//�� ������ � ������������ ������������� ������ � ������������ �������������� ���������,
//��������� ����������� � ����� ������� ������� ����� �������
class Eidos
{
private:
	long id;         		//������������� �������
    long id_parent;                 //������ �� ���������������

    std::string Name;
    std::string FullName;

	void Initialize();
	void GetData(Eidos* InClass,long ID_IN);
	void QueryForExtraAttrib(Eidos* InObj);
	void CleanExtraAttributes();

    IBPP::Transaction LocalTR;

	void GetExtraAttributesSet();

protected:
    std::string Species;

public:
    IBPP::Database DB;            //���� ������ IBPP
    IBPP::Timestamp DT;           //���� �����

	Eidos(IBPP::Database MyDB);
	Eidos(IBPP::Database MyDB, long ID_IN);
	~Eidos();

	std::vector <ExtraAttribute*> Attributes;

	void Save();

	long AddExtraAttribute(std::string Name, int FieldType);
	int NumEAByFieldName(std::string FieldName) const;

	long GetParentID() const;
	void SetParentID(long value);
	void SetName(std::string value);
	std::string GetName()const;
	long GetID()const;

    ExtraAttribute* GetEAByFieldName(std::string FieldName);
    ExtraAttribute* GetEAByNum(int NumEA);
    std::string GetEidosSpecies() const;
    std::string GetFullEidosName()const;
    HypotesisSQLManager* HypotesisSQL;
    HypotesisPragmaSQLManager* PragmaSQL;
    void FillEASQLProp();
};

void SetTimestampTemporalCompareFor(IBPP::Database MyDB, IBPP::Timestamp DTForSet);
long Get_EAID_ByName(IBPP::Database MyDB,std::string NameOfEA);
long Get_TopIDBySpecies(IBPP::Database MyDB,std::string NameOfBelong);
void BranchDisassemble(const std::string NameOfBranch, std::vector<std::string> &Elements);
long GetEidosIDByBranchFullName(IBPP::Database MyDB,const std::string NameOfBranch);
}

#endif
