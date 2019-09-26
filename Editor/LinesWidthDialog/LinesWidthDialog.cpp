#include "LinesWidthDialog.h"
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Engine/Core/Numerical/Numerical.h>




LinesWidthDialog::LinesWidthDialog(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	linesWidthSpinBox = new FloatSpinBox(this);
    linesWidthSpinBox->setGeometry(QRect(110, 20, 81, 22));
	linesWidthSpinBox->setRange(1.0f, 128);
	linesWidthSpinBox->setToolTip(QString::number(1.0) + " - " + QString::number(128.0));
	linesWidthSpinBox->setValue(_GetLinesWidthFromFile());
   
    linesWidthLabel = new QLabel(this);
    linesWidthLabel->setGeometry(QRect(20, 20, 111, 21));
	linesWidthLabel->setText("Lines width:");
	cancelButton = new QPushButton(this);
	cancelButton->setText("Cancel");
    cancelButton->setGeometry(QRect(20, 70, 75, 23));
	connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
    okButton = new QPushButton(this);
    okButton->setText("Ok");
    okButton->setGeometry(QRect(110, 70, 75, 23));
	connect(okButton, SIGNAL(clicked()), SLOT(IsOk()));


	setWindowTitle("Lines width");
    setFixedSize(206, 109);
} 


LinesWidthDialog::~LinesWidthDialog(void)
{}


float LinesWidthDialog::_GetLinesWidthFromFile(void)
{
	float value;

	if(File::_GetFloatKey("engine.config", true, "LINES", "Width", value))
	{
		return value;
	}

	return 1.0f;
}


void LinesWidthDialog::SetLinesWidth(float _value)
{
	linesWidthSpinBox->setValue(_value);
}



float LinesWidthDialog::GetLinesWidth(void)const
{
	return linesWidthSpinBox->value();
}


void LinesWidthDialog::IsOk(void)
{
	File::_SetFloatKey("engine.config", true, "LINES", "Width", linesWidthSpinBox->value());

	accept();
}