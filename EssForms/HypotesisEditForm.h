#ifndef HypotesisEditForm_H
#define HypotesisEditForm_H

#include "abstracthip_editform.h"
#include <QtGui>

#include "EssentialClass.h"
#include "ibpp.h"

namespace platon
{
class HypotesisEditForm  :public AbstarctHipEditForm
{

public:
	HypotesisEditForm(QWidget * parent, IBPP::Database InDB, long ID_Hypotesys);
	HypotesisEditForm(QWidget * parent, Hypotesis* InHip);
	~HypotesisEditForm();
};
}

#endif
