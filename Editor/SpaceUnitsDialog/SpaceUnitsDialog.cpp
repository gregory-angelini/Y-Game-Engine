#include "SpaceUnitsDialog.h"
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Engine/Core/Numerical/Numerical.h>




SpaceUnitsDialog::SpaceUnitsDialog(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	unitsSpinBox = new QSpinBox(this);
    unitsSpinBox->setGeometry(QRect(110, 20, 81, 22));
	unitsSpinBox->setRange(1, Numerical<int32>::_UpperValue());
	unitsSpinBox->setToolTip(QString::number(1) + " - " + QString::number(Numerical<int32>::_UpperValue()));
	unitsSpinBox->setValue(_GetUnitsFromFile());
   
    unitsLabel = new QLabel(this);
    unitsLabel->setGeometry(QRect(20, 20, 111, 21));
	unitsLabel->setText("Pixels per meter:");
	cancelButton = new QPushButton(this);
	cancelButton->setText("Cancel");
    cancelButton->setGeometry(QRect(20, 70, 75, 23));
	connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
    okButton = new QPushButton(this);
    okButton->setText("Ok");
    okButton->setGeometry(QRect(110, 70, 75, 23));
	connect(okButton, SIGNAL(clicked()), SLOT(IsOk()));


	setWindowTitle("Space units");
    setFixedSize(206, 109);
} 


SpaceUnitsDialog::~SpaceUnitsDialog(void)
{}


int32 SpaceUnitsDialog::_GetUnitsFromFile(void)
{
	int32 value;

	if(File::_GetIntKey("editor.config", true, "SPACE", "Pixels per meter", value))
	{
		return value;
	}

	return 1;
}


void SpaceUnitsDialog::SetPixelsPerMeter(int32 _value)
{
	unitsSpinBox->setValue(_value);
}



int SpaceUnitsDialog::GetPixelsPerMeter(void)const
{
	return unitsSpinBox->value();
}


void SpaceUnitsDialog::IsOk(void)
{
	File::_SetIntKey("editor.config", true, "SPACE", "Pixels per meter", unitsSpinBox->value());

	accept();
}