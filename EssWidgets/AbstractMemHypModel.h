/*
 * AbstractMemHypModel.h
 *
 *  Created on: 07.08.2008
 *      Author: slava
 */

#include <QAbstractTableModel>
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
	mutable QVector<QMap<long,QVariant>*> *FieldsInModel;	//������ ��� �������� �������� ����� � ���� QMap <long,QVariant>
	mutable QVector<long> *Id_records;						//������ ��� �������� ID ������ �� ������� ����� ����� ����� ������ � ������� FieldsInModel
	mutable int NumCol;										//����� ����� � ������� (�� ����� ���������������)
	int ReservedColumns;									//����������������� ����� �����
    QVariant GetDataFromBuffer(const QModelIndex &index) const;
	void ReadToBuffer()const;								// ��������� ��������� � ����� ��� ������

	Eidos* ForEidos;										//������ �� Eidos, �� �������� ���� ���������

	virtual ExtraAttribute* getEAFromEidos(int i){return NULL};		//��������� ���������� ������ �� ������������� �� ������
};

}

#endif /* ABSTRACTMEMHYPMODEL_H_ */
