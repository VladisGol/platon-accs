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
#include <QtGui/QDateTimeEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QMessageBox>
#include <QtGui/QCalendarWidget>
#include <QtGui/QIcon>
#include "EssentialClass.h"
#include "DialogchoiceEidos.h"
#include "DialogchoiceHypotesis.h"
#include "DialogchoicePragma.h"
#include "DialogEditHronologyEA.h"
#include "commonroutines.h"
#include "DataClass.h"
#include <QLibrary>
#include "LnkComboBox.h"

namespace platon
{

class EA_OneFrame :public QFrame
{
    Q_OBJECT
public:
    EA_OneFrame(QWidget *parent, AssociatedExtraAttribute* InEAA);
    void Save();
    DataClass* MyDCl;
public slots:
	void CallDllRoutine();
	void LNKClick();
	void HronologyClick();
protected:
    void fillVisibleWidget();	//Заполнение значением
    QVariant KeepValue;
    QHBoxLayout *topLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QFrame *frame;
    QToolButton *LNKButton;
    QToolButton* HronologyButton;
    QToolButton* DLLButton;
    QWidget* EditableWidget;
    QGridLayout* gridLayoutInFrame;
    AssociatedExtraAttribute * EAA;	//Ассоциированный экстраатрибут с гипотезой
};

}




#endif
