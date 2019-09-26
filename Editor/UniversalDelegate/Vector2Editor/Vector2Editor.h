#ifndef VECTOR2EDITOR_H
#define VECTOR2EDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/FloatSpinBox/FloatSpinBox.h>
class Vector2Editor: public QWidget
{
	Q_OBJECT
	private: FloatSpinBox *x, *y;
    public: Vector2Editor(QWidget* _parent = 0);
	private: virtual void resizeEvent(QResizeEvent* _event);
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	public: QString GetXString(void)const;
	public: QString GetYString(void)const;
	public: void SetX(float _x);
	public: void SetY(float _y);
};



#endif 

