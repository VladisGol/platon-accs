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

#include "hypotesis.h"

using namespace platon;

Hypotesis::Hypotesis(Eidos* MyEidos,std::string NameHypotesis)
{
//Конструктор класса типов для создания нового элемента в БД. В качестве параметров
//передается название типа, а также идентификатор класса к которому принадлежит элемент
        BelongIdent=ExtraAttribute::_theHypotesis;
        this->HostEidos=MyEidos;
        Autocommited=false;
        Initialize();
        this->ID=0;             //Новый атрибут
        this->ID=Save();        //Записываем и запоминаем идентификатор объекта
        CopyExtraatributesFromHostEidos();
        this->GetEAByFieldName("HypName")->SetStringValue(NameHypotesis);   //Заносим имя в обязательный экстраатрибут
        PragmaSQL= new PragmaSQLManager(this);
}

Hypotesis::Hypotesis(Eidos* MyEidos, long ID_Hypotesis)
{
//Конструктор класса типов для извлечения из базы уже имеющегося объекта. В качестве параметра
//используется идентификатор объекта типа, записанного в БД
	//Загружаем данные класса, записанные в БД
        BelongIdent=ExtraAttribute::_theHypotesis;
        this->HostEidos=MyEidos;
        Autocommited=false;
        Initialize();

        IBPP::Statement LocalST=IBPP::StatementFactory(this->HostEidos->DB, TransactionIBPP);
        if(!TransactionIBPP->Started()) TransactionIBPP->Start();

        long ID_returned;
        LocalST->Prepare("EXECUTE PROCEDURE GET_HYPOTESIS(?);");
        LocalST->Set(1,(int32_t)ID_Hypotesis);
        LocalST->Execute();
        LocalST->Get("ID",(int32_t*)&ID_returned);
        CommitProcedure();

        //Проверяем совпадает ли идентификатор запрошенного объекта с возвращенным
        if(ID_returned==ID_Hypotesis)
        {
        	//Получаем значения сохраненные в БД для указанного объекта
                this->ID=ID_returned;
                CopyExtraatributesFromHostEidos();
                PragmaSQL= new PragmaSQLManager(this);
        }
        else
        {
        	throw "Объект на который указывает идентификатор не существует";
        }
}

void Hypotesis::Initialize()
{
//Процедура инициализирует переменные класса OBJType независимо от того какой из перегруженных
//конструкторов используется

        Attributes.clear();
        TransactionIBPP=IBPP::TransactionFactory(this->HostEidos->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
}

Hypotesis::~Hypotesis()
{
//Деструктор класса очищаем все занятые переменными и объектами память
        CleanExtraAttributes();
        Attributes.clear();
        delete PragmaSQL;
}

long Hypotesis::GetID()const
{
//Чтение значение поля ID
	return ID;
}

void Hypotesis::CopyExtraatributesFromHostEidos()
{
//Процедура копирует ссылки на объекты экстраатрибутов в список объекта, отфильтровывая те из
//них, которые относятся к данному классу, идентификатор которого в переменной
		ExtraAttribute* tmpAttrib;
        Attributes.clear();
        for(unsigned int i=0;i<this->HostEidos->Attributes.size();i++)
        {
                tmpAttrib=(ExtraAttribute*)this->HostEidos->Attributes[i];
                if(tmpAttrib->belongTo==BelongIdent)
                {
                        AssociatedExtraAttribute* MyAttrib = new AssociatedExtraAttribute(tmpAttrib,this);
                        Attributes.push_back(MyAttrib);
                }
        }
}

int Hypotesis::NumEAByFieldName(std::string FieldName)const
{
//Функция находит и возвращает по имени атрибута его номер в случае если объект не найден, возвращается -1
        AssociatedExtraAttribute* tmpAttrib;
        int for_return=-1;
        for(unsigned int i=0;i < this->Attributes.size();i++)
        {
                tmpAttrib=(AssociatedExtraAttribute*)this->Attributes[i];
                if(ToUpper(tmpAttrib->EA->GetEAFieldName()) == ToUpper(FieldName))
                {
                	for_return=i;
                	break;
                }
        }
        return for_return;
}

int Hypotesis::NumEAByID(long FieldID)const
{
//Функция находит и возвращает по имени атрибута его номер в случае если объект не найден, возвращается -1
        AssociatedExtraAttribute* tmpAttrib;
        int for_return=-1;
        for(unsigned int i=0;i<this->Attributes.size();i++)
        {
                tmpAttrib=(AssociatedExtraAttribute*)this->Attributes[i];
                if(tmpAttrib->EA->GetEAID() == FieldID)
                {
                	for_return=i;
                	break;
                }
        }
        return for_return;
}



AssociatedExtraAttribute* Hypotesis::GetEAByFieldName(std::string FieldName)
{
//Процедура возвращает ссылку на экстраатрибут по имени поля экстраатрибута

        AssociatedExtraAttribute* tmpAttrib=NULL;
        int Number = NumEAByFieldName(FieldName);
        if(Number>-1) tmpAttrib=(AssociatedExtraAttribute*)this->Attributes[Number];
        return tmpAttrib;
}

AssociatedExtraAttribute* Hypotesis::GetEAByFieldID(long FieldID)
{
//Процедура возвращает ссылку на экстраатрибут по имени поля экстраатрибута

        AssociatedExtraAttribute* tmpAttrib=NULL;
        int Number = NumEAByID(FieldID);
        if(Number>-1) tmpAttrib=(AssociatedExtraAttribute*)this->Attributes[Number];
        return tmpAttrib;
}

AssociatedExtraAttribute* Hypotesis::GetEAByNum(int Number)
{
//Процедура возвращает ссылку на экстраатрибут по номеру экстраатрибута в списке
        AssociatedExtraAttribute* tmpAttrib=NULL;
        if(Number>-1) tmpAttrib=(AssociatedExtraAttribute*)this->Attributes[Number];
        return tmpAttrib;
}

long Hypotesis::Save()
{
//Процедура сохраняет значение атрибутов(не экстраатрибутов) в таблице типов объектов
        IBPP::Statement LocalST=IBPP::StatementFactory(this->HostEidos->DB, TransactionIBPP);
        if(!TransactionIBPP->Started()) TransactionIBPP->Start();

        long ID_returned;
        LocalST->Prepare("EXECUTE PROCEDURE SET_HYPOTESIS(?,?);");
        LocalST->Set(1,(int32_t)this->ID);
        LocalST->Set(2,(int32_t)HostEidos->GetID());
        LocalST->Execute();
        LocalST->Get("ID_OUT",(int32_t*)&ID_returned);
        CommitProcedure();
        return ID_returned;
}


void Hypotesis::SetHypotesName(std::string value)
{
        this->GetEAByFieldName("HypName")->SetStringValue(value);   //Заносим имя в обязательный экстраатрибут
        Save();
}
std::string Hypotesis::GetHypotesName()
{
        return this->GetEAByFieldName("HypName")->GetStringValue();
}

void Hypotesis::CommitProcedure()
{
//Процедура завершает транзакцию в случае если установлен флажок Autocommited=true
        if(Autocommited==true)
        {
                TransactionIBPP->Commit();
        }
}


void Hypotesis::GetEidosHypotesisIDS(IBPP::Database MyDB, long ID_IN,long &ID_Eidos, long &ID_Hypotesis)
{
//Статическая процедура которая извлекает информацию по объекту типа Hypotesis из базы данных
//без создания самого объекта. Возвращается ID Eidos и для совместимости с потомком копируется
//значение типа гипотеза в ID_Hypotesis

        IBPP::Transaction LocalTr=IBPP::TransactionFactory(MyDB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement LocalST=IBPP::StatementFactory(MyDB, LocalTr);
        LocalTr->Start();

        long ID_retHyp;
        long ID_retEidos;
        LocalST->Prepare("EXECUTE PROCEDURE GET_HYPOTESIS(?);");
        LocalST->Set(1,(int32_t)ID_IN);
        LocalST->Execute();
        LocalST->Get("ID",(int32_t*)&ID_retHyp);
        LocalST->Get("ID_EIDOS",(int32_t*)&ID_retEidos);
        LocalTr->Commit();

        if(ID_retHyp==ID_IN)
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

void Hypotesis::CleanExtraAttributes()
{
//Процедура очищает все имеющиеся в распоряжении объекта атрибуты из памяти
	for(unsigned int i =0;i<Attributes.size();i++) delete ((AssociatedExtraAttribute*)Attributes[i]);
        Attributes.clear();
}

void Hypotesis::FillEASQLProp()
{
//Процедура заполняет списки экстраатрибутов SQL строк для класса
        PragmaSQL->Clear();
        ExtraAttribute* OneAttrib;	//Указатель на элемент объекта класса дополнительного атрибута
        for(int i=this->HostEidos->Attributes.size()-1;i>=0;i--)	//Последовательно для всех дополнительных атрибутов
        {
                OneAttrib=(ExtraAttribute*)this->HostEidos->Attributes[i];
                if(OneAttrib->belongTo==ExtraAttribute::_thePragma && OneAttrib->Visible)
                {
                        PragmaSQL->AddFieldVisibleValue(OneAttrib);
                }
        }
}

