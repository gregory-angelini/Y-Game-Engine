#include "TimeHMSEditor.h"



TimeHMSEditor::TimeHMSEditor(QWidget* _parent): QTimeEdit(_parent)
{
	setToolTip("duration");  
	setDisplayFormat("HH:mm:ss:zzz");
}






