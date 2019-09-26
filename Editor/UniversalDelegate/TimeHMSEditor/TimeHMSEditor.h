#ifndef TIMEHMSEDITOR_H
#define TIMEHMSEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/IntSpinBox/IntSpinBox.h>
class TimeHMSEditor: public QTimeEdit
{
	Q_OBJECT
    public: TimeHMSEditor(QWidget* _parent = 0);
};



#endif 

