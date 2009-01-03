#ifndef LinksExplorer_H
#define LinksExplorer_H

#include "ui_LinksExplorer.h"
#include <QtGui>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include "LnkdHypMemModel.h"
#include "EssentialClass.h"
#include "ibpp.h"


namespace platon
{
class LinksExplorer  :public QMainWindow, public Ui_LinksExplorer
{
    Q_OBJECT

public:
	LinksExplorer(QWidget * parent, IBPP::Database InDB, long ID_Hypotesys);
	~LinksExplorer();

    IBPP::Database DB;
    Eidos* LocalEidos;
    Hypotesis*LocalHypotesis;
    LinkedHypotesys* LH;

    void PaintingEidos();
    QTreeWidgetItem * FindEidosByID(long ID);

public slots:
	void ExitWithSave();
	void ExitByCancel();
	void SetHypotesysView(QTreeWidgetItem*,int);

};
}

#endif
