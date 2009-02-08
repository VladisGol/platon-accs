#include <QApplication>
#include <QTextCodec>
#include "mainWin.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPixmap pixmap(":/PICS/Platon.png");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();

    mainWin myAPP;
    platon::DataClass myDCl;
    myAPP.MyDCl=&myDCl;
    myAPP.MyDCl->LoadDynLib(&splash, &app);
    app.processEvents();

    myAPP.show();
    splash.finish(&myAPP);
    return app.exec();
}
