#ifndef ES_MAINWINDOW_H
#define ES_MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_es_mainwindow.h"
#include "EssentialClass.h"
#include "ibpp.h"
#include "dbetc.h"
#include "DataClass.h"
#include "qeidostreewidget.h"


namespace platon
{
class es_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    es_mainwindow(QWidget *parent = 0);
    ~es_mainwindow();
    IBPP::Database DB;
    Ui::es_mainwindowClass ui;

public slots:
	void Exit();
	void FillEAGrid(QTreeWidgetItem*,int);//Заполнение списка экстраатрибутов в грид
	void EAChoosed(QTableWidgetItem*);		//Отображение одного элемента из таблицы
	void comboTypeChanged(int);			//Изменение вида детализации атрибута при смене типа в комбо боксе


protected:
    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();

private:
	bool IsViewID;
    void ViewID_Activated();
	Eidos* LocalEidos;
	ExtraAttribute* CurrentEA;
	QString GetEATypeName(int typeID);
	QIcon icon_locked;
	QIcon icon_editable;
	QIcon icon_occouped;
	int numColsInTableEA;

};
}
#endif // ES_MAINWINDOW_H
