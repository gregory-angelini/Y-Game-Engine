#include "StringLineEdit.h"
#include <Engine/Core/Numerical/Numerical.h>


StringLineEdit::StringLineEdit(QWidget* _parent): QLineEdit(_parent)
{

}


void StringLineEdit::focusInEvent(QFocusEvent* _event)
{
	QLineEdit::focusInEvent(_event);
	emit FocusIn();
}


void StringLineEdit::focusOutEvent(QFocusEvent* _event)
{
	QLineEdit::focusOutEvent(_event);
	emit FocusOut();
}
