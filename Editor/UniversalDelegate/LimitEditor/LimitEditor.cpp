
#include "LimitEditor.h"



LimitEditor::LimitEditor(QWidget* _parent): QWidget(_parent)
{
	intValue = new LimitIntSpinBox(this);
	intValue->setGeometry(QRect(30, 0, 30+25, 24));
}



void LimitEditor::SetValue(int _value)
{
	intValue->setValue(_value);
}


QString LimitEditor::GetValueString(void)const
{
	return QString::number(intValue->value());
}
 


void LimitEditor::resizeEvent(QResizeEvent* _event)
{
	int size = _event->size().width();
	intValue->setGeometry(QRect(0, 0, size, 24)); 
	QWidget::resizeEvent(_event);
}


