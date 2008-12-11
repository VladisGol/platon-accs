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

#ifndef ExtraatribH
#define ExtraatribH

#include "eidos.h"
#include "associatedextraattribute.h"

#include <string>
#include <locale>
#include <sstream>
#include "ibpp.h"


namespace platon
{

class Eidos;
//Thank Eloff http://www.codeproject.com/string/string_conversions.asp#table
template<class T>
std::string ToString(const T& val)
{
    std::stringstream strm;
    strm << val;
    return strm.str();
}
std::string ToUpper(const std::string val);
std::string Rtrim (const std::string val);

struct RoutineInOut
{
	std::string 	Key;
	std::string	VisibleValue;
};

struct LNK_Value
{
        long LinkTo;    //ID �������, �� ������� ��������� ������
        float Ratio;    //���������
};


enum FieldType {ft_String=1, ft_Integer=3, ft_Boolean=5, ft_Float=6, ft_DateTime=11,
                ft_RB=101, ft_DLL=102, ft_LinkHypotesis=103, ft_LinkPragma=104, ft_Security=105};


//����� �������������� ��������� ��������� ���������� �������������� ������� �������� ������ Eidos
//��� ������� � ���������� ��������� ������ ����� Eidos �������� ������������� � �������� � ����
//������ ���������� �� ������� �������������� ���������.

class ExtraAttribute
{
	friend class Eidos;
        friend class AssociatedExtraAttribute;
private:
	long id;
        std::string Caption;
        long    id_Class; 		//������������� ������� ������ � �������� ��������� ������� (���� = ��������, ������ ������ ������)
        std::string strClass_NodeName;	//��� ������ � �������� ��������� ������� (��� ����������� ������������)
        std::string FieldName;
        std::string sNameStoredProc;
        std::string sTemporalListSPName;

public:
    Eidos* HostEidos;
	//��������� ��� �������� � ������ ���� ������ ��������� �������
	enum _BelongFor {_theHypotesis=1, _thePragma};

    int	type;			//���, ������������ ��������� ��������� � TFieldType + 101(RB) � 102(DLL)
    int     belongTo;		//�������������� �������������� � ������ Hypotesis ��� Pragma
	long	ID_RB_Describer;	//(��� ���� 101)��������� �� ������ � ������� ���������� ������������
    std::string	DLL_FileName;   //(��� ���� 102)��� ����� ������������ ����������
    std::string	DLL_ProcName;	//(��� ���� 102)��� ��������� ������������ ����������
    bool	Temporality;		//(������ ��� ���������������, ������������� � Embodiment) ������������ (��� ���������������) �������������
    IBPP::Timestamp DTValue;        //�������� ���� ������� ��� ������������� ��������
    bool    Required;               //������������ �������� � ����������
    bool    Visible;                //����������� �������� � ����� � � ���� ������ � �����
    bool    Locked;                 //��������� �������������� �� ����������� ���������
    bool    Multilnk;               //������������ �� � ��������� �������� ������������� �����
    std::string	LNK_species;
    long 	LNK_EidosID;
    long 	LNK_HypID;
    bool	LNK_NeedList;

	long 	Save();
	ExtraAttribute();
	~ExtraAttribute();

	std::string GetEACaption()const;
	void  SetEACaption(std::string value);

    std::string GetClass_NodeName()const;
    long GetEAID()const;

    void SetEAFieldName(std::string value);
    std::string GetEAFieldName()const;		//������������ �������� � �������� ���������

	//std::string EATBLNameInSQL();

    std::string NameStoredProc()const;
	std::string TemporalListSPName()const;

    long RealRecordsCount(void);
};
}
#endif
