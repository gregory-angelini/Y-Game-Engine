#include "ObjectTreeWidget.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Parser/List/AbstractList.h>
#include <Editor/TriggerCreator/ActionCreator/ActionCreator.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Editor/NameCreator/NameCreator.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Editor/ObjectCreator/ObjectCreator.h>
#include <Engine/Core/Time/Timer/Timer.h>



ObjectTreeWidget::ObjectTreeWidget(QWidget* _parent): QTreeWidget(_parent), root(NIL), scrollTimerId(0), ignoreSelection(false), dropItem(NIL), autoParentEnable(true), altPressed(false)
{ 
	setMouseTracking(true);
	setAcceptDrops(true);
	setDragEnabled(true);
	setDragDropMode(QAbstractItemView::InternalMove);
	setDropIndicatorShown(true);
	setExpandsOnDoubleClick(false);
	setAnimated(true);


	setColumnCount(MAX_COLUMNS);
	headerItem()->setText(ICON_PROPERTY, "");
	headerItem()->setText(NAME_PROPERTY, "Name");
	headerItem()->setText(TYPE_PROPERTY, "Type");
    setSelectionMode(QAbstractItemView::ExtendedSelection);
	QFont font("Verdana", 10, QFont::Bold, false);
	headerItem()->setFont(NAME_PROPERTY, font);
	headerItem()->setFont(TYPE_PROPERTY, font);
	headerItem()->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
	headerItem()->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
	setColumnWidth(ICON_PROPERTY, 80);
	setColumnWidth(NAME_PROPERTY, 200);
	setColumnWidth(TYPE_PROPERTY, 100);
    header()->setMovable(false);
	CreateContextMenu();
	setStyleSheet("QTreeView::item { height: 16px; }"
		          "QTreeWidget::item:!selected { border: 2px solid gainsboro; "
		                                        "border-left: none; border-top: none; }"
		          "QTreeWidget::item:selected { background-color: rgb(149,199,233);"
                                               "border: 2px solid gainsboro;"
											   "border-left: none; border-top: none; }"); 

	texturedAnimationIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/t-anim.png");
	atlasIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/atlas.png");
	textureIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/texture.png");

	triggerIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/trigger.png");
	variableIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/variable.png");
	listIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/list.png");
	actionIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/action.png");
	eventIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/event.png");
	
	sceneIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/scene.png");
	objectIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/object.png");
	playerControlIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/p-control.png");
	meshIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/mesh.png");
	timerIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/timer.png");
	hitboxIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/hitbox.png");
	
	floatLerpIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/f-lerp.png");
	quaternionLerpIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/q-lerp.png");
	vectorLerpIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/v-lerp.png");

	cameraIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/camera.png");

	connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(ChangeSelectList()));
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ObjectTreeWidget::ObjectIsCreated));


	CreateRoot();
	show();
}



ObjectTreeWidget::~ObjectTreeWidget(void)
{}



ObjectSceneItem* ObjectTreeWidget::FindItem(AbstractObject* _object)
{
	return FindItem(root, _object);
}



ObjectSceneItem* ObjectTreeWidget::FindItem(QTreeWidgetItem* _it, AbstractObject* _object)
{
	if(_it)
	{
		if(_it != root)
		{
			ObjectSceneItem* item = dynamic_cast<ObjectSceneItem*>(_it);

			if(item->GetObjectPtr() == _object)
			{
				return item;
			}
		}
		
		ObjectSceneItem* search;
		int child = 0;

		while(_it->child(child))
		{
			if(search = FindItem(_it->child(child), _object))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}



void ObjectTreeWidget::Unselect(GraphicsSurface* _object, bool _ignoreSelection)
{
	if(_object)
	{
		ignoreSelection = _ignoreSelection;

		ObjectSceneItem* it = FindItem(_object);

		if(it) 
		{ 
			it->setSelected(false); 

			selectList.Remove(dynamic_cast<GraphicsSurface*>(it->GetObjectPtr()), MagicContainer::REMOVE_NODE);
		}

		ignoreSelection = false;
	}
}


void ObjectTreeWidget::Select(GraphicsSurface* _object, bool _ignoreSelection)
{
	if(_object)
	{
		ignoreSelection = _ignoreSelection;

		ObjectSceneItem* it = FindItem(_object);

		if(it) 
		{ 
			it->setSelected(true); 

			selectList.AddToTail(dynamic_cast<GraphicsSurface*>(it->GetObjectPtr()));
		}

		ignoreSelection = false;
	}
}


void ObjectTreeWidget::ChangeSelectList(void)
{
	if(!ignoreSelection)
	{
		while(selectList.IsNotEmpty())
		{
			GraphicsSurface* object = selectList.GetHead();

			emit UnselectObject(object);
			selectList.Remove(object, MagicContainer::REMOVE_NODE);
		}
		QList<QTreeWidgetItem*> list = selectedItems();
		for(int j = 0; j < list.size(); ++j)
		{
			ObjectSceneItem* it = dynamic_cast<ObjectSceneItem*>(list.at(j));
				
			AbstractObject* object = it->GetObjectPtr();

			if(object && object->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS))
			{ 
				if(selectList.AddToTail(dynamic_cast<GraphicsSurface*>(object)))
				{
					emit SelectObject(dynamic_cast<GraphicsSurface*>(object));
				}
			}
		}
	}
}






void ObjectTreeWidget::CreateContextMenu(void)
{
	contextMenu = new QMenu(this);
	importItemAct = new QAction("Import", contextMenu);
    connect(importItemAct, SIGNAL(triggered()), SLOT(ImportAsset()));
	contextMenu->addAction(importItemAct);
	saveAllAssetsAct = new QAction("Save all", contextMenu);
    connect(saveAllAssetsAct, SIGNAL(triggered()), SLOT(SaveAllAssets()));
	contextMenu->addAction(saveAllAssetsAct);
	saveLayersStructureAct = new QAction("Save layers structure", contextMenu);
    connect(saveLayersStructureAct, SIGNAL(triggered()), SLOT(SaveLayersStructure()));
	contextMenu->addAction(saveLayersStructureAct);
	renameItemAct = new QAction("Rename", contextMenu);
	connect(renameItemAct, SIGNAL(triggered()), SLOT(RenameCurrentItem()));
	contextMenu->addAction(renameItemAct);
	reviewItemAct = new QAction("Review", contextMenu);
    connect(reviewItemAct, SIGNAL(triggered()), SLOT(ReviewAsset()));
	contextMenu->addAction(reviewItemAct);
	saveToFileItemAct = new QAction("Save to file", contextMenu);
	connect(saveToFileItemAct, SIGNAL(triggered()), SLOT(SaveToFileSelectedItems()));
	contextMenu->addAction(saveToFileItemAct);
	destroyItemAct = new QAction("Destroy", contextMenu);
	connect(destroyItemAct, SIGNAL(triggered()), SLOT(DestroySelectedItems()));
	contextMenu->addAction(destroyItemAct);
	removeFromSceneAct = new QAction("Move out", contextMenu);
	connect(removeFromSceneAct, SIGNAL(triggered()), SLOT(RemoveFromSceneSelectedItems()));
	contextMenu->addAction(removeFromSceneAct);
	copyItemAct = new QAction("Copy", contextMenu);
	connect(copyItemAct, SIGNAL(triggered()), SLOT(CopyCurrentItem()));
	pickItemAct = new QAction("Pick", contextMenu);
	pickItemAct->setCheckable(true);
	connect(pickItemAct, SIGNAL(triggered()), SLOT(PickCurrentItem()));
	activateItemAct = new QAction("Activate", contextMenu);
	connect(activateItemAct, SIGNAL(triggered()), SLOT(ActivateCurrentItem()));
}




void ObjectTreeWidget::DestroySelectedItems(void)
{
	WarningDialog warning("Warning", "Selected assets will be deleted. Apply?", 270, 90);
		
	if(warning.exec() == QDialog::Rejected)
	{
		return;
	}
	while(!selectedItems().isEmpty())
	{ 
		QTreeWidgetItem* it = FindLeaf(selectedItems());
		
		if(it)
		{
			ObjectSceneItem* item = dynamic_cast<ObjectSceneItem*>(it); 

			delete item->GetObjectPtr();
		}
	}
}


void ObjectTreeWidget::SaveToFileSelectedItems(void)
{
	QList<QTreeWidgetItem*> list = selectedItems();
	for(int i = 0; i < list.size(); i++) 
	{
		ObjectSceneItem* item = dynamic_cast<ObjectSceneItem*>(list.at(i)); 

		item->GetObjectPtr()->SaveToFile();
	}
}




void ObjectTreeWidget::CopyCurrentItem(void)
{
	ObjectSceneItem* it = static_cast<ObjectSceneItem*>(itemAt(cursorPoint));
	
	if(it)
	{
		NameCreator dialog;
		dialog.SetAssetLibraryCheckEnable(true);
		dialog.SetName(it->text(NAME_PROPERTY));
		if(dialog.exec() == QDialog::Accepted)
		{
			if(it->type() == ObjectSceneItem::VECTOR_LERP)
			{
				File file((Editor::_GetWorkDir() + Editor::_GetTempDir() + dialog.GetName() + "." + VectorLerp::_GetFileSuffix().c_str()).toStdString(), File::REWRITE);
					
				if(it->GetObjectPtr()->SaveAsToFile(file, dialog.GetName().toStdString()))
				{
					if(file.Open(File::READ))
					{
						AssetLibrary::_LoadCommonAsset(file, AssetLibrary::VECTOR_LERP_ASSET);

						file.Remove();
					}
				}
			}
			else if(it->type() == ObjectSceneItem::FLOAT_LERP)
			{
				File file((Editor::_GetWorkDir() + Editor::_GetTempDir() + dialog.GetName() + "." + FloatLerp::_GetFileSuffix().c_str()).toStdString(), File::REWRITE);
					
				if(it->GetObjectPtr()->SaveAsToFile(file, dialog.GetName().toStdString()))
				{
					if(file.Open(File::READ))
					{
						AssetLibrary::_LoadCommonAsset(file, AssetLibrary::FLOAT_LERP_ASSET);

						file.Remove();
					}
				}
			}
			else if(it->type() == ObjectSceneItem::QUATERNION_LERP)
			{
				File file((Editor::_GetWorkDir() + Editor::_GetTempDir() + dialog.GetName() + "." + QuaternionLerp::_GetFileSuffix().c_str()).toStdString(), File::REWRITE);
					
				if(it->GetObjectPtr()->SaveAsToFile(file, dialog.GetName().toStdString()))
				{
					if(file.Open(File::READ))
					{
						AssetLibrary::_LoadCommonAsset(file, AssetLibrary::QUATERNION_LERP_ASSET);

						file.Remove();
					}
				}
			}
			else if(it->type() == ObjectSceneItem::VARIABLE)
			{
				File file((Editor::_GetWorkDir() + Editor::_GetTempDir() + dialog.GetName() + "." + AbstractVariable::_GetFileSuffix().c_str()).toStdString(), File::REWRITE);
					
				if(it->GetObjectPtr()->SaveAsToFile(file, dialog.GetName().toStdString()))
				{
					if(file.Open(File::READ))
					{
						AssetLibrary::_LoadCommonAsset(file, AssetLibrary::VARIABLE_ASSET);

						file.Remove();
					}
				}
			}
			else if(it->type() == ObjectSceneItem::OBJECT)
			{
				File file((Editor::_GetWorkDir() + Editor::_GetTempDir() + dialog.GetName() + "." + AbstractObject::_GetFileSuffix().c_str()).toStdString(), File::REWRITE);
					
				if(it->GetObjectPtr()->SaveAsToFile(file, dialog.GetName().toStdString()))
				{
					if(file.Open(File::READ))
					{
						AssetLibrary::_LoadCommonAsset(file, AssetLibrary::OBJECT_ASSET);

						file.Remove();
					}
				}
			}
		}
	}
}




void ObjectTreeWidget::PickCurrentItem(void)
{
	ObjectSceneItem* it = static_cast<ObjectSceneItem*>(itemAt(cursorPoint));
	
	if(it)
	{
		AbstractObject* object = it->GetObjectPtr();

		if(object)
		{
			if(object->IsClassPropertyExist(Camera3D::CAMERA_3D_CLASS))
			{
				Camera3D* camera = dynamic_cast<Camera3D*>(object);

				if(camera)
				{
					if(camera->IsPicked())
					{
						Camera3D::_Unpick();
					}
					else
					{
						camera->Pick();
					}
				}
			}
			else if(object->IsClassPropertyExist(Scene::SCENE_CLASS))
			{
				Scene* scene = dynamic_cast<Scene*>(object);

				if(scene)
				{
					if(scene->IsPicked())
					{
						Scene::_Unpick();
					}
					else
					{
						scene->Pick();
					}
				}
			}
		}
	}
}


void ObjectTreeWidget::ActivateCurrentItem(void)
{
	ObjectSceneItem* it = static_cast<ObjectSceneItem*>(itemAt(cursorPoint));
	
	if(it)
	{
		AbstractObject* object = it->GetObjectPtr();

		if(object)
		{
			if(object->IsClassPropertyExist(Trigger::TRIGGER_CLASS))
			{
				Trigger* trigger = dynamic_cast<Trigger*>(object);

				if(trigger)
				{
					trigger->SetValue(true);
				}
			}
			else if(object->IsClassPropertyExist(AbstractEvent::EVENT_CLASS))
			{
				AbstractEvent* event = dynamic_cast<AbstractEvent*>(object);

				if(event)
				{
					event->SetValue(true);
				}
			}
		}
	}
}


void ObjectTreeWidget::RenameCurrentItem(void)
{ 
	ObjectSceneItem* it = static_cast<ObjectSceneItem*>(itemAt(cursorPoint));
	
	if(it)
	{ 
		NameCreator dialog;
		dialog.SetAssetLibraryCheckEnable(true);
		dialog.SetName(it->text(NAME_PROPERTY));
		if(dialog.exec() == QDialog::Accepted)
		{
			it->GetObjectPtr()->Rename(dialog.GetName().toStdString());
		}
	} 
}



ObjectSceneItem* ObjectTreeWidget::FindLeaf(QTreeWidgetItem* _item)
{
	ObjectSceneItem* leaf = NIL;

	if(_item)
	{
		leaf = dynamic_cast<ObjectSceneItem*>(_item->child(0));

		if(leaf)
		{
			while(leaf->child(0))
			{
				leaf = dynamic_cast<ObjectSceneItem*>(leaf->child(0));
			}
		}
	}
	return leaf;
}


QTreeWidgetItem* ObjectTreeWidget::FindLeaf(QList<QTreeWidgetItem*>& _list)
{ 

	ObjectSceneItem* leaf = NIL;
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


void ObjectTreeWidget::SaveAllAssets(void)
{
	SaveAllAssets(root);
}



void ObjectTreeWidget::SaveAllAssets(QTreeWidgetItem* _it)
{
	if(_it)
	{
		if(_it != root)
		{
			ObjectSceneItem* item = dynamic_cast<ObjectSceneItem*>(_it);

			item->GetObjectPtr()->SaveToFile();
		}
		
		ObjectSceneItem* search;
		int child = 0;

		while(_it->child(child))
		{
			SaveAllAssets(_it->child(child));

			child++;
		}
	}
}



void ObjectTreeWidget::SaveLayersStructure(void)
{
	SaveLayersStructure(root);
}



void ObjectTreeWidget::SaveLayersStructure(QTreeWidgetItem* _it)
{
	if(_it)
	{
		if(_it != root)
		{
			if(_it->type() == ObjectSceneItem::SCENE)
			{
				ObjectSceneItem* item = dynamic_cast<ObjectSceneItem*>(_it);

				item->GetObjectPtr()->SaveToFile();
			}
		}
		
		ObjectSceneItem* search;
		int child = 0;

		while(_it->child(child))
		{
			SaveLayersStructure(_it->child(child));

			child++;
		}
	}
}



void ObjectTreeWidget::ImportAsset(void)
{
	QFileDialog dialog(this, "Choose assets");
	
	dialog.setFileMode(QFileDialog::ExistingFiles);
	
	dialog.setNameFilter(QString("*.") + AbstractObject::_GetFileSuffix().c_str() + 
			             QString(" *.") + ObjectScene::_GetFileSuffix().c_str() + 
						 QString(" *.") + Trigger::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractEvent::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractAction::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractVariable::_GetFileSuffix().c_str() + 
						 QString(" *.") + PlayerControl::_GetFileSuffix().c_str() + 
						 QString(" *.") + FloatLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + QuaternionLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + VectorLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + Camera3D::_GetFileSuffix().c_str() + 
						 QString(" *.") + Texture2D::_GetFileSuffix().c_str() + 
						 QString(" *.") + Atlas2D::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractList::_GetFileSuffix().c_str() +
						 QString(" *.") + Mesh::_GetFileSuffix().c_str() +
						 QString(" *.") + Hitbox::_GetFileSuffix().c_str() +
						 QString(" *.") + Timer::_GetFileSuffix().c_str() +
						 QString(" *.") + TexturedAnimation::_GetFileSuffix().c_str());
	
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory(AssetLibrary::_GetDir().c_str());


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}
		
		bool state = Trigger::_IsBlockEnabled();
		Trigger::_SetBlockEnable(false);
		for(int i = 0; i < assets.size(); ++i)
		{
			StringANSI suffix = File::_GetSuffix(assets.at(i).toStdString());
			AssetLibrary::_LoadCommonAsset(File::_GetBaseName(assets.at(i).toStdString()), AssetLibrary::_GetAssetTypeOfFileSuffix(suffix));
		}

		Trigger::_SetBlockEnable(state);
		ExpandRoot();
	}
}



void ObjectTreeWidget::ReviewAsset(void)
{
	ObjectSceneItem* it = static_cast<ObjectSceneItem*>(itemAt(cursorPoint));
	
	if(it)
	{ 
		ObjectCreator* dialog = new ObjectCreator(this);
		dialog->ReviewObject(it->GetObjectPtr());

		dialog->exec();
	}
}


void ObjectTreeWidget::NewAsset(void)
{
	QString firstName = "object";

	QString secondName;
	int count = 1;

	do
	{ 
		secondName = QString::number(count);
		++count;
	}
	while(AssetLibrary::_GetAssets().FindObject((firstName + secondName).toStdString()));
 


	ObjectCreator* dialog = new ObjectCreator(this);
	dialog->SetFastCreating(true);

	dialog->exec();
}



void ObjectTreeWidget::mouseReleaseEvent(QMouseEvent* _event)
{
	cursorPoint = _event->pos();
		
	if(dropItem)
	{
		QFont font("Verdana", 8, QFont::Normal, false);
		dropItem->setFont(NAME_PROPERTY, font);
		dropItem = NIL;
	}

	QTreeWidget::mouseReleaseEvent(_event);
}

 

void ObjectTreeWidget::mouseDoubleClickEvent(QMouseEvent* _event)
{
	{
		{
		} 
	}
}




void ObjectTreeWidget::mouseMoveEvent(QMouseEvent* _event)
{
	cursorPoint = _event->pos();

	ObjectSceneItem* it = static_cast<ObjectSceneItem*>(itemAt(cursorPoint));
	
	if(dropItem)
	{
		if(it)
		{
			if(it != dropItem)
			{
				QFont font("Verdana", 8, QFont::Normal, false);
				dropItem->setFont(NAME_PROPERTY, font);
				dropItem = NIL;
			}
		}
		else
		{
			QFont font("Verdana", 8, QFont::Normal, false);
			dropItem->setFont(NAME_PROPERTY, font);
			dropItem = NIL;
		}
	}
	
	QTreeWidget::mouseMoveEvent(_event);
}



void ObjectTreeWidget::mousePressEvent(QMouseEvent* _event)
{ 
	cursorPoint = _event->pos();

	if(_event->button() == Qt::LeftButton)
	{
		QTreeWidget::mousePressEvent(_event);
	}
	else if(_event->button() == Qt::RightButton)
	{
		QTreeWidget::mousePressEvent(_event);
	
		UpdateContextMenu();
		QPoint mousePos(QCursor::pos().x(), QCursor::pos().y()); 
		contextMenu->move(mousePos);
		contextMenu->show();
	}
}





void ObjectTreeWidget::UpdateContextMenu(void)
{
	QTreeWidgetItem* it = itemAt(cursorPoint);
	
	contextMenu->removeAction(saveLayersStructureAct);
	contextMenu->removeAction(saveAllAssetsAct);
	contextMenu->removeAction(importItemAct); 

	contextMenu->removeAction(activateItemAct);
	contextMenu->removeAction(copyItemAct);
	contextMenu->removeAction(pickItemAct);

	if(it)
	{
		if(selectedItems().indexOf(it) != -1)
		{
			if(it->type() == ObjectSceneItem::SCENE)
			{
				contextMenu->addAction(importItemAct); 
			}

			contextMenu->addAction(destroyItemAct);
			contextMenu->addAction(removeFromSceneAct);
			contextMenu->addAction(renameItemAct);
			contextMenu->addAction(saveToFileItemAct);
			contextMenu->addAction(reviewItemAct);

			ObjectSceneItem* object = dynamic_cast<ObjectSceneItem*>(it);
			
			if(object)
			{
				if(object->GetObjectPtr()->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(FloatLerp::FLOAT_LERP_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(VectorLerp::VECTOR_LERP_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(QuaternionLerp::QUATERNION_LERP_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS))
				{  
					contextMenu->addAction(copyItemAct);
				}

				if(object->GetObjectPtr()->IsClassPropertyExist(Camera3D::CAMERA_3D_CLASS))
				{
					contextMenu->addAction(pickItemAct);

					if(Camera3D::_GetPicked() == object->GetObjectPtr())
					{
						pickItemAct->setChecked(true);
					}
					else
					{
						pickItemAct->setChecked(false);
					}
				}  
				else if(object->GetObjectPtr()->IsClassPropertyExist(Scene::SCENE_CLASS))
				{
					contextMenu->addAction(pickItemAct);

					if(Scene::_GetPicked() == object->GetObjectPtr())
					{
						pickItemAct->setChecked(true);
					}
					else
					{
						pickItemAct->setChecked(false);
					}
				}

				if(object->GetObjectPtr()->IsClassPropertyExist(Trigger::TRIGGER_CLASS) ||
				   object->GetObjectPtr()->IsClassPropertyExist(AbstractEvent::EVENT_CLASS))
				{
					contextMenu->addAction(activateItemAct);
				}
			}
			return;
		} 
		else
		{
			if(it == root)
			{
				contextMenu->addAction(importItemAct); 
				contextMenu->addAction(saveAllAssetsAct);
				contextMenu->addAction(saveLayersStructureAct);
			} 
		}
	} 
	else
	{
		contextMenu->addAction(importItemAct); 
	}
	contextMenu->removeAction(destroyItemAct);
	contextMenu->removeAction(removeFromSceneAct);
	contextMenu->removeAction(renameItemAct);
	contextMenu->removeAction(saveToFileItemAct);
	contextMenu->removeAction(reviewItemAct);
}
  


void ObjectTreeWidget::keyPressEvent(QKeyEvent* _event)
{
	if(!_event->isAutoRepeat())
	{
		if(_event->key() == Qt::Key_Delete)
		{
			DestroySelectedItems();
		}
		else if(_event->key() == Qt::Key_Alt)
		{
			altPressed = true;
		}
		{
		}
	}
	if(_event->key() == Qt::Key_Left)
	{}
	else if(_event->key() == Qt::Key_Right)
	{}
	else
	{
		QTreeWidget::keyPressEvent(_event);
	}
}



void ObjectTreeWidget::keyReleaseEvent(QKeyEvent* _event)
{  
	if(_event->key() == Qt::Key_Alt)
	{
		altPressed = false;
	}

	QTreeWidget::keyReleaseEvent(_event);
}
 


QPoint ObjectTreeWidget::GetCursorPoint(void)const
{
	return cursorPoint;
}




void ObjectTreeWidget::ObjectIsCreated(AbstractObject* _object)
{
	if(FindItem(_object->GetName().c_str()) == NIL)
	{
		ObjectSceneItem::Type assetType = ObjectSceneItem::OBJECT;


		if(_object->IsClassPropertyExist(TexturedAnimation::TEXTURED_ANIMATION_CLASS))
		{
			assetType = ObjectSceneItem::TEXTURED_ANIMATION;
		}
		else if(_object->IsClassPropertyExist(Atlas2D::ATLAS_2D_CLASS))
		{
			assetType = ObjectSceneItem::ATLAS;
		}
		else if(_object->IsClassPropertyExist(Texture2D::TEXTURE_2D_CLASS))
		{
			assetType = ObjectSceneItem::TEXTURE;
		}
		else if(_object->IsClassPropertyExist(Trigger::TRIGGER_CLASS))
		{
			assetType = ObjectSceneItem::TRIGGER;
		}
		else if(_object->IsClassPropertyExist(AbstractEvent::EVENT_CLASS))
		{
			assetType = ObjectSceneItem::EVENT;
		}
		else if(_object->IsClassPropertyExist(AbstractAction::ACTION_CLASS))
		{
			assetType = ObjectSceneItem::ACTION;
		}
		else if(_object->IsClassPropertyExist(AbstractVariable::ABSTRACT_VARIABLE_CLASS))
		{
			assetType = ObjectSceneItem::VARIABLE;
		}
		else if(_object->IsClassPropertyExist(AbstractList::ABSTRACT_LIST_CLASS))
		{
			assetType = ObjectSceneItem::LIST;
		}
		else if(_object->IsClassPropertyExist(Mesh::MESH_CLASS))
		{
			assetType = ObjectSceneItem::MESH;
		}
		else if(_object->IsClassPropertyExist(Timer::TIMER_CLASS))
		{
			assetType = ObjectSceneItem::TIMER;
		}
		else if(_object->IsClassPropertyExist(Hitbox::HITBOX_CLASS))
		{
			assetType = ObjectSceneItem::HITBOX;
		}
		else if(_object->IsClassPropertyExist(PlayerControl::PLAYER_CONTROL_CLASS))
		{
			assetType = ObjectSceneItem::PLAYER_CONTROL;
		}
		else if(_object->IsClassPropertyExist(ObjectScene::OBJECT_SCENE_CLASS))
		{
			assetType = ObjectSceneItem::SCENE;
		}
		else if(_object->IsClassPropertyExist(FloatLerp::FLOAT_LERP_CLASS))
		{
			assetType = ObjectSceneItem::FLOAT_LERP;
		}
		else if(_object->IsClassPropertyExist(QuaternionLerp::QUATERNION_LERP_CLASS))
		{
			assetType = ObjectSceneItem::QUATERNION_LERP;
		}
		else if(_object->IsClassPropertyExist(VectorLerp::VECTOR_LERP_CLASS))
		{
			assetType = ObjectSceneItem::VECTOR_LERP;
		}
		else if(_object->IsClassPropertyExist(Camera3D::CAMERA_3D_CLASS))
		{
			assetType = ObjectSceneItem::CAMERA;
		} 
		

		ObjectSceneItem* parent = NIL;
		ObjectSceneItem* item = AddAsset(_object->GetName().c_str(), assetType, _object->GetClassProperties(), _object, parent);
		 
		if(autoParentEnable)
		{
			parent = dynamic_cast<ObjectSceneItem*>(currentItem());

			if(parent && parent != root)
			{
				if(parent->GetObjectPtr()->IsClassPropertyExist(ObjectScene::OBJECT_SCENE_CLASS))
				{
					ObjectScene* scene = dynamic_cast<ObjectScene*>(dynamic_cast<ObjectSceneItem*>(parent)->GetObjectPtr());

					if(scene)
					{
						scene->AddToLayer(item->GetObjectPtr());
					}
				}
			}
		}
	}
}



void ObjectTreeWidget::ExpandRoot(void)
{
	expandItem(root);
}


void ObjectTreeWidget::CreateRoot(void)
{
	ObjectSceneItem* it = new ObjectSceneItem(ObjectSceneItem::SCENE);
	addTopLevelItem(it);

	it->treeWidget = this;

	it->setFlags(Qt::ItemIsEnabled);
	it->setIcon(ICON_PROPERTY, GetIcon(ObjectSceneItem::SCENE));
	it->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
	it->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
	QFont font("Verdana", 8, QFont::Normal, false);
	it->setFont(NAME_PROPERTY, font);
	it->setFont(TYPE_PROPERTY, font);

	it->setText(NAME_PROPERTY, "ROOT");
	it->setText(TYPE_PROPERTY, GetStringOfAssetType(ObjectSceneItem::SCENE));

	root = it;
}



void ObjectTreeWidget::RemoveFromSceneSelectedItems(void)
{
	QList<QTreeWidgetItem*> list = selectedItems();
	for(int i = 0; i < list.size(); i++) 
	{
		ObjectSceneItem* item = dynamic_cast<ObjectSceneItem*>(list.at(i));
		QTreeWidgetItem* parent = item->parent();

		if(parent)
		{
			AbstractObject* object = dynamic_cast<ObjectSceneItem*>(item)->GetObjectPtr();
			
			ObjectScene* scene = dynamic_cast<ObjectScene*>(dynamic_cast<ObjectSceneItem*>(parent)->GetObjectPtr());

			if(scene)
			{
				scene->GetObjectTree().Remove(object);
			}
		}
	}
}




ObjectSceneItem* ObjectTreeWidget::AddAsset(QString _name, ObjectSceneItem::Type _assetType, int64 _objectClass, AbstractObject* _object, ObjectSceneItem* _parent)
{ 
	ObjectSceneItem* item;

	if(_parent == NIL)
	{
		item = new ObjectSceneItem(_assetType);
		root->addChild(item);
		item->SetObjectClass(_objectClass);
	}
	else 
	{
		item = new ObjectSceneItem(_assetType);
		_parent->addChild(item);
		item->SetObjectClass(_objectClass); 
	}

	item->treeWidget = this;
	item->SetObjectPtr(_object);

	if(_assetType == ObjectSceneItem::SCENE)
	{ 
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

		_object->Connect(ObjectScene::ADD_TO_LAYER_MESSAGE, item, Caller<>(item, &ObjectSceneItem::AddToObjectScene));
		_object->Connect(ObjectScene::REMOVE_FROM_LAYER_MESSAGE, item, Caller<>(item, &ObjectSceneItem::RemoveFromObjectScene));
	} 
	else
	{
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
	}

	item->setIcon(ICON_PROPERTY, GetIcon(_assetType));
	item->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
	item->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
	QFont font("Verdana", 8, QFont::Normal, false);
	item->setFont(NAME_PROPERTY, font);
	item->setFont(TYPE_PROPERTY, font);

	item->setText(NAME_PROPERTY, _name);
	item->setText(TYPE_PROPERTY, GetStringOfAssetType(_assetType));

	if(_assetType == ObjectSceneItem::SCENE)
	{ 

		if(_object->IsClassPropertyExist(ObjectScene::OBJECT_SCENE_CLASS))
		{
			ObjectScene* objectScene = dynamic_cast<ObjectScene*>(_object);

			if(objectScene->GetObjectTree().IsNotEmpty())
			{ 
				autoParentEnable = false;
				MagicList<AbstractObject> objectList = objectScene->GetObjectTree().ToList();

				for(MagicList<AbstractObject>::AddressDerived* it = objectList.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					item->AddToObjectScene(it->GetNode()); 
				} 
				autoParentEnable = true;
			}
		}
	}

	_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ObjectTreeWidget::ObjectIsRemoved));
	_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ObjectTreeWidget::ObjectIsRenamed));
	return item;  
}



ObjectSceneItem* ObjectTreeWidget::FindItem(QString _name, QString _parent)
{
	return FindItem(root, _name, _parent);
}



ObjectSceneItem* ObjectTreeWidget::FindItem(QTreeWidgetItem* _it, QString _name, QString _parent)
{
	if(_it)
	{
		if(_it != root)
		{
			ObjectSceneItem* item = dynamic_cast<ObjectSceneItem*>(_it);

			if(item->text(NAME_PROPERTY) == _name && item->parent()->text(NAME_PROPERTY) == _parent)
			{
				return item;
			}
		}
		
		ObjectSceneItem* search;
		int child = 0;

		while(_it->child(child))
		{
			if(search = FindItem(_it->child(child), _name, _parent))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}



ObjectSceneItem* ObjectTreeWidget::FindItem(QString _name)
{
	return FindItem(root, _name);
}



ObjectSceneItem* ObjectTreeWidget::FindItem(QTreeWidgetItem* _it, QString _name)
{
	if(_it)
	{
		if(_it != root)
		{
			ObjectSceneItem* item = dynamic_cast<ObjectSceneItem*>(_it);

			if(item->text(NAME_PROPERTY) == _name)
			{
				return item;
			}
		}
		
		ObjectSceneItem* search;
		int child = 0;

		while(_it->child(child))
		{
			if(search = FindItem(_it->child(child), _name))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}


QString ObjectTreeWidget::GetStringOfAssetType(ObjectSceneItem::Type _assetType)const
{
	switch(_assetType)
	{
		case ObjectSceneItem::OBJECT:
		{
			return "OBJECT";
		}

		case ObjectSceneItem::TEXTURED_ANIMATION:
		{
			return "TEXTURED_ANIMATION";
		}
		
		case ObjectSceneItem::ATLAS:
		{
			return "ATLAS";
		}

		case ObjectSceneItem::TEXTURE:
		{
			return "TEXTURE";
		}

		case ObjectSceneItem::TRIGGER:
		{
			return "TRIGGER";
		}

		case ObjectSceneItem::EVENT:
		{
			return "EVENT";
		}

		case ObjectSceneItem::ACTION:
		{
			return "ACTION";
		}

		case ObjectSceneItem::VARIABLE:
		{
			return "VARIABLE";
		}
			
		case ObjectSceneItem::LIST:
		{
			return "LIST";
		}

		case ObjectSceneItem::MESH:
		{
			return "MESH";
		}

		case ObjectSceneItem::TIMER:
		{
			return "TIMER";
		}
		 
		case ObjectSceneItem::HITBOX:
		{
			return "HITBOX";
		}

		case ObjectSceneItem::PLAYER_CONTROL:
		{
			return "PLAYER_CONTROL";
		}

		case ObjectSceneItem::SCENE:
		{
			return "SCENE";
		}

		case ObjectSceneItem::FLOAT_LERP:
		{
			return "FLOAT_LERP";
		}

		case ObjectSceneItem::QUATERNION_LERP:
		{
			return "QUATERNION_LERP";
		}

		case ObjectSceneItem::VECTOR_LERP:
		{
			return "VECTOR_LERP";
		} 

		case ObjectSceneItem::CAMERA:
		{
			return "CAMERA";
		}  
	}
			
	return "";
}



QPixmap ObjectTreeWidget::GetIcon(int _assetType)
{
	switch(_assetType)
	{
		case ObjectSceneItem::ATLAS:
		{
			return atlasIcon;
		}

		case ObjectSceneItem::TEXTURE:
		{
			return textureIcon;
		}

		case ObjectSceneItem::TEXTURED_ANIMATION:
		{
			return texturedAnimationIcon;
		}

		case ObjectSceneItem::TRIGGER:
		{
			return triggerIcon;
		}

		case ObjectSceneItem::EVENT:
		{
			return eventIcon;
		}

		case ObjectSceneItem::ACTION:
		{
			return actionIcon;
		}

		case ObjectSceneItem::VARIABLE:
		{
			return variableIcon;
		}

		case ObjectSceneItem::LIST:
		{
			return listIcon;
		}

		case ObjectSceneItem::PLAYER_CONTROL:
		{
			return playerControlIcon;
		}

		case ObjectSceneItem::MESH:
		{
			return meshIcon;
		}

		case ObjectSceneItem::TIMER:
		{
			return timerIcon;
		}

		case ObjectSceneItem::HITBOX:
		{
			return hitboxIcon;
		}

		case ObjectSceneItem::SCENE:
		{
			return sceneIcon;
		}

		case ObjectSceneItem::FLOAT_LERP:
		{
			return floatLerpIcon;
		}

		case ObjectSceneItem::QUATERNION_LERP:
		{
			return quaternionLerpIcon;
		}

		case ObjectSceneItem::VECTOR_LERP:
		{
			return vectorLerpIcon;
		}

		case ObjectSceneItem::CAMERA:
		{
			return cameraIcon;
		}  
	} 
	
	return objectIcon;
}




void ObjectTreeWidget::ObjectIsRemoved(AbstractObject* _object)
{
	ObjectSceneItem* it;
	
	while(it = FindItem(_object->GetName().c_str()))
	{
		if(it->isSelected())
		{
			it->setSelected(false);
		}
		delete it;
	}
}




void ObjectTreeWidget::ObjectIsRenamed(AbstractObject::NameInfo* _info)
{
	ObjectSceneItem* it;
	
	while(it = FindItem(_info->oldName.c_str()))
	{
		if(it->isSelected())
		{
			it->setSelected(false);
		}
		delete it;
	}
}




void ObjectTreeWidget::timerEvent(QTimerEvent* _event) 
{
	if(_event->timerId() == scrollTimerId)
	{
		Scroll();
	}
	QTreeWidget::timerEvent(_event);
}

	
void ObjectTreeWidget::Scroll(void) 
{
	QModelIndex indexA = indexAt(mapFromGlobal(QCursor::pos()));
	QModelIndex indexB = indexBelow(indexA);
	QRect vRect = visualRect(indexB);
	
	if(height() <= vRect.y() + (vRect.height()))
	{
		scrollTo(indexB);
	}
	else if( y() >= visualRect(indexAbove(indexA)).y())
	{
		scrollTo(indexAbove(indexA));
	}
	else 
	{
		killTimer(scrollTimerId);
		scrollTimerId = 0;
	}
}


void ObjectTreeWidget::dragMoveEvent(QDragMoveEvent* _event)
{
	if(!scrollTimerId && hasAutoScroll())
	{
		scrollTimerId = startTimer(100);
	}

	QModelIndex index = indexAt(_event->pos());

	QTreeWidgetItem* it = itemFromIndex(index);

	int dropIndicatorPos = dropIndicatorPosition();

	
	if(it)
	{
		if(it->type() == ObjectSceneItem::SCENE && dropIndicatorPos == QAbstractItemView::OnItem)
		{
			_event->setDropAction(Qt::MoveAction);
			_event->accept();

			if(dropItem && it != dropItem)
			{
				QFont font("Verdana", 8, QFont::Normal, false);
				dropItem->setFont(NAME_PROPERTY, font);
			}
			QFont font("Verdana", 12, QFont::Bold, false);
			it->setFont(NAME_PROPERTY, font);
			dropItem = it;
		}
		else
		{
			_event->ignore();
		}
		
		if(dropItem && it != dropItem)
		{
			QFont font("Verdana", 8, QFont::Normal, false);
			dropItem->setFont(NAME_PROPERTY, font);
		}
	}
}



void ObjectTreeWidget::dropEvent(QDropEvent* _event)
{
	if(_event->mimeData()->hasUrls())
	{
		QList<QUrl> urlList;
		QString fileName;
		QFileInfo info;

		urlList = _event->mimeData()->urls();
		
		for(int i = 0; i < urlList.size(); ++i)
		{
			fileName = urlList[i].toLocalFile();
				
			info.setFile(fileName);

			if(info.isFile())
			{
				bool state = Trigger::_IsBlockEnabled();
				Trigger::_SetBlockEnable(false);

				fileName = info.completeBaseName();
				StringANSI suffix = File::_GetSuffix(fileName.toStdString());
				AssetLibrary::_LoadCommonAsset(File::_GetBaseName(fileName.toStdString()), AssetLibrary::_GetAssetTypeOfFileSuffix(info.suffix().toStdString()));
				
				Trigger::_SetBlockEnable(state);
				_event->acceptProposedAction();
			}
		}
	}
	if(_event->dropAction() != Qt::IgnoreAction)
	{
		QModelIndex index = indexAt(_event->pos());
		QTreeWidgetItem* it = dropItem;
		if(it && it->type() == ObjectSceneItem::SCENE)
		{ 
			QList<QTreeWidgetItem*> selectedList = selectedItems();
			for(int i = 0; i < selectedList.size(); ++i)
			{
				if(!altPressed)
				{
					QTreeWidgetItem* parent = selectedList.at(i)->parent();

					if(selectedList.at(i) != dropItem)
					{
						if(parent && parent != root)
						{
							if(it != parent)
							{
								AbstractObject* object = dynamic_cast<ObjectSceneItem*>(selectedList.at(i))->GetObjectPtr();
									
								ObjectScene* scene = dynamic_cast<ObjectScene*>(dynamic_cast<ObjectSceneItem*>(parent)->GetObjectPtr());

								if(scene)
								{
									scene->GetObjectTree().Remove(object);
								}
							}
						}
					}
				}
			}
			
			selectedList = selectedItems();
			for(int i = 0; i < selectedList.size(); ++i)
			{
				AbstractObject* object = dynamic_cast<ObjectSceneItem*>(selectedList.at(i))->GetObjectPtr();

				ObjectScene* scene = dynamic_cast<ObjectScene*>(dynamic_cast<ObjectSceneItem*>(it)->GetObjectPtr());

				if(scene)
				{
					scene->AddToLayer(object);
				}
			}
		}
	}
	_event->setDropAction(Qt::IgnoreAction);
}




void ObjectTreeWidget::dragEnterEvent(QDragEnterEvent* _event)
{
	if(_event->mimeData()->hasFormat("text/uri-list")) 
	{ 	
		QList<QUrl> urlList;
		QString fileName;
		QFileInfo info;

		urlList = _event->mimeData()->urls();
		
		for(int i = 0; i < urlList.size(); ++i)
		{
			fileName = urlList[i].toLocalFile();
				
			info.setFile(fileName);

			if(info.isFile())
			{
				if(info.suffix() == AbstractAction::_GetFileSuffix().c_str() ||
				   info.suffix() == AbstractEvent::_GetFileSuffix().c_str() ||
				   info.suffix() == Trigger::_GetFileSuffix().c_str() ||

				   info.suffix() == AbstractVariable::_GetFileSuffix().c_str() ||
				   info.suffix() == AbstractList::_GetFileSuffix().c_str() ||

				   info.suffix() == Mesh::_GetFileSuffix().c_str() ||
				   info.suffix() == Hitbox::_GetFileSuffix().c_str() ||

				   info.suffix() == AbstractObject::_GetFileSuffix().c_str() ||
				   info.suffix() == ObjectScene::_GetFileSuffix().c_str() ||

				   info.suffix() == PlayerControl::_GetFileSuffix().c_str() ||
				   info.suffix() == Timer::_GetFileSuffix().c_str() ||
				   info.suffix() == FloatLerp::_GetFileSuffix().c_str() ||
				   info.suffix() == QuaternionLerp::_GetFileSuffix().c_str() ||
				   info.suffix() == VectorLerp::_GetFileSuffix().c_str() ||

				   info.suffix() == Texture2D::_GetFileSuffix().c_str() ||
				   info.suffix() == TexturedAnimation::_GetFileSuffix().c_str() ||
				   info.suffix() == Atlas2D::_GetFileSuffix().c_str() ||
				    
				   info.suffix() == Camera3D::_GetFileSuffix().c_str())
				{
					_event->acceptProposedAction();
				}
				else
				{
					_event->setDropAction(Qt::IgnoreAction);
					return;
				}
			}
		}
	}
	else 
	{ 
		QTreeWidget::dragEnterEvent(_event); 
	} 
}
	