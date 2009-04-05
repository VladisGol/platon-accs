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

#include "rescopy.h"
#include "restype.h"



using namespace platon;

RESCopy::RESCopy(Hypotesis* MyType,IBPP::Timestamp DateCreation)
	:Pragma(MyType,DateCreation)
{
}
RESCopy::RESCopy(Hypotesis* MyType, long ID_Copy)
	:Pragma(MyType, ID_Copy)
{
}


RESCopy* RESCopy::Fork(long ID_ACTCopy_BelongFor)
{
//Процедура создает копию текущего объекта экземпляра ресурса
//копируя в порождаемый объект все экстраатрибуты, после чего
//в ЭА LNK_ACTBelongFor помещается значение переданное в параметре
// - идентификатор экземпляра действия на которое списывается порождаемый ресурс
// в ЭА LNK_ForkedRES помещается идентификатор экземпляра ресурса от которого
//произошло отщепление.
//Значение ЭА Quantity присваивается значению остатка ЭА Remainder порождающего
//экземпляра ресурса.
//Так как создается самостоятельный экземпляр, дата создания прагмы меняется на текущее значение
//Функция возвращает ссылку на порожденный экземпляр ресурса

        RESCopy* FosteredRESCopy=((RESType*)this->HostHypotesis)->AddRESCopy();


        RESCopy*Source=this;
        RESCopy*Destination=FosteredRESCopy;

        for(unsigned int i=0;i<Source->Attributes.size();i++)
        {
				std::string curFieldName =((AssociatedExtraAttribute*)Source->Attributes[i])->EA->GetEAFieldName();
				if(curFieldName=="Remainder") continue;
				if(curFieldName=="PragmaDate") continue;
				if(curFieldName=="LNK_ACTBelongFor") continue;
				if(curFieldName=="LNK_ForkedRES") continue;
				if(curFieldName=="Quantity") continue;

       	        //По очереди скопируем все остальные значения экстраатрибутов
              	switch (((AssociatedExtraAttribute*)Source->Attributes[i])->EA->type)
                {
                        case ft_String:
                                Destination->GetEAByNum(i)->SetStringValue(Source->GetEAByNum(i)->GetStringValue());
                                break;
                        case ft_Integer:
                                Destination->GetEAByNum(i)->SetIntValue(Source->GetEAByNum(i)->GetIntValue());
                                break;
                        case ft_Float:
                                Destination->GetEAByNum(i)->SetFloatValue(Source->GetEAByNum(i)->GetFloatValue());
                                break;
                        case ft_Boolean:
                                Destination->GetEAByNum(i)->SetBoolValue(Source->GetEAByNum(i)->GetBoolValue());
                                break;
                        case ft_DateTime:
                                Destination->GetEAByNum(i)->SetDateTimeValue(Source->GetEAByNum(i)->GetDateTimeValue());
                                break;
                        case ft_RB:
                                Destination->GetEAByNum(i)->SetRefBookValue(Source->GetEAByNum(i)->GetRefBookValue());
                                break;
                        case ft_DLL:
                                Destination->GetEAByNum(i)->SetDLLValue(Source->GetEAByNum(i)->GetDLLValue());
                                break;
                        case ft_LinkHypotesis:
                        {
								LNK_Value OneLNK=Source->GetEAByNum(i)->GetLink2HValue();
								if(OneLNK.LinkTo>0) Destination->GetEAByNum(i)->SetLink2HValue(OneLNK);
                                break;
                        }
                        case ft_LinkPragma:
                        {
								LNK_Value OneLNK=Source->GetEAByNum(i)->GetLink2PValue();
								if(OneLNK.LinkTo>0) Destination->GetEAByNum(i)->SetLink2PValue(OneLNK);
                                break;
                        }
                        case ft_Security:
                                break;
                        default:
                                throw("Указанный тип данных не поддерживается");
                }
        }

        FosteredRESCopy->GetEAByFieldName("PragmaDate")->SetDateTimeValue(this->HostEidos->DT);

        LNK_Value LNK;

        LNK.LinkTo=ID_ACTCopy_BelongFor;
        LNK.Ratio=1;
        FosteredRESCopy->GetEAByFieldName("LNK_ACTBelongFor")->SetLink2PValue(LNK);

        LNK.LinkTo=this->GetID();
        LNK.Ratio=1;
        FosteredRESCopy->GetEAByFieldName("LNK_ForkedRES")->SetLink2PValue(LNK);

        FosteredRESCopy->GetEAByFieldName("Quantity")->SetFloatValue(this->GetEAByFieldName("Remainder")->GetFloatValue());
        return FosteredRESCopy;
}
