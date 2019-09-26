#include "LogicSceneItem.h"
#include <Editor/AssetManager/LogicTreeWidget/LogicTreeWidget.h>
#include <Engine/Core/Scene/LogicScene/LogicScene.h>



LogicSceneItem::LogicSceneItem(QTreeWidgetItem* _parent, Type _assetType, int64 _objectType): QTreeWidgetItem(_parent, _assetType), objectType(_objectType), object(NIL)
{}


int64 LogicSceneItem::GetObjectType(void)const
{
	return objectType;
}


LogicObject* LogicSceneItem::GetObjectPtr(void)const
{
	return object;
}



void LogicSceneItem::SetObjectPtr(LogicObject* _object)
{
	object = _object;
}



void LogicSceneItem::AddToLogicScene(LogicObject* _object)
{
	LogicSceneItem* item = treeWidget->FindItem(_object->GetName().c_str());

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
			item = dynamic_cast<LogicSceneItem*>(treeWidget->root->takeChild(index));

			addChild(item);
		}
	}
}


void LogicSceneItem::RemoveFromLogicScene(LogicObject* _object)
{
	LogicSceneItem* item = treeWidget->FindItem(_object->GetName().c_str(), text(LogicTreeWidget::NAME_PROPERTY));

	if(item)
	{
		if(item->parent() != treeWidget->invisibleRootItem())
		{
			QTreeWidgetItem* parent = item->parent();
			
			bool isSelected = item->isSelected();

			int index = parent->indexOfChild(item);
			item = dynamic_cast<LogicSceneItem*>(parent->takeChild(index));
			
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