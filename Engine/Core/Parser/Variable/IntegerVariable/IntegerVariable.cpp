#include "IntegerVariable.h"


Variable<int32>::Variable(void): value(0)
{
	AddClassProperty(INTEGER_VARIABLE_CLASS);

	lowerBound = Numerical<int32>::_LowerValue();
	upperBound = Numerical<int32>::_UpperValue();
}



Variable<int32>::Variable(int32 _value): value(_value)
{
	AddClassProperty(INTEGER_VARIABLE_CLASS);
}




Variable<int32>::~Variable(void)
{
	Destroying();
}



void Variable<int32>::SetLowerBound(int32 _lowerBound)
{
	lowerBound = _lowerBound;
}


int32 Variable<int32>::GetLowerBound(void)const
{
	return lowerBound;
}


void Variable<int32>::SetUpperBound(int32 _upperBound)
{
	upperBound = _upperBound;
}


int32 Variable<int32>::GetUpperBound(void)const
{
	return upperBound;
}


void Variable<int32>::SetRange(int32 _lowerBound, int32 _upperBound)
{
	lowerBound = _lowerBound;
	upperBound = _upperBound;
}



int32 Variable<int32>::GetValueType(void)const
{
	return INTEGER_TYPE;
}



int32 Variable<int32>::GetValue(void)const
{
	return value;
}



void Variable<int32>::SetValue(int32 _value)
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

		
void Variable<int32>::DisplaceValue(int32 _offset)
{
	SetValue(value + _offset);
}


bool Variable<int32>::operator == (const Variable& _node)
{
	if(this == &_node)
	{
		return true;
	}
	return (GetName() == _node.GetName() && GetValue() == _node.GetValue());
}



bool Variable<int32>::SaveToFile(StringANSI _path)
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



bool Variable<int32>::SaveToFile(File& _file)
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


bool Variable<int32>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + _name + "." + AbstractVariable::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Variable<int32>::SaveAsToFile(File& _file, StringANSI _name)
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

void Variable<int32>::LoadDerivedFromFile(File& _file)
{
	_file.Read(value);
	_file.Read(lowerBound);
	_file.Read(upperBound);
}
