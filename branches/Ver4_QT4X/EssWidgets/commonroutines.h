#ifndef COMMONROUTINES_H
#define COMMONROUTINES_H

#include <QtGui>
#include "ibpp.h"

namespace platon
{
	QDateTime IBPPTimestamp2QDateTime(IBPP::Timestamp InVal);
	IBPP::Timestamp QDateTime2IBPPTimestamp(QDateTime InVal);
}

#endif
