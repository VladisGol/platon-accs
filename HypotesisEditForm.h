#ifndef HypotesisEditForm_H
#define HypotesisEditForm_H

#include "ui_HypotesisEditForm.h"
#include <QtGui>


class HypEditDialog  :public QDialog, public Ui_Dialog
{
    Q_OBJECT

public:
    HypEditDialog(QWidget * parent = 0): QDialog(parent){};


};


#endif
