#ifndef LIMITINTSPINBOX_H
#define LIMITINTSPINBOX_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/IntSpinBox/IntSpinBox.h>
class LimitIntSpinBox: public QSpinBox
{
	Q_OBJECT
	public: LimitIntSpinBox(QWidget* _parent);
	public: virtual QString textFromValue(int _value)const;
};



#endif 

