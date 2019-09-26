#ifndef FLOATSPINBOX_H
#define FLOATSPINBOX_H
#include <Editor/ErrorDialog/ErrorDialog.h>
class FloatSpinBox: public QDoubleSpinBox
{
	Q_OBJECT
	public: FloatSpinBox(QWidget* _parent);
    protected: virtual void focusInEvent(QFocusEvent* _event);
    protected: virtual void focusOutEvent(QFocusEvent* _event);
	signals: void FocusIn(void);
	signals: void FocusOut(void);
};




#endif 

