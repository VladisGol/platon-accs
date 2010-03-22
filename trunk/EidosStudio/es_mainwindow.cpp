#include "es_mainwindow.h"

namespace platon
{
es_mainwindow::es_mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

	DataClass* DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;

	QObject::connect(ui.action_quit, SIGNAL(activated()), this, SLOT(Exit()));

	ReadFormWidgetsAppearance();
}
void es_mainwindow::Exit()
{
	WriteFormWidgetsAppearance();
	this->close();
}

es_mainwindow::~es_mainwindow()
{

}
void es_mainwindow::ReadFormWidgetsAppearance()
{
	//Процедура считывает из DbETC параметры элементов формы и устанавливает их значения
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName ()).toStdString(),true,-1);
	int w=855,h=825;
	if(MyETC->ParamExists("width")) w=MyETC->ReadInteger("width");
	if(MyETC->ParamExists("height")) h=MyETC->ReadInteger("height");
	this->resize (w,h);

	QList<int> vals;
	if(MyETC->ParamExists("splitter_V\\0")) vals.append(MyETC->ReadInteger("splitter_V\\0")); else vals.append(847);
	if(MyETC->ParamExists("splitter_V\\1")) vals.append(MyETC->ReadInteger("splitter_V\\1")); else vals.append(741);
	ui.splitter_V->setSizes(vals);
	vals.clear();
	if(MyETC->ParamExists("splitter_H\\0")) vals.append(MyETC->ReadInteger("splitter_H\\0")); else vals.append(473);
	if(MyETC->ParamExists("splitter_H\\1")) vals.append(MyETC->ReadInteger("splitter_H\\1")); else vals.append(741);
	ui.splitter_H->setSizes(vals);

	MyETC->CloseKey();
	delete MyETC;
}

void es_mainwindow::WriteFormWidgetsAppearance()
{
	//Процедура записывает в DbETC параметры элементов формы
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName ()).toStdString(),true,-1);
	MyETC->WriteInteger("width", this->width());
	MyETC->WriteInteger("height", this->height());

	QList<int> vals = ui.splitter_V->sizes();
	MyETC->WriteInteger("splitter_V\\0", vals.at(0));
	MyETC->WriteInteger("splitter_V\\1", vals.at(1));
	vals=ui.splitter_H->sizes();
	MyETC->WriteInteger("splitter_H\\0", vals.at(0));
	MyETC->WriteInteger("splitter_H\\1", vals.at(1));


	MyETC->CloseKey();
	delete MyETC;

}
}
