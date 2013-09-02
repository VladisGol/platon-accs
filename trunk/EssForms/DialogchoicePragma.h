#ifndef CHOICEPRAGMA_H
#define CHOICEPRAGMA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QTreeWidget>
#include <QtGui/QTableView>
#include "HypPragmaMemModel.h"
#include "dbetc.h"
#include "DataClass.h"

namespace platon
{
class ChoicePragma_Dialog: public QDialog
{
	Q_OBJECT
public:
    QGridLayout *gridLayout;
    QTableView *TableView;
    HypPragmaMemModel* Model;
    QSortFilterProxyModel* SFProxyModel;
    QDialogButtonBox *buttonBox;
    ChoicePragma_Dialog(QWidget * parent, platon::Eidos* InEidos, long ID_in);
    long Out_value;
    bool find(long ID_searchfor);

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
