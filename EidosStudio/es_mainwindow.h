#ifndef ES_MAINWINDOW_H
#define ES_MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_es_mainwindow.h"
#include "EssentialClass.h"
#include "ibpp.h"
#include "dbetc.h"
#include "DataClass.h"

namespace platon
{
class es_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    es_mainwindow(QWidget *parent = 0);
    ~es_mainwindow();
    IBPP::Database DB;

public slots:
	void Exit();


protected:
    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();

private:
    Ui::es_mainwindowClass ui;

};
}
#endif // ES_MAINWINDOW_H
