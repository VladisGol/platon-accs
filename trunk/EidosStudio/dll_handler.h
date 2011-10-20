#ifndef DLL_handler_H
#define DLL_handler_H

#include <QtGui/QMainWindow>
#include "ui_dll_handler.h"

#include "EssentialClass.h"
#include "ibpp.h"
#include "dbetc.h"
#include "DataClass.h"
#include "qeidostreewidget.h"


namespace platon
{
class mw_DLL_handler : public QMainWindow
{
    Q_OBJECT

public:
        mw_DLL_handler(QWidget *parent = 0);
        ~mw_DLL_handler();
        IBPP::Database DB;
        Ui::mw_DLL_handler ui;
        DataClass* DTL;
        bool is_file_process;  //Требуется обработка локальной папки с библиотеками
        QString DLL_Folder_name;

public slots:
        void Exit();
        void FillDLLGrid();
protected:
        void ReadFormWidgetsAppearance();
        void WriteFormWidgetsAppearance();

private:
};
}
#endif // DLL_handler_H
