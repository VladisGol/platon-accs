#ifndef HRONOLOGYEAMEMMODEL_H
#define HRONOLOGYEAMEMMODEL_H

#include "AbstractMemHypModel.h"
#include "EssentialClass.h"
#include "commonroutines.h"


namespace platon
{

class HronologyEaMemModel: public platon::AbstractMemHypModel
{
public:
	HronologyEaMemModel(AssociatedExtraAttribute* OneAEA, QObject * parent);
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    void ReadToBuffer() const;
    AssociatedExtraAttribute* LocalAEA;
    QVariant ReadValuefromRecord() const;

};

}

#endif
