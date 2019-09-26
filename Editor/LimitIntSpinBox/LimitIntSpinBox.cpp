#include "LimitIntSpinBox.h"
#include <Engine/Core/Numerical/Numerical.h>



LimitIntSpinBox::LimitIntSpinBox(QWidget* _parent): QSpinBox(_parent)
{
	setRange(-1, Numerical<int>::_UpperValue());
	setToolTip("count");
}



QString LimitIntSpinBox::textFromValue(int _value)const
{
	if(_value == -1) { return "UNLIMITED"; } 
	else if(_value == 0) { return "STOPED"; } 
	return QSpinBox::textFromValue(_value);
}
 
