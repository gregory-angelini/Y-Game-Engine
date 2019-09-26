#ifndef QUATERNIONEDITOR_H
#define QUATERNIONEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/FloatSpinBox/FloatSpinBox.h>
#include <Engine/Math/Quaternion/Quaternion.h>
class QuaternionEditor: public QWidget
{
	Q_OBJECT
	private: FloatSpinBox *x, *y, *z, *w;
	private: QToolButton *editObjectButton;
    public: QuaternionEditor(QWidget* _parent = 0);
	private: virtual void resizeEvent(QResizeEvent* _event);
	public: QString GetXString(void)const;
	public: QString GetYString(void)const;
	public: QString GetZString(void)const;
	public: QString GetWString(void)const;
	public: Quaternion GetQuaternion(void)const;
	public: void SetQuaternion(Quaternion _quaternion);
	public: void SetX(float _x);
	public: void SetY(float _y);
	public: void SetZ(float _z);
	public: void SetW(float _w);
	public slots: void OpenQuaternionEditor(void);
};



#endif 

