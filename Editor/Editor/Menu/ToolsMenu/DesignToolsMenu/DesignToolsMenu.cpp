
#include "..\..\..\Editor.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/WarningDialog/WarningDialog.h>

void Editor::CreateDesignToolsMenu(void)
{
	designToolsMenu = new QMenu(toolsMenu);
	designToolsMenu->setTitle("Design");
	toolsMenu->addMenu(designToolsMenu);

	CreateSelectTool();
	CreateScaleTool();
	CreateMoveTool();
	CreateRotateTool();
	CreateEraseTool();
}



void Editor::CreateEraseTool(void)
{
	eraseToolAct = new QAction("Erase", this);
	designToolsMenu->addAction(eraseToolAct);
	eraseToolAct->setCheckable(true);
	eraseToolAct->setChecked(false);
	connect(eraseToolAct, SIGNAL(triggered()), designToolBar, SLOT(SetEraseTool()));
}


void Editor::CreateRotateTool(void)
{
	rotateToolAct = new QAction("Rotate", this);  
	designToolsMenu->addAction(rotateToolAct);
	rotateToolAct->setCheckable(true);
	rotateToolAct->setChecked(false);
	connect(rotateToolAct, SIGNAL(triggered()), designToolBar, SLOT(SetRotateTool()));
}


void Editor::CreateMoveTool(void)
{
	moveToolAct = new QAction("Move", this);  
	designToolsMenu->addAction(moveToolAct);
	moveToolAct->setCheckable(true);
	moveToolAct->setChecked(false);
	connect(moveToolAct, SIGNAL(triggered()), designToolBar, SLOT(SetMoveTool()));
}


void Editor::CreateSelectTool(void)
{
	selectToolAct = new QAction("Select", this);  
	designToolsMenu->addAction(selectToolAct);
	selectToolAct->setCheckable(true);
	selectToolAct->setChecked(false);
	connect(selectToolAct, SIGNAL(triggered()), designToolBar, SLOT(SetSelectTool()));
}


void Editor::CreateScaleTool(void)
{
	scaleToolAct = new QAction("Scale", this);  
	designToolsMenu->addAction(scaleToolAct);
	scaleToolAct->setCheckable(true);
	scaleToolAct->setChecked(false);
	connect(scaleToolAct, SIGNAL(triggered()), designToolBar, SLOT(SetScaleTool()));
}