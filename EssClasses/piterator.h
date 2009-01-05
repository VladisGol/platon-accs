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
#ifndef PlatonIteratorH
#define PlatonIteratorH

#include "eidos.h"
#include "associatedextraattribute.h"
#include "hypotesis.h"
#include "pragma.h"

#include <string>
#include "ibpp.h"


namespace platon
{
	class pIterator
	{
    protected:
                void Initialize();
                bool IsFetched;
                bool IsStarted;
                long RowNum;
	public:
				~pIterator();
                IBPP::Database DB;
                IBPP::Statement LocalST;
                IBPP::Transaction LocalTR;
                std::string SQL_string;				//������ SQL �� ������� ������������ ������� ����� �������������� �� ������� ������ First() ��� Next()
                std::string SQL_string_forreccount; //������ SQL �� ������� ������������ ������ ����� �������

                long First();
                long Next();
                bool Fetched();
                long GetID();
                long GetRowNum();
                long GetRowCount();

	};

	class iterEidos :public pIterator
	{
	public:
		iterEidos(IBPP::Database inDB,std::string Species);
        std::string GetTitle();
		long GetParentID();
	};
	class iterHypotesis :public pIterator
	{
	public:
		iterHypotesis(Eidos* InEidos);
		iterHypotesis(IBPP::Database inDB,long ID_Eidos);
	};
	class iterPragma :public pIterator
	{
	public:
		iterPragma(Hypotesis* InHyp);
		iterPragma(IBPP::Database inDB,long ID_Eidos, long ID_Hypotesis);
		iterPragma(Eidos* InEidos);
        std::string GetTitle();
	};
	class iterHypPragma :public pIterator
	{
	protected:
		iterHypPragma(){;};
	public:
		iterHypPragma(Eidos* InEidos);
        std::string GetTitle();
	};

	class iterAllPragmaForEidos:public pIterator
	{
	public:
		iterAllPragmaForEidos(Eidos* InEidos);        //�������� ��� ��������� ������ ����������� ����� � ����� �������� �� ���������� ������
	};
	//������ ���������� ��� ������������ ������������ ��������
	class iterTemporalityListofOneEA:public pIterator
	{
	public:
		iterTemporalityListofOneEA(AssociatedExtraAttribute* OneAEA);
		IBPP::Timestamp GetGateTime();
	};

	//������ ����������, ��� ������������ ������
	class iterLNKS_Hyp :public pIterator
	{
	public:
		iterLNKS_Hyp(IBPP::Database inDB);
		void MasterChanged(long LEidosID,long ID_in);
		long EidosID;
		long ID_in;
	};

	class iterLNKS_HEidos :public pIterator
	{
	public:
		iterLNKS_HEidos(IBPP::Database inDB,long ID_in);
		iterLNKS_Hyp* DetailIter;
	};

	class iterLNKS_Pragma :public iterHypPragma
	{
	public:
		iterLNKS_Pragma(IBPP::Database inDB);
		void MasterChanged(long LEidosID,long ID_in);
		long EidosID;
		long ID_in;
	};

	class iterLNKS_PEidos :public pIterator
	{
	public:
		iterLNKS_PEidos(IBPP::Database inDB,long ID_in);
		iterLNKS_Pragma* DetailIter;
	};

}

#endif
