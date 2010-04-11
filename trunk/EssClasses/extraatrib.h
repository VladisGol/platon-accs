/*
software core of accounting system "Platon".
Copyright (C) 2005-2010 Borisenkov S., Golovyrin V.

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
Project site: http://code.google.com/p/platon-accs/

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
Cайт проекта http://code.google.com/p/platon-accs/
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
        long LinkTo;    //ID объекта, на который указывает ссылка
        float Ratio;    //Кратность
};


enum FieldType {ft_String=1, ft_Integer=3, ft_Boolean=5, ft_Float=6, ft_DateTime=11,ft_FmtMemo=17,
                ft_DLL=102, ft_LinkHypotesis=103, ft_LinkPragma=104, ft_Security=105};


//Класс дополнительных атрибутов реализует функционал дополнительных свойств объектов класса Eidos
//Для доступа к внутренним элементам класса класс Eidos является дружественным и содержит в себе
//список указателей на объекты дополнительных атрибутов.

class ExtraAttribute
{
	friend class Eidos;
        friend class AssociatedExtraAttribute;
private:
	long id;
	std::string Caption;
	long    id_Class; 		//Идентификатор объекта класса к которому относится атрибут (если = текущему, значит объект хозяин)
	std::string strClass_NodeName;	//Имя класса к которому относится атрибут (для отображения наследования)
	std::string FieldName;
	std::string sNameStoredProc;
	std::string sTemporalListSPName;

public:
    Eidos* HostEidos;
	//Нумератор для указания к какому виду класса относится атрибут
	enum _BelongFor {_theHypotesis=1, _thePragma};

    int		type;			//Тип, используются константы описанные в TFieldType + 101(RB) и 102(DLL)
    int     belongTo;		//Принадлежность экстраатрибута к классу Hypotesis или Pragma
	long	ID_RB_Describer;	//(для типа 101)Указатель на запись в таблице описателей справочников
    std::string	DLL_FileName;   //(для типа 102)Имя файла динамической библиотеки
    std::string	DLL_ProcName;	//(для типа 102)Имя процедуры динамической библиотеки
    bool	Temporality;		//(только для экстраатрибутов, принадлежащих к Embodiment) Темпоральный (или хронологический) экстраатрибут
    IBPP::Timestamp DTValue;        //Значение даты времени для темпорального атрибута
    bool    Required;               //Обязательный параметр к заполнению
    bool    Visible;                //Отображение атрибута в гриде и в виде фрейма в форме
    bool    Locked;                 //Записание экстраатрибута от возможности изменения
    bool    Multilnk;               //Используется ли в ссылочном атрибуте множественная связь
    std::string	LNK_species;
    long 	LNK_EidosID;
    long 	LNK_HypID;
    bool	LNK_NeedList;

    bool 	IsCaptionAlternated;	//Переменная показывающая была ли проведена замена заголовка
    long	AltCaptionEidosID;		//Идентификатор ейдоса на уровне которого проведена замена заголовка

	long 	Save();
	ExtraAttribute();
	~ExtraAttribute();

	std::string GetEACaption()const;
	void  SetEACaption(std::string value);

    std::string GetClass_NodeName()const;
    long GetEAID()const;

    void SetEAFieldName(std::string value);
    std::string GetEAFieldName()const;		//Применимость атрибута к объектам программы

    std::string NameStoredProc()const;
	std::string TemporalListSPName()const;

    long RealRecordsCount(void);
    void SetAlterCaption(std::string NewCaption);
    void DeleteAlterCaption();
};
}
#endif
