/*
 * DataClass.cpp
 *
 *  Created on: 08.02.2009
 *      Author: slava
 */

#include "DataClass.h"

namespace platon
{

DataClass::DataClass(QObject *parent=0) :QObject(parent)
{
	// TODO Auto-generated constructor stub
	this->setObjectName("DataModule");
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
				QLibrary* OneLib= new QLibrary(*it);
				if(!OneLib->isLoaded()) OneLib->load();
				this->ArrayDynLib.insert(*it, OneLib);

				sps->showMessage(QObject::tr("Загрузка библиотеки ")+OneLib->fileName(), Qt::AlignRight);
				aplic->processEvents();

		}
	}
}
DataClass* GetDataModule(QObject * fromform)
{
	QString DTLObjectName="DataModule";
	if(fromform->findChild<DataClass *>(DTLObjectName)==0)	//У переданного в параметрах объекте нет деток с типом DataModule
		if(fromform->parent()!=0)				//но при этом есть родители
			return GetDataModule(fromform->parent());	//переходим на рассмотрение родителя
		else
			return 0;							//Это последний в цепочке
	else
		return fromform->findChild<DataClass *>(DTLObjectName);//Есть детка нужного типа
}

}
