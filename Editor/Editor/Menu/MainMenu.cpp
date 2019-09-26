
#include "..\Editor.h"


void Editor::CreateMainMenu(void)
{
	CreateSceneMenu();
	CreateAssetsMenu();
	CreateToolsMenu();
	CreateHelpMenu();
}


void Editor::DestroyMainMenu(void)
{
	delete sceneMenu;
	delete toolsMenu;
	delete assetsMenu;
	delete helpMenu;
}
