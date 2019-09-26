#include "GraphicsScene.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Output/Graphics/Renderer/PixelBuffer.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>


GraphicsScene::GraphicsScene(void)
{
	renderList.scene = this;
	AddClassProperty(GRAPHICS_SCENE_CLASS);
}


GraphicsScene::~GraphicsScene(void)
{
	Destroy();
	Destroying();
}


void GraphicsScene::Destroy(void)
{
	GraphicsContainer::Destroy();

	renderList.Clear();
}



void GraphicsScene::ResetTransformationValid(void)
{
	ModelView::ResetTransformationValid(); 
	UpdateTransformation(renderList.GetHeadAddress());
}


void GraphicsScene::UpdateTransformation(MagicList<GraphicsObject>::AddressDerived* _address)const
{ 
	if(_address)
	{
		_address->GetNode()->ResetTransformationValid();
		UpdateTransformation(_address->GetRight());
	}
} 


int32 GraphicsScene::GetAmountOfGraphicsObjects(void)
{
	int32 size = 0;
	GetAmountOfGraphicsObjects(renderList.GetHeadAddress(), size);
	return size;
}


void GraphicsScene::GetAmountOfGraphicsObjects(MagicList<GraphicsObject>::AddressDerived* _address, int32& _size)const
{ 
	if(_address)
	{ 
		GetAmountOfGraphicsObjects(_address->GetRight(), _size);

		++_size;
		if(_address->GetNode()->IsClassPropertyExist(GraphicsScene::GRAPHICS_SCENE_CLASS))
		{
			GraphicsScene* scene = dynamic_cast<GraphicsScene*>(_address->GetNode());
			
			if(scene)
			{
				_size += scene->GetAmountOfGraphicsObjects();
			}
		}
	}
}



RenderList::AddressDerived* GraphicsScene::AddToRenderList(GraphicsObject* _parent, GraphicsObject* _object)
{
	return renderList.AddObject(_parent, _object);
}


RenderList::AddressDerived* GraphicsScene::AddToRenderList(GraphicsObject* _object)
{
	return renderList.AddObject(_object);
}



void GraphicsScene::RemoveFromRenderList(GraphicsObject* _object)
{
	if(_object)
	{
		delete _object->GetAddress(&renderList);
	}
}



RenderList& GraphicsScene::GetRenderList(void)
{
	return renderList;
}



void GraphicsScene::Draw(void)
{
	if(showEnable)
	{
		if(camera.IsNotEmpty())
		{
			if(texture.IsNotEmpty() && texture.Get()->IsExist())
			{
				HDC previousDeviceContext = wglGetCurrentDC();
				HGLRC previousRenderContext = wglGetCurrentContext();
				Camera3D* camera = GraphicsScene::camera.Get();

				pixelBuffer->Pick();
				glClear(pixelBuffer->clearActions);
				camera->BeginTransform(Renderer::PROJECTION);
				camera->BeginTransform(Renderer::MODELVIEW);
				MagicList<GraphicsObject>::AddressDerived* it = renderList.GetHeadAddress();
				int32 visibleInfo;
				GraphicsObject* object;

				while(it)
				{ 
					object = it->GetNode();

					if(object->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS))
					{
						camera->IsSphereInFrustum(dynamic_cast<GraphicsSurface*>(object)->GetTransformedSphere(), &visibleInfo);
						if(visibleInfo != Camera3D::INVISIBLE_OBJECT) { object->Draw(); }
					}
					else
					{
						object->Draw();
					}
					it = it->GetRight();
				}
				camera->EndTransform(Renderer::MODELVIEW);
				camera->EndTransform(Renderer::PROJECTION);
				wglMakeCurrent(previousDeviceContext, previousRenderContext);
			}
			else
			{
				Camera3D* camera = GraphicsScene::camera.Get();
				camera->BeginTransform(Renderer::PROJECTION);
				camera->BeginTransform(Renderer::MODELVIEW);
				MagicList<GraphicsObject>::AddressDerived* it = renderList.GetHeadAddress();
				int32 visibleInfo;
				GraphicsObject* object;

				while(it)
				{
					object = it->GetNode();

					if(object->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS))
					{
						camera->IsSphereInFrustum(dynamic_cast<GraphicsSurface*>(object)->GetTransformedSphere(), &visibleInfo);
						if(visibleInfo != Camera3D::INVISIBLE_OBJECT) { object->Draw(); }
					}
					else
					{
						object->Draw();
					}
					it = it->GetRight();
				}
				camera->EndTransform(Renderer::MODELVIEW);
				camera->EndTransform(Renderer::PROJECTION);
			}
		}
		else
		{ 
			if(texture.IsNotEmpty() && texture.Get()->IsExist())
			{
				HDC previousDeviceContext = wglGetCurrentDC();
				HGLRC previousRenderContext = wglGetCurrentContext();
				Camera3D* camera = GetCamera();

				pixelBuffer->Pick();
				glClear(pixelBuffer->clearActions);
				MagicList<GraphicsObject>::AddressDerived* it = renderList.GetHeadAddress();
				int32 visibleInfo;
				GraphicsObject* object;

				while(it)
				{
					object = it->GetNode();

					if(camera && object->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS))
					{
						camera->IsSphereInFrustum(dynamic_cast<GraphicsSurface*>(object)->GetTransformedSphere(), &visibleInfo);
						if(visibleInfo != Camera3D::INVISIBLE_OBJECT) { object->Draw(); }
					}
					else
					{
						object->Draw();
					}
					it = it->GetRight();
				}
				wglMakeCurrent(previousDeviceContext, previousRenderContext);
			}
			else
			{
				Camera3D* camera = GetCamera();
				MagicList<GraphicsObject>::AddressDerived* it = renderList.GetHeadAddress();
				int32 visibleInfo;
				GraphicsObject* object;

				while(it)
				{
					object = it->GetNode();

					if(camera && object->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS))
					{
						camera->IsSphereInFrustum(dynamic_cast<GraphicsSurface*>(object)->GetTransformedSphere(), &visibleInfo);
						if(visibleInfo != Camera3D::INVISIBLE_OBJECT) { object->Draw(); }
					}
					else
					{
						object->Draw();
					}
					it = it->GetRight();
				}
			}
		}
	}
}



bool GraphicsScene::SaveToFile(StringANSI _path)
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

 

bool GraphicsScene::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write((int64)GRAPHICS_SCENE_CLASS);

		GraphicsObject::SaveDerivedToFile(_file);
		
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}

 

bool GraphicsScene::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + ObjectScene::_GetDir() + _name + "." + ObjectScene::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool GraphicsScene::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write((int64)GRAPHICS_SCENE_CLASS);

		GraphicsObject::SaveDerivedToFile(_file);
		
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



void GraphicsScene::SaveDerivedToFile(File& _file)
{
	_file.Write(objectTree.GetSize());
	SaveObjectToFile(objectTree.GetRootAddress(), _file);
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
}

 


void GraphicsScene::SaveGraphicsObjectToFile(MagicList<GraphicsObject>::AddressDerived* _address, File& _file)
{ 
	if(_address)
	{
		GraphicsObject* object = _address->GetNode();
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

		SaveGraphicsObjectToFile(_address->GetRight(), _file);
	}
}





bool GraphicsScene::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}



bool GraphicsScene::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		GraphicsObject::LoadDerivedFromFile(_file);
		
		LoadDerivedFromFile(_file);
		return true;
	}
	return false;
}


void GraphicsScene::LoadDerivedFromFile(File& _file)
{
	Destroy();
	int32 amountOfObjects; 
	_file.Read(amountOfObjects);
	for(int32 i = 0; i < amountOfObjects; ++i)
	{
		LoadObjectFromFile(_file);
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
}


 


void GraphicsScene::LoadGraphicsObjectFromFile(File& _file)
{ 
	GraphicsObject* object;
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

		object = dynamic_cast<GraphicsObject*>(AssetLibrary::_LoadCommonAsset(name, assetType));
	}
	else
	{ 
		object = dynamic_cast<GraphicsObject*>(AssetLibrary::_LoadPrivateAsset(_file, assetType));
	}

	if(object)
	{
		AddToRenderList(dynamic_cast<GraphicsObject*>(object));
	}
}

