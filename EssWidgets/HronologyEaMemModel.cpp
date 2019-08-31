#include "HronologyEaMemModel.h"


namespace platon
{

HronologyEaMemModel::HronologyEaMemModel(AssociatedExtraAttribute* OneAEA, QObject * parent)
					:AbstractMemHypModel(parent)
{
	// TODO Auto-generated constructor stub
	setObjectName("HronologyEaMemModel");

	ForEidos =OneAEA->OwnerHypotesis->HostEidos;
	LocalAEA=OneAEA;

	NumCol=2;																//Фиксированное значение
	ReservedColumns=1;														//Одно зарезервированное поле ID

	Id_records = new QVector <long>;										//Выделяем необходимую память
	FieldsInModel= new QVector <QMap<long,QVariant>*>;
	for(int i=0;i<NumCol;i++) FieldsInModel->append(new QMap<long,QVariant>);
	KeyIterator=new iterTemporalityListofOneEA(OneAEA);										//Выставляем итератор ключей записей
	ReadToBuffer();															//Считываем значения в буфер

}

QVariant HronologyEaMemModel::headerData(int section, Qt::Orientation orientation,int role) const
{
	//Процедура выводит значения надписей столбцов и строк

	if (role != Qt::DisplayRole)
	         return QVariant();

	if (orientation==Qt::Horizontal)
	{
		if(section==0)
			return tr("ID");
		if(section==1)
			return tr("Дата-время");
		if(section==2)
			return tr("Значение");
	}
	else
	{
		return QString::number(section);
	}
}
void HronologyEaMemModel::ReadToBuffer() const
{
	//Процедура считывает в буфер по полям записи из БД
	//Заполняем значения в массиве идентификаторов записей

	Id_records->clear();
	KeyIterator->First();
	LastRequestedReccount=0;
	QMap<long,QVariant>* DTMap = FieldsInModel->at(0);//Нулевое поле - дата-время
	QMap<long,QVariant>* TitleMap = FieldsInModel->at(1);//Первое  поле - Наименование

	while(KeyIterator->Fetched())
	{
		int idrecord=KeyIterator->GetID();
		QVariant Val=ReadValuefromRecord();
		QVariant DT_Val=platon::IBPPTimestamp2QDateTime(((iterTemporalityListofOneEA*)KeyIterator)->GetGateTime());
		Id_records->append(idrecord);
		DTMap->insert(idrecord, DT_Val);
		TitleMap->insert(idrecord, Val);
		LastRequestedReccount++;
		KeyIterator->Next();
	}
}

QVariant HronologyEaMemModel::ReadValuefromRecord() const
{
	QVariant OneValue;

	switch(LocalAEA->EA->type)
	{
		case platon::ft_String:
		{
			std::string tmpValue;
			KeyIterator->LocalST->Get("MEANING",tmpValue);
			OneValue=tr(tmpValue.c_str());
			break;
		}
		case platon::ft_Integer:
		{
			int tmpValue;
			KeyIterator->LocalST->Get("MEANING",(int32_t*)&tmpValue);
			OneValue=tmpValue;
			break;
		}
		case platon::ft_Boolean:
		{
			bool tmpValue;
			KeyIterator->LocalST->Get("MEANING",tmpValue);
			OneValue=tmpValue;
			break;
		}
		case platon::ft_Float:
		{
			float tmpValue;
			KeyIterator->LocalST->Get("MEANING",tmpValue);
			OneValue=tmpValue;
			break;
		}
		case platon::ft_DateTime:
		{
			IBPP::Timestamp MyTs;
			KeyIterator->LocalST->Get("MEANING",MyTs);
			OneValue=platon::IBPPTimestamp2QDateTime(MyTs);
			break;
		}
		case platon::ft_DLL:
		case platon::ft_LinkHypotesis:
		case platon::ft_LinkPragma:
		{
			std::string tmpValue;
			KeyIterator->LocalST->Get("MEANING",tmpValue);
			OneValue=tr(tmpValue.c_str());

			break;
		}
		default:
			throw("Указанный тип данных не поддерживается");

	}
	return OneValue;
}


}
