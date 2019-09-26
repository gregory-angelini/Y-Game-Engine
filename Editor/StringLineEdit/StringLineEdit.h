#ifndef STRINGLINEEDIT_H
#define STRINGLINEEDIT_H
#include <Editor/ErrorDialog/ErrorDialog.h>
class StringLineEdit: public QLineEdit
{
	Q_OBJECT
	public: StringLineEdit(QWidget* _parent);
    protected: virtual void focusInEvent(QFocusEvent* _event);
    protected: virtual void focusOutEvent(QFocusEvent* _event);
	signals: void FocusIn(void);
	signals: void FocusOut(void);
};




#endif 

