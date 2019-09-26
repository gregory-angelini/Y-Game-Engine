#include "Vector3Variable.h"

Variable<Vector3>::Variable(void)
{
	AddClassProperty(VECTOR_3_VARIABLE_CLASS);
}



Variable<Vector3>::Variable(Vector3 _value): value(_value)
{
	AddClassProperty(VECTOR_3_VARIABLE_CLASS);
}


Variable<Vector3>::~Variable(void)
{
	Destroying();
}



int32 Variable<Vector3>::GetValueType(void)const
{
	return VECTOR_3_TYPE;
}



Vector3 Variable<Vector3>::GetValue(void)const
{
	return value;
}



void Variable<Vector3>::SetValue(Vector3 _value)
{
	value = _value;
	if(IsReceiversExist(AbstractVariable::CHANGE_VALUE_MESSAGE)) { SendPackage(AbstractVariable::CHANGE_VALUE_MESSAGE, dynamic_cast<AbstractVariable*>(this)); }
}



void Variable<Vector3>::SetPosition(Vector3 _position)
{
	SetValue(_position);
}



void Variable<Vector3>::DisplacePosition(Vector3 _offset)
{
	SetPosition(value + _offset);
}



void Variable<Vector3>::MoveTo(Vector3 _direction, float _units)
{
	if(_units != 0.0f) { SetPosition(value + _direction * _units); }
}



Vector3 Variable<Vector3>::GetPosition(void)
{
	return value;
}


Vector3 Variable<Vector3>::GetAbsolutePosition(void)
{
	return value;
}



void Variable<Vector3>::SetScale(Vector3 _scale)
{
	value = _scale;
	if(IsReceiversExist(AbstractVariable::CHANGE_VALUE_MESSAGE)) { SendPackage(AbstractVariable::CHANGE_VALUE_MESSAGE, dynamic_cast<AbstractVariable*>(this)); }
}
	


void Variable<Vector3>::DisplaceScale(Vector3 _scale)
{
	SetScale(value + _scale);
}



Vector3 Variable<Vector3>::GetScale(void)
{
	return value;
}


Vector3 Variable<Vector3>::GetAbsoluteScale(void)
{
	return value;
}



void Variable<Vector3>::SetColorRGB(Vector3 _rgb)
{
	value = _rgb;
}


void Variable<Vector3>::SetColor(RGBA _color)
{
	value = _color.GetRGBf();
}



Vector3 Variable<Vector3>::GetColorHSL(void)
{
	return RGBA::_RGBToHSL(value);
}


void Variable<Vector3>::SetColorHSL(Vector3 _hsl)
{
	value = RGBA::_HSLToRGB(_hsl);
}



void Variable<Vector3>::DisplaceColorRGB(Vector3 _offset)
{
	SetColorRGB(value + _offset);
}


Vector3 Variable<Vector3>::GetColorRGB(void)
{
	return value;
}



void Variable<Vector3>::SetAlpha(float _alpha)
{}



void Variable<Vector3>::SetRed(float _red)
{
	value.x = _red;
}


void Variable<Vector3>::SetGreen(float _green)
{
	value.y = _green;
}


void Variable<Vector3>::SetBlue(float _blue)
{
	value.z = _blue;
}




void Variable<Vector3>::SetHue(float _hue)
{
	Vector3 hsl = RGBA::_RGBToHSL(value);
	hsl.x = _hue * 360.0f;
	value = RGBA::_HSLToRGB(hsl); 
}


void Variable<Vector3>::SetHue360(float _hue)
{
	Vector3 hsl = RGBA::_RGBToHSL(value);
	hsl.x = _hue;
	value = RGBA::_HSLToRGB(hsl); 
}


void Variable<Vector3>::SetSaturation(float _saturation)
{
	Vector3 hsl = RGBA::_RGBToHSL(value);
	hsl.y = _saturation;
	value = RGBA::_HSLToRGB(hsl);
}

				
void Variable<Vector3>::SetLightness(float _lightness)
{
	Vector3 hsl = RGBA::_RGBToHSL(value);
	hsl.z = _lightness;
	value = RGBA::_HSLToRGB(hsl);
}



float Variable<Vector3>::GetAlpha(void)
{
	return 1.0f;
}



void Variable<Vector3>::SetColorRGBA(Vector4 _rgba)
{
	value.x = _rgba.x;
	value.y = _rgba.y;
	value.z = _rgba.z;
}



Vector4 Variable<Vector3>::GetColorRGBA(void)
{
	return Vector4(value, 1.0f);
}




bool Variable<Vector3>::operator == (const Variable& _node)
{
	if(this == &_node)
	{
		return true;
	}
	return (GetName() == _node.GetName() && GetValue() == _node.GetValue());
}


bool Variable<Vector3>::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + variablesDir + GetName() + "." + variablesSuffix;
	} 

	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Variable<Vector3>::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(GetValueType());
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(value);
		return true;
	}
	return false;
}


bool Variable<Vector3>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + _name + "." + AbstractVariable::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Variable<Vector3>::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(GetValueType());
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(value);
		return true;
	}
	return false;
}


void Variable<Vector3>::LoadDerivedFromFile(File& _file)
{
	_file.Read(value);

}
