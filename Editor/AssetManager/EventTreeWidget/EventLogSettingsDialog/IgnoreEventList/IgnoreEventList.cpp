#include "IgnoreEventList.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Parser/List/AbstractList.h>
#include <Editor/TriggerCreator/ActionCreator/ActionCreator.h>
#include <Editor/TriggerCreator/TriggerCreator.h>




IgnoreEventList::IgnoreEventList(QWidget* _parent): QListWidget(_parent)
{
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	
	actionIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/action.png");
	eventIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/event.png");
	triggerIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/trigger.png");

	setIconSize(QSize(25, 25));

	CreateContextMenu();
}


void IgnoreEventList::CreateContextMenu(void)
{
	contextMenu = new QMenu(this);
	removeEventAct = new QAction("Remove", contextMenu);
    connect(removeEventAct, SIGNAL(triggered()), SLOT(RemoveSelectedItems()));
	contextMenu->addAction(removeEventAct);
}



void IgnoreEventList::RemoveSelectedItems(void)
{
	QList<QListWidgetItem*> selectedList = selectedItems();
	while(!selectedList.empty())
	{
		delete selectedList.at(0);
		selectedList.removeAt(0);
	}
}


QList<QPair<int, QString>> IgnoreEventList::GetIgnoreList(void)const
{
	QList<QPair<int, QString>> ignoreList;
	for(int i = 0; i < count(); i++)
	{
		QListWidgetItem* it = item(i);

		ignoreList.push_back(QPair<int, QString>(it->type(),it->text()));
	}
	return ignoreList;
}



QPixmap IgnoreEventList::GetIcon(int _assetType)
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



void IgnoreEventList::AddEvent(int _category, QString _name)
{ 
	QList<QListWidgetItem*> list = findItems(_name, Qt::MatchExactly);

	if(list.isEmpty())
	{
		QListWidgetItem* item = new QListWidgetItem(this, _category);
		item->setText(_name);
		item->setIcon(GetIcon(_category));
		addItem(item);
	} 
}


void IgnoreEventList::UpdateContextMenu(void)
{
	QListWidgetItem* it = itemAt(cursorPoint);
	contextMenu->removeAction(removeEventAct);  

	if(it)
	{
		contextMenu->addAction(removeEventAct);
	}
}


  
void IgnoreEventList::mousePressEvent(QMouseEvent* _event)
{
	QListWidget::mousePressEvent(_event);
	
	if(_event->button() == Qt::RightButton)
	{
		QPoint mousePos(QCursor::pos().x(), QCursor::pos().y()); 
		cursorPoint = _event->pos();

		UpdateContextMenu();

		contextMenu->move(mousePos);
		contextMenu->show();
	}
}
