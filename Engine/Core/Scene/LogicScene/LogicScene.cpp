#include "LogicScene.h"
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


LogicScene::LogicScene(void)
{
	updateList.scene = this;
	AddClassProperty(LOGIC_SCENE_CLASS);
}


LogicScene::~LogicScene(void)
{
	Destroy();
	Destroying();
} 


void LogicScene::Destroy(void)
{
	objectTree.Clear();
	updateList.Clear();
}
	

UpdateList::AddressDerived* LogicScene::AddToUpdateList(LogicObject* _object)
{
	return updateList.AddObject(_object);
}



void LogicScene::SetDeltaScale(float _scale)
{
	LogicObject::SetDeltaScale(_scale);

	UpdateDeltaScale(updateList.GetHeadAddress());
}


void LogicScene::UpdateDeltaScale(void)
{ 
	UpdateDeltaScale(updateList.GetHeadAddress());
}


void LogicScene::UpdateDeltaScale(MagicList<LogicObject>::AddressDerived* _address)const
{ 
	if(_address)
	{ 
		UpdateDeltaScale(_address->GetRight());

		_address->GetNode()->LogicObject::UpdateDeltaScale();

		if(_address->GetNode()->IsClassPropertyExist(LogicScene::LOGIC_SCENE_CLASS))
		{
			dynamic_cast<LogicScene*>(_address->GetNode())->LogicScene::UpdateDeltaScale();
		}
	}
} 



void LogicScene::RemoveFromUpdateList(LogicObject* _object)
{
	if(_object)
	{
		delete _object->GetAddress(&updateList);
	}
}



void LogicScene::Update(float _delta)
{
	if(!freezeEnable)
	{
		_delta *= GetDeltaScale();
		MagicList<LogicObject>::AddressDerived* it = updateList.GetHeadAddress();

		while(it)
		{
			it->GetNode()->Update(_delta);
			it = it->GetRight();
		}
	}
}



UpdateList& LogicScene::GetUpdateList(void)
{
	return updateList;
}


int32 LogicScene::GetAmountOfLogicObjects(void)const
{
	int32 size = 0;
	GetAmountOfLogicObjects(updateList.GetHeadAddress(), size);
	return size;
}


void LogicScene::GetAmountOfLogicObjects(MagicList<LogicObject>::AddressDerived* _address, int32& _size)const
{ 
	if(_address)
	{
		GetAmountOfLogicObjects(_address->GetRight(), _size);

		++_size;
		if(_address->GetNode()->IsClassPropertyExist(LogicScene::LOGIC_SCENE_CLASS))
		{
			LogicScene* scene = dynamic_cast<LogicScene*>(_address->GetNode());
			
			if(scene)
			{
				_size += scene->GetAmountOfLogicObjects();
			}
		}
	}
}



void LogicScene::TotalPolygonsDivide4(void)
{
	TotalPolygonsDivide4(objectTree.GetRootAddress());
}



void LogicScene::TotalPolygonsDivide4(MagicBinaryTree<AbstractObject>::AddressDerived* _address)
{ 
	if(_address)
	{
		TotalPolygonsDivide4(_address->GetChild(0));
		TotalPolygonsDivide4(_address->GetChild(1));
		if(_address->GetNode()->IsClassPropertyExist(Mesh::MESH_CLASS))
		{
			Mesh* itDerived = dynamic_cast<Mesh*>(_address->GetNode());
			if(itDerived) { itDerived->TotalPolygonsDivide4(); }
		}
	}
}



bool LogicScene::SaveToFile(StringANSI _path)
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



bool LogicScene::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write((int64)LOGIC_SCENE_CLASS);

		LogicObject::SaveDerivedToFile(_file);

		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



bool LogicScene::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + ObjectScene::_GetDir() + _name + "." + ObjectScene::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}




bool LogicScene::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write((int64)LOGIC_SCENE_CLASS);

		LogicObject::SaveDerivedToFile(_file);

		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}




void LogicScene::SaveDerivedToFile(File& _file)
{
	_file.Write(objectTree.GetSize());
	SaveObjectToFile(objectTree.GetRootAddress(), _file);
	_file.Write(updateList.GetSize());
	SaveLogicObjectToFile(updateList.GetHeadAddress(), _file);
}




void LogicScene::SaveLogicObjectToFile(MagicList<LogicObject>::AddressDerived* _address, File& _file)
{ 
	if(_address)
	{
		LogicObject* object = _address->GetNode();
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

		SaveLogicObjectToFile(_address->GetRight(), _file);
	}
}



bool LogicScene::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}


bool LogicScene::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		LogicObject::LoadDerivedFromFile(_file);

		LoadDerivedFromFile(_file);
		return true;
	}
	return false;
}


void LogicScene::LoadDerivedFromFile(File& _file)
{
	Destroy();
	int32 amountOfObjects;
	_file.Read(amountOfObjects);
	for(int32 i = 0; i < amountOfObjects; ++i)
	{
		LoadObjectFromFile(_file);
	}
	int32 amountOfLogicObjects;
	_file.Read(amountOfLogicObjects);
	for(int32 i = 0; i < amountOfLogicObjects; ++i)
	{
		LoadLogicObjectFromFile(_file);
	}
}



void LogicScene::LoadLogicObjectFromFile(File& _file)
{ 
	LogicObject* object;
	int32 assetType;
	_file.Read(assetType);
	bool isCommonAsset;
	_file.Read(isCommonAsset);
	
	if(isCommonAsset)
	{
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);

		object = dynamic_cast<LogicObject*>(AssetLibrary::_LoadCommonAsset(name, assetType));
	}
	else
	{ 
		object = dynamic_cast<LogicObject*>(AssetLibrary::_LoadPrivateAsset(_file, assetType));
	}

	if(object)
	{
		AddToUpdateList(object);
	}
}

