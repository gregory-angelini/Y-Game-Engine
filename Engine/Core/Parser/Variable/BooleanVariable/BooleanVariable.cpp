#include "BooleanVariable.h"


Variable<bool>::Variable(void): value(false)
{
	AddClassProperty(BOOLEAN_VARIABLE_CLASS);
}


Variable<bool>::Variable(bool _value): value(_value)
{
	AddClassProperty(BOOLEAN_VARIABLE_CLASS);
}


Variable<bool>::~Variable(void)
{
	Destroying();
}



int32 Variable<bool>::GetValueType(void)const
{
	return BOOLEAN_TYPE;
}



bool Variable<bool>::GetValue(void)const
{
	return value;
}


void Variable<bool>::SetValue(bool _value)
{
	value = _value;
	
	if(IsReceiversExist(AbstractVariable::CHANGE_VALUE_MESSAGE)) { SendPackage(AbstractVariable::CHANGE_VALUE_MESSAGE, dynamic_cast<AbstractVariable*>(this)); }
}

	

bool Variable<bool>::operator == (const Variable& _node)
{
	if(this == &_node)
	{
		return true;
	}
	return (GetName() == _node.GetName() && GetValue() == _node.GetValue());
}

			

bool Variable<bool>::SaveToFile(StringANSI _path)
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



bool Variable<bool>::SaveToFile(File& _file)
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



bool Variable<bool>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + _name + "." + AbstractVariable::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Variable<bool>::SaveAsToFile(File& _file, StringANSI _name)
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


void Variable<bool>::LoadDerivedFromFile(File& _file)
{
	_file.Read(value);
}

