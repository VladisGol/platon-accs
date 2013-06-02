#ifndef PragmaEditForm_H
#define PragmaEditForm_H

#include "abstracthip_editform.h"
#include "DataClass.h"
#include <QtGui>

#include "EssentialClass.h"
#include "ibpp.h"

namespace platon
{
class PragmaEditForm  :public AbstarctHipEditForm
{

public:
	PragmaEditForm(QWidget * parent, long ID_Pragma);
	PragmaEditForm(QWidget * parent, Pragma* InPr);
	~PragmaEditForm();
	Hypotesis*LocalHostHypotesis;
};
}

#endif
