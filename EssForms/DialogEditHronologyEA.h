#ifndef DIALOGHRONOLOGYEA_H
#define DIALOGHRONOLOGYEA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QTreeWidget>
#include <QtGui/QTableView>
#include "HronologyEaMemModel.h"
#include "dbetc.h"
#include "DataClass.h"

namespace platon
{
class HronologyEaMemModel;
class DialogEditHronologyEA: public QDialog
{
	Q_OBJECT
public:
    QGridLayout *gridLayout;
    QTableView *TableView;
    HronologyEaMemModel *Model;
    QDialogButtonBox *buttonBox;
    DialogEditHronologyEA(QWidget * parent, AssociatedExtraAttribute* OneAEA);
    int Out_value;

public slots:
	void ExitWithAccept();

protected:
    void ReadFormWidgetsAppearance();
    void WriteFormWidgetsAppearance();
    IBPP::Database DB;
    void SetViewID(bool IsView);
    DataClass* DTL;
};

}
#endif
