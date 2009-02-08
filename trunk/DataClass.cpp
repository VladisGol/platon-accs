/*
 * DataClass.cpp
 *
 *  Created on: 08.02.2009
 *      Author: slava
 */

#include "DataClass.h"

namespace platon
{

DataClass::DataClass()
{
	// TODO Auto-generated constructor stub

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
}
