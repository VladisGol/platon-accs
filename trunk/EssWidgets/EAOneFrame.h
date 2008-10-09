

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


class EA_OneFrame :public QFrame
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QFrame *frame;
    QToolButton *toolButton;

    EA_OneFrame(QWidget *parent);
    ~EA_OneFrame();

};


#endif
