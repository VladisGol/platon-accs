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
#include "dbetc.h"

#include "ibpp.h"

namespace platon
{
enum TypeETC {ETC_database,ETC_localfile};
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

    // Набор процедур и функций для сохранения параметров открываемых форм

    void SetTypeETC(int TypeForUse);
    int  GetTypeETC();
    bool ETC_ParamExists(QString ParamName);
    void ETC_OpenKey(QString key_val);
    void ETC_CloseKey();
    bool ETC_ReadBool(QString ParamName);
    int  ETC_ReadInteger(QString ParamName);
    void ETC_WriteBool(QString ParamName, bool InValue);
    void ETC_WriteInteger(QString ParamName, int InValue);


private:
    bool IsDatabaseConnect;
    DbEtc *MyETC;                           //Объект для сохранения и считывания параметров из БД
    QSettings* FileETC;                     //Объект для сохранения и считывания параметров из локального файла
    int usedTypeETC;                            //Какой вид ETC используется

};
DataClass* GetDataModule(QObject * fromform);

}

#endif /* DATACLASS_H_ */
