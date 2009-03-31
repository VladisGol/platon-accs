#include "mainWin.h"

mainWin::mainWin(QWidget *parent)
    : QMainWindow(parent)

{
		QTextCodec * codec = QTextCodec::codecForName("CP1251"); 	//������������� ����������
		QTextCodec::setCodecForTr(codec);
		QTextCodec::setCodecForCStrings(codec);

		setupUi(this);								//��������� �������� �����

		this->MyDCl=new platon::DataClass(this);	//������� ������ ������ ����������
		if(this->MyDCl->DB->Connected())			//���� ���������� � ����� ������ ��� �������� ������ ������ �������
		{
			LocalEidos=NULL;
			LocalHypotesis=NULL;

			QObject::connect(EidosTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem* ,int)), this, SLOT(SetHypotesysView(QTreeWidgetItem*,int)));
			QObject::connect(tableViewHypotesis, SIGNAL(activated(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));

			QObject::connect(tableViewHypotesis, SIGNAL(pressed(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
			QObject::connect(tableViewHypotesis, SIGNAL(clicked(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
			//QObject::connect(tableViewHypotesis, SIGNAL(clicked(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));
			QObject::connect(tableViewHypotesis, SIGNAL(entered(QModelIndex)), this, SLOT(SetPragmaView(QModelIndex)));

			//����������� �������� ���������� � ��������
			QObject::connect(comboBox_Species, SIGNAL(currentIndexChanged(int)), this, SLOT(SetEidosView(int)));
			QObject::connect(action_edit, SIGNAL(activated()), this, SLOT(EditItem()));
			QObject::connect(action_add, SIGNAL(activated()), this, SLOT(AddItem()));
			QObject::connect(action_del, SIGNAL(activated()), this, SLOT(DeleteItem()));
			QObject::connect(action_refresh, SIGNAL(activated()), this, SLOT(RefreshViews()));
			QObject::connect(action_links, SIGNAL(activated()), this, SLOT(Showlinks()));
			QObject::connect(action_quit, SIGNAL(activated()), this, SLOT(CloseForm()));

			SetEidosView(0);

			this->EidosTreeWidget->installEventFilter(this);
			this->tableViewHypotesis->installEventFilter(this);
			this->tableViewPragma->installEventFilter(this);
			CurrentObjectLevel=0;

			//����������� �����
			this->ProgramDateTime = QDateTime::currentDateTime();
			DTBaseShifter=new QTimer(this);
			connect(DTBaseShifter, SIGNAL(timeout()), this, SLOT(BaseTimeShift()));
			BaseTimeShift();
			DTBaseShifter->start(10*1000);	//������������� ����� ����������

			//���������� ������ ������ ��� ����������� ���������� � ������������
			SFProxyModelH= new QSortFilterProxyModel(this);
			SFProxyModelP= new QSortFilterProxyModel(this);

			tableViewPragma->setModel(SFProxyModelP);
			tableViewPragma->setSortingEnabled(true);
			tableViewHypotesis->setModel(SFProxyModelH);
			tableViewHypotesis->setSortingEnabled(true);

			//������������� ������� �������� �� �����
			ReadFormWidgetsAppearance();
		}
}

bool mainWin::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == EidosTreeWidget)
	{
		if(event->type()==QEvent::FocusIn)
		{
			this->action_add->setEnabled(false);
			this->action_del->setEnabled(false);
			this->action_edit->setEnabled(false);
			this->action_links->setEnabled(false);
		}
	}
	if (obj == tableViewHypotesis)
	{
		if(event->type()==QEvent::FocusIn)
		{
			this->action_add->setEnabled(true);
			this->action_del->setEnabled(true);
			this->action_edit->setEnabled(true);
			this->action_links->setEnabled(true);
			CurrentObjectLevel=Level_Hypotesis;
		}
	}
	if (obj == tableViewPragma)
	{
		if(event->type()==QEvent::FocusIn)
		{
			QString Species=QString::fromStdString(LocalEidos->GetEidosSpecies());
			if(Species=="OBJ" || Species=="RES")
			{
				this->action_add->setEnabled(true);
				this->action_del->setEnabled(true);
				this->action_edit->setEnabled(true);
				this->action_links->setEnabled(true);
			}
			else
			{
				this->action_add->setEnabled(false);
				this->action_del->setEnabled(true);
				this->action_edit->setEnabled(true);
				this->action_links->setEnabled(true);
			}
			CurrentObjectLevel=Level_Pragma;
		}
	}

	return false;
}


void mainWin::SetEidosView(int Row)
{
	switch (Row)
	{
	case 0:
		EidosTreeWidget->SetSpecies("ALL");
		break;
	case 1:
		EidosTreeWidget->SetSpecies("OBJ");
		break;
	case 2:
		EidosTreeWidget->SetSpecies("ACT");
		break;
	case 3:
		EidosTreeWidget->SetSpecies("RES");
		break;
	case 4:
		EidosTreeWidget->SetSpecies("NSI");
	}

	EidosTreeWidget->AttachToDB(this->MyDCl->DB);
}

void mainWin::SetHypotesysView(QTreeWidgetItem*CurItem , int Column)
{
	platon::Eidos* keep4delete=LocalEidos;

	long id_eidos=CurItem->text(1).toLong();

	LocalEidos=new platon::Eidos(this->MyDCl->DB,id_eidos);
	platon::HypotesisMemModel* MyModel=new platon::HypotesisMemModel(LocalEidos, this);
	SFProxyModelH->setSourceModel(MyModel);
	this->tableViewHypotesis->resizeColumnsToContents();

	if(tableViewHypotesis->model()->rowCount()>0)
		SetPragmaView(tableViewHypotesis->model()->index(0,0,QModelIndex()));
	else
		SFProxyModelP->setSourceModel(NULL);

	if(keep4delete!=NULL) delete keep4delete;
}

void mainWin::SetPragmaView(const QModelIndex & HypModelindex)
{
	platon::Hypotesis* keep4delete=LocalHypotesis;
	long id_hypotesys=HypModelindex.sibling(HypModelindex.row(),0).data(Qt::DisplayRole).toInt();

	LocalHypotesis=new platon::Hypotesis(this->LocalEidos, id_hypotesys);
	platon::PragmaMemModel* MyModel=new platon::PragmaMemModel(LocalHypotesis, this);
	SFProxyModelP->setSourceModel(MyModel);
	this->tableViewPragma->resizeColumnsToContents();
	if(keep4delete!=NULL) delete keep4delete;
}

void mainWin::EditItem()
{
	if(CurrentObjectLevel==Level_Hypotesis)
	{
		int myrow=tableViewHypotesis->currentIndex().row();
		long id_hypotesys=QVariant(tableViewHypotesis->model()->data(tableViewHypotesis->model()->index(myrow,0,QModelIndex()))).toInt();
		platon::HypotesisEditForm * md=new platon::HypotesisEditForm(this,id_hypotesys);
		md->setWindowTitle(tr("�������������� ������� \"���\""));
		md->show();
	}
	if(CurrentObjectLevel==Level_Pragma)
	{
		int myrow=tableViewPragma->currentIndex().row();
		long id_pragma=QVariant(tableViewPragma->model()->data(tableViewPragma->model()->index(myrow,0,QModelIndex()))).toInt();
		platon::PragmaEditForm * md=new platon::PragmaEditForm(this,id_pragma);
		md->setWindowTitle(tr("�������������� ������� \"���������\""));
		md->show();
	}
	return ;
}
void mainWin::AddItem()
{
	QString Species=QString::fromStdString(LocalEidos->GetEidosSpecies());
	platon::Eidos* formEidos=new platon::Eidos(LocalEidos->DB,LocalEidos->GetID());
	platon::Hypotesis* formHypotesis;

	if(CurrentObjectLevel==Level_Hypotesis)
	{
		//������� ���������� �������� ��� �����

		if(Species=="OBJ") formHypotesis= ((platon::OBJClass*)formEidos)->AddOBJType();
		if(Species=="ACT") formHypotesis= ((platon::ACTClass*)formEidos)->AddACTType();
		if(Species=="RES") formHypotesis= ((platon::RESClass*)formEidos)->AddRESType();
		if(Species=="NSI") formHypotesis=new platon::Hypotesis(formEidos,QString(tr("����� ��� ����������-���������� ����������")).toStdString());

		platon::HypotesisEditForm * md=new platon::HypotesisEditForm(this,formHypotesis);
		md->setWindowTitle(tr("�������� ������� \"���\""));
		md->show();
	}
	if(CurrentObjectLevel==Level_Pragma)
	{
		int myrow=tableViewHypotesis->currentIndex().row();
		if(myrow==-1) myrow=0;	//��� �� ������ ���� ����� ����������� � ���� Pragma ��� ������ bp Hypotesis
		long id_hypotesys=QVariant(tableViewHypotesis->model()->data(tableViewHypotesis->model()->index(myrow,0,QModelIndex()))).toInt();
		if(id_hypotesys>0)
		{
			formHypotesis=new platon::Hypotesis(formEidos,id_hypotesys);
			platon::Pragma*formPragma;

			//������� ���������� �������� ��� �����
			if(Species=="OBJ") formPragma= ((platon::OBJType*)formHypotesis)->AddOBJCopy();
			if(Species=="ACT")
				{
					; //���������� ������� � ����� �������� ���������� ��������
				}
			if(Species=="RES") formPragma= ((platon::RESType*)formHypotesis)->AddRESCopy(); //�������� �������, ������� ����������� "�� ������"
			if(Species=="NSI")	;//�� ����� ���� ����������� �� ��������� �����
			platon::PragmaEditForm * md=new platon::PragmaEditForm(this,formPragma);
			md->setWindowTitle(tr("�������� ������� \"���������\""));
			md->show();
		}
	}
	return ;
}
void mainWin::DeleteItem()
{
}
void mainWin::Showlinks()
{
	if(CurrentObjectLevel==Level_Hypotesis)
	{
		int myrow=tableViewHypotesis->currentIndex().row();
		long id_hypotesys=QVariant(tableViewHypotesis->model()->data(tableViewHypotesis->model()->index(myrow,0,QModelIndex()))).toInt();
		platon::LinksExplorer* LnkForm=new platon::LinksExplorer(this,id_hypotesys,"ALL");
		LnkForm->show();
	}
	if(CurrentObjectLevel==Level_Pragma)
	{
		int myrow=tableViewPragma->currentIndex().row();
		long id_hypotesys=QVariant(tableViewPragma->model()->data(tableViewPragma->model()->index(myrow,0,QModelIndex()))).toInt();
		platon::LinksExplorer* LnkForm=new platon::LinksExplorer(this,id_hypotesys,"ALL");
		LnkForm->show();
	}
}

void mainWin::RefreshViews()
{
//��������� ��������� �������� � ����� ����������� ������� � �����
	if(CurrentObjectLevel==Level_Hypotesis) RefreshHView();
	if(CurrentObjectLevel==Level_Pragma) RefreshPView();

}
void mainWin::RefreshHView()
{
	SetHypotesysView(this->EidosTreeWidget->currentItem(),0);
}
void mainWin::RefreshPView()
{
	SetPragmaView(tableViewHypotesis->currentIndex());
}


void mainWin::BaseTimeShift()
{
//��������� - ���� ���������� ������� �������� ���������� ������� � ���� ������ ��� ������������� ������� �� ������������ ���������
	//��������� �������� �� ��������� ���� ������� � ������� +-5 ����� �� ���������� �������
	if(QDateTime::currentDateTime() < this->ProgramDateTime.addSecs(5*60) && QDateTime::currentDateTime().addSecs(5*60) > this->ProgramDateTime)
	{
		ProgramDateTime= QDateTime::currentDateTime();
	}

	platon::SetTimestampTemporalCompareFor(this->MyDCl->DB, platon::QDateTime2IBPPTimestamp(ProgramDateTime));
	this->statusbar->showMessage(tr("����������� ����� ")+ProgramDateTime.toString(tr("dd.MMMM.yyyy hh:mm:ss")));
}
void mainWin::CloseForm()
{
	//���� �������� �����. ����� ��������� ����� ��������� ��������� �����
	WriteFormWidgetsAppearance();
	this->close();
}
void mainWin::ReadFormWidgetsAppearance()
{
	//��������� ��������� �� DbETC ��������� ��������� ����� � ������������� �� ��������
	platon::DbEtc* MyETC=new platon::DbEtc(this->MyDCl->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName ()).toStdString(),true,-1);
	int w=874,h=744;
	if(MyETC->ParamExists("width")) w=MyETC->ReadInteger("width");
	if(MyETC->ParamExists("height")) h=MyETC->ReadInteger("height");
	this->resize (w,h);

	QList<int> vals;
	if(MyETC->ParamExists("splitter_e\\0")) vals.append(MyETC->ReadInteger("splitter_e\\0")); else vals.append(350);
	if(MyETC->ParamExists("splitter_e\\1")) vals.append(MyETC->ReadInteger("splitter_e\\1")); else vals.append(762);
	splitter_e->setSizes(vals);
	vals.clear();
	if(MyETC->ParamExists("splitter_hp\\0")) vals.append(MyETC->ReadInteger("splitter_hp\\0")); else vals.append(361);
	if(MyETC->ParamExists("splitter_hp\\1")) vals.append(MyETC->ReadInteger("splitter_hp\\1")); else vals.append(362);
	splitter_hp->setSizes(vals);

	if(MyETC->ParamExists("comboBox_Species")) this->comboBox_Species->setCurrentIndex(MyETC->ReadInteger("comboBox_Species"));
	if(MyETC->ParamExists("EidosID")) this->EidosTreeWidget->findNMakeCurrent(MyETC->ReadInteger("EidosID"));

	SetHypotesysView(this->EidosTreeWidget->currentItem(),0);

	MyETC->CloseKey();
	delete MyETC;

}
void mainWin::WriteFormWidgetsAppearance()
{
	//��������� ���������� � DbETC ��������� ��������� �����
	platon::DbEtc* MyETC=new platon::DbEtc(this->MyDCl->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName ()).toStdString(),true,-1);
	MyETC->WriteInteger("width", this->width());
	MyETC->WriteInteger("height", this->height());

	QList<int> vals = this->splitter_e->sizes();
	MyETC->WriteInteger("splitter_e\\0", vals.at(0));
	MyETC->WriteInteger("splitter_e\\1", vals.at(1));
	vals=this->splitter_hp->sizes();
	MyETC->WriteInteger("splitter_hp\\0", vals.at(0));
	MyETC->WriteInteger("splitter_hp\\1", vals.at(1));

	MyETC->WriteInteger("comboBox_Species", this->comboBox_Species->currentIndex());
	MyETC->WriteInteger("EidosID",this->EidosTreeWidget->GetEidosID());

	MyETC->CloseKey();
	delete MyETC;

}
