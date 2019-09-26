#include "TiledSpace3D.h"
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Output/Graphics/Renderer/PixelBuffer.h>
#include <Engine/Output/Graphics/Renderer/PixelBuffer.h>
#include <Engine/Core/Shape/Plane/Plane.h>



#ifdef SendMessage
   #undef SendMessage
#endif




TiledSpace3D::TiledSpace3D(void): tiledSpace(NIL), amountOfTiles(0)
{
	AddClassProperty(TILED_SPACE_3D_CLASS);
}


TiledSpace3D::~TiledSpace3D(void)
{
	Destroy();
	Destroying();
} 



void TiledSpace3D::Destroy(void)
{
	if(tiledSpace)
	{
		delete[] tiledSpace; 
		tiledSpace = NIL; 
		amountOfTiles = 0;
		spaceSize = tileSize = Vector3();
	}
}



void TiledSpace3D::ResetTransformationValid(void)
{
	ModelView::ResetTransformationValid();
	for(int i = 0; i < amountOfTiles; ++i)
	{
		TexturingSurface* object = tiledSpace[i].GetObject();
		if(object) { object->ResetTransformationValid(); }
	}
}


bool TiledSpace3D::AddTile(Vector3 _position, int32 _brushId)
{ 
	if(tiledSpace && IsTileExist(_position))
	{
		Brush* brush = brushes.Find(_brushId, &Brush::GetBrushId);
	
		if(brush)
		{
			TexturingSurface* object = new TexturingSurface();
			*object = *(brush->GetSample());
			int32 index = TileIndexOf(_position);
			object->SetPosition(tiledSpace[index].GetPosition());
			tiledSpace[index].SetBrushId(_brushId);
			tiledSpace[index].SetPassEnable(brush->IsPassed());
			return tiledSpace[index].SetObject(object);
		}
	}
	return false;
}



bool TiledSpace3D::IsParentScene(TiledSpace3D* _tiledSpace)
{
	if(graphicsContainer.IsNotEmpty())
	{
		if(graphicsContainer.Get()->IsClassPropertyExist(GraphicsScene::GRAPHICS_SCENE_CLASS))
		{
			GraphicsScene* it = dynamic_cast<GraphicsScene*>(graphicsContainer.Get());
			return it->GetRenderList().IsParentScene(_tiledSpace);
		}
		else if(graphicsContainer.Get()->IsClassPropertyExist(TiledSpace3D::TILED_SPACE_3D_CLASS))
		{
			TiledSpace3D* it = dynamic_cast<TiledSpace3D*>(graphicsContainer.Get());

			if(it == _tiledSpace)
			{
				return true;
			}
			return it->IsParentScene(_tiledSpace);
		}
	}
	return false;
}



bool TiledSpace3D::IsParentScene(GraphicsScene* _scene)
{
	if(graphicsContainer.IsNotEmpty())
	{
		if(graphicsContainer.Get()->IsClassPropertyExist(GraphicsScene::GRAPHICS_SCENE_CLASS))
		{
			GraphicsScene* it = dynamic_cast<GraphicsScene*>(graphicsContainer.Get());

			if(it == _scene)
			{
				return true;
			}
			return it->GetRenderList().IsParentScene(_scene);
		}
		else if(graphicsContainer.Get()->IsClassPropertyExist(TiledSpace3D::TILED_SPACE_3D_CLASS))
		{
			TiledSpace3D* it = dynamic_cast<TiledSpace3D*>(graphicsContainer.Get());
			return it->IsParentScene(_scene);
		}
	}
	return false;
}



bool TiledSpace3D::RemoveTile(Vector3 _position)
{
	if(tiledSpace && IsTileExist(_position))
	{
		int32 index = TileIndexOf(_position);
		tiledSpace[index].SetBrushId(0);
		tiledSpace[index].SetPassEnable(true);
		tiledSpace[index].SetObject(NIL);
		return true;
	}
	return false;
}


bool TiledSpace3D::Fill(int32 _plane, int32 _index, int32 _brushId)
{
	Brush* brush = brushes.Find(_brushId, &Brush::GetBrushId);
	
	if(brush && tiledSpace != NIL)
	{
		switch(_plane)
		{
			case Plane::YZ_PLANE:
			{
				if(_index >= 0 && _index < spaceSize.x)
				{
					int32 nextFloor = int32(spaceSize.x * spaceSize.z);
					int32 index = (int32)_index;
					TexturingSurface* object;
					int32 k;
					for(int32 i = 0; i < (int32)spaceSize.y; i++)
					{
						for(int32 j = 0; j < (int32)spaceSize.z; j++)
						{
							object = new TexturingSurface();
							*object = *(brush->GetSample());
							k = index + j * (int32)spaceSize.x;
							object->SetPosition(tiledSpace[k].GetPosition());
							tiledSpace[k].SetBrushId(_brushId);
							tiledSpace[k].SetPassEnable(brush->IsPassed());
							tiledSpace[k].SetObject(object);
						}
						index += nextFloor;
					}
					return true;
				}
				break;
			}
						
			case Plane::XZ_PLANE:
			{
				if(_index >= 0 && _index < (int32)spaceSize.y)
				{
					int32 floorSize = int32(spaceSize.x * spaceSize.z);
					int32 index = _index * floorSize;
					TexturingSurface* object;
					for(int32 i = 0; i < floorSize; i++)
					{
						object = new TexturingSurface();
						*object = *(brush->GetSample());
						object->SetPosition(tiledSpace[index].GetPosition());
						tiledSpace[index].SetBrushId(_brushId);
						tiledSpace[index].SetPassEnable(brush->IsPassed());
						tiledSpace[index].SetObject(object);
						index++;
					}
					return true;
				}
				break;
			}
						
			case Plane::XY_PLANE:
			{
				if(_index >= 0 && _index < spaceSize.z)
				{
					int32 nextFloor = int32(spaceSize.x * spaceSize.z);
					int32 index = int32(_index * spaceSize.x);
					TexturingSurface* object;
					int32 k;
					for(int32 i = 0; i < (int32)spaceSize.y; i++)
					{
						for(int32 j = 0; j < (int32)spaceSize.x; j++)
						{
							object = new TexturingSurface();
							*object = *(brush->GetSample());
							k = index + j;
							object->SetPosition(tiledSpace[k].GetPosition());
							tiledSpace[k].SetBrushId(_brushId);
							tiledSpace[k].SetPassEnable(brush->IsPassed());
							tiledSpace[k].SetObject(object);
						}
						index += nextFloor;
					}
					return true;
				}
				break;
			}
		}
	}

	return false;
}



Vector3 TiledSpace3D::GetTileSize(void)const
{
	return tileSize;
}


void TiledSpace3D::Create(Vector3 _spaceSize, Vector3 _tileSize)
{
	if(_spaceSize.x >= 1.0f && _spaceSize.y >= 1.0f && _spaceSize.z >= 1.0f)
	{
		if(_tileSize.x > 0.0f && _tileSize.y > 0.0f && _tileSize.z > 0.0f)
		{
			if(tiledSpace) 
			{ 
				delete[] tiledSpace; 
				tiledSpace = NIL; 
			}

			spaceSize = _spaceSize;
			tileSize = _tileSize;
			Vector3 tilePosition;
			amountOfTiles = (int32)spaceSize.x * (int32)spaceSize.y * (int32)spaceSize.z;
			tiledSpace = new Tile[amountOfTiles];
			for(int i = 0; i < amountOfTiles; ++i)
			{ 
				tilePosition = TilePositionOf(i);
				tiledSpace[i].SetTilePosition(tilePosition);
				tiledSpace[i].SetPosition(TileCenterOf(tilePosition));
				tiledSpace[i].SetTiledSpace(this);
			}
		}
	}
}



Vector3 TiledSpace3D::GetSpaceSize(void)const
{
	return spaceSize;
}


int32 TiledSpace3D::GetAmountOfTiles(void)const
{
	return amountOfTiles;
}



bool TiledSpace3D::IsExist(void)const
{
	return (tiledSpace != NIL);
}

	

TexturingSurface* TiledSpace3D::GetBrush(int32 _brushId)
{
	Brush* brush = brushes.Find(_brushId, &Brush::GetBrushId);
	
	if(brush)
	{
		return brush->GetSample(); 
	} 
	return NIL;
}


int32 TiledSpace3D::GetAmountOfBrushes(void)const
{
	return brushes.GetSize();
}


void TiledSpace3D::RemoveBrush(int32 _brushId)
{
	delete brushes.Find(_brushId, &Brush::GetBrushId);
}


bool TiledSpace3D::CreateBrush(TexturingSurface* _sample, bool _passEnable, int32 _brushId)
{
	if(_sample)
	{
		if(brushes.Find(_brushId, &Brush::GetBrushId) == NIL)
		{
			TexturingSurface* object = new TexturingSurface();
			*object = *_sample;
			if(brushes.Add(new Brush(object, _passEnable, _brushId), &Brush::GetBrushId))
			{
				return true;
			}
		}
	}
	return false;
}



bool TiledSpace3D::AddBrush(Brush* _brush)
{
	if(_brush)
	{
		if(brushes.Find(_brush->GetBrushId(), &Brush::GetBrushId) == NIL)
		{
			if(brushes.Add(_brush, &Brush::GetBrushId))
			{
				return true;
			}
		}
	}
	return false;
}



int32 TiledSpace3D::TileIndexOf(Vector3 _position)
{
	return (int32)_position.x + (int32)_position.z * (int32)spaceSize.x + (int32)_position.y * (int32)spaceSize.x * (int32)spaceSize.z;
}



Vector3 TiledSpace3D::TilePositionOf(int32 _index)
{
	if(tiledSpace)
	{
		int32 x = _index % (int32)spaceSize.x;
		int32 y = _index / ((int32)spaceSize.x * (int32)spaceSize.z);
		int32 z = _index < (int32)spaceSize.x * (int32)spaceSize.z ? _index / (int32)spaceSize.x : (_index - y * (int32)spaceSize.x * (int32)spaceSize.z) / (int32)spaceSize.x;
		return Vector3(x, y, z);
	}
	return Vector3();
}


Vector3 TiledSpace3D::TileCenterOf(Vector3 _position)
{
	return Vector3((_position.x * tileSize.x) - ((int32)spaceSize.x / 2) * tileSize.x,
				   (_position.y * tileSize.y) - ((int32)spaceSize.y / 2) * tileSize.y + (tileSize.y / 2),
				   (_position.z * tileSize.z) - ((int32)spaceSize.z / 2) * tileSize.z);
}



bool TiledSpace3D::IsTileSolid(Vector3 _position)
{
	if(tiledSpace)
	{
		Tile* tile = GetTile(_position);
		if(tile) { return tile->IsSolid(); } 
	}
	return false;
}
		

bool TiledSpace3D::IsTileSolid(int32 _index)
{
	if(tiledSpace)
	{
		Tile* tile = GetTile(_index);
		if(tile) { return tile->IsSolid(); } 
	}
	return false;
}


bool TiledSpace3D::IsTileExist(Vector3 _position)const
{
	if(tiledSpace)
	{
		return (_position.x >= 0.0f && _position.x < spaceSize.x && 
				_position.z >= 0.0f && _position.z < spaceSize.z && 
				_position.y >= 0.0f && _position.y < spaceSize.y);
	}
	return false;
}


bool TiledSpace3D::IsTileExist(int32 _index)const
{
	if(tiledSpace)
	{
		return (_index >= 0 && _index < amountOfTiles);
	}
	return false;
}



TiledSpace3D::Tile* TiledSpace3D::GetTile(Vector3 _position)
{
	if(tiledSpace && IsTileExist(_position))
	{
		return &tiledSpace[TileIndexOf(_position)];
	}
	return NIL;
}


TiledSpace3D::Tile* TiledSpace3D::GetTile(int32 _index)
{
	if(tiledSpace && IsTileExist(_index))
	{
		return &tiledSpace[_index];
	}
	return NIL;
}



void TiledSpace3D::Draw(void)
{
	if(showEnable)
	{
		if(camera.IsNotEmpty())
		{
			if(texture.IsNotEmpty() && texture.Get()->IsExist())
			{
				HDC previousDeviceContext = wglGetCurrentDC();
				HGLRC previousRenderContext = wglGetCurrentContext();
				Camera3D* camera = TiledSpace3D::camera.Get();

				pixelBuffer->Pick();
				glClear(pixelBuffer->clearActions);
				camera->BeginTransform(Renderer::PROJECTION);
				camera->BeginTransform(Renderer::MODELVIEW);
				for(int i = 0; i < amountOfTiles; ++i)
				{ 
					tiledSpace[i].Draw();
				}
				camera->EndTransform(Renderer::MODELVIEW);
				camera->EndTransform(Renderer::PROJECTION);
				wglMakeCurrent(previousDeviceContext, previousRenderContext);
			}
			else
			{
				Camera3D* camera = TiledSpace3D::camera.Get();
				camera->BeginTransform(Renderer::PROJECTION);
				camera->BeginTransform(Renderer::MODELVIEW);
				for(int i = 0; i < amountOfTiles; ++i)
				{ 
					tiledSpace[i].Draw();
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

				pixelBuffer->Pick();
				glClear(pixelBuffer->clearActions);
				for(int i = 0; i < amountOfTiles; ++i)
				{ 
					tiledSpace[i].Draw();
				}
				wglMakeCurrent(previousDeviceContext, previousRenderContext);
			}
			else
			{
				for(int i = 0; i < amountOfTiles; ++i)
				{ 
					tiledSpace[i].Draw();
				}
			}
		}
	}
}



bool TiledSpace3D::MoveTileFromTo(Vector3 _from, Vector3 _to)
{
	if(tiledSpace) 
	{
		int32 indexFrom = TileIndexOf(_from);
		int32 indexTo = TileIndexOf(_to);

		if(IsTileExist(indexFrom) && IsTileExist(indexTo))
		{
			if(tiledSpace[indexTo].IsPassed())
			{
				tiledSpace[indexTo].SetBrushId(tiledSpace[indexFrom].GetBrushId());
				tiledSpace[indexTo].SetPassEnable(tiledSpace[indexFrom].IsPassed());
				tiledSpace[indexTo].SetObject(tiledSpace[indexFrom].GetObject());
				tiledSpace[indexFrom].SetBrushId(0);
				tiledSpace[indexFrom].SetPassEnable(true);
				tiledSpace[indexFrom].SetObject(NIL);
				return true;
			}
		}
	}
	return false;
}





bool TiledSpace3D::CopyTile(Vector3 _from, Vector3 _to)
{
	if(tiledSpace) 
	{
		int32 indexFrom = TileIndexOf(_from);
		int32 indexTo = TileIndexOf(_to);

		if(IsTileExist(indexFrom) && IsTileExist(indexTo))
		{
			tiledSpace[indexTo].SetBrushId(tiledSpace[indexFrom].GetBrushId());
			tiledSpace[indexTo].SetPassEnable(tiledSpace[indexFrom].IsPassed());
			
			if(tiledSpace[indexFrom].GetObject())
			{
				TexturingSurface* object = new TexturingSurface();
				*object = *(tiledSpace[indexFrom].GetObject());
				object->SetPosition(TileCenterOf(_to));
				tiledSpace[indexTo].SetObject(object);
			}
			else
			{
				tiledSpace[indexTo].SetObject(NIL);
			}
			return true;
		}
	}
	return false;
}


bool TiledSpace3D::SwapTile(Vector3 _from, Vector3 _to)
{
	if(tiledSpace) 
	{
		int32 indexFrom = TileIndexOf(_from);
		int32 indexTo = TileIndexOf(_to);

		if(IsTileExist(indexFrom) && IsTileExist(indexTo))
		{
			int32 brushId = tiledSpace[indexTo].GetBrushId();
			bool passEnable = tiledSpace[indexTo].IsPassed();
			TexturingSurface* object = tiledSpace[indexTo].GetObject();
			tiledSpace[indexTo].SetBrushId(tiledSpace[indexFrom].GetBrushId());
			tiledSpace[indexTo].SetPassEnable(tiledSpace[indexFrom].IsPassed());
			tiledSpace[indexTo].SetObject(tiledSpace[indexFrom].GetObject());
			tiledSpace[indexFrom].SetBrushId(brushId);
			tiledSpace[indexFrom].SetPassEnable(passEnable);
			tiledSpace[indexFrom].SetObject(object);
			return true;
		}
	}
	return false;
}


bool TiledSpace3D::SaveToFile(StringANSI _path)
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



bool TiledSpace3D::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write((int64)TILED_SPACE_3D_CLASS);

		GraphicsObject::SaveDerivedToFile(_file);

		SaveDerivedToFile(_file); 
		return true;
	}
	return false;
}



bool TiledSpace3D::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + ObjectScene::_GetDir() + _name + "." + ObjectScene::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}




bool TiledSpace3D::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write((int64)TILED_SPACE_3D_CLASS);

		GraphicsObject::SaveDerivedToFile(_file);

		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



void TiledSpace3D::SaveDerivedToFile(File& _file)
{
	_file.Write(objectTree.GetSize());
	SaveObjectToFile(objectTree.GetRootAddress(), _file);
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
	_file.Write(brushes.GetSize());
	SaveBrushToFile(brushes.GetRootAddress(), _file);
	_file.Write(spaceSize);
	_file.Write(tileSize);
	for(int i = 0; i < amountOfTiles; ++i)
	{ 
		SaveTile(&tiledSpace[i], _file);
	}
}



void TiledSpace3D::SaveBrushToFile(MagicBinaryTree<Brush>::AddressDerived* _address, File& _file)
{  
	if(_address)
	{
		Brush* brush = _address->GetNode();

		brush->SaveToFile(_file);

		SaveBrushToFile(_address->GetChild(0), _file);
		SaveBrushToFile(_address->GetChild(1), _file);
	}
}


void TiledSpace3D::SaveTile(TiledSpace3D::Tile* _tile, File& _file)
{
	if(_tile)
	{
		_file.Write(_tile->IsPassed());
		_file.Write(_tile->GetBrushId());
		_file.Write(_tile->IsEmpty());

		if(!_tile->IsEmpty())
		{
			TexturingSurface* object = _tile->GetObject();
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
		}
	}
}



bool TiledSpace3D::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}


bool TiledSpace3D::LoadFromFile(File& _file)
{ 
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		GraphicsObject::LoadDerivedFromFile(_file);
		
		LoadDerivedFromFile(_file);
		return true;
	}
	return false;
}



void TiledSpace3D::LoadBrushFromFile(File& _file)
{ 
	 Brush* brush = new Brush();

	 brush->LoadFromFile(_file);
		
	if(!AddBrush(brush))
	{
		delete brush;
	}
}


 
void TiledSpace3D::LoadDerivedFromFile(File& _file)
{
	Destroy();
	int32 amountOfObjects;
	_file.Read(amountOfObjects);
	for(int32 i = 0; i < amountOfObjects; ++i)
	{
		LoadObjectFromFile(_file);
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
	int32 amountOfBrushes;
	_file.Read(amountOfBrushes);
	for(int32 i = 0; i < amountOfBrushes; ++i)
	{ 
		LoadBrushFromFile(_file);
	}
	_file.Read(spaceSize);
	_file.Read(tileSize);

	Create(spaceSize, tileSize);
	for(int i = 0; i < amountOfTiles; ++i)
	{
		LoadTile(_file, i);
	}
}


void TiledSpace3D::LoadTile(File& _file, int32 _index)
{
	bool isPassed;
	_file.Read(isPassed);
	int32 brushId;
	_file.Read(brushId);
	bool isEmpty;
	_file.Read(isEmpty);

	if(!isEmpty)
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

		tiledSpace[_index].SetObject(dynamic_cast<TexturingSurface*>(object));
	}
			
	tiledSpace[_index].SetBrushId(brushId);
	tiledSpace[_index].SetPassEnable(isPassed);
}

