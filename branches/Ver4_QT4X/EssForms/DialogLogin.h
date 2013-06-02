#ifndef LOGIN_H
#define LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QSettings>


namespace platon
{

class Login_Dialog:public QDialog
{
    Q_OBJECT
public:
	QGridLayout *gridLayout;
	QLabel *Host_label;
	QLineEdit *Host;
	QLabel *Alias_label;
	QLineEdit *Alias;
	QLabel *UserName_label;
	QLineEdit *UserName;
	QLabel *Password_label;
	QLineEdit *Password;
	QDialogButtonBox *buttonBox;
	QSettings*localsettings;

    Login_Dialog(QWidget * parent);


public slots:
	void ExitWithAccept();
protected:

};
}
#endif
