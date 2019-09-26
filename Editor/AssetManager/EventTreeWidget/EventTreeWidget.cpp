#include "EventTreeWidget.h"
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
#include <Editor/AssetManager/EventTreeWidget/EventLogSettingsDialog/EventLogSettingsDialog.h>



EventTreeWidget::EventTreeWidget(QWidget* _parent): QTreeWidget(_parent), logShowMode(EventLogSettingsDialog::POSTPONE), triggersShowEnable(true), eventsShowEnable(true), actionsShowEnable(true)
{
	setAcceptDrops(false);
	setDragEnabled(false);
	setExpandsOnDoubleClick(false);
	setAnimated(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

	setColumnCount(MAX_COLUMNS);
	headerItem()->setText(ICON_PROPERTY, "");
	headerItem()->setText(TYPE_PROPERTY, "Type");
	headerItem()->setText(NAME_PROPERTY, "Name");
	headerItem()->setText(TIME_PROPERTY, "Time");
	QFont font("Verdana", 10, QFont::Bold, false);
	headerItem()->setFont(TYPE_PROPERTY, font);
	headerItem()->setFont(NAME_PROPERTY, font);
	headerItem()->setFont(TIME_PROPERTY, font);
	headerItem()->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
	headerItem()->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
	headerItem()->setTextAlignment(TIME_PROPERTY, Qt::AlignHCenter);
	setColumnWidth(ICON_PROPERTY, 80);
	setColumnWidth(TYPE_PROPERTY, 120);
	setColumnWidth(NAME_PROPERTY, 200);
	setColumnWidth(TIME_PROPERTY, 90);
    header()->setMovable(false);
	setStyleSheet("QTreeView::item { height: 16px; }"
		          "QTreeWidget::item:!selected { border: 2px solid gainsboro; "
		                                        "border-left: none; border-top: none; }"
		          "QTreeWidget::item:selected { background-color: rgb(149,199,233);"
                                               "border: 2px solid gainsboro;"
											   "border-left: none; border-top: none; }"); 

	triggerIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/trigger.png");
	eventIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/event.png");
	actionIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/action.png");
	warningIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/warning.png");

		
	CreateContextMenu();


	show();
}


void EventTreeWidget::mousePressEvent(QMouseEvent* _event)
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
}


void EventTreeWidget::UpdateContextMenu(void)
{
	QTreeWidgetItem* it = itemAt(cursorPoint);
	contextMenu->removeAction(ignoreAct);  
	contextMenu->removeAction(hideAct);  

	if(it)
	{
		contextMenu->addAction(ignoreAct);
		contextMenu->addAction(hideAct);

		ignoreAct->blockSignals(true);

		ignoreAct->setChecked(false);
		for(int i = 0; i < eventIgnoreList.size(); ++i)
		{
			if(eventIgnoreList.at(i).second == it->text(NAME_PROPERTY))
			{
				ignoreAct->setChecked(true);
				break;
			}
		}

		ignoreAct->blockSignals(false);
	}
}


int EventTreeWidget::GetLogShowMode(void)const
{
	return logShowMode;
}


void EventTreeWidget::CreateContextMenu(void)
{
	contextMenu = new QMenu(this);
	ignoreAct = new QAction("Ignore", contextMenu);
	ignoreAct->setCheckable(true);
	connect(ignoreAct, SIGNAL(triggered()), this, SLOT(EventIgnoreIsClicked()));
	hideAct = new QAction("Hide", contextMenu);
	connect(hideAct, SIGNAL(triggered()), this, SLOT(EventHideIsClicked()));
}


void EventTreeWidget::FindItem(QString _name, QList<QTreeWidgetItem*>& _list)
{
	FindItem(invisibleRootItem(), _name, _list);
}



void EventTreeWidget::FindItem(QTreeWidgetItem* _it, QString _name, QList<QTreeWidgetItem*>& _list)
{
	if(_it)
	{
		if(_it != invisibleRootItem())
		{
			if(_it->text(NAME_PROPERTY) == _name)
			{
				_list.push_back(_it);
			}
		}
		QTreeWidgetItem* search;
		int child = 0;

		while(_it->child(child))
		{
			FindItem(_it->child(child), _name, _list);
			child++;
		}
	}
}



void EventTreeWidget::EventIgnoreIsClicked(void)
{
	QTreeWidgetItem* it = itemAt(cursorPoint);
	
	if(it)
	{
		if(ignoreAct->isChecked())
		{  
			eventIgnoreList.push_back(QPair<int, QString>(it->type(), it->text(NAME_PROPERTY)));
			
			eventIgnoreTree.Add(it->text(NAME_PROPERTY));  
		}
		else
		{
			for(int i = 0; i < eventIgnoreList.size(); ++i)
			{
				if(eventIgnoreList.at(i).second == it->text(NAME_PROPERTY))
				{
					eventIgnoreList.removeAt(i);
					eventIgnoreTree.Remove(it->text(NAME_PROPERTY));
					break;
				}
			}
		}
	}
}


void EventTreeWidget::EventHideIsClicked(void)
{ 
	SimpleList<QString> selectedList;
	for(int i = 0; i < selectedItems().size(); i++)
	{
		selectedList.AddToTail(selectedItems().at(i)->text(NAME_PROPERTY));
	}

	selectedList.Unique();

	QList<QTreeWidgetItem*> deletionList;
	for(int i = 0; i < selectedList.GetSize(); ++i)
	{
		FindItem(selectedList.Get(i)->key, deletionList);
		while(!deletionList.isEmpty())
		{
			QTreeWidgetItem* it = deletionList.takeAt(0);

			if(it->isSelected())
			{
				it->setSelected(false);
			}
			delete it;
		}
	}
}



void EventTreeWidget::EventLogSettings(void)
{
	EventLogSettingsDialog dialog;
	dialog.SetLogShowMode(logShowMode);
	dialog.SetTriggersShowEnable(triggersShowEnable);
	dialog.SetEventsShowEnable(eventsShowEnable);
	dialog.SetActionsShowEnable(actionsShowEnable);
	dialog.SetIgnoreList(eventIgnoreList);
	if(dialog.exec() == QDialog::Accepted)
	{
		logShowMode = dialog.GetLogShowMode();
		eventIgnoreList = dialog.GetIgnoreList();
		triggersShowEnable = dialog.IsShowTriggersEnabled();
		actionsShowEnable = dialog.IsShowActionsEnabled();
		eventsShowEnable = dialog.IsShowEventsEnabled();
		
		eventIgnoreTree.Clear();
		for(QList<QPair<int, QString>>::iterator it = eventIgnoreList.begin(); it != eventIgnoreList.end(); ++it)
		{
			eventIgnoreTree.Add((*it).second);
		}
	}
}



void EventTreeWidget::ClearLogList(void)
{
	clear();

	eventStack.Clear();
}


QPixmap EventTreeWidget::GetIcon(int _assetType)
{
	switch(_assetType)
	{
		case Trigger::EventInfo::TRIGGER_CATEGORY:
		{
			return triggerIcon;
		}

		case Trigger::EventInfo::EVENT_CATEGORY:
		{
			return eventIcon;
		}

		case Trigger::EventInfo::ACTION_CATEGORY:
		{
			return actionIcon;
		}
	} 
	
	return QPixmap();
}



QString EventTreeWidget::GetStringCategory(int _category)
{
	switch(_category)
	{
		case Trigger::EventInfo::TRIGGER_CATEGORY:
		{
			return "TRIGGER";
		}

		case Trigger::EventInfo::EVENT_CATEGORY:
		{
			return "EVENT";
		}

		case Trigger::EventInfo::ACTION_CATEGORY:
		{
			return "ACTION";
		}
	} 
	
	return "";
}


QString EventTreeWidget::GetStringType(int _category, int _type)
{
	switch(_category)
	{
		case Trigger::EventInfo::TRIGGER_CATEGORY:
		{ 
			if(_type == -2) { return "WARNING"; } 
			else if(_type == -1) { return "UNLIMITED"; }  
			return "ITERATION: " + QString::number(_type);
		}

		case Trigger::EventInfo::EVENT_CATEGORY:
		{ 
			return AbstractEvent::_GetStringType(_type).c_str();
		}

		case Trigger::EventInfo::ACTION_CATEGORY:
		{
			return AbstractAction::_GetStringType(_type).c_str();
		}
	}
	return "";
}

 
void EventTreeWidget::AddEvent(int _category, int _type, QString _name, uint32 _time)
{
	if(_category == Trigger::EventInfo::ACTION_CATEGORY)
	{
		if(_type == Trigger::TRIGGER_FINISH_MESSAGE)
		{
			return;
		}
	}
	QTreeWidgetItem* item = new QTreeWidgetItem(_category); 
	item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable); 

	addTopLevelItem(item);
	QFont font("Verdana", 8, QFont::Normal, false);
	item->setFont(TYPE_PROPERTY, font);
	item->setFont(NAME_PROPERTY, font);
	item->setFont(TIME_PROPERTY, font);
	item->setIcon(ICON_PROPERTY, _type == -2 ? warningIcon : GetIcon(_category));
	item->setText(TYPE_PROPERTY, GetStringType(_category, _type));
	item->setText(NAME_PROPERTY, _name);
	item->setText(TIME_PROPERTY, HMS::_MsToHMS(_time).ToString().c_str());
	item->setTextAlignment(TYPE_PROPERTY, Qt::AlignHCenter);
	item->setTextAlignment(NAME_PROPERTY, Qt::AlignHCenter);
	item->setTextAlignment(TIME_PROPERTY, Qt::AlignHCenter);

	scrollTo(indexFromItem(item));
}



void EventTreeWidget::UpdateEventTree(void)
{  
	SimpleList<Trigger::EventInfo>::Node* it = Trigger::_GetEventLogList().GetHead();
	while(it)
	{
		if((it->key.category == Trigger::EventInfo::TRIGGER_CATEGORY && triggersShowEnable) ||
           (it->key.category == Trigger::EventInfo::EVENT_CATEGORY && eventsShowEnable) ||
		   (it->key.category == Trigger::EventInfo::ACTION_CATEGORY && actionsShowEnable))
		{
			if(eventIgnoreTree.Find(it->key.name.c_str()) == NIL) 
			{
				AddEvent(it->key.category, it->key.type, it->key.name.c_str(), it->key.time);
			}
		}
		it = it->GetRight();
	}

	Trigger::_GetEventLogList().Clear();
}
