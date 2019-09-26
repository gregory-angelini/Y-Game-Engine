#include "Set.h"
#include <Engine/Core/Parser/Variable/IntegerVariable/IntegerVariable.h>
#include <Engine/Core/Parser/Variable/FloatVariable/FloatVariable.h>
#include <Engine/Core/Parser/Variable/BooleanVariable/BooleanVariable.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>
#include <Engine/Core/Parser/Variable/Vector2Variable/Vector2Variable.h>
#include <Engine/Core/Parser/Variable/Vector3Variable/Vector3Variable.h>
#include <Engine/Core/Parser/Variable/QuaternionVariable/QuaternionVariable.h>


Set::Set(void): size(0), array(NIL)
{
	AddClassProperty(SET_CLASS);
}


Set::Set(const Set& _set): size(0), array(NIL)
{
	AddClassProperty(SET_CLASS);

	*this = _set;
}


Set::Set(int32 _size): size(0), array(NIL)
{
	AddClassProperty(SET_CLASS);

	Resize(_size);
}
	

Set::~Set(void)
{
	Destroy();
	Destroying();
}


void Set::Destroy(void)
{
	if(array)
	{
		for(int32 i = 0; i < size; i++)
		{
			delete array[i];
		}
		delete[] array;
		array = NIL;
		size = 0;
	}
}


void Set::Resize(int32 _size)
{
	Destroy();

	if(_size > 0)
	{
		array = new AbstractVariable*[_size];
		for(int32 i = 0; i < _size; i++)
		{
			array[i] = NIL;
		}
	}
	size = _size;
}



Set& Set::operator = (const Set& _set)
{
	if(this == &_set)
	{
		return *this;
	}

	Resize(_set.size);
	for(int32 i = 0; i < _set.size; i++)
	{
		SetSlot(i, _set.GetSlot(i)); 
	}

	return *this;
}


AbstractVariable* Set::GetSlot(int32 _index)const
{
	if(_index >= 0 && _index < size)
	{
		return array[_index];
	}
	return NIL;
}


void Set::SetSlot(int32 _index, AbstractVariable* _variable)
{
	if(_index >= 0 && _index < size)
	{
		if(array[_index])
		{
			delete array[_index];
			array[_index] = NIL;
		}

		if(_variable)
		{
			switch(_variable->GetValueType())
			{
				case Variable<int32>::INTEGER_TYPE:
				{
					array[_index] = new Variable<int32>(dynamic_cast<Variable<int32>*>(_variable)->GetValue());
					break;
				}

				case Variable<float>::FLOAT_TYPE:
				{
					array[_index] = new Variable<float>(dynamic_cast<Variable<float>*>(_variable)->GetValue());
					break;
				}

				case Variable<bool>::BOOLEAN_TYPE:
				{
					array[_index] = new Variable<bool>(dynamic_cast<Variable<bool>*>(_variable)->GetValue());
					break;
				}

				case Variable<StringANSI>::STRING_TYPE:
				{
					array[_index] = new Variable<StringANSI>(dynamic_cast<Variable<StringANSI>*>(_variable)->GetValue());
					break;
				}

				case Variable<Vector2>::VECTOR_2_TYPE:
				{
					array[_index] = new Variable<Vector2>(dynamic_cast<Variable<Vector2>*>(_variable)->GetValue());
					break;
				}

				case Variable<Vector3>::VECTOR_3_TYPE:
				{
					array[_index] = new Variable<Vector3>(dynamic_cast<Variable<Vector3>*>(_variable)->GetValue());
					break;
				}

				case Variable<Quaternion>::QUATERNION_TYPE:
				{
					array[_index] = new Variable<Quaternion>(dynamic_cast<Variable<Quaternion>*>(_variable)->GetValue());
					break;
				}
			}
		}
	}
}


int32 Set::GetSize(void)const
{
	return size;
}



bool Set::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		if(AbstractObject::SaveToFile(_file))
		{
			SaveDerivedToFile(_file);
			return true;
		}
	}
	return false;
}



bool Set::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true;  
}



bool Set::SaveAsToFile(File& _file, StringANSI _name)
{
	if(AbstractObject::SaveAsToFile(_file, _name))
	{ 
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}


void Set::SaveDerivedToFile(File& _file)
{
	_file.Write(size);
	for(int32 i = 0; i < size; i++)
	{
		if(array[i])
		{
			array[i]->SaveToFile(_file);
		}
	}
}



bool Set::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		LoadDerivedFromFile(_file);
		return true;
	} 
	return false;
}


void Set::LoadDerivedFromFile(File& _file)
{
	int32 size;
	_file.Read(size);
		
	Resize(size);
	for(int32 i = 0; i < size; i++)
	{
		AbstractVariable* variable = dynamic_cast<AbstractVariable*>(AssetLibrary::_LoadPrivateAsset<AbstractVariable>(_file));

		if(variable) { SetSlot(i, variable); } 
	}
}