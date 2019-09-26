#ifndef MOOLSPINBOX_H
#define BOOLSPINBOX_H
#include <Editor/ErrorDialog/ErrorDialog.h>
class BoolSpinBox: public QSpinBox
{
	Q_OBJECT
	public: BoolSpinBox(void);
	public: BoolSpinBox(QWidget *_parent = 0);
	public: virtual QString textFromValue(int _value)const;
};



#endif 

