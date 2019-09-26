
#include "..\..\Editor.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Core/Time/Timer/Timer.h>



void Editor::CreateAssetsMenu(void)
{
	assetsMenu = menuBar()->addMenu("Assets");
	CreateGraphicsMenu();
	
	CreateAnimationsMenu();

	CreateTexturesMenu();
	CreateLogicsMenu();
	
	CreateTriggersMenu();
	
	CreateObjectsMenu();

	CreateControlsMenu();

	CreateLerpsMenu();

	CreateHitboxesMenu();
	CreateAssetsImportAct();
}


void Editor::CreateAssetsImportAct(void)
{ 
	assetsImportAct = new QAction("Import", assetsMenu);
	connect(assetsImportAct, SIGNAL(triggered()), SLOT(AssetsImport()));
	assetsMenu->addAction(assetsImportAct);
}



void Editor::AssetsImport(void)
{  
	QFileDialog dialog(this, "Choose assets");
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(QString("*.") + AbstractObject::_GetFileSuffix().c_str() + 
			             QString(" *.") + ObjectScene::_GetFileSuffix().c_str() + 
						 QString(" *.") + Trigger::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractEvent::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractAction::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractVariable::_GetFileSuffix().c_str() + 
						 QString(" *.") + PlayerControl::_GetFileSuffix().c_str() + 
						 QString(" *.") + FloatLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + QuaternionLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + VectorLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + Camera3D::_GetFileSuffix().c_str() + 
						 QString(" *.") + Texture2D::_GetFileSuffix().c_str() + 
						 QString(" *.") + Atlas2D::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractList::_GetFileSuffix().c_str() +
						 QString(" *.") + Mesh::_GetFileSuffix().c_str() +
						 QString(" *.") + Hitbox::_GetFileSuffix().c_str() +
						 QString(" *.") + Timer::_GetFileSuffix().c_str() +
						 QString(" *.") + TexturedAnimation::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory(AssetLibrary::_GetDir().c_str());


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}

		
		bool state = Trigger::_IsBlockEnabled();
		Trigger::_SetBlockEnable(false);
		for(int i = 0; i < assets.size(); ++i)
		{
			StringANSI suffix = File::_GetSuffix(assets.at(i).toAscii().data());
			AssetLibrary::_LoadCommonAsset(File::_GetBaseName(assets.at(i).toAscii().data()), AssetLibrary::_GetAssetTypeOfFileSuffix(suffix));
		}

		Trigger::_SetBlockEnable(state);
		assetManager->ExpandRoot();
	}
}
 

void Editor::CreateAnimationsMenu(void)
{
	animationsAssetMenu = new QMenu(graphicsAssetMenu);
	animationsAssetMenu->setTitle("Animations");
	graphicsAssetMenu->addMenu(animationsAssetMenu);

	CreateTexturedAnimationsMenu();
}



void Editor::CreateControlsMenu(void)
{
	controlsAssetMenu = new QMenu(logicsAssetMenu);
	controlsAssetMenu->setTitle("Controls");
	logicsAssetMenu->addMenu(controlsAssetMenu);

	CreatePlayerControlsMenu();
}



void Editor::CreateLerpsMenu(void)
{
	lerpsAssetMenu = new QMenu(logicsAssetMenu);
	lerpsAssetMenu->setTitle("Lerps");
	logicsAssetMenu->addMenu(lerpsAssetMenu);

	CreateFloatLerpsMenu();

	CreateQuaternionLerpsMenu();

	CreateVectorLerpsMenu();
}


void Editor::CreateLogicsMenu(void)
{
	logicsAssetMenu = new QMenu(assetsMenu);
	logicsAssetMenu->setTitle("Logics");
	assetsMenu->addMenu(logicsAssetMenu);
}


void Editor::CreateGraphicsMenu(void)
{
	graphicsAssetMenu = new QMenu(assetsMenu);
	graphicsAssetMenu->setTitle("Graphics");
	assetsMenu->addMenu(graphicsAssetMenu);
}