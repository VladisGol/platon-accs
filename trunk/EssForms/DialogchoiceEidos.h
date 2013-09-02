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
#include "dbetc.h"
#include "DataClass.h"

namespace platon
{

class ChoiceEidos_Dialog:public QDialog
{
    Q_OBJECT
public:
	QVBoxLayout *VBoxLayout;
    QEidosTreeWidget *treeWidget;
    QDialogButtonBox *buttonBox;
    QCheckBox*checkBox;
    ChoiceEidos_Dialog(QWidget * parent,QString Species, long ID_in);
    long Out_value;
    bool find(long ID_searchfor);

public slots:
	void ExitWithAccept();
	void collapseExp(int);

protected:
    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();
    IBPP::Database DB;
    void SetViewID(bool IsView);
    DataClass* DTL;
};
}
#endif
