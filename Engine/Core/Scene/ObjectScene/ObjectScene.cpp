#include "ObjectScene.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>


StringANSI ObjectScene::scenesSuffix = "scene";
StringANSI ObjectScene::scenesDir = "scenes/";



ObjectScene::ObjectScene(void)
{
	objectTree.scene = this;
	AddClassProperty(OBJECT_SCENE_CLASS);
}


ObjectScene::~ObjectScene(void)
{
	Destroy();
	Destroying();
}


ObjectBinaryTree::AddressDerived* ObjectScene::AddToLayer(AbstractObject* _object)
{
	return objectTree.AddObject(_object);
}



void ObjectScene::Destroy(void)
{
	objectTree.Clear();
}

	

void ObjectScene::RemoveFromLayer(AbstractObject* _object)
{
	if(_object)
	{
		delete _object->GetAddress(&objectTree);
	}
}



AbstractObject* ObjectScene::FindObject(StringANSI _name)
{
	return objectTree.Find(_name, &AbstractObject::GetName);
}




ObjectTree& ObjectScene::GetObjectTree(void)
{
	return objectTree;
}



int32 ObjectScene::GetAmountOfObjects(void)const
{
	int32 size = 0;
	GetAmountOfObjects(objectTree.GetRootAddress(), size);
	return size;
}


void ObjectScene::GetAmountOfObjects(MagicBinaryTree<AbstractObject>::AddressDerived* _address, int32& _size)const
{ 
	if(_address)
	{
		++_size;

		AbstractObject* object = _address->GetNode();
		if(object->IsClassPropertyExist(ObjectScene::OBJECT_SCENE_CLASS))
		{
			ObjectScene* scene = dynamic_cast<ObjectScene*>(object);
			
			if(scene)
			{
				_size += scene->GetAmountOfObjects();
			}
		}
		GetAmountOfObjects(_address->GetChild(0), _size);
		GetAmountOfObjects(_address->GetChild(1), _size);
	}
}



void ObjectScene::_SetFileSuffix(StringANSI _suffix)
{
	scenesSuffix = _suffix;
}


StringANSI ObjectScene::_GetFileSuffix(void)
{
	return scenesSuffix;
}


StringANSI ObjectScene::_GetDir(void)
{
	return scenesDir;
}


bool ObjectScene::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + scenesDir + GetName() + "." + scenesSuffix;
	} 

	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool ObjectScene::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write((int64)OBJECT_SCENE_CLASS);

		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}


bool ObjectScene::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + ObjectScene::_GetDir() + _name + "." + ObjectScene::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool ObjectScene::SaveAsToFile(File& _file, StringANSI _name)
{  
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write((int64)OBJECT_SCENE_CLASS);

		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}
 


void ObjectScene::SaveDerivedToFile(File& _file)
{
	_file.Write(objectTree.GetSize());
	SaveObjectToFile(objectTree.GetRootAddress(), _file);
}



void ObjectScene::SaveObjectToFile(MagicBinaryTree<AbstractObject>::AddressDerived* _address, File& _file)
{ 
	if(_address)
	{
		AbstractObject* object = _address->GetNode();
		int32 assetType = AssetLibrary::_GetAssetType(object->GetClassProperties());
		_file.Write(assetType);
		bool isCommonAsset = object->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
		_file.Write(isCommonAsset);

		if(isCommonAsset)
		{
			if(!AssetLibrary::_IsAssetExist(object->GetName(), assetType))
			{
				object->SaveToFile();
			}
			_file.Write(object->GetName().length());
			_file.WriteString(object->GetName());
		}
		else
		{
			object->SaveToFile(_file); 
		}

		SaveObjectToFile(_address->GetChild(0), _file);
		SaveObjectToFile(_address->GetChild(1), _file);
	}
}



bool ObjectScene::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}



bool ObjectScene::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		LoadDerivedFromFile(_file);
		return true;
	} 
	return false;
}


void ObjectScene::LoadDerivedFromFile(File& _file)
{
	Destroy();
	int32 amountOfObjects;
	_file.Read(amountOfObjects);
	for(int32 i = 0; i < amountOfObjects; ++i)
	{
		LoadObjectFromFile(_file);
	}
}



void ObjectScene::LoadObjectFromFile(File& _file)
{
	int32 assetType;
	_file.Read(assetType);
	bool isCommonAsset;
	_file.Read(isCommonAsset);
	AbstractObject* object;
			
	if(isCommonAsset)
	{
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);

		object = AssetLibrary::_LoadCommonAsset(name, assetType);
	}
	else
	{
		object = AssetLibrary::_LoadPrivateAsset(_file, assetType);
	}

	if(object)
	{
		AddToLayer(object);
	}
}


ObjectScene* ObjectScene::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + scenesDir + _path + "." + scenesSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}
 


ObjectScene* ObjectScene::_LoadFromFile(File& _file)
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
		int64 type;
		_file.Read(type);

		ObjectScene* scene; 
		
		switch(type)
		{
			case Scene::SCENE_CLASS:
			{
				scene = new Scene();
				break;
			}

			case LogicScene::LOGIC_SCENE_CLASS:
			{
				scene = new LogicScene();
				break;
			}

			case GraphicsScene::GRAPHICS_SCENE_CLASS:
			{
				scene = new GraphicsScene();
				break;
			}

			case ObjectScene::OBJECT_SCENE_CLASS:
			{
				scene = new ObjectScene();
				break;
			}

			case TiledSpace3D::TILED_SPACE_3D_CLASS:
			{
				scene = new TiledSpace3D();
				break;
			}

			default:
			{
				scene = NIL;
				break;
			}
		}
		
		if(scene)
		{ 
			scene->LoadFromFile(_file);
			scene->SetProperties(properties);
			scene->Rename(name);
			return scene;
		}
	}
	return NIL;
}

