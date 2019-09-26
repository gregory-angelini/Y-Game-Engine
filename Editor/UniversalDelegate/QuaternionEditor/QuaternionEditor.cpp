#include "QuaternionEditor.h"
#include <Editor/QuaternionCreator/QuaternionCreator.h>
#include <Engine/Core/Trigger/Trigger.h>



QuaternionEditor::QuaternionEditor(QWidget* _parent): QWidget(_parent)
{
	x = new FloatSpinBox(this);
	x->setGeometry(QRect(0, 0, 30, 24));
	x->setToolTip("X");
	x->setDisabled(true);

	y = new FloatSpinBox(this);
	y->setGeometry(QRect(30, 0, 30, 24));
	y->setToolTip("Y");
	y->setDisabled(true);

	z = new FloatSpinBox(this);
	z->setGeometry(QRect(60, 0, 30, 24));
	z->setToolTip("Z");
	z->setDisabled(true);

	w = new FloatSpinBox(this);
	w->setGeometry(QRect(90, 0, 30, 24));
	w->setToolTip("W");
	w->setDisabled(true);

	SetQuaternion(Quaternion());
	editObjectButton = new QToolButton(this);
	editObjectButton->setGeometry(QRect(90+24-1, 0, 50, 24+2));
	editObjectButton->setText("edit");
	connect(editObjectButton, SIGNAL(clicked()), this, SLOT(OpenQuaternionEditor()));
}




void QuaternionEditor::OpenQuaternionEditor(void)
{
	QuaternionCreator* dialog = new QuaternionCreator(this);

	dialog->SetQuaternion(GetQuaternion());
	if(dialog->exec() == QDialog::Accepted)
	{
		SetQuaternion(dialog->GetQuaternion());
	}
	delete dialog;
}


Quaternion QuaternionEditor::GetQuaternion(void)const
{
	return Quaternion(x->value(), y->value(), z->value(), w->value());
}


void QuaternionEditor::SetQuaternion(Quaternion _quaternion)
{
	x->setValue(_quaternion.x);
	y->setValue(_quaternion.y);
	z->setValue(_quaternion.z);
	w->setValue(_quaternion.w);
}


QString QuaternionEditor::GetXString(void)const
{
	return QString::number(x->value());
}
 

QString QuaternionEditor::GetYString(void)const
{
	return QString::number(y->value());
}
 

QString QuaternionEditor::GetZString(void)const
{
	return QString::number(z->value());
}


QString QuaternionEditor::GetWString(void)const
{
	return QString::number(w->value());
}


void QuaternionEditor::SetX(float _x)
{
	x->setValue(_x);
}

void QuaternionEditor::SetY(float _y)
{
	y->setValue(_y);
}

void QuaternionEditor::SetZ(float _z)
{
	z->setValue(_z);
}


void QuaternionEditor::SetW(float _w)
{
	w->setValue(_w);
}


void QuaternionEditor::resizeEvent(QResizeEvent* _event)
{
	int32 size = _event->size().width() / 5;

	x->setGeometry(QRect(0, 0, size, 24)); 
	y->setGeometry(QRect(size, 0, size, 24));
	z->setGeometry(QRect(size*2, 0, size, 24)); 
	w->setGeometry(QRect(size*3, 0, size, 24)); 
	editObjectButton->setGeometry(QRect(size*4-1, -1, size+6, 24+2));
	QWidget::resizeEvent(_event);
}


