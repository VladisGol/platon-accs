#ifndef ES_MAINWINDOW_H
#define ES_MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_es_mainwindow.h"
#include "EssentialClass.h"
#include "ibpp.h"
#include "dbetc.h"
#include "DataClass.h"
#include "qeidostreewidget.h"
#include "dll_handler.h"


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
        void RefreshEAonForm();				//Процедура проводит обновление элементов управления формы для текущего экстраатрибута

public slots:
	void Exit();
        void Start_DLL_Manager();
        void FillEAGrid(QTreeWidgetItem*,int);          //Заполнение списка экстраатрибутов в грид
	void EAChoosed(QTableWidgetItem*,QTableWidgetItem*);		//Отображение одного элемента из таблицы
	void comboTypeChanged(int);			//Изменение вида детализации атрибута при смене типа в комбо боксе
        void SetAltCaption();				//Слот заполнения псевдонима экстраатрибута
        void ChangeCheckBoxAlterCaption();              //Слот ручного изменения состояния чекбокса псевдонима
	void UserTryToEditEA();				//Слот для отслеживания корректировки экстраатрибута
        void SaveCurEA();                               //Слот для сохранения текущего экстраатрибута
        void AddOneEA();                                //Слот для создания нового экстраатрибута
        //Слоты для работы с потомками Eidos-а
        void RenameEidos();                             //Слот для переименования текущего Eidos-а
        void AddChildEidos();                           //Добавить Потомка к Eidos-у
        void DeleteEidos();                             //Удалить текущий Eidos

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
	bool IsEAEditable;
	bool CurEAChanged;
};
}
#endif // ES_MAINWINDOW_H
