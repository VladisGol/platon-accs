#ifndef CHOICEHYPOTESIS_H
#define CHOICEHYPOTESIS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QTreeWidget>


class ChoiceHypotesis_Dialog
{
public:
    QGridLayout *gridLayout;
    QTreeWidget *treeWidget;
    QDialogButtonBox *buttonBox;
    ChoiceHypotesis_Dialog();
};

#endif
