#include "LnkdEidosMemModel.h"
namespace platon
{

LnkdEidosMemModel::LnkdEidosMemModel(IBPP::Database InDB, QString Spec, pIterator* Iter, QObject *parent)
    :EidosMemModel(InDB, Spec,parent)
{
    setupMapIDs(Iter);
}

void LnkdEidosMemModel::setupMapIDs(pIterator* Iter)
{
	Iter->First();
	FoundedCountOnBranchID.clear();
	while(Iter->Fetched())
	{
		FoundedCountOnBranchID.insert(Iter->GetID(),QVariant::fromValue(Iter->GetRowNum()));
		Iter->Next();
	}

}

QVariant LnkdEidosMemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (role == Qt::DisplayRole) return item->data(index.column());
    else if(role == Qt::FontRole)
    {
		long ID=data(index.sibling(index.row(),1), Qt::DisplayRole).toInt();
		if(FoundedCountOnBranchID.count(ID)>0)
			return qvariant_cast<QFont>(this->SelectedFont);
		else
			return qvariant_cast<QFont>(this->OrdinarFont);
    }
    else return QVariant();
}
void LnkdEidosMemModel::SetSelectedFont(const QFont& in)
{
	this->SelectedFont=in;
}
void LnkdEidosMemModel::SetOrdinarFont(const QFont& in)
{
	this->OrdinarFont=in;
}

}
