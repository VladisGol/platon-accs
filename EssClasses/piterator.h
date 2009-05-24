/*
software core of accounting system "Platon".
Copyright (C) 2005-2009 Borisenkov S., Golovyrin V.

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

Ядро системы учета "Платон".
Авторами программы являются Борисенков Сергей Александрович и Головырин Владислав Владимирович, 2005-2009г.
Данная библиотека является свободным программным обеспечением. Вы вправе распространять ее и/или модифицировать
в соответствии с условиями версии 2.1 Стандартной Общественной Лицензии Ограниченного Применения GNU,
опубликованной Free Software Foundation.
Мы распространяем эту программу в надежде на то, что она будет вам полезной,
однако НЕ ПРЕДОСТАВЛЯЕМ НА НЕЕ НИКАКИХ ГАРАНТИЙ, в том числе ГАРАНТИИ ТОВАРНОГО СОСТОЯНИЯ ПРИ ПРОДАЖЕ и
ПРИГОДНОСТИ ДЛЯ ИСПОЛЬЗОВАНИЯ В КОНКРЕТНЫХ ЦЕЛЯХ.
Для получения более подробной информации ознакомьтесь со Стандартной Общественной Лицензией Ограниченного
Применений GNU.
Связаться с авторами программы вы можете по e-mail vladisgol@rambler.ru
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
                std::string SQL_string;				//Строка SQL по которой производится выборка можно переопределить до первого вызова First() или Next()
                std::string SQL_string_forreccount; //Строка SQL по которой производится расчет числа записей

                long First();
                long Next();
                bool Fetched();
                long GetID();
                long GetRowNum();
                long GetRowCount();

	};

	class iterEidos :public pIterator			//Итератор получения всех Eidos-ов по Species
	{
	public:
		iterEidos(IBPP::Database inDB,std::string Species);
        std::string GetTitle();
		long GetParentID();
	};
	class iterHypotesis :public pIterator		//Итератор получения всех Hypotesis по Eidos-у
	{
	public:
		iterHypotesis(Eidos* InEidos);
		iterHypotesis(IBPP::Database inDB,long ID_Eidos);
		std::string GetTitle();
	};
	class iterPragma :public pIterator		//Итератор получения Pragma
	{
	public:
		iterPragma(Hypotesis* InHyp);			//  по Hypotesis
		iterPragma(IBPP::Database inDB,long ID_Eidos, long ID_Hypotesis);
		iterPragma(Eidos* InEidos);			//  по Eidos-у
        std::string GetTitle();
	};

	class iterHypPragma :public pIterator		//Итератор получения связки Hypotesis и Pragma по Eidos-у используется в iterLNKS_Pragma
	{
	protected:
		iterHypPragma(){;};
	public:
		iterHypPragma(Eidos* InEidos);
        std::string GetTitle();
	};

	//Классы итераторов для обслуживания темпоральных значений
	class iterTemporalityListofOneEA:public pIterator
	{
	public:
		iterTemporalityListofOneEA(AssociatedExtraAttribute* OneAEA);
		IBPP::Timestamp GetGateTime();
	};

	//Классы итераторов, для обслуживания ссылок

	class iterLNKS_Hyp :public pIterator				//Итератор получения ссылающихся объектов с типом Hypotesis
	{
	public:
		iterLNKS_Hyp(IBPP::Database inDB);
		void MasterChanged(long LEidosID,long ID_in);
		long EidosID;
		long ID_in;
	};

	class iterLNKS_HEidos :public pIterator			//Итератор получения Eidos-ов ссылающихся объектов с типом Hypotesis	{
	{
	public:
		iterLNKS_HEidos(IBPP::Database inDB,long ID_in);
		iterLNKS_Hyp* DetailIter;
	};

	class iterLNKS_Pragma :public iterHypPragma		//Итератор получения ссылающихся объектов с типом Pragma
	{
	public:
		iterLNKS_Pragma(IBPP::Database inDB);
		void MasterChanged(long LEidosID,long ID_in);
		long EidosID;
		long ID_in;
	};

	class iterLNKS_PEidos :public pIterator			//Итератор получения Eidos-ов ссылающихся объектов с типом Pragma
	{
	public:
		iterLNKS_PEidos(IBPP::Database inDB,long ID_in);
		iterLNKS_Pragma* DetailIter;
	};

	class iterMultilink:public pIterator				//Итератор получения списка ссылок многострочного ссылочного атрибута
	{
	public:
		iterMultilink(AssociatedExtraAttribute* OneAEA);
		std::string GetTitle();
	};
}

#endif
