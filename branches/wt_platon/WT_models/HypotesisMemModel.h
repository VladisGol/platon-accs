/*
 * HypotesisMemModel.h
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */
#ifndef HYPOTESISMEMMODEL_H_
#define HYPOTESISMEMMODEL_H_

#include "AbstractMemHypModel.h"
#include "EssentialClass.h"
#include <Wt/WAbstractTableModel>


using namespace Wt;

namespace platon
{

class HypotesisMemModel: public AbstractMemHypModel
{
public:
    HypotesisMemModel(Eidos*, WObject * parent);
    boost::any headerData(int section, Wt::Orientation orientation,int role = Wt::DisplayRole) const;
    void ReadToBuffer() const;
protected:
	virtual ExtraAttribute* getEAFromEidos(int i) const ;		//Процедура возвращает ссылку на экстраатрибут по номеру
    HypotesisMemModel(WObject * parent):AbstractMemHypModel(parent){;};//Скрытый конструктор для потомков использующих собственный конструктор
};

}

#endif /* HYPOTESISMEMMODEL_H_ */
