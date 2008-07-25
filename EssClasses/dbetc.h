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
 ������� ��������� beginupdate � endupdate ��� ���������� DbIni,
 �� beginupdate - �������� ������ ��� commit`a
 �� endupdate - ���������� commit � ������������� ������, �
 ���� ������ ����� ��������� ��������� - ������������� ������
*/
namespace platon
{
//�������������� ����
typedef enum {
        ptNone,         //����������� ���
        ptBlob,         //����� ������� �������� ������, ����� ������� (��������, doc, pdf � �.�.)
        ptBool,         //����������
        ptCurrency,     //�������� ��� �� -922337203685477.5808 �� 922337203685477.5807  ������ � Ints
        ptDateTime,     //����-�����
        ptFloat,        //������������
        ptInt,          //�����
        ptInt64,        //������� �����
        ptShortString,  //�������� ������
        ptString,       //������ ������ � �����
} EtcParamType;


typedef std::map<std::string,int,std::less<std::string> > cache_keys;
typedef cache_keys::value_type value_type;

//��� �����: ��������� � �� � ����������� ���������
class DbEtc
{
        int CurrentUserId, CurrentKeyId, KeyUserId, bmCurrentKeyId, bmKeyUserId;
        bool ReadOnly,isOpenKey, bmReadOnly, bmIsOpenKey, isUpdateMode;
        std::string CurrentUser, OpenedKey, bmOpenedKey;
        EtcParamType  NameToType(const std::string Name);
        IBPP::Database DB;            //���� ������ IBPP
        IBPP::Transaction LocalTR;
        IBPP::Statement stKey,stParameters;
        std::vector<IBPP::Row> Parameters;   //��������� ����������� �����
        void GetCurrentUser();                                                   //Ok!
        void  GetParameters(const int CurrentKeyId);                                   //Ok!
        int  SaveKey(const int id, const std::string Key, const int IdUser);    //Ok!
        int  GetKey(const std::string Key, const int IdUser);                    //Ok!
        void  SaveBookmark();               //�������� ��������� ��������� �����   //Ok!
        bool  RestoreBookmark();            //����������� ��������� ��������� �����   //Ok!
        int  CheckParam(const std::string ValueName);  //�������� ������������� ��������� ��� ��������� ������� //Ok!
        void PrepareWriteParamValue(const std::string SQL, const std::string Name);    //Ok!
        void ExecuteWriteParamValue();
        void Init(void);  //Ok!
        cache_keys AllUserKeys;
        int  KeyCacheExists(const std::string Key);
        void ReadCache(void);
public:
        //����������
        class DbEtcError{};
        class ErrorWriteKey:public DbEtcError{};
        class KeyReadOnly:public DbEtcError{};    //������ ������ ������ ��� ������
        class KeyNotOpen:public DbEtcError{};     //������ �� ������
        class BadValue:public DbEtcError{};       //������������ �������� ���������
        class MismatchTypeValue:public DbEtcError{}; //������������� ���� ��������� � ��������
        class ErrWriteValue:public DbEtcError{};     //������ ���������� ���������
        class UnkownParameter:public DbEtcError{};     //����������� ��������
        class NonRegisterUser:public DbEtcError{};
        class NotReadMode:public DbEtcError{}; //������������ ����� �������� ������, ������ ����������!
        class BigNameKey: public DbEtcError{};
        class BigNameValue: public DbEtcError{};
        IBPP::Timestamp DT;           //���� �����

        //�����������
        DbEtc(IBPP::Database myDB);                                             //Ok!
        DbEtc(std::string HostName, std::string DatabaseName,
                std::string UserName, std::string Password, std::string Role,
                std::string CharSet, std::string CreateParams);                  //Ok!
        //����������
         ~DbEtc(void);                                                          //Ok!
        //������� ������
        void  CloseKey(void);                                                     //Ok!
        //������� ����� ������������� ������ � ��
        inline void BeginUpdate(void) {isUpdateMode=true;}                        //Ok!
        //�������� ����� ������������� ������ � ��
        inline void EndUpdate(void){                                              //Ok!
                        isUpdateMode=false;
                        LocalTR->Commit();
                        ReadCache();
                };
        //��������� ������ ��� ������������ -1 - ��������, 0 - root, ���� ��� ������� �� ��� UID
        bool  CreateKey(const std::string Key,const int IdUser = -1);                 //Ok!
        //������ ������ ��� ������������ -1 - ��������, 0 - root, ���� ��� ������� �� ��� UID
        bool  DeleteKey(const std::string Key,const int IdUser = -1);                 //Ok!
        //�������� ������������� �������
        bool  KeyExists(const std::string Key,const int IdUser = -1);                 //Ok!
        //������� ������
        bool  OpenKey(const std::string Key, const bool CanCreate=false, const int IdUser = -1);  //Ok!
        //������� ������ ������ ��� ������ - ���� �� ���������������
        bool  OpenKeyReadOnly(const std::string Key, const int IdUser=-1);       //Ok!

        //��������� ������ �� ����� ����������� � ������������ �� �����
        bool  LoadKeysFromFile(const std::string UserName, const std::string FileName);
        //���������� (��������� ��� Delete==false) ������ � ������ �����
        bool  RenameKey(const std::string OldName, const std::string NewName,const int IdUser = -1);    //Ok!
        //��������� ���� �� ���� �����������
        bool  CopyKey(const std::string OldName, const int OldUid, const std::string NewName, const int NewUid);
        //�������� ������ �� ����� ������������ � ����������� � ����
        bool  SaveKeysToFile(const std::string UserName, const std::string FileName);
        //������� ���������� �� �������
        //bool  GetKeyInfo(TEtcKeyInfo &Value);

        //������ ��������
        bool  DeleteValue(const std::string Name);                               //Ok!
        //������� ���������� � ���������
        //void  GetDataInfo(const std::string ValueName, TEtcDataInfo* &Value);
        //������� ������ ������
        int  GetDataSize(const std::string ValueName);
        //������� ��� ���������
        EtcParamType  GetDataType(const std::string ValueName);                 //Ok!
        //������� ������ ����������
        std::vector<std::string>  GetValueNames();                               //Ok!
        //��������� �������� ������ � ���� ��� �����
        bool  ReadBinaryData(const std::string Name,std::iostream &Stream);       //Ok!
        bool  ReadBinaryData(const std::string Name,const std::string FileName); //Ok!
        //����� ���� � DBEtc �� ����� ��� ������
        bool  WriteBinaryData(const std::string Name,std::iostream &Stream);      //Ok!
        bool  WriteBinaryData(const std::string Name,const std::string FileName); //Ok!
        //������ ������ �������� ���������
        bool  ReadBool(const std::string Name);                                  //Ok!
        //������ �������� ���� ����/����� ���������
        IBPP::Timestamp  ReadDateTime(const std::string Name);                  //Ok!
        //������ ������������ ��������
        double  ReadFloat(const std::string Name);                              //Ok!
        //������ ������������� ��������
        int  ReadInteger(const std::string Name);                                //Ok!
        //������ �������� ���� ������� �����
        int64_t  ReadInt64(const std::string Name);                              //Ok!
        //������ �������� ������
        std::string  ReadShortString(const std::string Name);                    //Ok!
        //������ ������ �� bloba
        std::string  ReadString(const std::string Name);                         //Ok!
        //����������� ��������
        void  RenameValue(const std::string OldName, const std::string NewName); //Ok!
        //�������� ������������� ���������
        bool  ParamExists(const std::string Name);                               //Ok!
        //����� �������� ��������� ��������������� ����
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
