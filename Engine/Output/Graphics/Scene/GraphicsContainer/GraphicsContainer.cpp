#include "GraphicsContainer.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Output/Graphics/Renderer/PixelBuffer.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>


GraphicsContainer::GraphicsContainer(void): pixelBuffer(NIL)
{
	AddClassProperty(GRAPHICS_CONTAINER_CLASS);
}


GraphicsContainer::~GraphicsContainer(void)
{
	Destroying();
}



void GraphicsContainer::Destroy(void)
{
	if(pixelBuffer)
	{ 
		delete pixelBuffer; 
		pixelBuffer = NIL;
	}
	if(texture.Get()) { texture.Get()->SetRenderTarget(NIL); }
	
	objectTree.Clear();
}



Camera3D* GraphicsContainer::GetCamera(bool _checkParentScene)
{
	if(camera.IsNotEmpty()) { return camera.Get(); }

	if(_checkParentScene)
	{
		GraphicsObject* scene = GetGraphicsContainer();

		if(scene)
		{
			if(scene->IsClassPropertyExist(GraphicsScene::GRAPHICS_SCENE_CLASS))
			{
				return dynamic_cast<GraphicsContainer*>(scene)->GetCamera(_checkParentScene);
			}
			else if(scene->IsClassPropertyExist(TiledSpace3D::TILED_SPACE_3D_CLASS))
			{
				return dynamic_cast<TiledSpace3D*>(scene)->GetCamera(_checkParentScene);
			}
		}
	}
	return NIL;
}



void GraphicsContainer::BindCamera(Camera3D* _camera)
{
	if(camera.Get() != _camera)
	{
		if(camera.IsNotEmpty())
		{
			camera.Clear();
		}

		if(_camera)
		{
			camera.Attach(_camera);

			if(IsReceiversExist(GraphicsContainer::CHANGE_CAMERA_MESSAGE)) { SendPackage(GraphicsContainer::CHANGE_CAMERA_MESSAGE, camera.Get()->GetName()); }
		}
	}
}



void GraphicsContainer::TextureIsDestroyed(void)
{ 

	if(pixelBuffer)
	{ 
		delete pixelBuffer; 
		pixelBuffer = NIL;
	}
	texture.Get()->SetRenderTarget(NIL);
}


void GraphicsContainer::TextureIsCreated(void)
{ 

	texture.Get()->CreateRenderTarget(texture.Get()->GetImageSize());
	pixelBuffer = new PixelBuffer();
	PixelBuffer::Define define;

	define.mode = PixelBuffer::RENDER_TO_TEXTURE;
	define.width = texture.Get()->GetImageSize().x;
	define.height = texture.Get()->GetImageSize().y;
	define.backgroundColor = GetColorRGBA();
	define.filteringMode = texture.Get()->GetFilteringMode();

	if(pixelBuffer->Create(define))
	{
		texture.Get()->SetRenderTarget(pixelBuffer);
	}
	else
	{
		delete pixelBuffer; 
		pixelBuffer = NIL;
	}
}


void GraphicsContainer::BindTexture(Texture2D* _texture)
{
	if(texture.Get() != _texture)
	{
		if(texture.IsNotEmpty())
		{
			texture.Get()->Disconnect(Texture2D::DESTROY_TEXTURE_MESSAGE, this);
			texture.Get()->Disconnect(Texture2D::INIT_TEXTURE_MESSAGE, this);

			TextureIsDestroyed();
			texture.Clear();
		}

		if(_texture)
		{
			texture.Attach(_texture);
			texture.Get()->Connect(Texture2D::DESTROY_TEXTURE_MESSAGE, this, Caller<>(this, &GraphicsContainer::TextureIsDestroyed));
			texture.Get()->Connect(Texture2D::INIT_TEXTURE_MESSAGE, this, Caller<>(this, &GraphicsContainer::TextureIsCreated));

			if(texture.Get()->IsExist())
			{ 
				TextureIsCreated();
			}
			if(IsReceiversExist(GraphicsContainer::CHANGE_RENDER_TARGET_MESSAGE)) { SendPackage(GraphicsContainer::CHANGE_RENDER_TARGET_MESSAGE, texture.Get()->GetName()); }
		}
	}
}



Texture2D* GraphicsContainer::GetTexture(void)const
{
	return texture.Get();
}



bool GraphicsContainer::SaveToFile(StringANSI _path)
{
	return false;
}

 

bool GraphicsContainer::SaveToFile(File& _file)
{
	return false;
}

 

bool GraphicsContainer::SaveAsToFile(StringANSI _name)
{
	return false;
}


bool GraphicsContainer::SaveAsToFile(File& _file, StringANSI _name)
{
	return false;
}



bool GraphicsContainer::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}


bool GraphicsContainer::LoadFromFile(File& _file)
{
	return false;
}