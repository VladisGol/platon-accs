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
#include "EidosMemModel.h"
#include <iostream>

namespace platon
{
class LinksExplorer  :public QMainWindow, public Ui_LinksExplorer
{
    Q_OBJECT

public:
	LinksExplorer(QWidget * parent, IBPP::Database InDB, long ID_in, QString InSpecies);
	~LinksExplorer();

    IBPP::Database DB;
    Eidos* LocalEidosH;
    Eidos* LocalEidosP;

    iterLNKS_Hyp* IHyp;
    iterLNKS_Pragma* IPragma;
    iterLNKS_HEidos* IEidosH;
    iterLNKS_PEidos* IEidosP;
    QSortFilterProxyModel* SFProxyModelH;
    QSortFilterProxyModel* SFProxyModelP;
    long IDFor;


    void PaintingEidos(QTreeView *TreeView,pIterator * iter);
    QTreeWidgetItem * FindEidosByID(QTreeWidget* EidosTreeWidget, long ID);
    void UnBoldTreeView(QTreeView *TreeView,const QModelIndex&);

public slots:

	void SetHGridView(const QModelIndex&);
	void SetPGridView(const QModelIndex&);
};
}

#endif
