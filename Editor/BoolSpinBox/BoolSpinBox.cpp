#include "BoolSpinBox.h"


BoolSpinBox::BoolSpinBox(void)
{}


BoolSpinBox::BoolSpinBox(QWidget *_parent): QSpinBox(_parent)
{
	setRange(0, 1);
}


QString BoolSpinBox::textFromValue(int _value)const
{
	if(_value) { return "true"; } else { return "false"; }
}
 