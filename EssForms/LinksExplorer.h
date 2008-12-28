#ifndef LinksExplorer_H
#define LinksExplorer_H

#include "ui_LinksExplorer.h"
#include <QtGui>
#include "EssentialClass.h"
#include "ibpp.h"


namespace platon
{
class LinksExplorer  :public QMainWindow, public Ui_LinksExplorer
{
    Q_OBJECT

public:
	LinksExplorer(QWidget * parent);

    IBPP::Database DB;
    //Eidos* LocalEidos;
    //Hypotesis*LocalHypotesis;
    //void FormFillFrames();

public slots:
	void ExitWithSave();
	void ExitByCancel();
};
}

#endif
