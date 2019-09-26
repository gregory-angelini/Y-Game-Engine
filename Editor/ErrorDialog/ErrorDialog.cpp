#include "ErrorDialog.h"



void ErrorDialog(QString _title, QString _text, int _width, int _height)
{
	QDialog* dialog = new QDialog(0, Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	dialog->setWindowTitle(_title);
	dialog->setFixedSize(_width, _height);
	QLabel* label = new QLabel(dialog);
	label->setWordWrap(true);
	label->setText(_text);
	QGridLayout* layout = new QGridLayout(dialog);
	layout->addWidget(label, 0, 0);
	dialog->setLayout(layout);

	dialog->exec();
	delete dialog;
}


