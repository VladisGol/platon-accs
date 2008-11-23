#ifndef CHOICEEIDOS_H
#define CHOICEEIDOS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QTreeWidget>
#include "qeidostreewidget.h"

class ChoiceEidos_Dialog:public QDialog
{
    Q_OBJECT
public:
    QGridLayout *gridLayout;
    QEidosTreeWidget *treeWidget;
    QDialogButtonBox *buttonBox;
    ChoiceEidos_Dialog(QWidget * parent,IBPP::Database InDB, long ID_in);
    long Out_value;
public slots:
	void ExitWithAccept();
};

#endif
