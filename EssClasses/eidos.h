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
License aint with this library; if not, write to the Free Software
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



//Класс классов объектов - основной класс для манипулирования метаданными объектов учета
//их связки в иерархически упорядоченные списки и инкапсуляции дополнительных атрибутов,
//аддитивно добавляемых с более верхних уровней учета объекта
class Eidos
{
private:
	int id;         						//Идентификатор объекта
    int id_parent;                 //Ссылка на предшественника

    std::string Name;
    std::string FullName;

	void Initialize();
	void GetData(Eidos* InClass,int ID_IN);
	void QueryForExtraAttrib(Eidos* InObj);
	void CleanExtraAttributes();

    IBPP::Transaction LocalTR;

	void GetExtraAttributesSet();
	void AlternateEACaption();	//Процедура изменения подписи экстраатрибутов

protected:
    std::string Species;

public:
    IBPP::Database DB;            //База данных IBPP
    IBPP::Timestamp DT;           //Дата время

	Eidos(IBPP::Database MyDB);
	Eidos(IBPP::Database MyDB, int ID_IN);
	~Eidos();

	std::vector <ExtraAttribute*> Attributes;

	void Save();

	int AddExtraAttribute(std::string Name, int FieldType);
	int NumEAByFieldName(std::string FieldName) const;

	int GetParentID() const;
	void SetParentID(int value);
	void SetName(std::string value);
	std::string GetName()const;
	int GetID()const;

    ExtraAttribute* GetEAByFieldName(std::string FieldName);
    ExtraAttribute* GetEAByNum(int NumEA);
    ExtraAttribute* GetEAByID(int EAID);
    std::string GetEidosSpecies() const;
    std::string GetFullEidosName()const;
    HypotesisSQLManager* HypotesisSQL;
    HypotesisPragmaSQLManager* PragmaSQL;
    void FillEASQLProp();
};

void SetTimestampTemporalCompareFor(IBPP::Database MyDB, IBPP::Timestamp DTForSet);
int Get_EAID_ByName(IBPP::Database MyDB,std::string NameOfEA);
int Get_TopIDBySpecies(IBPP::Database MyDB,std::string NameOfBeint);
void BranchDisassemble(const std::string NameOfBranch, std::vector<std::string> &Elements);
int GetEidosIDByBranchFullName(IBPP::Database MyDB,const std::string NameOfBranch);
std::string GetEidosSpecies(IBPP::Database MyDB, int id_eidos);
std::string GetDLLMD5(IBPP::Database MyDB, std::string NameOfDll);
void SetDLLMD5(IBPP::Database MyDB, const std::string NameOfDll ,const std::string MD5string);
void DeleteEidosItem(IBPP::Database MyDB, int ID_EIDOS);
void DeleteHypotesisItem(IBPP::Database MyDB, int ID_HYPOTESYS);
void DeletePragmaItem(IBPP::Database MyDB, int ID_PRAGMA);
int NumberChildofEidos(IBPP::Database MyDB, int ID_EIDOS);

}

#endif
