#include "ModelView.h"

ModelView::ModelView(void): transformationValid(false)
{}


 
ModelView& ModelView::operator = (const ModelView& _object)
{
	if(this == &_object)
	{
		return *this;
	}

	SetPosition(_object.position);
	SetScale(_object.scale);
	SetRotation(_object.rotation);

	return *this;
}


ModelView::~ModelView(void)
{}


Matrix4x4 ModelView::GetTransformation(void)
{
	if(!transformationValid)
	{ 
		transformation.MakeTransform(GetScale(), GetRotation(), GetPosition());
		SetTransformationValid();
	}
	return transformation;
}

 
void ModelView::SetTransformation(Vector3 _scale, Quaternion _rotation, Vector3 _position)
{
	scale = _scale;
	rotation = _rotation;
	position = _position;
	ResetTransformationValid();
}


void ModelView::ResetTransformationValid(void)
{
	transformationValid = false;
}


void ModelView::SetTransformationValid(void)
{
	transformationValid = true;
}


void ModelView::SetPosition(Vector3 _position)
{
	position = _position;
	ResetTransformationValid();
}


void ModelView::DisplacePosition(Vector3 _offset)
{
	SetPosition(position + _offset);
}


Vector3 ModelView::GetPosition(void)
{
	return position;
}


Vector3 ModelView::GetAbsolutePosition(void)
{
	return position;
}
				

void ModelView::MoveTo(Vector3 _direction, float _units)
{
	if(_units != 0.0f) { SetPosition(position + _direction * _units); }
}


void ModelView::SetRotation(Quaternion _rotation)
{
	rotation = _rotation;
	ResetTransformationValid();
}


void ModelView::DisplaceRotation(Quaternion _rotation)
{
	SetRotation(rotation * _rotation);
}


Quaternion ModelView::GetRotation(void)
{
	return rotation;
}


Quaternion ModelView::GetAbsoluteRotation(void)
{
	return rotation;
}



void ModelView::SetScale(Vector3 _scale)
{
	scale = _scale;
	ResetTransformationValid();
}
			

void ModelView::DisplaceScale(Vector3 _scale)
{
	SetScale(scale + _scale);
}


Vector3 ModelView::GetScale(void)
{
	return scale;
}


Vector3 ModelView::GetAbsoluteScale(void)
{
	return scale;
}



bool ModelView::IsScaled(void)
{ 
	if(scale.x == 1.0f && scale.y == 1.0f && scale.z == 1.0f)
	{ 
		return false;
	}
	return true;
}

	
bool ModelView::IsSymmetricalScaled(void)
{
	if(scale.x == scale.y && scale.y == scale.z)
	{
		return true;
	}
	return false;
}