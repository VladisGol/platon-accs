#ifndef LinksExplorer_H
#define LinksExplorer_H

#include "ui_LinksExplorer.h"
#include <QtGui>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include "LnkdHypMemModel.h"
#include "LnkdPragmaMemModel.h"
#include "EssentialClass.h"
#include "ibpp.h"
#include "dbetc.h"
#include "DataClass.h"


namespace platon
{
class LinksExplorer  :public QMainWindow, public Ui_LinksExplorer
{
    Q_OBJECT

public:
	LinksExplorer(QWidget * parent, long ID_in, QString InSpecies);
	~LinksExplorer();

    IBPP::Database DB;
    Eidos* LocalEidosH;
    Eidos* LocalEidosP;
    Hypotesis*LocalHypotesis;

    iterLNKS_Hyp* IHyp;
    iterLNKS_Pragma* IPragma;
    iterLNKS_HEidos* IEidosH;
    iterLNKS_PEidos* IEidosP;
    long IDFor;


    void PaintingEidos(QTreeWidget* EidosTreeWidget,pIterator*);
    QTreeWidgetItem * FindEidosByID(QTreeWidget* EidosTreeWidget, long ID);

public slots:
	void ExitWithSave();
	void ExitByCancel();
	void SetHGridView(QTreeWidgetItem*,int);
	void SetPGridView(QTreeWidgetItem*,int);

protected:
    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();
};
}

#endif
