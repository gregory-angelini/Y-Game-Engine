#include "ObjectSceneItem.h"
#include <Editor/AssetManager/ObjectTreeWidget/ObjectTreeWidget.h>
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>



ObjectSceneItem::ObjectSceneItem(Type _assetType): QTreeWidgetItem(_assetType), object(NIL), objectType(0)
{}



ObjectSceneItem::~ObjectSceneItem(void)
{
	if(treeWidget->dropItem && this == treeWidget->dropItem)
	{
		treeWidget->dropItem = NIL;
	}
}


void ObjectSceneItem::SetObjectClass(int64 _classType)
{
	objectType = _classType;
}



int64 ObjectSceneItem::GetObjectClass(void)const
{
	return objectType;
}


AbstractObject* ObjectSceneItem::GetObjectPtr(void)const
{
	return object;
}



void ObjectSceneItem::SetObjectPtr(AbstractObject* _object)
{
	object = _object;
}



void ObjectSceneItem::AddToObjectScene(AbstractObject* _object)
{
	ObjectSceneItem* item = treeWidget->FindItem(_object->GetName().c_str());

	if(item)
	{
		if(item->parent() == this)
		{
			return;
		}
		if(item->parent() != treeWidget->root)
		{
			treeWidget->AddAsset(item->text(ObjectTreeWidget::NAME_PROPERTY), (ObjectSceneItem::Type)item->type(), item->GetObjectClass(), item->GetObjectPtr(), this);
		}
		else if(item->parent() == treeWidget->root)
		{
			int index = treeWidget->root->indexOfChild(item);
			item = dynamic_cast<ObjectSceneItem*>(treeWidget->root->takeChild(index));

			addChild(item);
		}
	}
}



void ObjectSceneItem::RemoveFromObjectScene(AbstractObject* _object)
{
	ObjectSceneItem* item = treeWidget->FindItem(_object->GetName().c_str(), text(ObjectTreeWidget::NAME_PROPERTY));

	if(item)
	{ 
		if(item->parent() != treeWidget->invisibleRootItem())
		{
			QTreeWidgetItem* parent = item->parent();

			bool isSelected = item->isSelected();

			int index = parent->indexOfChild(item);
			item = dynamic_cast<ObjectSceneItem*>(parent->takeChild(index));
			
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