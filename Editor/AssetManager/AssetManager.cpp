#include "AssetManager.h"
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
#include <Editor/AssetManager/EventTreeWidget/EventLogSettingsDialog/EventLogSettingsDialog.h>




AssetManager::AssetManager(QWidget* _parent): QTabWidget(_parent)
{
	objectTreeWidget = new ObjectTreeWidget(this);
	addTab(objectTreeWidget, "LAYER TREE");
	drawTreeWidget = new GraphicsTreeWidget(this);
	addTab(drawTreeWidget, "RENDER TREE");
	updateTreeWidget = new LogicTreeWidget(this);
	addTab(updateTreeWidget, "UPDATE TREE");
	eventTreeWidget = new EventTreeWidget(this);
	addTab(eventTreeWidget, "EVENT LOG");

	tabBar()->installEventFilter(this);


	CreateContextMenu();


	show();
}


bool AssetManager::eventFilter(QObject* _object, QEvent* _event)
{
	if(_event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* keyEvent = static_cast<QMouseEvent*>(_event);
	
		if(keyEvent->button() == Qt::RightButton)
		{		
			setCurrentIndex(tabBar()->tabAt(keyEvent->pos()));
			
			if(currentWidget() == eventTreeWidget)
			{
				ShowEventLogContextMenu(QPoint(QCursor::pos().x(), QCursor::pos().y()));
			}
		}
	}
	return false;
}



void AssetManager::CreateContextMenu(void)
{
	contextMenu = new QMenu(this);
	eventLogSettingsAct = new QAction("Settings", contextMenu);
	connect(eventLogSettingsAct, SIGNAL(triggered()), eventTreeWidget, SLOT(EventLogSettings()));
	eventLogClearAct = new QAction("Clear", contextMenu);
	connect(eventLogClearAct, SIGNAL(triggered()), eventTreeWidget, SLOT(ClearLogList()));
}


bool AssetManager::IsEventLogPostponeShowModeEnabled(void)const
{
	return eventTreeWidget->GetLogShowMode() == EventLogSettingsDialog::POSTPONE;
}


bool AssetManager::IsEventLogRealTimeShowModeEnabled(void)const
{
	return eventTreeWidget->GetLogShowMode() == EventLogSettingsDialog::REAL_TIME;
}



void AssetManager::ShowEventLogContextMenu(QPoint _mousePos)
{
	contextMenu->move(_mousePos);
	contextMenu->addAction(eventLogSettingsAct);
	contextMenu->addAction(eventLogClearAct);
	contextMenu->show();
}



void AssetManager::ExpandRoot(void)
{
	objectTreeWidget->ExpandRoot();

	updateTreeWidget->ExpandRoot();

	drawTreeWidget->ExpandRoot();
}



void AssetManager::UnselectObject(GraphicsSurface* _object)
{
	objectTreeWidget->Unselect(_object);
}


void AssetManager::SelectObject(GraphicsSurface* _object)
{
	objectTreeWidget->Select(_object);
}



ObjectTreeWidget* AssetManager::GetObjectTreeWidget(void)const
{
	return objectTreeWidget;
}


LogicTreeWidget* AssetManager::GetUpdateTreeWidget(void)const
{
	return updateTreeWidget;
}


GraphicsTreeWidget* AssetManager::GetDrawTreeWidget(void)const
{
	return drawTreeWidget;
}


EventTreeWidget* AssetManager::GetEventTreeWidget(void)const
{
	return eventTreeWidget;
}