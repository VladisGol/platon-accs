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


class mainWin : public QMainWindow, public Ui::MainWindow

{
    Q_OBJECT

public:
    mainWin(QWidget * parent = 0);

private slots:
	void Go();    // ���� ��� ������ Go
	void GoGrid();// ���� ��� ������ Grid
private:
	platon::Eidos* LocalEidos;
	platon::Hypotesis* LocalHypotesis;
	void SetHypotesysView();
	void SetPragmaView();


};


#endif
