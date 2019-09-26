
#include "..\..\Editor.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/WarningDialog/WarningDialog.h>


void Editor::CreateSceneMenu(void)
{
	sceneMenu = menuBar()->addMenu("Scene");
	
	CreateNewSceneAct();
	CreateOpenSceneAct();
	CreateSaveAsSceneAct();
	CreateExitAct();
}


void Editor::CreateNewSceneAct(void)
{
	newSceneAct = new QAction("New", this);  
	connect(newSceneAct, SIGNAL(triggered()), SLOT(NewScene()));
	sceneMenu->addAction(newSceneAct);
}

void Editor::CreateOpenSceneAct(void)
{
	openSceneAct = new QAction("Open", this);  
	connect(openSceneAct, SIGNAL(triggered()), SLOT(OpenScene()));
	sceneMenu->addAction(openSceneAct);
}
	

void Editor::CreateSaveAsSceneAct(void)
{
	saveAsSceneAct = new QAction("Save as...", this);  
	connect(saveAsSceneAct, SIGNAL(triggered()), SLOT(SaveSceneAs()));
	sceneMenu->addAction(saveAsSceneAct);
}


void Editor::CreateExitAct(void)
{
	exitAct = new QAction("Exit", this);  
	connect(exitAct, SIGNAL(triggered()), SLOT(Exit()));
	sceneMenu->addAction(exitAct);
}



void Editor::NewScene(void)
{	
	bool result = false;

	if(Scene::_GetPicked())
	{
		WarningDialog dialog("Warning", "Do you really want to create a new scene? Current scene will be lost.", 200, 90);
			
		if(dialog.exec() == QDialog::Accepted)
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}

	if(result)
	{ 
		DestroyScene();
	}
}


void Editor::SaveSceneAs(void)
{		
	if(Scene::_GetPicked())
	{
		StringANSI sceneName = Scene::_GetPicked()->GetName();

		QString pathFile = QFileDialog::getSaveFileName(this,
	                                                    "Save scene as...",
													    (AssetLibrary::_GetDir() + ObjectScene::_GetDir() + sceneName).c_str(),
													    QString("*") + ObjectScene::_GetFileSuffix().c_str());

		if(!pathFile.isEmpty())
		{	
			StringANSI name = File::_GetBaseName(pathFile.toStdString());
			if(File::_IsExist(AssetLibrary::_GetDir() + ObjectScene::_GetDir() + name + "." + ObjectScene::_GetFileSuffix()))
			{ 
				WarningDialog warning("Warning", "Scene '" +  QString(name.c_str()) + "' is already exist. Rewrite file?", 200, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					return;
				}
			}

			Scene::_GetPicked()->SaveAsToFile(name);
		}
	}
	else
	{
		ErrorDialog("Error", "Picked scene is not defined. You must pick scene.", 270, 50);
	}
}



void Editor::OpenScene(void)
{
	bool result = false;

	if(Scene::_GetPicked())
	{
		WarningDialog dialog("Warning", "Do you really want to load scene? Current scene will be lost.", 200, 90);
		
		if(dialog.exec() == QDialog::Accepted)
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}

	if(result)
	{
		QString pathToFile = QFileDialog::getOpenFileName(this,
	                                                      "Load scene",
	                                                      (AssetLibrary::_GetDir() + ObjectScene::_GetDir()).c_str(),
														  QString("*") + ObjectScene::_GetFileSuffix().c_str());
		
		if(pathToFile.length())
		{
			QFileInfo fileInfo(pathToFile);
			QString baseName = fileInfo.completeBaseName();

			{
				File file(pathToFile.toStdString());

				if(file.Open(File::READ))
				{
					int32 version;
					file.Read(version);
					StringANSI name;
					int32 length;
					file.Read(length);
					file.ReadString(name, length);
					int64 type;
					file.Read(type);

					if(!(type & Scene::SCENE_CLASS))
					{
						if(type & LogicScene::LOGIC_SCENE_CLASS)
						{
							ErrorDialog("Error", "<" + baseName + "> is a LogicScene.", 255, 40);
						}
						else if(type & GraphicsScene::GRAPHICS_SCENE_CLASS)
						{
							ErrorDialog("Error", "<" + baseName + "> is a GraphicsScene.", 255, 40);
						}
						else if(type & ObjectScene::OBJECT_SCENE_CLASS)
						{
							ErrorDialog("Error", "<" + baseName + "> is a ObjectScene.", 255, 40);
						}
						return;
					}
				}
			}
		
			LoadScene(pathToFile);

			assetManager->setFocus();
		}
	}
}



void Editor::Exit(void)
{
	close();
}


