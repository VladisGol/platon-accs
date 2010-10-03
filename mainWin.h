#ifndef mainWin_H
#define mainWin_H

#include "ui_mainwindow.h"
#include <QtGui>
#include <QModelIndexList>
#include "EssentialClass.h"
#include "ibpp.h"
#include "HypotesisMemModel.h"
#include "pragmamodel.h"
#include "HypotesisMemModel.h"
#include "PragmaMemModel.h"
#include "HypotesisEditForm.h"
#include "PragmaEditForm.h"
#include "EAOneFrame.h"
#include "LinksExplorer.h"
#include "dbetc.h"
#include "DataClass.h"
#include "aboutprogram.h"
#include "es_mainwindow.h"




class mainWin : public QMainWindow, public Ui::MainWindow

{
    Q_OBJECT

public:
    mainWin(QWidget * parent = 0);
	platon::DataClass* MyDCl;
	void RefreshHView();
	void RefreshPView();

private slots:
	void SetPragmaView(const QModelIndex & index);
	void SetHypotesysView(QTreeWidgetItem*,int);
	void SetEidosView(int Row);
	void EditItem();// Слот для редактирования текущего объекта
	void AddItem();// Слот для добавления объекта
	void DeleteItem();// Слот для удаления объекта
	void RefreshViews();
	void BaseTimeShift();//Слот для установки в базе данных времени работы в программе
	void Showlinks();// Слот для вызова формы отображения ссылок на текущую запись
	void CloseForm();
	void AddFilter();	//Слот для задания фитра
	void RemoveFilter();	//Слот для снятия условий фильтра
	void DisableAllActions();	//Отключение всех действий на форме
	void ViewID_Activated();//Управление отображением идентификаторов объектов
	void AboutShow();//Вывод диалогового окна "О программе"
	void ESShow();	//Запуск Эйдос студии
        void slotEidosCntxMenu(const QPoint &point);    //Слот для реализации контекстного меню в Eidos
        void slotHypCntxMenu(const QPoint &point);      //Слот для реализации контекстного меню в Hypotesis
        void slotPragmaCntxMenu(const QPoint &point);   //Слот для реализации контекстного меню в Pragma

private:
	platon::Eidos* LocalEidos;
	platon::Hypotesis* LocalHypotesis;
	QTimer* DTBaseShifter;
	QIcon icon_filter;
	AboutProgram* AboutDlg;
        QMenu* ContextMenuEidos;
        QMenu* ContextMenuHyp;
        QMenu* ContextMenuPragma;
protected:
	bool eventFilter(QObject *obj, QEvent *ev);
	int CurrentObjectLevel;
	enum ObjectLevel {Level_Hypotesis=1,Level_Pragma=2};
    QSortFilterProxyModel* SFProxyModelH;
    QSortFilterProxyModel* SFProxyModelP;
    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();


};


#endif
