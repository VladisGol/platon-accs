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

#ifndef PRAGMAMODEL_H
#define PRAGMAMODEL_H

#include <Wt/WAbstractTableModel>
#include <Wt/WDate>
#include <Wt/WString>
#include <Wt/WWidget>
#include "EssentialClass.h"


using namespace Wt;
class PragmaModel : public WAbstractTableModel
{

public:
    PragmaModel(platon::Hypotesis*, WObject * parent);
    ~PragmaModel();

    int rowCount(const WModelIndex &parent = WModelIndex())const;
    int columnCount(const WModelIndex & index) const;
    boost::any data(const WModelIndex &index, int role) const;
    boost::any headerData(int section, Wt::Orientation orientation,int role = Wt::DisplayRole) const;
    boost::any GetDataFromBuffer(const WModelIndex &index) const;

protected:
    static const int BufferCapacity =100;
    platon::Eidos* ForEidos;
    platon::pIterator* MyIterator;	// Итератор для перемещения по БД

    mutable std::vector <boost::any> Buffer;			// Массив элементов для сохранения кэша записей из БД

    int GetOffset(int Row,int Col) const;		// Расчет смещения в массиве
    mutable int NumCol;							// Число полей в запросе
    mutable int BufferStartRow;					// Номер записи базы соответствующей первой в буфере
    mutable int BufferLastRow;					// Номер записи базы соответствующей последней в буфере
    mutable int LastRequestedReccount;
    int ReadToBuffer(int RowInit, int RowInBuffer, int RowCount)const;		// Процедура считывает в буфер записи начиная с RowInit записи
    int SkipTo(int RowInit)const;			// Процедура перемещается в итераторе на строку с заданным номером, возвращая номер строки в которую удалось переместиться

    void shiftBuffer(const WModelIndex &index) const;
    mutable platon::Hypotesis* MyHyp;
};

#endif // PRAGMAMODEL_H
