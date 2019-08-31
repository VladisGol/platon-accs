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

#include "actclass.h"


using namespace platon;


ACTClass::ACTClass(IBPP::Database MyDB, int ID_IN)
		:Eidos(MyDB, ID_IN)
{
	//Перегруженный конструктор в качестве параметра получает идентификатор
        //объекта класса
	;
}

ACTClass::ACTClass(IBPP::Database MyDB)
		:Eidos(MyDB)
{
	//Перегруженный конструктор в качестве параметра получает идентификатор
        //объекта класса
        Species="ACT";
}
ACTType* ACTClass::GetACTType(int ACTID)
{
//Процедура создает объект типа OBJType принадлежащий к текущему классу объектов учета
//на основании идентификатора паереданного в параметре
	ACTType*TmpPtr= new ACTType(this,ACTID);
        return TmpPtr;
}

ACTType* ACTClass::AddACTType()
{
//Процедура создает объект типа OBJType принадлежащий к текущему классу объектов учета
//на основании идентификатора паереданного в параметре
	ACTType*TmpPtr= new ACTType(this,"Новое учетное действие");
        return TmpPtr;
}

std::string ACTClass::GetClassSQL()
{
	return "select * from GET_EIDOS_LIST('ACT') where ID>1;";
}

