/*
 * HypotesysMemModel.cpp
 *
 *  Created on: 10.08.2008
 *      Author: slava
 */

#include "LnkComboBox.h"


namespace platon
{

LnkHypComboBox::LnkHypComboBox(Eidos* InEidos, QWidget * parent)
					:QComboBox(parent)
{
	setObjectName("LnkHypComboBox");
	MyEidos =InEidos;
	MyIter= new iterHypotesis(MyEidos);

	this->clear();

	MyIter->First();
	while(MyIter->Fetched())
	{
		this->addItem(tr(MyIter->GetTitle().c_str()),QVariant::fromValue(MyIter->GetID()));
		MyIter->Next();
	}
}

int LnkHypComboBox::SetCurrentIndexByID(long ID)
{
	int IndexNumber=this->findData(QVariant::fromValue(ID),Qt::UserRole,Qt::MatchExactly);
	this->setCurrentIndex(IndexNumber);
	return IndexNumber;
}
long LnkHypComboBox::GetCurrentID()
{
	return this->itemData(this->currentIndex(),Qt::UserRole).toInt();
}

LnkHypComboBox::~LnkHypComboBox()
{
	delete MyIter;
}

LnkHypPragmaComboBox::LnkHypPragmaComboBox(Eidos* InEidos, QWidget * parent)
					:QComboBox(parent)
{
	setObjectName("LnkHypPragmaComboBox");
	MyEidos =InEidos;

	MyIter= new iterHypPragma(MyEidos);

	this->clear();

	MyIter->First();
	while(MyIter->Fetched())
	{
		this->addItem(tr(MyIter->GetTitle().c_str()),QVariant::fromValue(MyIter->GetID()));
		MyIter->Next();
	}
}

int LnkHypPragmaComboBox::SetCurrentIndexByID(long ID)
{
	int IndexNumber=this->findData(QVariant::fromValue(ID),Qt::UserRole,Qt::MatchExactly);
	this->setCurrentIndex(IndexNumber);
	return IndexNumber;
}
long LnkHypPragmaComboBox::GetCurrentID()
{
	return this->itemData(this->currentIndex(),Qt::UserRole).toInt();
}

LnkHypPragmaComboBox::~LnkHypPragmaComboBox()
{
	delete MyIter;
}

}

