#ifndef mainWin_H
#define mainWin_H

#include "ui_mainwindow.h"
#include <QtGui>
#include <QModelIndexList>
#include "EssentialClass.h"
#include "ibpp.h"
#include "HypotesisMemModel.h"
//#include "phipotesystablemodel.h"
//#include "hypotesismodel.h"
#include "pragmamodel.h"
#include "HypotesisMemModel.h"
#include "PragmaMemModel.h"
#include "HypotesisEditForm.h"
#include "PragmaEditForm.h"
#include "EAOneFrame.h"
#include "LinksExplorer.h"


class mainWin : public QMainWindow, public Ui::MainWindow

{
    Q_OBJECT

public:
    mainWin(QWidget * parent = 0);
	QDateTime ProgramDateTime;

private slots:
	void SetPragmaView(const QModelIndex & index);
	void SetHypotesysView(QTreeWidgetItem*,int);
	void SetEidosView(int Row);
	void EditItem();// ���� ��� �������������� �������� �������
	void AddItem();// ���� ��� ���������� �������
	void DeleteItem();// ���� ��� �������� �������
	void RefreshViews();
	void BaseTimeShift();//���� ��� ��������� � ���� ������ ������� ������ � ���������
	void Showlinks();// ���� ��� ������ ����� ����������� ������ �� ������� ������
private:
	platon::Eidos* LocalEidos;
	platon::Hypotesis* LocalHypotesis;
	IBPP::Database MyDB;
	QTimer* DTBaseShifter;
protected:
	bool eventFilter(QObject *obj, QEvent *ev);
	int CurrentObjectLevel;
	enum ObjectLevel {Level_Hypotesis=1,Level_Pragma=2};

};


#endif
