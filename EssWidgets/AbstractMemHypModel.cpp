/*
 * AbstractMemHypModel.cpp
 *
 *  Created on: 07.08.2008
 *      Author: slava
 */

#include "AbstractMemHypModel.h"

namespace platon
{


AbstractMemHypModel::~AbstractMemHypModel()
{
	// TODO Auto-generated destructor stub
	for(int i==0; i<FieldsInModel->size();i++)
			delete FieldsInModel[i];
	delete FieldsInModel;
	delete Id_records;
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

AbstractMemHypModel::ReadToBuffer() const
{
	//Процедура считывает в буфер по полям записи из БД
	//Заполняем значения в массиве идентификаторов записей
	iterHypotesis* MyIterator=new iterHypotesis(ForEidos);
	Id_records->clear();
	MyIterator->First();
	while(MyIterator->Fetched())
	{
		Id_records->append(MyIterator->GetID());
		MyIterator->Next();
	}

	//Считываем поля экстраатрибутов
	for(int i=0;i<NumCol;i++)
	{
		GetOneFieldInBuffer(i, FieldsInModel[i]);
	}
}

void AbstractMemHypModel::GetOneFieldInBuffer(int FieldNumber, QMap<long,QVariant>* OneMap)
{
	//Процедура заполняет QMap переданный в параметре значениями поля

	OneMap->clear();
	iterHypotesis* MyIterator=new iterHypotesis(ForEidos);
	ExtraAttribute* MyEA;

	MyEA=getEAFromEidos(FieldNumber);	//Получаем экстраатрибут

	QString SQLString;
	SQLString="select * from "+QString::fromStdString(MyEA->NameStoredProc())+"("+QString::number(ForEidos->GetID())+");";
	MyIterator->SQL_string=SQLString.toStdString();

	MyIterator->First();

	while(MyIterator->Fetched())
	{
			QVariant OneValue;
			long Key;

			switch(MyEA->type)
			{
				case platon::ft_String:
				{
					std::string tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
					OneValue=QString::fromStdString(tmpValue);
					Key=MyIterator->LocalST->Get("id_link",(int32_t*)&Key);
					break;
				}
				case platon::ft_Integer:
				{
					int tmpValue;
					MyIterator->LocalST->Get("MEANING",(int32_t*)&tmpValue);
					OneValue=tmpValue;
					Key=MyIterator->LocalST->Get("id_link",(int32_t*)&Key);
					break;
				}
				case platon::ft_Boolean:
				{
					bool tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
					OneValue=tmpValue;
					Key=MyIterator->LocalST->Get("id_link",(int32_t*)&Key);
					break;
				}
				case platon::ft_Float:
				{
					float tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
					OneValue=tmpValue;
					Key=MyIterator->LocalST->Get("id_link",(int32_t*)&Key);
					break;
				}
				case platon::ft_DateTime:
				{
					IBPP::Timestamp MyTs;
					MyIterator->LocalST->Get("MEANING",MyTs);
					Key=MyIterator->LocalST->Get("id_link",(int32_t*)&Key);

					QString year=QString::number(MyTs.Year());
					QString month=QString("0"+QString::number(MyTs.Month())).right(2);
					QString day=QString("0"+QString::number(MyTs.Day())).right(2);
					QDateTime LocAlValue=QDateTime::fromString(year+month+day,"yyyyMMdd");
					LocAlValue.addSecs(MyTs.Hours()*3600+MyTs.Minutes()*60+MyTs.Seconds());

					OneValue=LocAlValue;
					break;
				}
				case platon::ft_RB:
				case platon::ft_DLL:
				case platon::ft_LinkHypotesis:
				case platon::ft_LinkPragma:
				{
					std::string tmpValue;
					MyIterator->LocalST->Get("MEANING",tmpValue);
					OneValue=QString::fromStdString(tmpValue);
					Key=MyIterator->LocalST->Get("id_link",(int32_t*)&Key);

					break;
				}
				default:
					throw("Указанный тип данных не поддерживается");

			}
			OneMap->insert(key,OneValue);
			MyIterator->Next();
	}

}
QVariant AbstractMemHypModel::GetDataFromBuffer(const QModelIndex &index)const
{
//Функция возвращает значение с индексом, переданном в параметре
	int row=index.row();
	int col=index.column();

	if(row==0) return Id_records[col];

	QMap<long,QVariant>* OneMap=FieldsInModel[row];
	return OneMap.take(Id_records[col]);
}

}
