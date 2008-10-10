#include "HypotesisEditForm.h"

namespace platon
{
HypotesisEditForm::HypotesisEditForm(QWidget * parent, IBPP::Database InDB): QDialog(parent)
{
	setupUi(this);
	//6819
	this->DB=InDB;
	long EidosID,HypotesysID;
	Hypotesis::GetEidosHypotesisIDS(DB, 6819,EidosID,HypotesysID);

	LocalEidos= new Eidos(DB, EidosID);
	LocalHypotesis=new Hypotesis(LocalEidos,6819);

	for(unsigned int i =0;i<LocalHypotesis->Attributes.size();i++)
	{
        AssociatedExtraAttribute* tmpAttrib =(AssociatedExtraAttribute*)LocalHypotesis->Attributes[i];
		if(tmpAttrib->EA->type!=platon::ft_Security && tmpAttrib->EA->Visible)
		{
			EA_OneFrame* my =new EA_OneFrame(this,tmpAttrib);
			my->setObjectName("Frame"+QString::number(i));
			verticalLayout->addWidget(my);
		}
	}
}
}
