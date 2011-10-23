#include "dll_handler.h"

namespace platon
{
mw_DLL_handler::mw_DLL_handler(QWidget *parent)
    : QMainWindow(parent)
{
        ui.setupUi(this);

        DTL=platon::GetDataModule(this);
        this->DB=DTL->DB;

        is_file_process=true;  //Требуется обработка локальной папки с библиотеками
        QString ProgDir=QDir::currentPath();
        QDir Folder(ProgDir);
        if(Folder.cd("DLL")==false)	//Вход в каталог не выполнен
            is_file_process=false;

        DLL_Folder_name= Folder.absolutePath()+QDir::separator();   //Запоминаем имя папки с библиотеками оканчивающееся системным разделителем

        QObject::connect(ui.action_exit, SIGNAL(activated()), this, SLOT(Exit()));
        QObject::connect(ui.action_add_dll, SIGNAL(activated()), this, SLOT(AddNewFromFile()));
        QObject::connect(ui.action_dll_delete, SIGNAL(activated()), this, SLOT(DeleteOneDLLfromDB()));
        QObject::connect(ui.action_dll_subscribe, SIGNAL(activated()), this, SLOT(SubscribeOneDLL()));

        ContextMenu=new QMenu(this);

        ContextMenu->addAction(ui.action_add_dll);
        ContextMenu->addAction(ui.action_dll_delete);
        ContextMenu->addAction(ui.action_dll_subscribe);
        ContextMenu->addSeparator();
        ContextMenu->addAction(ui.action_exit);

        ui.tableWidget_reged_dll->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui.tableWidget_reged_dll, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCntxMenu(QPoint)));

        ReadFormWidgetsAppearance();
        FillDLLGrid();
}

mw_DLL_handler::~mw_DLL_handler()
{

}


void mw_DLL_handler::slotCntxMenu(const QPoint &point)      //Слот для реализации контекстного меню в Hypotesis
{
    ContextMenu->popup(ui.tableWidget_reged_dll->mapToGlobal(point));
}

void mw_DLL_handler::FillDLLGrid()
{
    //Процедура заполняет таблицу зарегистрированными в программе библиотеками, устанавливая текущим элементом элемент, переданный в параметре CurItem, и колонку переданную в параметре Num

    IBPP::Transaction TmpTR=IBPP::TransactionFactory(this->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
    IBPP::Statement TmpST=IBPP::StatementFactory(this->DB, TmpTR);

    TmpTR->Start();
    TmpST->Prepare("select count(GET_REGED_DLLS.DLL_NAME) Number_dll from GET_REGED_DLLS;");
    TmpST->Execute();
    long NumberDLLs;    //Определяем количество библиотек прописанных в базе
    if(TmpST->Fetch()) TmpST->Get("Number_dll",(int32_t*)&NumberDLLs);

    ui.tableWidget_reged_dll->setSortingEnabled(false);
    ui.tableWidget_reged_dll->clearContents(); //Очищаем внутреннее содержимое таблицы

    ui.tableWidget_reged_dll->setColumnCount(2);
    ui.tableWidget_reged_dll->setRowCount(NumberDLLs);

    QTableWidgetItem *__vhi1 = new QTableWidgetItem();
    __vhi1->setText(tr("Имя библиотеки"));
    ui.tableWidget_reged_dll->setHorizontalHeaderItem(0, __vhi1);
    QTableWidgetItem *__vhi2 = new QTableWidgetItem();
    __vhi2->setText(tr("MD5 сумма"));
    ui.tableWidget_reged_dll->setHorizontalHeaderItem(1, __vhi2);

    TmpTR->Start();
    TmpST->Prepare("select * from GET_REGED_DLLS;");
    TmpST->Execute();

    int i=0;
    while (TmpST->Fetch())
    {
        //Считываем значения текущей записи для библиотеки
        std::string DLL_Name;
        std::string MD5_SUMM;
        TmpST->Get("DLL_NAME",DLL_Name);
        TmpST->Get("MD5_SUMM",MD5_SUMM);

        QTableWidgetItem *__hhi = new QTableWidgetItem();
        __hhi->setText(QString::number(i+1));   //В качестве подписи выступает последовательный номер записи в базе
        ui.tableWidget_reged_dll->setVerticalHeaderItem(i, __hhi);

        QTableWidgetItem *__ti1 = new QTableWidgetItem();
        __ti1->setText(QString::fromStdString(DLL_Name));
        ui.tableWidget_reged_dll->setItem(i, 0, __ti1);
        QTableWidgetItem *__ti2 = new QTableWidgetItem();
        __ti2->setText(QString::fromStdString(MD5_SUMM));
        ui.tableWidget_reged_dll->setItem(i, 1, __ti2);

        if(is_file_process)
        {
            if(QFile::exists(DLL_Folder_name+QString::fromStdString(DLL_Name))) //Файл найден на диске
            {
                ui.action_dll_subscribe->setEnabled(true);
                if(DTL->CalcFileMD5(DLL_Folder_name+QString::fromStdString(DLL_Name))==DTL->GetSavedMD5(QString::fromStdString(DLL_Name))) //Проверяем совпадение MD5 суммы
                {
                    __ti2->setTextColor(Qt::black);
                    __ti2->setToolTip(tr("Сумма MD5 проверена"));
                }
                else
                {
                    __ti2->setTextColor(Qt::red);
                    __ti2->setToolTip(tr("Сумма MD5 файла не совпадает с суммой прописанной в БД"));
                }            
            }
            else
            {
                __ti2->setToolTip(QString::fromStdString(DLL_Name)+" не найден");
            }

        }
        else
        {
            ui.action_dll_subscribe->setDisabled(true);
            __ti2->setToolTip(tr("Операции с файлами заблокированы"));
        }

        i++;
    }
    ui.tableWidget_reged_dll->resizeColumnsToContents();
    ui.tableWidget_reged_dll->setSortingEnabled(true);
    //Устанавливаем текущим элементом первый элемент и первую видимую колонку
    //ui.tableWidget->setCurrentCell(0,Num);
}


void mw_DLL_handler::AddNewFromFile()
{
    QString FullFileName = QFileDialog::getOpenFileName(this, tr("Укажите файл для внесения в базу данных"), DLL_Folder_name, tr("*.*"));
    if(FullFileName!=NULL)  //Выбор файла осуществлен
    {
        QStringList FileL=FullFileName.split(QDir::separator());
        QString fileName = FileL.at(FileL.count()-1);   //Имя файла последнее в списке

        QString md5 = DTL->CalcFileMD5(FullFileName);

        IBPP::Transaction TmpTR=IBPP::TransactionFactory(this->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement TmpST=IBPP::StatementFactory(this->DB, TmpTR);
        TmpTR->Start();
        TmpST->Prepare("EXECUTE PROCEDURE SET_MD5DLL(?,?);");
        //1- fileName 2 -md5hash
        TmpST->Set(1, fileName.toStdString());
        TmpST->Set(2, md5.toStdString());
        TmpST->Execute();
        TmpTR->Commit();
        FillDLLGrid();

    }
}

void mw_DLL_handler::DeleteOneDLLfromDB()
{
    QString DLL_Item_Name=ui.tableWidget_reged_dll->item(ui.tableWidget_reged_dll->currentRow(),0)->text();

    if (QMessageBox::warning(this, tr("Внимание, данная операция необратима"),
                                    tr("Подтвердите удаление записи о библиотеке ")+DLL_Item_Name,
                                    QMessageBox::Ok | QMessageBox::Cancel,
                                    QMessageBox::Cancel)==QMessageBox::Ok)
    {
        IBPP::Transaction TmpTR=IBPP::TransactionFactory(this->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
        IBPP::Statement TmpST=IBPP::StatementFactory(this->DB, TmpTR);
        TmpTR->Start();
        TmpST->Prepare("EXECUTE PROCEDURE DEL_REGED_DLL(?);");
        TmpST->Set(1, DLL_Item_Name.toStdString());
        TmpST->Execute();
        TmpTR->Commit();
        FillDLLGrid();
    }
}

void mw_DLL_handler::SubscribeOneDLL()
{
    QString DLL_Item_Name=ui.tableWidget_reged_dll->item(ui.tableWidget_reged_dll->currentRow(),0)->text();

    QString md5 = DTL->CalcFileMD5(DLL_Folder_name+DLL_Item_Name);

    IBPP::Transaction TmpTR=IBPP::TransactionFactory(this->DB,IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
    IBPP::Statement TmpST=IBPP::StatementFactory(this->DB, TmpTR);
    TmpTR->Start();
    TmpST->Prepare("EXECUTE PROCEDURE SET_MD5DLL(?,?);");
    //1- fileName 2 -md5hash
    TmpST->Set(1, DLL_Item_Name.toStdString());
    TmpST->Set(2, md5.toStdString());
    TmpST->Execute();
    TmpTR->Commit();
    FillDLLGrid();

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
