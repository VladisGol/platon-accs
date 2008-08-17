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

protected:
	mutable QVector <long> * Id_records;					//������ ��� �������� ID ������ �� ������� ����� ����� ����� ������ � ������� FieldsInModel
	mutable QVector <QMap<long,QVariant>*> * FieldsInModel;	//������ ��� �������� �������� ����� � ���� QMap <long,QVariant>
	mutable int NumCol;										//����� ����� � ������� (�� ����� ���������������)
	mutable int LastRequestedReccount;						//����� ������� � �������

	int ReservedColumns;									//����������������� ����� �����
    QVariant GetDataFromBuffer(const QModelIndex &index) const;
	void ReadToBuffer()const;								// ��������� ��������� � ����� ��� ������
	void GetOneFieldInBuffer(int FieldNumber, QMap<long,QVariant>* OneMap) const;

	Eidos* ForEidos;										//������ �� Eidos, �� �������� ���� ���������

	virtual ExtraAttribute* getEAFromEidos(int i) const {return NULL;};			//������� ���������� ������ �� ������������� �� ������
	virtual QString getSQLstringforEA(ExtraAttribute*MyEA) const {return "";};	//������� ���������� ����������� SQL ������ ��� ��������� 1 ��������������
};

}

#endif /* ABSTRACTMEMHYPMODEL_H_ */
