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
#ifndef dbetcH
#define dbetcH
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include "ibpp.h"
/*to do
 Сделать поддержку beginupdate и endupdate для экземпляра DbIni,
 по beginupdate - начинаем запись без commit`a
 по endupdate - производим commit и переоткрываем секцию, в
 ином случае после изменения параметра - переоткрываем секцию
*/
namespace platon
{
//Поддерживаемые типы
typedef enum {
        ptNone,         //неизвестный тип
        ptBlob,         //любой большой бинарный объект, нужны подтипы (например, doc, pdf и т.п.)
        ptBool,         //логический
        ptCurrency,     //Денежный тип от -922337203685477.5808 до 922337203685477.5807  храним в Ints
        ptDateTime,     //дата-время
        ptFloat,        //вещественное
        ptInt,          //целое
        ptInt64,        //большое целое
        ptShortString,  //короткая строка
        ptString,       //строка храним в Блобе
} EtcParamType;


typedef std::map<std::string,int,std::less<std::string> > cache_keys;
typedef cache_keys::value_type value_type;

//Сам класс: интерфейс к БД с настройками программы
class DbEtc
{
        int CurrentUserId, CurrentKeyId, KeyUserId, bmCurrentKeyId, bmKeyUserId;
        bool ReadOnly,isOpenKey, bmReadOnly, bmIsOpenKey, isUpdateMode;
        std::string CurrentUser, OpenedKey, bmOpenedKey;
        EtcParamType  NameToType(const std::string Name);
        IBPP::Database DB;            //База данных IBPP
        IBPP::Transaction LocalTR;
        IBPP::Statement stKey,stParameters;
        std::vector<IBPP::Row> Parameters;   //Параметры конкретного ключа
        void GetCurrentUser();                                                   //Ok!
        void  GetParameters(const int CurrentKeyId);                                   //Ok!
        int  SaveKey(const int id, const std::string Key, const int IdUser);    //Ok!
        int  GetKey(const std::string Key, const int IdUser);                    //Ok!
        void  SaveBookmark();               //Сохраним параметры открытого ключа   //Ok!
        bool  RestoreBookmark();            //Восстановим параметры открытого ключа   //Ok!
        int  CheckParam(const std::string ValueName);  //Проверим существование параметра для открытого объекта //Ok!
        void PrepareWriteParamValue(const std::string SQL, const std::string Name);    //Ok!
        void ExecuteWriteParamValue();
        void Init(void);  //Ok!
        cache_keys AllUserKeys;
        int  KeyCacheExists(const std::string Key);
        void ReadCache(void);
public:
        //Исключения
        class DbEtcError{};
        class ErrorWriteKey:public DbEtcError{};
        class KeyReadOnly:public DbEtcError{};    //Объект открыт только для чтения
        class KeyNotOpen:public DbEtcError{};     //Объект не открыт
        class BadValue:public DbEtcError{};       //Неправильное значение параметра
        class MismatchTypeValue:public DbEtcError{}; //Несоответсвие типа параметра и значения
        class ErrWriteValue:public DbEtcError{};     //Ошибка сохранения параметра
        class UnkownParameter:public DbEtcError{};     //Неизвестный параметр
        class NonRegisterUser:public DbEtcError{};
        class NotReadMode:public DbEtcError{}; //Используется режим массовой записи, чтение невозможно!
        class BigNameKey: public DbEtcError{};
        class BigNameValue: public DbEtcError{};
        IBPP::Timestamp DT;           //Дата время

        //Конструктор
        DbEtc(IBPP::Database myDB);                                             //Ok!
        DbEtc(std::string HostName, std::string DatabaseName,
                std::string UserName, std::string Password, std::string Role,
                std::string CharSet, std::string CreateParams);                  //Ok!
        //Деструктор
         ~DbEtc(void);                                                          //Ok!
        //Закроем объект
        void  CloseKey(void);                                                     //Ok!
        //Включим режим массированной записи в БД
        inline void BeginUpdate(void) {isUpdateMode=true;}                        //Ok!
        //Завершим режим массированной записи в БД
        inline void EndUpdate(void){                                              //Ok!
                        isUpdateMode=false;
                        LocalTR->Commit();
                        ReadCache();
                };
        //Создадаим объект для пользователя -1 - текущего, 0 - root, либо для другого по его UID
        bool  CreateKey(const std::string Key,const int IdUser = -1);                 //Ok!
        //Удалим объект для пользователя -1 - текущего, 0 - root, либо для другого по его UID
        bool  DeleteKey(const std::string Key,const int IdUser = -1);                 //Ok!
        //Проверим существование объекта
        bool  KeyExists(const std::string Key,const int IdUser = -1);                 //Ok!
        //Откроем объект
        bool  OpenKey(const std::string Key, const bool CanCreate=false, const int IdUser = -1);  //Ok!
        //Откроем объект только для чтения - пока не поддерживаеться
        bool  OpenKeyReadOnly(const std::string Key, const int IdUser=-1);       //Ok!

        //Прочитаем объект со всеми параметрами и подобъектами из файла
        bool  LoadKeysFromFile(const std::string UserName, const std::string FileName);
        //Переместим (скопируем при Delete==false) объект в другую ветку
        bool  RenameKey(const std::string OldName, const std::string NewName,const int IdUser = -1);    //Ok!
        //Скопируем ключ со веми параметрами
        bool  CopyKey(const std::string OldName, const int OldUid, const std::string NewName, const int NewUid);
        //Сохраним объект со всеми подобъектами и параметрами в файл
        bool  SaveKeysToFile(const std::string UserName, const std::string FileName);
        //Получим информацию об объекте
        //bool  GetKeyInfo(TEtcKeyInfo &Value);

        //Удалим параметр
        bool  DeleteValue(const std::string Name);                               //Ok!
        //Получим информацию о параметре
        //void  GetDataInfo(const std::string ValueName, TEtcDataInfo* &Value);
        //Получим размер данных
        int  GetDataSize(const std::string ValueName);
        //Получим тип параметра
        EtcParamType  GetDataType(const std::string ValueName);                 //Ok!
        //Получим список параметров
        std::vector<std::string>  GetValueNames();                               //Ok!
        //Прочитаем двоичные данные в файл или поток
        bool  ReadBinaryData(const std::string Name,std::iostream &Stream);       //Ok!
        bool  ReadBinaryData(const std::string Name,const std::string FileName); //Ok!
        //Пишем Блоб в DBEtc из файла или потока
        bool  WriteBinaryData(const std::string Name,std::iostream &Stream);      //Ok!
        bool  WriteBinaryData(const std::string Name,const std::string FileName); //Ok!
        //Читаем булево значение параметра
        bool  ReadBool(const std::string Name);                                  //Ok!
        //Читаем значение типа Дата/Время параметра
        IBPP::Timestamp  ReadDateTime(const std::string Name);                  //Ok!
        //Читаем вещественный параметр
        double  ReadFloat(const std::string Name);                              //Ok!
        //Читаем целочисленный параметр
        int  ReadInteger(const std::string Name);                                //Ok!
        //Читаем параметр типа большое целое
        int64_t  ReadInt64(const std::string Name);                              //Ok!
        //Читаем короткую строку
        std::string  ReadShortString(const std::string Name);                    //Ok!
        //Читаем строку из bloba
        std::string  ReadString(const std::string Name);                         //Ok!
        //Переименуем параметр
        void  RenameValue(const std::string OldName, const std::string NewName); //Ok!
        //Проверим существование параметра
        bool  ParamExists(const std::string Name);                               //Ok!
        //пишем значение параметра соответсвующего типа
        void  WriteBool(const std::string Name, bool Value);                     //Ok!
        void  WriteDateTime(const std::string Name, IBPP::Timestamp Value);      //Ok!
        void  WriteFloat(const std::string Name, double Value);                  //Ok!
        void  WriteInteger(const std::string Name, int Value);                   //Ok!
        void  WriteInt64(const std::string Name, int64_t Value);                 //Ok!
        void  WriteShortString(const std::string Name, const std::string Value); //Ok!
        void  WriteString(const std::string Name, const std::string Value);      //Ok!

};
};
#endif
