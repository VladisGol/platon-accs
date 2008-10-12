#include "HypotesisEditForm.h"

namespace platon
{
HypotesisEditForm::HypotesisEditForm(QWidget * parent, IBPP::Database InDB, long ID_Hypotesys): QDialog(parent)
{
	setupUi(this);

	this->DB=InDB;
	long EidosID,HypotesysID;
	Hypotesis::GetEidosHypotesisIDS(DB, ID_Hypotesys,EidosID,HypotesysID);
	if(ID_Hypotesys==HypotesysID)	//Проверяем найдены ли среди гипотез заданная в параметре
	{
		LocalEidos= new Eidos(DB, EidosID);
		LocalHypotesis=new Hypotesis(LocalEidos,HypotesysID);

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
	else throw "Объект на который указывает идентификатор из параметра не существует";
}
}
