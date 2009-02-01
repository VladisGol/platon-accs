#include "DialogLogin.h"
namespace platon
{


Login_Dialog::Login_Dialog(QWidget * parent): QDialog(parent)
{
	if (this->objectName().isEmpty())
		this->setObjectName("Login_Dialog");
	this->resize(420, 230);
	gridLayout = new QGridLayout(this);
	gridLayout->setObjectName("gridLayout");

	Host_label = new QLabel(this);
	Host_label->setObjectName("Host_label");
	Host_label->setText(tr("сервер БД"));

	gridLayout->addWidget(Host_label, 0, 0, 1, 1);

	Host = new QLineEdit(this);
	Host->setObjectName("Host");

	gridLayout->addWidget(Host, 1, 0, 1, 1);

	Alias_label = new QLabel(this);
	Alias_label->setObjectName("Alias_label");
	Alias_label->setText(tr("наименование базы"));

	gridLayout->addWidget(Alias_label, 2, 0, 1, 1);

	Alias = new QLineEdit(this);
	Alias->setObjectName("Alias");
	gridLayout->addWidget(Alias, 3, 0, 1, 1);

	UserName_label = new QLabel(this);
	UserName_label->setObjectName("UserName_label");
	UserName_label->setText(tr("имя пользователя"));

	gridLayout->addWidget(UserName_label, 4, 0, 1, 1);

	UserName = new QLineEdit(this);
	UserName->setObjectName("UserName");

	gridLayout->addWidget(UserName, 5, 0, 1, 1);

	Password_label = new QLabel(this);
	Password_label->setObjectName("Password_label");
	Password_label->setText(tr("пароль"));

	gridLayout->addWidget(Password_label, 6, 0, 1, 1);

	Password = new QLineEdit(this);
	Password->setObjectName("Password");
	Password->setEchoMode(QLineEdit::Password);

	gridLayout->addWidget(Password, 7, 0, 1, 1);

	buttonBox = new QDialogButtonBox(this);
	buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

	gridLayout->addWidget(buttonBox, 8, 0, 1, 1);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ExitWithAccept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    this->setWindowTitle(tr("Регистрация пользователя системы"));


    localsettings= new QSettings("PLATON","Platon-accs");
    localsettings->beginGroup("login");

    Host->setText(localsettings->value("Host").toString());
    Alias->setText(localsettings->value("Alias").toString());
    UserName->setText(localsettings->value("UserName").toString());
    Password->setText(localsettings->value("Password").toString());

    localsettings->endGroup();

}
void Login_Dialog::ExitWithAccept()
{
	//Выход с возвратом значения выбранного объекта
    localsettings->beginGroup("login");

    localsettings->setValue("Host",this->Host->text());
    localsettings->setValue("Alias",this->Alias->text());
    localsettings->setValue("UserName",this->UserName->text());
    localsettings->setValue("Password",this->Password->text());

    localsettings->endGroup();

	this->accept();
}
}
