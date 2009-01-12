#ifndef LNKDEIDOSMEMMODEL_H
#define LNKDEIDOSMEMMODEL_H

#include "EidosMemModel.h"
#include "EssentialClass.h"
#include <QMap>

namespace platon
{

class LnkdEidosMemModel : public EidosMemModel
{


public:
	LnkdEidosMemModel(IBPP::Database InDB, QString Spec,pIterator* Iter, QObject *parent);
    QVariant data(const QModelIndex &index, int role) const;
    void SetSelectedFont(const QFont& in);
    void SetOrdinarFont(const QFont& in);

private:
	QFont SelectedFont;
	QFont OrdinarFont;
	QMap<long,QVariant> FoundedCountOnBranchID;
	void setupMapIDs(pIterator* Iter);

};
}
#endif
