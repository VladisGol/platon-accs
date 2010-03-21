#ifndef ES_MAINWINDOW_H
#define ES_MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_es_mainwindow.h"

class es_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    es_mainwindow(QWidget *parent = 0);
    ~es_mainwindow();

private:
    Ui::es_mainwindowClass ui;
};

#endif // ES_MAINWINDOW_H
