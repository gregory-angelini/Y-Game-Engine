#include "Vector2Variable.h"


Variable<Vector2>::Variable(void)
{
	AddClassProperty(VECTOR_2_VARIABLE_CLASS);
}


Variable<Vector2>::Variable(Vector2 _value): value(_value)
{
	AddClassProperty(VECTOR_2_VARIABLE_CLASS);
}


Variable<Vector2>::~Variable(void)
{
	Destroying();
}



int32 Variable<Vector2>::GetValueType(void)const
{
	return VECTOR_2_TYPE;
}



Vector2 Variable<Vector2>::GetValue(void)const
{
	return value;
}



void Variable<Vector2>::SetValue(Vector2 _value)
{
	value = _value;
	if(IsReceiversExist(AbstractVariable::CHANGE_VALUE_MESSAGE)) { SendPackage(AbstractVariable::CHANGE_VALUE_MESSAGE, dynamic_cast<AbstractVariable*>(this)); }
}



bool Variable<Vector2>::operator == (const Variable& _node)
{
	if(this == &_node)
	{
		return true;
	}
	return (GetName() == _node.GetName() && GetValue() == _node.GetValue());
}


bool Variable<Vector2>::SaveToFile(StringANSI _path)
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



bool Variable<Vector2>::SaveToFile(File& _file)
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


bool Variable<Vector2>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + _name + "." + AbstractVariable::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Variable<Vector2>::SaveAsToFile(File& _file, StringANSI _name)
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


void Variable<Vector2>::LoadDerivedFromFile(File& _file)
{
	_file.Read(value);
}
