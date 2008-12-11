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

#include "eidos.h"


namespace platon
{

Eidos::Eidos(IBPP::Database MyDB)
{
//Конструктор класса, основной задачей которого является создание пустого экземпляра
//объекта. Применяется он для создания нового объекта и для служебных нужд самого класса
        this->DB=MyDB;
        Initialize();
        this->id =0;	//Для нового объекта важно чтобы это значение было нулевым
        this->id_parent =0;

        this->HypotesisSQL=new HypotesisSQLManager(this);
        this->PragmaSQL=new HypotesisPragmaSQLManager(this);
}

Eidos::~Eidos()
{
	//Деструктор класса, освобождаем память
        CleanExtraAttributes();
        delete HypotesisSQL;
}

Eidos::Eidos(IBPP::Database MyDB, long ID_IN)
{
	//Перегруженный конструктор в качестве параметра получает идентификатор
        //объекта класса
        this->DB=MyDB;
        Initialize();
        GetData(this,ID_IN);
        //Загружаем данные об имеющихся у объекта собственных аттрибутах
        GetExtraAttributesSet();
        this->HypotesisSQL=new HypotesisSQLManager(this);
        this->PragmaSQL=new HypotesisPragmaSQLManager(this);
        FillEASQLProp();
}

void Eidos::Initialize()
{
	//Инициализация внутренних переменных, создание элементов управления для объекта
        LocalTR=IBPP::TransactionFactory(this->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        Attributes.clear();
        FullName="";
        DT.Now();       //Текущее системное время
}

void Eidos::GetExtraAttributesSet()
{
//Метод пробегает по всем объектам класса Eidos и вызывает процедуру создания
//списка атрибутов данного класса из данных в БД. Процесс идет до тех пор,
//пока объект не окажется корневым
	QueryForExtraAttrib(this);//Получение атрибутов собственно объекта
        FullName=this->Name;
	if(this->id_parent>0)	//Нужно получить дополнительные атрибуты объектов выше по иерархии
        {
	        Eidos* tmp = new Eidos(this->DB);	//Временный объект аналогичного класса
                long Parent_ID=this->id_parent;
                do
                {
                	GetData(tmp,Parent_ID);	//Заполняем атрибуты временного класса
			QueryForExtraAttrib(tmp);	//Запрашиваем его экстраатрибуты
                        FullName=tmp->Name+"/"+FullName; //Формируем строку с полным именем
                        Parent_ID=tmp->id_parent;	//Присвиваем указатель на более высокий в иерархии класс
                }while(Parent_ID>0);
                delete tmp;
        }
        FullName="/"+FullName;  //Приписываем лидирующий слэш
}

void Eidos::GetData(Eidos* InClass, long ID_IN)
{
//Процедура опрашивает БД на свойства, сохраненные для объекта типа Eidos (переданного в параметре)
//и заполняет данными поля класса. В случае если запись для объекта в БД отсутствует вызывается ошибка

	//Загружаем данные класса, записанные в БД
        IBPP::Statement LocalST=IBPP::StatementFactory(DB, LocalTR);
        if(!LocalTR->Started())LocalTR->Start();
        LocalST->Prepare("SELECT * FROM GET_EIDOS(?);");
        LocalST->Set(1,(int32_t)ID_IN);
        LocalST->Execute();

        //Проверяем совпадает ли идентификатор запрошенного объекта с возвращенным
        if(LocalST->Fetch())
        {
        	//Получаем значения сохраненные в БД для указанного объекта
        	LocalST->Get("id",(int32_t*)&InClass->id);
                LocalST->Get("ID_PARENT",(int32_t*)&InClass->id_parent);
                LocalST->Get("NAME",InClass->Name);
                LocalST->Get("SPECIES",InClass->Species);
        }
        else
        {
        	throw "Объект на который указывает идентификатор не существует";
        }
}

void Eidos::QueryForExtraAttrib(Eidos* InObj)
{
//Процедура запрашивает в БД экстраатрибуты для объекта, переданного в параметре
//На основании полученных данных происходит создание объектов экстраатрибутов самого
//класса, элементы которых сохраняются в списке ссылок Attributes текущего экземпляра класса

        IBPP::Statement LocalST=IBPP::StatementFactory(DB, LocalTR);
        if(!LocalTR->Started())LocalTR->Start();
        LocalST->Prepare("select * from GET_EIDOSEXTRAATTRIB_LIST(?);");
        LocalST->Set(1,(int32_t)InObj->id);
        LocalST->Execute();

        //Проверяем совпадает ли идентификатор запрошенного объекта с возвращенным
        while(LocalST->Fetch())
        {
               	ExtraAttribute* OneRecord;
                OneRecord=new ExtraAttribute;

                LocalST->Get("FIELDTYPE",OneRecord->type);
                LocalST->Get("ID",(int32_t*)&OneRecord->id);
                LocalST->Get("CAPTION",OneRecord->Caption);
                OneRecord->id_Class  =InObj->id;
                LocalST->Get("ID_BELONGFOR",(int32_t*)&OneRecord->belongTo);
                LocalST->Get("ID_RB_DESCRIBER",(int32_t*)&OneRecord->ID_RB_Describer);
                LocalST->Get("DLL_FILENAME",OneRecord->DLL_FileName);
                LocalST->Get("DLL_PROCNAME",OneRecord->DLL_ProcName);
                LocalST->Get("FIELDNAME",OneRecord->FieldName);
                LocalST->Get("TEMPORALITY",OneRecord->Temporality);
                LocalST->Get("REQUIRED",OneRecord->Required);
                LocalST->Get("VISIBLE",OneRecord->Visible);
                LocalST->Get("LOCKED",OneRecord->Locked);
                LocalST->Get("NAMESTOREDPROC",OneRecord->sNameStoredProc);
                OneRecord->sNameStoredProc=Rtrim(OneRecord->sNameStoredProc);
                LocalST->Get("TEMPORALLISTSPNAME",OneRecord->sTemporalListSPName);
                LocalST->Get("Multilnk",OneRecord->Multilnk);
                LocalST->Get("LNK_species",OneRecord->LNK_species);
                LocalST->Get("LNK_EidosID",(int32_t*)&OneRecord->LNK_EidosID);
                LocalST->Get("LNK_HypID",(int32_t*)&OneRecord->LNK_HypID);
                LocalST->Get("LNK_NeedList",OneRecord->LNK_NeedList);


                OneRecord->HostEidos=this;
                OneRecord->DTValue=this->DT;
                OneRecord->strClass_NodeName =InObj->Name;
                this->Attributes.push_back(OneRecord);
        }
}

void Eidos::SetParentID(long value)
{//Потенциально небезопасная процедура надо доработать проверку на наличие у изменяемых эйдосов прагм и гипотез для контроля за целостностью данных
	if(id_parent != value ) {
		id_parent = value;
		if(this->id > 0)
                {
                	//В случае, если это не новый объект изменение значения
                	//приведет к его перезаписи и перечитыванию информации
                	//о дополнительных атрибутах
                	CleanExtraAttributes();
                        Save();
			QueryForExtraAttrib(this);
                }
	}
}
long Eidos::GetParentID()const
{
	return id_parent;
}

void Eidos::CleanExtraAttributes()
{
//Процедура очищает все имеющиеся в распоряжении объекта атрибуты из памяти
	for(unsigned int i =0;i<Attributes.size();i++) delete ((ExtraAttribute*)Attributes[i]);
        Attributes.clear();
}

void Eidos::Save()
{
//Запись значений аттрибутов класса в базу данных
        IBPP::Statement LocalST=IBPP::StatementFactory(DB, LocalTR);
        if(!LocalTR->Started())LocalTR->Start();
        LocalST->Prepare("EXECUTE PROCEDURE SET_EIDOS(?,?,?,?);");
        LocalST->Set(1,(int32_t)this->id);
        LocalST->Set(2,(int32_t)this->id_parent);
        LocalST->Set(3,this->Name);
        LocalST->Set(4,this->Species);
        LocalST->Execute();
        LocalST->Get("ID_OUT",(int32_t*)&this->id);
        LocalTR->Commit();
}

void Eidos::SetName(std::string value)
{
	if(Name != value) {
		Name = value;
                if(this->id >0)
                {
                //В случае, если это не новый объект изменение значения
                //приведет к его перезаписи и перечитыванию информации
                //о дополнительных атрибутах
	                CleanExtraAttributes();
        	        Save();
                	QueryForExtraAttrib(this);
                }
	}
}
std::string Eidos::GetName()const
{
	return Name;
}


long Eidos::AddExtraAttribute(std::string Caption, int FieldType)
{
//Процедура добавляет дополнительный атрибут к объекту

	//Необходима переработка для перевода на полиморфные объекты
        ExtraAttribute* OneRecord =new ExtraAttribute;
        OneRecord->Caption =Caption;
        OneRecord->id_Class  =this->id;
        OneRecord->type =FieldType;
        OneRecord->belongTo =ExtraAttribute::_theHypotesis;
        //Получим уникальное имя поля из БД
        IBPP::Statement LocalST=IBPP::StatementFactory(DB, LocalTR);
        if(!LocalTR->Started())LocalTR->Start();
        LocalST->Prepare("EXECUTE PROCEDURE GET_EA_NEWFIELDNAME;");
        LocalST->Execute();
        LocalST->Get("FLDNAME",OneRecord->FieldName);
        LocalTR->Commit();

        OneRecord->id=OneRecord->Save();
        this->Attributes.push_back(OneRecord);
        return OneRecord->id;
}

long Eidos::GetID()const
{
//Получаем ID Eidos-а
	return id;
}

void Eidos::FillEASQLProp()
{
//Процедура заполняет списки экстраатрибутов SQL строк для класса
        HypotesisSQL->Clear();
        PragmaSQL->Clear();

        ExtraAttribute* OneAttrib;	//Указатель на элемент объекта класса дополнительного атрибута
        for(int i=this->Attributes.size()-1;i>=0;i--)	//Последовательно для всех дополнительных атрибутов
        {
                OneAttrib=(ExtraAttribute*)this->Attributes[i];
                if(OneAttrib->belongTo==ExtraAttribute::_theHypotesis && OneAttrib->Visible && OneAttrib->type!=ft_Security)//Выбираем атрибуты определенные для типов объектов учета
                {
                        HypotesisSQL->AddFieldVisibleValue(OneAttrib);
                }
                if(OneAttrib->belongTo==ExtraAttribute::_thePragma && OneAttrib->Visible  && OneAttrib->type!=ft_Security)
                {
                        PragmaSQL->AddFieldVisibleValue(OneAttrib);
                }
        }
}

int Eidos::NumEAByFieldName(std::string FieldName)const
{
//Функция находит и возвращает по имени атрибута его номер в случае если объект не найден, возвращается -1
        ExtraAttribute* tmpAttrib;
        int for_return=-1;
        for(unsigned int i=0;i<this->Attributes.size();i++)
        {
                tmpAttrib=(ExtraAttribute*)this->Attributes[i];
                if(ToUpper(tmpAttrib->GetEAFieldName())== ToUpper(FieldName))
                {
                	for_return=i;
                	break;
                }
        }
        return for_return;
}

ExtraAttribute* Eidos::GetEAByFieldName(std::string FieldName)
{
//Процедура возвращает ссылку на экстраатрибут по имени поля экстраатрибута

        ExtraAttribute* tmpAttrib=NULL;
        int Number = this->NumEAByFieldName(FieldName);
        if(Number>-1) tmpAttrib=(ExtraAttribute*)this->Attributes[Number];
        return tmpAttrib;
}
ExtraAttribute* Eidos::GetEAByNum(int Number)
{
//Процедура возвращает ссылку на экстраатрибут по номеру экстраатрибута в списке
        ExtraAttribute* tmpAttrib=NULL;
        if(Number>-1) tmpAttrib=(ExtraAttribute*)this->Attributes[Number];
        return tmpAttrib;
}


std::string Eidos::GetEidosSpecies()const
{
//Функция возвращает тип Эйдоса (ACT, OBJ)
        return Rtrim(this->Species);
}

std::string Eidos::GetFullEidosName()const
{
//Функция возвращает полное имя эйдоса начиная от корня
        return FullName;
}

void SetTimestampTemporalCompareFor(IBPP::Database MyDB, IBPP::Timestamp DTForSet)
{
//Процедура выполняет установку даты врмени на которое происходит проверка
//темпоральных экстраатрибутов для вывода

        IBPP::Transaction TmpTR=IBPP::TransactionFactory(MyDB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement TmpST=IBPP::StatementFactory(MyDB, TmpTR);
        TmpTR->Start();
        TmpST->Prepare("EXECUTE PROCEDURE SP_SET_VAR_DT(?);");
        TmpST->Set(1,DTForSet);
        TmpST->Execute();
        TmpTR->Commit();
}

long Get_EAID_ByName(IBPP::Database MyDB,std::string NameOfEA)
{
//Процедура получает значение идентификатора экстраатрибута из базы по заданному имени
        long ID_ForReturn;
        IBPP::Transaction TmpTR=IBPP::TransactionFactory(MyDB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement TmpST=IBPP::StatementFactory(MyDB, TmpTR);
        TmpTR->Start();
        TmpST->Prepare("EXECUTE PROCEDURE EA_NUM_BY_NAME(?);");
        TmpST->Set(1,NameOfEA);
        TmpST->Execute();
        TmpST->Get("ID_EA",(int32_t*)&ID_ForReturn);
        TmpTR->Commit();

        return ID_ForReturn;
}

long Get_TopIDBySpecies(IBPP::Database MyDB,std::string NameOfBelong)
{
//Процедура получает значение идентификатора экстраатрибута из базы по заданному имени
        long ID_ForReturn;
        IBPP::Transaction TmpTR=IBPP::TransactionFactory(MyDB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement TmpST=IBPP::StatementFactory(MyDB, TmpTR);
        TmpTR->Start();
        TmpST->Prepare("EXECUTE PROCEDURE GET_ID_TOPSPECIES(?);");
        TmpST->Set(1,NameOfBelong);
        TmpST->Execute();
        TmpST->Get("ID_TOP_SPECIES",(int32_t*)&ID_ForReturn);
        TmpTR->Commit();

        return ID_ForReturn;

}
std::string GetEidosSpecies(IBPP::Database MyDB, long id_eidos)
{
//Функция возвращает тип Эйдоса (ACT, OBJ)
	std::string ForReturn;
    IBPP::Transaction TmpTR=IBPP::TransactionFactory(MyDB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
    IBPP::Statement TmpST=IBPP::StatementFactory(MyDB, TmpTR);
    TmpTR->Start();
    TmpST->Prepare("EXECUTE PROCEDURE GET_EIDOS(?);");
    TmpST->Set(1,(int32_t*)&id_eidos);
    TmpST->Execute();
    TmpST->Get("SPECIES",ForReturn);
    TmpTR->Commit();
    return platon::Rtrim(ForReturn);
}

void BranchDisassemble(const std::string NameOfBranch, std::vector<std::string> &Elements)
{
//Процедура производит разборку переданной строки на составляющие, помещая элементы в массив
        int PointBegin, PointEnd;

        Elements.clear();
        PointBegin=NameOfBranch.find("/root/",0);
        PointEnd=5;

        if(PointBegin==0)//Нормальное условие с началом строки /root/, иначе выход и возвращается 0
        {
                Elements.push_back("root");
                do
                {
                        PointBegin=NameOfBranch.find('/',PointEnd);
                        PointEnd=NameOfBranch.find('/',PointBegin+1);
                        if(PointBegin>0 && PointEnd >0)
                        {
                                Elements.push_back(NameOfBranch.substr(PointBegin+1,PointEnd-PointBegin-1));
                        }
                        PointBegin=PointEnd+1;
                }while (PointBegin>0);
        }
}

long GetEidosIDByBranchFullName(IBPP::Database MyDB,const std::string NameOfBranch)
{
//Процедура разбирает содержание ветки, переданной в параметре на составляющие и создает
//запрос, который двигаясь по иерархии возвращает идентификатор эйдоса. Корректная строка ветки должна иметь
//символы разделителя '/' как в начале строки так и в конце
        long ID_ForReturn=0;
        std::string SQLRequest;
        std::vector <std::string> Elements;

        IBPP::Transaction TmpTR=IBPP::TransactionFactory(MyDB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement TmpST=IBPP::StatementFactory(MyDB, TmpTR);

        BranchDisassemble(NameOfBranch,Elements);

        if(Elements.size()>0 && Elements.at(0)=="root")//Нормальное условие с началом строки /root/, иначе выход и возвращается 0
        {
                SQLRequest="select id from GET_EIDOS_LIST('ALL') where GET_EIDOS_LIST.name='root' and id_parent in (0)";
                for(unsigned int i=1;i<Elements.size();i++)
                {
                        SQLRequest="select id from GET_EIDOS_LIST('ALL') where GET_EIDOS_LIST.name='"+
                        Elements.at(i)+"' and id_parent in ("+SQLRequest+")";
                }

                TmpTR->Start();
                TmpST->Prepare(SQLRequest);
                TmpST->Execute();
                if(TmpST->Fetch())TmpST->Get("id",(int32_t*)&ID_ForReturn);
                TmpTR->Commit();
        }
        return ID_ForReturn;
}

}
