#include "FloatEditorEx.h"
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Parser/List/AbstractList.h>


FloatEditorEx::FloatEditorEx(QWidget* _parent): QWidget(_parent)
{
	floatValue = new FloatSpinBox(this);
	floatValue->setGeometry(QRect(30, 0, 30+25, 24));
	floatValue->setToolTip("value");
	connect(floatValue, SIGNAL(FocusOut()), this, SLOT(FocusOut()));
	hmsEditor = new TimeHMSEditor(this);
	hmsEditor->setGeometry(QRect(30, 0, 30+25, 24));
	hmsEditor->hide();
	connect(hmsEditor, SIGNAL(timeChanged(const QTime&)), this, SLOT(ChangeTimeValue(const QTime&)));
	angleDegreesEditor = new AngleEditor(this);
	angleDegreesEditor->setGeometry(QRect(30, 0, 30+25, 24));
	angleDegreesEditor->hide();
	connect(angleDegreesEditor, SIGNAL(valueChanged(double)), this, SLOT(ChangeAngleValue(double)));
	angleRadiansComboBox = new QComboBox(this);
	angleRadiansComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	angleRadiansComboBox->setToolTip("value");
	angleRadiansComboBox->hide();
	angleRadiansComboBox->addItem("PI (1/2 CIRCLE)");
	angleRadiansComboBox->addItem("HALF_PI (1/4 CIRCLE)");
	angleRadiansComboBox->addItem("DOUBLE_PI (1 CIRCLE)");
	connect(angleRadiansComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickAngleRadiansValue(const QString&)));
	constsComboBox = new QComboBox(this);
	constsComboBox->setGeometry(QRect(0, 0, 60, 24));
	constsComboBox->setToolTip("Help");
	constsComboBox->addItem("NUMBER");
	constsComboBox->addItem("TIME_HMS");  
	constsComboBox->addItem("ANGLE_DEGREES");
	constsComboBox->addItem("ANGLE_RADIANS");
	connect(constsComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickConstValue(const QString&)));
}



void FloatEditorEx::FocusOut(void)
{
	emit FocusOut(this);
}



float FloatEditorEx::GetAngleRadiansIndex(int _value)
{ 
	if(Numerical<double>::_Equals(_value, Angle::PI))
	{
		return angleRadiansComboBox->findText("PI (1/2 CIRCLE)");
	}
	else if(Numerical<double>::_Equals(_value, Angle::HALF_PI))
	{
		return angleRadiansComboBox->findText("HALF_PI (1/4 CIRCLE)");
	}
	else if(Numerical<double>::_Equals(_value, Angle::DOUBLE_PI))
	{
		return angleRadiansComboBox->findText("DOUBLE_PI (1 CIRCLE)");
	}
	return -1.0f;
}


void FloatEditorEx::PickAngleRadiansValue(const QString& _value)
{
	if(_value == "PI (1/2 CIRCLE)")
	{
		floatValue->setValue(Angle::PI);
	}
	else if(_value == "HALF_PI (1/4 CIRCLE)")
	{
		floatValue->setValue(Angle::HALF_PI);
	}
	else if(_value == "DOUBLE_PI (1 CIRCLE)")
	{
		floatValue->setValue(Angle::DOUBLE_PI);
	}
}



void FloatEditorEx::Set_0_1_Mode(void)
{
	floatValue->setToolTip("0.0 - 1.0");
	floatValue->setRange(0.0f, 1.0f);
}



void FloatEditorEx::Set_0_360_Mode(void)
{
	floatValue->setToolTip("0.0 - 360.0");
	floatValue->setRange(0.0f, 360.0f);
}



void FloatEditorEx::ChangeTimeValue(const QTime& _time)
{
	HMS newTime(_time.hour(), _time.minute(), _time.second(), _time.msec());
	floatValue->setValue((float)newTime.ToMs() / HMS::MS_IN_SECOND);
}



void FloatEditorEx::ChangeAngleValue(double _value)
{
	float angle = Angle::_DegreesToRadians(_value);
	floatValue->setValue(angle);
}



void FloatEditorEx::PickConstValue(const QString& _value)
{
	floatValue->hide();
	hmsEditor->hide();
	angleDegreesEditor->hide();
	angleRadiansComboBox->hide();
	
	if(_value == "NUMBER")
	{
		floatValue->show();
	}
	else if(_value == "TIME_HMS")
	{
		hmsEditor->show();
		HMS time = HMS::_MsToHMS(HMS::_SecondsToMs((float)floatValue->value()));
		hmsEditor->setTime(QTime(time.GetHours(), time.GetMinutes(), time.GetSeconds(), time.GetMs()));
	}
	else if(_value == "ANGLE_DEGREES")
	{
		angleDegreesEditor->show();
		float angle = Angle::_RadiansToDegrees(floatValue->value());
		angleDegreesEditor->setValue(angle);
	} 
	else if(_value == "ANGLE_RADIANS")
	{
		angleRadiansComboBox->show();
		angleRadiansComboBox->setCurrentIndex(GetAngleRadiansIndex(floatValue->value()));
	}
}


void FloatEditorEx::SetValue(float _value)
{
	floatValue->setValue(_value);
}


QString FloatEditorEx::GetValueString(void)const
{
	return QString::number(floatValue->value());
}
 


void FloatEditorEx::resizeEvent(QResizeEvent* _event)
{
	int32 size = _event->size().width() / 2;

	constsComboBox->setGeometry(QRect(0, 0, size, 24));
	
	floatValue->setGeometry(QRect(size, 0, size, 24)); 
	hmsEditor->setGeometry(QRect(size, 0, size, 24)); 
	angleDegreesEditor->setGeometry(QRect(size, 0, size, 24)); 
	angleRadiansComboBox->setGeometry(QRect(size, 0, size, 24)); 
	
	QWidget::resizeEvent(_event);
}


