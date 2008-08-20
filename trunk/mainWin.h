#ifndef mainWin_H
#define mainWin_H

#include "ui_mainwindow.h"


class mainWin : public QMainWindow, public Ui::MainWindow

{
    Q_OBJECT

public:
    mainWin(QWidget * parent = 0);

private slots:
	void Go();    // Слот для кнопки Go
	void GoGrid();// Слот для кнопки Grid

};


#endif
