#ifndef mainWin_H
#define mainWin_H

#include "ui_mainwindow.h"


class mainWin : public QMainWindow, public Ui::MainWindow

{
    Q_OBJECT

public:
    mainWin(QWidget * parent = 0);

private slots:
	void Go();    // ���� ��� ������ Go
	void GoGrid();// ���� ��� ������ Grid

};


#endif
