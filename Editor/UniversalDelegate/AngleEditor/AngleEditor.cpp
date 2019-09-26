#include "AngleEditor.h"
#include <Engine/Core/Numerical/Numerical.h>


AngleEditor::AngleEditor(QWidget* _parent): QDoubleSpinBox(_parent)
{
	setDecimals(7);
	setRange(-360.0f, 360.0f);
	setToolTip("degrees [-360, 360]");  
}