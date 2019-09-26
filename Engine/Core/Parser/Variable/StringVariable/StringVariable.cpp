#include "StringVariable.h"


Variable<StringANSI>::Variable(void)
{
	AddClassProperty(STRING_VARIABLE_CLASS);
}


Variable<StringANSI>::Variable(StringANSI _value): value(_value)
{
	AddClassProperty(STRING_VARIABLE_CLASS);
}


Variable<StringANSI>::~Variable(void)
{
	Destroying();
}



int32 Variable<StringANSI>::GetValueType(void)const
{
	return STRING_TYPE;
}



StringANSI Variable<StringANSI>::GetValue(void)const
{
	return value;
}


void Variable<StringANSI>::SetValue(StringANSI _value)
{
	value = _value;
	if(IsReceiversExist(AbstractVariable::CHANGE_VALUE_MESSAGE)) { SendPackage(AbstractVariable::CHANGE_VALUE_MESSAGE, dynamic_cast<AbstractVariable*>(this)); }
}

		

bool Variable<StringANSI>::operator == (const Variable& _node)
{
	if(this == &_node)
	{
		return true;
	}
	return (GetName() == _node.GetName() && GetValue() == _node.GetValue());
}



bool Variable<StringANSI>::SaveToFile(StringANSI _path)
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




bool Variable<StringANSI>::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(GetValueType());
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(value.length());
		_file.WriteString(value);
		return true;
	}
	return false;
}


bool Variable<StringANSI>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + _name + "." + AbstractVariable::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Variable<StringANSI>::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(GetValueType());
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(value.length());
		_file.WriteString(value);
		return true;
	}
	return false;
}


void Variable<StringANSI>::LoadDerivedFromFile(File& _file)
{
	int32 length;
	_file.Read(length);
	_file.ReadString(value, length);
}
