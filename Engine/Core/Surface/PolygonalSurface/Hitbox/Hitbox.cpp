#include "Hitbox.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


#ifdef SendMessage
   #undef SendMessage
#endif
StringANSI Hitbox::hitboxesSuffix = "hitbox";
StringANSI Hitbox::hitboxesDir = "/hitboxes/";



Hitbox::Hitbox(void)
{
	AddClassProperty(HITBOX_CLASS);
}



Hitbox::~Hitbox(void)
{
	Destroy();
	Destroying();
}


	
bool Hitbox::IsExist(void)const
{
	return shapes.IsNotEmpty();
}


void Hitbox::Destroy(void)
{
	if(IsExist())
	{
		if(IsReceiversExist(DESTROY_HITBOX_MESSAGE)) { SendMessage(DESTROY_HITBOX_MESSAGE); }
		shapes.Clear();
	}
}


bool Hitbox::AddShape(Mesh* _mesh, StringANSI _name, int32 _collisionGroupIndex, bool _collisionEnable, bool _collisionPolygonsInfoEnable)
{
	if(_mesh)
	{
		shapes.AddToTail(new ShapeInfo(_mesh, _name, _collisionGroupIndex, _collisionEnable, _collisionPolygonsInfoEnable));
		
		if(IsReceiversExist(INIT_HITBOX_MESSAGE)) { SendMessage(INIT_HITBOX_MESSAGE); }
		return true;
	}
	return false;
}



MagicList<Hitbox::ShapeInfo>& Hitbox::GetShapes(void)
{
	return shapes;
}



bool Hitbox::RemoveShape(StringANSI _name)
{
	MagicList<ShapeInfo>::AddressDerived* it = shapes.GetHeadAddress();

	while(it)
	{
		if(it->GetNode()->name == _name)
		{
			delete it->GetNode();
			 
			if(shapes.IsEmpty())
			{
				if(IsReceiversExist(DESTROY_HITBOX_MESSAGE)) { SendMessage(DESTROY_HITBOX_MESSAGE); }
			}
			else
			{
				if(IsReceiversExist(INIT_HITBOX_MESSAGE)) { SendMessage(INIT_HITBOX_MESSAGE); }
			}
			return true;
		}
		it = it->GetRight();
	}
	return false;
}




StringANSI Hitbox::_GetDir(void)
{
	return hitboxesDir;
}


void Hitbox::_SetFileSuffix(StringANSI _suffix)
{
	hitboxesSuffix = _suffix;
}


StringANSI Hitbox::_GetFileSuffix(void)
{
	return hitboxesSuffix;
}




bool Hitbox::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + hitboxesDir + GetName() + "." + hitboxesSuffix;
	}

	File file(_path, File::REWRITE);
		
	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Hitbox::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(shapes.GetSize());
		MagicList<ShapeInfo>::AddressDerived* it = shapes.GetHeadAddress();
		
		while(it)
		{
			it->GetNode()->SaveToFile(_file);
			it = it->GetRight();
		}
		return true;
	}
	return false;
}


bool Hitbox::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + Hitbox::_GetDir() + _name + "." + Hitbox::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Hitbox::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(shapes.GetSize());
		MagicList<ShapeInfo>::AddressDerived* it = shapes.GetHeadAddress();
		
		while(it)
		{
			it->GetNode()->SaveToFile(_file);
			it = it->GetRight();
		}
		return true;
	}
	return false;
}




bool Hitbox::LoadFromFile(StringANSI _path, bool _auto)
{ 
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + hitboxesDir + _path + "." + hitboxesSuffix;
	}
	
	File file(_path, File::READ);
	
	return LoadFromFile(file);
}



bool Hitbox::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		Destroy();
		int32 version;
		_file.Read(version);
		_file.Read(properties);
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		Rename(name);
		int32 size;
		_file.Read(size);
		for(int32 i = 0; i < size; i++)
		{
			ShapeInfo* shape = new ShapeInfo();
			shape->LoadFromFile(_file);
			shapes.AddToTail(shape);
		}

		if(IsReceiversExist(INIT_HITBOX_MESSAGE)) { SendMessage(INIT_HITBOX_MESSAGE); }
		return true;
	} 
	return false;
}



Hitbox* Hitbox::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + hitboxesDir + _path + "." + hitboxesSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


Hitbox* Hitbox::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		Hitbox* hitbox = new Hitbox();

		if(hitbox->LoadFromFile(_file))
		{
			return hitbox;
		}
		delete hitbox;
	}
	return NIL;
}
