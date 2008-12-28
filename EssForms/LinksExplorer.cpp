#include "LinksExplorer.h"

namespace platon
{
LinksExplorer::LinksExplorer(QWidget * parent): QMainWindow(parent)
{
	setupUi(this);

//	QObject::connect(action_Cancel, SIGNAL(activated()), this, SLOT(ExitByCancel()));
//	QObject::connect(action_saveNexit, SIGNAL(activated()), this, SLOT(ExitWithSave()));
}

void LinksExplorer::ExitWithSave()
{
//	this->LocalHypotesis->TransactionIBPP->Commit();
	this->close();
}
void LinksExplorer::ExitByCancel()
{
//	this->LocalHypotesis->TransactionIBPP->Rollback();
	this->close();
}
/*
void AbstarctHipEditForm::FormFillFrames()
{
	for(unsigned int i =0; i<LocalHypotesis->Attributes.size();i++)
	{
		AssociatedExtraAttribute* tmpAttrib =(AssociatedExtraAttribute*)LocalHypotesis->Attributes[i];
		if(tmpAttrib->EA->type!=platon::ft_Security && tmpAttrib->EA->Visible)
		{
			EA_OneFrame* my =new EA_OneFrame(this,tmpAttrib);
			my->setObjectName("EA_Frame"+QString::number(i));
			verticalLayout->insertWidget(0,my); //Заносим в форму фреймы с нулевого индекса, чтобы последние были вверху
		}
	}
}
*/

}
