#include "StringExprEditor.h"
#include <Editor/Editor/Editor.h>



StringExprEditor::StringExprEditor(QWidget* _parent): QWidget(_parent)
{
	fileNameEdit = new StringExprLineEdit(this);
	fileNameEdit->setGeometry(QRect(0, 0, 90, 24));
	connect(fileNameEdit, SIGNAL(FocusOut()), this, SLOT(FocusOut()));
	connect(fileNameEdit, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
	helpButton = new QToolButton(this);
	helpButton->setGeometry(QRect(390+210-50+150+200+30, 560+100+5+3, 32, 32));
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setToolTip(GetStringExprTable());
	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));
}



void StringExprEditor::SetText(QString _text)
{
	fileNameEdit->setText(_text);
}


void StringExprEditor::FocusOut(void)
{
	emit FocusOut(this);
}


QString StringExprEditor::GetText(void)const
{
	return fileNameEdit->text();
}



QString StringExprEditor::GetStringExprTable(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>String expression</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>PLUS (+)</th>\n"
	"<th>CONST ($)</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"
	"</table>\n");
}



void StringExprEditor::ShowDoc(void)
{
	QString path = QString("editor\\docs\\") + "StringExpression.htm";

	QUrl url = QUrl(path);

	QDesktopServices::openUrl(url);
}




void StringExprEditor::resizeEvent(QResizeEvent* _event)
{
	fileNameEdit->setGeometry(QRect(0, 0, _event->size().width() - 24, 24));
	helpButton->setGeometry(QRect(_event->size().width() - 24-1, -1, 26, 26));
	QWidget::resizeEvent(_event);
}

