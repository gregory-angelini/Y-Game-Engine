#include "EventsExistList.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Core/Trigger/Event/AbstractEvent.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Parser/List/AbstractList.h>
#include <Editor/TriggerCreator/EventCreator/EventCreator.h>
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Editor/TriggerCreator/VariableCreator/VariableCreator.h>




EventsExistList::EventsExistList(QWidget* _parent): QListWidget(_parent)
{
	eventIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/event.png");

	triggerIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/trigger.png");

	variableIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/variable.png");

	setIconSize(QSize(25, 25));

	CreateContextMenu();
}



void EventsExistList::AddEvent(QString _name)
{ 
	QList<QListWidgetItem*> list = findItems(_name, Qt::MatchExactly);

	if(list.isEmpty())
	{
		TriggerCreator::ListWidgetItemEx* item = new TriggerCreator::ListWidgetItemEx;
		item->SetType(TriggerCreator::ListWidgetItemEx::EVENT_TYPE);
		item->setText(_name);
		item->setIcon(eventIcon);
		addItem(item);
	}
}


void EventsExistList::AddTrigger(QString _name)
{ 
	QList<QListWidgetItem*> list = findItems(_name, Qt::MatchExactly);

	if(list.isEmpty())
	{
		TriggerCreator::ListWidgetItemEx* item = new TriggerCreator::ListWidgetItemEx;
		item->SetType(TriggerCreator::ListWidgetItemEx::TRIGGER_TYPE);
		item->setText(_name);
		item->setIcon(triggerIcon);
		addItem(item);
	}
}



void EventsExistList::AddVariable(QString _name)
{ 
	QList<QListWidgetItem*> list = findItems(_name, Qt::MatchExactly);

	if(list.isEmpty())
	{
		TriggerCreator::ListWidgetItemEx* item = new TriggerCreator::ListWidgetItemEx;
		item->SetType(TriggerCreator::ListWidgetItemEx::VARIABLE_TYPE);
		item->setText(_name);
		item->setIcon(variableIcon);
		addItem(item);
	}
}


void EventsExistList::CreateContextMenu(void)
{
	contextMenu = new QMenu(this);
	editEventAct = new QAction("Edit", contextMenu);
    connect(editEventAct, SIGNAL(triggered()), SLOT(EditEvent()));
}



void EventsExistList::UpdateContextMenu(void)
{
	QListWidgetItem* it = itemAt(cursorPoint);
	
	contextMenu->removeAction(editEventAct);

	if(it)
	{
		if(selectedItems().indexOf(it) != -1)
		{
			contextMenu->addAction(editEventAct);
		}
	}
}



void EventsExistList::EditEvent(void)
{
	QList<QListWidgetItem*> list = selectedItems();
	
	TriggerCreator::ListWidgetItemEx* it = dynamic_cast<TriggerCreator::ListWidgetItemEx*>(list.at(0));

	if(it)
	{
		if(it->GetType() == TriggerCreator::ListWidgetItemEx::EVENT_TYPE)
		{
			EventCreator* dialog = new EventCreator();

			dialog->SetEvent(it->text());

			dialog->setAttribute(Qt::WA_DeleteOnClose);

			dialog->show();

			connect(dialog, SIGNAL(EventIsEdited(QString)), this, SIGNAL(EventIsEdited(QString)));
		}
		else if(it->GetType() == TriggerCreator::ListWidgetItemEx::TRIGGER_TYPE)
		{
			TriggerCreator* dialog = new TriggerCreator();
	
			dialog->SetTrigger(it->text());

			dialog->setAttribute(Qt::WA_DeleteOnClose);

			dialog->show();

			connect(dialog, SIGNAL(TriggerIsEdited(QString)), this, SIGNAL(TriggerIsEdited(QString)));
		} 
		else if(it->GetType() == TriggerCreator::ListWidgetItemEx::VARIABLE_TYPE)
		{
			VariableCreator* dialog = new VariableCreator();

			dialog->SetVariable(it->text());

			dialog->setAttribute(Qt::WA_DeleteOnClose);

			dialog->show();

			connect(dialog, SIGNAL(VariableIsEdited(QString)), this, SIGNAL(VariableIsEdited(QString)));
		}
	}
}




void EventsExistList::mouseDoubleClickEvent(QMouseEvent* _event)
{
	if(_event->button() == Qt::LeftButton)
	{
		EditEvent();
	}
}

 
  
void EventsExistList::mousePressEvent(QMouseEvent* _event)
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
	else if(_event->button() == Qt::LeftButton)
	{
		cursorPoint = _event->pos();
		 
		QListWidgetItem* it = itemAt(cursorPoint);

		if(it == 0)
		{
			QList<QListWidgetItem*> list = selectedItems();

			QListWidgetItem* it = list.at(0);

			if(it)
			{
				if(it->isSelected())
				{
					it->setSelected(false);
				}
			}
		}
	}
}
