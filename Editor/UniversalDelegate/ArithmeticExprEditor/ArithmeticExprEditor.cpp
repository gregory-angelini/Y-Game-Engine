#include "ArithmeticExprEditor.h"
#include <Editor/Editor/Editor.h>



ArithmeticExprEditor::ArithmeticExprEditor(QWidget* _parent, int32 _variableType): QWidget(_parent)
{
	fileNameEdit = new ArithmeticExprLineEdit(this);
	fileNameEdit->SetVariableType(_variableType);
	fileNameEdit->setGeometry(QRect(0, 0, 90, 24));
	connect(fileNameEdit, SIGNAL(FocusOut()), this, SLOT(FocusOut()));
	connect(fileNameEdit, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
	helpButton = new QToolButton(this);
	helpButton->setGeometry(QRect(390+210-50+150+200+30, 560+100+5+3, 32, 32));
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	
	if(_variableType == Variable<int32>::INTEGER_TYPE)
	{
		helpButton->setToolTip(GetIntegerArithmeticExprTable());
	}
	else if(_variableType == Variable<float>::FLOAT_TYPE)
	{
		helpButton->setToolTip(GetFloatArithmeticExprTable());
	}
	else if(_variableType == Variable<Vector2>::VECTOR_2_TYPE)
	{
		helpButton->setToolTip(GetVector2ArithmeticExprTable());
	}
	else if(_variableType == Variable<Vector2>::VECTOR_3_TYPE)
	{
		helpButton->setToolTip(GetVector3ArithmeticExprTable());
	}
	else if(_variableType == Variable<Quaternion>::QUATERNION_TYPE)
	{
		helpButton->setToolTip(GetQuaternionArithmeticExprTable());
	}

	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));
}



void ArithmeticExprEditor::SetText(QString _text)
{
	fileNameEdit->setText(_text);
}


void ArithmeticExprEditor::FocusOut(void)
{
	emit FocusOut(this);
}


QString ArithmeticExprEditor::GetText(void)const
{
	return fileNameEdit->text();
}


QString ArithmeticExprEditor::GetIntegerArithmeticExprTable(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>Arithmetic expression (INTEGER)</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>PLUS (+)</th>\n"
	"<th>MINUS (-)</th>\n"
	"<th>MULTIPLY (*)</th>\n"
	"<th>DIVIDE (/)</th>\n"
	"<th>INVERSION (-)</th>\n"
	"<th>ABSOLUTE (+)</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#00b200\">3+3=6</font></td>\n"
	"<td align = center><font color = \"#00b200\">3-3=0</font></td>\n"
	"<td align = center><font color = \"#00b200\">3*3=9</font></td>\n"
	"<td align = center><font color = \"#00b200\">3/3=1</font></td>\n"
	"<td align = center><font color = \"#00b200\">-3</font></td>\n"
	"<td align = center><font color = \"#00b200\">3</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#00b200\">3+3.0=6</font></td>\n"
	"<td align = center><font color = \"#00b200\">3-3.0=0</font></td>\n"
	"<td align = center><font color = \"#00b200\">3*3.0=9</font></td>\n"
	"<td align = center><font color = \"#00b200\">3/3.0=1</font></td>\n"
	"</tr>\n"

	"</table>\n");
}




QString ArithmeticExprEditor::GetFloatArithmeticExprTable(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>Arithmetic expression (FLOAT)</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>PLUS (+)</th>\n"
	"<th>MINUS (-)</th>\n"
	"<th>MULTIPLY (*)</th>\n"
	"<th>DIVIDE (/)</th>\n"
	"<th>INVERSION (-)</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#00b200\">3.0+3=6.0</font></td>\n"
	"<td align = center><font color = \"#00b200\">3.0-3=0.0</font></td>\n"
	"<td align = center><font color = \"#00b200\">3.0*3=9.0</font></td>\n"
	"<td align = center><font color = \"#00b200\">3.0/3=1.0</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#00b200\">3.0+3.0=6.0</font></td>\n"
	"<td align = center><font color = \"#00b200\">3.0-3.0=0.0</font></td>\n"
	"<td align = center><font color = \"#00b200\">3.0*3.0=9.0</font></td>\n"
	"<td align = center><font color = \"#00b200\">3.0/3.0=1.0</font></td>\n"
	"<td align = center><font color = \"#00b200\">-3.0</font></td>\n"
	"<td align = center><font color = \"#00b200\">3.0</font></td>\n"
	"</tr>\n"

	"</table>\n");
}



QString ArithmeticExprEditor::GetVector2ArithmeticExprTable(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>Arithmetic expression (VECTOR_2)</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>PLUS (+)</th>\n"
	"<th>MINUS (-)</th>\n"
	"<th>MULTIPLY (*)</th>\n"
	"<th>DIVIDE (/)</th>\n"
	"<th>INVERSION (-)</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\"></font></td>\n"
	"<td align = center><font color = \"#ff0000\"></font></td>\n"
	"<td align = center><font color = \"#00b200\">scale v</font></td>\n"
	"<td align = center><font color = \"#00b200\">scale v</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\"></font></td>\n"
	"<td align = center><font color = \"#ff0000\"></font></td>\n"
	"<td align = center><font color = \"#00b200\">scale v</font></td>\n"
	"<td align = center><font color = \"#00b200\">scale v</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#00b200\">middle v1-v2</font></td>\n"
	"<td align = center><font color = \"#00b200\">dist v2-v1</font></td>\n"
	"<td align = center><font color = \"#00b200\">cross product (perpendicular; only itself)</font></td>\n" 
	"<td align = center><font color = \"#00b200\">dot product (>0 = <90d, <0 = >90d, 0 = 90d)</font></td>\n" 
	"<td align = center><font color = \"#00b200\">-v</font></td>\n"
	"</tr>\n"

	"</table>\n");
}



QString ArithmeticExprEditor::GetVector3ArithmeticExprTable(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>Arithmetic expression (VECTOR_3)</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>PLUS (+)</th>\n"
	"<th>MINUS (-)</th>\n"
	"<th>MULTIPLY (*)</th>\n"
	"<th>DIVIDE (/)</th>\n"
	"<th>INVERSION (-)</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\"></font></td>\n"
	"<td align = center><font color = \"#ff0000\"></font></td>\n"
	"<td align = center><font color = \"#00b200\">scale v</font></td>\n"
	"<td align = center><font color = \"#00b200\">scale v</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\"></font></td>\n"
	"<td align = center><font color = \"#ff0000\"></font></td>\n"
	"<td align = center><font color = \"#00b200\">scale v</font></td>\n"
	"<td align = center><font color = \"#00b200\">scale v</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#00b200\"></font></td>\n"
	"<td align = center><font color = \"#00b200\"></font></td>\n"
	"<td align = center><font color = \"#00b200\">rotate v</font></td>\n" 
	"<td align = center><font color = \"#00b200\"></font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#00b200\">middle v1-v2</font></td>\n"
	"<td align = center><font color = \"#00b200\">dist v2-v1</font></td>\n"
	"<td align = center><font color = \"#00b200\">cross product (perpendicular v1-v2)</font></td>\n" 
	"<td align = center><font color = \"#00b200\">dot product (>0 = <90d, <0 = >90d, 0 = 90d)</font></td>\n" 
	"<td align = center><font color = \"#00b200\">-v</font></td>\n"
	"</tr>\n"

	"</table>\n");
}



QString ArithmeticExprEditor::GetQuaternionArithmeticExprTable(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>Arithmetic expression (QUATERNION)</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>PLUS (+)</th>\n"
	"<th>MINUS (-)</th>\n"
	"<th>MULTIPLY (*)</th>\n"
	"<th>DIVIDE (/)</th>\n"
	"<th>INVERSION (-)</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#00b200\"></font></td>\n"
	"<td align = center><font color = \"#00b200\">dir q1-q2</font></td>\n"
	"<td align = center><font color = \"#00b200\">q2 of q1</font></td>\n" 
	"<td align = center><font color = \"#00b200\"></font></td>\n" 
	"<td align = center><font color = \"#00b200\">-q (only non-unit)</font></td>\n"
	"</tr>\n"

	"</table>\n");
}


void ArithmeticExprEditor::ShowDoc(void)
{
	QString path = QString("editor\\docs\\") + "ArithmeticExpression.htm";

	QUrl url = QUrl(path);

	QDesktopServices::openUrl(url);
}




void ArithmeticExprEditor::resizeEvent(QResizeEvent* _event)
{
	fileNameEdit->setGeometry(QRect(0, 0, _event->size().width() - 24, 24));
	helpButton->setGeometry(QRect(_event->size().width() - 24-1, -1, 26, 26));
	QWidget::resizeEvent(_event);
}

