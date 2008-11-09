#ifndef HypotesisEditForm_H
#define HypotesisEditForm_H

#include "ui_HypotesisEditForm.h"
#include <QtGui>
#include "EAOneFrame.h"
#include "EssentialClass.h"
#include "ibpp.h"

namespace platon
{
class HypotesisEditForm  :public QMainWindow, public Ui_HypotesisWindow
{
    Q_OBJECT

public:
	HypotesisEditForm(QWidget * parent, IBPP::Database InDB, long ID_Hypotesys);
    long id_hypotesys;
protected:
    IBPP::Database DB;
    Eidos* LocalEidos;
    Hypotesis*LocalHypotesis;

private slots:
	void ExitWithSave();
	void ExitByCancel();

};
}

#endif
