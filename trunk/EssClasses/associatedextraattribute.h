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

#ifndef AssociatedExtraAttributeH
#define AssociatedExtraAttributeH

#include "extraatrib.h"
#include "hypotesis.h"

#include <string>
#include <sstream>
#include "ibpp.h"

namespace platon
{

class ExtraAttribute;
class Hypotesis;
struct RoutineInOut;
struct LNK_Value;

//����� ��������������, ���������������� � �������� ���� HYPOTESIS � ����������� �������
//��������� ������ � ����������������
class AssociatedExtraAttribute
{
        Hypotesis* OwnerHypotesis;
public:
        ExtraAttribute* EA;
        AssociatedExtraAttribute(ExtraAttribute* InEA, Hypotesis* InHyp);

        std::string      GetVisibleValue()const;

        std::string      GetStringValue()const;
        int              GetIntValue()const;
        bool             GetBoolValue()const;
        float            GetFloatValue()const;
        IBPP::Timestamp        GetDateTimeValue()const;
        RoutineInOut     GetDLLValue()const;
        LNK_Value        GetLink2HValue()const;
        LNK_Value        GetLink2PValue()const;
        long             GetRefBookValue()const;

        void SetStringValue(std::string In);
        void SetIntValue(int In);
        void SetBoolValue(bool In);
        void SetFloatValue(float In);
        void SetDateTimeValue(IBPP::Timestamp In);
        void SetDLLValue(RoutineInOut In);
        void SetLink2HValue(LNK_Value In);
        void SetLink2PValue(LNK_Value In);
        void SetRefBookValue(long In);

        long SetMultiLNKValue(LNK_Value In, long ID_inlist);
};

}
#endif
