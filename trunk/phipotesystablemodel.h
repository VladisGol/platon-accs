//
// C++ Interface: phipotesystablemodel
//
// Description: 
//
//
// Author: Vladislav V Golovyrin <slava@linux-Golovyrin>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PLATONPHIPOTESYSTABLEMODEL_H
#define PLATONPHIPOTESYSTABLEMODEL_H

#include <QtGui>
#include <QAbstractTableModel>

namespace platon {

static int i;
class pHipotesysTableModel : public  QAbstractTableModel
{
    Q_OBJECT

public:
    pHipotesysTableModel(QObject *parent);
    ~pHipotesysTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex & index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
};
}

#endif
