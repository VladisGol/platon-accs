

#ifndef EA_OneFrame_H
#define EA_OneFrame_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QLineEdit>
#include "EssentialClass.h"

namespace platon
{
class EA_OneFrame :public QFrame
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QFrame *frame;
    QToolButton *toolButton;
    QWidget* EditableWidget;
    QGridLayout* gridLayoutInFrame;
    AssociatedExtraAttribute * EAA;	//Ассоциированный экстраатрибут с гипотезой

    EA_OneFrame(QWidget *parent, AssociatedExtraAttribute* InEAA);
    ~EA_OneFrame();
protected:
    void fillVisibleWidget();	//Заполнение значением
    QVariant KeepValue;

};
}

#endif
