/*
 * HypotesisMemModel.h
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */
#ifndef LNKCOMBOBOX_H_
#define LNKCOMBOBOX_H_

#include "EssentialClass.h"
#include <QComboBox>



namespace platon
{

class LnkHypComboBox: public QComboBox
{
	Q_OBJECT

public:
	LnkHypComboBox(Eidos* InEidos, QWidget * parent);
	~LnkHypComboBox();
	int SetCurrentIndexByID(long ID);
	long GetCurrentID();
protected:
	Eidos* MyEidos;
	iterHypotesis* MyIter;

};

class LnkHypPragmaComboBox: public QComboBox
{
	Q_OBJECT

public:
	LnkHypPragmaComboBox(Eidos* InEidos, QWidget * parent);
	~LnkHypPragmaComboBox();
	int SetCurrentIndexByID(long ID);
	long GetCurrentID();
protected:
	Eidos* MyEidos;
	iterHypPragma* MyIter;
};

}

#endif
