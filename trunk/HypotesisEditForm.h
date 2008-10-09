#ifndef HypotesisEditForm_H
#define HypotesisEditForm_H

#include "ui_HypotesisEditForm.h"
#include <QtGui>
#include "EAOneFrame.h"


class HypotesisEditForm  :public QDialog, public Ui_HypotesisEditForm
{
    Q_OBJECT

public:
	HypotesisEditForm(QWidget * parent = 0);
    long id_hypotesys;

};


#endif
