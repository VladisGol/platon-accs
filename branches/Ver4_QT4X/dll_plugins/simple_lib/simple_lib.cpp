#include "simple_lib.h"


Simple_lib::Simple_lib()
{
    QMessageBox msgBox;
    msgBox.setText("Hello");
    msgBox.exec();
}
int AnProc(const QString& instring)
{
    QMessageBox msgBox;
    msgBox.setText(instring);
    msgBox.setWindowTitle("Hello");
    msgBox.exec();

}
