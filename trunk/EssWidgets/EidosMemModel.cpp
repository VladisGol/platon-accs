/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the example classes of the Qt Toolkit.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#include "EidosMemModel.h"
namespace platon
{

EidosMemModel::EidosMemModel(IBPP::Database InDB, QString Spec, QObject *parent)
    : QAbstractItemModel(parent)
{
	DB=InDB;
	Species=Spec;

	QList<QVariant> rootData;
    rootData << tr("Наименование") << tr("ID");
    rootItem = new TreeItem(rootData);
    setupModelData(rootItem);
}

EidosMemModel::~EidosMemModel()
{
    delete rootItem;
}

int EidosMemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant EidosMemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags EidosMemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant EidosMemModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex EidosMemModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex EidosMemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
int EidosMemModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void EidosMemModel::setupModelData(TreeItem *TopItem)
{
	platon::iterEidos* MyEidosIter= new  platon::iterEidos(this->DB,Species.toStdString());

   	while(MyEidosIter->Next())
    {
   		//Готовим данные для одной записи
   		QList<QVariant> columnData;
        columnData << tr( MyEidosIter->GetTitle().c_str());
        columnData << QVariant::fromValue(MyEidosIter->GetID());

    	TreeItem* BranchItem= TopItem->findByValue(1,QVariant::fromValue(MyEidosIter->GetParentID()));
    	if(BranchItem==NULL)	//Не найден, добавление в корень представления
    		TopItem->appendChild(new TreeItem(columnData,TopItem));
    	else
    		BranchItem->appendChild(new TreeItem(columnData,BranchItem));
    }
	delete MyEidosIter;
}


TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
    parentItem = parent;
    itemData = data;
}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    return childItems.value(row);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::columnCount() const
{
    return itemData.count();
}

QVariant TreeItem::data(int column) const
{
    return itemData.value(column);
}

TreeItem *TreeItem::parent()
{
    return parentItem;
}

int TreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

TreeItem *TreeItem::findByValue(int col, QVariant SearchedValue)
{
	//Процедура ищет значение и возвращает ссылку найденного объекта
	TreeItem *ForReturn=NULL;
	//Пробегаем по всем деткам текущего объекта
	for(int i=0;i<this->childCount();i++)
	{
		if(this->child(i)->data(col)==SearchedValue)	//Если сама детка - искомое значение
		{
			ForReturn=this->child(i);
			break;
		}
		else
		{
			if(this->child(i)->childCount()>0)			//Если детка сама имеет деток
			{
				ForReturn=this->child(i)->findByValue(col, SearchedValue);	//Рекурсивный вызов
				if(ForReturn!=NULL) break;
			}
		}
	}
    return ForReturn;
}
}
