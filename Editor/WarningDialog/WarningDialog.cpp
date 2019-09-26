
#include "WarningDialog.h"



WarningDialog::WarningDialog(QString _title, QString _message, int _width, int _height, QString _buttonOkName, QString _buttonCancelName, QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	message = new QLabel(_message, this);
	message->setWordWrap(true);
	buttonOk = new QPushButton(_buttonOkName, this);
	connect(buttonOk, SIGNAL(clicked()), SLOT(accept()));
	buttonCancel = new QPushButton(_buttonCancelName, this);
	connect(buttonCancel, SIGNAL(clicked()), SLOT(reject()));
	buttonOk->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	buttonCancel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum); 
		
	SetFocusCancel();
	layout = new QGridLayout(this);
	layout->addWidget(message, 1, 0, 1, 0);
	
	layout->addWidget(buttonCancel, 2, 0);
	layout->addWidget(buttonOk, 2, 1);

	setLayout(layout);

	setWindowTitle(_title);
	
	setFixedSize(_width, _height);
}


void WarningDialog::SetFocusOk(void)
{
	buttonOk->setFocus();
}

	


void WarningDialog::SetFocusCancel(void)
{
	buttonCancel->setFocus();
}

