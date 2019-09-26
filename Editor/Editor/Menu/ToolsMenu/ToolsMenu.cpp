
#include "..\..\Editor.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/WarningDialog/WarningDialog.h>


void Editor::CreateToolsMenu(void)
{
	toolsMenu = menuBar()->addMenu("Tools");
	
	CreateDesignToolsMenu();
}
