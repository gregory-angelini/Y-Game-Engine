#include "EnumerationExprEditor.h"
#include <Editor/Editor/Editor.h>


EnumerationExprEditor::EnumerationExprEditor(QWidget* _parent): QWidget(_parent)
{
	fileNameEdit = new EnumerationExprLineEdit(this);
	fileNameEdit->setGeometry(QRect(0, 0, 90, 24));
	connect(fileNameEdit, SIGNAL(FocusOut()), this, SLOT(FocusOut()));
	connect(fileNameEdit, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
	helpButton = new QToolButton(this);
	helpButton->setGeometry(QRect(390+210-50+150+200+30, 560+100+5+3, 32, 32));
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	

	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));
}



void EnumerationExprEditor::SetText(QString _text)
{
	fileNameEdit->setText(_text);
}


void EnumerationExprEditor::FocusOut(void)
{
	emit FocusOut(this);
}


QString EnumerationExprEditor::GetText(void)const
{
	return fileNameEdit->text();
}


void EnumerationExprEditor::ShowDoc(void)
{
	QString path = QString("editor\\docs\\") + "EnumerationExpression.htm";

	QUrl url = QUrl(path);

	QDesktopServices::openUrl(url);
}




void EnumerationExprEditor::resizeEvent(QResizeEvent* _event)
{
	fileNameEdit->setGeometry(QRect(0, 0, _event->size().width() - 24, 24));
	helpButton->setGeometry(QRect(_event->size().width() - 24-1, -1, 26, 26));
	QWidget::resizeEvent(_event);
}

