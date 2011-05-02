#include "es_mainwindow.h"

namespace platon
{
es_mainwindow::es_mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
	//Конструктор формы
	numColsInTableEA=7;

	ui.setupUi(this);

	icon_locked.addPixmap(QPixmap(QString::fromUtf8((":/PICS/padlock_aj_ashton_01.png"))), QIcon::Normal, QIcon::Off);
	icon_editable.addPixmap(QPixmap(QString::fromUtf8((":/PICS/bb_txt_.png"))), QIcon::Normal, QIcon::Off);
	icon_occouped.addPixmap(QPixmap(QString::fromUtf8((":/PICS/attention_niels_epting_.png"))), QIcon::Normal, QIcon::Off);

	//Чтобы можно было идентифицировать подписи комбобокса делаем через функцию
	ui.comboBox_Type->clear();
	ui.comboBox_Type->addItem(GetEATypeName(ft_String),ft_String);
	ui.comboBox_Type->addItem(GetEATypeName(ft_Integer),ft_Integer);
	ui.comboBox_Type->addItem(GetEATypeName(ft_Boolean),ft_Boolean);
	ui.comboBox_Type->addItem(GetEATypeName(ft_Float),ft_Float);
	ui.comboBox_Type->addItem(GetEATypeName(ft_DateTime),ft_DateTime);
	ui.comboBox_Type->addItem(GetEATypeName(ft_DLL),ft_DLL);
	ui.comboBox_Type->addItem(GetEATypeName(ft_LinkHypotesis),ft_LinkHypotesis);
	ui.comboBox_Type->addItem(GetEATypeName(ft_LinkPragma),ft_LinkPragma);
	ui.comboBox_Type->addItem(GetEATypeName(ft_Security),ft_Security);

	ui.comboBox_Species->clear();
	ui.comboBox_Species->addItem(tr("/"),"ALL");
	ui.comboBox_Species->addItem(tr("Объекты учета"),"OBJ");
	ui.comboBox_Species->addItem(tr("Подучетные действия"),"ACT");
	ui.comboBox_Species->addItem(tr("Ресурсы"),"RES");
	ui.comboBox_Species->addItem(tr("Нормативно-справочная информация"),"NSI");

        //Устанавливаем заголовки к таблице атрибутов
	ui.tableWidget_EAs->setColumnCount(numColsInTableEA);
	for (int j=0;j<numColsInTableEA;j++)
	{
		QTableWidgetItem* OneItem =new QTableWidgetItem();
		ui.tableWidget_EAs->setHorizontalHeaderItem(j, OneItem);
	}
	QStringList labels;
	labels << tr("ID")<<tr("Статус")<<tr("Поле")<<tr("Заголовок")<<tr("Тип")<<tr("Принадлежит")<<tr("Eidos определения");
	ui.tableWidget_EAs->setHorizontalHeaderLabels (labels);


	LocalEidos=NULL;
	CurrentEA=NULL;
	CurEAChanged=false;

	DataClass* DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;
	this->IsViewID=DTL->ViewIDs;

	ui.EidosTreeWidget->SetSpecies("ALL");
	ui.EidosTreeWidget->AttachToDB(this->DB);
	ReadFormWidgetsAppearance();
	ViewID_Activated();

        //Заполняем из датамодуля загруженные динамические библиотеки в comboBox_DLL_Name
        ui.comboBox_DLL_Name->addItems(DTL->ArrayDynLib.keys());

	QObject::connect(ui.action_quit, SIGNAL(activated()), this, SLOT(Exit()));
        QObject::connect(ui.EidosTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem* ,int)), this, SLOT(FillEAGrid(QTreeWidgetItem*,int)));
        QObject::connect(ui.comboBox_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(comboTypeChanged(int)));
        QObject::connect(ui.action_Eidos_rename, SIGNAL(activated()), this, SLOT(RenameEidos()));
        QObject::connect(ui.action_Eidos_add, SIGNAL(activated()), this, SLOT(AddChildEidos()));
        QObject::connect(ui.action_Eidos_del, SIGNAL(activated()), this, SLOT(DeleteEidos()));
        //QObject::connect(ui.action_EA_Del, SIGNAL(activated()), this, SLOT(RemoveChildEidos()));
        QObject::connect(ui.action_DLL_manager, SIGNAL(activated()), this, SLOT(Exit()));

        QObject::disconnect(ui.action_SaveChanges, SIGNAL(activated()), this, SLOT(SaveCurEA()));
        ui.action_SaveChanges->setEnabled(false);
}

void es_mainwindow::EAChoosed(QTableWidgetItem*CurElement,QTableWidgetItem*PrevElement )
{
	//при выборе экстраатрибута следует загрузить и отобразить его свойства
	QObject::disconnect(ui.comboBox_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(UserTryToEditEA()));
	QObject::disconnect(ui.comboBox_DLL_Name, SIGNAL(currentIndexChanged(int)), this, SLOT(UserTryToEditEA()));
        QObject::disconnect(ui.lineEdit_DLL_Proc, SIGNAL(textChanged(QString)), this ,SLOT(UserTryToEditEA()));
	QObject::disconnect(ui.comboBox_Species, SIGNAL(currentIndexChanged(int)), this, SLOT(UserTryToEditEA()));
	QObject::disconnect(ui.radioButton_BFH, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::disconnect(ui.checkBox_Locked, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::disconnect(ui.checkBox_Multiple, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::disconnect(ui.checkBox_NeedList, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::disconnect(ui.checkBox_Required, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::disconnect(ui.checkBox_Temporality, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::disconnect(ui.checkBox_Visible, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
        QObject::disconnect(ui.lineEdit_Caption, SIGNAL(textChanged(QString)), this ,SLOT(UserTryToEditEA()));
        QObject::disconnect(ui.lineEdit_FieldName, SIGNAL(textChanged(QString)), this ,SLOT(UserTryToEditEA()));

	if(CurEAChanged==true)	//Переход на другой элемент в режиме редактирования свойств атрибута
	{
		int ret = QMessageBox::warning(this, tr("Внимание, не сохраненные изменения"),
		                                tr("Атрибут был изменен\n"
		                                   "Желаете сохранить изменения?"),
		                                QMessageBox::Save | QMessageBox::Discard,
		                                QMessageBox::Save);
		if(ret==QMessageBox::Save) SaveCurEA();
	}

	int curRow=CurElement->row();
	CurrentEA = LocalEidos->GetEAByFieldName(ui.tableWidget_EAs->item(curRow, 2)->text().toStdString());
	int ComdoIndex=ui.comboBox_Type->findData(CurrentEA->type,Qt::UserRole, Qt::MatchExactly);
	ui.comboBox_Type->setCurrentIndex(ComdoIndex);

	if(CurrentEA->belongTo==ExtraAttribute::_theHypotesis)
		ui.radioButton_BFH->setChecked(true);
	else
		ui.radioButton_BFP->setChecked(true);

	ui.checkBox_Locked->setChecked(CurrentEA->Locked);
	ui.checkBox_Required->setChecked(CurrentEA->Required);
	ui.checkBox_Visible->setChecked(CurrentEA->Visible);
	ui.checkBox_Temporality->setChecked(CurrentEA->Temporality);
	ui.checkBox_NeedList->setChecked(CurrentEA->LNK_NeedList);
	ui.checkBox_Multiple->setChecked(CurrentEA->Multilnk);
	ui.lineEdit_Caption->setText(QString::fromStdString(CurrentEA->GetEACaption()));
	ui.lineEdit_FieldName->setText(QString::fromStdString(CurrentEA->GetEAFieldName()));
        //ui.comboBox_DLL_Name->setCurrentIndex(urrentText=QString::fromStdString(CurrentEA->DLL_FileName);
        ui.lineEdit_DLL_Proc->setText(QString::fromStdString(CurrentEA->DLL_ProcName));


	ComdoIndex=ui.comboBox_Species->findData(QString::fromStdString(CurrentEA->LNK_species),Qt::UserRole, Qt::MatchExactly);
	ui.comboBox_Species->setCurrentIndex(ComdoIndex);

	ui.lineEdit_Eidos->setText("");
	if(CurrentEA->LNK_EidosID>0)
	{
		Eidos* tmpEidos=new Eidos (this->DB,CurrentEA->LNK_EidosID);
		ui.lineEdit_Eidos->setText(QString::fromStdString(tmpEidos->GetFullEidosName()));
		delete tmpEidos;
	}

	ui.checkBox_alternated->setChecked(CurrentEA->IsCaptionAlternated);
	if(CurrentEA->IsCaptionAlternated)
	{
		Eidos* tmpEidos=new Eidos (this->DB,CurrentEA->AltCaptionEidosID);
		ui.checkBox_alternated->setToolTip(tr("Псевдоним определен в ")+QString::fromStdString(tmpEidos->GetFullEidosName()));
		delete tmpEidos;
	}
	//Проверяем находится ли атрибут в режиме "только чтение" и если это так, блокируем доступ к элементам управления

	QObject::connect(ui.comboBox_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(UserTryToEditEA()));
	QObject::connect(ui.comboBox_DLL_Name, SIGNAL(currentIndexChanged(int)), this, SLOT(UserTryToEditEA()));
        QObject::connect(ui.lineEdit_DLL_Proc, SIGNAL(textChanged(QString)), this ,SLOT(UserTryToEditEA()));
	QObject::connect(ui.comboBox_Species, SIGNAL(currentIndexChanged(int)), this, SLOT(UserTryToEditEA()));
	QObject::connect(ui.radioButton_BFH, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::connect(ui.checkBox_Locked, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::connect(ui.checkBox_Multiple, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::connect(ui.checkBox_NeedList, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::connect(ui.checkBox_Required, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::connect(ui.checkBox_Temporality, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
	QObject::connect(ui.checkBox_Visible, SIGNAL(toggled(bool)), this, SLOT(UserTryToEditEA()));
        QObject::connect(ui.lineEdit_Caption, SIGNAL(textChanged(QString)), this ,SLOT(UserTryToEditEA()));
        QObject::connect(ui.lineEdit_FieldName, SIGNAL(textChanged(QString)), this ,SLOT(UserTryToEditEA()));

	if(ui.tableWidget_EAs->item(ui.tableWidget_EAs->currentRow(),1)->text()==QString(tr("Н/Д")))
	{
		IsEAEditable=false;
		ui.toolButton_ChooseEidos->setEnabled(false);
		ui.toolButton_ChooseHipotesys->setEnabled(false);
		ui.lineEdit_Caption->setReadOnly(true);
		ui.lineEdit_FieldName->setReadOnly(true);
	}
	else
	{
		IsEAEditable=true;
		ui.toolButton_ChooseEidos->setEnabled(true);
		ui.toolButton_ChooseHipotesys->setEnabled(true);
		ui.lineEdit_Caption->setReadOnly(false);
		ui.lineEdit_FieldName->setReadOnly(false);
	}
	CurEAChanged=false;
}
void es_mainwindow::comboTypeChanged(int NewIndex)
{
	//При изменеии типа атрибута следует отображать или скрывать присущие конкретным типам атрибутов свойства
	int EAType=ui.comboBox_Type->itemData(NewIndex,Qt::UserRole).toInt();
	switch (EAType)
			{
				case ft_String:
				case ft_Integer:
				case ft_Boolean:
				case ft_Float:
				case ft_DateTime:
				{
					//Простые типы данных
					ui.groupBox_DLL->setVisible(false);
					ui.groupBox_LNK->setVisible(false);
					break;
				}
				case ft_DLL:
				{
					ui.groupBox_DLL->setVisible(true);
					ui.groupBox_LNK->setVisible(false);
					break;
				}
				case ft_LinkHypotesis:
				{
					ui.lineEdit_Hipotesys->setVisible(false);
					ui.toolButton_ChooseHipotesys->setVisible(false);
					ui.label_LnkHyp->setVisible(false);
					ui.groupBox_DLL->setVisible(false);
					ui.groupBox_LNK->setVisible(true);
					break;
				}
				case ft_LinkPragma:
				{
					ui.lineEdit_Hipotesys->setVisible(true);
					ui.toolButton_ChooseHipotesys->setVisible(true);
					ui.label_LnkHyp->setVisible(true);
					ui.groupBox_DLL->setVisible(false);
					ui.groupBox_LNK->setVisible(true);
					break;
				}
				case ft_Security:
				{
					//Пока не понятно что нужно отображать
					ui.groupBox_DLL->setVisible(false);
					ui.groupBox_LNK->setVisible(false);
					break;
				}

				default:
					throw("Указанный тип данных не поддерживается");
			}
}
void es_mainwindow::Exit()
{
//Выход из формы
    WriteFormWidgetsAppearance();
    this->close();
}
void es_mainwindow::SetAltCaption()
{
//Установка псевдонима заголовка текущего экстраатрибута
    bool ok;
    QString textExp = QInputDialog::getText(this, tr("Изменение псевдонима заголовка текущего экстраатрибута"),	tr("Новый заголовок:"),QLineEdit::Normal,"",&ok);
    if (ok && !textExp.isEmpty()) CurrentEA->SetAlterCaption(textExp.toStdString());
}
void es_mainwindow::ChangeCheckBoxAlterCaption()
{
//Изменение вручную состояния чекбокса псевдонима заголовка
	if(ui.checkBox_alternated->checkState()==Qt::Unchecked)
	{
		if(CurrentEA->AltCaptionEidosID == LocalEidos->GetID())
		{
			if(CurrentEA->IsCaptionAlternated==true)
			{
				int ret = QMessageBox::critical(this, tr("Удаление псевдонима:"),tr("Вы действительно хотите удалить псевдоним заголовка для текущего атрибута?"),QMessageBox::Yes,QMessageBox::No|QMessageBox::Default);
				if(ret==QMessageBox::Yes)
				{
					CurrentEA->DeleteAlterCaption();
					RefreshEAonForm();
				}
				else
					ui.checkBox_alternated->setChecked(CurrentEA->IsCaptionAlternated);
			}
		}
		else
		{
			ui.checkBox_alternated->setChecked(CurrentEA->IsCaptionAlternated);
			QMessageBox::information(this, tr("Удаление псевдонима:"),tr("Удалить псевдоним заголовка для текущего атрибута можно только на том уровне Eodos-а, на котором он был определен"));
		}
	}
	else	//Qt::Checked
	{
		SetAltCaption();
		RefreshEAonForm();
	}
}

void es_mainwindow::FillEAGrid(QTreeWidgetItem* CurItem,int Num)
{
    //Процедура заполняет таблицу с экстраатрибутами, устанавливая текущим элементом элемент, переданный в параметре CurItem, и колонку переданную в параметре Num
	QObject::disconnect(ui.tableWidget_EAs, SIGNAL(currentItemChanged(QTableWidgetItem *, QTableWidgetItem *)), this, SLOT(EAChoosed(QTableWidgetItem*,QTableWidgetItem*)));
	QObject::disconnect(ui.action_AltCaption, SIGNAL(activated()), this, SLOT(SetAltCaption()));
	QObject::disconnect(ui.checkBox_alternated, SIGNAL(clicked(bool)), this, SLOT(ChangeCheckBoxAlterCaption()));
        QObject::disconnect(ui.action_EA_Add, SIGNAL(activated()), this, SLOT(AddOneEA()));

	if(CurEAChanged==true)	//Переход на другой элемент в режиме редактирования свойств атрибута
	{
		int ret = QMessageBox::warning(this, tr("Внимание, не сохраненные изменения"),
										tr("Атрибут был изменен\n"
										   "Желаете сохранить изменения?"),
										QMessageBox::Save | QMessageBox::Discard,
										QMessageBox::Save);
		if(ret==QMessageBox::Save) SaveCurEA();

	}

	ui.tableWidget_EAs->setSortingEnabled(false);
	ui.tableWidget_EAs->clearContents(); //Очищаем внутреннее содержимое таблицы

	Eidos* keep4delete=LocalEidos;

	long id_eidos;
	//Проверяем не впервые ли запущена программа
	if(CurItem==NULL || CurItem==0)
		id_eidos=1;	//Выводим root
	else
		id_eidos=CurItem->text(1).toLong();

	LocalEidos=new Eidos(this->DB,id_eidos);
	int AttrNumber = LocalEidos->Attributes.size();

	ui.tableWidget_EAs->setRowCount(AttrNumber);	//Устанавливаем число строк таблицы по количеству атрибутов

	for(int i=0;i<AttrNumber;i++)
	{
		ExtraAttribute*OneAttrib=(ExtraAttribute*)LocalEidos->Attributes[i];

		QString NodeName=QString::fromStdString(OneAttrib->GetClass_NodeName());
		bool is_locked=false;
		if(NodeName!=ui.EidosTreeWidget->currentItem()->text(0)) is_locked=true;

		for (int j=0;j<numColsInTableEA;j++)	//Создаем элементы таблицы
		{
			QTableWidgetItem* OneItem =new QTableWidgetItem();
			ui.tableWidget_EAs->setItem(i, j, OneItem);
		}

		ui.tableWidget_EAs->item(i, 0)->setText(QString::number(OneAttrib->GetEAID()));	//ID
		ui.tableWidget_EAs->item(i, 2)->setText(QString::fromStdString(OneAttrib->GetEAFieldName()));	//Поле
		ui.tableWidget_EAs->item(i, 3)->setText(QString::fromStdString(OneAttrib->GetEACaption()));	//Заголовок
		ui.tableWidget_EAs->item(i, 4)->setText(GetEATypeName(OneAttrib->type));	//Тип
		if(OneAttrib->belongTo==ExtraAttribute::_theHypotesis)
			ui.tableWidget_EAs->item(i, 5)->setText(tr("Типу"));
		else
			ui.tableWidget_EAs->item(i, 5)->setText(tr("Экземпляру"));

		ui.tableWidget_EAs->item(i, 6)->setText(NodeName);

		//Разбираемся со статусом атрибута
		if(is_locked)
		{
			ui.tableWidget_EAs->item(i, 1)->setIcon(icon_locked);
			ui.tableWidget_EAs->item(i, 1)->setText(tr("Н/Д"));
			ui.tableWidget_EAs->item(i, 1)->setToolTip(tr("Атрибут определен на другом уровне Eidos"));

			QFont currentFont=ui.tableWidget_EAs->item(i,0)->font();
			currentFont.setItalic(true);
			for (int j=0;j<numColsInTableEA;j++)	//Создаем элементы таблицы
				ui.tableWidget_EAs->item(i,j)->setFont(currentFont);
		}
		else
		{
			long RecordsCount=0;
			if(!OneAttrib->Multilnk) RecordsCount=OneAttrib->RealRecordsCount();

			if(RecordsCount>0)
			{
				ui.tableWidget_EAs->item(i, 1)->setIcon(icon_occouped);
				ui.tableWidget_EAs->item(i, 1)->setText(tr("Н/Д"));
				ui.tableWidget_EAs->item(i, 1)->setToolTip(tr("В базе имеется ")+QString::number(RecordsCount)+tr(" записей атрибута"));
			}
			else
			{
				ui.tableWidget_EAs->item(i, 1)->setIcon(icon_editable);
				ui.tableWidget_EAs->item(i, 1)->setText(tr("Дост."));
				ui.tableWidget_EAs->item(i, 1)->setToolTip(tr("Свойства атрибута могут быть изменены"));
			}
		}
	}

	ui.tableWidget_EAs->resizeColumnsToContents();
	ui.tableWidget_EAs->setSortingEnabled(true);
	ui.tableWidget_EAs->setEditTriggers(QAbstractItemView::NoEditTriggers);

	if(!this->IsViewID)	//Отметка снята
		ui.tableWidget_EAs->hideColumn(0);

	if(keep4delete!=NULL) delete keep4delete;
	QObject::connect(ui.tableWidget_EAs, SIGNAL(currentItemChanged(QTableWidgetItem *, QTableWidgetItem *)), this, SLOT(EAChoosed(QTableWidgetItem*,QTableWidgetItem*)));
	QObject::connect(ui.action_AltCaption, SIGNAL(activated()), this, SLOT(SetAltCaption()));
	QObject::connect(ui.checkBox_alternated, SIGNAL(clicked(bool)), this, SLOT(ChangeCheckBoxAlterCaption()));
        QObject::connect(ui.action_EA_Add, SIGNAL(activated()), this, SLOT(AddOneEA()));

	CurEAChanged=false;

        //Устанавливаем текущим элементом первый элемент и первую видимую колонку
        if(Num==0 && (!this->IsViewID))
            ui.tableWidget_EAs->setCurrentCell(0,1);
        else
            ui.tableWidget_EAs->setCurrentCell(0,Num);

}

QString es_mainwindow::GetEATypeName(int typeID)
{
	switch (typeID)
		{
			case ft_String:
				return tr("Строка");
			case ft_Integer:
				return tr("Целое");
			case ft_Boolean:
				return tr("Логическое");
			case ft_Float:
				return tr("Вещественное");
			case ft_DateTime:
				return tr("Дата время");
			case ft_DLL:
				return tr("DLL");
			case ft_LinkHypotesis:
				return tr("Ссылка на тип");
			case ft_LinkPragma:
				return tr("Ссылка на экземпляр");
			case ft_Security:
				return tr("Опреденение доступа");
			default:
				throw(tr("Указанный тип данных не поддерживается"));
		}
}

es_mainwindow::~es_mainwindow()
{

}
void es_mainwindow::ReadFormWidgetsAppearance()
{
	//Процедура считывает из DbETC параметры элементов формы и устанавливает их значения
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName ()).toStdString(),true,-1);
	int w=855,h=825;
	if(MyETC->ParamExists("width")) w=MyETC->ReadInteger("width");
	if(MyETC->ParamExists("height")) h=MyETC->ReadInteger("height");
	this->resize (w,h);

	QList<int> vals;
	if(MyETC->ParamExists("splitter_V\\0")) vals.append(MyETC->ReadInteger("splitter_V\\0")); else vals.append(847);
	if(MyETC->ParamExists("splitter_V\\1")) vals.append(MyETC->ReadInteger("splitter_V\\1")); else vals.append(741);
	ui.splitter_V->setSizes(vals);
	vals.clear();
	if(MyETC->ParamExists("splitter_H\\0")) vals.append(MyETC->ReadInteger("splitter_H\\0")); else vals.append(473);
	if(MyETC->ParamExists("splitter_H\\1")) vals.append(MyETC->ReadInteger("splitter_H\\1")); else vals.append(741);
	ui.splitter_H->setSizes(vals);

	MyETC->CloseKey();
	delete MyETC;
}

void es_mainwindow::WriteFormWidgetsAppearance()
{
	//Процедура записывает в DbETC параметры элементов формы
	platon::DbEtc* MyETC=new platon::DbEtc(this->DB);
	MyETC->OpenKey(QString("FormsAppearance\\"+this->objectName ()).toStdString(),true,-1);
	MyETC->WriteInteger("width", this->width());
	MyETC->WriteInteger("height", this->height());

	QList<int> vals = ui.splitter_V->sizes();
        MyETC->WriteInteger("splitter_V\\0", vals.at(0));
	MyETC->WriteInteger("splitter_V\\1", vals.at(1));
	vals=ui.splitter_H->sizes();
	MyETC->WriteInteger("splitter_H\\0", vals.at(0));
	MyETC->WriteInteger("splitter_H\\1", vals.at(1));


	MyETC->CloseKey();
	delete MyETC;

}

void es_mainwindow::ViewID_Activated()
{
	//Слот для изменения отображения идентификаторов объектов в программе
	if(!this->IsViewID)	//Отметка снята
	{
		for (int i=0;i<ui.EidosTreeWidget->columnCount();i++)
			{
				QString FieldCaption = ui.EidosTreeWidget->headerItem()->text(i);
				if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
					ui.EidosTreeWidget->hideColumn(i);
			}
	}
	else								//Отметка в меню установлена
	{
		for (int i=0;i<ui.EidosTreeWidget->columnCount();i++)
			{
				QString FieldCaption = ui.EidosTreeWidget->headerItem()->text(i);
				if(FieldCaption.indexOf(QRegExp("^ID"))!=-1)
					ui.EidosTreeWidget->showColumn(i);
			}
	}
}

void es_mainwindow::RefreshEAonForm()
{
	//Процедура проводит обновление списка экстраатрибутов и открывает атрибут
	int KeepCol=ui.tableWidget_EAs->currentColumn();
	QString EAID4Find=ui.tableWidget_EAs->item(ui.tableWidget_EAs->currentRow(),0)->text();//EAID

        FillEAGrid(ui.EidosTreeWidget->currentItem(),1);
	QList<QTableWidgetItem *> AnItems=ui.tableWidget_EAs->findItems(EAID4Find,Qt::MatchExactly);
	ui.tableWidget_EAs->setCurrentItem(AnItems.at(0));
	ui.tableWidget_EAs->setCurrentCell(ui.tableWidget_EAs->currentRow(),KeepCol);

}
void es_mainwindow::UserTryToEditEA()
{
	//Процедура проверяет статус экстраатрибута и если атрибут редактируем, то ставит признак изменения иначе выводит
	//сообщение о невозможности корректировки свойств атрибута и обновляет свойства атрибута на ранее установленные
	if(IsEAEditable==true)
        {
		CurEAChanged=true;
                QObject::connect(ui.action_SaveChanges, SIGNAL(activated()), this, SLOT(SaveCurEA()));
                ui.action_SaveChanges->setEnabled(true);
        }
	else
	{
		QMessageBox::information(this, tr("Предупреждение"),tr("Атрибут не может быть изменен, так как ")+ui.tableWidget_EAs->item(ui.tableWidget_EAs->currentRow(),1)->toolTip());
		EAChoosed(ui.tableWidget_EAs->currentItem(),ui.tableWidget_EAs->currentItem());
	}
}
void es_mainwindow::SaveCurEA()
{
	//Процедура сохраняет изменения в текущем экстраатрибуте
	if(CurEAChanged==true)
	{
		if(ui.radioButton_BFH->isChecked())
			CurrentEA->belongTo=ExtraAttribute::_theHypotesis;
		else
			CurrentEA->belongTo=ExtraAttribute::_thePragma;

		CurrentEA->Locked= ui.checkBox_Locked->isChecked();
		CurrentEA->Required=ui.checkBox_Required->isChecked();
		CurrentEA->Visible=ui.checkBox_Visible->isChecked();
		CurrentEA->Temporality=ui.checkBox_Temporality->isChecked();
		CurrentEA->LNK_NeedList=ui.checkBox_NeedList->isChecked();
		CurrentEA->Multilnk=ui.checkBox_Multiple->isChecked();
		CurrentEA->SetEACaption(ui.lineEdit_Caption->text().toStdString());
		CurrentEA->SetEAFieldName(ui.lineEdit_FieldName->text().toStdString());
                CurrentEA->type=ui.comboBox_Type->itemData(ui.comboBox_Type->currentIndex(),Qt::UserRole).toInt();
                CurrentEA->DLL_FileName=ui.comboBox_DLL_Name->currentText().toStdString();
                CurrentEA->DLL_ProcName=ui.lineEdit_DLL_Proc->text().toStdString();

		//определение остальных параметров экстраатрибута
		CurrentEA->Save();
	}
	CurEAChanged=false;
        ui.action_SaveChanges->setEnabled(false);
        QObject::disconnect(ui.action_SaveChanges, SIGNAL(activated()), this, SLOT(SaveCurEA()));
        RefreshEAonForm();
}
void es_mainwindow::AddOneEA()
{
    //Процедура создает новый экстраатрибут для текущего эйдоса. По умолчанию создается самый простой тип - строка
    long id_NewEA=LocalEidos->AddExtraAttribute(QString(tr("Новый экстраатрибут")).toStdString(),platon::ft_String);
    QString Str4search;
    Str4search.setNum(id_NewEA);

    FillEAGrid(ui.EidosTreeWidget->currentItem(),1);
    QList<QTableWidgetItem *> AnItems=ui.tableWidget_EAs->findItems(Str4search,Qt::MatchExactly);
    ui.tableWidget_EAs->setCurrentItem(AnItems.at(0));
    ui.tableWidget_EAs->setCurrentCell(ui.tableWidget_EAs->currentRow(),1);
}
void es_mainwindow::RenameEidos()
{
    //Слот для переименования текущего Eidos-а
    long id_eidos=ui.EidosTreeWidget->currentItem()->text(1).toLong();
    QString CurrentCaption =ui.EidosTreeWidget->currentItem()->text(0);

    bool ok;
    QString textExp = QInputDialog::getText(this, tr("Переименование текущей ветки Eidos"),	tr("Новое имя:"),QLineEdit::Normal,CurrentCaption,&ok);
    if (ok && !textExp.isEmpty())
    {
        Eidos* tmpEidos=new Eidos (this->DB,id_eidos);
        tmpEidos->SetName(textExp.toStdString());
        tmpEidos->Save();
        delete tmpEidos;
    }
    ui.EidosTreeWidget->AttachToDB(this->DB);
    ui.EidosTreeWidget->findNMakeCurrent(id_eidos);
}

void es_mainwindow::AddChildEidos()
{
    //Добавить Потомка к Eidos-у
    long id_eidos;
    long id_parent_eidos=ui.EidosTreeWidget->currentItem()->text(1).toLong();
    bool ok;
    QString textExp = QInputDialog::getText(this, tr("Добавление ветки Eidos"),	tr("Введите имя ветви:"),QLineEdit::Normal,"",&ok);
    if (ok && !textExp.isEmpty())
    {
        Eidos* tmpEidos=new Eidos (this->DB);
        tmpEidos->SetParentID(id_parent_eidos);
        tmpEidos->SetName(textExp.toStdString());
        tmpEidos->Save();
        id_eidos=tmpEidos->GetID();
        delete tmpEidos;
    }
    ui.EidosTreeWidget->AttachToDB(this->DB);
    ui.EidosTreeWidget->findNMakeCurrent(id_eidos);
}
void es_mainwindow::DeleteEidos()
{
    //Удалить текущий элемент Eidos-а
    //Нужно проверить, что у теккущего элемента нет активных потомков
    if(platon::NumberChildofEidos(this->DB,ui.EidosTreeWidget->currentItem()->text(1).toLong())==0)
    {
        int ret = QMessageBox::warning(this, tr("Внимание,критическая операция"),
                                        tr("Вы действительно хотите удалить текущий объект\n"
                                           "КЛАСС?"),
                                        QMessageBox::Yes | QMessageBox::Cancel,
                                        QMessageBox::Cancel);
        if(ret==QMessageBox::Yes)
        {
            platon::DeleteEidosItem(this->DB, ui.EidosTreeWidget->currentItem()->text(1).toLong());
            ui.EidosTreeWidget->AttachToDB(this->DB);
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Внимание"),
                                   tr("Невозможно удалить объект КЛАСС, имеющий потомков"),
                                   QMessageBox::Ok,
                                   QMessageBox::Ok);
    }


}
void es_mainwindow::Start_DLL_Manager()
{
//Запуск менеджера DLL
}
}
