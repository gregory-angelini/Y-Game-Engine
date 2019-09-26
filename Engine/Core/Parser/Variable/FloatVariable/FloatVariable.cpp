#include "FloatVariable.h"


Variable<float>::Variable(void): value(0.0f)
{
	AddClassProperty(FLOAT_VARIABLE_CLASS);

	lowerBound = Numerical<float>::_LowerValue();
	upperBound = Numerical<float>::_UpperValue();
}



Variable<float>::Variable(float _value): value(_value)
{
	AddClassProperty(FLOAT_VARIABLE_CLASS);
}

		

Variable<float>::~Variable(void)
{
	Destroying();
}



void Variable<float>::SetLowerBound(float _lowerBound)
{
	lowerBound = _lowerBound;
}


float Variable<float>::GetLowerBound(void)const
{
	return lowerBound;
}


void Variable<float>::SetUpperBound(float _upperBound)
{
	upperBound = _upperBound;
}


float Variable<float>::GetUpperBound(void)const
{
	return upperBound;
}



void Variable<float>::SetRange(float _lowerBound, float _upperBound)
{
	lowerBound = _lowerBound;
	upperBound = _upperBound;
}



int32 Variable<float>::GetValueType(void)const
{
	return FLOAT_TYPE;
}



float Variable<float>::GetValue(void)const
{
	return value;
}



void Variable<float>::SetValue(float _value)
{
	if(_value < lowerBound)
	{
		_value = lowerBound;
	}
	else if(_value > upperBound)
	{
		_value = upperBound;
	}

	value = _value;
	if(IsReceiversExist(AbstractVariable::CHANGE_VALUE_MESSAGE)) { SendPackage(AbstractVariable::CHANGE_VALUE_MESSAGE, dynamic_cast<AbstractVariable*>(this)); }
}


void Variable<float>::DisplaceValue(float _offset)
{
	SetValue(value + _offset);
}



bool Variable<float>::operator == (const Variable& _node)
{
	if(this == &_node)
	{
		return true;
	}
	return (GetName() == _node.GetName() && GetValue() == _node.GetValue());
}
	

bool Variable<float>::SaveToFile(StringANSI _path)
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



bool Variable<float>::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(GetValueType());
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(value);
		_file.Write(lowerBound);
		_file.Write(upperBound);
		return true;
	}
	return false;
}


bool Variable<float>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + _name + "." + AbstractVariable::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Variable<float>::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(GetValueType());
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(value);
		_file.Write(lowerBound);
		_file.Write(upperBound);
		return true;
	}
	return false;
}


void Variable<float>::LoadDerivedFromFile(File& _file)
{
	_file.Read(value);
	_file.Read(lowerBound);
	_file.Read(upperBound);
}

