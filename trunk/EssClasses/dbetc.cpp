//---------------------------------------------------------------------------
#include "dbetc.h"

using namespace platon;
//---------------------------------------------------------------------------
//Конструктор при передаче ссылки на сущ.БД
//---------------------------------------------------------------------------
DbEtc::DbEtc(IBPP::Database myDB)
{
        DB = myDB;
        Init();
}
//---------------------------------------------------------------------------
//Конструктор при передаче параметров соединения с БД
//---------------------------------------------------------------------------
DbEtc::DbEtc(std::string HostName, std::string DatabaseName, std::string UserName, std::string Password, std::string Role, std::string CharSet, std::string CreateParams)
{
        DB = IBPP::DatabaseFactory(HostName, DatabaseName, UserName, Password, Role, CharSet, CreateParams);            //База данных IBPP
        Init();
};
//---------------------------------------------------------------------------
//Деструктор
//---------------------------------------------------------------------------
DbEtc::~DbEtc(void)
{
        if(LocalTR->Started()) LocalTR->Rollback(); //если до сих пор не подтверждена, откатываем
        AllUserKeys.clear();
        //DB->Disconnect(); Я тут понимаешь коннекчусь к базе, а он ее дисконнектит :)
};
//---------------------------------------------------------------------------
//Получим UID пользователя
//---------------------------------------------------------------------------
void DbEtc::GetCurrentUser()
{
        if (!LocalTR->Started()) LocalTR->Start();
        IBPP::Statement st1 = IBPP::StatementFactory(DB, LocalTR);
        st1->Execute("select * from SP_READ_CURRENT_USER");
        if(!st1->Fetch()) throw(NonRegisterUser());

        st1->Get("UID",CurrentUserId);
        st1->Get("NAME",CurrentUser);
        LocalTR->Commit();
        st1->Close();
};
//---------------------------------------------------------------------------
//Создание объекта
//---------------------------------------------------------------------------
bool  DbEtc::CreateKey(const std::string Key,const int IdUser)
{
//       if(!KeyExists(Key,IdUser))
               return this->SaveKey(-1,Key,IdUser);
         //return false;
};
//---------------------------------------------------------------------------
//Переименуем объект
//---------------------------------------------------------------------------
bool  DbEtc::RenameKey(const std::string OldName, const std::string NewName, const int IdUser)
{
        std::string tmpOpenedKey=OpenedKey;
        if(ReadOnly) throw KeyReadOnly();
        bool result=false;
        if (isOpenKey && OldName!=tmpOpenedKey)
                {
                        SaveBookmark();
                };
        if(this->OpenKey(OldName, false, IdUser))
                {
                        result=this->SaveKey(CurrentKeyId,NewName,IdUser);
                        if (bmIsOpenKey) RestoreBookmark();
                        if(OldName==tmpOpenedKey) this->OpenKey(NewName,false,IdUser);
                };
        return result;
};
//---------------------------------------------------------------------------
//Удалим объект
//---------------------------------------------------------------------------
bool  DbEtc::DeleteKey(const std::string Key,const int IdUser)
{
        if(ReadOnly) throw KeyReadOnly();
        bool result=false;
        if (isOpenKey && Key!=OpenedKey)
                {
                        SaveBookmark();
                };
        if(this->OpenKey(Key, false, IdUser))
                {
                        result=this->SaveKey(CurrentKeyId,"",IdUser);
                        if (bmIsOpenKey) RestoreBookmark();
                        else this->CloseKey();
                };
        return result;
};
//---------------------------------------------------------------------------
//Закроем объект
//---------------------------------------------------------------------------
void  DbEtc::CloseKey(void)
{
        Parameters.clear();
        isOpenKey = false;
        ReadOnly = false;
        CurrentKeyId = -1;
        KeyUserId=-1;
        OpenedKey="";
};
//---------------------------------------------------------------------------
//Откроем объект
//---------------------------------------------------------------------------
bool  DbEtc::OpenKey(const std::string Key, const bool CanCreate, const int IdUser)
{
        if (isOpenKey) CloseKey();
        int result;
        result=GetKey( Key, IdUser);
        if((result<0) && CanCreate)
           result=this->SaveKey(-1,Key,IdUser);
        else if(result<0) return false;
        CurrentKeyId=result;
        isOpenKey=true;
        OpenedKey = Key;
        KeyUserId=IdUser;
        ReadOnly=false; //безопасность - проверка прав доступа
        GetParameters(CurrentKeyId);
        return true;

};
//---------------------------------------------------------------------------
//Откроем объект только для чтения
//---------------------------------------------------------------------------
bool  DbEtc::OpenKeyReadOnly(const std::string Key, const int IdUser)
{
        if (isUpdateMode) throw NotReadMode();
        bool result;
        result=this->OpenKey(Key, false, IdUser);
        if(result) ReadOnly=true;
        return result;
};
//---------------------------------------------------------------------------
//Проверим существование объекта
//---------------------------------------------------------------------------
bool  DbEtc::KeyExists(const std::string Key,const int IdUser)
{
        if (GetKey( Key, IdUser)>-1) return true;
        else return false;
};
//---------------------------------------------------------------------------
//Загрузим объект из базы
//---------------------------------------------------------------------------
int  DbEtc::GetKey(const std::string Key, const int IdUser)
{
        int result=-1;

        if(Key.length()>512) throw BigNameKey();

        if(IdUser==CurrentUserId||IdUser==-1) { //Поищем в кэше
            result=KeyCacheExists(Key);
            if(result>-1) return result; //Нашли - вернем id
        };
        //Не нашли проверим в базе, может что-то изменилось
        if (!LocalTR->Started()) LocalTR->Start();
        IBPP::Statement stKey = IBPP::StatementFactory(DB, LocalTR);
        stKey->Prepare("select * from SP_READ_ETC(?,?)");
        stKey->Set(1,Key);
        stKey->Set(2, IdUser);
        stKey->Execute();
        if (stKey->Fetch()) stKey->Get("ID",result);
        if (!isUpdateMode) LocalTR->Commit();
        stKey->Close();
        return (int)result;
};
//---------------------------------------------------------------------------
//Запишем объект в базу
//---------------------------------------------------------------------------
int  DbEtc::SaveKey(const int id, const std::string Key, const int IdUser)
{
        if(Key.length()>512) throw BigNameKey();
        int result=-1;
        if (!LocalTR->Started()) LocalTR->Start();
        IBPP::Statement stKey = IBPP::StatementFactory(DB, LocalTR);
        stKey->Prepare("execute procedure SP_SET_ETC(?,?,?)");
        stKey->Set(1, id);
        stKey->Set(2,Key);
        stKey->Set(3,IdUser);
        try
           {
            stKey->Execute();
            stKey->Get("RESULT",result);
           }
        catch(...)
                {
                  LocalTR->Rollback();
                  stKey->Close();
                  throw ErrorWriteKey();
                };

        if (!isUpdateMode)
            {
                LocalTR->Commit();
                ReadCache();
            };
        stKey->Close();
        AllUserKeys.insert(value_type(Key,result));
        return result;
};
//---------------------------------------------------------------------------
//Закладка на открытый объкт
//---------------------------------------------------------------------------
void  DbEtc::SaveBookmark()
{
                  bmOpenedKey = OpenedKey;
                  bmCurrentKeyId = CurrentKeyId;
                  bmKeyUserId=KeyUserId;
                  bmReadOnly=ReadOnly;
                  bmIsOpenKey=isOpenKey;

};
//---------------------------------------------------------------------------
//Восстановим по закладке открытый объект
//---------------------------------------------------------------------------
bool  DbEtc::RestoreBookmark()
{
        bool result;
        if(bmIsOpenKey)
                {
                        if(bmReadOnly) result=this->OpenKeyReadOnly(bmOpenedKey, bmKeyUserId);
                        else result=this->OpenKey(bmOpenedKey, false, bmKeyUserId);
                };
        bmOpenedKey = "";
        bmCurrentKeyId = 0;
        bmKeyUserId=-1;
        bmReadOnly=false;
        bmIsOpenKey=false;
        return result;

};
//---------------------------------------------------------------------------
//Получим из базы список параметров
//---------------------------------------------------------------------------
void  DbEtc::GetParameters(const int CurrentKeyId)
{
  //if (isUpdateMode) throw NotReadMode();
  Parameters.clear();
  IBPP::Row r;
  if (!DB->Connected()) DB->Connect();
  if (!LocalTR->Started()) LocalTR->Start();
  stParameters = IBPP::StatementFactory(DB, LocalTR);
  stParameters->Prepare("select * from SP_READ_ETC_PARAMETERS (?) order by name");
  stParameters->Set(1, CurrentKeyId);
  stParameters->Execute();
  while (stParameters->Fetch(r))
        {
                Parameters.push_back(r); /*for (int i = 0; i < rows.size(); i++)  { ; }*/

        };
  if (!isUpdateMode) LocalTR->Commit();
  stParameters->Close();
};

//---------------------------------------------------------------------------
//Получим тип параметра
//---------------------------------------------------------------------------
EtcParamType  DbEtc::GetDataType(const std::string ValueName)
{
    int result=CheckParam(ValueName);
    if(result>=0) Parameters[result]->Get(3,result);
    return (EtcParamType) result;
};

int  DbEtc::CheckParam(const std::string ValueName)
{
     std::string ParamName;
//     int result=0;
     unsigned int sz=Parameters.size();
     for(unsigned int i=0;i<sz;i++)
        {
                Parameters[i]->Get(2,ParamName);
                if(ParamName == ValueName) return i;
        };
     return -1;//result;
};

std::vector<std::string>  DbEtc::GetValueNames()
{
    std::string ParamName;
    std::vector<std::string> Values;
    if (isUpdateMode) throw NotReadMode();
    for(unsigned int i=0;i<Parameters.size();i++)
        {
                Parameters[i]->Get(2,ParamName);
                Values.push_back(ParamName);
        };
    return Values;
};

bool  DbEtc::ReadBool(const std::string Name)
{
     int64_t result;
     if (isUpdateMode) throw NotReadMode();
     int numParam=CheckParam(Name);
     if (numParam<0) throw UnkownParameter();
     if(GetDataType(Name)!= ptBool) throw MismatchTypeValue() ;
     Parameters[numParam]->Get(7,result);
     return (bool) result;
};

int  DbEtc::ReadInteger(const std::string Name)
{
     int64_t result;
     if (isUpdateMode) throw NotReadMode();
     int numParam=CheckParam(Name);
     if (numParam<0) throw UnkownParameter();
     if(GetDataType(Name)!=ptInt) throw MismatchTypeValue();
     Parameters[numParam]->Get(7,result);
     return (int) result;
};

IBPP::Timestamp  DbEtc::ReadDateTime(const std::string Name)
{
     IBPP::Timestamp result;
     if (isUpdateMode) throw NotReadMode();
     int numParam=CheckParam(Name);
     if (numParam<0) throw UnkownParameter();
     if(GetDataType(Name)!=ptDateTime) throw MismatchTypeValue();
     Parameters[numParam]->Get(5,result);
     return  result;
};

int64_t  DbEtc::ReadInt64(const std::string Name)
{
     int64_t result;
     if (isUpdateMode) throw NotReadMode();
     int numParam=CheckParam(Name);
     if (numParam<0) throw UnkownParameter();
     if(GetDataType(Name)!=ptInt64) throw MismatchTypeValue();
     Parameters[numParam]->Get(7,result);
     return  result;
};
std::string  DbEtc::ReadShortString(const std::string Name)
{
     std::string result;
     if (isUpdateMode) throw NotReadMode();
     int numParam=CheckParam(Name);
     if (numParam<0) throw UnkownParameter();
     if(GetDataType(Name)!=ptShortString) throw MismatchTypeValue();
     Parameters[numParam]->Get(8,result);
     return  result;
};

std::string  DbEtc::ReadString(const std::string Name)
{
     int numParam=CheckParam(Name);
     if (isUpdateMode) throw NotReadMode();
     if (numParam<0) throw UnkownParameter();
     if(GetDataType(Name)!=ptString) throw MismatchTypeValue();

     IBPP::Blob bb = IBPP::BlobFactory(DB, LocalTR);
     if(!LocalTR->Started()) LocalTR->Start();
     std::string result;
     Parameters[numParam]->Get(9,bb);
     //bb->Open();
     bb->Load(result);
     bb->Close();
     return result;
};

double  DbEtc::ReadFloat(const std::string Name)
{
     double result;
     if (isUpdateMode) throw NotReadMode();
     int numParam=CheckParam(Name);
     if (numParam<0) throw UnkownParameter();
     if(GetDataType(Name)!=ptFloat) throw MismatchTypeValue();
     Parameters[numParam]->Get(6,result);
     return  result;
};

bool  DbEtc::ParamExists(const std::string Name)
{
        return (bool) (CheckParam(Name)>-1);
};

void DbEtc::PrepareWriteParamValue(const std::string SQL, const std::string Name)
{
        if (ReadOnly) throw KeyReadOnly();
        if (Name.length()>250) throw BigNameValue();
        int numParam=CheckParam(Name);
        int idParam=-1;
        if (numParam>-1)
                Parameters[numParam]->Get(1,idParam);
        if (!DB->Connected()) DB->Connect();
        if (!LocalTR->Started()) LocalTR->Start();
        stParameters = IBPP::StatementFactory(DB, LocalTR);
        stParameters->Prepare(SQL);
        stParameters->Set(1, idParam);
        stParameters->Set(2, Name);
        stParameters->Set(3, CurrentKeyId);
};

void DbEtc::ExecuteWriteParamValue()
{
        int result;
         try
           {
            stParameters->Execute();
            stParameters->Get("RESULT",result);
           }
        catch(...)
                {
                  LocalTR->Rollback();
                  stParameters->Close();
                  throw ErrWriteValue() ;
                };

          if ((result>0)&&(!isUpdateMode))
                {
                        LocalTR->Commit();
                        stParameters->Close();
                        return;
                }
          if (result<1)  //контролируем всю сессию на безошибочность, если что откатываем все!
                {
                        LocalTR->Rollback();
                        stParameters->Close();
                        throw ErrWriteValue() ;
                };
};

bool  DbEtc::DeleteValue(const std::string Name)
{
        int numParam=CheckParam(Name);
        int idParam=-1;
        int result;
        if (numParam>-1)
                Parameters[numParam]->Get(1,idParam);
        else throw UnkownParameter();
        if (!DB->Connected()) DB->Connect();
        if (!LocalTR->Started()) LocalTR->Start();
        stParameters = IBPP::StatementFactory(DB, LocalTR);
        stParameters->Prepare("EXECUTE PROCEDURE SP_SET_PARAMETERS(?, '', NULL, NULL, NULL, NULL, NULL , NULL, NULL)");
        stParameters->Set(1, idParam);
        try
           {
            stParameters->Execute();
            stParameters->Get("RESULT",result);
           }
        catch(...)
                {
                  LocalTR->Rollback();
                  stParameters->Close();
                  throw ErrWriteValue() ;
                };


        if ((result==0)&&(!isUpdateMode))
                {
                        LocalTR->Commit();
                        stParameters->Close();
                        return true;
                }
        if (result!=0)
                {
                        LocalTR->Rollback();
                        stParameters->Close();
                        throw ErrWriteValue() ;
                };
        return true;

};

        //пишем значение параметра соответсвующего типа
void  DbEtc::WriteBool(const std::string Name,const bool Value)
{
        PrepareWriteParamValue("EXECUTE PROCEDURE SP_SET_PARAMETERS(?, ?, 2, ?, NULL, NULL, ? , NULL, NULL)", Name);
        stParameters->Set(4, (int) Value);
        ExecuteWriteParamValue();
};

void  DbEtc::WriteDateTime(const std::string Name, const IBPP::Timestamp Value)
{
        PrepareWriteParamValue("EXECUTE PROCEDURE SP_SET_PARAMETERS(?, ?, 4, ?, ?, NULL, NULL , NULL, NULL)", Name);
        stParameters->Set(4,  Value);
        ExecuteWriteParamValue();
};

void  DbEtc::WriteFloat(const std::string Name, const double Value)
{
        PrepareWriteParamValue("EXECUTE PROCEDURE SP_SET_PARAMETERS(?, ?, 5, ?, NULL, ?, NULL , NULL, NULL)", Name);
        stParameters->Set(4,  Value);
        ExecuteWriteParamValue();
};

void  DbEtc::WriteInteger(const std::string Name, const int Value)
{
        PrepareWriteParamValue("EXECUTE PROCEDURE SP_SET_PARAMETERS(?, ?, 6, ?, NULL, NULL, ? , NULL, NULL)", Name);
        stParameters->Set(4,  Value);
        ExecuteWriteParamValue();
};
void  DbEtc::WriteInt64(const std::string Name, const int64_t Value)
{
        PrepareWriteParamValue("EXECUTE PROCEDURE SP_SET_PARAMETERS(?, ?, 7, ?, NULL, NULL, ? , NULL, NULL)", Name);
        stParameters->Set(4, Value);
        ExecuteWriteParamValue();
};

void  DbEtc::WriteShortString(const std::string Name, const std::string Value)
{
        if (Value.length()>255) throw BadValue();
        PrepareWriteParamValue("EXECUTE PROCEDURE SP_SET_PARAMETERS(?, ?, 8, ?, NULL, NULL , NULL, ?, NULL)", Name);
        stParameters->Set(4, Value);
        ExecuteWriteParamValue();
};

void  DbEtc::WriteString(const std::string Name, const std::string Value)
{
     PrepareWriteParamValue("EXECUTE PROCEDURE SP_SET_PARAMETERS(?, ?, 9, ?, NULL, NULL , NULL, NULL,?)", Name);

     IBPP::Blob bb = IBPP::BlobFactory(DB, LocalTR);
     bb->Save(Value);
     bb->Close();
     stParameters->Set(4, bb);
     ExecuteWriteParamValue();
};

bool  DbEtc::ReadBinaryData(const std::string Name,const std::string FileName)
{
     int numParam=CheckParam(Name);
     if (numParam<0) throw UnkownParameter();
     if(GetDataType(Name)!=ptBlob) throw MismatchTypeValue();

     IBPP::Blob bb = IBPP::BlobFactory(DB, LocalTR);
     Parameters[numParam]->Get(9,bb);
     bb->Open();

    char buffer[4096];
    int len;
    try{
      FILE* file = fopen(FileName.c_str(), "w");
      do
       {
         len = bb->Read(buffer, 4096);
         fwrite(buffer, 1, len, file);
       } while (len == 4096);
      bb->Close();
      fclose(file);
      return true;
    }
    catch(...) {return false;};

};

bool  DbEtc::ReadBinaryData(const std::string Name,std::iostream &Stream)
{
     int numParam=CheckParam(Name);
     if (numParam<0) throw UnkownParameter();
     if(GetDataType(Name)!=ptBlob) throw MismatchTypeValue();

     IBPP::Blob bb = IBPP::BlobFactory(DB, LocalTR);
     Parameters[numParam]->Get(9,bb);
     bb->Open();

    char buffer[4096];
    int len;
    do
    {
      len = bb->Read(buffer, 4096);
      Stream.write(buffer,len);
    } while (len == 4096);
    bb->Close();
    return true;
};
bool  DbEtc::WriteBinaryData(const std::string Name,const std::string FileName)
{
     PrepareWriteParamValue("EXECUTE PROCEDURE SP_SET_PARAMETERS(?, ?, 1, ?, NULL, NULL , NULL, ?, NULL)", Name);
     char buffer[4096];
     int len;
     IBPP::Blob bb = IBPP::BlobFactory(DB, LocalTR);
     bb->Create();
     FILE* file = fopen(FileName.c_str(), "r");
     do
      {
              len = (int)fread(buffer, 1, 4096, file);
              bb->Write(buffer, len);

      } while (len== 4096);
     bb->Close();
     fclose(file);
     stParameters->Set(4, bb);
     ExecuteWriteParamValue();
     return true;
};
bool  DbEtc::WriteBinaryData(const std::string Name,std::iostream &Stream)
{
     PrepareWriteParamValue("EXECUTE PROCEDURE SP_SET_PARAMETERS(?, ?, 1, ?, NULL, NULL , NULL, ?, NULL)", Name);
     char buffer[4096];
     int len;
     IBPP::Blob bb = IBPP::BlobFactory(DB, LocalTR);
     bb->Create();

     bb->Write((char*)Stream.rdbuf() /*buffer*/, Stream.width());

     bb->Close();
     stParameters->Set(4, bb);
     ExecuteWriteParamValue();
     return true;

};


void DbEtc::RenameValue(const std::string OldName, const std::string NewName)
{
        if (NewName.length()>250) throw BigNameValue();
        int numParam=CheckParam(OldName);
        if (numParam<0) throw  UnkownParameter();
        int idParam=-1;
        if (numParam>-1)
                Parameters[numParam]->Get(1,idParam);
        else throw UnkownParameter();
        if (!DB->Connected()) DB->Connect();
        if (!LocalTR->Started()) LocalTR->Start();
        stParameters = IBPP::StatementFactory(DB, LocalTR);
        stParameters->Prepare("EXECUTE PROCEDURE SP_SET_RENAME_PARAMETER(?, ?);");
        stParameters->Set(1, idParam);
        stParameters->Set(2, NewName);

        try
           {
            stParameters->Execute();
           }
        catch(...)
                {
                  LocalTR->Rollback();
                  stParameters->Close();
                  return;
                };
        if (!isUpdateMode) LocalTR->Commit();
        stParameters->Close();
        return;
};

void DbEtc::Init(void)
{
         if (!DB->Connected()) DB->Connect();
         LocalTR = IBPP::TransactionFactory(DB, IBPP::amWrite, IBPP::ilReadCommitted, IBPP::lrWait);
         GetCurrentUser();
         isUpdateMode=false;
         ReadCache();
};
void DbEtc::ReadCache(void)
{
         if (!LocalTR->Started()) LocalTR->Start();
         IBPP::Statement stKeys = IBPP::StatementFactory(DB, LocalTR);
         AllUserKeys.clear();
         stKeys->Prepare("select * from SP_READ_ETC_ALL(?) order by object");
         stKeys->Set(1,CurrentUserId);
         stKeys->Execute();
         std::string stdstring;
         int cashe_id;
         while (stKeys->Fetch())
           {
                stKeys->Get(2, stdstring);
                stKeys->Get(1,cashe_id);
                AllUserKeys.insert(value_type(stdstring,cashe_id));
            };
         LocalTR->Commit();
         stKeys->Close();
};
int  DbEtc::KeyCacheExists(const std::string Key)
{
    cache_keys::iterator p = AllUserKeys.find(Key);
    if(p!=AllUserKeys.end())
        {
            //std::cout<<p->second<<"/n";
            return p->second;
        }
    else return -1;
};
