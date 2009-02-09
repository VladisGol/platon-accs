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
	//��������� ���������� �������� ������������ ��������� � ���������
	this->ArrayDynLib.clear();	//������� ������ ������������ ���������

	QString ProgDir=QDir::currentPath();
	QDir Folder(ProgDir);
	if(Folder.cd("DLL"))	//���� ������� ����������
	{
		QStringList DLLList=Folder.entryList (QDir::Files, QDir::NoSort);
		QStringList::iterator it;
		//��������� ������������ ��������� ���������� � �������
		for(it=DLLList.begin();it!=DLLList.end();++it)
		{
				QLibrary* OneLib= new QLibrary(*it);
				if(!OneLib->isLoaded()) OneLib->load();
				this->ArrayDynLib.insert(*it, OneLib);

				sps->showMessage(QObject::tr("�������� ���������� ")+OneLib->fileName(), Qt::AlignRight);
				aplic->processEvents();

		}
	}
}
DataClass* GetDataModule(QObject * fromform)
{
	QString DTLObjectName="DataModule";
	if(fromform->findChild<DataClass *>(DTLObjectName)==0)	//� ����������� � ���������� ������� ��� ����� � ����� DataModule
		if(fromform->parent()!=0)				//�� ��� ���� ���� ��������
			return GetDataModule(fromform->parent());	//��������� �� ������������ ��������
		else
			return 0;							//��� ��������� � �������
	else
		return fromform->findChild<DataClass *>(DTLObjectName);//���� ����� ������� ����
}

}
