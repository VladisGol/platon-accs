#include <QApplication>
#include "mainWin.h"
#include <QTextCodec>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mainWin myAPP;

    myAPP.show();
    return app.exec();
}
