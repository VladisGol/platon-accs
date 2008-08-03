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

Ядро системы учета "Платон".
Авторами программы являются Борисенков Сергей Александрович и Головырин Владислав Владимирович, 2005-2007г.
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

#ifndef SQLMANAGERH
#define SQLMANAGERH

#include "eidos.h"
#include "hypotesis.h"
#include "extraatrib.h"


#include <string>
#include <vector>
#include "ibpp.h"


namespace platon
{
	class Eidos;
	class Hypotesis;
	class ExtraAttribute;
	struct HeaderEADescriber
	{
			std::string StoredProcedureName;
			std::string FieldName;
			std::string AliasName;
			std::string FieldForRetVal;
			std::string Caption;

			int FieldType;
			long EAID;

	};

	class SQLManager
	{
        protected:
                IBPP::Database DB;
                IBPP::Statement LocalST;
                IBPP::Transaction LocalTR;
                void Initialize();
                std::string SQL_string;
                bool IsFetched;
                bool IsStarted;
                long EidosID;
	public:
		void Clear();
		~SQLManager();
        SQLManager(Eidos* Ei);
        std::vector <HeaderEADescriber> AttributesList;
		void AddFieldVisibleValue(std::string FieldName);
		void AddFieldVisibleValue(ExtraAttribute* EA);
		void AddFieldKeyValue(std::string FieldName);
		void AddFieldKeyValue(ExtraAttribute* EA);
	};
        class HypotesisSQLManager: public SQLManager
        {
        public:
		const std::string SQLString();
                HypotesisSQLManager(Eidos* Ei);
        };
        class HypotesisPragmaSQLManager: public SQLManager
        {
        public:
		const std::string SQLString();
                HypotesisPragmaSQLManager(Eidos* Ei);
        };
        class PragmaSQLManager: public SQLManager
        {
        	long HypotesisID;
        public:
		const std::string SQLString();
                PragmaSQLManager(Hypotesis* Hi);
        };

}
#endif
