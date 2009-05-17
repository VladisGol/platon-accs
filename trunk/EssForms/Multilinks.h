#ifndef Multilinks_H
#define Multilinks_H

#include "ui_Multilinks.h"
#include <QtGui>
#include "EssentialClass.h"
#include "ibpp.h"
#include "dbetc.h"
#include "DataClass.h"

namespace platon
{
class Multilinks  :public QMainWindow, public Ui_Multilinks
{
    Q_OBJECT

public:
	Multilinks(QWidget * parent, AssociatedExtraAttribute* InAEA);
	~Multilinks();

    IBPP::Database DB;
    AssociatedExtraAttribute* AEAttrib;
    iterMultilink* MyIter;

public slots:
	void Exit();

protected:
    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();
    long CurrentID;
};
}

#endif
