#ifndef Multilinks_H
#define Multilinks_H

#include "ui_Multilinks.h"
#include <QtGui>
#include "EssentialClass.h"
#include "ibpp.h"
#include "dbetc.h"
#include "DataClass.h"
#include "DialogchoiceEidos.h"
#include "DialogchoiceHypotesis.h"
#include "DialogchoicePragma.h"


namespace platon
{
class Multilinks  :public QMainWindow, public Ui_Multilinks
{
    Q_OBJECT

public:
	Multilinks(QWidget * parent, AssociatedExtraAttribute* InAEA);
	~Multilinks();
    IBPP::Database DB;
    bool RatedLinks;
    platon::LNK_Value LNKChoice(platon::LNK_Value InLNK);

public slots:
	void Exit();
	void AddLink();
	void EditLink();
	void DeleteLink();


protected:
    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();

    AssociatedExtraAttribute* AEAttrib;
    iterMultilink* MyIter;
    void FillWidgets(int prow,int pcolumn);
    QMap<int,LNK_Value>* LNKMap;
    void SetViewID(bool IsView);
    DataClass* DTL;
};
}

#endif
