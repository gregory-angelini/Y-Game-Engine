#include "PointsSizeDialog.h"
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Engine/Core/Numerical/Numerical.h>




PointsSizeDialog::PointsSizeDialog(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	pointsSizeSpinBox = new FloatSpinBox(this);
    pointsSizeSpinBox->setGeometry(QRect(110, 20, 81, 22));
	pointsSizeSpinBox->setRange(1.0f, 128);
	pointsSizeSpinBox->setToolTip(QString::number(1.0) + " - " + QString::number(128.0));
	pointsSizeSpinBox->setValue(_GetPointsSizeFromFile());
   
    pointsSizeLabel = new QLabel(this);
    pointsSizeLabel->setGeometry(QRect(20, 20, 111, 21));
	pointsSizeLabel->setText("Points size:");
	cancelButton = new QPushButton(this);
	cancelButton->setText("Cancel");
    cancelButton->setGeometry(QRect(20, 70, 75, 23));
	connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
    okButton = new QPushButton(this);
    okButton->setText("Ok");
    okButton->setGeometry(QRect(110, 70, 75, 23));
	connect(okButton, SIGNAL(clicked()), SLOT(IsOk()));


	setWindowTitle("Points size");
    setFixedSize(206, 109);
} 


PointsSizeDialog::~PointsSizeDialog(void)
{}


float PointsSizeDialog::_GetPointsSizeFromFile(void)
{
	float value;

	if(File::_GetFloatKey("engine.config", true, "POINTS", "Size", value))
	{
		return value;
	}

	return 1.0f;
}


void PointsSizeDialog::SetPointsSize(float _value)
{
	pointsSizeSpinBox->setValue(_value);
}



float PointsSizeDialog::GetPointsSize(void)const
{
	return pointsSizeSpinBox->value();
}


void PointsSizeDialog::IsOk(void)
{
	File::_SetFloatKey("engine.config", true, "POINTS", "Size", pointsSizeSpinBox->value());

	accept();
}