#ifndef VIEWPORT_H
#define VIEWPORT_H
#include <QtGui/QMainWindow>
#include <QtGui>
class Viewport: public QWidget
{
	Q_OBJECT
	public: Viewport(int _width, int _height, int _x, int _y, QWidget* _parent = 0);
	public: virtual ~Viewport(void);
	public: HWND GetHWND(void)const;
	protected: virtual void enterEvent(QEvent* _event);
	protected: virtual void leaveEvent(QEvent* _event);
	signals: void CursorEnter(void);
    signals: void CursorLeave(void);
};

#endif 
