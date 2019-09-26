#include "IntSpinBox.h"
#include <Engine/Core/Numerical/Numerical.h>


IntSpinBox::IntSpinBox(QWidget* _parent): QSpinBox(_parent)
{
	setRange(Numerical<int>::_LowerValue(), Numerical<int32>::_UpperValue());
}



void IntSpinBox::focusInEvent(QFocusEvent* _event)
{
	QSpinBox::focusInEvent(_event);
	emit FocusIn();
}


void IntSpinBox::focusOutEvent(QFocusEvent* _event)
{
	QSpinBox::focusOutEvent(_event);
	emit FocusOut();
}
