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
	//�������� ������ ������ ������
	this->setObjectName("DataModule");
	//����� ������� ����������� � ���� ������
	platon::Login_Dialog* LoginDLG= new platon::Login_Dialog((mainWin*)parent);
	while(true)
	{
		if(LoginDLG->exec()==QDialog::Rejected)
		{
			((mainWin*)this->parent())->close();	//�������� ������ �� �������� ������� �����
			break;
		}
		else
		{
			DB = IBPP::DatabaseFactory(LoginDLG->Host->text().toStdString() ,
										 LoginDLG->Alias->text().toStdString(),
										 LoginDLG->UserName->text().toStdString(),
										 LoginDLG->Password->text().toStdString(),
										"",//Role
										"WIN1251",//codepage
										"DEFAULT CHARACTER SET WIN1251");//��� ���������
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
				//���� - ������� ���������� ���������� �.�. �������� ������ � ������� �����������
				QString Key= OneLib->fileName().left(OneLib->fileName().lastIndexOf("."));

				this->ArrayDynLib.insert(Key, OneLib);

				sps->showMessage(QObject::tr("�������� ���������� ")+OneLib->fileName(), Qt::AlignRight);
				aplic->processEvents();

		}
	}
}

QLibrary* DataClass::GetLibByName(QString inString)
{
//������� ���������� ������ �� ����������, ��� ������� �������� � ���������.
//� ��������� ������ ���� �������� ��� ��������� ��� ����������
	QMap<QString, QLibrary*>::const_iterator i = ArrayDynLib.find(inString);
	if(i == ArrayDynLib.end())
		 return NULL;
	 else
		 return i.value();
}

DataClass* GetDataModule(QObject * fromform)
{
	//��������� ���������� ����� ������ ������ ��������� �������� �������� QObject. ��������� ��� ���� �������.
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
