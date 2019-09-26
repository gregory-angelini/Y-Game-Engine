#include "RenderList.h"
#include <Engine/Output/Graphics/Scene/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>




RenderList::RenderList(void)
{}



RenderList::~RenderList(void)
{}


MagicList<GraphicsObject>::AddressDerived* RenderList::AddObject(GraphicsObject* _object)
{
	if(_object == NIL || _object == scene) { return NIL; }

	if(_object->graphicsContainer.IsNotEmpty())
	{
		return NIL;
	}

	if(_object->IsClassPropertyExist(GraphicsScene::GRAPHICS_SCENE_CLASS))
	{
		GraphicsScene* it = dynamic_cast<GraphicsScene*>(_object);

		if(IsParentScene(it))
		{
			return NIL;
		}
	}
	return AddToTail(_object);
}


MagicList<GraphicsObject>::AddressDerived* RenderList::AddObject(GraphicsObject* _parent, GraphicsObject* _object)
{
	if(_object == NIL || _object == scene) { return NIL; }

	if(_object->graphicsContainer.IsNotEmpty())
	{
		return NIL;
	}

	if(_object->IsClassPropertyExist(GraphicsScene::GRAPHICS_SCENE_CLASS))
	{
		GraphicsScene* it = dynamic_cast<GraphicsScene*>(_object);

		if(IsParentScene(it))
		{
			return NIL;
		}
	}
	else if(_object->IsClassPropertyExist(TiledSpace3D::TILED_SPACE_3D_CLASS))
	{
		TiledSpace3D* it = dynamic_cast<TiledSpace3D*>(_object);

		if(IsParentScene(it))
		{
			return NIL;
		}
	}
	return AddToRight(_parent, _object);
}


bool RenderList::IsParentScene(TiledSpace3D* _tiledSpace)
{
	if(scene->graphicsContainer.IsNotEmpty())
	{
		if(scene->graphicsContainer.Get()->IsClassPropertyExist(GraphicsScene::GRAPHICS_SCENE_CLASS))
		{
			GraphicsScene* it = dynamic_cast<GraphicsScene*>(scene->graphicsContainer.Get());
			return it->GetRenderList().IsParentScene(_tiledSpace);
		}
		else if(scene->graphicsContainer.Get()->IsClassPropertyExist(TiledSpace3D::TILED_SPACE_3D_CLASS))
		{
			TiledSpace3D* it = dynamic_cast<TiledSpace3D*>(scene->graphicsContainer.Get());

			if(it == _tiledSpace)
			{
				return true;
			}
			return it->IsParentScene(_tiledSpace);
		}
	}
	return false;
}



bool RenderList::IsParentScene(GraphicsScene* _scene)
{
	if(scene->graphicsContainer.IsNotEmpty())
	{
		if(scene->graphicsContainer.Get()->IsClassPropertyExist(GraphicsScene::GRAPHICS_SCENE_CLASS))
		{
			GraphicsScene* it = dynamic_cast<GraphicsScene*>(scene->graphicsContainer.Get());

			if(it == _scene)
			{
				return true;
			}
			return it->GetRenderList().IsParentScene(_scene);
		}
		else if(scene->graphicsContainer.Get()->IsClassPropertyExist(TiledSpace3D::TILED_SPACE_3D_CLASS))
		{
			TiledSpace3D* it = dynamic_cast<TiledSpace3D*>(scene->graphicsContainer.Get());
			return it->IsParentScene(_scene);
		}
	}
	return false;
} 


void RenderList::RemoveNodeEvent(MagicNode* _node)
{
	GraphicsObject* object = dynamic_cast<GraphicsObject*>(_node);

	object->graphicsContainer.Clear(MagicContainer::REMOVE_NODE);
	object->ResetTransformationValid();

	if(scene->IsReceiversExist(GraphicsScene::REMOVE_FROM_RENDER_LIST_MESSAGE)) { scene->SendPackage(GraphicsScene::REMOVE_FROM_RENDER_LIST_MESSAGE, object); }
}


void RenderList::AddNodeEvent(MagicNode* _node)
{
	GraphicsObject* object = dynamic_cast<GraphicsObject*>(_node);

	object->graphicsContainer.Attach(scene);
	object->ResetTransformationValid();
		
	if(scene->IsReceiversExist(GraphicsScene::ADD_TO_RENDER_LIST_MESSAGE)) { scene->SendPackage(GraphicsScene::ADD_TO_RENDER_LIST_MESSAGE, object); }
}


AbstractObject* RenderList::FindObject(StringANSI _name)
{
	return Find(_name, &AbstractObject::GetName);
}


