#ifndef ABOUTPROGRAM_H
#define ABOUTPROGRAM_H

#include <QtGui/QDialog>
#include "ui_aboutprogram.h"

class AboutProgram : public QDialog
{
    Q_OBJECT

public:
    AboutProgram(QWidget *parent = 0);
    ~AboutProgram();

private:
    Ui::AboutProgramClass ui;
};

#endif // ABOUTPROGRAM_H
