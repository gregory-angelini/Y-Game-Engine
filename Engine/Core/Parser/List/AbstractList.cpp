
#include "AbstractList.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Parser/List/IntegerList/IntegerList.h> 
#include <Engine/Core/Parser/List/FloatList/FloatList.h> 
#include <Engine/Core/Parser/List/BooleanList/BooleanList.h> 
#include <Engine/Core/Parser/List/StringList/StringList.h> 
#include <Engine/Core/Parser/List/Vector2List/Vector2List.h> 
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h> 
#include <Engine/Core/Parser/List/QuaternionList/QuaternionList.h> 
#include <Engine/Core/Parser/List/ColorableObjectList/ColorableObjectList.h>
#include <Engine/Core/Parser/List/AbstractObjectList/AbstractObjectList.h>
#include <Engine/Core/Parser/Variable/AbstractVariable.h>
#include <Engine/Core/Parser/List/SetList/SetList.h>


StringANSI AbstractList::listsSuffix = "list";
StringANSI AbstractList::listsDir = "lists/";



AbstractList::AbstractList(void)
{
	AddClassProperty(ABSTRACT_LIST_CLASS);
}


AbstractList::~AbstractList(void)
{ 
	Destroying();
}



void AbstractList::SetValue(int32 _value)
{}


void AbstractList::SetValue(float _value) 
{}



void AbstractList::SetValue(bool _value)
{}


void AbstractList::SetValue(StringANSI _value)
{}


void AbstractList::SetValue(Vector2 _value)
{}


void AbstractList::SetValue(Vector3 _value)
{}


void AbstractList::SetValue(Vector4 _value)
{}


void AbstractList::SetValue(Quaternion _value)
{}



void AbstractList::SetItemValue(int32 _index, int32 _value)
{}



float AbstractList::GetLength(void)const
{
	return -1.0f;
}



void AbstractList::SetItemValue(int32 _index, float _value)
{}


void AbstractList::SetItemValue(int32 _index, bool _value)
{}


void AbstractList::SetItemValue(int32 _index, StringANSI _value)
{}


void AbstractList::SetItemValue(int32 _index, Vector2 _value)
{}


void AbstractList::SetItemValue(int32 _index, Vector3 _value)
{}


void AbstractList::SetItemValue(int32 _index, Quaternion _value)
{}




void AbstractList::_SetFileSuffix(StringANSI _suffix)
{
	listsSuffix = _suffix;
}



StringANSI AbstractList::_GetFileSuffix(void)
{
	return listsSuffix;
}

		

StringANSI AbstractList::_GetDir(void)
{
	return listsDir;
}



bool AbstractList::SaveToFile(StringANSI _path)
{
	return false;
}


bool AbstractList::SaveToFile(File& _file)
{
	return false;
}


bool AbstractList::SaveAsToFile(StringANSI _name)
{
	return false;
}



bool AbstractList::SaveAsToFile(File& _file, StringANSI _name)
{
	return false;
}



bool AbstractList::LoadFromFile(StringANSI _path, bool _auto)
{
	return false; 
}


bool AbstractList::LoadFromFile(File& _file)
{
	return false;
}




AbstractList* AbstractList::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + listsDir + _path + "." + listsSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


AbstractList* AbstractList::_LoadFromFile(File& _file)
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
			case List<int32>::INTEGER_TYPE:
			{
				List<int32>* list = new List<int32>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<float>::FLOAT_TYPE:
			{
				List<float>* list = new List<float>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<bool>::BOOLEAN_TYPE:
			{
				List<bool>* list = new List<bool>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<StringANSI>::STRING_TYPE:
			{
				List<StringANSI>* list = new List<StringANSI>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<Vector2>::VECTOR_2_TYPE:
			{
				List<Vector2>* list = new List<Vector2>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<Vector3>::VECTOR_3_TYPE:
			{
				List<Vector3>* list = new List<Vector3>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<Quaternion>::QUATERNION_TYPE:
			{
				List<Quaternion>* list = new List<Quaternion>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}
			
			case List<AbstractObject>::OBJECT_TYPE:
			{
				List<AbstractObject>* list = new List<AbstractObject>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<TranslatableObject>::TRANSLATABLE_OBJECT_TYPE:
			{
				List<TranslatableObject>* list = new List<TranslatableObject>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<RotatableObject>::ROTATABLE_OBJECT_TYPE:
			{
				List<RotatableObject>* list = new List<RotatableObject>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<ScalableObject>::SCALABLE_OBJECT_TYPE:
			{
				List<ScalableObject>* list = new List<ScalableObject>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<ColorableObject>::COLORABLE_OBJECT_TYPE:
			{
				List<ColorableObject>* list = new List<ColorableObject>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}

			case List<Set>::SET_TYPE:
			{
				List<Set>* list = new List<Set>();
				list->Rename(name);
				list->LoadDerivedFromFile(_file);
				return list;
			}
		}
	}
	return NIL;
}
