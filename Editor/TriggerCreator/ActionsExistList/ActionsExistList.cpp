#include "ActionsExistList.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Parser/List/AbstractList.h>
#include <Editor/TriggerCreator/ActionCreator/ActionCreator.h>
#include <Editor/TriggerCreator/TriggerCreator.h>




ActionsExistList::ActionsExistList(QWidget* _parent): QListWidget(_parent)
{
	actionIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/action.png");

	setIconSize(QSize(25, 25));

	CreateContextMenu();
}



void ActionsExistList::AddAction(QString _name)
{ 
	QList<QListWidgetItem*> list = findItems(_name, Qt::MatchExactly);

	if(list.isEmpty())
	{
		TriggerCreator::ListWidgetItemEx* item = new TriggerCreator::ListWidgetItemEx;
		item->SetType(TriggerCreator::ListWidgetItemEx::ACTION_TYPE);
		item->setText(_name);
		item->setIcon(actionIcon);
		addItem(item);
	}
}


void ActionsExistList::CreateContextMenu(void)
{
	contextMenu = new QMenu(this);
	editActionAct = new QAction("Edit", contextMenu);
    connect(editActionAct, SIGNAL(triggered()), SLOT(EditAction()));
}



void ActionsExistList::UpdateContextMenu(void)
{ 
	QListWidgetItem* it = itemAt(cursorPoint);
	
	contextMenu->removeAction(editActionAct);

	if(it)
	{
		if(selectedItems().indexOf(it) != -1)
		{
			contextMenu->addAction(editActionAct);
		}
	}
}



void ActionsExistList::EditAction(void)
{
	QList<QListWidgetItem*> list = selectedItems();
	
	TriggerCreator::ListWidgetItemEx* it = dynamic_cast<TriggerCreator::ListWidgetItemEx*>(list.at(0));

	if(it)
	{
		ActionCreator* dialog = new ActionCreator();

		dialog->SetAction(list.at(0)->text());

		dialog->setAttribute(Qt::WA_DeleteOnClose);

		dialog->show();

		connect(dialog, SIGNAL(ActionIsEdited(QString)), this, SIGNAL(ActionIsEdited(QString)));
	}
}



void ActionsExistList::mouseDoubleClickEvent(QMouseEvent* _event)
{
	if(_event->button() == Qt::LeftButton)
	{
		EditAction();
	}
}


  
void ActionsExistList::mousePressEvent(QMouseEvent* _event)
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
