#ifndef abstracthipeditform_H
#define abstracthipeditform_H

#include "ui_HypotesisEditForm.h"
#include <QtGui>
#include "EAOneFrame.h"
#include "EssentialClass.h"
#include "ibpp.h"


namespace platon
{
class AbstarctHipEditForm  :public QMainWindow, public Ui_HypotesisWindow
{
    Q_OBJECT

public:
	AbstarctHipEditForm(QWidget * parent);

    IBPP::Database DB;
    Eidos* LocalEidos;
    Hypotesis*LocalHypotesis;
    void FormFillFrames();

public slots:
	void ExitWithSave();
	void ExitByCancel();
};
}

#endif
