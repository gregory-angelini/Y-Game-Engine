#include "AbstractObject.h"
#include <stdarg.h>
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Output/Graphics/Surface/GraphicsSurface/GraphicsSurface.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Object/AbstractObject/Class.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Parser/List/IntegerList/IntegerList.h> 
#include <Engine/Core/Parser/List/FloatList/FloatList.h> 
#include <Engine/Core/Parser/List/BooleanList/BooleanList.h> 
#include <Engine/Core/Parser/List/StringList/StringList.h> 
#include <Engine/Core/Parser/List/Vector2List/Vector2List.h> 
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h> 
#include <Engine/Core/Parser/List/QuaternionList/QuaternionList.h> 
#include <Engine/Core/Parser/List/ColorableObjectList/ColorableObjectList.h> 
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Core/Parser/Variable/IntegerVariable/IntegerVariable.h>
#include <Engine/Core/Parser/Variable/FloatVariable/FloatVariable.h>
#include <Engine/Core/Parser/Variable/BooleanVariable/BooleanVariable.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>
#include <Engine/Core/Parser/Variable/Vector2Variable/Vector2Variable.h>
#include <Engine/Core/Parser/Variable/Vector3Variable/Vector3Variable.h>
#include <Engine/Core/Parser/Variable/QuaternionVariable/QuaternionVariable.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsStrip/GraphicsStrip.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>
#include <Engine/Core/Parser/Set/Set.h>
#include <Engine/Core/Parser/List/SetList/SetList.h>
StringANSI AbstractObject::objectsSuffix = "object";
StringANSI AbstractObject::objectsDir = "objects/";
StringANSI AbstractObject::logFileDir = "Errors/";
StringANSI AbstractObject::logFileSuffix = ".log";
AbstractObject::ObjectTypeProperties AbstractObject::supportedProperties;


#ifdef SendMessage
   #undef SendMessage
#endif



AbstractObject::AbstractObject(void): objectType(0), renaming(false), properties(0)
{
	AddClassProperty(ABSTRACT_OBJECT_CLASS);
}


AbstractObject::~AbstractObject(void)
{
	Destroying();
}



void AbstractObject::_SetLogFileDir(StringANSI _directory)
{
	AbstractObject::logFileDir = _directory;
}


StringANSI AbstractObject::_GetLogFileDir(void)
{
	return AbstractObject::logFileDir;
}


void AbstractObject::_SetLogFileSuffix(StringANSI _suffix)
{
	AbstractObject::logFileSuffix = _suffix;
}


StringANSI AbstractObject::_GetLogFileSuffix(void)
{
	return AbstractObject::logFileSuffix;
}


StringANSI AbstractObject::GetName(void)const
{
	return name;
}


void AbstractObject::Destroying(void)
{ 
	if(IsReceiversExist(AbstractObject::DESTROY_OBJECT_MESSAGE))
	{ 
		SendPackage(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
	}
	DisconnectAllReceivers();
}


bool AbstractObject::Rename(StringANSI _newName)
{
	if(name != _newName)
	{
		if(IsReceiversExist(AbstractObject::CHANGE_NAME_MESSAGE))
		{
			RenameInfo renameInfo(_newName, false);
		
			SendPackage(AbstractObject::CHECK_OBJECT_NAME_MESSAGE, &renameInfo);

			if(!renameInfo.reserved)
			{ 
				renaming = true;
				NameInfo nameInfo(name, _newName, this);
				
				SendPackage(AbstractObject::CHANGE_NAME_MESSAGE, &nameInfo);
				
				name = _newName;
				renaming = false;
				return true;
			}
			return false;
		}
		else
		{ 
			name = _newName;
		} 
	}
	return true;	
}


StringANSI AbstractObject::GetLogFile(void)
{
	return logFileDir + GetName() + logFileSuffix; 
}


StringANSI AbstractObject::_GetLogFile(StringANSI _name)
{
	return logFileDir + _name + logFileSuffix; 
}


void AbstractObject::RemoveLogFile(void)
{
	File::_Remove(GetLogFile());
}


void AbstractObject::_RemoveLogFile(StringANSI _name)
{
	File::_Remove(_GetLogFile(_name));
}


int32 AbstractObject::WriteToLogFile(StringANSI _string, ...)
{
	if(!File::_IsExist(logFileDir))
	{
		File::_CreateFolder(logFileDir);
	}
	
	File file(GetLogFile());

	if(file.Open(File::WRITE))
	{
		va_list argList;
		va_start(argList, _string);
		int32 symbols = vfprintf(file.GetFile(), _string.c_str(), argList);
		va_end(argList);

		if(symbols > 0) { file.WriteString("\r\n"); }
		return symbols;
	}
	return -1;
}


int32 AbstractObject::_WriteToLogFile(StringANSI _fileName, StringANSI _string, ...)
{
	if(!File::_IsExist(logFileDir))
	{
		File::_CreateFolder(logFileDir);
	}
			
	File file(_GetLogFile(_fileName));

	if(file.Open(File::WRITE))
	{
		va_list argList;
		va_start(argList, _string);
		int32 symbols = vfprintf(file.GetFile(), _string.c_str(), argList);
		va_end(argList);

		if(symbols > 0) { file.WriteString("\r\n"); }
		return symbols;
	}
	return -1;
}




void AbstractObject::Destroy(void)
{}



int64 AbstractObject::GetClassProperties(void)
{
	return objectType;
}



void AbstractObject::AddClassProperty(int64 _property)
{
	objectType |= _property;
} 

 
bool AbstractObject::IsClassPropertyExist(int64 _property)const
{
	return objectType & _property;
}


StringANSI AbstractObject::GetClassPropertiesString(void)const
{
	return Bitset::_ToString(objectType);
}



StringANSI AbstractObject::_GetDir(void)
{
	return objectsDir;
}


void AbstractObject::_SetFileSuffix(StringANSI _suffix)
{
	objectsSuffix = _suffix;
}


StringANSI AbstractObject::_GetFileSuffix(void)
{
	return objectsSuffix;
}



void AbstractObject::EnableProperty(int32 _property, bool _enable)
{
	if(_property >= 0 && _property < (sizeof(int64) * Memory::BIT_IN_BYTE))
	{
		if(_enable)
		{
			properties |= (int64(1) << _property);

			if(IsReceiversExist(ENABLE_PROPERTY_MESSAGE)) { SendPackage(ENABLE_PROPERTY_MESSAGE, &PropertyInfo(_property, true)); }
		}
		else
		{
			properties &= ~(int64(1) << _property);

			if(IsReceiversExist(ENABLE_PROPERTY_MESSAGE)) { SendPackage(ENABLE_PROPERTY_MESSAGE, &PropertyInfo(_property, false)); }
		}
	}
}



bool AbstractObject::IsPropertyExist(int32 _property)const
{
	if(_property >= 0 && _property < sizeof(int64) * Memory::BIT_IN_BYTE)
	{
		return (properties & (int64(1) << _property) ? true : false);
	}
	return false;
}



int64 AbstractObject::GetProperties(void)
{
	return properties;
}


void AbstractObject::SetProperties(int64 _properties)
{
	properties = _properties;
}


bool AbstractObject::_IsChild(int64 _parent, int64 _child)
{
	ObjectTypeProperty* parent = supportedProperties.Find(_parent, &ObjectTypeProperty::GetType);
	ObjectTypeProperty* child = supportedProperties.Find(_child, &ObjectTypeProperty::GetType);

	if(parent && child)
	{
		if(parent->IsChild(child))
		{
			return true;
		}
	}
	return false;
}


 
bool AbstractObject::_IsCompatible(int64 _property1, int64 _property2)
{
	ObjectTypeProperty* property1 = supportedProperties.Find(_property1, &ObjectTypeProperty::GetType);
	ObjectTypeProperty* property2 = supportedProperties.Find(_property2, &ObjectTypeProperty::GetType);

	if(property1 && property2)
	{
		if(property1->IsChild(property2))
		{
			return true;
		}
		else if(property2->IsChild(property1))
		{
			return true;
		}
		else
		{
			int64 type = _property1 | _property2;
			
			switch(type)
			{
				case LogicScene::LOGIC_SCENE_CLASS | GraphicsScene::GRAPHICS_SCENE_CLASS:
				case GraphicsSurface::GRAPHICS_SURFACE_CLASS | Timer::TIMER_CLASS: 
				case GraphicsSurface::GRAPHICS_SURFACE_CLASS | Scene::SCENE_CLASS:
				case TexturingSurface::TEXTURING_SURFACE_CLASS | Timer::TIMER_CLASS:
				case TexturingSurface::TEXTURING_SURFACE_CLASS | Scene::SCENE_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | Variable<int32>::INTEGER_VARIABLE_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | Variable<float>::FLOAT_VARIABLE_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | Variable<bool>::BOOLEAN_VARIABLE_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | Variable<StringANSI>::STRING_VARIABLE_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | Variable<Vector2>::VECTOR_2_VARIABLE_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | Variable<Vector3>::VECTOR_3_VARIABLE_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | Variable<Quaternion>::QUATERNION_VARIABLE_CLASS:
					
				case AbstractList::ABSTRACT_LIST_CLASS | TranslatableObject::TRANSLATABLE_OBJECT_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | ScalableObject::SCALABLE_OBJECT_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | RotatableObject::ROTATABLE_OBJECT_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | ColorableObject::COLORABLE_OBJECT_CLASS:
				case AbstractList::ABSTRACT_LIST_CLASS | Set::SET_CLASS:
				{
					return true;
				}
			}
		}
	}
	return false;
}


bool AbstractObject::_IsSupported(int64 _properties)
{
	ObjectType objectType = _CreateObjectType(_properties);

	switch(objectType.GetProperties())
	{
		case AbstractObject::ABSTRACT_OBJECT_CLASS:
		case Timer::TIMER_CLASS:
		case Plane::PLANE_CLASS:
		case Set::SET_CLASS:
		case GraphicsStrip::GRAPHICS_STRIP_CLASS:
		case GraphicsSurface::GRAPHICS_SURFACE_CLASS:
		case TexturingSurface::TEXTURING_SURFACE_CLASS:
		case Trigger::TRIGGER_CLASS:
			case AbstractVariable::ABSTRACT_VARIABLE_CLASS:
				case Variable<int32>::INTEGER_VARIABLE_CLASS:
				case Variable<float>::FLOAT_VARIABLE_CLASS:
				case Variable<bool>::BOOLEAN_VARIABLE_CLASS:
				case Variable<StringANSI>::STRING_VARIABLE_CLASS:
				case Variable<Vector2>::VECTOR_2_VARIABLE_CLASS:
				case Variable<Vector3>::VECTOR_3_VARIABLE_CLASS:
				case Variable<Quaternion>::QUATERNION_VARIABLE_CLASS:
			case AbstractEvent::EVENT_CLASS:
			case AbstractAction::ACTION_CLASS:
		case ObjectScene::OBJECT_SCENE_CLASS:
		case LogicScene::LOGIC_SCENE_CLASS:
		case GraphicsScene::GRAPHICS_SCENE_CLASS:
		case Scene::SCENE_CLASS:
		case TiledSpace3D::TILED_SPACE_3D_CLASS:
		case PlayerControl::PLAYER_CONTROL_CLASS:
		case Mesh::MESH_CLASS:
	    case Hitbox::HITBOX_CLASS:
		case TexturedAnimation::TEXTURED_ANIMATION_CLASS:
			case AbstractAnimation::ABSTRACT_ANIMATION_CLASS:
		case Atlas2D::ATLAS_2D_CLASS:
		case Texture2D::TEXTURE_2D_CLASS:
		case Camera3D::CAMERA_3D_CLASS:
		case FloatLerp::FLOAT_LERP_CLASS:
		case QuaternionLerp::QUATERNION_LERP_CLASS:
		case VectorLerp::VECTOR_LERP_CLASS:
		case AbstractList::ABSTRACT_LIST_CLASS:
		   case List<AbstractObject>::OBJECT_LIST_CLASS: 
		   case List<TranslatableObject>::TRANSLATABLE_LIST_CLASS:
		   case List<ScalableObject>::SCALABLE_LIST_CLASS:
		   case List<RotatableObject>::ROTATABLE_LIST_CLASS:
		   case List<ColorableObject>::COLORABLE_LIST_CLASS:
		   case List<Set>::SET_LIST_CLASS: 
		   case List<int32>::INTEGER_LIST_CLASS:
		   case List<float>::FLOAT_LIST_CLASS:
		   case List<bool>::BOOLEAN_LIST_CLASS:
		   case List<StringANSI>::STRING_LIST_CLASS:
		   case List<Vector2>::VECTOR_2_LIST_CLASS:
		   case List<Vector3>::VECTOR_3_LIST_CLASS:
		   case List<Quaternion>::QUATERNION_LIST_CLASS:
		case AbstractList::ABSTRACT_LIST_CLASS | Variable<int32>::INTEGER_VARIABLE_CLASS:
		case AbstractList::ABSTRACT_LIST_CLASS | Variable<float>::FLOAT_VARIABLE_CLASS:
		case AbstractList::ABSTRACT_LIST_CLASS | Variable<bool>::BOOLEAN_VARIABLE_CLASS:
		case AbstractList::ABSTRACT_LIST_CLASS | Variable<StringANSI>::STRING_VARIABLE_CLASS:
		case AbstractList::ABSTRACT_LIST_CLASS | Variable<Vector2>::VECTOR_2_VARIABLE_CLASS:
		case AbstractList::ABSTRACT_LIST_CLASS | Variable<Vector3>::VECTOR_3_VARIABLE_CLASS:
		case AbstractList::ABSTRACT_LIST_CLASS | Variable<Quaternion>::QUATERNION_VARIABLE_CLASS:

		case AbstractList::ABSTRACT_LIST_CLASS | TranslatableObject::TRANSLATABLE_OBJECT_CLASS:
		case AbstractList::ABSTRACT_LIST_CLASS | ScalableObject::SCALABLE_OBJECT_CLASS: 
		case AbstractList::ABSTRACT_LIST_CLASS | RotatableObject::ROTATABLE_OBJECT_CLASS: 
		case AbstractList::ABSTRACT_LIST_CLASS | ColorableObject::COLORABLE_OBJECT_CLASS: 
		case AbstractList::ABSTRACT_LIST_CLASS | Set::SET_CLASS:
		case LogicScene::LOGIC_SCENE_CLASS | GraphicsScene::GRAPHICS_SCENE_CLASS:
		case GraphicsSurface::GRAPHICS_SURFACE_CLASS | Timer::TIMER_CLASS:
		case GraphicsSurface::GRAPHICS_SURFACE_CLASS | Scene::SCENE_CLASS:

		case TexturingSurface::TEXTURING_SURFACE_CLASS | Timer::TIMER_CLASS:
		case TexturingSurface::TEXTURING_SURFACE_CLASS | Scene::SCENE_CLASS:
		{
			return true;
		}
	} 
	return false;
}  



void AbstractObject::_RemoveLowPriorityProperties(MagicList<ObjectTypeProperty>& _properties)
{ 
	MagicList<ObjectTypeProperty> checkingProperties = _properties;
	while(checkingProperties.IsNotEmpty())
	{
		ObjectTypeProperty* it = checkingProperties.GetHead();

		if(_IsLowPriority(it, _properties))
		{
			_properties.Remove(it, MagicContainer::REMOVE_NODE);
		}
		checkingProperties.RemoveHead();
	}
}
	 



bool AbstractObject::_IsLowPriority(ObjectTypeProperty* _property, MagicList<ObjectTypeProperty>& _properties)
{
	MagicList<ObjectTypeProperty>::AddressDerived* it = _properties.GetHeadAddress();
		
	while(it)
	{
		if(it->GetNode() != _property)
		{
			if(it->GetNode()->IsChild(_property))
			{
				return true;
			}
		}
		it = it->GetRight();
	}
	return false;
}



AbstractObject::ObjectType AbstractObject::_CreateObjectType(int64 _properties)
{
	MagicList<ObjectTypeProperty> properties; 

	if(_ToList(_properties, properties))
	{
		_RemoveLowPriorityProperties(properties);
		int64 objectType = 0;
		MagicList<ObjectTypeProperty>::AddressDerived* it = properties.GetHeadAddress();
			
		while(it)
		{
			objectType |= it->GetNode()->GetType();
			it = it->GetRight();
		}
		return ObjectType(objectType);
	}
	return ObjectType();
}


bool AbstractObject::_ToList(int64 _type, MagicList<ObjectTypeProperty>& _properties)
{
	if(_type != 0)
	{
		int32 count = Bitset::_GetSize(_type);
		int64 subType;
		for(int32 i = 0; i < count; ++i)
		{
			if(Bitset::_GetBit(_type, i))
			{
				subType = 0; Bitset::_SetBit(subType, i, true);
				if(_properties.AddToTail(supportedProperties.Find(subType, &ObjectTypeProperty::GetType)) == NIL)
				{ 
					_properties.Clear();
					return false;
				}
			}
		}
		return true;
	}
	return false;
}


AbstractObject* AbstractObject::_CreateObject(ObjectType _classType)
{
	switch(_classType.GetProperties())
	{
		case AbstractObject::ABSTRACT_OBJECT_CLASS:
		{
			return new Class1<AbstractObject>();
		}
		case Timer::TIMER_CLASS:
		{
			return new Class1<Timer>();
		}
		case Plane::PLANE_CLASS:
		{
			return new Class1<Plane>();
		}
		case Set::SET_CLASS:
		{
			return new Class1<Set>();
		}
		case GraphicsStrip::GRAPHICS_STRIP_CLASS:
		{
			return new Class1<GraphicsStrip>();
		}
		case GraphicsSurface::GRAPHICS_SURFACE_CLASS:
		{
			return new Class1<GraphicsSurface>();
		}
		case TexturingSurface::TEXTURING_SURFACE_CLASS:
		{
			return new Class1<TexturingSurface>();
		}
		case AbstractList::ABSTRACT_LIST_CLASS | Variable<int32>::INTEGER_VARIABLE_CLASS:
		{
			return new Class1<List<int32>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | Variable<float>::FLOAT_VARIABLE_CLASS:
		{
			return new Class1<List<float>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | Variable<bool>::BOOLEAN_VARIABLE_CLASS:
		{
			return new Class1<List<bool>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | Variable<StringANSI>::STRING_VARIABLE_CLASS:
		{
			return new Class1<List<StringANSI>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | Variable<Vector2>::VECTOR_2_VARIABLE_CLASS:
		{
			return new Class1<List<Vector2>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | Variable<Vector3>::VECTOR_3_VARIABLE_CLASS:
		{
			return new Class1<List<Vector3>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | Variable<Quaternion>::QUATERNION_VARIABLE_CLASS:
		{
			return new Class1<List<Quaternion>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | TranslatableObject::TRANSLATABLE_OBJECT_CLASS:
		{
			return new Class1<List<TranslatableObject>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | ScalableObject::SCALABLE_OBJECT_CLASS:
		{
			return new Class1<List<ScalableObject>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | RotatableObject::ROTATABLE_OBJECT_CLASS:
		{
			return new Class1<List<RotatableObject>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | ColorableObject::COLORABLE_OBJECT_CLASS:
		{
			return new Class1<List<ColorableObject>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS | Set::SET_CLASS:
		{
			return new Class1<List<Set>>();
		}

		case AbstractList::ABSTRACT_LIST_CLASS:
		{
			return new Class1<List<AbstractObject>>();
		}
		case LogicScene::LOGIC_SCENE_CLASS | GraphicsScene::GRAPHICS_SCENE_CLASS:
		{ 
			return new Class1<Scene>();
		}
		case GraphicsSurface::GRAPHICS_SURFACE_CLASS | Timer::TIMER_CLASS:
		{
			return new Class2<GraphicsSurface, Timer>();
		}
		case GraphicsSurface::GRAPHICS_SURFACE_CLASS | Scene::SCENE_CLASS:
		{
			return new Class2<GraphicsSurface, Scene>();
		}

		case TexturingSurface::TEXTURING_SURFACE_CLASS | Timer::TIMER_CLASS:
		{
			return new Class2<TexturingSurface, Timer>();
		}
		case TexturingSurface::TEXTURING_SURFACE_CLASS | Scene::SCENE_CLASS:
		{
			return new Class2<TexturingSurface, Scene>();
		}
	}
	return NIL;
}



bool AbstractObject::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}

				
bool AbstractObject::LoadFromFile(File& _file)
{
	return false;
}



bool AbstractObject::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + objectsDir + name + "." + objectsSuffix;
	} 

	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}




bool AbstractObject::SaveToFile(File& _file)
{
	ObjectType type = _CreateObjectType(objectType);
	if(type.GetProperties() == 0) { return false; }

	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(name.length());
		_file.WriteString(name);
		_file.Write(type);
		return true;
	}
	return false;
}




bool AbstractObject::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool AbstractObject::SaveAsToFile(File& _file, StringANSI _name)
{
	ObjectType type = _CreateObjectType(objectType);
	if(type.GetProperties() == 0) { return false; }

	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(type);
		return true;
	}
	return false;
}



AbstractObject* AbstractObject::_LoadFromFile(StringANSI _path, bool _auto)
{ 
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + objectsDir + _path + "." + objectsSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


AbstractObject* AbstractObject::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		int32 version;
		_file.Read(version);
		int64 properties;
		_file.Read(properties);
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		AbstractObject::ObjectType type;
		_file.Read(type);

		AbstractObject* object = AbstractObject::_CreateObject(type);
		
		if(object) 
		{
			object->LoadFromFile(_file);
			object->SetProperties(properties);
			object->Rename(name);
			return object;
		}
	}
	return NIL;
}
