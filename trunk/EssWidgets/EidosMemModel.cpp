#include "EidosMemModel.h"
namespace platon
{

EidosMemModel::EidosMemModel(IBPP::Database InDB, QString Spec, QObject *parent)
    : QAbstractItemModel(parent)
{
	DB=InDB;
	Species=Spec;

    rootItem = new TreeItem(GetRootData());
    AdditionalColumnsNumber=0;	//� ���� ������ �������������� �������� �������� ������
    setupModelData(rootItem);
}

EidosMemModel::~EidosMemModel()
{
    delete rootItem;
}

QList<QVariant> EidosMemModel::GetRootData()
{
	//��������� ����������� ������� ����� ��� ���������.
	//������ ������� �������� ��� ��������� �������� � ����������� ������� �������� ����� �������� ������
	QList<QVariant> ForReturn;
	ForReturn<<QObject::tr("������������")<<QObject::tr("ID");
	return ForReturn;
}

int EidosMemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount()+AdditionalColumnsNumber;
    else
        return rootItem->columnCount();
}

QVariant EidosMemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (role == Qt::DisplayRole)
    {
    	return item->data(index.column());
    }
/*    else if(role == Qt::FontRole)
        //return qvariant_cast<QFont>(item->Myfont);
    	return qvariant_cast<QFont>(item->Myfont);*/
    else return QVariant();

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

QList<QVariant> EidosMemModel::GetColumnsData(QString Title, long ID)
{
	QList<QVariant> columnData;
	columnData << Title;
	columnData << QVariant::fromValue(ID);
	return columnData;
}


void EidosMemModel::setupModelData(TreeItem *TopItem)
{
	platon::iterEidos* MyEidosIter= new  platon::iterEidos(this->DB,Species.toStdString());

   	while(MyEidosIter->Next())
    {
   		//������� ������ ��� ����� ������
   		QList<QVariant> columnData =GetColumnsData(tr( MyEidosIter->GetTitle().c_str()), MyEidosIter->GetID());

    	TreeItem* BranchItem= TopItem->findByValue(1,QVariant::fromValue(MyEidosIter->GetParentID()));
    	if(BranchItem==NULL)	//�� ������, ���������� � ������ �������������
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
	//��������� ���� �������� � ���������� ������ ���������� �������
	TreeItem *ForReturn=NULL;
	//��������� �� ���� ������ �������� �������
	for(int i=0;i<this->childCount();i++)
	{
		if(this->child(i)->data(col)==SearchedValue)	//���� ���� ����� - ������� ��������
		{
			ForReturn=this->child(i);
			break;
		}
		else
		{
			if(this->child(i)->childCount()>0)			//���� ����� ���� ����� �����
			{
				ForReturn=this->child(i)->findByValue(col, SearchedValue);	//����������� �����
				if(ForReturn!=NULL) break;
			}
		}
	}
    return ForReturn;
}
}
