#ifndef INTSPINBOX_H
#define INTSPINBOX_H
#include <Editor/ErrorDialog/ErrorDialog.h>
class IntSpinBox: public QSpinBox
{
	Q_OBJECT
	public: IntSpinBox(QWidget* _parent);
    protected: virtual void focusInEvent(QFocusEvent* _event);
    protected: virtual void focusOutEvent(QFocusEvent* _event);
	signals: void FocusIn(void);
	signals: void FocusOut(void);
};




#endif 

