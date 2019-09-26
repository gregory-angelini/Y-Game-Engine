#include "Scene.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>




Scene* Scene::pickedScene = NIL;



Scene::Scene(void)
{
	AddClassProperty(SCENE_CLASS);
}



Scene::~Scene(void)
{
	Destroy();
	Destroying();
}


void Scene::Destroy(void)
{
	if(IsPicked()) { Scene::_Unpick(); }

	objectTree.Clear();
	renderList.Clear();
	updateList.Clear();
}



bool Scene::IsPicked(void)const
{
	return (Scene::pickedScene == this);
}


void Scene::Pick(void)
{
	if(Scene::pickedScene != this)
	{
		Scene::_Unpick();

		Scene::pickedScene = this;

		if(IsReceiversExist(PICK_SCENE_MESSAGE)) { SendPackage(PICK_SCENE_MESSAGE, this); }
	}
}


void Scene::_Unpick(void)
{
	if(Scene::pickedScene)
	{
		Scene::pickedScene = NIL;
	}
}


Scene* Scene::_GetPicked(void)
{
	return Scene::pickedScene;
}



bool Scene::SaveToFile(StringANSI _path)
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



bool Scene::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write((int64)SCENE_CLASS);

		LogicObject::SaveDerivedToFile(_file);
		
		GraphicsObject::SaveDerivedToFile(_file);
		
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}




bool Scene::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + ObjectScene::_GetDir() + _name + "." + ObjectScene::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Scene::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write((int64)SCENE_CLASS);

		LogicObject::SaveDerivedToFile(_file);
		
		GraphicsObject::SaveDerivedToFile(_file);
		
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



void Scene::SaveDerivedToFile(File& _file)
{
	_file.Write(objectTree.GetSize());
	SaveObjectToFile(objectTree.GetRootAddress(), _file);
	_file.Write(updateList.GetSize());
	SaveLogicObjectToFile(updateList.GetHeadAddress(), _file);
	_file.Write(renderList.GetSize());
	SaveGraphicsObjectToFile(renderList.GetHeadAddress(), _file);
	_file.Write(camera.IsNotEmpty());
			
	if(camera.IsNotEmpty())
	{
		bool cameraIsCommonAsset = camera.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
		_file.Write(cameraIsCommonAsset);
		if(cameraIsCommonAsset)
		{
			if(!AssetLibrary::_IsAssetExist(camera.Get()->GetName(), AssetLibrary::CAMERA_ASSET))
			{
				camera.Get()->SaveToFile();
			}
			_file.Write(camera.Get()->GetName().length());
			_file.WriteString(camera.Get()->GetName());
		}
		else
		{
			camera.Get()->SaveToFile(_file);
		}
	}
	_file.Write(texture.IsNotEmpty());
	
	if(texture.IsNotEmpty())
	{
		bool textureIsCommonAsset = texture.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
		_file.Write(textureIsCommonAsset);

		if(textureIsCommonAsset)
		{
			if(!AssetLibrary::_IsAssetExist(texture.Get()->GetName(), AssetLibrary::TEXTURE_2D_ASSET))
			{
				texture.Get()->SaveToFile();
			}
			_file.Write(texture.Get()->GetName().length());
			_file.WriteString(texture.Get()->GetName());
		}
		else
		{
			texture.Get()->SaveToFile(_file);
		}
	}
	_file.Write(IsPicked());
}



void Scene::LoadDerivedFromFile(File& _file)
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
	int32 amountOfGraphicsObjects;
	_file.Read(amountOfGraphicsObjects);
	for(int32 i = 0; i < amountOfGraphicsObjects; ++i)
	{
		LoadGraphicsObjectFromFile(_file);
	}
	bool cameraIsExist;
	_file.Read(cameraIsExist);
		
	if(cameraIsExist)
	{
		bool cameraIsCommonAsset;
		_file.Read(cameraIsCommonAsset);
		Camera3D* buildInCamera;
				
		if(cameraIsCommonAsset)
		{
			StringANSI name;
			int32 length;
			_file.Read(length);
			_file.ReadString(name, length);

			buildInCamera = dynamic_cast<Camera3D*>(AssetLibrary::_LoadCommonAsset<Camera3D>(name));
		}
		else
		{
			buildInCamera = dynamic_cast<Camera3D*>(AssetLibrary::_LoadPrivateAsset<Camera3D>(_file));
		}
		if(buildInCamera) { BindCamera(buildInCamera); }
	}
	bool textureIsExist;
	_file.Read(textureIsExist);
			
	if(textureIsExist)
	{
		bool textureIsCommonAsset;
		_file.Read(textureIsCommonAsset);
		Texture2D* buildInTexture;
				
		if(textureIsCommonAsset)
		{
			StringANSI name;
			int32 length;
			_file.Read(length);
			_file.ReadString(name, length);

			buildInTexture = dynamic_cast<Texture2D*>(AssetLibrary::_LoadCommonAsset<Texture2D>(name));
		}
		else
		{
			buildInTexture = dynamic_cast<Texture2D*>(AssetLibrary::_LoadPrivateAsset<Texture2D>(_file));
		}
		if(buildInTexture) { BindTexture(buildInTexture); }
	}
	bool isPickedScene;
	_file.Read(isPickedScene);
				
	if(isPickedScene)
	{
		Pick();
	}
}



bool Scene::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}


bool Scene::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		LogicObject::LoadDerivedFromFile(_file);
		
		GraphicsObject::LoadDerivedFromFile(_file);
		
		LoadDerivedFromFile(_file); 
		return true;
	}
	return false;
}


