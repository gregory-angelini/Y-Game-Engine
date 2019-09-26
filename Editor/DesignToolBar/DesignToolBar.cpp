#include "DesignToolBar.h"
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




DesignToolBar::DesignToolBar(QWidget* _parent): QToolBar(_parent), currentTool(NO_TOOL), viewToolEnable(false)
{
	setIconSize(QSize(32, 32));

	CreateViewTool();
	CreateSelectTool();
	CreateScaleTool();
	CreateMoveTool();
	CreateRotateTool();
	CreateEraseTool();
	
	setWindowTitle("Design tools");
}



DesignToolBar::~DesignToolBar(void)
{}



bool DesignToolBar::IsViewToolEnabled(void)
{
	return viewToolEnable;
}




void DesignToolBar::CreateSelectTool(void)
{
	selectAction = new QAction(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/select.png"), "Select", this);
	selectAction->setCheckable(true);
	connect(selectAction, SIGNAL(triggered()), this, SLOT(SetSelectTool()));
	
	addAction(selectAction);
}



void DesignToolBar::CreateScaleTool(void)
{
	scaleAction = new QAction(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/scale.png"), "Scale", this);
	scaleAction->setCheckable(true);
	scaleAction->setChecked(false);
	connect(scaleAction, SIGNAL(triggered()), this, SLOT(SetScaleTool()));
	
	scaleToolButton = new QToolButton(this);
	scaleToolButton->setDefaultAction(scaleAction);
	scaleToolButton->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(scaleToolButton, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ScaleContextMenu(const QPoint&)));
	addWidget(scaleToolButton);

	CreateScaleContextMenu();
}


void DesignToolBar::CreateScaleContextMenu(void)
{
	scaleContextMenu = new QMenu(this);
	scaleToolHomeAct = new QAction("Home", scaleContextMenu);
	connect(scaleToolHomeAct, SIGNAL(triggered()), SLOT(ScaleToolHome()));
	scaleContextMenu->addAction(scaleToolHomeAct);
}


void DesignToolBar::ScaleToolHome(void)
{
	emit ScaleHomeIsChecked();
}


void DesignToolBar::ScaleContextMenu(const QPoint& _point)
{
	scaleContextMenu->exec(scaleToolButton->mapToGlobal(_point));
}


void DesignToolBar::CreateMoveTool(void)
{
	moveAction = new QAction(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/move.png"), "Move", this);
	moveAction->setCheckable(true);
	moveAction->setChecked(false);
	connect(moveAction, SIGNAL(triggered()), this, SLOT(SetMoveTool()));
	
	moveToolButton = new QToolButton(this);
	moveToolButton->setDefaultAction(moveAction);
	moveToolButton->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(moveToolButton, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(MoveContextMenu(const QPoint&)));
	addWidget(moveToolButton);
   
	CreateMoveContextMenu();
}


void DesignToolBar::CreateMoveContextMenu(void)
{
	moveContextMenu = new QMenu(this);
	moveToolHomeAct = new QAction("Home", moveContextMenu);
	connect(moveToolHomeAct, SIGNAL(triggered()), SLOT(MoveToolHome()));
	moveContextMenu->addAction(moveToolHomeAct);
}


void DesignToolBar::MoveToolHome(void)
{
	emit MoveHomeIsChecked();
}


void DesignToolBar::MoveContextMenu(const QPoint& _point)
{
	moveContextMenu->exec(moveToolButton->mapToGlobal(_point));
}



void DesignToolBar::CreateRotateTool(void)
{
	rotateAction = new QAction(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/rotate.png"), "Rotate", this);
	rotateAction->setCheckable(true);
	scaleAction->setChecked(false);
	connect(rotateAction, SIGNAL(triggered()), this, SLOT(SetRotateTool()));
	
	rotateToolButton = new QToolButton(this);
	rotateToolButton->setDefaultAction(rotateAction);
	rotateToolButton->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(rotateToolButton, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(RotateContextMenu(const QPoint&)));
	addWidget(rotateToolButton);

	CreateRotateContextMenu();
}



void DesignToolBar::CreateRotateContextMenu(void)
{
	rotateContextMenu = new QMenu(this);
	rotateToolHomeAct = new QAction("Home", rotateContextMenu);
	connect(rotateToolHomeAct, SIGNAL(triggered()), SLOT(RotateToolHome()));
	rotateContextMenu->addAction(rotateToolHomeAct);
}


void DesignToolBar::RotateToolHome(void)
{
	emit RotateHomeIsChecked();
}


void DesignToolBar::RotateContextMenu(const QPoint& _point)
{
	rotateContextMenu->exec(rotateToolButton->mapToGlobal(_point));
}



void DesignToolBar::CreateEraseTool(void)
{
	eraseAction = new QAction(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/erase.png"), "Erase", this);
	eraseAction->setCheckable(true);
	connect(eraseAction, SIGNAL(triggered()), this, SLOT(SetEraseTool()));
	 
	addAction(eraseAction);
}





void DesignToolBar::CreateViewTool(void)
{
	viewAction = new QAction(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/view.png"), "Camera", this);

	viewToolButton = new QToolButton(this);
	viewToolButton->setDefaultAction(viewAction);
	viewToolButton->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(viewToolButton, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(UpdateViewContextMenu(const QPoint&)));
	addWidget(viewToolButton);
	
	CreateViewContextMenu();
}


void DesignToolBar::CreateViewContextMenu(void)
{
	viewContextMenu = new QMenu(this);
	viewToolHomeAct = new QAction("Home", viewContextMenu);
	connect(viewToolHomeAct, SIGNAL(triggered()), SLOT(ViewToolHome()));
	viewContextMenu->addAction(viewToolHomeAct);
	viewToolMovementEnableAct = new QAction("Movement enable", viewContextMenu);
	connect(viewToolMovementEnableAct, SIGNAL(triggered()), SLOT(ViewToolMovementEnable()));
	viewToolMovementEnableAct->setCheckable(true);
	viewContextMenu->addAction(viewToolMovementEnableAct);
	viewToolMovementEnableAct->setChecked(true);
	viewToolShowSightAct = new QAction("Show sight", viewContextMenu);
	connect(viewToolShowSightAct, SIGNAL(triggered()), SLOT(ViewToolShowSightEnable()));
	viewToolShowSightAct->setCheckable(true);
	viewContextMenu->addAction(viewToolShowSightAct);


	viewContextMenu->addSeparator();
	viewZoomEnableAct = new QAction("Zoom enable", viewContextMenu);
	connect(viewZoomEnableAct, SIGNAL(triggered()), SIGNAL(CameraZoomEnableIsChecked()));
	viewZoomEnableAct->setCheckable(true);
	viewContextMenu->addAction(viewZoomEnableAct);
	viewRotationEnableAct = new QAction("Rotation enable", viewContextMenu);
	connect(viewRotationEnableAct, SIGNAL(triggered()), SIGNAL(CameraRotationEnableIsChecked()));
	viewRotationEnableAct->setCheckable(true);
	viewContextMenu->addAction(viewRotationEnableAct);
	viewToolRotationModeMenu = new QMenu(viewContextMenu);
	viewToolRotationModeMenu->setTitle("Rotation mode");
	viewContextMenu->addMenu(viewToolRotationModeMenu);
	viewToolFirstPersonRotateModeAct = new QAction("First person", viewToolRotationModeMenu);
	connect(viewToolFirstPersonRotateModeAct, SIGNAL(triggered()), SLOT(ViewToolFirstPersonIsChecked()));
	viewToolFirstPersonRotateModeAct->setCheckable(true);
	viewToolRotationModeMenu->addAction(viewToolFirstPersonRotateModeAct);
	viewToolObjectViewerRotateModeAct = new QAction("Object viewer", viewToolRotationModeMenu);
	connect(viewToolObjectViewerRotateModeAct, SIGNAL(triggered()), SLOT(ViewToolObjectViewerIsChecked()));
	viewToolObjectViewerRotateModeAct->setCheckable(true);
	viewToolRotationModeMenu->addAction(viewToolObjectViewerRotateModeAct);
	viewToolProjectionModeMenu = new QMenu(viewContextMenu);
	viewToolProjectionModeMenu->setTitle("Projection mode");
	viewContextMenu->addMenu(viewToolProjectionModeMenu);
	viewToolPerspectiveProjectionModeAct = new QAction("Perspective", viewToolProjectionModeMenu);
	connect(viewToolPerspectiveProjectionModeAct, SIGNAL(triggered()), SLOT(ViewToolPerspectiveProjectionMode()));
	viewToolPerspectiveProjectionModeAct->setCheckable(true);
	viewToolProjectionModeMenu->addAction(viewToolPerspectiveProjectionModeAct);
	viewToolOrthographicProjectionModeAct = new QAction("Orthographic", viewToolProjectionModeMenu);
	connect(viewToolOrthographicProjectionModeAct, SIGNAL(triggered()), SLOT(ViewToolOrthographicProjectionMode()));
	viewToolOrthographicProjectionModeAct->setCheckable(true);
	viewToolProjectionModeMenu->addAction(viewToolOrthographicProjectionModeAct);
	viewToolScreenProjectionModeAct = new QAction("Screen", viewToolProjectionModeMenu);
	connect(viewToolScreenProjectionModeAct, SIGNAL(triggered()), SLOT(ViewToolScreenProjectionMode()));
	viewToolScreenProjectionModeAct->setCheckable(true);
	viewToolProjectionModeMenu->addAction(viewToolScreenProjectionModeAct);
	viewToolMovementModeMenu = new QMenu(viewContextMenu);
	viewToolMovementModeMenu->setTitle("Movement mode");
	viewContextMenu->addMenu(viewToolMovementModeMenu);
	viewToolSpectatorMovementModeAct = new QAction("Spectator", viewToolMovementModeMenu);
	connect(viewToolSpectatorMovementModeAct, SIGNAL(triggered()), SLOT(ViewToolSpectatorMovementMode()));
	viewToolSpectatorMovementModeAct->setCheckable(true);
	viewToolMovementModeMenu->addAction(viewToolSpectatorMovementModeAct);
	viewToolOverlandMovementModeAct = new QAction("Overland", viewToolMovementModeMenu);
	connect(viewToolOverlandMovementModeAct, SIGNAL(triggered()), SLOT(ViewToolOverlandMovementMode()));
	viewToolOverlandMovementModeAct->setCheckable(true);
	viewToolMovementModeMenu->addAction(viewToolOverlandMovementModeAct);
	viewToolFlightMovementModeAct = new QAction("Flight", viewToolMovementModeMenu);
	connect(viewToolFlightMovementModeAct, SIGNAL(triggered()), SLOT(ViewToolFlightMovementMode()));
	viewToolFlightMovementModeAct->setCheckable(true);
	viewToolMovementModeMenu->addAction(viewToolFlightMovementModeAct);
} 



void DesignToolBar::ViewToolSpectatorMovementMode(void)
{
	emit CameraSpectatorIsChecked();
	viewToolOverlandMovementModeAct->setChecked(false); 
	viewToolFlightMovementModeAct->setChecked(false);
	viewToolSpectatorMovementModeAct->setChecked(true); 
}



void DesignToolBar::ViewToolOverlandMovementMode(void)
{
	emit CameraOverlandIsChecked();
	viewToolFlightMovementModeAct->setChecked(false);
	viewToolSpectatorMovementModeAct->setChecked(false); 
	viewToolOverlandMovementModeAct->setChecked(true); 
}



void DesignToolBar::ViewToolFlightMovementMode(void)
{
	emit CameraFlightIsChecked();
	viewToolSpectatorMovementModeAct->setChecked(false); 
	viewToolOverlandMovementModeAct->setChecked(false); 
	viewToolFlightMovementModeAct->setChecked(true);
}



void DesignToolBar::ViewToolPerspectiveProjectionMode(void)
{
	emit CameraPerspectiveProjectionIsChecked();
	viewToolOrthographicProjectionModeAct->setChecked(false); 
	viewToolScreenProjectionModeAct->setChecked(false);
	viewToolPerspectiveProjectionModeAct->setChecked(true); 
}



void DesignToolBar::ViewToolOrthographicProjectionMode(void)
{
	emit CameraOrthographicProjectionIsChecked();
	viewToolScreenProjectionModeAct->setChecked(false);
	viewToolPerspectiveProjectionModeAct->setChecked(false); 
	viewToolOrthographicProjectionModeAct->setChecked(true); 
}


void DesignToolBar::ViewToolScreenProjectionMode(void)
{
	emit CameraScreenProjectionIsChecked();
	viewToolPerspectiveProjectionModeAct->setChecked(false); 
	viewToolOrthographicProjectionModeAct->setChecked(false); 
	viewToolScreenProjectionModeAct->setChecked(true);
}


void DesignToolBar::ViewToolFirstPersonIsChecked(void)
{
	emit CameraFirstPersonIsChecked();
	viewToolObjectViewerRotateModeAct->setChecked(false); 
	viewToolFirstPersonRotateModeAct->setChecked(true); 
}


void DesignToolBar::ViewToolObjectViewerIsChecked(void)
{
	emit CameraObjectViewerIsChecked();
	viewToolFirstPersonRotateModeAct->setChecked(false); 
	viewToolObjectViewerRotateModeAct->setChecked(true); 
}


void DesignToolBar::ViewToolMovementEnable(void)
{
	emit CameraMovementEnableIsChecked(viewToolMovementEnableAct->isChecked());
}


void DesignToolBar::SetViewMovementEnable(bool _enable)
{ 
	viewToolMovementEnableAct->setChecked(_enable);

	emit CameraMovementEnableIsChecked(viewToolMovementEnableAct->isChecked());
}


void DesignToolBar::ViewToolShowSightEnable(void)
{
	emit CameraShowSightIsChecked(viewToolShowSightAct->isChecked());
}


void DesignToolBar::SetViewShowSightEnable(bool _enable)
{
	viewToolShowSightAct->setChecked(_enable);

	emit CameraShowSightIsChecked(viewToolShowSightAct->isChecked());
}


void DesignToolBar::ViewToolHome(void)
{
	emit CameraHomeIsChecked();
}



void DesignToolBar::UpdateViewContextMenu(const QPoint& _point)
{
	UpdateViewContextMenu();

	viewContextMenu->exec(viewToolButton->mapToGlobal(_point));
}



void DesignToolBar::UpdateViewContextMenu(void)
{
	if(Camera3D::_GetPicked())
	{
		if(Camera3D::_GetPicked()->GetRotationMode() == Camera3D::OBJECT_VIEWER_ROTATION)
		{
			viewToolMovementModeMenu->setEnabled(false);
			viewToolObjectViewerRotateModeAct->setChecked(true); 
			viewToolFirstPersonRotateModeAct->setChecked(false); 
			
		}
		else
		{
			viewToolMovementModeMenu->setEnabled(true);
			if(Camera3D::_GetPicked()->GetRotationMode() == Camera3D::FIRST_PERSON_ROTATION)
			{
				viewToolFirstPersonRotateModeAct->setChecked(true); 
				viewToolObjectViewerRotateModeAct->setChecked(false); 
			}
		}
		switch(Camera3D::_GetPicked()->GetProjectionMode())
		{
			case Camera3D::PERSPECTIVE_PROJECTION:
			{
				viewToolPerspectiveProjectionModeAct->setChecked(true);
				viewToolOrthographicProjectionModeAct->setChecked(false);
				viewToolScreenProjectionModeAct->setChecked(false);
				break;
			}
			
			case Camera3D::ORTHOGRAPHIC_PROJECTION:
			{ 
				viewToolPerspectiveProjectionModeAct->setChecked(false);
				viewToolOrthographicProjectionModeAct->setChecked(true);
				viewToolScreenProjectionModeAct->setChecked(false);
				break;
			}

			case Camera3D::SCREEN_PROJECTION:
			{
				viewToolPerspectiveProjectionModeAct->setChecked(false);
				viewToolOrthographicProjectionModeAct->setChecked(false);
				viewToolScreenProjectionModeAct->setChecked(true);
				break;
			}
		}
		switch(Camera3D::_GetPicked()->GetMovementMode())
		{
			case Camera3D::SPECTATOR_MOVEMENT:
			{
				viewToolSpectatorMovementModeAct->setChecked(true);
				viewToolOverlandMovementModeAct->setChecked(false);
				viewToolFlightMovementModeAct->setChecked(false);
				break;
			}

			case Camera3D::OVERLAND_MOVEMENT:
			{
				viewToolSpectatorMovementModeAct->setChecked(false);
				viewToolOverlandMovementModeAct->setChecked(true);
				viewToolFlightMovementModeAct->setChecked(false);
				break;
			}

			case Camera3D::FLIGHT_MOVEMENT:
			{
				viewToolSpectatorMovementModeAct->setChecked(false);
				viewToolOverlandMovementModeAct->setChecked(false);
				viewToolFlightMovementModeAct->setChecked(true);
				break;
			}
		}
		viewZoomEnableAct->setChecked(Camera3D::_GetPicked()->IsZoomEnabled());
		viewRotationEnableAct->setChecked(Camera3D::_GetPicked()->IsRotationEnabled());
		viewToolHomeAct->setEnabled(true);
		viewToolProjectionModeMenu->setEnabled(true);
		viewToolRotationModeMenu->setEnabled(true); 
		viewZoomEnableAct->setEnabled(true);
		viewRotationEnableAct->setEnabled(true);
	}
	else
	{
		viewToolHomeAct->setEnabled(false);
		viewToolProjectionModeMenu->setEnabled(false);
		viewToolRotationModeMenu->setEnabled(false);
		viewToolMovementModeMenu->setEnabled(false);
		viewZoomEnableAct->setEnabled(false);
		viewRotationEnableAct->setEnabled(false);
	}
}



int DesignToolBar::GetCurrentTool(void)const
{
	return currentTool;
}



void DesignToolBar::SetEraseTool(void)
{
	if(currentTool != ERASE)
	{
		if(eraseAction) 
		{ 
			if(!eraseAction->isChecked()) { eraseAction->setChecked(true); } 
		}

		if(scaleAction)   { scaleAction->setChecked(false); }
		if(moveAction)    { moveAction->setChecked(false); }
		if(rotateAction)  { rotateAction->setChecked(false); }
		if(selectAction)  { selectAction->setChecked(false); }

		emit PickEraseTool();
		currentTool = ERASE;
	}
	else
	{
		if(eraseAction) { if(eraseAction->isChecked()) { eraseAction->setChecked(false); } }

		emit UnpickEraseTool();
		currentTool = NO_TOOL;
	}
}




void DesignToolBar::SetSelectTool(void)
{
	if(currentTool != SELECT)
	{
		if(selectAction) 
		{ 
			if(!selectAction->isChecked()) { selectAction->setChecked(true); } 
		}

		if(scaleAction)  { scaleAction->setChecked(false); }
		if(moveAction)   { moveAction->setChecked(false); }
		if(rotateAction) { rotateAction->setChecked(false); }
		if(eraseAction)  { eraseAction->setChecked(false); }

		emit PickSelectTool();
		currentTool = SELECT;
	}
	else
	{
		if(selectAction) { if(selectAction->isChecked()) { selectAction->setChecked(false); } }

		emit UnpickSelectTool();
		currentTool = NO_TOOL;
	}
}



void DesignToolBar::SetScaleTool(void)
{
	if(currentTool != SCALE)
	{
		if(scaleAction)   
		{ 
			if(!scaleAction->isChecked()) { scaleAction->setChecked(true); } 
		}

		if(selectAction)  { selectAction->setChecked(false); }
		if(moveAction)    { moveAction->setChecked(false); }
		if(rotateAction)  { rotateAction->setChecked(false); }
		if(eraseAction)   { eraseAction->setChecked(false); }

		emit PickScaleTool();
		currentTool = SCALE;
	}
	else
	{
		if(scaleAction) { if(scaleAction->isChecked()) { scaleAction->setChecked(false); } }

		emit UnpickScaleTool();
		currentTool = NO_TOOL;
	}
}





void DesignToolBar::SetMoveTool(void)
{
	if(currentTool != TRANSLATE)
	{
		if(moveAction)     
		{ 
			if(!moveAction->isChecked()) { moveAction->setChecked(true); } 
		}

		if(selectAction)   { selectAction->setChecked(false); }
		if(scaleAction)    { scaleAction->setChecked(false); }
		if(rotateAction)   { rotateAction->setChecked(false); }
		if(eraseAction)    { eraseAction->setChecked(false); }

		emit PickMoveTool();
		currentTool = TRANSLATE;
	}
	else
	{
		if(moveAction) 
		{ 
			if(moveAction->isChecked()) { moveAction->setChecked(false); } 
		}

		emit UnpickMoveTool();
		currentTool = NO_TOOL;
	}
}




void DesignToolBar::SetRotateTool(void)
{
	if(currentTool != ROTATE)
	{
		if(rotateAction)  
		{ 
			if(!rotateAction->isChecked()) { rotateAction->setChecked(true); } 
		}

		if(scaleAction)   { scaleAction->setChecked(false); }
		if(moveAction)    { moveAction->setChecked(false); }
		if(selectAction)  { selectAction->setChecked(false); }
		if(eraseAction)   { eraseAction->setChecked(false); }

		emit PickRotateTool();
		currentTool = ROTATE;
	}
	else
	{
		if(rotateAction) { if(rotateAction->isChecked()) { rotateAction->setChecked(false); } }

		emit UnpickRotateTool();
		currentTool = NO_TOOL;
	}
}
