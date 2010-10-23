#include <QApplication>
#include <QTextCodec>
#include "mainWin.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Выводим предшествующее окно программы во время загрузки
    QPixmap pixmap(":/PICS/Platon.png");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();

    mainWin myAPP;
    myAPP.MyDCl->LoadDynLib(&splash, &app);	//Загружаем библиотеки и отображаем ход процесса на предшествующем окне
    app.processEvents();

    splash.finish(&myAPP);
    myAPP.show();

    return app.exec();
}
