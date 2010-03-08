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
	while(true)
	{
		if(LoginDLG->exec()==QDialog::Rejected)
		{
			((mainWin*)this->parent())->close();	//Посылаем сигнал на закрытие главной формы
			break;
		}
		else
		{
			DB = IBPP::DatabaseFactory(LoginDLG->Host->text().toStdString() ,
										 LoginDLG->Alias->text().toStdString(),
										 LoginDLG->UserName->text().toStdString(),
										 LoginDLG->Password->text().toStdString(),
										"",//Role
										"UTF8",//codepage
										"DEFAULT CHARACTER SET RUSSIAN");//Доп параметры
			DB->Connect();
			if(DB->Connected()) break;
		}
	}
}

DataClass::~DataClass()
{
	// TODO Auto-generated destructor stub
}

void DataClass::LoadDynLib(QSplashScreen* sps, QApplication * aplic)
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
		    //	QMessageBox::about (0,tr("1"),CalculatedHash);
			if(CalcFileMD5(Folder.absoluteFilePath(*it))==GetSavedMD5(*it))	//Проверяем соответствие найденной библиотеки и той,которая прописана в базе данных по MD5
			{
				QLibrary* OneLib= new QLibrary(*it);
				if(!OneLib->isLoaded()) OneLib->load();
				//Ключ - убираем расширение библиотеки т.к. возможна работа с разными платформами
				QString Key= OneLib->fileName().left(OneLib->fileName().lastIndexOf("."));

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

}
