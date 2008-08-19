#include <QApplication>
#include "mainWin.h"
#include <QTextCodec>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mainWin myAPP;

    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(codec);

    myAPP.show();
    return app.exec();
}
