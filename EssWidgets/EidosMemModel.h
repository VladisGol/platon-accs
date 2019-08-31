#ifndef EIDOSMEMMODEL_H
#define EIDOSMEMMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QList>
#include <QFont>
#include "EssentialClass.h"
#include "ibpp.h"
#include <iostream>

namespace platon
{
class TreeItem	//Приношу благодарность фирме Nokia за использованный из примеров код
{
public:
    TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parent();
    TreeItem *findByValue(int col, QVariant SearchedValue);

private:
    QList<TreeItem*> childItems;
    QList<QVariant> itemData;
    TreeItem *parentItem;
};

class EidosMemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
	EidosMemModel(IBPP::Database InDB,QString Spec, QObject *parent);
    ~EidosMemModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

protected:
    void setupModelData(TreeItem *parent);
    IBPP::Database DB;
    TreeItem *rootItem;
    QString Species;
	QList<QVariant> GetRootData();
	QList<QVariant> GetColumnsData(QString Title, int ID);
    int AdditionalColumnsNumber;
    EidosMemModel(QObject *parent):QAbstractItemModel(parent){;};
};
}
#endif
