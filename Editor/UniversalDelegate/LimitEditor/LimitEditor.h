#ifndef LIMITEDITOR_H
#define LIMITEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/LimitIntSpinBox/LimitIntSpinBox.h>
class LimitEditor: public QWidget
{
	Q_OBJECT
	private: LimitIntSpinBox* intValue;
    public: LimitEditor(QWidget* _parent = 0);
	private: virtual void resizeEvent(QResizeEvent* _event);
	public: QString GetValueString(void)const;
	public: void SetValue(int _value);
};



#endif 

