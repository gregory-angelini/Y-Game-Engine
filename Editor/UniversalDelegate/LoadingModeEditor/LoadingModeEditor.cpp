
#include "LoadingModeEditor.h"



LoadingModeIntSpinBox::LoadingModeIntSpinBox(QWidget* _parent): QSpinBox(_parent)
{
	setRange(0, 1);
}


QString LoadingModeIntSpinBox::textFromValue(int _value)const
{
	return ValueToString(_value);
}
 

QString LoadingModeIntSpinBox::ValueToString(int _value)const
{
	if(_value == 1) 
	{ 
		return "AUTO"; 
	} 
	else if(_value == 0)
	{ 
		return "MANUAL"; 
	}
	return "";
}



int LoadingModeIntSpinBox::StringToValue(QString _value)const
{
	if(_value == "AUTO")
	{
		return 1;
	}
	else if(_value == "MANUAL")
	{
		return 0;
	}
	return -1;
}


void LoadingModeIntSpinBox::SetValue(QString _value)
{
	setValue(StringToValue(_value));
}



QString LoadingModeIntSpinBox::GetValueString(void)const
{
	return textFromValue(value());
}
 



LoadingModeEditor::LoadingModeEditor(QWidget* _parent): QWidget(_parent)
{
	intValue = new LoadingModeIntSpinBox(this);
	intValue->setGeometry(QRect(30, 0, 30+25, 24));
}



void LoadingModeEditor::SetValue(int _value)
{
	intValue->setValue(_value);
}



void LoadingModeEditor::SetValue(QString _value)
{
	intValue->SetValue(_value);
}



QString LoadingModeEditor::GetValueString(void)const
{
	return intValue->GetValueString();
}
 


void LoadingModeEditor::resizeEvent(QResizeEvent* _event)
{
	int size = _event->size().width();
	intValue->setGeometry(QRect(0, 0, size, 24)); 
	QWidget::resizeEvent(_event);
}


