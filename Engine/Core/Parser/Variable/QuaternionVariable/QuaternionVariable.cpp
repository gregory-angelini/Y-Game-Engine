#include "QuaternionVariable.h"


Variable<Quaternion>::Variable(void)
{
	AddClassProperty(QUATERNION_VARIABLE_CLASS);
}



Variable<Quaternion>::Variable(Quaternion _value): value(_value)
{
	AddClassProperty(QUATERNION_VARIABLE_CLASS);
}


Variable<Quaternion>::~Variable(void)
{
	Destroying();
}



int32 Variable<Quaternion>::GetValueType(void)const
{
	return QUATERNION_TYPE;
}


Quaternion Variable<Quaternion>::GetValue(void)const
{
	return value;
}



void Variable<Quaternion>::SetValue(Quaternion _value)
{
	value = _value;
	if(IsReceiversExist(AbstractVariable::CHANGE_VALUE_MESSAGE)) { SendPackage(AbstractVariable::CHANGE_VALUE_MESSAGE, dynamic_cast<AbstractVariable*>(this)); }
}

			


void Variable<Quaternion>::SetRotation(Quaternion _rotation)
{
	SetValue(_rotation);
}



void Variable<Quaternion>::DisplaceRotation(Quaternion _rotation)
{
	SetRotation(value * _rotation);
}



Quaternion Variable<Quaternion>::GetRotation(void)
{
	return value;
}
		

Quaternion Variable<Quaternion>::GetAbsoluteRotation(void)
{
	return value;
}
	


bool Variable<Quaternion>::operator == (const Variable& _node)
{
	if(this == &_node)
	{
		return true;
	}
	return (GetName() == _node.GetName() && GetValue() == _node.GetValue());
}

		

bool Variable<Quaternion>::SaveToFile(StringANSI _path)
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



bool Variable<Quaternion>::SaveToFile(File& _file)
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


bool Variable<Quaternion>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + _name + "." + AbstractVariable::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Variable<Quaternion>::SaveAsToFile(File& _file, StringANSI _name)
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


void Variable<Quaternion>::LoadDerivedFromFile(File& _file)
{
	_file.Read(value);
}
