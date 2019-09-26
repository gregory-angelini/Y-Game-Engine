#include "Vector3Editor.h"



Vector3Editor::Vector3Editor(QWidget* _parent): QWidget(_parent)
{
	x = new FloatSpinBox(this);
	x->setGeometry(QRect(0+60, 0, 30, 24));
	x->setToolTip("X");
	connect(x, SIGNAL(FocusOut()), this, SLOT(FocusOut()));

	y = new FloatSpinBox(this);
	y->setGeometry(QRect(30+60, 0, 30, 24));
	y->setToolTip("Y");
	connect(y, SIGNAL(FocusOut()), this, SLOT(FocusOut()));

	z = new FloatSpinBox(this);
	z->setGeometry(QRect(60+60, 0, 30, 24));
	z->setToolTip("Z");
	connect(z, SIGNAL(FocusOut()), this, SLOT(FocusOut()));
	constsComboBox = new QComboBox(this);
	constsComboBox->setGeometry(QRect(0, 0, 60, 24));
	constsComboBox->setToolTip("Help");
	constsComboBox->addItem("XYZ");
	constsComboBox->addItem("DIRECTION");
	constsComboBox->addItem("RGB_COLOR");

	connect(constsComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickConstValue(const QString&)));
}



void Vector3Editor::FocusOut(void)
{
	emit FocusOut(this);
}



void Vector3Editor::SetRGBMode(void)
{
	x->setToolTip("Red (0.0 - 1.0)");
	x->setRange(0.0f, 1.0f);

	y->setToolTip("Green (0.0 - 1.0)");
	y->setRange(0.0f, 1.0f);

	z->setToolTip("Blue (0.0 - 1.0)");
	z->setRange(0.0f, 1.0f);
}


void Vector3Editor::SetHSLMode(void)
{
	x->setToolTip("Hue (0.0 - 360.0)");
	x->setRange(0.0f, 360.0f);

	y->setToolTip("Saturation (0.0 - 1.0)");
	y->setRange(0.0f, 1.0f);

	z->setToolTip("Lightness (0.0 - 1.0)");
	z->setRange(0.0f, 1.0f);
}



QString Vector3Editor::GetXString(void)const
{
	return QString::number(x->value());
}
 

QString Vector3Editor::GetYString(void)const
{
	return QString::number(y->value());
}
 

QString Vector3Editor::GetZString(void)const
{
	return QString::number(z->value());
}


void Vector3Editor::SetX(float _x)
{
	x->setValue(_x);
}


void Vector3Editor::SetY(float _y)
{
	y->setValue(_y);
}


void Vector3Editor::SetZ(float _z)
{
	z->setValue(_z);
}


void Vector3Editor::PickConstValue(const QString& _value)
{
	if(_value == "XYZ")
	{}
	else if(_value == "DIRECTION")
	{
	}
	else if(_value == "RGB_COLOR")
	{
		QColor color;
		color.setRedF(x->value());
		color.setGreenF(y->value());
		color.setBlueF(z->value());

		QColor pickedColor = QColorDialog::getColor(color, this, "Color Picker");

		if(pickedColor.isValid())
		{
			x->setValue(pickedColor.redF());
			y->setValue(pickedColor.greenF());
			z->setValue(pickedColor.blueF());
		}
	}  
}


void Vector3Editor::resizeEvent(QResizeEvent* _event)
{
	int size = _event->size().width() / 4;

	constsComboBox->setGeometry(QRect(0, 0, size + 1, 24)); 
	x->setGeometry(QRect(size, 0, size + 1, 24)); 
	y->setGeometry(QRect(size*2, 0, size + 2, 24));
	z->setGeometry(QRect(size*3, 0, size + 2, 24)); 
	QWidget::resizeEvent(_event);
}


