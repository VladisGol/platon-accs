#ifndef mw_DLL_handler_H
#define mw_DLL_handler_H

#include <QtGui/QMainWindow>
//#include "ui_mw_DLL_handler.h"
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
        //Ui::mw_DLL_handler ui;

public slots:
protected:
        void ReadFormWidgetsAppearance();
        void WriteFormWidgetsAppearance();

private:
};
}
#endif // mw_DLL_handler_H
