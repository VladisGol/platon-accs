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
	void DoMultilink(int i);
	void DoLinks();

protected:

    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();
    QString dbEtcBranchName;
    void FormActionsTune();
    QSignalMapper* signalMapper;

};
}

#endif
