#include "EventsLibraryTree.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Core/Trigger/Event/AbstractEvent.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Parser/List/AbstractList.h>
#include <Editor/TriggerCreator/EventCreator/EventCreator.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Engine/Output/Graphics/Surface/GraphicsSurface/GraphicsSurface.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Editor/TriggerCreator/VariableCreator/VariableCreator.h>
#include <Engine/Core/Time/Timer/Timer.h>



EventsLibraryTree::EventsLibraryTree(QWidget* _parent): QTreeWidget(_parent), lastEvent(NIL), lastCategory(NIL), lastGroup(NIL)
{
	setSortingEnabled(true);
	selectedBeginColor = QColor(165,165,165);
	categoryBeginColor = QColor(220,220,220);
	groupBeginColor = QColor(220,220,220);
	variablesBeginColor = QColor(220,220,220);
	triggersBeginColor = QColor(220,220,220);
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
	eventIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/event.png");
	triggerIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/trigger.png");
	variableIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/variable.png");


	Refresh();
}



EventsLibraryTree::~EventsLibraryTree(void)
{}

 

void EventsLibraryTree::UpdateContextMenu(void)
{
	QTreeWidgetItem* it = itemAt(cursorPoint);
	contextMenu->removeAction(editEventAct);  
	contextMenu->removeAction(removeItemAct);

	contextMenu->removeAction(refreshAct);
	

	if(it)
	{
		if(it->type() == Item::EVENT_TYPE ||
		   it->type() == Item::TRIGGER_TYPE ||
		   it->type() == Item::VARIABLE_TYPE)
		{
			contextMenu->addAction(editEventAct);
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


void EventsLibraryTree::CreateContextMenu(void)
{
	contextMenu = new QMenu(this);
	editEventAct = new QAction("Edit", contextMenu);
    connect(editEventAct, SIGNAL(triggered()), SLOT(EditItem()));
	removeItemAct = new QAction("Remove", contextMenu);
	connect(removeItemAct, SIGNAL(triggered()), SLOT(RemoveSelectedItems()));
	refreshAct = new QAction("Refresh", contextMenu);
	connect(refreshAct, SIGNAL(triggered()), SLOT(RefreshEventTree()));
}



void EventsLibraryTree::RemoveSelectedItems(void)
{
	WarningDialog warning("Warning", "Selected events will be deleted. Apply?", 270, 90);
		
	if(warning.exec() == QDialog::Rejected)
	{
		return;
	}
	while(!selectedItems().isEmpty())
	{
		Item* it = dynamic_cast<Item*>(FindLeaf(selectedItems()));
		
		if(it)
		{
			if(it->GetItemType() == Item::EVENT_TYPE)
			{ 
				Item* parent = dynamic_cast<Item*>(it->parent());
					
				QString name = it->GetName();
				
				File::_Remove(AssetLibrary::_GetDir() + AbstractEvent::_GetDir() + name.toAscii().data() + "." + AbstractEvent::_GetFileSuffix());
			
				delete it;

				emit EventIsEdited(name);

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
			else if(it->GetItemType() == Item::TRIGGER_TYPE)
			{
				Item* parent = dynamic_cast<Item*>(it->parent());
					
				QString name = it->GetName();
				
				File::_Remove(AssetLibrary::_GetDir() + Trigger::_GetDir() + name.toAscii().data() + "." + Trigger::_GetFileSuffix());
			
				delete it;

				emit TriggerIsEdited(name);

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
			else if(it->GetItemType() == Item::VARIABLE_TYPE)
			{ 
				Item* parent = dynamic_cast<Item*>(it->parent());
					
				QString name = it->GetName();
				
				File::_Remove(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + name.toAscii().data() + "." + AbstractVariable::_GetFileSuffix());
			
				delete it;

				emit VariableIsEdited(name);

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



void EventsLibraryTree::EditItem(void)
{
	Item* it = dynamic_cast<Item*>(currentItem());
	
	if(it)
	{
		if(it->GetItemType() == Item::EVENT_TYPE)
		{
			EditEvent();
		}
		else if(it->GetItemType() == Item::TRIGGER_TYPE)
		{
			EditTrigger();
		}
		else if(it->GetItemType() == Item::VARIABLE_TYPE)
		{
			EditVariable();
		}
	} 
}


void EventsLibraryTree::EditEvent(void)
{
	Item* it = dynamic_cast<Item*>(currentItem());
	
	if(it)
	{
		if(it->GetItemType() == Item::EVENT_TYPE)
		{
			EventCreator* dialog = new EventCreator();

			dialog->SetEvent(it->GetName());

			dialog->setAttribute(Qt::WA_DeleteOnClose);

			dialog->show();

			connect(dialog, SIGNAL(EventIsEdited(QString)), this, SIGNAL(EventIsEdited(QString)));
		}
	}
}




void EventsLibraryTree::EditTrigger(void)
{
	Item* it = dynamic_cast<Item*>(currentItem());
	
	if(it)
	{
		if(it->GetItemType() == Item::TRIGGER_TYPE)
		{
			TriggerCreator* dialog = new TriggerCreator();

			dialog->SetTrigger(it->GetName());

			dialog->setAttribute(Qt::WA_DeleteOnClose);

			dialog->show();
		}
	}
}



void EventsLibraryTree::EditVariable(void)
{
	Item* it = dynamic_cast<Item*>(currentItem());
	
	if(it)
	{
		if(it->GetItemType() == Item::VARIABLE_TYPE)
		{
			VariableCreator* dialog = new VariableCreator();

			dialog->SetVariable(it->GetName());

			dialog->setAttribute(Qt::WA_DeleteOnClose);

			dialog->show();

			connect(dialog, SIGNAL(VariableIsEdited(QString)), this, SIGNAL(VariableIsEdited(QString)));
		}
	}
}


QTreeWidgetItem* EventsLibraryTree::FindLeaf(QList<QTreeWidgetItem*>& _list)
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



void EventsLibraryTree::RefreshEventTree(void)
{  
	Refresh();
}



void EventsLibraryTree::Refresh(void)
{
	for(int i = 0; i < topLevelItemCount(); i++)
	{ 
		Item* it = dynamic_cast<Item*>(topLevelItem(i));

		if(it && it->GetItemType() == Item::CATEGORY_TYPE)
		{ 
			if(it->GetEventType() == TRIGGER_CATEGORY)
			{
				while(it->child(0))
				{
					delete it->child(0);
				}
			}
			else if(it->GetEventType() == VARIABLE_CATEGORY)
			{
				while(it->child(0))
				{
					delete it->child(0);
				}
			}
			else
			{
				for(int j = 0; j < it->childCount(); j++)
				{
					Item* child = dynamic_cast<Item*>(it->child(j));
					
					if(child)
					{
						if(child->GetItemType() == Item::GROUP_TYPE)
						{ 
							child->RemoveChildItems();
						} 
					}
				}
			}
		} 
	}

	UpdateEventsTree();
	UpdateTriggersTree();
	UpdateVariablesTree();
}

 


void EventsLibraryTree::UpdateTriggersTree(void)
{
	QString path = QString(AssetLibrary::_GetDir().c_str()) + QString(Trigger::_GetDir().c_str());
	
	QDir dir(path);

	QFileInfoList triggers = dir.entryInfoList(QStringList() << "*." + QString(Trigger::_GetFileSuffix().c_str()), QDir::Files);

	Trigger* trigger;
	for(int i = 0; i < triggers.size(); i++)
	{
		StringANSI path = AssetLibrary::_GetDir() + Trigger::_GetDir() + triggers.at(i).completeBaseName().toStdString() + "." + Trigger::_GetFileSuffix();

		File file(path, File::READ);

		if(file.IsExist())
		{
			AddTrigger(triggers.at(i).completeBaseName());
		}
	}
	SimpleList<Item*> removeList;
	for(int i = 0; i < topLevelItemCount(); i++)
	{ 
		Item* it = dynamic_cast<Item*>(topLevelItem(i));

		if(it && it->GetItemType() == Item::CATEGORY_TYPE)
		{ 
			if(it->GetEventType() == TRIGGER_CATEGORY)
			{
				if(it->IsEmpty())
				{
					removeList.AddToTail(it);
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
}


void EventsLibraryTree::UpdateEventsTree(void)
{
	QString path = QString(AssetLibrary::_GetDir().c_str()) + QString(AbstractEvent::_GetDir().c_str());
	
	QDir dir(path);

	QFileInfoList events = dir.entryInfoList(QStringList() << "*." + QString(AbstractEvent::_GetFileSuffix().c_str()), QDir::Files);
	for(int i = 0; i < events.size(); i++)
	{
		StringANSI path = AssetLibrary::_GetDir() + AbstractEvent::_GetDir() + events.at(i).completeBaseName().toStdString() + "." + AbstractEvent::_GetFileSuffix();

		File file(path, File::READ);

		if(file.IsOpened() && file.GetAccessMode() == File::READ)
		{
			int32 type;
			file.Read(type);

			AddEvent(events.at(i).completeBaseName(), type);
		}
	}
	SimpleList<Item*> removeList;

	for(int i = 0; i < topLevelItemCount(); i++)
	{ 
		Item* it = dynamic_cast<Item*>(topLevelItem(i));

		if(it && it->GetItemType() == Item::CATEGORY_TYPE)
		{ 
			if(it->GetEventType() != TRIGGER_CATEGORY && 
			   it->GetEventType() != VARIABLE_CATEGORY)
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
			if(it->GetEventType() != TRIGGER_CATEGORY && 
			   it->GetEventType() != VARIABLE_CATEGORY)
			{
				if(it->IsEmpty())
				{
					removeList.AddToTail(it);
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
}



void EventsLibraryTree::AddVariable(QString _name)
{
	Item* it = FindVariable(_name);

	if(it == NIL)
	{
		Item* category = FindCategory("VARIABLES");
		if(category == NIL)
		{
			category = new Item(invisibleRootItem(), VARIABLE_CATEGORY, Item::CATEGORY_TYPE);
			category->setIcon(ICON_PROPERTY, folderIcon); 

			category->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			category->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
			category->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
			QFont font1("Verdana", 8, QFont::Bold, false);
			category->setFont(NAME_PROPERTY, font1);
			QFont font2("Verdana", 8, QFont::Normal, false);
			category->setFont(TYPE_PROPERTY, font2);

			category->SetName("VARIABLES");
			category->setText(TYPE_PROPERTY, "CATEGORY");
		}
		Item* it = new Item(category, 0, Item::VARIABLE_TYPE);
		it->setIcon(ICON_PROPERTY, variableIcon); 

		it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		it->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
		it->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
		QFont font("Verdana", 8, QFont::Normal, false);
		it->setFont(NAME_PROPERTY, font);
		it->setFont(TYPE_PROPERTY, font);

		it->SetName(_name);
		it->setText(TYPE_PROPERTY, "VARIABLE");

		category->setText(NAME_PROPERTY, category->GetName() + "(" + QString::number(GetAmountOfItems(category)) + ")");
	}
}



void EventsLibraryTree::AddTrigger(QString _name)
{
	Item* it = FindTrigger(_name);

	if(it == NIL)
	{
		Item* category = FindCategory("TRIGGERS");
		if(category == NIL)
		{
			category = new Item(invisibleRootItem(), TRIGGER_CATEGORY, Item::CATEGORY_TYPE);
			category->setIcon(ICON_PROPERTY, folderIcon);

			category->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			category->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
			category->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
			QFont font1("Verdana", 8, QFont::Bold, false);
			category->setFont(NAME_PROPERTY, font1);
			QFont font2("Verdana", 8, QFont::Normal, false);
			category->setFont(TYPE_PROPERTY, font2);

			category->SetName("TRIGGERS");
			category->setText(TYPE_PROPERTY, "CATEGORY");
		}
		Item* it = new Item(category, 0, Item::TRIGGER_TYPE);
		it->setIcon(ICON_PROPERTY, triggerIcon);

		it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		it->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
		it->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
		QFont font("Verdana", 8, QFont::Normal, false);
		it->setFont(NAME_PROPERTY, font);
		it->setFont(TYPE_PROPERTY, font);

		it->SetName(_name);
		it->setText(TYPE_PROPERTY, "TRIGGER");

		category->setText(NAME_PROPERTY, category->GetName() + "(" + QString::number(GetAmountOfItems(category)) + ")");
	}
}



void EventsLibraryTree::UpdateVariablesTree(void)
{
	QString path = QString(AssetLibrary::_GetDir().c_str()) + QString(AbstractVariable::_GetDir().c_str());
	
	QDir dir(path);

	QFileInfoList variables = dir.entryInfoList(QStringList() << "*." + QString(AbstractVariable::_GetFileSuffix().c_str()), QDir::Files);

	AbstractVariable* variable;
	for(int i = 0; i < variables.size(); i++)
	{ 
		AbstractVariable* variable = AbstractVariable::_LoadFromFile(variables.at(i).completeBaseName().toStdString());
				
		if(variable)
		{
			if(variable->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				AddVariable(variables.at(i).completeBaseName());
			}
			delete variable;
		}
	}
	SimpleList<Item*> removeList;
	for(int i = 0; i < topLevelItemCount(); i++)
	{ 
		Item* it = dynamic_cast<Item*>(topLevelItem(i));

		if(it && it->GetItemType() == Item::CATEGORY_TYPE)
		{ 
			if(it->GetEventType() == VARIABLE_CATEGORY)
			{
				if(it->IsEmpty())
				{
					removeList.AddToTail(it);
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
}



void EventsLibraryTree::Refresh(QString _name)
{
	StringANSI path = AssetLibrary::_GetDir() + AbstractEvent::_GetDir() + _name.toStdString() + "." + AbstractEvent::_GetFileSuffix();

	File file(path, File::READ);

	if(file.IsOpened())
	{
		if(file.GetAccessMode() == File::READ)
		{
			int32 type;
			file.Read(type);

			AddEvent(_name, type);
		}
	}
	else
	{
		delete FindEvent(_name);
	}
}


EventsLibraryTree::Item* EventsLibraryTree::FindTrigger(QString _name)
{
	return FindTrigger(invisibleRootItem(), _name);
}


EventsLibraryTree::Item* EventsLibraryTree::FindTrigger(QTreeWidgetItem* _it, QString _name)
{
	if(_it)
	{
		int child = 0;

		if(_it != invisibleRootItem())
		{
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::TRIGGER_TYPE)
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
			if(search = FindTrigger(_it->child(child), _name))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}



EventsLibraryTree::Item* EventsLibraryTree::FindVariable(QString _name)
{
	return FindVariable(invisibleRootItem(), _name);
}


EventsLibraryTree::Item* EventsLibraryTree::FindVariable(QTreeWidgetItem* _it, QString _name)
{
	if(_it)
	{
		int child = 0;

		if(_it != invisibleRootItem())
		{
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::VARIABLE_TYPE)
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
			if(search = FindVariable(_it->child(child), _name))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}




void EventsLibraryTree::RefreshEvent(QString _name)
{
	AbstractEvent* event;

	if(event = AbstractEvent::_LoadFromFile(_name.toAscii().data()))
	{
		AddEvent(event->GetName().c_str(), event->GetType());
		delete event;
	}
	else
	{
		delete FindEvent(_name);
	}
}



void EventsLibraryTree::RefreshTrigger(QString _name)
{
	if(File::_IsExist(AssetLibrary::_GetDir() + Trigger::_GetDir() + StringANSI(_name.toAscii().data()) + "." + Trigger::_GetFileSuffix()))
	{
		AddTrigger(_name);
	}
	else
	{
		delete FindTrigger(_name);
	}
}



void EventsLibraryTree::RefreshVariable(QString _name)
{
	AbstractVariable* variable = AbstractVariable::_LoadFromFile(_name.toAscii().data());

	if(variable && variable->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
	{
		AddVariable(_name);
	} 
	else
	{
		delete FindVariable(_name);
	}

	delete variable;
}





void EventsLibraryTree::AddEvent(QString _name, int _eventType)
{
	Item* it = FindEvent(_name, _eventType);

	if(it == NIL)
	{
		Item* group = FindGroup(_eventType);
		Item* category = FindCategory(_eventType);
		if(group == NIL)
		{
			if(category == NIL)
			{
				category = AddCategory(GetCategoryName(_eventType), _eventType);
			}
			
			group = AddGroup(category, GetGroupName(_eventType), _eventType);
		}
		it = new Item(group, _eventType, Item::EVENT_TYPE);
		it->setIcon(ICON_PROPERTY, eventIcon); 

		it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		it->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
		it->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);

		lastEvent = it;
		lastGroup = group;
		lastCategory = category;
		QFont font("Verdana", 8, QFont::Normal, false);
		it->setFont(NAME_PROPERTY, font);
		it->setFont(TYPE_PROPERTY, font);

		it->SetName(_name);
		it->setText(TYPE_PROPERTY, "EVENT");

				
		category->setText(NAME_PROPERTY, category->GetName() + "(" + QString::number(GetAmountOfItems(category)) + ")");
		group->setText(NAME_PROPERTY, group->GetName() + "(" + QString::number(GetAmountOfItems(group)) + ")");
	}
}




EventsLibraryTree::Item* EventsLibraryTree::FindEvent(QString _name, int _eventType)
{
	return FindEvent(invisibleRootItem(), _name, _eventType);
}



EventsLibraryTree::Item* EventsLibraryTree::FindEvent(QTreeWidgetItem* _it, QString _name, int _eventType)
{
	if(_it)
	{
		if(_it != invisibleRootItem())
		{
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::EVENT_TYPE)
			{
				if(item->GetEventType() == _eventType)
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
			if(search = FindEvent(_it->child(child), _name, _eventType))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}

 

 
EventsLibraryTree::Item* EventsLibraryTree::FindEvent(QString _name)
{
	return FindEvent(invisibleRootItem(), _name);
}


EventsLibraryTree::Item* EventsLibraryTree::FindEvent(QTreeWidgetItem* _it, QString _name)
{
	if(_it)
	{
		int child = 0;

		if(_it != invisibleRootItem())
		{
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::EVENT_TYPE)
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
			if(search = FindEvent(_it->child(child), _name))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}



EventsLibraryTree::Item* EventsLibraryTree::AddCategory(QString _name, int _eventType)
{
	Item* it = new Item(invisibleRootItem(), _eventType, Item::CATEGORY_TYPE);
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



EventsLibraryTree::Item* EventsLibraryTree::FindCategory(QString _name)
{
	return FindCategory(invisibleRootItem(), _name);
}


EventsLibraryTree::Item* EventsLibraryTree::FindCategory(QTreeWidgetItem* _it, QString _name)
{
	if(_it)
	{
		int child = 0;

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


EventsLibraryTree::Item* EventsLibraryTree::FindCategory(int _eventType)
{
	return FindCategory(invisibleRootItem(), _eventType);
}



EventsLibraryTree::Item* EventsLibraryTree::FindCategory(QTreeWidgetItem* _it, int _eventType)
{
	if(_it)
	{
		int child = 0;

		if(_it != invisibleRootItem())
		{ 
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::CATEGORY_TYPE)
			{
				if(GetCategoryName(item->GetEventType()) == GetCategoryName(_eventType))
				{
					return item;
				}
			} 
		}
		
		Item* search;

		while(_it->child(child))
		{
			if(search = FindCategory(_it->child(child), _eventType))
			{
				return search;
			}
			child++;
		}
	}
	return NIL;
}



EventsLibraryTree::Item* EventsLibraryTree::FindGroup(int _eventType)
{
	return FindGroup(invisibleRootItem(), _eventType);
}


EventsLibraryTree::Item* EventsLibraryTree::FindGroup(QTreeWidgetItem* _it, int _actionType)
{
	if(_it)
	{
		int child = 0;

		if(_it != invisibleRootItem())
		{ 
			Item* item = dynamic_cast<Item*>(_it);

			if(item->GetItemType() == Item::GROUP_TYPE)
			{
				if(GetGroupName(item->GetEventType()) == GetGroupName(_actionType) && GetCategoryName(item->GetEventType()) == GetCategoryName(_actionType))
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


 

EventsLibraryTree::Item* EventsLibraryTree::AddGroup(Item* _parent, QString _name, int _eventType)
{
	Item* it = new Item(_parent, _eventType, Item::GROUP_TYPE);
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





void EventsLibraryTree::drawRow(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index)const
{
	QStyleOptionViewItem newOption(_option);

	Item* it = dynamic_cast<Item*>(itemFromIndex(_index));

	if(it)
	{
		if(it->GetItemType() == Item::CATEGORY_TYPE)
		{
			if(it->GetEventType() == TRIGGER_CATEGORY)
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
					grad.setColorAt(0, triggersBeginColor); 
					grad.setColorAt(1, QColor(255,255,255));
				}
		
				QBrush brush(grad);
				_painter->fillRect(_option.rect, brush);
				QTreeWidget::drawRow(_painter, _option, _index);
				return;
			}
			else if(it->GetEventType() == VARIABLE_CATEGORY)
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
					grad.setColorAt(0, variablesBeginColor); 
					grad.setColorAt(1, QColor(255,255,255));
				}
		
				QBrush brush(grad);
				_painter->fillRect(_option.rect, brush);
				QTreeWidget::drawRow(_painter, _option, _index);
				return;
			}
			else
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
			if(it == lastEvent)
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



void EventsLibraryTree::keyPressEvent(QKeyEvent* _event)
{
	if(!_event->isAutoRepeat())
	{
		if(_event->key() == Qt::Key_Delete)
		{
			RemoveSelectedItems();
		}
		else if(_event->key() == Qt::Key_Return)
		{
			EditItem();
		}
	}
	
	QTreeWidget::keyPressEvent(_event);
}



QString EventsLibraryTree::GetGroupName(int _eventType)
{ 
	switch(_eventType)
	{
		case MessageHandler<int32>::SEND_MESSAGE:

		case AbstractObject::CHANGE_NAME_MESSAGE:
		case AbstractObject::DESTROY_OBJECT_MESSAGE:
		case AbstractObject::ENABLE_PROPERTY_MESSAGE:

		case AssetLibrary::CREATE_OBJECT_MESSAGE:
		{
			return "ABSTRACT_OBJECT";
		}
		case Timer::TIMER_FINISH_MESSAGE:
		{
			return "TIMER";
		}
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
		case FloatLerp::FLOAT_LERP_FINISH_MESSAGE:
		case FloatLerp::FLOAT_LERP_START_MESSAGE:
		{
			return "FLOAT_LERP";
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
		case GraphicsContainer::CHANGE_CAMERA_MESSAGE:
		{
			return "GRAPHICS_CONTAINER";
		}
		case TexturingSurface::CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE:
		case TexturingSurface::CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE: 
		case TexturingSurface::CHANGE_FRONT_TEXTURE_MESSAGE:
		case TexturingSurface::CHANGE_BACK_TEXTURE_MESSAGE: 
		{
			return "TEXTURING_SURFACE";
		}
		case PolygonalSurface::CHANGE_MESH_MESSAGE:
		case PolygonalSurface::CHANGE_HITBOX_MESSAGE:
		case PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE:
		{
			return "POLYGONAL_SURFACE";
		}
		case AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE:
		{
			return "ABSTRACT_ANIMATION";
		}
		case TexturedAnimation::CHANGE_ATLAS_MESSAGE:
		case TexturedAnimation::ENABLE_POSITION_OFFSET_MESSAGE:
		{
			return "TEXTURED_ANIMATION";
		}
		case PlayerControl::PLAYER_CONTROL_FINISH_MESSAGE:
		{
			return "PLAYER_CONTROL";
		}
		case Trigger::TRIGGER_FINISH_MESSAGE:
		{
			return "TRIGGER";
		}
		case Camera3D::PICK_CAMERA_MESSAGE:
		{
			return "CAMERA";
		}
		case LerpingObject::CHANGE_FLOAT_LERP_MESSAGE:
		{
			return "LERPING_OBJECT";
		}
		case PlayingObject::CHANGE_PLAYING_STATE_MESSAGE:
		case PlayingObject::CHANGE_PLAYING_MODE_MESSAGE:
		{
			return "PLAYING_OBJECT";
		}
	}

	return "";
}





QString EventsLibraryTree::GetCategoryName(int _eventType)
{ 
	switch(_eventType)
	{
		case MessageHandler<int32>::SEND_MESSAGE:

		case AbstractObject::CHANGE_NAME_MESSAGE:
		case AbstractObject::DESTROY_OBJECT_MESSAGE:
		case AbstractObject::ENABLE_PROPERTY_MESSAGE:

		case AssetLibrary::CREATE_OBJECT_MESSAGE:
		{
			return "BASIC";
		}
		case Timer::TIMER_FINISH_MESSAGE:
		{
			return "TIME";
		}
		case FloatLerp::FLOAT_LERP_FINISH_MESSAGE:
		case FloatLerp::FLOAT_LERP_START_MESSAGE:
		{
			return "INTERPOLATION";
		}
		case Scene::PICK_SCENE_MESSAGE:

		case ObjectScene::ADD_TO_LAYER_MESSAGE:
		case ObjectScene::REMOVE_FROM_LAYER_MESSAGE:

		case LogicScene::ADD_TO_UPDATE_LIST_MESSAGE:
		case LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE:

		case GraphicsScene::ADD_TO_RENDER_LIST_MESSAGE:
		case GraphicsScene::REMOVE_FROM_RENDER_LIST_MESSAGE:
			case AbstractVariable::CHANGE_INTEGER_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_FLOAT_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_STRING_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_BOOLEAN_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_VECTOR_2_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_VECTOR_3_VALUE_MESSAGE:
			case AbstractVariable::CHANGE_QUATERNION_VALUE_MESSAGE:
		{
			return "CONTAINER";
		}
		case GraphicsContainer::CHANGE_CAMERA_MESSAGE:
					
		case Camera3D::PICK_CAMERA_MESSAGE:
		{
			return "VIEW";
		}
		case TexturedAnimation::ENABLE_POSITION_OFFSET_MESSAGE:
					
		case AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE:
					
		case LerpingObject::CHANGE_FLOAT_LERP_MESSAGE:
		{
			return "TRANSFORMATION";
		}
		case TexturingSurface::CHANGE_FRONT_TEXTURE_MESSAGE:
		case TexturingSurface::CHANGE_BACK_TEXTURE_MESSAGE:

		case TexturingSurface::CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE:
		case TexturingSurface::CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE:

		case TexturedAnimation::CHANGE_ATLAS_MESSAGE:
		{
			return "MATERIAL";
		}
		case PlayerControl::PLAYER_CONTROL_FINISH_MESSAGE:
		{
			return "INPUT";
		}
		case Trigger::TRIGGER_FINISH_MESSAGE:
		{
			return "SCRIPT";
		}
		case PolygonalSurface::CHANGE_MESH_MESSAGE:
		{
			return "SHAPE";
		}
		case PolygonalSurface::CHANGE_HITBOX_MESSAGE:
		case PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE:
		{
			return "COLLISION";
		}
		case PlayingObject::CHANGE_PLAYING_STATE_MESSAGE:
		case PlayingObject::CHANGE_PLAYING_MODE_MESSAGE:
		{
			return "PLAYBACK";
		}
	}

	return "";
}



void EventsLibraryTree::mouseMoveEvent(QMouseEvent *_event)
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
				
				if(item)
				{
					if(item->GetItemType() == Item::EVENT_TYPE ||
				       item->GetItemType() == Item::TRIGGER_TYPE ||
					   item->GetItemType() == Item::VARIABLE_TYPE)
					{
						list.append(QUrl(item->GetName()));
					}
				}
			}

			mimeData->setUrls(list);
			drag->setMimeData(mimeData);
			drag->start(Qt::CopyAction);
		}
	}
}



void EventsLibraryTree::mousePressEvent(QMouseEvent* _event)
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


void EventsLibraryTree::mouseDoubleClickEvent(QMouseEvent* _event)
{
	if(_event->button() == Qt::LeftButton)
	{
		Item* it = dynamic_cast<Item*>(itemAt(cursorPoint));
	
		if(it)
		{
			if(it->GetItemType() == Item::EVENT_TYPE)
			{
				EditEvent();
			}
			else if(it->GetItemType() == Item::TRIGGER_TYPE)
			{
				EditTrigger();
			}
			else if(it->GetItemType() == Item::VARIABLE_TYPE)
			{
				EditVariable();
			}
		} 
	}
	QTreeWidget::mouseDoubleClickEvent(_event);
}


int EventsLibraryTree::GetAmountOfItems(QTreeWidgetItem* _it)
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
