//
// C++ Implementation: phipotesystablemodel
//
// Description: 
//
//
// Author: Vladislav V Golovyrin <slava@linux-Golovyrin>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "phipotesystablemodel.h"

namespace platon {

pHipotesysTableModel::pHipotesysTableModel(QObject *parent)
 :  QAbstractTableModel(parent)
{
i=0;
}

pHipotesysTableModel::~pHipotesysTableModel()
{
}
int pHipotesysTableModel::rowCount(const QModelIndex &parent) const
{
	return 10;
}

int pHipotesysTableModel::columnCount(const QModelIndex& index ) const
 {
     	return 10;
 }

QVariant pHipotesysTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole)
        	return QVariant();
	i++;
	return QString::number(index.row())+"x"+QString::number(index.column())+":"+QString::number(i);

}
QVariant pHipotesysTableModel::headerData(int section, Qt::Orientation orientation,int role) const
{
	return section;
}



}
