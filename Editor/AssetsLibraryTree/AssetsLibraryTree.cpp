#include "AssetsLibraryTree.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Parser/List/AbstractList.h>
#include <Editor/TriggerCreator/ActionCreator/ActionCreator.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Engine/Output/Graphics/Surface/GraphicsSurface/GraphicsSurface.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Editor/TriggerCreator/EventCreator/EventCreator.h>
#include <Editor/ObjectCreator/ObjectCreator.h>
#include <Editor/TriggerCreator/VariableCreator/VariableCreator.h>
#include <Editor/PlayerControlCreator/PlayerControlCreator.h>
#include <Editor/QuaternionLerpCreator/QuaternionLerpCreator.h>
#include <Editor/FloatLerpCreator/FloatLerpCreator.h>
#include <Editor/TextureCreator/TextureCreator.h>



AssetsLibraryTree::AssetsLibraryTree(QWidget* _parent): QTreeWidget(_parent), creatingAssetsEnable(false), editingAssetsEnable(false)
{
	setSortingEnabled(true);
	categoryBeginColor = QColor(205,205,205);
	texturedAnimationIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/t-anim.png");
	atlasIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/atlas.png");
	textureIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/texture.png");

	triggerIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/trigger.png");
	variableIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/variable.png");
	listIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/list.png");
	actionIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/action.png");
	eventIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/event.png");
	
	sceneIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/scene.png");
	objectIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/object.png");
	playerControlIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/p-control.png");
	meshIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/mesh.png");
	hitboxIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/hitbox.png");
	
	floatLerpIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/f-lerp.png");
	quaternionLerpIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/q-lerp.png");
	vectorLerpIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/v-lerp.png");

	cameraIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/camera.png");
	
	folderIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/folder.png");
    setSelectionMode(QAbstractItemView::ExtendedSelection);
	header()->setMovable(false);
	setExpandsOnDoubleClick(true);
	setAnimated(true);

	
	setColumnCount(MAX_COLUMNS);
	headerItem()->setText(ICON_PROPERTY, "");
	headerItem()->setText(NAME_PROPERTY, "Name");
	headerItem()->setText(TYPE_PROPERTY, "Type");
	QFont font("Verdana", 10, QFont::Bold, false);
	headerItem()->setFont(NAME_PROPERTY, font);
	headerItem()->setFont(TYPE_PROPERTY, font);
	headerItem()->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
	headerItem()->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
	setColumnWidth(ICON_PROPERTY, 80);
	setColumnWidth(NAME_PROPERTY, 880);
	setColumnWidth(TYPE_PROPERTY, 150);
	header()->setResizeMode(ICON_PROPERTY, QHeaderView::Fixed);
	header()->setResizeMode(TYPE_PROPERTY, QHeaderView::Fixed);
	CreateContextMenu();
	setStyleSheet("QTreeView::item { height: 16px; }"
		          "QTreeWidget::item:!selected { border: 2px solid gainsboro; "
		                                        "border-left: none; border-top: none; }"
		          "QTreeWidget::item:selected { background-color: rgb(149,199,233);"
                                               "border: 2px solid gainsboro;"
											   "border-left: none; border-top: none; }"); 	

	Refresh();
}



AssetsLibraryTree::~AssetsLibraryTree(void)
{}


void AssetsLibraryTree::SetCreatingAssetsEnable(bool _enable)
{
	creatingAssetsEnable = _enable;
}


void AssetsLibraryTree::SetEditingAssetsEnable(bool _enable)
{
	editingAssetsEnable = _enable;
}


void AssetsLibraryTree::UpdateContextMenu(void)
{
	QTreeWidgetItem* it = itemAt(cursorPoint);
	if(creatingAssetsEnable) { contextMenu->removeAction(newAssetAct); }
	contextMenu->removeAction(refreshAct);
	if(editingAssetsEnable) { contextMenu->removeAction(editAssetAct); }
	contextMenu->removeAction(removeItemAct);


	if(it)
	{
		if(it->type() == Item::ASSET_TYPE)
		{
			if(editingAssetsEnable) { contextMenu->addAction(editAssetAct); }
			contextMenu->addAction(removeItemAct);
		}
		else
		{
			if(creatingAssetsEnable) { contextMenu->addAction(newAssetAct); }
			contextMenu->addAction(removeItemAct);
			contextMenu->addAction(refreshAct);
		}
	}
	else
	{
		if(creatingAssetsEnable) { contextMenu->addAction(newAssetAct); }
		contextMenu->addAction(refreshAct);
	}
}



void AssetsLibraryTree::CreateContextMenu(void)
{
	contextMenu = new QMenu(this);
	newAssetAct = new QAction("New asset", contextMenu);
    connect(newAssetAct, SIGNAL(triggered()), SLOT(NewAsset()));
	editAssetAct = new QAction("Edit", contextMenu);
    connect(editAssetAct, SIGNAL(triggered()), SLOT(EditAsset()));
	removeItemAct = new QAction("Remove", contextMenu);
	connect(removeItemAct, SIGNAL(triggered()), SLOT(RemoveSelectedItem()));
	refreshAct = new QAction("Refresh", contextMenu);
	connect(refreshAct, SIGNAL(triggered()), SLOT(RefreshAssetTree()));
}


void AssetsLibraryTree::NewAsset(void)
{ 
	Item* item = dynamic_cast<Item*>(itemAt(cursorPoint));
	
	if(item)
	{
		if(item->GetItemType() == Item::CATEGORY_TYPE)
		{
			if(item->GetName() == "ACTION")
			{
				ActionCreator* dialog = new ActionCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "EVENT")
			{
				EventCreator* dialog = new EventCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			} 
			else if(item->GetName() == "TRIGGER")
			{
				TriggerCreator* dialog = new TriggerCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "VARIABLE")
			{
				VariableCreator* dialog = new VariableCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "OBJECT")
			{
				ObjectCreator* dialog = new ObjectCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "FLOAT_LERP")
			{
				FloatLerpCreator* dialog = new FloatLerpCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "QUATERNION_LERP")
			{
				QuaternionLerpCreator* dialog = new QuaternionLerpCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "VECTOR_LERP")
			{
				TextureCreator* dialog = new TextureCreator();
				dialog->SetVectorLerpEditor();

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}  
			else if(item->GetName() == "PLAYER_CONTROL")
			{
				PlayerControlCreator* dialog = new PlayerControlCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "TEXTURE")
			{
				TextureCreator* dialog = new TextureCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "ATLAS")
			{
				TextureCreator* dialog = new TextureCreator();
				dialog->SetAtlasEditor();

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "TEXTURED_ANIMATION")
			{
				TextureCreator* dialog = new TextureCreator();
				dialog->SetTexturedAnimationEditor();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "SCENE")
			{
				ObjectCreator* dialog = new ObjectCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "MESH")
			{  
				TextureCreator* dialog = new TextureCreator();
				dialog->SetHitboxEditor();

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "CAMERA")
			{
				ObjectCreator* dialog = new ObjectCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "HITBOX")
			{ 
				TextureCreator* dialog = new TextureCreator();
				dialog->SetHitboxEditor();

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->GetName() == "LIST")
			{
				ObjectCreator* dialog = new ObjectCreator();
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
		}
	}
	else
	{
		ObjectCreator* dialog = new ObjectCreator();
				
		dialog->setAttribute(Qt::WA_DeleteOnClose);

		dialog->show();

		connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
	}
}



void AssetsLibraryTree::EditAsset(void)
{
	Item* item = dynamic_cast<Item*>(currentItem());
	
	if(item)
	{
		if(item->GetItemType() == Item::ASSET_TYPE)
		{
			if(item->text(TYPE_PROPERTY) == "ACTION")
			{
				ActionCreator* dialog = new ActionCreator();
				
				dialog->SetAction(item->GetName());
				
				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->text(TYPE_PROPERTY) == "EVENT")
			{
				EventCreator* dialog = new EventCreator();
				
				dialog->SetEvent(item->GetName());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			} 
			else if(item->text(TYPE_PROPERTY) == "TRIGGER")
			{
				TriggerCreator* dialog = new TriggerCreator();
				
				dialog->SetTrigger(item->GetName());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->text(TYPE_PROPERTY) == "VARIABLE")
			{
				VariableCreator* dialog = new VariableCreator();
				
				dialog->SetVariable(item->GetName());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->text(TYPE_PROPERTY) == "FLOAT_LERP")
			{
				FloatLerpCreator* dialog = new FloatLerpCreator();
				
				dialog->SetFloatLerp(item->GetName());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->text(TYPE_PROPERTY) == "QUATERNION_LERP")
			{
				QuaternionLerpCreator* dialog = new QuaternionLerpCreator();
				
				dialog->SetQuaternionLerp(item->GetName());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->text(TYPE_PROPERTY) == "PLAYER_CONTROL")
			{
				PlayerControlCreator* dialog = new PlayerControlCreator();
				
				dialog->SetPlayerControl(item->GetName());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->text(TYPE_PROPERTY) == "TEXTURE")
			{
				TextureCreator* dialog = new TextureCreator();
				
				dialog->SetTexture(item->GetName());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->text(TYPE_PROPERTY) == "ATLAS")
			{
				TextureCreator* dialog = new TextureCreator();
				dialog->SetAtlasEditor();

				dialog->SetAtlas(item->GetName());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else if(item->text(TYPE_PROPERTY) == "TEXTURED_ANIMATION")
			{
				TextureCreator* dialog = new TextureCreator();
				dialog->SetTexturedAnimationEditor();
				
				dialog->SetTexturedAnimation(item->GetName());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->show();

				connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
			}
			else
			{
				int assetType = AssetLibrary::_IsAssetExist(item->GetName().toStdString());

				if(assetType != AssetLibrary::UNKNOWN_ASSET)
				{
					ObjectCreator* dialog = new ObjectCreator();
						
					QString suffix = AssetLibrary::_GetFileSuffixOfAssetType(assetType).c_str();
					QString dir = AssetLibrary::_GetDirOfAssetType(assetType).c_str();
 
					dialog->ReviewObject(dir + item->GetName() + QString(".") + suffix);

					dialog->setAttribute(Qt::WA_DeleteOnClose);

					dialog->show();

					connect(dialog, SIGNAL(accepted()), this, SLOT(RefreshAssetTree()));
				}
			}
		} 
	} 
}


int AssetsLibraryTree::GetStringOfAssetType(QString _assetType)const
{
	if(_assetType == "OBJECT")
	{
		return AssetLibrary::OBJECT_ASSET;
	}
	else if(_assetType == "TEXTURED_ANIMATION")
	{
		return AssetLibrary::TEXTURED_ANIMATION_ASSET;
	}
	else if(_assetType == "ATLAS")
	{
		return AssetLibrary::ATLAS_2D_ASSET;
	}
	else if(_assetType == "TEXTURE")
	{
		return AssetLibrary::TEXTURE_2D_ASSET;
	}
	else if(_assetType == "TRIGGER")
	{
		return AssetLibrary::TRIGGER_ASSET;
	}
	else if(_assetType == "EVENT")
	{
		return AssetLibrary::EVENT_ASSET;
	}
	else if(_assetType == "ACTION")
	{
		return AssetLibrary::ACTION_ASSET;
	}
	else if(_assetType == "VARIABLE")
	{
		return AssetLibrary::VARIABLE_ASSET;
	}
	else if(_assetType == "LIST")
	{
		return AssetLibrary::LIST_ASSET;
	}
	else if(_assetType == "MESH")
	{
		return AssetLibrary::MESH_ASSET;
	}
	else if(_assetType == "HITBOX")
	{
		return AssetLibrary::HITBOX_ASSET;
	}
	else if(_assetType == "PLAYER_CONTROL")
	{
		return AssetLibrary::PLAYER_CONTROL_ASSET;
	}
	else if(_assetType == "SCENE")
	{
		return AssetLibrary::SCENE_ASSET;
	}
	else if(_assetType == "FLOAT_LERP")
	{
		return AssetLibrary::FLOAT_LERP_ASSET;
	}
	else if(_assetType == "QUATERNION_LERP")
	{
		return AssetLibrary::QUATERNION_LERP_ASSET;
	}
	else if(_assetType == "VECTOR_LERP")
	{
		return AssetLibrary::VECTOR_LERP_ASSET;
	}
	else if(_assetType == "CAMERA")
	{
		return AssetLibrary::CAMERA_ASSET;
	} 
	
	return AssetLibrary::UNKNOWN_ASSET;
}


QString AssetsLibraryTree::GetStringOfAssetType(int32 _assetType)const
{
	switch(_assetType)
	{
		case AssetLibrary::OBJECT_ASSET:
		{
			return "OBJECT";
		}

		case AssetLibrary::TEXTURED_ANIMATION_ASSET:
		{
			return "TEXTURED_ANIMATION";
		}
		
		case AssetLibrary::ATLAS_2D_ASSET:
		{
			return "ATLAS";
		}

		case AssetLibrary::TEXTURE_2D_ASSET:
		{
			return "TEXTURE";
		} 

		case AssetLibrary::TRIGGER_ASSET:
		{
			return "TRIGGER";
		}

		case AssetLibrary::EVENT_ASSET:
		{
			return "EVENT";
		}

		case AssetLibrary::ACTION_ASSET:
		{
			return "ACTION";
		}

		case AssetLibrary::VARIABLE_ASSET:
		{
			return "VARIABLE";
		}
			
		case AssetLibrary::LIST_ASSET:
		{
			return "LIST";
		}

		case AssetLibrary::MESH_ASSET:
		{
			return "MESH";
		}

		case AssetLibrary::HITBOX_ASSET:
		{
			return "HITBOX";
		} 

		case AssetLibrary::PLAYER_CONTROL_ASSET:
		{
			return "PLAYER_CONTROL";
		}

		case AssetLibrary::SCENE_ASSET:
		{
			return "SCENE";
		}

		case AssetLibrary::FLOAT_LERP_ASSET:
		{
			return "FLOAT_LERP";
		}

		case AssetLibrary::QUATERNION_LERP_ASSET:
		{
			return "QUATERNION_LERP";
		}

		case AssetLibrary::VECTOR_LERP_ASSET:
		{
			return "VECTOR_LERP";
		}

		case AssetLibrary::CAMERA_ASSET:
		{
			return "CAMERA";
		} 
	}
	
	return "";
}



void AssetsLibraryTree::mouseDoubleClickEvent(QMouseEvent* _event)
{
	if(_event->button() == Qt::LeftButton)
	{ 
		cursorPoint = _event->pos();

		EditAsset();
	}

	QTreeWidget::mouseDoubleClickEvent(_event);
}



void AssetsLibraryTree::RefreshAssetTree(void)
{ 
}


void AssetsLibraryTree::Refresh(void)
{ 
}

 
void AssetsLibraryTree::CreateCategory(int _assetType)
{
	if(_assetType != AssetLibrary::UNKNOWN_ASSET)
	{
		QString path = QString(AssetLibrary::_GetDir().c_str()) + QString(AssetLibrary::_GetDirOfAssetType(_assetType).c_str());
		
		QDir dir(path); 

		QFileInfoList assets = dir.entryInfoList(QStringList() << "*." + QString(AssetLibrary::_GetFileSuffixOfAssetType(_assetType).c_str()), QDir::Files);

		AbstractObject* asset;
		for(int i = 0; i < assets.size(); i++)
		{
			AddAsset(assets.at(i).completeBaseName(), _assetType);
		} 
	}
}


AssetsLibraryTree::Item* AssetsLibraryTree::FindCategory(QString _name)
{
	return FindCategory(invisibleRootItem(), _name);
}



AssetsLibraryTree::Item* AssetsLibraryTree::FindCategory(QTreeWidgetItem* _it, QString _name)
{
	if(_it)
	{
		if(_it != invisibleRootItem())
		{ 
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::CATEGORY_TYPE)
			{
				if(item->GetName() == _name)
				{
					return item;
				}
			} 
		}
		
		Item* search;
		int child = 0;

		while(_it->child(child))
		{
			if(search = FindCategory(_it->child(child), _name))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}



AssetsLibraryTree::Item* AssetsLibraryTree::FindAsset(QString _name)
{
	return FindAsset(invisibleRootItem(), _name);
}




AssetsLibraryTree::Item* AssetsLibraryTree::FindAsset(QTreeWidgetItem* _it, QString _name)
{
	if(_it)
	{
		if(_it != invisibleRootItem())
		{
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::ASSET_TYPE)
			{
				if(item->GetName() == _name)
				{
					return item;
				}
			}
		}
		
		Item* search;
		int child = 0;

		while(_it->child(child))
		{
			if(search = FindAsset(_it->child(child), _name))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}

 



QPixmap AssetsLibraryTree::GetIcon(int _assetType)
{
	switch(_assetType)
	{
		case AssetLibrary::ATLAS_2D_ASSET:
		{
			return atlasIcon;
		}

		case AssetLibrary::TEXTURE_2D_ASSET:
		{
			return textureIcon;
		}

		case AssetLibrary::TEXTURED_ANIMATION_ASSET:
		{
			return texturedAnimationIcon;
		}

		case AssetLibrary::TRIGGER_ASSET:
		{
			return triggerIcon;
		}

		case AssetLibrary::EVENT_ASSET:
		{
			return eventIcon;
		}

		case AssetLibrary::ACTION_ASSET:
		{
			return actionIcon;
		}

		case AssetLibrary::VARIABLE_ASSET:
		{
			return variableIcon;
		}

		case AssetLibrary::LIST_ASSET:
		{
			return listIcon;
		}

		case AssetLibrary::PLAYER_CONTROL_ASSET:
		{
			return playerControlIcon;
		}

		case AssetLibrary::MESH_ASSET:
		{
			return meshIcon;
		}
	
		case AssetLibrary::HITBOX_ASSET:
		{
			return hitboxIcon;
		}

		case AssetLibrary::SCENE_ASSET:
		{
			return sceneIcon;
		}

		case AssetLibrary::FLOAT_LERP_ASSET:
		{
			return floatLerpIcon;
		}

		case AssetLibrary::QUATERNION_LERP_ASSET:
		{
			return quaternionLerpIcon;
		}

		case AssetLibrary::VECTOR_LERP_ASSET:
		{
			return vectorLerpIcon;
		}

		case AssetLibrary::CAMERA_ASSET:
		{
			return cameraIcon;
		}
	}
	return objectIcon;
}


void AssetsLibraryTree::AddAsset(QString _name, int _assetType)
{
	Item* it = FindAsset(_name);

	if(it == NIL)
	{
		Item* category = FindCategory(GetStringOfAssetType(_assetType));
		if(category == NIL)
		{
			category = AddCategory(_assetType);
		}
		it = new Item(category, _assetType, Item::ASSET_TYPE);
		it->setIcon(ICON_PROPERTY, GetIcon(_assetType)); 

		it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		it->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
		it->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
		QFont font("Verdana", 8, QFont::Normal, false);
		it->setFont(NAME_PROPERTY, font);
		it->setFont(TYPE_PROPERTY, font);

		it->SetName(_name);
		it->setText(TYPE_PROPERTY, GetStringOfAssetType(_assetType));

		category->setText(NAME_PROPERTY, category->GetName() + "(" + QString::number(GetAmountOfItems(category)) + ")");
	}
	else
	{
		it->SetNameReserved(true);
		it->setToolTip(NAME_PROPERTY, "One or more assets have identical names");
	}
}



int AssetsLibraryTree::GetAmountOfItems(QTreeWidgetItem* _it)
{
	if(_it)
	{
		int child = 0;

		while(_it->child(child))
		{
			child++;
		}
		return child;
	}
	return 0;
}




void AssetsLibraryTree::mouseMoveEvent(QMouseEvent *_event)
{
	if(_event->buttons() == Qt::LeftButton)
	{
		if(!selectedItems().isEmpty())
		{
			QDrag *drag = new QDrag(this);
			QMimeData *mimeData = new QMimeData;
			QList<QUrl> list;
			for(int i = 0; i < selectedItems().size(); i++)
			{
				Item* item = dynamic_cast<Item*>(selectedItems().at(i));
				
				if(item && item->GetItemType() == Item::ASSET_TYPE)
				{
					list.append(QUrl(item->GetName()));
				}
			}

			mimeData->setUrls(list);
			drag->setMimeData(mimeData);
			drag->start(Qt::CopyAction);
		}
	}
}



AssetsLibraryTree::Item* AssetsLibraryTree::AddCategory(int _assetType)
{
	Item* it = new Item(invisibleRootItem(), _assetType, Item::CATEGORY_TYPE);
	it->setForeground(NAME_PROPERTY, Qt::white);
	it->setForeground(TYPE_PROPERTY, Qt::white);

	it->setIcon(ICON_PROPERTY, folderIcon); 
	
	it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	it->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
	it->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
	QFont font1("Verdana", 8, QFont::Bold, false);
	it->setFont(NAME_PROPERTY, font1);
	
	QFont font2("Verdana", 8, QFont::Normal, false);
	it->setFont(TYPE_PROPERTY, font2);

	it->SetName(GetStringOfAssetType(_assetType));
	it->setText(TYPE_PROPERTY, "CATEGORY");

	return it;
}



void AssetsLibraryTree::mousePressEvent(QMouseEvent* _event)
{
	QTreeWidget::mousePressEvent(_event);
	
	if(_event->button() == Qt::RightButton)
	{
		QPoint mousePos(QCursor::pos().x(), QCursor::pos().y()); 
		cursorPoint = _event->pos();

		UpdateContextMenu();

		contextMenu->move(mousePos);
		contextMenu->show();
	}
	else if(_event->button() == Qt::LeftButton)
	{
		cursorPoint = _event->pos();
	}
}



void AssetsLibraryTree::drawRow(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index)const
{
	QStyleOptionViewItem newOption(_option);

	Item* it = dynamic_cast<Item*>(itemFromIndex(_index));

	if(it)
	{
		if(it->GetItemType() == Item::CATEGORY_TYPE)
		{ 
			QBrush brush(QColor(128, 128, 128));
			_painter->fillRect(_option.rect, brush);
			QTreeWidget::drawRow(_painter, _option, _index);
			return;
		}
		if(it->GetItemType() == Item::ASSET_TYPE)
		{ 
			if(it->IsNameReserved())
			{
				QBrush brush(QColor(229, 229, 7));
				_painter->fillRect(_option.rect, brush);
				QTreeWidget::drawRow(_painter, _option, _index);
				return;
			}
		}
	}

    QTreeWidget::drawRow(_painter, _option, _index);
}



void AssetsLibraryTree::RemoveSelectedItem(void)
{
	WarningDialog warning("Warning", "Selected assets will be deleted. Apply?", 270, 90);
		
	if(warning.exec() == QDialog::Rejected)
	{
		return;
	}
	while(!selectedItems().isEmpty())
	{
		Item* it = dynamic_cast<Item*>(FindLeaf(selectedItems()));
		
		if(it)
		{
			if(it->GetItemType() == Item::ASSET_TYPE)
			{ 
				Item* parent = dynamic_cast<Item*>(it->parent());
					
				QString name = it->GetName();
				
				File::_Remove(AssetLibrary::_GetDir() + AssetLibrary::_GetDirOfAssetType(it->GetAssetType()) + name.toStdString() + "." + AssetLibrary::_GetFileSuffixOfAssetType(it->GetAssetType()));
			
				delete it;

				if(parent)
				{
					if(parent->child(0) == NIL)
					{
						parent->setSelected(true);
					}
					else
					{
						parent->setText(NAME_PROPERTY, parent->GetName() + "(" + QString::number(GetAmountOfItems(parent)) + ")");
					}
				}
				
				parent->setText(NAME_PROPERTY, parent->GetName() + "(" + QString::number(GetAmountOfItems(parent)) + ")");

				emit AssetIsRemoved();
			}
			else
			{
				if(it->GetItemType() == Item::CATEGORY_TYPE)
				{
					delete it;
				}
			}
		}
	}
}


void AssetsLibraryTree::keyPressEvent(QKeyEvent* _event)
{
	if(!_event->isAutoRepeat())
	{
		if(_event->key() == Qt::Key_Delete)
		{
			RemoveSelectedItem();
		}
		else if(_event->key() == Qt::Key_Return)
		{
			EditAsset();
		}
	}
	
	QTreeWidget::keyPressEvent(_event);
}


QTreeWidgetItem* AssetsLibraryTree::FindLeaf(QList<QTreeWidgetItem*>& _list)
{
	for(int i = 0; i < _list.count(); ++i)
	{
		QTreeWidgetItem* it = _list.at(i);
		
		while(it->child(0))
		{
			it = it->child(0);
		}
		return it;
	}
	return NIL;
}
