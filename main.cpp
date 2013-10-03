#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WDate>
#include <Wt/WTree>
#include <Wt/WTreeNode>
#include <Wt/WIconPair>
#include <Wt/WTreeTable>
#include <Wt/WTreeTableNode>
#include <Wt/WTableView>
#include <Wt/WGridLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WTable>
#include <map>
#include <string>
#include <boost/lexical_cast.hpp>
#include "WT_models/HypotesisMemModel.h"
#include "WT_models/PragmaMemModel.h"

#include "ibpp/core/ibpp.h"
#include "EssentialClass.h"
namespace Wt
{
class WTPlatonNode :public Wt::WTreeNode
{
    public:
    long id_Eidos;
    WTPlatonNode(const WString& labelText,
                 WIconPair *labelIcon = 0,
                 WTreeNode *parentNode = 0);
};

WTPlatonNode::WTPlatonNode(const WString& Wname, WIconPair* IconPair ,WTreeNode *i):WTreeNode(Wname, IconPair ,i)
{}
};

class PlatonViewer : public Wt::WApplication
{
    public:
    PlatonViewer(const Wt::WEnvironment& env);
    Wt::WText *createTitle(const Wt::WString& title);
    private:
    platon::Eidos* MyEidos;
    platon::Hypotesis* Myhypotesys;

    platon::HypotesisMemModel* HypModel;
    platon::PragmaMemModel* PrModel;

    void ChooseTheEidos();
    void FillHypotesysView();
    void FillPragmaView(const Wt::WModelIndex& item);

    Wt::WContainerWidget *w;
    Wt::WGridLayout *layout;
    Wt::WTree *tree;
    Wt::WVBoxLayout *vbox_Eidos;
    Wt::WVBoxLayout *vbox_Hip;
    Wt::WVBoxLayout *vbox_Pragma;

    Wt::WTableView * HTView;
    Wt::WTableView * PrView;
    IBPP::Database DB;
};


PlatonViewer::PlatonViewer(const Wt::WEnvironment& env)
: Wt::WApplication(env)
{
    MyEidos=NULL;
    Myhypotesys=NULL;
    HypModel=NULL;
    PrModel=NULL;

    DB = IBPP::DatabaseFactory("localhost" , "OASUP","sysdba","masterkey",
    "",//Role
    "UTF8",//codepage
    "DEFAULT CHARACTER SET RUSSIAN");//Доп параметры
    DB->Connect();

    platon::iterEidos* MyEidosIter= new  platon::iterEidos(DB,"ALL");

    //Настраиваем виджет
    w = root();
    /*
     * The main layout is a 3x2 grid layout.
     */
    layout = new Wt::WGridLayout();

    tree = new Wt::WTree();
    tree->setSelectionMode(Wt::SingleSelection);

    Wt::WIconPair *folderIcon = new Wt::WIconPair("icons/document.png","icons/yellow-folder-open.png",false);

    std::map<long int,Wt::WTreeNode*> Tree_map;
    std::map<long int,Wt::WTreeNode*>::iterator it;

    while(MyEidosIter->Next())
    {
        //Добавляем в элемент управления отображения дерева

        Wt::WTPlatonNode *node = new Wt::WTPlatonNode(Wt::WString(MyEidosIter->GetTitle(),Wt::UTF8),NULL,0);
        node->id_Eidos=MyEidosIter->GetID();

        Tree_map[MyEidosIter->GetID()]=node;
        it=Tree_map.find(MyEidosIter->GetParentID());
        if(it==Tree_map.end())
        {
            tree->setTreeRoot(node);
        }
        else
        {
            Wt::WTreeNode *tonode=Tree_map.at(MyEidosIter->GetParentID());
            tonode->addChildNode(node);
            tonode->setInteractive(true);
            tonode->expand();
        }
        node->setSelectable(true);
        node->selected().connect(this, &PlatonViewer::ChooseTheEidos);
    }

    vbox_Eidos = new Wt::WVBoxLayout();
    vbox_Hip = new Wt::WVBoxLayout();
    vbox_Pragma = new Wt::WVBoxLayout();

    vbox_Eidos->addWidget(tree, 1);
    vbox_Eidos->setResizable(0);

    layout->addLayout(vbox_Eidos, 0, 0);
    layout->addLayout(vbox_Hip, 0, 1);
    layout->addLayout(vbox_Pragma, 1, 1);

    layout->setRowStretch(0, 0);
    layout->setColumnStretch(0, 0);
    layout->setRowStretch(1, 0);
    layout->setColumnStretch(1, 0);

    HTView = new Wt::WTableView();
    PrView = new Wt::WTableView();
    PrView->setSelectionMode(Wt::SingleSelection);
    HTView->setSelectionMode(Wt::SingleSelection);

    vbox_Hip->addWidget(HTView);
    vbox_Pragma->addWidget(PrView);

    w->setLayout(layout);
}

void PlatonViewer::ChooseTheEidos()
{
    if(!tree->selectedNodes().empty())
    {
        Wt::WTPlatonNode *OneNode = (Wt::WTPlatonNode *)*tree->selectedNodes().begin();
        if(MyEidos!=NULL)
            delete MyEidos;
        MyEidos= new platon::Eidos(DB,OneNode->id_Eidos);
        FillHypotesysView();
        PrView->hide();
    }
}


void PlatonViewer::FillHypotesysView()
{
    if(HypModel!=NULL) delete HypModel;
    HypModel= new platon::HypotesisMemModel(this->MyEidos, this);
    HTView->setModel(HypModel);    
    HTView->clicked().connect(this, &PlatonViewer::FillPragmaView);
}

void PlatonViewer::FillPragmaView(const Wt::WModelIndex& item)
{
    PrView->show();
    long HipID = boost::any_cast<long>(HTView->model()->data(HTView->model()->index(item.row(),0,WModelIndex())));

    this->log("Mouse typed by Hipotesys ID=" + boost::lexical_cast<std::string>(HipID));
    if (Myhypotesys!=NULL)
        delete Myhypotesys;
    Myhypotesys= new platon::Hypotesis(MyEidos, HipID);

    if(PrModel!=NULL) delete PrModel;
    PrModel= new platon::PragmaMemModel(Myhypotesys, this);
    PrView->setModel(PrModel);
}


Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    Wt::WApplication* app = new PlatonViewer(env);
    return app;

}

int main(int argc, char **argv)
{
    return Wt::WRun(argc, argv, &createApplication);
}

