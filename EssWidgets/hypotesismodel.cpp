/****************************************************************************
**
** Trolltech hereby grants a license to use the Qt/Eclipse Integration
** plug-in (the software contained herein), in binary form, solely for the
** purpose of creating code to be used with Trolltech's Qt software.
**
** Qt Designer is licensed under the terms of the GNU General Public
** License versions 2.0 and 3.0 ("GPL License"). Trolltech offers users the
** right to use certain no GPL licensed software under the terms of its GPL
** Exception version 1.2 (http://trolltech.com/products/qt/gplexception).
**
** THIS SOFTWARE IS PROVIDED BY TROLLTECH AND ITS CONTRIBUTORS (IF ANY) "AS
** IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
** TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
** PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
** OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** Since we now have the GPL exception I think that the "special exception
** is no longer needed. The license text proposed above (other than the
** special exception portion of it) is the BSD license and we have added
** the BSD license as a permissible license under the exception.
**
****************************************************************************/

#include "hypotesismodel.h"

HypotesisModel::HypotesisModel(platon::Eidos* InEidos, QWidget *parent)
    : QAbstractTableModel(parent)
{
	//ui.setupUi(this);
	//�������������� ���������� � �������
	setObjectName("HypotesisModel");
	ForEidos =InEidos;
	MyIterator=new platon::iterHypotesis(InEidos);							//������� �������� �� ����
	//MyIterator->SQL_string=InEidos->HypotesisSQL->SQLString();			//� ��� �������� ������ ������� �� ������ �� Eidos-�

	NumCol=InEidos->HypotesisSQL->AttributesList.size();					//�������� ���������� ����� � �������
	Buffer.resize(BufferCapacity * (NumCol+1));								//������������� ������ ������� = ����� ����� ��������������� + ID

	BufferStartRow=0;														//������
	BufferLastRow=ReadToBuffer(BufferStartRow,0,BufferCapacity);			//������ �� ���� ������ BufferCapacity �������
	if(BufferLastRow>=BufferCapacity)
		LastRequestedReccount=MyIterator->GetRowCount();
	else
		LastRequestedReccount=BufferLastRow;
}

HypotesisModel::~HypotesisModel()
{
	delete MyIterator;
}

int HypotesisModel::GetOffset(int Row,int Col) const
{
	//��������� ������������ �������� �������, ������ �� ���������� � ���������� ��������� ������ � ������� �������
	//������������ �������� �������� ������� ������� � 0
	return ((Row)*NumCol+Col);

}
int HypotesisModel::ReadToBuffer(int RowInit , int RowInBuffer, int RowCount) const
{
	//��������� ��������� � ����� ������ �� �� ������� � ������ ����� ������� ������� � ��������� RowInit
	//��������� � ����� ������� �� ������ � ������ RowInBuffer � ���������� RowCount �����
	//������������ ����� ����������� �������

	SkipTo(RowInit);	//������������ � ������ � ������� �������

	int ReadedRow=0;

	int ReservedColumns=0;
	if(this->objectName()=="HypotesisModel") ReservedColumns=1;
	if(this->objectName()=="PragmaModel") ReservedColumns=2;

	while(ReadedRow<RowCount)
	{
		if(MyIterator->Fetched())
		{
			MyHyp= GetHypotesys(MyIterator->GetID());
			if(ReservedColumns>0) Buffer[GetOffset(RowInBuffer+ReadedRow,0)]=QString::number(MyHyp->GetID());
			if(ReservedColumns==2) Buffer[GetOffset(RowInBuffer+ReadedRow,1)]=QString::fromStdString(MyHyp->GetHypotesName());

			for(int i=0;i<NumCol;i++)
			{
				QVariant OneValue;

				//�������� ��� ���� � ��� ��� �� ������
				std::string curFieldName;
				int EA_type;
				GetFieldNamenType(i,curFieldName,EA_type);

				//�������� ������ �� ������� ������������� � ������� ����������
				switch(EA_type)
				{
					case platon::ft_String:
					{
						OneValue=tr(MyHyp->GetEAByFieldName(curFieldName)->GetStringValue().c_str());
						break;
					}
					case platon::ft_Integer:
					{
						OneValue=MyHyp->GetEAByFieldName(curFieldName)->GetIntValue();
						break;
					}
					case platon::ft_Boolean:
					{
						OneValue=MyHyp->GetEAByFieldName(curFieldName)->GetBoolValue();
						break;
					}
					case platon::ft_Float:
					{
						OneValue=MyHyp->GetEAByFieldName(curFieldName)->GetFloatValue();
						break;
					}
					case platon::ft_DateTime:
					{
						IBPP::Timestamp MyTs =MyHyp->GetEAByFieldName(curFieldName)->GetDateTimeValue();
						QString year=QString::number(MyTs.Year());
						QString month=QString("0"+QString::number(MyTs.Month())).right(2);
						QString day=QString("0"+QString::number(MyTs.Day())).right(2);
						QDateTime LocAlValue=QDateTime::fromString(year+month+day,"yyyyMMdd");
						LocAlValue.addSecs(MyTs.Hours()*3600+MyTs.Minutes()*60+MyTs.Seconds());

						OneValue=LocAlValue;
						break;
					}
					case platon::ft_RB:
					case platon::ft_DLL:
					case platon::ft_LinkHypotesis:
					case platon::ft_LinkPragma:
					{
						OneValue=tr(MyHyp->GetEAByFieldName(curFieldName)->GetStringValue().c_str());
						break;
					}
					default:
						throw("��������� ��� ������ �� ��������������");

				}

				Buffer[GetOffset(RowInBuffer+ReadedRow,i+ReservedColumns)]=OneValue;

			}
			DeleteHypotesis(MyHyp);
		}
		else
		{
			break;	// ������ �����������
		}

		ReadedRow++;
		MyIterator->Next();
	}
	return ReadedRow;
}

platon::Hypotesis* HypotesisModel::GetHypotesys(const long id) const
{
	return new platon::Hypotesis(ForEidos,id);
}
void HypotesisModel::DeleteHypotesis(platon::Hypotesis*Fd) const
{
	delete Fd;
}

void HypotesisModel::GetFieldNamenType(const int i,std::string &fname,int &ftype) const
{
	fname = ForEidos->HypotesisSQL->AttributesList[i].FieldName;
	ftype = ForEidos->HypotesisSQL->AttributesList[i].FieldType;
}


int HypotesisModel::SkipTo(int RowNumber)const
{
	//������� ����������� � ����� ������� ���������� ������������� � ���������� ��������� ��������� ���� ����
	// ������������ �������� ������� ��� ���������� ��������
	if(RowNumber==0)
	{
		MyIterator->First();
		return 0;
	}
	if(MyIterator->GetRowNum()-1==RowNumber) return MyIterator->GetRowNum();	//�������� ��� ��� ����
	if(MyIterator->GetRowNum()-1<RowNumber)
	{
		while(MyIterator->GetRowNum()-1<RowNumber)
		{
			MyIterator->Next();
			if(!MyIterator->Fetched()) break;
		}
		return MyIterator->GetRowNum()-1;
	}
	else
	{
		//�������� ������� �� �������� �������� ��� ������ ������ ������� ��� �����������, ���������� ������� �� ������ � ���������� ����
		MyIterator->First();
		while(MyIterator->GetRowNum()-1<RowNumber)
		{
			MyIterator->Next();
			if(!MyIterator->Fetched())
			{
				break;
			}
		}
		return MyIterator->GetRowNum()-1;
	}
}

int HypotesisModel::columnCount(const QModelIndex & index) const
{
	//��������� ���������� ����� �������� (�����)
	int ReservedColumns=0;
	if(this->objectName()=="HypotesisModel") ReservedColumns=1;
	if(this->objectName()=="PragmaModel") ReservedColumns=2;

	return this->NumCol+ReservedColumns;
}

int HypotesisModel::rowCount(const QModelIndex &parent) const
{
	//��������� ���������� ����� �����
	return LastRequestedReccount;
}

QVariant HypotesisModel::headerData(int section, Qt::Orientation orientation,int role) const
{
	//��������� ������� �������� �������� �������� � �����

	if (role != Qt::DisplayRole)
	         return QVariant();

	if (orientation==Qt::Horizontal)
	{
		int ReservedColumns=0;
		if(this->objectName()=="HypotesisModel")
		{
			ReservedColumns=1;
			if(section==0) return "ID";
		}
		if(this->objectName()=="PragmaModel")
		{
			ReservedColumns=2;
			if(section==0) return "ID";
			if(section==1) return "Hipotesys Name";
		}
		return tr(this->ForEidos->HypotesisSQL->AttributesList[section-ReservedColumns].Caption.c_str());
	}
	else
	{
		return QString::number(section);
	}
}

QVariant HypotesisModel::data(const QModelIndex &index, int role) const
{
	 	if (!index.isValid())
	         return QVariant();

	     if (index.row() >= LastRequestedReccount)
	         return QVariant();

	     if (role == Qt::DisplayRole)
	         return GetDataFromBuffer(index);
	     else
	         return QVariant();
}

QVariant HypotesisModel::GetDataFromBuffer(const QModelIndex &index)const
{
//������� ���������� �������� � ��������, ���������� � ���������
	int row=index.row();
	int col=index.column();


	if (row >=BufferStartRow && row <BufferLastRow)	//������ � ��������� �������, �������� � �����
	{
		return Buffer[GetOffset(row-BufferStartRow,col)];
	}
	else
	{
		shiftBuffer(index);							//������� ����� �� ��������� �������� index � �����
		return GetDataFromBuffer(index);			//����������� ����� ���������
	}
}
void  HypotesisModel::shiftBuffer(const QModelIndex &index) const
{
//��������� ������� ����� ���, ����� ������������� ������� ����� � �����
	int row=index.row();
	int ShiftVal=BufferCapacity/4;

	if(row<BufferStartRow)	//�������� ������ ������ ������ ��� ���� � ������, ��������� �����
	{
		//��������� �������� �������� �� ������� ����� �������� ����� ����� ���������� ��������
		if(row>=BufferStartRow-ShiftVal && BufferStartRow-ShiftVal>=0)	//������� ����� 1/4 ������ �����
		{
			//�������� � ��������� ������, ������ 0+Delta �� ���������� BufferCapacity-Delta �����
			for(int i=BufferCapacity-1;i>=ShiftVal;i--)
				for(int j=0;j<NumCol;j++)
					Buffer[GetOffset(i,j)]=Buffer[GetOffset(i-ShiftVal,j)];

			int ReadedLines=ReadToBuffer(BufferStartRow-ShiftVal,0,ShiftVal);
			//(int RowInit , int RowInBuffer, int RowCount) const
			BufferLastRow=BufferLastRow-ReadedLines;
			BufferStartRow=BufferStartRow-ShiftVal;
		}
		else//������ ����� ������� ����
		{
			int ReadedLines=ReadToBuffer(row,0,BufferCapacity);
			BufferStartRow=row;
			BufferLastRow=row+ReadedLines;
		}
	}
	if(row>=BufferLastRow)	//�������� ������ ������ ������ ��� ���� � ������, ��������� ����
	{
		//��������� �������� �������� �� ������� ����� �������� ����� ����� ���������� ��������
		if(row<BufferLastRow+ShiftVal)	//������� ����� 1/4 ������ ����
		{
			//�������� � ������� ������, ������ 0+Delta �� ���������� BufferCapacity-Delta �����
			for(int i=0;i<BufferCapacity-ShiftVal;i++)
				for(int j=0;j<NumCol;j++)
					Buffer[GetOffset(i,j)]=Buffer[GetOffset(i+ShiftVal,j)];
			int ReadedLines=ReadToBuffer(BufferLastRow,BufferCapacity-ShiftVal,ShiftVal);
			//(int RowInit , int RowInBuffer, int RowCount) const
			BufferStartRow=BufferStartRow+ShiftVal;
			BufferLastRow=BufferLastRow+ReadedLines;
		}
		else	//������ ���� ������� ����
		{
			int ReadedLines=ReadToBuffer(row,0,BufferCapacity);
			BufferStartRow = row;
			BufferLastRow = row+ReadedLines;
		}
	}
}
