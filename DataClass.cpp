/*
 * DataClass.cpp
 *
 *  Created on: 08.02.2009
 *      Author: slava
 */

#include "DataClass.h"
#include "mainWin.h"


namespace platon
{

DataClass::DataClass(QObject *parent=0) :QObject(parent)
{
	//Создание класса модуля данных
	this->setObjectName("DataModule");
	//Вывод диалога регистрации в базе данных
	platon::Login_Dialog* LoginDLG= new platon::Login_Dialog((mainWin*)parent);
    IsDatabaseConnect=false;
	while(true)
	{
		if(LoginDLG->exec()==QDialog::Rejected)
            break;  //Выход без попытки соединения
		else
		{
			try
			{
				DB = IBPP::DatabaseFactory(LoginDLG->Host->text().toStdString() ,
											 LoginDLG->Alias->text().toStdString(),
											 LoginDLG->UserName->text().toStdString(),
											 LoginDLG->Password->text().toStdString(),
											"",//Role
											"UTF8",//codepage
											"DEFAULT CHARACTER SET RUSSIAN");//Доп параметры
				DB->Connect();                
			}
			catch (IBPP::Exception& e)
			{
				QMessageBox::warning(0,tr("При попытке подключиться к БД возникло исключение:"),
						QString::fromStdString(e.ErrorMessage()),
						QMessageBox::Ok,QMessageBox::Ok);
			}

            if(DB->Connected())
            {
                IsDatabaseConnect=true;
                MyETC=new platon::DbEtc(this->DB);
                FileETC= new QSettings("PLATON","Platon-accs");
                MyETC->OpenKey(QString("ProgramConfig").toStdString(),true,-1);
                if(MyETC->ParamExists("Use_ETCType")) usedTypeETC = MyETC->ReadInteger("UseETCType");
                MyETC->CloseKey();
                break;
            }
		}
	}
}

DataClass::~DataClass()
{
	// TODO Auto-generated destructor stub
    if(IsDatabaseConnect==true) delete MyETC;
}

void DataClass::LoadDynLib(QSplashScreen* sps, QApplication * aplic)
{
    if(IsDBConnected())
    {
        //Процедура производит загрузку динамических библиотек в программу
        this->ArrayDynLib.clear();	//Очищаем массив динамических библиотек

        QString ProgDir=QDir::currentPath();
        QDir Folder(ProgDir);
        if(Folder.cd("DLL"))	//Если каталог существует
        {
            QStringList DLLList=Folder.entryList (QDir::Files, QDir::NoSort);
            QStringList::iterator it;
            //Считываем наименования библиотек помещенных в каталог
            for(it=DLLList.begin();it!=DLLList.end();++it)
            {
                if(CalcFileMD5(Folder.absoluteFilePath(*it))==GetSavedMD5(*it))	//Проверяем соответствие найденной библиотеки и той,которая прописана в базе данных по MD5
                {
                                    int PointBeginDot=Folder.absolutePath().length();
                                    QString OneLibFileName=Folder.absoluteFilePath(*it).left(Folder.absoluteFilePath(*it).indexOf('.',PointBeginDot));
                                    QString Key= *it;
                                    Key=Key.left(Key.indexOf('.',0));

                                    QLibrary* OneLib= new QLibrary(OneLibFileName);
                                    //Ключ - убираем расширение библиотеки т.к. возможна работа с разными платформами
                    this->ArrayDynLib.insert(Key, OneLib);

                    sps->showMessage(QObject::tr("Загрузка библиотеки ")+OneLib->fileName(), Qt::AlignRight);
                    aplic->processEvents();
                }
                else
                {
                    QMessageBox::warning(0,tr("Внимание !"),
                            tr("Библиотека ")+Folder.absoluteFilePath(*it)+tr(" не загружена - MD5 сумма не совпадает"),
                            QMessageBox::Ok,QMessageBox::Ok);

                }

            }
        }
    }
}

QLibrary* DataClass::GetLibByName(QString inString)
{
//Функция возвращает ссылку на библиотеку, имя которой передано в параметре.
//В параметре должно быть передано имя процедуры без расширения
	QMap<QString, QLibrary*>::const_iterator i = ArrayDynLib.find(inString);
	if(i == ArrayDynLib.end())
		 return NULL;
	 else
		 return i.value();
}

DataClass* GetDataModule(QObject * fromform)
{
	//Процедура возвращает адрес модуля данных используя иерархию объектов QObject. Применимо для форм системы.
	QString DTLObjectName="DataModule";
	if(fromform->findChild<DataClass *>(DTLObjectName)==0)	//У переданного в параметрах объекте нет деток с типом DataModule
		if(fromform->parent()!=0)				//но при этом есть родители
			return GetDataModule(fromform->parent());	//переходим на рассмотрение родителя
		else
			return 0;							//Это последний в цепочке
	else
		return fromform->findChild<DataClass *>(DTLObjectName);//Есть детка нужного типа
}
void DataClass::SetProgramDateTime()
{
	//Проверяем попадает ли установка даты времени в диапазо +-5 минут от системного времени
	if(QDateTime::currentDateTime() < this->ProgramDateTime.addSecs(5*60) && QDateTime::currentDateTime().addSecs(5*60) > this->ProgramDateTime)
	{
		ProgramDateTime= QDateTime::currentDateTime();
	}
	platon::SetTimestampTemporalCompareFor(this->DB, platon::QDateTime2IBPPTimestamp(ProgramDateTime));
}

QString DataClass::CalcFileMD5(QString fileName)
{
	//Процедура рассчитывает MD5 хэш файла, имя которого передано в параметре

	QString Result="";
	QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
    	QByteArray content = file.readAll();
    	Result = QString::fromAscii(QCryptographicHash::hash(content, QCryptographicHash::Md5).toHex());
    }
    return Result;
}
QString DataClass::GetSavedMD5(QString fileName)
{
	//Процедура по имени файла находит сохраненное значение MD5 в базе данных

	return QString::fromStdString(GetDLLMD5(this->DB, fileName.toStdString()));
}
bool DataClass::IsDBConnected()
{
    return IsDatabaseConnect;
}

//Методы для работы с параметрами форм. Работа ведется либо с локальным файлом либо с базой данных
//локальный файл используется для снижения трафика с сервером базы данных
void DataClass::SetTypeETC(int TypeForUse)
{
    switch(TypeForUse)
        {
        case ETC_database:
        case ETC_localfile:
            // Для сохранения и присвоения используется только предопределенный тип и запись показателя
            // проводится только в базу DBETC откуда считывается при старте программы

            MyETC->OpenKey(QString("ProgramConfig").toStdString(),true,-1);
            MyETC->WriteInteger("UseETCType", TypeForUse);
            MyETC->CloseKey();
            usedTypeETC=TypeForUse;
            break;
        }
}
int  DataClass::GetTypeETC()
{
    return (usedTypeETC);
}

bool DataClass::ETC_ParamExists(QString ParamName)
{
    bool ForReturn;
    switch(usedTypeETC)
        {
        case ETC_database:
            ForReturn = MyETC->ParamExists(ParamName.toStdString());
            break;
        case ETC_localfile:
            ForReturn = FileETC->contains(ParamName);
            break;
        }
    return ForReturn;
}

void DataClass::ETC_OpenKey(QString key_val)
{
    switch(usedTypeETC)
        {
        case ETC_database:
            MyETC->OpenKey(key_val.toStdString(),true,-1);
            break;
        case ETC_localfile:
            FileETC->beginGroup(key_val);
            break;
        }
}

void DataClass::ETC_CloseKey()
{
    switch(usedTypeETC)
        {
        case ETC_database:
            MyETC->CloseKey();
            break;
        case ETC_localfile:
            FileETC->endGroup();
            break;
        }
}

bool DataClass::ETC_ReadBool(QString ParamName)
{
    bool ForReturn;
    switch(usedTypeETC)
        {
        case ETC_database:
            ForReturn = MyETC->ReadBool(ParamName.toStdString());
            break;
        case ETC_localfile:
            ForReturn = FileETC->value(ParamName).toBool();
            break;
        }
    return ForReturn;
}

int  DataClass::ETC_ReadInteger(QString ParamName)
{
    int ForReturn;
    switch(usedTypeETC)
        {
        case ETC_database:
            ForReturn = MyETC->ReadInteger(ParamName.toStdString());
            break;
        case ETC_localfile:
            ForReturn = FileETC->value(ParamName).toInt();
            break;
        }
    return ForReturn;
}

void DataClass::ETC_WriteBool(QString ParamName, bool InValue)
{
    switch(usedTypeETC)
        {
        case ETC_database:
            MyETC->WriteBool(ParamName.toStdString(), InValue);
            break;
        case ETC_localfile:
            FileETC->setValue(ParamName, InValue);
            break;
        }
}

void DataClass::ETC_WriteInteger(QString ParamName, int InValue)
{
    switch(usedTypeETC)
    {
    case ETC_database:
        MyETC->WriteInteger(ParamName.toStdString(), InValue);
        break;
    case ETC_localfile:
        FileETC->setValue(ParamName, InValue);
        break;
    }
}


}
