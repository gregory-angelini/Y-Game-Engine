#include "GraphicsObject.h"
#include <Engine/Output/Graphics/Scene/GraphicsScene/GraphicsScene.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Renderer/Renderer.h>




GraphicsObject::GraphicsObject(void): graphicsType(0), showEnable(true), color(1.0f, 1.0f, 1.0f), alpha(1.0f)
{
	AddClassProperty(GRAPHICS_OBJECT_CLASS);
	graphicsContainer.SetRemoveMode(MagicContainer::REMOVE_NODE);
	SetScale(Vector3(1.0f, 1.0f, 1.0f));
}



GraphicsObject& GraphicsObject::operator = (const GraphicsObject& _object)
{
	if(this == &_object)
	{
		return *this;
	}

	SetColorRGB(_object.color);
	SetAlpha(_object.alpha);
	SetShowEnable(_object.IsShowEnabled());

	return *this;
}



GraphicsObject::~GraphicsObject(void)
{
	Destroying();
}

 

void GraphicsObject::BeginTransform(void)
{
	Renderer::_SetCurrentStack(Renderer::MODELVIEW);
	Renderer::_PushMatrix();
	Renderer::_MultMatrix(GetTransformation());
}


void GraphicsObject::EndTransform(void)
{
	Renderer::_SetCurrentStack(Renderer::MODELVIEW);
	Renderer::_PopMatrix();
}



Matrix4x4 GraphicsObject::GetTransformation(void)
{
	if(!transformationValid)
	{ 
		transformation.MakeTransform(GetScale(), GetRotation(), GetPosition());
		
		if(graphicsContainer.IsNotEmpty())
		{
			transformation = graphicsContainer.Get()->GetTransformation() * transformation; 
		}
		SetTransformationValid();
	}
	return transformation;
}


Vector3 GraphicsObject::GetAbsolutePosition(void)
{
	if(graphicsContainer.IsNotEmpty())
	{
		return graphicsContainer.Get()->GetTransformation() * position;
	}
	return position;
}



Quaternion GraphicsObject::GetAbsoluteRotation(void)
{
	if(graphicsContainer.IsNotEmpty())
	{
		return graphicsContainer.Get()->GetAbsoluteRotation() * rotation;
	}
	return rotation;
}


Vector3 GraphicsObject::GetAbsoluteScale(void)
{
	if(graphicsContainer.IsNotEmpty())
	{
		return graphicsContainer.Get()->GetAbsoluteScale() + scale;
	}
	return scale;
}


GraphicsObject* GraphicsObject::GetGraphicsContainer(void)const
{
	return graphicsContainer.Get();
}



bool GraphicsObject::IsShowEnabled(void)const
{
	return showEnable;
}


void GraphicsObject::SetShowEnable(bool _enable)
{
	showEnable = _enable; 
}



int64 GraphicsObject::GetGraphicsType(void)const
{
	return graphicsType;
}


bool GraphicsObject::IsGraphicsTypeEnabled(int64 _property)const
{
	return graphicsType & _property;
}



void GraphicsObject::ModifyGraphicsType(int64 _property, bool _enable)
{
	graphicsType = _enable ? graphicsType | _property : graphicsType & ~_property;
}


StringANSI GraphicsObject::GetGraphicsTypeString(void)const
{
	return Bitset::_ToString(graphicsType);
}


void GraphicsObject::SetColorRGB(Vector3 _rgb)
{
	color = _rgb;
}


void GraphicsObject::DisplaceColorRGB(Vector3 _offset)
{
	SetColorRGB(color + _offset);
}



Vector3 GraphicsObject::GetColorRGB(void)
{
	return color;
}


void GraphicsObject::SetColor(RGBA _color)
{
	color = _color.GetRGBf();
	alpha = _color.GetAf();
}


void GraphicsObject::SetHue(float _hue)
{
	Vector3 hsl = RGBA::_RGBToHSL(color);
	hsl.x = _hue * 360.0f;
	color = RGBA::_HSLToRGB(hsl); 
}


void GraphicsObject::SetHue360(float _hue)
{
	Vector3 hsl = RGBA::_RGBToHSL(color);
	hsl.x = _hue;
	color = RGBA::_HSLToRGB(hsl); 
}


void GraphicsObject::SetSaturation(float _saturation)
{
	Vector3 hsl = RGBA::_RGBToHSL(color);
	hsl.y = _saturation;
	color = RGBA::_HSLToRGB(hsl);
}

				
void GraphicsObject::SetLightness(float _lightness)
{
	Vector3 hsl = RGBA::_RGBToHSL(color);
	hsl.z = _lightness;
	color = RGBA::_HSLToRGB(hsl);
}



void GraphicsObject::SetColorHSL(Vector3 _hsl)
{
	color = RGBA::_HSLToRGB(_hsl);
}



Vector3 GraphicsObject::GetColorHSL(void)
{
	return RGBA::_RGBToHSL(color);
}



void GraphicsObject::SetColorRGBA(Vector4 _rgba) 
{
	color.x = _rgba.x;
	color.y = _rgba.y;
	color.z = _rgba.z;
	alpha = _rgba.w;
}


Vector4 GraphicsObject::GetColorRGBA(void)
{
	return Vector4(color, alpha);
}



void GraphicsObject::SetRed(float _red)
{
	color.x = _red;
}


void GraphicsObject::SetGreen(float _green)
{
	color.y = _green;
}


void GraphicsObject::SetBlue(float _blue)
{
	color.z = _blue;
}



void GraphicsObject::SetAlpha(float _alpha)
{
	alpha = _alpha;
}


float GraphicsObject::GetAlpha(void)
{
	return alpha;
}




void GraphicsObject::SaveDerivedToFile(File& _file)
{
	_file.Write(GetColorRGBA());
	_file.Write(GetPosition());
	_file.Write(GetScale());
	_file.Write(GetRotation());
	_file.Write(showEnable);
}




void GraphicsObject::LoadDerivedFromFile(File& _file)
{
	Vector4 color;
	_file.Read(color);
	SetColorRGBA(color);
	Vector3 position;
	_file.Read(position);
	SetPosition(position);
	Vector3 scale;
	_file.Read(scale);
	SetScale(scale);
	Quaternion rotation;
	_file.Read(rotation);
	SetRotation(rotation);
	_file.Read(showEnable);
}