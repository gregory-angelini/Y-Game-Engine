#include "LogicTreeWidget.h"
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



LogicTreeWidget::LogicTreeWidget(QWidget* _parent): QTreeWidget(_parent), root(NIL), scrollTimerId(0), ignoreSelection(false), dropItem(NIL), autoParentEnable(true)
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

	sceneIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/scene.png");
	objectIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/object.png");
	floatLerpIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/f-lerp.png");
	timerIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/timer.png");
	texturedAnimationIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/t-anim.png");
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &LogicTreeWidget::ObjectIsCreated));


	CreateRoot();
	show();
}



LogicTreeWidget::~LogicTreeWidget(void)
{}




void LogicTreeWidget::CreateContextMenu(void)
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
}


void LogicTreeWidget::RemoveFromSceneSelectedItems(void)
{
	QList<QTreeWidgetItem*> list = selectedItems();
	for(int i = 0; i < list.size(); i++) 
	{
		LogicSceneItem* item = dynamic_cast<LogicSceneItem*>(list.at(i));
		QTreeWidgetItem* parent = item->parent();

		if(parent)
		{	
			LogicObject* object = dynamic_cast<LogicSceneItem*>(item)->GetObjectPtr();
						
			LogicScene* scene = dynamic_cast<LogicScene*>(dynamic_cast<LogicSceneItem*>(parent)->GetObjectPtr());

			if(scene)
			{
				scene->GetUpdateList().Remove(object);
			}
		}
	}
}


void LogicTreeWidget::PickCurrentItem(void)
{
	LogicSceneItem* it = static_cast<LogicSceneItem*>(itemAt(cursorPoint));
	
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
					camera->Pick();
				}
			}
			else if(object->IsClassPropertyExist(Scene::SCENE_CLASS))
			{
				Scene* scene = dynamic_cast<Scene*>(object);

				if(scene)
				{
					scene->Pick();
				}
			}
		}
	}
}


void LogicTreeWidget::SaveToFileSelectedItems(void)
{
	QList<QTreeWidgetItem*> list = selectedItems();
	for(int i = 0; i < list.size(); i++) 
	{
		LogicSceneItem* item = dynamic_cast<LogicSceneItem*>(list.at(i)); 

		item->GetObjectPtr()->SaveToFile();
	}
}



void LogicTreeWidget::RenameCurrentItem(void)
{
	LogicSceneItem* it = static_cast<LogicSceneItem*>(itemAt(cursorPoint));
	
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



void LogicTreeWidget::ReviewAsset(void)
{
	LogicSceneItem* it = static_cast<LogicSceneItem*>(itemAt(cursorPoint));
	
	if(it)
	{ 
		ObjectCreator* dialog = new ObjectCreator(this);
		dialog->ReviewObject(it->GetObjectPtr());

		dialog->exec();
	}
}


void LogicTreeWidget::ImportAsset(void)
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


void LogicTreeWidget::DestroySelectedItems(void)
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
			LogicSceneItem* item = dynamic_cast<LogicSceneItem*>(it); 

			delete item->GetObjectPtr();
		}
	}
}



LogicSceneItem* LogicTreeWidget::FindLeaf(QTreeWidgetItem* _item)
{
	LogicSceneItem* leaf = NIL;

	if(_item)
	{
		leaf = dynamic_cast<LogicSceneItem*>(_item->child(0));

		if(leaf)
		{
			while(leaf->child(0))
			{
				leaf = dynamic_cast<LogicSceneItem*>(leaf->child(0));
			}
		}
	}
	return leaf;
}


QTreeWidgetItem* LogicTreeWidget::FindLeaf(QList<QTreeWidgetItem*>& _list)
{ 

	LogicSceneItem* leaf = NIL;
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



void LogicTreeWidget::NewAsset(void)
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



void LogicTreeWidget::mouseReleaseEvent(QMouseEvent* _event)
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

 

void LogicTreeWidget::mouseDoubleClickEvent(QMouseEvent* _event)
{
	{
		{
		} 
	}
}




void LogicTreeWidget::mouseMoveEvent(QMouseEvent* _event)
{
	cursorPoint = _event->pos();

	LogicSceneItem* it = static_cast<LogicSceneItem*>(itemAt(cursorPoint));
	
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



void LogicTreeWidget::mousePressEvent(QMouseEvent* _event)
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





void LogicTreeWidget::CopyCurrentItem(void)
{
	LogicSceneItem* it = static_cast<LogicSceneItem*>(itemAt(cursorPoint));
	
	if(it)
	{
		NameCreator dialog;
		dialog.SetAssetLibraryCheckEnable(true);
		dialog.SetName(it->text(NAME_PROPERTY));
		if(dialog.exec() == QDialog::Accepted)
		{
			if(it->type() == LogicSceneItem::FLOAT_LERP)
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
			else if(it->type() == LogicSceneItem::TIMER)
			{
				File file((Editor::_GetWorkDir() + Editor::_GetTempDir() + dialog.GetName() + "." + Timer::_GetFileSuffix().c_str()).toStdString(), File::REWRITE);
					
				if(it->GetObjectPtr()->SaveAsToFile(file, dialog.GetName().toStdString()))
				{
					if(file.Open(File::READ))
					{
						AssetLibrary::_LoadCommonAsset(file, AssetLibrary::TIMER_ASSET);

						file.Remove();
					}
				}
			}
			else if(it->type() == LogicSceneItem::OBJECT)
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


void LogicTreeWidget::UpdateContextMenu(void)
{
	QTreeWidgetItem* it = itemAt(cursorPoint);
	
	contextMenu->removeAction(saveLayersStructureAct);
	contextMenu->removeAction(saveAllAssetsAct);
	contextMenu->removeAction(importItemAct); 

	if(it)
	{
		if(selectedItems().indexOf(it) != -1)
		{
			if(it->type() == LogicSceneItem::SCENE)
			{
				contextMenu->addAction(importItemAct); 
			}

			contextMenu->addAction(destroyItemAct);
			contextMenu->addAction(removeFromSceneAct);
			contextMenu->addAction(renameItemAct);
			contextMenu->addAction(saveToFileItemAct);
			contextMenu->addAction(reviewItemAct);
			destroyItemAct->setEnabled(true);
			removeFromSceneAct->setEnabled(true);
			renameItemAct->setEnabled(true);
			saveToFileItemAct->setEnabled(true);
			reviewItemAct->setEnabled(true);
			

			LogicSceneItem* object = dynamic_cast<LogicSceneItem*>(it);
			
			if(object)
			{
				if(object->GetObjectPtr()->IsClassPropertyExist(FloatLerp::FLOAT_LERP_CLASS))
				{
					contextMenu->addAction(copyItemAct);
					copyItemAct->setEnabled(true);
				}
				else if(object->GetObjectPtr()->IsClassPropertyExist(Timer::TIMER_CLASS))
				{
					contextMenu->addAction(copyItemAct);
					copyItemAct->setEnabled(true);
				}
				else if(object->GetObjectPtr()->IsClassPropertyExist(Scene::SCENE_CLASS))
				{
					contextMenu->addAction(pickItemAct);
					pickItemAct->setEnabled(true);

					if(Scene::_GetPicked() == object->GetObjectPtr())
					{
						pickItemAct->setChecked(true);
					}
					else
					{
						pickItemAct->setChecked(false);
					}
				}
				else
				{
					contextMenu->removeAction(pickItemAct);
					pickItemAct->setEnabled(false);
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
	contextMenu->removeAction(saveToFileItemAct);


	destroyItemAct->setEnabled(false);
	removeFromSceneAct->setEnabled(false);
	renameItemAct->setEnabled(false);
	saveToFileItemAct->setEnabled(false);
	reviewItemAct->setEnabled(false);

	copyItemAct->setEnabled(false);
	pickItemAct->setEnabled(false);
}
 



void LogicTreeWidget::SaveAllAssets(void)
{
	SaveAllAssets(root);
}



void LogicTreeWidget::SaveAllAssets(QTreeWidgetItem* _it)
{
	if(_it)
	{
		if(_it != root)
		{
			LogicSceneItem* item = dynamic_cast<LogicSceneItem*>(_it);

			item->GetObjectPtr()->SaveToFile();
		}
		
		LogicSceneItem* search;
		int child = 0;

		while(_it->child(child))
		{
			SaveAllAssets(_it->child(child));

			child++;
		}
	}
}



void LogicTreeWidget::SaveLayersStructure(void)
{
	SaveLayersStructure(root);
}



void LogicTreeWidget::SaveLayersStructure(QTreeWidgetItem* _it)
{
	if(_it)
	{
		if(_it != root)
		{
			if(_it->type() == LogicSceneItem::SCENE)
			{
				LogicSceneItem* item = dynamic_cast<LogicSceneItem*>(_it);

				item->GetObjectPtr()->SaveToFile();
			}
		}
		
		LogicSceneItem* search;
		int child = 0;

		while(_it->child(child))
		{
			SaveLayersStructure(_it->child(child));

			child++;
		}
	}
}


void LogicTreeWidget::keyPressEvent(QKeyEvent* _event)
{
	if(!_event->isAutoRepeat())
	{
		if(_event->key() == Qt::Key_Delete)
		{
			DestroySelectedItems();
		}
		else if(_event->key() == Qt::Key_Return)
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



void LogicTreeWidget::keyReleaseEvent(QKeyEvent* _event)
{  
	QTreeWidget::keyReleaseEvent(_event);
}
 


QPoint LogicTreeWidget::GetCursorPoint(void)const
{
	return cursorPoint;
}




void LogicTreeWidget::ObjectIsCreated(AbstractObject* _object)
{
	if(FindItem(_object->GetName().c_str()) == NIL)
	{
		if(_object->IsClassPropertyExist(LogicObject::LOGIC_OBJECT_CLASS))
		{
			LogicSceneItem* parent = NIL;
			LogicSceneItem::Type assetType = GetAssetType(_object);
			LogicSceneItem* item = AddAsset(_object->GetName().c_str(), assetType, _object->GetClassProperties(), dynamic_cast<LogicObject*>(_object), parent);
		
			if(autoParentEnable)
			{
				parent = dynamic_cast<LogicSceneItem*>(currentItem());

				if(parent && parent != root)
				{
					if(parent->GetObjectPtr()->IsClassPropertyExist(LogicScene::LOGIC_SCENE_CLASS))
					{
						LogicScene* scene = dynamic_cast<LogicScene*>(dynamic_cast<LogicSceneItem*>(parent)->GetObjectPtr());

						if(scene)
						{
							scene->AddToUpdateList(item->GetObjectPtr());
						}
					}
				}
			}

			_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &LogicTreeWidget::ObjectIsRemoved));
			_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &LogicTreeWidget::ObjectIsRenamed));
		}
	}
}



LogicSceneItem::Type LogicTreeWidget::GetAssetType(AbstractObject* _object)
{
	if(_object)
	{
		if(_object->IsClassPropertyExist(LogicScene::LOGIC_SCENE_CLASS))
		{
			return LogicSceneItem::SCENE;
		}
		else if(_object->IsClassPropertyExist(TexturedAnimation::TEXTURED_ANIMATION_CLASS))
		{
			return LogicSceneItem::TEXTURED_ANIMATION;
		}
		else if(_object->IsClassPropertyExist(FloatLerp::FLOAT_LERP_CLASS))
		{
			return LogicSceneItem::FLOAT_LERP;
		}
		else if(_object->IsClassPropertyExist(Timer::TIMER_CLASS))
		{
			return LogicSceneItem::TIMER;
		}
	}

	return LogicSceneItem::OBJECT; 
}


void LogicTreeWidget::ExpandRoot(void)
{
	expandItem(root);
}


void LogicTreeWidget::CreateRoot(void)
{
	LogicSceneItem* it = new LogicSceneItem(invisibleRootItem(), LogicSceneItem::SCENE, 0);
	it->treeWidget = this;

	it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsDropEnabled);

	it->setIcon(ICON_PROPERTY, GetIcon(LogicSceneItem::SCENE));
	it->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
	it->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
	QFont font("Verdana", 8, QFont::Normal, false);
	it->setFont(NAME_PROPERTY, font);
	it->setFont(TYPE_PROPERTY, font);

	it->setText(NAME_PROPERTY, "ROOT");
	it->setText(TYPE_PROPERTY, GetStringOfAssetType(LogicSceneItem::SCENE));

	root = it;
}




LogicSceneItem* LogicTreeWidget::AddAsset(QString _name, LogicSceneItem::Type _assetType, int64 _objectClass, LogicObject* _object, LogicSceneItem* _parent)
{
	LogicSceneItem* item = FindItem(_name);

	if(item == NIL)
	{
		if(_parent == NIL)
		{
			item = new LogicSceneItem(root, _assetType, _objectClass);
		}
		else 
		{
			item = new LogicSceneItem(root, _assetType, _objectClass);
		}


		item->treeWidget = this;
		item->SetObjectPtr(_object);

		if(_assetType == LogicSceneItem::SCENE)
		{
			item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

			_object->Connect(LogicScene::ADD_TO_UPDATE_LIST_MESSAGE, item, Caller<>(item, &LogicSceneItem::AddToLogicScene));
			_object->Connect(LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE, item, Caller<>(item, &LogicSceneItem::RemoveFromLogicScene));
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

		if(_assetType == LogicSceneItem::SCENE)
		{ 

			if(_object->IsClassPropertyExist(LogicScene::LOGIC_SCENE_CLASS))
			{
				LogicScene* logicScene = dynamic_cast<LogicScene*>(_object);

				if(logicScene->GetUpdateList().IsNotEmpty())
				{ 
					autoParentEnable = false;
					MagicList<LogicObject> objectList = logicScene->GetUpdateList();

					for(MagicList<LogicObject>::AddressDerived* it = objectList.GetHeadAddress(); it != NIL; it = it->GetRight())
					{
						item->AddToLogicScene(it->GetNode());
					}
					autoParentEnable = true;
				}
			}
		}
	}
	return item;
}




LogicSceneItem* LogicTreeWidget::FindItem(QString _name, QString _parent)
{
	return FindItem(root, _name, _parent);
}



LogicSceneItem* LogicTreeWidget::FindItem(QTreeWidgetItem* _it, QString _name, QString _parent)
{
	if(_it)
	{
		if(_it != root)
		{
			LogicSceneItem* item = dynamic_cast<LogicSceneItem*>(_it);

			if(item->text(NAME_PROPERTY) == _name && item->parent()->text(NAME_PROPERTY) == _parent)
			{
				return item;
			}
		}
		
		LogicSceneItem* search;
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



LogicSceneItem* LogicTreeWidget::FindItem(QString _name)
{
	return FindItem(root, _name);
}



LogicSceneItem* LogicTreeWidget::FindItem(QTreeWidgetItem* _it, QString _name)
{
	if(_it)
	{
		if(_it != root)
		{
			LogicSceneItem* item = dynamic_cast<LogicSceneItem*>(_it);

			if(item->text(NAME_PROPERTY) == _name)
			{
				return item;
			}
		}
		
		LogicSceneItem* search;
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



QString LogicTreeWidget::GetStringOfAssetType(LogicSceneItem::Type _assetType)const
{
	switch(_assetType)
	{
		case LogicSceneItem::OBJECT:
		{
			return "OBJECT";
		}

		case LogicSceneItem::SCENE:
		{
			return "SCENE";
		}

		case LogicSceneItem::FLOAT_LERP:
		{
			return "FLOAT_LERP";
		}

		case LogicSceneItem::TIMER:
		{
			return "TIMER";
		}

		case LogicSceneItem::TEXTURED_ANIMATION:
		{
			return "TEXTURED_ANIMATION";
		}
	}
			
	return "";
}



QPixmap LogicTreeWidget::GetIcon(int _assetType)
{
	switch(_assetType)
	{
		case LogicSceneItem::SCENE:
		{
			return sceneIcon;
		}

		case LogicSceneItem::OBJECT:
		{
			return objectIcon;
		}

		case LogicSceneItem::FLOAT_LERP:
		{
			return floatLerpIcon;
		}

		case LogicSceneItem::TIMER:
		{
			return timerIcon;
		}

		case LogicSceneItem::TEXTURED_ANIMATION:
		{
			return texturedAnimationIcon;
		}
	}
	

	return QPixmap();
}




void LogicTreeWidget::ObjectIsRemoved(AbstractObject* _object)
{
	LogicSceneItem* item;
	
	while(item = FindItem(_object->GetName().c_str()))
	{
		if(item->isSelected())
		{
			item->setSelected(false);
		}

		if(dropItem && item == dropItem)
		{
			dropItem = NIL;
		}
		delete item;
	}
}



void LogicTreeWidget::ObjectIsRenamed(AbstractObject::NameInfo* _info)
{
	LogicSceneItem* it;
	
	while(it = FindItem(_info->oldName.c_str()))
	{
		if(it->isSelected())
		{
			it->setSelected(false);
		}
		delete it;
	}
}





void LogicTreeWidget::timerEvent(QTimerEvent* _event) 
{
	if(_event->timerId() == scrollTimerId)
	{
		Scroll();
	}
	QTreeWidget::timerEvent(_event);
}


	
void LogicTreeWidget::Scroll(void) 
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


void LogicTreeWidget::dragMoveEvent(QDragMoveEvent* _event)
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
		if(it->type() == LogicSceneItem::SCENE && dropIndicatorPos == QAbstractItemView::OnItem)
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



void LogicTreeWidget::dropEvent(QDropEvent* _event)
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
		if(it && it->type() == LogicSceneItem::SCENE)
		{	
			QList<QTreeWidgetItem*> selectedList = selectedItems();
			for(int i = 0; i < selectedList.size(); ++i)
			{
				if(it == root)
				{
					QTreeWidgetItem* parent = selectedList.at(i)->parent();

					if(parent)
					{
						LogicObject* object = dynamic_cast<LogicSceneItem*>(selectedList.at(i))->GetObjectPtr();
						
						LogicScene* scene = dynamic_cast<LogicScene*>(dynamic_cast<LogicSceneItem*>(parent)->GetObjectPtr());

						if(scene)
						{
							scene->GetUpdateList().Remove(object);
						}
					}
				}
				else
				{
					LogicObject* object = dynamic_cast<LogicSceneItem*>(selectedList.at(i))->GetObjectPtr();

					LogicScene* scene = dynamic_cast<LogicScene*>(dynamic_cast<LogicSceneItem*>(it)->GetObjectPtr());

					if(scene)
					{
						scene->AddToUpdateList(object);
					}
				}
			}
		}
	}
	_event->setDropAction(Qt::IgnoreAction);
}




void LogicTreeWidget::dragEnterEvent(QDragEnterEvent* _event)
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