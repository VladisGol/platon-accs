#ifndef firstFormapp_H
#define firstFormapp_H
 
#include "ui_MyFirstForm.h"
 
 
class firstformAPP : public QWidget, private Ui::MyFirstForm

{
    Q_OBJECT
 
public:
    firstformAPP(QWidget * parent = 0);

private slots:
	void Go();    // ���� ��� ������ Go
	void GoGrid();// ���� ��� ������ Grid
 
};
 
 
#endif
