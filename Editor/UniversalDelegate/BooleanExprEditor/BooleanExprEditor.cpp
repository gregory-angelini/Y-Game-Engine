#include "BooleanExprEditor.h"
#include <Editor/Editor/Editor.h>



BooleanExprEditor::BooleanExprEditor(QWidget* _parent): QWidget(_parent)
{
	fileNameEdit = new BooleanExprLineEdit(this);
	fileNameEdit->setGeometry(QRect(0, 0, 90, 24));
	connect(fileNameEdit, SIGNAL(FocusOut()), this, SLOT(FocusOut()));
	connect(fileNameEdit, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
	helpButton = new QToolButton(this);
	helpButton->setGeometry(QRect(390+210-50+150+200+30, 560+100+5+3, 32, 32));
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setToolTip(GetBooleanExprTable());
	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));
}



void BooleanExprEditor::SetText(QString _text)
{
	fileNameEdit->setText(_text);
}


void BooleanExprEditor::FocusOut(void)
{
	emit FocusOut(this);
}


QString BooleanExprEditor::GetText(void)const
{
	return fileNameEdit->text();
}


QString BooleanExprEditor::GetBooleanExprTable(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>Boolean expression</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>OR (|)</th>\n" 
	"<th>AND (&)</th>\n"
	"<th>NOT (~)</th>\n"
	"<th>XOR (^)</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"</tr>\n"

	"</table>\n");
} 



void BooleanExprEditor::ShowDoc(void)
{
	QString path = QString("editor\\docs\\") + "BooleanExpression.htm";

	QUrl url = QUrl(path);

	QDesktopServices::openUrl(url);
}




void BooleanExprEditor::resizeEvent(QResizeEvent* _event)
{
	fileNameEdit->setGeometry(QRect(0, 0, _event->size().width() - 24, 24));
	helpButton->setGeometry(QRect(_event->size().width() - 24-1, -1, 26, 26));
	QWidget::resizeEvent(_event);
}

