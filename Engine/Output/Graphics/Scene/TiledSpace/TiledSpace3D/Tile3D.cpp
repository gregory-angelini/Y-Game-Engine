#include "TiledSpace3D.h"



TiledSpace3D::Tile::Tile(void): tiledSpace(NIL), brushId(0), isPassed(true)
{}


TiledSpace3D::Tile::~Tile(void)
{}



void TiledSpace3D::Tile::SetPassEnable(bool _enable)
{
	isPassed = _enable;
}


int32 TiledSpace3D::Tile::GetBrushId(void)const
{
	return brushId;
}


void TiledSpace3D::Tile::SetBrushId(int32 _brushId)
{
	brushId = _brushId;
}



bool TiledSpace3D::Tile::IsPassed(void)const
{
	return isPassed;
}


bool TiledSpace3D::Tile::IsSolid(void)const
{
	return (object.Get() != NIL);
}


bool TiledSpace3D::Tile::IsEmpty(void)const
{
	return (object.Get() == NIL);
}


void TiledSpace3D::Tile::Draw(void)
{
	if(object.Get()) { object.Get()->Draw(); }
}


bool TiledSpace3D::Tile::SetObject(TexturingSurface* _object)
{
	if(tiledSpace)
	{ 
		if(_object)
		{
			if(_object->graphicsContainer.IsEmpty())
			{
				if(_object->IsClassPropertyExist(GraphicsScene::GRAPHICS_SCENE_CLASS))
				{
					GraphicsScene* it = dynamic_cast<GraphicsScene*>(_object);

					if(tiledSpace->IsParentScene(it))
					{
						return false;
					}
				}
				else if(_object->IsClassPropertyExist(TiledSpace3D::TILED_SPACE_3D_CLASS))
				{
					TiledSpace3D* it = dynamic_cast<TiledSpace3D*>(_object);

					if(tiledSpace->IsParentScene(it))
					{
						return false;
					}
				}

				if(object.IsNotEmpty())
				{ 
					object.Get()->graphicsContainer.Clear(MagicContainer::REMOVE_NODE);
					object.Get()->ResetTransformationValid();
					object.Clear();
				}
				
				object.Attach(_object);
				_object->ResetTransformationValid();
				return true;
			}
		}
		else
		{
			if(object.IsNotEmpty())
			{ 
				object.Get()->ResetTransformationValid();
				object.Clear();
			}
			return true;
		}
	}
	return false;
}




TexturingSurface* TiledSpace3D::Tile::GetObject(void)
{
	return object.Get();
}


void TiledSpace3D::Tile::SetPosition(Vector3 _position)
{
	position = _position;
}


Vector3 TiledSpace3D::Tile::GetPosition(void)const
{
	return position;
}


Vector3 TiledSpace3D::Tile::GetSize(void)const
{
	if(tiledSpace)
	{ 
		return tiledSpace->GetTileSize(); 
	} 
	return Vector3();
}



Vector3 TiledSpace3D::Tile::GetTilePosition(void)const
{
	return tilePosition;
}


void TiledSpace3D::Tile::SetTilePosition(Vector3 _position)
{
	tilePosition = _position;
}



TiledSpace3D* TiledSpace3D::Tile::GetTiledSpace(void)
{
	return tiledSpace;
}



void TiledSpace3D::Tile::SetTiledSpace(TiledSpace3D* _tiledSpace)
{
	tiledSpace = _tiledSpace;
}

