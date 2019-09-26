
#include "Viewport.h"
#include <QtGui>


Viewport::Viewport(int _width, int _height, int _x, int _y, QWidget* _parent): QWidget(_parent)
{
	resize(_width, _height);
	move(_x, _y);
	setFocusPolicy(Qt::WheelFocus);
    show();
}



Viewport::~Viewport(void)
{
}




HWND Viewport::GetHWND(void)const
{
	return winId();
}


void Viewport::enterEvent(QEvent* _event)
{
	emit CursorEnter();
}


void Viewport::leaveEvent(QEvent* _event)
{
	emit CursorLeave();
}




