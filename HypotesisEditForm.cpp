#include "HypotesisEditForm.h"


HypotesisEditForm::HypotesisEditForm(QWidget * parent ): QDialog(parent)
{
	setupUi(this);

	for (int i=0;i<150;i++)
	{
		EA_OneFrame* my =new EA_OneFrame(this);
		my->setObjectName("Frame"+QString::number(i));
		my->label->setText("Frame "+QString::number(i));

		verticalLayout->addWidget(my);


	}

	;

}

