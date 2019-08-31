#ifndef HypotesisEditForm_H
#define HypotesisEditForm_H

#include "abstracthip_editform.h"
#include <QtGui>

#include "EssentialClass.h"
#include "DataClass.h"
#include "ibpp.h"

namespace platon
{
class HypotesisEditForm  :public AbstarctHipEditForm
{

public:
	HypotesisEditForm(QWidget * parent, int  ID_Hypotesys);
	HypotesisEditForm(QWidget * parent, Hypotesis* InHip);
	~HypotesisEditForm();
};
}

#endif
