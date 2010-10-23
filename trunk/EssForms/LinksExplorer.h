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
#include "HypotesisEditForm.h"
#include "PragmaEditForm.h"



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
	void Exit();
	void SetHGridView(QTreeWidgetItem*,int);
	void SetPGridView(QTreeWidgetItem*,int);
	void ClearCorespActions();
	void SetCorespActions();
	void OpenHypotesis();
	void OpenPragma();

private slots:
        void slotHypCntxMenu(const QPoint &point);      //Слот для реализации контекстного меню в Hypotesis
        void slotPragmaCntxMenu(const QPoint &point);   //Слот для реализации контекстного меню в Pragma
        void slotCopySelectedFromView();                //Слот для реализации копирования выделения

protected:
    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();
    bool eventFilter(QObject *obj, QEvent *ev);
    void SetViewID();
    bool IsViewID;
    QMenu* ContextMenuHyp;
    QMenu* ContextMenuPragma;


};
}

#endif
