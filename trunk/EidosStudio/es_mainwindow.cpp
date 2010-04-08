#include "es_mainwindow.h"

namespace platon
{
es_mainwindow::es_mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
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

	LocalEidos=NULL;

	DataClass* DTL=platon::GetDataModule(this);
	this->DB=DTL->DB;
	this->IsViewID=DTL->ViewIDs;

	QObject::connect(ui.action_quit, SIGNAL(activated()), this, SLOT(Exit()));

	ui.EidosTreeWidget->SetSpecies("ALL");
	ui.EidosTreeWidget->AttachToDB(this->DB);
	ReadFormWidgetsAppearance();
	ViewID_Activated();

    QObject::connect(ui.EidosTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem* ,int)), this, SLOT(FillEAGrid(QTreeWidgetItem*,int)));
    QObject::connect(ui.tableWidget_EAs, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(EAChoosed(QTableWidgetItem*)));
    QObject::connect(ui.tableWidget_EAs, SIGNAL(itemActivated(QTableWidgetItem*)), this, SLOT(EAChoosed(QTableWidgetItem*)));
    QObject::connect(ui.comboBox_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(comboTypeChanged(int)));


}
void es_mainwindow::EAChoosed(QTableWidgetItem*CurElement)
{
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
	ui.lineEdit_Caption->setText(QString::fromStdString(CurrentEA->GetEACaption()));
	ui.lineEdit_FieldName->setText(QString::fromStdString(CurrentEA->GetEAFieldName()));

}
void es_mainwindow::comboTypeChanged(int NewIndex)
{
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
				case ft_LinkPragma:
				{
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
	WriteFormWidgetsAppearance();
	this->close();
}

void es_mainwindow::FillEAGrid(QTreeWidgetItem* CurItem,int Num)
{
	ui.tableWidget_EAs->setSortingEnabled(false);
	ui.tableWidget_EAs->clear(); //Очищаем содержимое таблицы

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
	ui.tableWidget_EAs->setColumnCount(numColsInTableEA);


	for (int j=0;j<numColsInTableEA;j++)
	{
		QTableWidgetItem* OneItem =new QTableWidgetItem();
		ui.tableWidget_EAs->setHorizontalHeaderItem(j, OneItem);
	}
	QStringList labels;
	labels << tr("ID")<<tr("Статус")<<tr("Поле")<<tr("Заголовок")<<tr("Тип")<<tr("Принадлежит")<<tr("Eidos определения");
	ui.tableWidget_EAs->setHorizontalHeaderLabels (labels);

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
				throw("Указанный тип данных не поддерживается");
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


}
