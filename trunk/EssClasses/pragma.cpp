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

#include "pragma.h"


using namespace platon;

Pragma::Pragma(Hypotesis* MyHypotesis,IBPP::Timestamp DateCreation)
{
//Конструктор класса типов для создания нового элемента в БД. В качестве параметров
//передается название типа, а также идентификатор класса к которому принадлежит элемент
        BelongIdent=ExtraAttribute::_thePragma;
        Autocommited=true;
        this->HostEidos=MyHypotesis->HostEidos;
        this->HostHypotesis=MyHypotesis;
        Initialize();
        this->ID=0;             //Новый атрибут
        this->ID=Save();        //Записываем имя и запоминаем идентификатор объекта
        CopyExtraatributesFromHostEidos();
        this->GetEAByFieldName("PragmaDate")->SetDateTimeValue(DateCreation); //Заносим дату создания в обязательный экстраатрибут
        PragmaSQL= new PragmaSQLManager(this);
}
Pragma::Pragma(Hypotesis* MyHypotesis, long ID_Pragma)
{
//Конструктор класса типов для извлечения из базы уже имеющегося объекта. В качестве параметра
//используется идентификатор объекта типа, записанного в БД
        BelongIdent=ExtraAttribute::_thePragma;
        Autocommited=true;

        this->HostEidos=MyHypotesis->HostEidos;
      	this->HostHypotesis=MyHypotesis;

      	Initialize();

        IBPP::Statement LocalST=IBPP::StatementFactory(this->HostEidos->DB, TransactionIBPP);
        if(!TransactionIBPP->Started()) TransactionIBPP->Start();

        long ID_returned;
        LocalST->Prepare("EXECUTE PROCEDURE GET_PRAGMA(?);");
        LocalST->Set(1,(int32_t)ID_Pragma);
        LocalST->Execute();
        LocalST->Get("ID",(int32_t*)&ID_returned);
        CommitProcedure();

        //Проверяем совпадает ли идентификатор запрошенного объекта с возвращенным
        if(ID_returned==ID_Pragma)
        {
        	//Получаем значения сохраненные в БД для указанного объекта
                this->ID=ID_returned;
                CopyExtraatributesFromHostEidos();
        }
        else
        {
        	throw "Объект на который указывает идентификатор не существует";
        }
        PragmaSQL= new PragmaSQLManager(this);
}

long Pragma::Save()
{
//Процедура сохраняет значение атрибутов(не экстраатрибутов) в таблице экземпляров объектов учета
        IBPP::Statement LocalST=IBPP::StatementFactory(this->HostEidos->DB, TransactionIBPP);
        if(!TransactionIBPP->Started()) TransactionIBPP->Start();

        long ID_returned;
        LocalST->Prepare("EXECUTE PROCEDURE SET_PRAGMA(?,?,?);");
        LocalST->Set(1,(int32_t)this->ID);
        LocalST->Set(2,(int32_t)HostEidos->GetID());
        LocalST->Set(3,(int32_t)HostHypotesis->GetID());
        LocalST->Execute();
        LocalST->Get("ID_OUT",(int32_t*)&ID_returned);
        CommitProcedure();
        return ID_returned;
}

void Pragma::GetEidosHypotesisIDS(IBPP::Database MyDB, long ID_IN,long &ID_Eidos, long &ID_Hypotesis)
{
//Статическая процедура которая извлекает информацию по объекту типа Hypotesis из базы данных
//без создания самого объекта. Возвращается ID Eidos и для совместимости с потомком копируется
//значение типа гипотеза в ID_Hypotesis

        IBPP::Transaction LocalTr=IBPP::TransactionFactory(MyDB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement LocalST=IBPP::StatementFactory(MyDB, LocalTr);
        LocalTr->Start();

        long ID_retHyp;
        long ID_retEidos;
        long ID;
        LocalST->Prepare("EXECUTE PROCEDURE GET_PRAGMA(?);");
        LocalST->Set(1,(int32_t)ID_IN);
        LocalST->Execute();
        LocalST->Get("ID_HYPOTESIS",(int32_t*)&ID_retHyp);
        LocalST->Get("ID_EIDOS",(int32_t*)&ID_retEidos);
        LocalST->Get("ID",(int32_t*)&ID);
        LocalTr->Commit();

        if(ID==ID_IN)
        {
        	//Получаем значения сохраненные в БД для указанного объекта
                ID_Eidos=ID_retEidos;
                ID_Hypotesis=ID_retHyp;
        }
        else
        {
                //Возвращаем все нули
                ID_Eidos=0;
                ID_Hypotesis=0;
        }
}

std::string Pragma::GetLinkedPragmaSQL(long IDLinkEA)const
{
//Процедура возвращает строку SQL для вывода списка прилинкованных Pragma по экстраатрибуту, идентификатор которого
//передан в параметре (не номер, а идентификатор в БД)
//
        return "select * from GET_LINKED_PRAGMA_LIST("+ToString(IDLinkEA)+","+ToString(this->GetID())+");";
}

std::string Pragma::GetLinkedPragmaSQL(std::string LinkEAName)const
{
//Процедура возвращает строку SQL для вывода списка прилинкованных Pragma по экстраатрибуту, имя которого
//передано в параметре

        return GetLinkedPragmaSQL(Get_EAID_ByName(this->HostEidos->DB,LinkEAName));
}

std::string Pragma::GetPragmaName()
{
//Функция возвращает имя прагмы в формате - имя гипотезы + дата появления
        IBPP::Statement LocalST=IBPP::StatementFactory(this->HostEidos->DB, TransactionIBPP);
        if(!TransactionIBPP->Started()) TransactionIBPP->Start();

        std::string val_returned;
        LocalST->Prepare("select get_pragma_name_list.meaning TITLE from get_pragma_name_list where get_pragma_name_list.id=?;");
        LocalST->Set(1,(int32_t)this->ID);
        LocalST->Execute();
        if(LocalST->Fetch()) LocalST->Get("TITLE",val_returned);
        CommitProcedure();
        return val_returned;
}
