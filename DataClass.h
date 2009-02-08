/*
 * DataClass.h
 *
 *  Created on: 08.02.2009
 *      Author: slava
 */

#ifndef DATACLASS_H_
#define DATACLASS_H_

#include <QLibrary>
#include <QMap>
#include <QDir>
#include <QApplication>
#include <QSplashScreen>
#include <QMessageBox>


#include "ibpp.h"

namespace platon
{
//����� �������� � ���� �������� �������� ���������, � ������� ��������� ������ �� ���� ���������
class DataClass : public QObject
{
	Q_OBJECT
public:
	DataClass();
	virtual ~DataClass();
	QMap <QString, QLibrary*> ArrayDynLib;	//������ ���������
	IBPP::Database DB;						//���� ������

	void LoadDynLib(QSplashScreen* sps, QApplication * aplic);

};

}

#endif /* DATACLASS_H_ */
