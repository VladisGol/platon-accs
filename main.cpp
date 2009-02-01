#include <QApplication>
#include "mainWin.h"
#include <QTextCodec>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPixmap pixmap(":/PICS/Platon.png");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();

    mainWin myAPP;

    myAPP.show();
    splash.finish(&myAPP);
    return app.exec();
}
