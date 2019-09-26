#include "GraphicsSceneItem.h"
#include <Editor/AssetManager/GraphicsTreeWidget/GraphicsTreeWidget.h>
#include <Engine/Output/Graphics/Scene/GraphicsScene/GraphicsScene.h>



GraphicsSceneItem::GraphicsSceneItem(QTreeWidgetItem* _parent, Type _assetType, int64 _objectType): QTreeWidgetItem(_parent, _assetType), objectType(_objectType), object(NIL)
{}


int64 GraphicsSceneItem::GetObjectType(void)const
{
	return objectType;
}


GraphicsObject* GraphicsSceneItem::GetObjectPtr(void)const
{
	return object;
}



void GraphicsSceneItem::SetObjectPtr(GraphicsObject* _object)
{
	object = _object;
}



void GraphicsSceneItem::AddToGraphicsScene(GraphicsObject* _object)
{
	GraphicsSceneItem* item = treeWidget->FindItem(_object->GetName().c_str());

	if(item && item->parent() != treeWidget->invisibleRootItem())
	{
		if(item->parent() == this)
		{
			return;
		}
		if(item->parent() != treeWidget->root)
		{
		}
		else if(item->parent() == treeWidget->root)
		{
			int index = treeWidget->root->indexOfChild(item);
			item = dynamic_cast<GraphicsSceneItem*>(treeWidget->root->takeChild(index));

			addChild(item);
		}
	}
}


void GraphicsSceneItem::RemoveFromGraphicsScene(GraphicsObject* _object)
{
	GraphicsSceneItem* item = treeWidget->FindItem(_object->GetName().c_str(), text(GraphicsTreeWidget::NAME_PROPERTY));

	if(item)
	{
		if(item->parent() != treeWidget->invisibleRootItem())
		{
			QTreeWidgetItem* parent = item->parent();

			bool isSelected = item->isSelected();

			int index = parent->indexOfChild(item);
			item = dynamic_cast<GraphicsSceneItem*>(parent->takeChild(index));
			
			if(treeWidget->FindItem(_object->GetName().c_str()) == NIL)
			{
				treeWidget->root->addChild(item);

				if(isSelected) { item->setSelected(true); }
			}
			else
			{
				delete item;
			}
		}
	}
}