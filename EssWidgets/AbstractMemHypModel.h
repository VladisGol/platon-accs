/*
 * AbstractMemHypModel.h
 *
 *  Created on: 07.08.2008
 *      Author: slava
 */

#ifndef ABSTRACTMEMHYPMODEL_H_
#define ABSTRACTMEMHYPMODEL_H_

#include <QAbstractTableModel>
#include <QVector>
#include <QMap>
#include <QDate>
#include "EssentialClass.h"
#include "commonroutines.h"


namespace platon
{
class AbstractMemHypModel : public QAbstractTableModel
{
public:
	AbstractMemHypModel(QObject * parent);
	virtual ~AbstractMemHypModel();

    int rowCount(const QModelIndex &parent = QModelIndex())const;
    int columnCount(const QModelIndex & index) const;
    QVariant data(const QModelIndex &index, int role) const;
	pIterator* KeyIterator;
	void ReadToBuffer()const;								// Процедура считывает в буфер все записи
	QModelIndex GetQModelIndexByID(int ID_in);				// Функция возвращает значение индекса модели по идентификатору записи
	void setHeaderIcon(int section, int orientation,QIcon& Icon); //Функция назначения иконки на элемент заголовка
	void RemoveHeaderIcon(int section, int orientation);			 //Функция удаления иконки с элемента заголовка
	QVariant GetHeaderIcon(int section, int orientation) const;			 //Функция возвращает значение иконки для указанного в параметрах элемента заголовка

protected:
	mutable QVector <long> * Id_records;					//Вектор для хранения ID записи по которой можно будет найти записи в векторе FieldsInModel
	mutable QVector <QMap<long,QVariant>*> * FieldsInModel;	//Вектор для хранения значений полей в виде QMap <long,QVariant>
	mutable int NumCol;										//Число полей в запросе (из числа экстраатрибутов)
	mutable int LastRequestedReccount;						//Число записей в запросе

	int ReservedColumns;									//Зарезервированное число полей
    QVariant GetDataFromBuffer(const QModelIndex &index) const;
	void GetOneFieldInBuffer(int FieldNumber, QMap<long,QVariant>* OneMap) const;

	Eidos* ForEidos;										//Ссылка на Eidos, по которому идет обработка

	virtual ExtraAttribute* getEAFromEidos(int i) const {return NULL;};			//Функция возвращает ссылку на экстраатрибут по номеру
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const {return "";};	//Функция возвращает заполненную SQL строку для получения 1 экстраатрибута
	QMap <QString, QIcon> HeaderDecorationMap;			//Контейнер, содержащий элементы декора (иконки)
};

}

#endif /* ABSTRACTMEMHYPMODEL_H_ */
