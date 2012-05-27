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
#include "DialogLogin.h"
#include <QDateTime>

#include "ibpp.h"

namespace platon
{
//Класс хранящий в себе основные элементы программы, к которым необходим доступ из форм программы
class DataClass : public QObject
{
	Q_OBJECT
public:
	DataClass(QObject *parent);
	virtual ~DataClass();
	QMap <QString, QLibrary*> ArrayDynLib;	//Массив библиотек
	IBPP::Database DB;						//База данных
	QDateTime ProgramDateTime;				//Переменная для программного времени
	bool ViewIDs;							//Переменная отвечающая за то, отображаются идентификаторы объектов или нет
    bool IsDBConnected();                   //Функция возвращающая значение переменной присоединена ли БД

	void LoadDynLib(QSplashScreen* sps, QApplication * aplic);
	QLibrary* GetLibByName(QString inString);
	void SetProgramDateTime();
	QString CalcFileMD5(QString fileName);
	QString GetSavedMD5(QString fileName);
private:
    bool IsDatabaseConnect;
};
DataClass* GetDataModule(QObject * fromform);

}

#endif /* DATACLASS_H_ */
