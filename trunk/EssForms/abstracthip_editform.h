#ifndef abstracthipeditform_H
#define abstracthipeditform_H


#include "ui_HypotesisEditForm.h"
#include <QtGui>
#include "EAOneFrame.h"
#include "EssentialClass.h"
#include "ibpp.h"
#include "dbetc.h"
#include "DataClass.h"
#include "Multilinks.h"


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
    DataClass* MyDCl;

public slots:
	void ExitWithSave();
	void ExitByCancel();
	void DoAddAction();
	void DoWriteOffRes();
	void DoOpenType();
	void DoMultilink();

protected:

    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();
    QString dbEtcBranchName;
    void FormActionsTune();

};

class MultilinkAction :public QAction
{
	Q_OBJECT
	public:
	AssociatedExtraAttribute* AEAttrib;
	MultilinkAction(const QString &text, QObject* parent):QAction(text, parent){;};
};

}

#endif
