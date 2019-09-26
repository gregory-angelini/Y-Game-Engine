#include "AbstractVariable.h"
#include <Engine/Core/Parser/BooleanExprParser/BooleanExprParser.h>
#include <Engine/Core/Parser/Variable/IntegerVariable/IntegerVariable.h>
#include <Engine/Core/Parser/Variable/FloatVariable/FloatVariable.h>
#include <Engine/Core/Parser/Variable/BooleanVariable/BooleanVariable.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>
#include <Engine/Core/Parser/Variable/Vector2Variable/Vector2Variable.h>
#include <Engine/Core/Parser/Variable/Vector3Variable/Vector3Variable.h>
#include <Engine/Core/Parser/Variable/QuaternionVariable/QuaternionVariable.h>



StringANSI AbstractVariable::variablesSuffix = "variable";
StringANSI AbstractVariable::variablesDir = "variables/";


AbstractVariable::AbstractVariable(void)
{
	AddClassProperty(ABSTRACT_VARIABLE_CLASS);
}
	

AbstractVariable::~AbstractVariable(void)
{		
	Destroying();
}


bool AbstractVariable::Rename(StringANSI _name)
{
	if(BooleanExprParser::_IsNameValid(_name) == -1)
	{
		return AbstractObject::Rename(_name);
	}
	return false;
}


void AbstractVariable::_SetFileSuffix(StringANSI _suffix)
{
	variablesSuffix = _suffix;
}



StringANSI AbstractVariable::_GetFileSuffix(void)
{
	return variablesSuffix;
}

		

StringANSI AbstractVariable::_GetDir(void)
{
	return variablesDir;
}



bool AbstractVariable::SaveToFile(StringANSI _path)
{
	return false; 
}


bool AbstractVariable::SaveToFile(File& _file)
{
	return false; 

}

bool AbstractVariable::SaveAsToFile(StringANSI _name)
{
	return false;
}



bool AbstractVariable::SaveAsToFile(File& _file, StringANSI _name)
{
	return false;
}


bool AbstractVariable::LoadFromFile(StringANSI _path, bool _auto)
{
	return false; 
}


bool AbstractVariable::LoadFromFile(File& _file)
{
	return false;
}



AbstractVariable* AbstractVariable::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + variablesDir + _path + "." + variablesSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


AbstractVariable* AbstractVariable::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		int32 version;
		_file.Read(version);
		int32 valueType;
		_file.Read(valueType);
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		switch(valueType)
		{
			case Variable<int32>::INTEGER_TYPE:
			{
				Variable<int32>* variable = new Variable<int32>();
				variable->Rename(name);
				variable->LoadDerivedFromFile(_file);
				return variable;
			}

			case Variable<float>::FLOAT_TYPE:
			{
				Variable<float>* variable = new Variable<float>();
				variable->Rename(name);
				variable->LoadDerivedFromFile(_file);
				return variable;
			}

			case Variable<bool>::BOOLEAN_TYPE:
			{
				Variable<bool>* variable = new Variable<bool>();
				variable->Rename(name);
				variable->LoadDerivedFromFile(_file);
				return variable;
			}

			case Variable<StringANSI>::STRING_TYPE:
			{
				Variable<StringANSI>* variable = new Variable<StringANSI>();
				variable->Rename(name);
				variable->LoadDerivedFromFile(_file);
				return variable;
			}

			case Variable<Vector2>::VECTOR_2_TYPE:
			{
				Variable<Vector2>* variable = new Variable<Vector2>();
				variable->Rename(name);
				variable->LoadDerivedFromFile(_file);
				return variable;
			}

			case Variable<Vector3>::VECTOR_3_TYPE:
			{
				Variable<Vector3>* variable = new Variable<Vector3>();
				variable->Rename(name);
				variable->LoadDerivedFromFile(_file);
				return variable;
			}

			case Variable<Quaternion>::QUATERNION_TYPE:
			{
				Variable<Quaternion>* variable = new Variable<Quaternion>();
				variable->Rename(name);
				variable->LoadDerivedFromFile(_file);
				return variable;
			}
		}
	}
	return NIL;
}
