#ifndef QEidosTreeWidget_H
#define QEidosTreeWidget_H

#include <QtGui>
#include "EssentialClass.h"
#include "ibpp.h"
#include "dbetc.h"
#include "DataClass.h"

class QEidosTreeWidget: public QTreeWidget
{
	private:
        platon::DataClass* DTL;
		IBPP::Database MyDB;
		platon::iterEidos* MyEidosIter;
		QString Species;
	public:
		QEidosTreeWidget (QWidget* parent);
		~QEidosTreeWidget();
		void AttachToDB(IBPP::Database InDB);
		void SetSpecies(const QString InSpecies);
		void SaveAppearance();
		long GetEidosID();
		bool findNMakeCurrent(long ID_searchfor);
};
#endif
