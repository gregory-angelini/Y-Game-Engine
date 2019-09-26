#ifndef VECTOR3EDITOR_H
#define VECTOR3EDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/FloatSpinBox/FloatSpinBox.h>
class Vector3Editor: public QWidget
{
	Q_OBJECT
	private: QComboBox* constsComboBox;
	private: FloatSpinBox *x, *y, *z;
    public: Vector3Editor(QWidget* _parent = 0);
	private: virtual void resizeEvent(QResizeEvent* _event);
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	public: QString GetXString(void)const;
	public: QString GetYString(void)const;
	public: QString GetZString(void)const;
	public: void SetRGBMode(void);
	public: void SetHSLMode(void);
	public: void SetX(float _x);
	public: void SetY(float _y);
	public: void SetZ(float _z);
	private slots: void PickConstValue(const QString& _value);
};



#endif 

