#include "dll_handler.h"

namespace platon
{
mw_DLL_handler::mw_DLL_handler(QWidget *parent)
    : QMainWindow(parent)
{
        ui.setupUi(this);

        DataClass* DTL=platon::GetDataModule(this);
        this->DB=DTL->DB;

        QObject::connect(ui.action_exit, SIGNAL(activated()), this, SLOT(Exit()));
        ReadFormWidgetsAppearance();
}

mw_DLL_handler::~mw_DLL_handler()
{

}
void mw_DLL_handler::ReadFormWidgetsAppearance()
{
    //Процедура считывает из DbETC параметры элементов формы и устанавливает их значения
    platon::DbEtc* MyETC=new platon::DbEtc(this->DB);
    MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName ()).toStdString(),true,-1);
    int w=800,h=600;
    if(MyETC->ParamExists("width")) w=MyETC->ReadInteger("width");
    if(MyETC->ParamExists("height")) h=MyETC->ReadInteger("height");
    this->resize (w,h);

    MyETC->CloseKey();
    delete MyETC;
}

void mw_DLL_handler::WriteFormWidgetsAppearance()
{
    //Процедура записывает в DbETC параметры элементов формы
    platon::DbEtc* MyETC=new platon::DbEtc(this->DB);
    MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName ()).toStdString(),true,-1);
    MyETC->WriteInteger("width", this->width());
    MyETC->WriteInteger("height", this->height());
    MyETC->CloseKey();
    delete MyETC;

}
void mw_DLL_handler::Exit()
{
//Выход из формы
    WriteFormWidgetsAppearance();
    this->close();
}

}
