#include <QtGui>
#include "EssentialClass.h"
#include "ibpp.h"
#include "dbetc.h"

class QEidosTreeWidget: public QTreeWidget
{
		private:
		IBPP::Database MyDB;
		platon::iterEidos* MyEidosIter;
		QString Species;
		platon::DbEtc *MyETC;
		public:
		QEidosTreeWidget (QWidget* parent);
		~QEidosTreeWidget();
		void AttachToDB(IBPP::Database InDB);
		void SetSpecies(const QString InSpecies);
		void SaveAppearance();
};
