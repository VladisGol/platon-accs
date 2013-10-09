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

#include "pragmamodel.h"

PragmaModel::PragmaModel(platon::Hypotesis* InHyp, WObject * parent)
    : WAbstractTableModel(parent)
{
	//Инициализируем переменные и объекты
	setObjectName("PragmaModel");

    MyHyp=InHyp;

    MyIterator=new platon::iterPragma(InHyp);                             //Создаем итератор по базе

    NumCol=InHyp->HostEidos->PragmaSQL->AttributesList.size();						//Получаем количество полей в запросе
    Buffer.resize(BufferCapacity * (NumCol+1));								//Устанавливаем размер вектора = числу полей экстраатрибутов + ID+HipotesysName

	BufferStartRow=0;														//Начало
	BufferLastRow=ReadToBuffer(BufferStartRow,0,BufferCapacity);			//Читаем из базы первые BufferCapacity записей
	if(BufferLastRow>=BufferCapacity)
		LastRequestedReccount=MyIterator->GetRowCount();
	else
		LastRequestedReccount=BufferLastRow;
}

PragmaModel::~PragmaModel()
{
    delete MyIterator;
}

int PragmaModel::GetOffset(int Row,int Col) const
{
    //Процедура рассчитывает значение индекса, исходя из переданных в параметрах значениях строки и колонки массива
    //возвращается значение элемента массива начиная с 0
    return ((Row)*NumCol+Col);

}
int PragmaModel::ReadToBuffer(int RowInit , int RowInBuffer, int RowCount) const
{
    //Процедура считывает в буфер записи из БД начиная с записи номер которой передан в параметре RowInit
    //вставляет в буфер начиная со строки в буфере RowInBuffer в количестве RowCount строк
    //возвращается число прочитанных записей

    SkipTo(RowInit);	//Перемещаемся к записи с искомым номером

    int ReadedRow=0;

    int ReservedColumns=0;
    ReservedColumns=1;

    while(ReadedRow<RowCount)
    {
        if(MyIterator->Fetched())
        {
            platon::Pragma* OnePragma=new platon::Pragma(MyHyp,MyIterator->GetID());
            Buffer[GetOffset(RowInBuffer+ReadedRow,0)]=std::string(boost::lexical_cast<std::string>(OnePragma->GetID()));

            for(int i=0;i<NumCol;i++)
            {
                boost::any OneValue;

                //Получаем имя поля и его тип по номеру
                std::string curFieldName;
                int EA_type;

                curFieldName = ForEidos->PragmaSQL->AttributesList[i].FieldName;
                EA_type = ForEidos->PragmaSQL->AttributesList[i].FieldType;

                //Получаем ссылку на текущий экстраатрибут в составе рекордсета
                switch(EA_type)
                {
                    case platon::ft_String:
                    {
                        OneValue=WString(OnePragma->GetEAByFieldName(curFieldName)->GetStringValue(),Wt::UTF8);
                        break;
                    }
                    case platon::ft_Integer:
                    {
                        OneValue=OnePragma->GetEAByFieldName(curFieldName)->GetIntValue();
                        break;
                    }
                    case platon::ft_Boolean:
                    {
                        OneValue=OnePragma->GetEAByFieldName(curFieldName)->GetBoolValue();
                        break;
                    }
                    case platon::ft_Float:
                    {
                        OneValue=OnePragma->GetEAByFieldName(curFieldName)->GetFloatValue();
                        break;
                    }
                    case platon::ft_DateTime:
                    {
                        IBPP::Timestamp MyTs;
                        MyTs = OnePragma->GetEAByFieldName(curFieldName)->GetDateTimeValue();
                        Wt::WDate LocalDate = Wt::WDate(MyTs.Year(),MyTs.Month(),MyTs.Day());
                        OneValue=LocalDate.toString();
                        break;
                    }
                    case platon::ft_DLL:
                    case platon::ft_LinkHypotesis:
                    case platon::ft_LinkPragma:
                    {
                        OneValue=WString(OnePragma->GetEAByFieldName(curFieldName)->GetStringValue(),Wt::UTF8);
                        break;
                    }
                    default:
                        throw("Указанный тип данных не поддерживается");

                }

                Buffer[GetOffset(RowInBuffer+ReadedRow,i+ReservedColumns)]=OneValue;
            }
            delete OnePragma;
        }
        else
        {
            break;	// Данные закончились
        }

        ReadedRow++;
        MyIterator->Next();
    }
    return ReadedRow;
}


int PragmaModel::SkipTo(int RowNumber)const
{
    //Функция анализирует в какую позицию необходимо переместиться и перемещает указатель итератора куда надо
    // возвращается значение позиции где расположен итератор
    if(RowNumber==0)
    {
        MyIterator->First();
        return 0;
    }
    if(MyIterator->GetRowNum()-1==RowNumber) return MyIterator->GetRowNum();	//Итератор уже где надо
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
        //Осталось условие по которому итератор уже считал больше записей чем требовалось, необходимо перейти на начало и спуститься ниже
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

int PragmaModel::columnCount(const WModelIndex & index) const
{
    //Процедура возвращает число столбцов (полей)
    int ReservedColumns=0;
    ReservedColumns=1;
    return this->NumCol+ReservedColumns;
}

int PragmaModel::rowCount(const WModelIndex &parent) const
{
    //Процедура возвращает число строк
    return LastRequestedReccount;
}

boost::any PragmaModel::headerData(int section, Wt::Orientation orientation,int role) const
{
    //Процедура выводит значения надписей столбцов и строк

    if (role != Wt::DisplayRole)
             return boost::any();

    if (orientation==Wt::Horizontal)
    {
        int ReservedColumns=1;
        if(section==0) return "ID";
        return WString(this->ForEidos->PragmaSQL->AttributesList[section-ReservedColumns].Caption,Wt::UTF8);
    }
    else
    {
        return WString(std::string(boost::lexical_cast<std::string>(section)));
    }
}

boost::any PragmaModel::data(const WModelIndex &index, int role) const
{
        if (!index.isValid())
             return boost::any();

         if (index.row() >= LastRequestedReccount)
             return boost::any();

         if (role == Wt::DisplayRole)
             return GetDataFromBuffer(index);
         else
             return boost::any();
}

boost::any PragmaModel::GetDataFromBuffer(const WModelIndex &index)const
{
//Функция возвращает значение с индексом, переданном в параметре
    int row=index.row();
    int col=index.column();

    if (row >=BufferStartRow && row <BufferLastRow)	//Строка в диапазоне номеров, вошедших в буфер
    {
        boost::any value = Buffer[GetOffset(row-BufferStartRow,col)];

        if(!value.empty())
            return value;
        else
            return boost::any();
    }
    else
    {
        shiftBuffer(index);							//Смещаем буфер до получения значения index в буфер
        return GetDataFromBuffer(index);			//Рекурсивный вызов процедуры
    }
}
void  PragmaModel::shiftBuffer(const WModelIndex &index) const
{
//Процедура смещает буфер так, чтобы запрашиваемый элемент попал в буфер
    int row=index.row();
    int ShiftVal=BufferCapacity/4;

    if(row<BufferStartRow)	//Значение номера строки меньше чем есть в буфере, смещаемся вверх
    {
        //Вычисляем значение смещения на которое нужно сместить буфер чтобы отобразить значение
        if(row>=BufferStartRow-ShiftVal && BufferStartRow-ShiftVal>=0)	//Считать нужно 1/4 буфера вверх
        {
            //копируем в последнюю строку, строку 0+Delta на протяжении BufferCapacity-Delta строк
            for(int i=BufferCapacity-1;i>=ShiftVal;i--)
                for(int j=0;j<NumCol;j++)
                    Buffer[GetOffset(i,j)]=Buffer[GetOffset(i-ShiftVal,j)];

            int ReadedLines=ReadToBuffer(BufferStartRow-ShiftVal,0,ShiftVal);
            //(int RowInit , int RowInBuffer, int RowCount) const
            BufferLastRow=BufferLastRow-ReadedLines;
            BufferStartRow=BufferStartRow-ShiftVal;
        }
        else//Чтение вверх полного кэша
        {
            int ReadedLines=ReadToBuffer(row,0,BufferCapacity);
            BufferStartRow=row;
            BufferLastRow=row+ReadedLines;
        }
    }
    if(row>=BufferLastRow)	//Значение номера строки больше чем есть в буфере, смещаемся вниз
    {
        //Вычисляем значение смещения на которое нужно сместить буфер чтобы отобразить значение
        if(row<BufferLastRow+ShiftVal)	//Считать нужно 1/4 буфера вниз
        {
            //копируем в нулевую строку, строку 0+Delta на протяжении BufferCapacity-Delta строк
            for(int i=0;i<BufferCapacity-ShiftVal;i++)
                for(int j=0;j<NumCol;j++)
                    Buffer[GetOffset(i,j)]=Buffer[GetOffset(i+ShiftVal,j)];
            int ReadedLines=ReadToBuffer(BufferLastRow,BufferCapacity-ShiftVal,ShiftVal);
            //(int RowInit , int RowInBuffer, int RowCount) const
            BufferStartRow=BufferStartRow+ShiftVal;
            BufferLastRow=BufferLastRow+ReadedLines;
        }
        else	//Чтение вниз полного кэша
        {
            int ReadedLines=ReadToBuffer(row,0,BufferCapacity);
            BufferStartRow = row;
            BufferLastRow = row+ReadedLines;
        }
    }
}

