/*
 * AbstractMemHypModel.cpp
 *
 *  Created on: 07.08.2008
 *      Author: slava
 */

#include "AbstractMemHypModel.h"

namespace platon
{
AbstractMemHypModel::AbstractMemHypModel(QObject * parent)
			:QAbstractTableModel(parent)
{
	;
}
AbstractMemHypModel::~AbstractMemHypModel()
{
	// TODO Auto-generated destructor stub
	for(int i=0; i<FieldsInModel->size();i++)
	{
		FieldsInModel->at(i)->clear();
		delete FieldsInModel->at(i);
	}
	FieldsInModel->clear();
	delete FieldsInModel;
	delete Id_records;
	delete KeyIterator;
	HeaderDecorationMap.clear();
}

int AbstractMemHypModel::columnCount(const QModelIndex & index) const
{
	//Процедура возвращает число столбцов (полей)
	return this->NumCol+ReservedColumns;
}

int AbstractMemHypModel::rowCount(const QModelIndex &parent) const
{
	//Процедура возвращает число строк
	return Id_records->size();
}

QVariant AbstractMemHypModel::data(const QModelIndex &index, int role) const
{
	 	if (!index.isValid())
	         return QVariant();

	     if (index.row() >= LastRequestedReccount)
	         return QVariant();

	     if (role == Qt::DisplayRole)
	         return GetDataFromBuffer(index);
	     else
	         return QVariant();
}

void AbstractMemHypModel::ReadToBuffer() const
{
	//Процедура считывает в буфер по полям записи из БД
	//Заполняем значения в массиве идентификаторов записей

	Id_records->clear();
	KeyIterator->First();
	LastRequestedReccount=0;
	while(KeyIterator->Fetched())
	{
		Id_records->append(KeyIterator->GetID());
		LastRequestedReccount++;
		KeyIterator->Next();
	}

	//Считываем поля экстраатрибутов
	for(int i=0;i<NumCol;i++)
	{
		GetOneFieldInBuffer(i, FieldsInModel->at(i));
	}
}

void AbstractMemHypModel::GetOneFieldInBuffer(int FieldNumber, QMap<long,QVariant>* OneMap) const
{
	//Процедура заполняет QMap переданный в параметре значениями поля

	OneMap->clear();
	iterHypotesis* MyIterator=new iterHypotesis(ForEidos);
	ExtraAttribute* MyEA;
	long Key;

	MyEA=getEAFromEidos(FieldNumber);	//Получаем экстраатрибут
	QString SQLString= getSQLstringforEA(MyEA);

	MyIterator->SQL_string=SQLString.toStdString();
	MyIterator->First();

	while(MyIterator->Fetched())
	{
			QVariant OneValue;

			switch(MyEA->type)
			{
				case platon::ft_String:
				{
					std::string tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
					OneValue=tr(tmpValue.c_str());
					Key=MyIterator->GetID();
					break;
				}
				case platon::ft_Integer:
				{
					int tmpValue;
					MyIterator->LocalST->Get("MEANING",(int32_t*)&tmpValue);
					OneValue=tmpValue;
					Key=MyIterator->GetID();
					break;
				}
				case platon::ft_Boolean:
				{
					bool tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
					OneValue=tmpValue;
					Key=MyIterator->GetID();
					break;
				}
				case platon::ft_Float:
				{
					float tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
					OneValue=tmpValue;
					Key=MyIterator->GetID();
					break;
				}
				case platon::ft_DateTime:
				{
					IBPP::Timestamp MyTs;
					MyIterator->LocalST->Get("MEANING",MyTs);
					Key=MyIterator->GetID();
					OneValue=IBPPTimestamp2QDateTime(MyTs);
					break;
				}
				case platon::ft_DLL:
				case platon::ft_LinkHypotesis:
				case platon::ft_LinkPragma:
				{
					std::string tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
					OneValue=tr(tmpValue.c_str());
					Key=MyIterator->GetID();

					break;
				}
				default:
					throw("Указанный тип данных не поддерживается");

			}
			OneMap->insert(Key,OneValue);
			MyIterator->Next();
	}
}
QVariant AbstractMemHypModel::GetDataFromBuffer(const QModelIndex &index)const
{
//Функция возвращает значение с индексом, переданном в параметре
	int row=index.row();
	int col=index.column();

	if(col==0) return QVariant::fromValue(Id_records->at(row));

	QMap<long,QVariant>* OneMap=FieldsInModel->at(col-1);
	long key=Id_records->at(row);
	QVariant ForReturn = OneMap->value(key);
	return ForReturn;
}
QModelIndex AbstractMemHypModel::GetQModelIndexByID(long ID_in)
{
	return this->index(Id_records->indexOf(ID_in),0,QModelIndex());
}

void AbstractMemHypModel::setHeaderIcon(int section, int orientation,QIcon& Icon)
{
	//Процедура проверяет наличие в контейнере установленной на элемент заголовка иконки и переопределяет его или
	//просто добавляет новый элемент с переданной в параметре иконкой

	QString CurrentKey=QString::number(orientation)+"|"+QString::number(section);

	if(HeaderDecorationMap.contains(CurrentKey)) HeaderDecorationMap.erase(HeaderDecorationMap.find(CurrentKey));
	HeaderDecorationMap.insert(CurrentKey,Icon);
}
void AbstractMemHypModel::RemoveHeaderIcon(int section, int orientation)
{
	QString CurrentKey=QString::number(orientation)+"|"+QString::number(section);
	if(HeaderDecorationMap.contains(CurrentKey)) HeaderDecorationMap.erase(HeaderDecorationMap.find(CurrentKey));
}
QVariant AbstractMemHypModel::GetHeaderIcon(int section, int orientation) const
{
	//Функция возвращает значение для декора (иконку)
	QString CurrentKey=QString::number(orientation)+"|"+QString::number(section);
	if(HeaderDecorationMap.contains(CurrentKey))
		return qVariantFromValue(HeaderDecorationMap.value(CurrentKey));
	else
		return QVariant();
}
}
