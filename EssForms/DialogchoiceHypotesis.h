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
#include <QtGui/QTableView>
#include "HypotesisMemModel.h"
#include "dbetc.h"
#include "DataClass.h"

namespace platon
{
class ChoiceHypotesis_Dialog: public QDialog
{
	Q_OBJECT
public:
    QGridLayout *gridLayout;
    QTableView *TableView;
    HypotesisMemModel* Model;
    QSortFilterProxyModel* SFProxyModel;
    QDialogButtonBox *buttonBox;
    ChoiceHypotesis_Dialog(QWidget * parent, platon::Eidos* InEidos, int ID_in);
    int Out_value;
    bool find(int ID_searchfor);

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
