#include "commonroutines.h"
namespace platon
{
QDateTime IBPPTimestamp2QDateTime(IBPP::Timestamp InVal)
{
//Функция переводит дату время из формата IBPP::Timestamp в формат QDateTime
	QString year=QString::number(InVal.Year());
	QString month=QString("0"+QString::number(InVal.Month())).right(2);
	QString day=QString("0"+QString::number(InVal.Day())).right(2);
	QString hours=QString("0"+QString::number(InVal.Hours())).right(2);
	QString minutes=QString("0"+QString::number(InVal.Minutes())).right(2);
	QString seconds=QString("0"+QString::number(InVal.Seconds())).right(2);
	QDateTime LocAlValue=QDateTime::fromString(year+month+day+" "+hours+":"+minutes+":"+seconds,"yyyyMMdd hh:mm:ss");
	return LocAlValue;
}

IBPP::Timestamp QDateTime2IBPPTimestamp(QDateTime InVal)
{
//Функция переводит дату время из формата QDateTime в формат IBPP::Timestamp

	int y, mo, d, h, mi, s;
	y=QString(InVal.toString("yyyy")).toInt();
	mo=QString(InVal.toString("MM")).toInt();
	d=QString(InVal.toString("dd")).toInt();
	h=QString(InVal.toString("hh")).toInt();
	mi=QString(InVal.toString("mm")).toInt();
	s=QString(InVal.toString("ss")).toInt();
	return IBPP::Timestamp(y, mo, d, h, mi, s);
}
}


