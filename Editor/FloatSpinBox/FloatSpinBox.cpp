#include "FloatSpinBox.h"
#include <Engine/Core/Numerical/Numerical.h>


FloatSpinBox::FloatSpinBox(QWidget* _parent): QDoubleSpinBox(_parent)
{
	setDecimals(7);
	setRange(Numerical<float>::_LowerValue(), Numerical<float>::_UpperValue());
}


void FloatSpinBox::focusInEvent(QFocusEvent* _event)
{
	QDoubleSpinBox::focusInEvent(_event);
	emit FocusIn();
}


void FloatSpinBox::focusOutEvent(QFocusEvent* _event)
{
	QDoubleSpinBox::focusOutEvent(_event);
	emit FocusOut();
}
