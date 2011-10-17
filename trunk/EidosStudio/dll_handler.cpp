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
        FillDLLGrid();

}

void mw_DLL_handler::FillDLLGrid()
{
    //Процедура заполняет таблицу зарегистрированными в программе библиотеками, устанавливая текущим элементом элемент, переданный в параметре CurItem, и колонку переданную в параметре Num

        ui.tableWidget->setSortingEnabled(false);
        ui.tableWidget->clearContents(); //Очищаем внутреннее содержимое таблицы

        IBPP::Transaction TmpTR=IBPP::TransactionFactory(this->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement TmpST=IBPP::StatementFactory(this->DB, TmpTR);
        TmpTR->Start();
        TmpST->Prepare("select * from GET_REGED_DLLS;");
        TmpST->Execute();
        int i=0;
        while (i<1)
        {
            std::string DLL_Name;
            std::string MD5_SUMM;
            TmpST->Get("DLL_NAME",DLL_Name);
            TmpST->Get("MD5_SUMM",MD5_SUMM);

            ui.tableWidget->setColumnCount(i);
            QTableWidgetItem* AnOneItem =new QTableWidgetItem();
            QTableWidgetItem* AnTwoItem =new QTableWidgetItem();
            AnOneItem->setText(QString::fromStdString(DLL_Name));
            AnTwoItem->setText(QString::fromStdString(MD5_SUMM));

            ui.tableWidget->setItem(i,0, AnOneItem);
            ui.tableWidget->setItem(i,1, AnTwoItem);


            //Считываем значения текущей записи для библиотеки

/*            ui.tableWidget->item(i, 1)->setIcon(icon_locked);
            ui.tableWidget->item(i, 1)->setText(tr("Н/Д"));
            ui.tableWidget->item(i, 1)->setToolTip(tr("Атрибут определен на другом уровне Eidos"));*/
/*
            QFont currentFont=ui.tableWidget->item(i,0)->font();
            currentFont.setItalic(true);
            for (int j=0;j<2;j++)	//Создаем элементы таблицы
                    ui.tableWidget->item(i,j)->setFont(currentFont);*/
            i++;
        }

        ui.tableWidget->resizeColumnsToContents();
        ui.tableWidget->setSortingEnabled(true);

        //Устанавливаем текущим элементом первый элемент и первую видимую колонку
        //ui.tableWidget->setCurrentCell(0,Num);
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
