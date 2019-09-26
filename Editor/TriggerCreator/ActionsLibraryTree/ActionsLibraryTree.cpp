#include "ActionsLibraryTree.h"
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
#include <Engine/Output/Graphics/Shape/Line/GraphicsStrip/GraphicsStrip.h>
#include <Engine/Core/Time/Timer/Timer.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>
#include <Engine/Core/Parser/Set/Set.h>



ActionsLibraryTree::ActionsLibraryTree(QWidget* _parent): QTreeWidget(_parent), lastAction(NIL), lastCategory(NIL), lastGroup(NIL)
{
	setSortingEnabled(true);
	selectedBeginColor = QColor(165,165,165);
	categoryBeginColor = QColor(220,220,220);
	groupBeginColor = QColor(220,220,220);
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
	setColumnWidth(NAME_PROPERTY, 210+50+30+47-3-1+20+100-4-5-6);
	setColumnWidth(TYPE_PROPERTY, 40);
	header()->setResizeMode(ICON_PROPERTY, QHeaderView::Fixed);
	header()->setResizeMode(TYPE_PROPERTY, QHeaderView::Fixed);
	CreateContextMenu();
	setStyleSheet("QTreeView::item { height: 16px; }"
		          "QTreeWidget::item:!selected { border: 2px solid gainsboro; "
		                                        "border-left: none; border-top: none; }"
		          "QTreeWidget::item:selected { background-color: rgb(149,199,233);"
                                               "border: 2px solid gainsboro;"
											   "border-left: none; border-top: none; }"); 
		
	folderIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/folder.png");
	actionIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/action.png");

	Refresh();
}



ActionsLibraryTree::~ActionsLibraryTree(void)
{}



void ActionsLibraryTree::UpdateContextMenu(void)
{
	QTreeWidgetItem* it = itemAt(cursorPoint);
	contextMenu->removeAction(editActionAct);  
	contextMenu->removeAction(removeItemAct);

	contextMenu->removeAction(refreshAct);
	

	if(it)
	{
		if(it->type() == Item::ACTION_TYPE)
		{
			contextMenu->addAction(editActionAct);
			contextMenu->addAction(removeItemAct);
	
		}
		else if(it->type() == Item::GROUP_TYPE)
		{
			contextMenu->addAction(removeItemAct);
			contextMenu->addAction(refreshAct);
		}
		else if(it->type() == Item::CATEGORY_TYPE)
		{
			contextMenu->addAction(removeItemAct);
			contextMenu->addAction(refreshAct);
		}
	}
	else
	{
		contextMenu->addAction(refreshAct);
	}
}


void ActionsLibraryTree::CreateContextMenu(void)
{
	contextMenu = new QMenu(this);
	editActionAct = new QAction("Edit", contextMenu);
    connect(editActionAct, SIGNAL(triggered()), SLOT(EditAction()));
	removeItemAct = new QAction("Remove", contextMenu);
	connect(removeItemAct, SIGNAL(triggered()), SLOT(RemoveSelectedActions()));
	refreshAct = new QAction("Refresh", contextMenu);
	connect(refreshAct, SIGNAL(triggered()), SLOT(RefreshActionTree()));
}



void ActionsLibraryTree::RemoveSelectedActions(void)
{
	WarningDialog warning("Warning", "Selected actions will be deleted. Apply?", 300, 90);
		
	if(warning.exec() == QDialog::Rejected)
	{
		return;
	}
	while(!selectedItems().isEmpty())
	{
		Item* it = dynamic_cast<Item*>(FindLeaf(selectedItems()));
		
		if(it)
		{
			if(it->GetItemType() == Item::ACTION_TYPE)
			{ 
				Item* parent = dynamic_cast<Item*>(it->parent());
					
				QString name = it->GetName();
				
				File::_Remove(AssetLibrary::_GetDir() + AbstractAction::_GetDir() + name.toAscii().data() + "." + AbstractAction::_GetFileSuffix());
			
				delete it;

				emit ActionIsEdited(name);

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
			}
			else
			{
				if(it->GetItemType() == Item::GROUP_TYPE)
				{
					Item* parent = dynamic_cast<Item*>(it->parent());

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
				}
				else if(it->GetItemType() == Item::CATEGORY_TYPE)
				{
					delete it;
				}
			}
		}
	} 
}


void ActionsLibraryTree::EditAction(void)
{
	Item* it = dynamic_cast<Item*>(currentItem());
	
	if(it && it->GetItemType() == Item::ACTION_TYPE)
	{
		ActionCreator* dialog = new ActionCreator();

		dialog->SetAction(it->GetName());

		dialog->setAttribute(Qt::WA_DeleteOnClose);

		dialog->show();

		connect(dialog, SIGNAL(ActionIsEdited(QString)), this, SIGNAL(ActionIsEdited(QString)));
	}
}



QTreeWidgetItem* ActionsLibraryTree::FindLeaf(QList<QTreeWidgetItem*>& _list)
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



void ActionsLibraryTree::RefreshActionTree(void)
{
	Refresh();
}


void ActionsLibraryTree::Refresh(void)
{
	for(int i = 0; i < topLevelItemCount(); i++)
	{
		Item* it = dynamic_cast<Item*>(topLevelItem(i));

		if(it && it->GetItemType() == Item::CATEGORY_TYPE)
		{
			for(int j = 0; j < it->childCount(); j++)
			{
				Item* child = dynamic_cast<Item*>(it->child(j));
				
				if(child && child->GetItemType() == Item::GROUP_TYPE)
				{ 
					child->RemoveChildItems();
				} 
			}
		}
	}
	QString path = QString(AssetLibrary::_GetDir().c_str()) + QString(AbstractAction::_GetDir().c_str());
	
	QDir dir(path);

	QFileInfoList actions = dir.entryInfoList(QStringList() << "*." + QString(AbstractAction::_GetFileSuffix().c_str()), QDir::Files/* | QDir::NoDotAndDotDot*/);
	for(int i = 0; i < actions.size(); i++)
	{
		StringANSI path = AssetLibrary::_GetDir() + AbstractAction::_GetDir() + actions.at(i).completeBaseName().toStdString() + "." + AbstractAction::_GetFileSuffix();

		File file(path, File::READ);

		if(file.IsOpened() && file.GetAccessMode() == File::READ)
		{
			int32 type;
			file.Read(type);

			AddAction(actions.at(i).completeBaseName(), type);
		}
	}
	SimpleList<Item*> removeList;

	for(int i = 0; i < topLevelItemCount(); i++)
	{ 
		Item* it = dynamic_cast<Item*>(topLevelItem(i));

		if(it && it->GetItemType() == Item::CATEGORY_TYPE)
		{
			for(int j = 0; j < it->childCount(); j++)
			{
				Item* child = dynamic_cast<Item*>(it->child(j));
				
				if(child && child->GetItemType() == Item::GROUP_TYPE)
				{ 
					if(child->IsEmpty())
					{
						removeList.AddToTail(child);
					}
				}
			}
		}
	}

	if(removeList.IsNotEmpty())
	{
		for(int i = 0; i < removeList.GetSize(); i++)
		{
			delete removeList.Get(i)->key;
		}
		removeList.Clear();
	}
	for(int i = 0; i < topLevelItemCount(); i++)
	{ 
		Item* it = dynamic_cast<Item*>(topLevelItem(i));

		if(it && it->GetItemType() == Item::CATEGORY_TYPE)
		{ 
			if(it->IsEmpty())
			{
				removeList.AddToTail(it);
			}
		}
	}

	if(removeList.IsNotEmpty())
	{
		for(int i = 0; i < removeList.GetSize(); i++)
		{
			delete removeList.Get(i)->key;
		}
		removeList.Clear();
	}
}


void ActionsLibraryTree::Refresh(QString _name)
{
	StringANSI path = AssetLibrary::_GetDir() + AbstractAction::_GetDir() + _name.toStdString() + "." + AbstractAction::_GetFileSuffix();

	File file(path, File::READ);

	if(file.IsOpened())
	{
		if(file.GetAccessMode() == File::READ)
		{
			int32 type;
			file.Read(type);

			AddAction(_name, type);
		}
	}
	else
	{
		delete FindAction(_name);
	}
}



void ActionsLibraryTree::AddAction(QString _name, int _actionType)
{
	Item* it = FindAction(_name, _actionType);
 
	if(it == NIL)
	{
		Item* group = FindGroup(_actionType);
		Item* category = FindCategory(_actionType);
		if(group == NIL)
		{
			if(category == NIL)
			{
				category = AddCategory(GetCategoryName(_actionType), _actionType);
			}
			
			group = AddGroup(category, GetGroupName(_actionType), _actionType);
		}
		it = new Item(group, _actionType, Item::ACTION_TYPE);
		it->setIcon(ICON_PROPERTY, actionIcon); 

		it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		it->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
		it->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);

		lastAction = it;
		lastGroup = group;
		lastCategory = category;
		QFont font("Verdana", 8, QFont::Normal, false);
		it->setFont(NAME_PROPERTY, font);
		it->setFont(TYPE_PROPERTY, font);

		it->SetName(_name);
		it->setText(TYPE_PROPERTY, "ACTION");

				
		category->setText(NAME_PROPERTY, category->GetName() + "(" + QString::number(GetAmountOfItems(category)) + ")");
		group->setText(NAME_PROPERTY, group->GetName() + "(" + QString::number(GetAmountOfItems(group)) + ")");
	}
}



ActionsLibraryTree::Item* ActionsLibraryTree::FindAction(QString _name, int _actionType)
{
	return FindAction(invisibleRootItem(), _name, _actionType);
}



ActionsLibraryTree::Item* ActionsLibraryTree::FindAction(QTreeWidgetItem* _it, QString _name, int _actionType)
{
	if(_it)
	{
		if(_it != invisibleRootItem())
		{
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::ACTION_TYPE)
			{
				if(item->GetActionType() == _actionType)
				{
					if(item->GetName() == _name)
					{
						return item; 
					}
				}
			}
		}
		
		Item* search;

		int child = 0;

		while(_it->child(child))
		{ 
			if(search = FindAction(_it->child(child), _name, _actionType))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}

 
ActionsLibraryTree::Item* ActionsLibraryTree::FindAction(QString _name)
{
	return FindAction(invisibleRootItem(), _name);
}


ActionsLibraryTree::Item* ActionsLibraryTree::FindAction(QTreeWidgetItem* _it, QString _name)
{
	if(_it)
	{
		int child = 0;

		if(_it != invisibleRootItem())
		{
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::ACTION_TYPE)
			{
				if(item->GetName() == _name)
				{
					return item;
				}
			}
		}
		
		Item* search;

		while(_it->child(child))
		{
			if(search = FindAction(_it->child(child), _name))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}


ActionsLibraryTree::Item* ActionsLibraryTree::AddCategory(QString _name, int _actionType)
{
	Item* it = new Item(invisibleRootItem(), _actionType, Item::CATEGORY_TYPE);
	it->setIcon(ICON_PROPERTY, folderIcon);

	it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	it->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
	it->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
	QFont font1("Verdana", 8, QFont::Bold, false);
	it->setFont(NAME_PROPERTY, font1);
	QFont font2("Verdana", 8, QFont::Normal, false);
	it->setFont(TYPE_PROPERTY, font2);

	it->SetName(_name);
	it->setText(TYPE_PROPERTY, "CATEGORY");
	return it;
}



ActionsLibraryTree::Item* ActionsLibraryTree::FindCategory(int _actionType)
{
	return FindCategory(invisibleRootItem(), _actionType);
}



ActionsLibraryTree::Item* ActionsLibraryTree::FindCategory(QTreeWidgetItem* _it, int _actionType)
{
	if(_it)
	{
		int child = 0;

		if(_it != invisibleRootItem())
		{ 
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::CATEGORY_TYPE)
			{
				if(GetCategoryName(item->GetActionType()) == GetCategoryName(_actionType))
				{
					return item; 
				}
			} 
		}
		
		Item* search;

		while(_it->child(child))
		{
			if(search = FindCategory(_it->child(child), _actionType))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}



ActionsLibraryTree::Item* ActionsLibraryTree::FindGroup(int _actionType)
{
	return FindGroup(invisibleRootItem(), _actionType);
}



ActionsLibraryTree::Item* ActionsLibraryTree::FindGroup(QTreeWidgetItem* _it, int _actionType)
{
	if(_it)
	{
		int child = 0;

		if(_it != invisibleRootItem())
		{ 
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::GROUP_TYPE)
			{
				if(GetGroupName(item->GetActionType()) == GetGroupName(_actionType) && GetCategoryName(item->GetActionType()) == GetCategoryName(_actionType))
				{ 
					return item; 
				}
			} 
		}
		
		Item* search;

		while(_it->child(child))
		{
			if(search = FindGroup(_it->child(child), _actionType))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}


ActionsLibraryTree::Item* ActionsLibraryTree::AddGroup(Item* _parent, QString _name, int _actionType)
{
	Item* it = new Item(_parent, _actionType, Item::GROUP_TYPE);
	it->setIcon(ICON_PROPERTY, folderIcon);

	it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	it->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
	it->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
	QFont font1("Verdana", 8, QFont::Bold, false);
	it->setFont(NAME_PROPERTY, font1);
	QFont font2("Verdana", 8, QFont::Normal, false);
	it->setFont(TYPE_PROPERTY, font2); 

	it->SetName(_name);
	it->setText(TYPE_PROPERTY, "GROUP");
	return it;
}



void ActionsLibraryTree::drawRow(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index)const
{
	QStyleOptionViewItem newOption(_option);

	Item* it = dynamic_cast<Item*>(itemFromIndex(_index));

	if(it)
	{
		if(it->GetItemType() == Item::CATEGORY_TYPE)
		{
			QLinearGradient grad(QPointF(0.6f, 1.0f), QPointF(1.0f, 1.0f));
			grad.setCoordinateMode(QGradient::ObjectBoundingMode);
			
			if(it == lastCategory)
			{
				grad.setColorAt(0, selectedBeginColor);
				grad.setColorAt(1, QColor(255,255,255));
			}
			else
			{ 
				grad.setColorAt(0, categoryBeginColor); 
				grad.setColorAt(1, QColor(255,255,255));
			}
	
			QBrush brush(grad);
			_painter->fillRect(_option.rect, brush);
			QTreeWidget::drawRow(_painter, _option, _index);
			return;
		}
		else if(it->GetItemType() == Item::GROUP_TYPE)
		{
			QLinearGradient grad(QPointF(0.6f, 1.0f), QPointF(1.0f, 1.0f));
			grad.setCoordinateMode(QGradient::ObjectBoundingMode);
			
			if(it == lastGroup)
			{
				grad.setColorAt(0, selectedBeginColor);
				grad.setColorAt(1, QColor(255,255,255));
			}
			else
			{
				grad.setColorAt(0, groupBeginColor);
				grad.setColorAt(1, QColor(255,255,255));
			}
			
			QBrush brush(grad);
			_painter->fillRect(_option.rect, brush);
			QTreeWidget::drawRow(_painter, _option, _index);
			return;
		}
		else
		{ 
			if(it == lastAction)
			{
				QLinearGradient grad(QPointF(0.6f, 1.0f), QPointF(1.0f, 1.0f));
				grad.setCoordinateMode(QGradient::ObjectBoundingMode);
				
				grad.setColorAt(0, selectedBeginColor);
				grad.setColorAt(1, QColor(255,255,255));
				
				QBrush brush(grad);
				_painter->fillRect(_option.rect, brush);
				QTreeWidget::drawRow(_painter, _option, _index);
				return;
			}
		}
	}

    QTreeWidget::drawRow(_painter, _option, _index);
}



void ActionsLibraryTree::keyPressEvent(QKeyEvent* _event)
{
	if(!_event->isAutoRepeat())
	{
		if(_event->key() == Qt::Key_Delete)
		{
			RemoveSelectedActions();
		}
		else if(_event->key() == Qt::Key_Return)
		{
			EditAction();
		}
	}
	
	QTreeWidget::keyPressEvent(_event);
}


QString ActionsLibraryTree::GetGroupName(int _actionType)
{ 
	switch(_actionType)
	{
		case Mouse::ENABLE_SHOW_CURSOR_MESSAGE:
		case Renderer::ENABLE_Z_BUFFER_MESSAGE:
		case ColorableObject::CHANGE_BACKGROUND_COLOR_MESSAGE:
		case SystemTime::GET_SYSTEM_TIME_MESSAGE:
		case AssetLibrary::LOAD_ASSET_MESSAGE:
		case Random::CHANGE_RANDOM_SEED_MESSAGE:
		case AbstractVariable::CHECK_EQUALITY_MESSAGE:
		{
			return "SYSTEM";
		}
		case MessageHandler<int32>::SEND_MESSAGE:

		case AbstractObject::CHANGE_NAME_MESSAGE:
		case AbstractObject::GET_NAME_MESSAGE:
		case AbstractObject::DESTROY_OBJECT_MESSAGE:
		case AbstractObject::CHECK_PROPERTY_MESSAGE:
		case AbstractObject::ENABLE_PROPERTY_MESSAGE:

		case AssetLibrary::CHECK_OBJECT_EXIST_MESSAGE:
		case AssetLibrary::SET_PRIVATE_ASSET_MESSAGE:
		{
			return "ABSTRACT_OBJECT";
		}
		case Timer::CREATE_TIMER_MESSAGE:
		{
			return "TIMER";
		}
		case AbstractObject::SAVE_TO_FILE_MESSAGE:
		{
			return "ABSTRACT_OBJECT";
		}
		case AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE:
		case AbstractAnimation::GET_AMOUNT_OF_FRAMES_MESSAGE:
		{
			return "ABSTRACT_ANIMATION";
		}
		case ActivatableObject::CHANGE_ACTIVATION_LIMIT_MESSAGE:
		case ActivatableObject::CHANGE_DEACTIVATION_MODE_MESSAGE:
		{
			return "ACTIVATABLE_OBJECT";
		}
		case GraphicsObject::ENABLE_SHOW_MESSAGE:
		{
			return "GRAPHICS_OBJECT";
		}
		case LogicObject::ENABLE_FREEZE_MESSAGE:
		case LogicObject::CHANGE_DELTA_SCALE_MESSAGE:
		{
			return "LOGIC_OBJECT";
		}
		case TexturedAnimation::CHANGE_ATLAS_MESSAGE:
		case TexturedAnimation::GET_FRAME_SIZE_MESSAGE:
		case TexturedAnimation::ENABLE_POSITION_OFFSET_MESSAGE: 
		case TexturedAnimation::CREATE_TEXTURED_ANIMATION_MESSAGE: 
		{
			return "TEXTURED_ANIMATION";
		}
		case Texture2D::GET_TEXTURE_SIZE_MESSAGE:
		case Texture2D::CREATE_BOX_TEXTURE_MAP_MESSAGE:
		case Texture2D::CHANGE_BOX_TEXTURE_MAP_MESSAGE:
		case Texture2D::CREATE_TEXTURE_MESSAGE:
		{
			return "TEXTURE";
		}
			case Random::GET_RANDOM_INTEGER_MESSAGE:
			case Random::GET_RANDOM_FLOAT_MESSAGE:
			case Random::GET_RANDOM_BOOLEAN_MESSAGE:
		{
			return "VARIABLE";
		}
		case AbstractList::ADD_TO_LIST_MESSAGE:
			case AbstractList::REMOVE_INDEX_FROM_LIST_MESSAGE:
			case AbstractList::REMOVE_ITEM_FROM_LIST_MESSAGE:
		case AbstractList::GET_DATA_FROM_ITEM_MESSAGE:
		case AbstractList::COPY_LIST_MESSAGE:
			case AbstractList::CHANGE_ITEM_INTEGER_MESSAGE:
			case AbstractList::CHANGE_ITEM_FLOAT_MESSAGE:
			case AbstractList::CHANGE_ITEM_STRING_MESSAGE:
			case AbstractList::CHANGE_ITEM_BOOLEAN_MESSAGE:
			case AbstractList::CHANGE_ITEM_VECTOR_2_MESSAGE:
			case AbstractList::CHANGE_ITEM_VECTOR_3_MESSAGE:
			case AbstractList::CHANGE_ITEM_QUATERNION_MESSAGE:
			case AbstractList::CHANGE_ITEM_ROTATABLE_MESSAGE:
			case AbstractList::CHANGE_ITEM_SCALABLE_MESSAGE:
			case AbstractList::CHANGE_ITEM_TRANSLATABLE_MESSAGE:
			case AbstractList::CHANGE_ITEM_COLORABLE_MESSAGE:
		case AbstractList::RANDOMIZE_LIST_MESSAGE:
			case AbstractList::SWAP_ITEM_MESSAGE:
			case AbstractList::SWAP_INDEX_MESSAGE:
			case AbstractList::SWAP_LIST_MESSAGE:
		case AbstractList::CLEAR_LIST_MESSAGE:
		case AbstractList::CREATE_LIST_MESSAGE:
		case AbstractList::GET_AMOUNT_OF_ITEMS_MESSAGE:
		case AbstractList::GET_LIST_LENGTH_MESSAGE:
			case AbstractList::CHANGE_LIST_INTEGER_MESSAGE:
			case AbstractList::CHANGE_LIST_FLOAT_MESSAGE:
			case AbstractList::CHANGE_LIST_STRING_MESSAGE:
			case AbstractList::CHANGE_LIST_BOOLEAN_MESSAGE:
			case AbstractList::CHANGE_LIST_VECTOR_2_MESSAGE:
			case AbstractList::CHANGE_LIST_VECTOR_3_MESSAGE:
			case AbstractList::CHANGE_LIST_QUATERNION_MESSAGE:
			case AbstractList::CHANGE_LIST_ROTATABLE_MESSAGE:
			case AbstractList::CHANGE_LIST_SCALABLE_MESSAGE:
			case AbstractList::CHANGE_LIST_TRANSLATABLE_MESSAGE:
			case AbstractList::CHANGE_LIST_COLORABLE_MESSAGE:
		case AbstractList::CHECK_LIST_EMPTY_MESSAGE: 
		case AbstractList::ROTATE_LIST_MESSAGE: 
		case AbstractList::INVERSE_LIST_MESSAGE: 
		case AbstractList::GET_ITEM_INDEX_MESSAGE: 
		{
			return "LIST";
		}
			case Variable<Vector3>::GET_VECTOR_3_ANGLE_MESSAGE:
			case Variable<Vector2>::GET_VECTOR_2_ANGLE_MESSAGE:
			case Variable<Vector2>::GET_VECTOR_2_LENGTH_MESSAGE:
			case Variable<Vector3>::GET_VECTOR_3_LENGTH_MESSAGE:
			case Variable<Vector3>::GET_NORMALIZED_VECTOR_3_MESSAGE:
			case Variable<Vector2>::GET_NORMALIZED_VECTOR_2_MESSAGE:
			case Variable<Vector3>::CHANGE_VECTOR_3_COMPONENT_MESSAGE:
			case Variable<Vector2>::CHANGE_VECTOR_2_COMPONENT_MESSAGE:
			case Variable<Vector3>::GET_VECTOR_3_COMPONENT_MESSAGE:
			case Variable<Vector2>::GET_VECTOR_2_COMPONENT_MESSAGE:
			case Variable<Vector3>::VECTOR_3_TO_SPHERICAL_MESSAGE:
			case Variable<Vector2>::VECTOR_2_TO_SPHERICAL_MESSAGE:
			case Variable<float>::CHANGE_FLOAT_RANGE_MESSAGE:
			case Variable<int32>::CHANGE_INTEGER_RANGE_MESSAGE:
			case AbstractVariable::CREATE_INTEGER_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_FLOAT_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_STRING_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_BOOLEAN_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_VECTOR_2_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_VECTOR_3_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_QUATERNION_VARIABLE_MESSAGE:
			case AbstractVariable::CHANGE_INTEGER_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_FLOAT_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_STRING_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_BOOLEAN_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_VECTOR_2_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_VECTOR_3_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_QUATERNION_VALUE_MESSAGE:
		{ 
			return "VARIABLE";
		}
			case ColorableObject::CHANGE_COLOR_RGB_MESSAGE:
			case ColorableObject::CHANGE_COLOR_HSL_MESSAGE:
			case ColorableObject::GET_COLOR_RGB_MESSAGE:
			case ColorableObject::GET_COLOR_HSL_MESSAGE:
			case ColorableObject::CHANGE_ALPHA_MESSAGE:
			case ColorableObject::CHANGE_RED_MESSAGE:
			case ColorableObject::CHANGE_GREEN_MESSAGE:
			case ColorableObject::CHANGE_BLUE_MESSAGE:
			case ColorableObject::CHANGE_HUE_MESSAGE:
			case ColorableObject::CHANGE_SATURATION_MESSAGE:
			case ColorableObject::CHANGE_LIGHTNESS_MESSAGE:
			case ColorableObject::GET_ALPHA_MESSAGE:
			case ColorableObject::GET_RED_MESSAGE:
			case ColorableObject::GET_GREEN_MESSAGE:
			case ColorableObject::GET_BLUE_MESSAGE:
			case ColorableObject::GET_HUE_MESSAGE:
			case ColorableObject::GET_SATURATION_MESSAGE:
			case ColorableObject::GET_LIGHTNESS_MESSAGE:
		{
			return "COLORABLE_OBJECT";
		}
		case ScalableObject::REPLACE_SCALE_MESSAGE:
		case ScalableObject::DISPLACE_SCALE_MESSAGE:
		case ScalableObject::GET_SCALE_MESSAGE:
		{
			return "SCALABLE OBJECT";
		}
		case TranslatableObject::REPLACE_POSITION_MESSAGE:
		case TranslatableObject::DISPLACE_POSITION_MESSAGE:
		case TranslatableObject::GET_POSITION_MESSAGE:
		case TranslatableObject::MOVE_AROUND_PIVOT_MESSAGE:
		{
			return "TRANSLATABLE_OBJECT";
		}
		case RotatableObject::REPLACE_ROTATION_MESSAGE:
		case RotatableObject::DISPLACE_ROTATION_MESSAGE:
		case RotatableObject::GET_ROTATION_MESSAGE:
		case RotatableObject::CREATE_ROTATION_MESSAGE:
			case Variable<Quaternion>::CREATE_ROTATION_FROM_VECTOR_3_TO_VECTOR_3_MESSAGE:
			case Variable<Quaternion>::CREATE_ROTATION_FROM_QUATERNION_TO_QUATERNION_MESSAGE:
		case RotatableObject::ROTATE_AROUND_PIVOT_MESSAGE:
		{
			return "ROTATABLE_OBJECT";
		}
		case Line::GET_LINE_ANGLE_Z_MESSAGE:
		case Line::GET_LINE_LENGTH_MESSAGE:
		case Line::GET_LINE_DIRECTION_MESSAGE:
		{
			return "LINE";
		}
		case Circle::GET_CIRCLE_LENGTH_MESSAGE:
		case Circle::GET_RADIUS_FROM_2_POINTS_MESSAGE:
		case Circle::GET_RADIUS_FROM_3_POINTS_MESSAGE:
		{
			return "CIRCLE";
		}
		case Plane::CHANGE_PLANE_NORMAL_MESSAGE:
		{
			return "PLANE";
		}
		case PolygonalSurface::CHECK_LINE_IN_POLYGON_MESSAGE:
		case PolygonalSurface::CHANGE_MESH_MESSAGE:
		case PolygonalSurface::CHANGE_HITBOX_MESSAGE:
		case PolygonalSurface::ENABLE_COLLISION_POLYGONS_INFO_MESSAGE:
		case PolygonalSurface::ENABLE_COLLISION_CHECKING_MESSAGE:
		case PolygonalSurface::CHANGE_COLLISION_GROUP_MESSAGE:
		    case PolygonalSurface::CHECK_COLLISION_GROUP_GROUP_MESSAGE:
			case PolygonalSurface::CHECK_COLLISION_OBJECT_OBJECT_MESSAGE:
			case PolygonalSurface::CHECK_COLLISION_OBJECT_GROUP_MESSAGE:
		{
			return "POLYGONAL_SURFACE";
		}
		case Mesh::CREATE_QUAD_MESSAGE:
		case Mesh::CREATE_BOX_MESSAGE:
		case Mesh::CREATE_MESH_MESSAGE:
		{
			return "MESH";
		}
		case AbstractSurface::CHECK_LINE_IN_SURFACE_MESSAGE:
		case AbstractSurface::CHANGE_INTERSECTION_MODE_MESSAGE:
		{
			return "ABSTRACT_SURFACE";
		}
			case GraphicsStrip::CHANGE_STRIP_VECTORS_MESSAGE:
			case GraphicsStrip::CHANGE_STRIP_TRANSLATABLES_MESSAGE:
			case GraphicsStrip::CHANGE_STRIP_COLORABLES_MESSAGE:
			case GraphicsStrip::CHANGE_STRIP_SCALABLES_MESSAGE:
		{
			return "GRAPHICS_STRIP";
		}
		case GraphicsSurface::ENABLE_BACK_FACE_CULLING_MESSAGE:
		case GraphicsSurface::ENABLE_STATIC_OBJECT_MESSAGE:
		{
			return "GRAPHICS_SURFACE";
		}
		case TexturingSurface::CHANGE_FRONT_TEXTURE_MESSAGE:
		case TexturingSurface::CHANGE_BACK_TEXTURE_MESSAGE:
		case TexturingSurface::CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE:
		case TexturingSurface::CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE:
		case TexturingSurface::CHECK_LINE_IN_IMAGE_MESSAGE:
		case TexturingSurface::ENABLE_TEXTURE_FLIP_MESSAGE:
		case TexturingSurface::SET_PUBLIC_TEXTURE_MESSAGE:
		{
			return "TEXTURING_SURFACE";
		}
		case Keyboard::CREATE_KEYBOARD_EVENT_MESSAGE:
		{ 
			return "KEYBOARD";
		}
		case Scene::PICK_SCENE_MESSAGE:
		{
			return "SCENE";
		}
		case ObjectScene::ADD_TO_LAYER_MESSAGE:
		case ObjectScene::REMOVE_FROM_LAYER_MESSAGE:
		{
			return "LAYER";
		}
		case LogicScene::ADD_TO_UPDATE_LIST_MESSAGE:
		case LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE:
		{
			return "LOGIC_SCENE";
		}
		case GraphicsScene::ADD_TO_RENDER_LIST_MESSAGE:
		case GraphicsScene::REMOVE_FROM_RENDER_LIST_MESSAGE:
		{
			return "GRAPHICS_SCENE";
		}
		case PlayerControl::ADD_INPUT_EVENT_MESSAGE:
		case PlayerControl::CLEAR_INPUT_EVENTS_MESSAGE:
		case PlayerControl::SET_ONE_INPUT_EVENT_MESSAGE:
		case PlayerControl::CREATE_PLAYER_CONTROL_MESSAGE:
		case PlayerControl::CHANGE_INPUT_TIMING_MESSAGE:
		{
			return "PLAYER_CONTROL";
		}
		case TiledSpace3D::CREATE_TILED_SPACE_MESSAGE:
		case TiledSpace3D::CREATE_BRUSH_MESSAGE:
		case TiledSpace3D::TILED_SPACE_FILL_MESSAGE:
		case TiledSpace3D::GET_TILE_CENTER_MESSAGE:
		case TiledSpace3D::CREATE_SOLID_TILE_MESSAGE:
		case TiledSpace3D::CREATE_EMPTY_TILE_MESSAGE:
		case TiledSpace3D::CHANGE_TILED_SPACE_SIZE_MESSAGE:
		case TiledSpace3D::SET_PUBLIC_TILE_MESSAGE:
		case TiledSpace3D::REMOVE_BRUSH_MESSAGE:
		case TiledSpace3D::MOVE_TILE_FROM_TO_MESSAGE:
		case TiledSpace3D::CHECK_TILE_EXIST_MESSAGE:
		case TiledSpace3D::CHECK_TILE_PASS_MESSAGE:
		case TiledSpace3D::CHECK_TILE_SOLID_MESSAGE:
		case TiledSpace3D::CHANGE_TILE_PASS_MESSAGE:
		case TiledSpace3D::GET_BRUSH_OF_TILE_MESSAGE:
		case TiledSpace3D::COPY_TILE_MESSAGE:
		case TiledSpace3D::SWAP_TWO_TILES_MESSAGE:
		{
			return "TILED_SPACE";
		}
		case Set::CREATE_SET_MESSAGE:
		case Set::RESIZE_SET_MESSAGE:
		case Set::GET_AMOUNT_OF_SLOTS_MESSAGE:
		case Set::GET_DATA_FROM_SLOT_MESSAGE:
		case Set::SET_DATA_TO_SLOT_MESSAGE:
		{
			return "CREATE_SET";
		}
		case GraphicsContainer::CHANGE_CAMERA_MESSAGE:
		case GraphicsContainer::CHANGE_RENDER_TARGET_MESSAGE:
		{ 
			return "GRAPHICS_CONTAINER";
		}
		case Trigger::CREATE_TRIGGER_MESSAGE:
		case Trigger::ENABLE_TRIGGERS_BLOCK_MESSAGE:
		{
			return "TRIGGER";
		}
		case Camera3D::PICK_CAMERA_MESSAGE:
		case Camera3D::CHANGE_PROJECTION_MODE_MESSAGE:
		case Camera3D::CHANGE_MOVEMENT_MODE_MESSAGE:
		case Camera3D::CHANGE_ROTATION_MODE_MESSAGE:
		case Camera3D::CHANGE_VIEWING_POINT_MESSAGE:
		case Camera3D::GET_VIEWING_POINT_MESSAGE:
		case Camera3D::CHANGE_DISTANCE_TO_VIEWING_POINT_MESSAGE:
		case Camera3D::CAMERA_LOOK_AT_MESSAGE:
		case Camera3D::CHANGE_ZOOM_FAR_MESSAGE: 
		case Camera3D::CHANGE_ZOOM_NEAR_MESSAGE: 
		case Camera3D::CHANGE_ZOOM_SPEED_MESSAGE: 
		case Camera3D::ENABLE_ZOOM_MESSAGE: 
		case Camera3D::ENABLE_ROTATION_MESSAGE: 
		case Camera3D::GET_SELECT_RAY_MESSAGE:
		case Camera3D::CHECK_FRUSTUM_IN_QUAD_MESSAGE:
		case Camera3D::CREATE_CAMERA_MESSAGE:
		case Camera3D::GET_CAMERA_LOOK_MESSAGE:
		case Camera3D::ENABLE_ROTATION_RANGE_MESSAGE:
		case Camera3D::CHANGE_ROTATION_RANGE_MESSAGE:
		{
			return "CAMERA";
		}
		case FloatLerp::CHANGE_FLOAT_LERP_MODE_MESSAGE:		
		case FloatLerp::CREATE_FLOAT_LERP_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_ALPHA_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_RED_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_GREEN_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_BLUE_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_HUE_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_SATURATION_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_LIGHTNESS_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_DELTA_SCALE_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_ANIMATION_FRAME_MESSAGE:
		case FloatLerp::CHANGE_FLOAT_LERP_SPEED_MESSAGE:
		case FloatLerp::CHANGE_FLOAT_LERP_T_MESSAGE:
		case FloatLerp::GET_FLOAT_LERP_T_MESSAGE:
		case FloatLerp::GET_FLOAT_LERP_CURRENT_MESSAGE:
		case FloatLerp::GET_FLOAT_LERP_DELTA_MESSAGE:
			case FloatLerp::CHANGE_FLOAT_LERP_FLOAT_TARGET_MESSAGE:
			case FloatLerp::CHANGE_FLOAT_LERP_INTEGER_TARGET_MESSAGE:
		{
			return "FLOAT_LERP";
		}
			case QuaternionLerp::SET_DATA_TO_CONTROL_QUATERNION_MESSAGE:
			case QuaternionLerp::SET_DATA_TO_CONTROL_ROTATABLE_MESSAGE:
			case QuaternionLerp::GET_DATA_FROM_CONTROL_QUATERNION_MESSAGE:
			case QuaternionLerp::GET_DATA_FROM_CONTROL_ROTATABLE_MESSAGE:
   	    case QuaternionLerp::CREATE_QUATERNION_LERP_MESSAGE:
			case QuaternionLerp::STREAMING_QUATERNION_CURRENT_MESSAGE:
			case QuaternionLerp::STREAMING_ROTATABLE_CURRENT_MESSAGE:
			case QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE:
			case QuaternionLerp::STREAMING_ROTATABLE_DELTA_MESSAGE:
			case QuaternionLerp::CHANGE_CONTROL_QUATERNIONS_MESSAGE:
			case QuaternionLerp::CHANGE_CONTROL_ROTATABLES_MESSAGE:
			case QuaternionLerp::SET_TWO_QUATERNIONS_MESSAGE:
			case QuaternionLerp::SET_TWO_ROTATABLES_MESSAGE:
			case QuaternionLerp::GET_QUATERNION_LERP_T_MESSAGE:
			case QuaternionLerp::GET_ROTATABLE_LERP_T_MESSAGE:
		case QuaternionLerp::GET_QUATERNION_LERP_PATH_MESSAGE:
		case QuaternionLerp::GET_CONTROL_QUATERNIONS_MESSAGE:
		{
			return "QUATERNION_LERP";
		}
			case VectorLerp::GET_VECTOR_LERP_T_MESSAGE:
			case VectorLerp::GET_TRANSLATABLE_LERP_T_MESSAGE:
			case VectorLerp::GET_SCALABLE_LERP_T_MESSAGE:
			case VectorLerp::GET_COLORABLE_LERP_T_MESSAGE:
			case VectorLerp::SET_DATA_TO_CONTROL_VECTOR_MESSAGE:
			case VectorLerp::SET_DATA_TO_CONTROL_TRANSLATABLE_MESSAGE:
			case VectorLerp::SET_DATA_TO_CONTROL_SCALABLE_MESSAGE:
			case VectorLerp::SET_DATA_TO_CONTROL_COLORABLE_MESSAGE:
			case VectorLerp::GET_DATA_FROM_CONTROL_VECTOR_MESSAGE:
			case VectorLerp::GET_DATA_FROM_CONTROL_TRANSLATABLE_MESSAGE:
			case VectorLerp::GET_DATA_FROM_CONTROL_SCALABLE_MESSAGE:
			case VectorLerp::GET_DATA_FROM_CONTROL_COLORABLE_MESSAGE:
		case VectorLerp::CREATE_VECTOR_LERP_MESSAGE:
			case VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE:
			case VectorLerp::STREAMING_TRANSLATABLE_CURRENT_MESSAGE:
			case VectorLerp::STREAMING_SCALABLE_CURRENT_MESSAGE:
			case VectorLerp::STREAMING_COLORABLE_CURRENT_MESSAGE:
			case VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE:
			case VectorLerp::STREAMING_TRANSLATABLE_DELTA_MESSAGE:
			case VectorLerp::STREAMING_SCALABLE_DELTA_MESSAGE:
			case VectorLerp::STREAMING_COLORABLE_DELTA_MESSAGE:
			case VectorLerp::CHANGE_CONTROL_VECTORS_MESSAGE:
			case VectorLerp::CHANGE_CONTROL_TRANSLATABLES_MESSAGE:
			case VectorLerp::CHANGE_CONTROL_SCALABLES_MESSAGE:
			case VectorLerp::CHANGE_CONTROL_COLORABLES_MESSAGE:
			case VectorLerp::SET_TWO_VECTORS_MESSAGE:
			case VectorLerp::SET_TWO_TRANSLATABLES_MESSAGE:
			case VectorLerp::SET_TWO_SCALABLES_MESSAGE:
			case VectorLerp::SET_TWO_COLORABLES_MESSAGE:
		case VectorLerp::CHANGE_VECTOR_LERP_MODE_MESSAGE:
		case VectorLerp::GET_VECTOR_LERP_PATH_MESSAGE:
		case VectorLerp::GET_CONTROL_VECTORS_MESSAGE:
		{
			return "VECTOR_LERP";
		}
		case LerpingObject::CHANGE_FLOAT_LERP_MESSAGE:
		{
			return "LERPING_OBJECT";
		}
		case PlayingObject::CHANGE_PLAYING_MODE_MESSAGE:
		case PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE:
		case PlayingObject::CHANGE_PLAYING_STATE_MESSAGE:
		case PlayingObject::REPLAY_MESSAGE:
		{
			return "PLAYING_OBJECT";
		}
	}

	return "";
}



QString ActionsLibraryTree::GetCategoryName(int _actionType)
{ 
	switch(_actionType)
	{
		case MessageHandler<int32>::SEND_MESSAGE:

		case AbstractObject::CHANGE_NAME_MESSAGE:
		case AbstractObject::GET_NAME_MESSAGE:
		case AbstractObject::DESTROY_OBJECT_MESSAGE:
		case AbstractObject::CHECK_PROPERTY_MESSAGE:
		case AbstractObject::ENABLE_PROPERTY_MESSAGE:

		case AssetLibrary::CHECK_OBJECT_EXIST_MESSAGE:
		case AssetLibrary::SET_PRIVATE_ASSET_MESSAGE:

		case Mouse::ENABLE_SHOW_CURSOR_MESSAGE:
		{ 
			return "BASIC";
		}
		case SystemTime::GET_SYSTEM_TIME_MESSAGE:

		case LogicObject::ENABLE_FREEZE_MESSAGE:
		case LogicObject::CHANGE_DELTA_SCALE_MESSAGE:

		case Timer::CREATE_TIMER_MESSAGE:
		{
			return "TIME";
		}
		case Trigger::CREATE_TRIGGER_MESSAGE:
		case Trigger::ENABLE_TRIGGERS_BLOCK_MESSAGE:
			
		case AbstractVariable::CHECK_EQUALITY_MESSAGE:
		{
			return "SCRIPT";
		}
			case Variable<Vector3>::CHANGE_VECTOR_3_COMPONENT_MESSAGE:
			case Variable<Vector2>::CHANGE_VECTOR_2_COMPONENT_MESSAGE:
			case Variable<Vector3>::GET_VECTOR_3_COMPONENT_MESSAGE:
			case Variable<Vector2>::GET_VECTOR_2_COMPONENT_MESSAGE:
			case Variable<Vector3>::VECTOR_3_TO_SPHERICAL_MESSAGE:
			case Variable<Vector2>::VECTOR_2_TO_SPHERICAL_MESSAGE:
			case Variable<float>::CHANGE_FLOAT_RANGE_MESSAGE:
			case Variable<int32>::CHANGE_INTEGER_RANGE_MESSAGE:
			case AbstractVariable::CREATE_INTEGER_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_FLOAT_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_STRING_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_BOOLEAN_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_VECTOR_2_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_VECTOR_3_VARIABLE_MESSAGE:
			case AbstractVariable::CREATE_QUATERNION_VARIABLE_MESSAGE:
			case AbstractVariable::CHANGE_INTEGER_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_FLOAT_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_STRING_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_BOOLEAN_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_VECTOR_2_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_VECTOR_3_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_QUATERNION_VALUE_MESSAGE:

		case AbstractList::ADD_TO_LIST_MESSAGE:
			case AbstractList::REMOVE_INDEX_FROM_LIST_MESSAGE:
			case AbstractList::REMOVE_ITEM_FROM_LIST_MESSAGE:
		case AbstractList::GET_DATA_FROM_ITEM_MESSAGE:
		case AbstractList::COPY_LIST_MESSAGE:
			case AbstractList::CHANGE_ITEM_INTEGER_MESSAGE:
			case AbstractList::CHANGE_ITEM_FLOAT_MESSAGE:
			case AbstractList::CHANGE_ITEM_STRING_MESSAGE:
			case AbstractList::CHANGE_ITEM_BOOLEAN_MESSAGE:
			case AbstractList::CHANGE_ITEM_VECTOR_2_MESSAGE:
			case AbstractList::CHANGE_ITEM_VECTOR_3_MESSAGE:
			case AbstractList::CHANGE_ITEM_QUATERNION_MESSAGE:
			case AbstractList::CHANGE_ITEM_ROTATABLE_MESSAGE:
			case AbstractList::CHANGE_ITEM_SCALABLE_MESSAGE:
			case AbstractList::CHANGE_ITEM_TRANSLATABLE_MESSAGE:
			case AbstractList::CHANGE_ITEM_COLORABLE_MESSAGE:
		case AbstractList::CLEAR_LIST_MESSAGE:
		case AbstractList::CREATE_LIST_MESSAGE:
		case AbstractList::GET_AMOUNT_OF_ITEMS_MESSAGE:
		case AbstractList::GET_LIST_LENGTH_MESSAGE:
			case AbstractList::CHANGE_LIST_INTEGER_MESSAGE:
			case AbstractList::CHANGE_LIST_FLOAT_MESSAGE:
			case AbstractList::CHANGE_LIST_STRING_MESSAGE:
			case AbstractList::CHANGE_LIST_BOOLEAN_MESSAGE:
			case AbstractList::CHANGE_LIST_VECTOR_2_MESSAGE:
			case AbstractList::CHANGE_LIST_VECTOR_3_MESSAGE:
			case AbstractList::CHANGE_LIST_QUATERNION_MESSAGE:
			case AbstractList::CHANGE_LIST_ROTATABLE_MESSAGE:
			case AbstractList::CHANGE_LIST_SCALABLE_MESSAGE:
			case AbstractList::CHANGE_LIST_TRANSLATABLE_MESSAGE:
			case AbstractList::CHANGE_LIST_COLORABLE_MESSAGE:
		case AbstractList::CHECK_LIST_EMPTY_MESSAGE: 
		case AbstractList::ROTATE_LIST_MESSAGE: 
		case AbstractList::INVERSE_LIST_MESSAGE: 
		case AbstractList::GET_ITEM_INDEX_MESSAGE:
			case AbstractList::SWAP_ITEM_MESSAGE:
			case AbstractList::SWAP_INDEX_MESSAGE:
			case AbstractList::SWAP_LIST_MESSAGE:

		case Scene::PICK_SCENE_MESSAGE:

		case ObjectScene::ADD_TO_LAYER_MESSAGE:
		case ObjectScene::REMOVE_FROM_LAYER_MESSAGE:

		case LogicScene::ADD_TO_UPDATE_LIST_MESSAGE:
		case LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE:
				
		case GraphicsScene::ADD_TO_RENDER_LIST_MESSAGE:
		case GraphicsScene::REMOVE_FROM_RENDER_LIST_MESSAGE:

		case TiledSpace3D::CREATE_TILED_SPACE_MESSAGE:
		case TiledSpace3D::CREATE_BRUSH_MESSAGE:
		case TiledSpace3D::TILED_SPACE_FILL_MESSAGE:
		case TiledSpace3D::GET_TILE_CENTER_MESSAGE:
		case TiledSpace3D::CREATE_SOLID_TILE_MESSAGE:
		case TiledSpace3D::CREATE_EMPTY_TILE_MESSAGE:
		case TiledSpace3D::CHANGE_TILED_SPACE_SIZE_MESSAGE:
		case TiledSpace3D::SET_PUBLIC_TILE_MESSAGE:
		case TiledSpace3D::REMOVE_BRUSH_MESSAGE:
		case TiledSpace3D::MOVE_TILE_FROM_TO_MESSAGE:
		case TiledSpace3D::CHECK_TILE_EXIST_MESSAGE:
		case TiledSpace3D::CHECK_TILE_PASS_MESSAGE:
		case TiledSpace3D::CHECK_TILE_SOLID_MESSAGE:
		case TiledSpace3D::CHANGE_TILE_PASS_MESSAGE:
		case TiledSpace3D::GET_BRUSH_OF_TILE_MESSAGE:
		case TiledSpace3D::COPY_TILE_MESSAGE:
		case TiledSpace3D::SWAP_TWO_TILES_MESSAGE:

		case Set::CREATE_SET_MESSAGE:
		case Set::RESIZE_SET_MESSAGE:
		case Set::GET_AMOUNT_OF_SLOTS_MESSAGE:
		case Set::GET_DATA_FROM_SLOT_MESSAGE:
		case Set::SET_DATA_TO_SLOT_MESSAGE:
		{ 
			return "CONTAINER";
		}
		case AssetLibrary::LOAD_ASSET_MESSAGE:

		case AbstractObject::SAVE_TO_FILE_MESSAGE:
		{
			return "FILE";
		}
		case TexturedAnimation::CHANGE_ATLAS_MESSAGE:
        case TexturedAnimation::GET_FRAME_SIZE_MESSAGE:
		case TexturedAnimation::CREATE_TEXTURED_ANIMATION_MESSAGE:
			case ColorableObject::CHANGE_COLOR_RGB_MESSAGE:
			case ColorableObject::CHANGE_COLOR_HSL_MESSAGE:
			case ColorableObject::GET_COLOR_RGB_MESSAGE:
			case ColorableObject::GET_COLOR_HSL_MESSAGE:
			case ColorableObject::CHANGE_ALPHA_MESSAGE:
			case ColorableObject::CHANGE_RED_MESSAGE:
			case ColorableObject::CHANGE_GREEN_MESSAGE:
			case ColorableObject::CHANGE_BLUE_MESSAGE:
			case ColorableObject::CHANGE_HUE_MESSAGE:
			case ColorableObject::CHANGE_SATURATION_MESSAGE:
			case ColorableObject::CHANGE_LIGHTNESS_MESSAGE:
			case ColorableObject::GET_ALPHA_MESSAGE:
			case ColorableObject::GET_RED_MESSAGE:
			case ColorableObject::GET_GREEN_MESSAGE:
			case ColorableObject::GET_BLUE_MESSAGE:
			case ColorableObject::GET_HUE_MESSAGE:
			case ColorableObject::GET_SATURATION_MESSAGE:
			case ColorableObject::GET_LIGHTNESS_MESSAGE:

		case GraphicsSurface::ENABLE_BACK_FACE_CULLING_MESSAGE:

		case TexturingSurface::CHANGE_FRONT_TEXTURE_MESSAGE:
		case TexturingSurface::CHANGE_BACK_TEXTURE_MESSAGE:
		case TexturingSurface::CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE:
		case TexturingSurface::CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE:
		case TexturingSurface::SET_PUBLIC_TEXTURE_MESSAGE:

		case Texture2D::GET_TEXTURE_SIZE_MESSAGE:
		case Texture2D::CREATE_BOX_TEXTURE_MAP_MESSAGE:
		case Texture2D::CHANGE_BOX_TEXTURE_MAP_MESSAGE:
		case Texture2D::CREATE_TEXTURE_MESSAGE:
		{
			return "MATERIAL";
		}
		case Random::CHANGE_RANDOM_SEED_MESSAGE:
			case Random::GET_RANDOM_INTEGER_MESSAGE:
			case Random::GET_RANDOM_FLOAT_MESSAGE:
			case Random::GET_RANDOM_BOOLEAN_MESSAGE:

		case AbstractList::RANDOMIZE_LIST_MESSAGE:
		{
			return "RANDOMIZATION";
		}
			case Variable<Vector3>::GET_VECTOR_3_ANGLE_MESSAGE:
			case Variable<Vector2>::GET_VECTOR_2_ANGLE_MESSAGE:
			case Variable<Vector2>::GET_VECTOR_2_LENGTH_MESSAGE:
			case Variable<Vector3>::GET_VECTOR_3_LENGTH_MESSAGE:
			case Variable<Vector3>::GET_NORMALIZED_VECTOR_3_MESSAGE:
			case Variable<Vector2>::GET_NORMALIZED_VECTOR_2_MESSAGE:

		case ScalableObject::REPLACE_SCALE_MESSAGE:
		case ScalableObject::DISPLACE_SCALE_MESSAGE:
		case ScalableObject::GET_SCALE_MESSAGE:

		case TranslatableObject::REPLACE_POSITION_MESSAGE:
		case TranslatableObject::DISPLACE_POSITION_MESSAGE:
		case TranslatableObject::GET_POSITION_MESSAGE:
		case TranslatableObject::MOVE_AROUND_PIVOT_MESSAGE:

		case RotatableObject::REPLACE_ROTATION_MESSAGE:
		case RotatableObject::DISPLACE_ROTATION_MESSAGE:
		case RotatableObject::GET_ROTATION_MESSAGE:
		case RotatableObject::CREATE_ROTATION_MESSAGE:
			case Variable<Quaternion>::CREATE_ROTATION_FROM_VECTOR_3_TO_VECTOR_3_MESSAGE:
			case Variable<Quaternion>::CREATE_ROTATION_FROM_QUATERNION_TO_QUATERNION_MESSAGE:
		case RotatableObject::ROTATE_AROUND_PIVOT_MESSAGE:

		case TexturedAnimation::ENABLE_POSITION_OFFSET_MESSAGE:  

		case TexturingSurface::ENABLE_TEXTURE_FLIP_MESSAGE:

		case LerpingObject::CHANGE_FLOAT_LERP_MESSAGE:

		case GraphicsSurface::ENABLE_STATIC_OBJECT_MESSAGE:
		{
			return "TRANSFORMATION";
		}
		case PolygonalSurface::CHANGE_MESH_MESSAGE:

		case Mesh::CREATE_QUAD_MESSAGE:
		case Mesh::CREATE_BOX_MESSAGE:
		case Mesh::CREATE_MESH_MESSAGE:
			case GraphicsStrip::CHANGE_STRIP_VECTORS_MESSAGE:
			case GraphicsStrip::CHANGE_STRIP_TRANSLATABLES_MESSAGE:
			case GraphicsStrip::CHANGE_STRIP_COLORABLES_MESSAGE:
			case GraphicsStrip::CHANGE_STRIP_SCALABLES_MESSAGE:

		case Plane::CHANGE_PLANE_NORMAL_MESSAGE:

		case Line::GET_LINE_ANGLE_Z_MESSAGE:
		case Line::GET_LINE_LENGTH_MESSAGE:
		case Line::GET_LINE_DIRECTION_MESSAGE:

		case Circle::GET_CIRCLE_LENGTH_MESSAGE:
		case Circle::GET_RADIUS_FROM_2_POINTS_MESSAGE:
		case Circle::GET_RADIUS_FROM_3_POINTS_MESSAGE:
		{
			return "SHAPE";
		}
		case AbstractSurface::CHANGE_INTERSECTION_MODE_MESSAGE:
		case AbstractSurface::CHECK_LINE_IN_SURFACE_MESSAGE:
			
		case TexturingSurface::CHECK_LINE_IN_IMAGE_MESSAGE:
			
		case PolygonalSurface::CHECK_LINE_IN_POLYGON_MESSAGE:
		case PolygonalSurface::CHANGE_HITBOX_MESSAGE:
		case PolygonalSurface::ENABLE_COLLISION_POLYGONS_INFO_MESSAGE:
		case PolygonalSurface::ENABLE_COLLISION_CHECKING_MESSAGE:
		case PolygonalSurface::CHANGE_COLLISION_GROUP_MESSAGE:
			case PolygonalSurface::CHECK_COLLISION_GROUP_GROUP_MESSAGE:
			case PolygonalSurface::CHECK_COLLISION_OBJECT_OBJECT_MESSAGE:
			case PolygonalSurface::CHECK_COLLISION_OBJECT_GROUP_MESSAGE:

		case Camera3D::CHECK_FRUSTUM_IN_QUAD_MESSAGE:
		case Camera3D::GET_SELECT_RAY_MESSAGE:
		{
			return "COLLISION";
		}
		case Keyboard::CREATE_KEYBOARD_EVENT_MESSAGE:

		case PlayerControl::ADD_INPUT_EVENT_MESSAGE:
		case PlayerControl::CLEAR_INPUT_EVENTS_MESSAGE:
		case PlayerControl::SET_ONE_INPUT_EVENT_MESSAGE:
		case PlayerControl::CREATE_PLAYER_CONTROL_MESSAGE:
		case PlayerControl::CHANGE_INPUT_TIMING_MESSAGE:
		{
			return "INPUT";
		}
		case Camera3D::PICK_CAMERA_MESSAGE:
		case Camera3D::CHANGE_PROJECTION_MODE_MESSAGE:
		case Camera3D::CHANGE_MOVEMENT_MODE_MESSAGE:
		case Camera3D::CHANGE_ROTATION_MODE_MESSAGE:
		case Camera3D::CHANGE_VIEWING_POINT_MESSAGE:
		case Camera3D::GET_VIEWING_POINT_MESSAGE:
		case Camera3D::CHANGE_DISTANCE_TO_VIEWING_POINT_MESSAGE:
		case Camera3D::CAMERA_LOOK_AT_MESSAGE: 
		case Camera3D::CHANGE_ZOOM_FAR_MESSAGE: 
		case Camera3D::CHANGE_ZOOM_NEAR_MESSAGE: 
		case Camera3D::CHANGE_ZOOM_SPEED_MESSAGE: 
		case Camera3D::ENABLE_ZOOM_MESSAGE: 
		case Camera3D::ENABLE_ROTATION_MESSAGE: 
		case Camera3D::CREATE_CAMERA_MESSAGE:
		case Camera3D::GET_CAMERA_LOOK_MESSAGE:
		case Camera3D::ENABLE_ROTATION_RANGE_MESSAGE:
		case Camera3D::CHANGE_ROTATION_RANGE_MESSAGE:

		case GraphicsContainer::CHANGE_CAMERA_MESSAGE:
		case GraphicsContainer::CHANGE_RENDER_TARGET_MESSAGE:

		case GraphicsObject::ENABLE_SHOW_MESSAGE:

		case ColorableObject::CHANGE_BACKGROUND_COLOR_MESSAGE:

		case Renderer::ENABLE_Z_BUFFER_MESSAGE:
		{  
			return "VIEW";
		}
		case FloatLerp::CHANGE_FLOAT_LERP_MODE_MESSAGE:
		case FloatLerp::CREATE_FLOAT_LERP_MESSAGE:
		case FloatLerp::CHANGE_FLOAT_LERP_SPEED_MESSAGE:
		case FloatLerp::CHANGE_FLOAT_LERP_T_MESSAGE:
		case FloatLerp::GET_FLOAT_LERP_T_MESSAGE:
		case FloatLerp::GET_FLOAT_LERP_CURRENT_MESSAGE:
		case FloatLerp::GET_FLOAT_LERP_DELTA_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_ALPHA_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_RED_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_GREEN_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_BLUE_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_HUE_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_SATURATION_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_LIGHTNESS_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_DELTA_SCALE_MESSAGE:
			case FloatLerp::STREAMING_FLOAT_T_ANIMATION_FRAME_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE:
			case FloatLerp::CHANGE_FLOAT_LERP_FLOAT_TARGET_MESSAGE:
			case FloatLerp::CHANGE_FLOAT_LERP_INTEGER_TARGET_MESSAGE:
			case QuaternionLerp::SET_DATA_TO_CONTROL_QUATERNION_MESSAGE:
			case QuaternionLerp::SET_DATA_TO_CONTROL_ROTATABLE_MESSAGE:
			case QuaternionLerp::GET_DATA_FROM_CONTROL_QUATERNION_MESSAGE:
			case QuaternionLerp::GET_DATA_FROM_CONTROL_ROTATABLE_MESSAGE:
		case QuaternionLerp::CREATE_QUATERNION_LERP_MESSAGE:
			case QuaternionLerp::STREAMING_QUATERNION_CURRENT_MESSAGE:
			case QuaternionLerp::STREAMING_ROTATABLE_CURRENT_MESSAGE:
			case QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE:
			case QuaternionLerp::STREAMING_ROTATABLE_DELTA_MESSAGE:
			case QuaternionLerp::SET_TWO_QUATERNIONS_MESSAGE:
			case QuaternionLerp::SET_TWO_ROTATABLES_MESSAGE:
			case QuaternionLerp::CHANGE_CONTROL_QUATERNIONS_MESSAGE:
			case QuaternionLerp::CHANGE_CONTROL_ROTATABLES_MESSAGE:
			case QuaternionLerp::GET_QUATERNION_LERP_T_MESSAGE:
			case QuaternionLerp::GET_ROTATABLE_LERP_T_MESSAGE:
		case QuaternionLerp::GET_QUATERNION_LERP_PATH_MESSAGE:
		case QuaternionLerp::GET_CONTROL_QUATERNIONS_MESSAGE:
			case VectorLerp::GET_VECTOR_LERP_T_MESSAGE:
			case VectorLerp::GET_TRANSLATABLE_LERP_T_MESSAGE:
			case VectorLerp::GET_SCALABLE_LERP_T_MESSAGE:
			case VectorLerp::GET_COLORABLE_LERP_T_MESSAGE:
			case VectorLerp::SET_DATA_TO_CONTROL_VECTOR_MESSAGE:
			case VectorLerp::SET_DATA_TO_CONTROL_TRANSLATABLE_MESSAGE:
			case VectorLerp::SET_DATA_TO_CONTROL_SCALABLE_MESSAGE:
			case VectorLerp::SET_DATA_TO_CONTROL_COLORABLE_MESSAGE:
			case VectorLerp::GET_DATA_FROM_CONTROL_VECTOR_MESSAGE:
			case VectorLerp::GET_DATA_FROM_CONTROL_TRANSLATABLE_MESSAGE:
			case VectorLerp::GET_DATA_FROM_CONTROL_SCALABLE_MESSAGE:
			case VectorLerp::GET_DATA_FROM_CONTROL_COLORABLE_MESSAGE:
		case VectorLerp::CREATE_VECTOR_LERP_MESSAGE:
			case VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE:
			case VectorLerp::STREAMING_TRANSLATABLE_CURRENT_MESSAGE:
			case VectorLerp::STREAMING_SCALABLE_CURRENT_MESSAGE:
			case VectorLerp::STREAMING_COLORABLE_CURRENT_MESSAGE:
			case VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE:
			case VectorLerp::STREAMING_TRANSLATABLE_DELTA_MESSAGE:
			case VectorLerp::STREAMING_SCALABLE_DELTA_MESSAGE:
			case VectorLerp::STREAMING_COLORABLE_DELTA_MESSAGE:
			case VectorLerp::CHANGE_CONTROL_VECTORS_MESSAGE:
			case VectorLerp::CHANGE_CONTROL_TRANSLATABLES_MESSAGE:
			case VectorLerp::CHANGE_CONTROL_SCALABLES_MESSAGE:
			case VectorLerp::CHANGE_CONTROL_COLORABLES_MESSAGE:
			case VectorLerp::SET_TWO_VECTORS_MESSAGE:
			case VectorLerp::SET_TWO_TRANSLATABLES_MESSAGE:
			case VectorLerp::SET_TWO_SCALABLES_MESSAGE:
			case VectorLerp::SET_TWO_COLORABLES_MESSAGE:
		case VectorLerp::CHANGE_VECTOR_LERP_MODE_MESSAGE:
		case VectorLerp::GET_VECTOR_LERP_PATH_MESSAGE:
		case VectorLerp::GET_CONTROL_VECTORS_MESSAGE:
		{
			return "INTERPOLATION";
		}
		case ActivatableObject::CHANGE_ACTIVATION_LIMIT_MESSAGE:
		case ActivatableObject::CHANGE_DEACTIVATION_MODE_MESSAGE:
		{ 
			return "PLAYBACK";
		}
		case PlayingObject::CHANGE_PLAYING_MODE_MESSAGE:
		case PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE:
		case PlayingObject::CHANGE_PLAYING_STATE_MESSAGE:
		case PlayingObject::REPLAY_MESSAGE:

		case AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE:
		case AbstractAnimation::GET_AMOUNT_OF_FRAMES_MESSAGE:
		{ 
			return "PLAYBACK";
		}
	}

	return "";
}




void ActionsLibraryTree::mouseMoveEvent(QMouseEvent *_event)
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
				
				if(item && item->GetItemType() == Item::ACTION_TYPE)
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


  
void ActionsLibraryTree::mousePressEvent(QMouseEvent* _event)
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


void ActionsLibraryTree::mouseDoubleClickEvent(QMouseEvent* _event)
{
	if(_event->button() == Qt::LeftButton)
	{  
		Item* it = dynamic_cast<Item*>(itemAt(cursorPoint));
	
		if(it && it->GetItemType() == Item::ACTION_TYPE)
		{
			EditAction();
		}
	}
	QTreeWidget::mouseDoubleClickEvent(_event);
}



int ActionsLibraryTree::GetAmountOfItems(QTreeWidgetItem* _it)
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