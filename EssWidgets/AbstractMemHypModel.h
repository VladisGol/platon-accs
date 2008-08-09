/*
 * AbstractMemHypModel.h
 *
 *  Created on: 07.08.2008
 *      Author: slava
 */

#include <QAbstractTableModel>
#include <QVector>
#include <QMap>
#include <QDate>
#include "EssentialClass.h"

#ifndef ABSTRACTMEMHYPMODEL_H_
#define ABSTRACTMEMHYPMODEL_H_

namespace platon
{

class AbstractMemHypModel : public QAbstractTableModel
{
public:
	AbstractMemHypModel(QObject * parent):QAbstractTableModel(parent){};
	virtual ~AbstractMemHypModel();

    int rowCount(const QModelIndex &parent = QModelIndex())const;
    int columnCount(const QModelIndex & index) const;
    QVariant data(const QModelIndex &index, int role) const;


protected:
	mutable QVector <long> * Id_records;					//Вектор для хранения ID записи по которой можно будет найти записи в векторе FieldsInModel
	mutable QVector <QMap<long,QVariant>*> * FieldsInModel;	//Вектор для хранения значений полей в виде QMap <long,QVariant>
	mutable int NumCol;										//Число полей в запросе (из числа экстраатрибутов)
	mutable int LastRequestedReccount;						//Число записей в запросе

	int ReservedColumns;									//Зарезервированное число полей
    QVariant GetDataFromBuffer(const QModelIndex &index) const;
	void ReadToBuffer()const;								// Процедура считывает в буфер все записи
	void GetOneFieldInBuffer(int FieldNumber, QMap<long,QVariant>* OneMap) const;

	Eidos* ForEidos;										//Ссылка на Eidos, по которому идет обработка

	virtual ExtraAttribute* getEAFromEidos(int i) const {return NULL;};		//Процедура возвращает ссылку на экстраатрибут по номеру
};

}

#endif /* ABSTRACTMEMHYPMODEL_H_ */
