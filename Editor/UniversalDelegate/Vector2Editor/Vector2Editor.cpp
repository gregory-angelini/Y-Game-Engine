#include "Vector2Editor.h"



Vector2Editor::Vector2Editor(QWidget* _parent): QWidget(_parent)
{
	x = new FloatSpinBox(this);
	x->setGeometry(QRect(0, 0, 30, 24));
	x->setToolTip("X");
	connect(x, SIGNAL(FocusOut()), this, SLOT(FocusOut()));

	y = new FloatSpinBox(this);
	y->setGeometry(QRect(30, 0, 30, 24));
	y->setToolTip("Y");
	connect(y, SIGNAL(FocusOut()), this, SLOT(FocusOut()));
}



void Vector2Editor::FocusOut(void)
{
	emit FocusOut(this);
}


QString Vector2Editor::GetXString(void)const
{
	return QString::number(x->value());
}
 

QString Vector2Editor::GetYString(void)const
{
	return QString::number(y->value());
}
 


void Vector2Editor::SetX(float _x)
{
	x->setValue(_x);
}

void Vector2Editor::SetY(float _y)
{
	y->setValue(_y);
}


void Vector2Editor::resizeEvent(QResizeEvent* _event)
{
	int size = _event->size().width() / 2;

	x->setGeometry(QRect(0, 0, size, 24)); 
	y->setGeometry(QRect(size, 0, size, 24));
	QWidget::resizeEvent(_event);
}


