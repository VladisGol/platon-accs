

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
#include "EssentialClass.h"

namespace platon
{
QDateTime IBPPTimestamp2QDateTime(IBPP::Timestamp InVal);
IBPP::Timestamp QDateTime2IBPPTimestamp(QDateTime InVal);

class EA_OneFrame :public QFrame
{
public:

    EA_OneFrame(QWidget *parent, AssociatedExtraAttribute* InEAA);
    void Save();
    ~EA_OneFrame();
public slots:
	void CallDllRoutine();
protected:
    void fillVisibleWidget();	//Заполнение значением
    QVariant KeepValue;

    QHBoxLayout *topLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QFrame *frame;
    QToolButton *toolButton;
    QWidget* EditableWidget;
    QGridLayout* gridLayoutInFrame;
    AssociatedExtraAttribute * EAA;	//Ассоциированный экстраатрибут с гипотезой
    //QAction DllCalling;

};
}




#endif
