
#include "..\..\..\Editor.h"
#include <Editor/ObjectCreator/ObjectCreator.h>
#include <Engine/Core/Time/Timer/Timer.h>


void Editor::CreateObjectsMenu(void)
{
	objectsAssetMenu = new QMenu(logicsAssetMenu);
	objectsAssetMenu->setTitle("Objects");
	logicsAssetMenu->addMenu(objectsAssetMenu);

	CreateNewObjectAct();
	CreateReviewObjectAct();
}



void Editor::CreateReviewObjectAct(void)
{
	objectReviewAct = new QAction("Review", objectsAssetMenu);
	connect(objectReviewAct, SIGNAL(triggered()), SLOT(ReviewObject()));
	objectsAssetMenu->addAction(objectReviewAct);
}



void Editor::CreateNewObjectAct(void)
{
	objectNewAct = new QAction("New", objectsAssetMenu);
	connect(objectNewAct, SIGNAL(triggered()), SLOT(NewObject()));
	objectsAssetMenu->addAction(objectNewAct);
}



void Editor::NewObject(void)
{ 
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), QString("ObjectEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void Editor::ReviewObject(void)
{ 
	QFileDialog dialog(this, "Choose asset");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + AbstractObject::_GetFileSuffix().c_str() + 
			             QString(" *.") + ObjectScene::_GetFileSuffix().c_str() + 
						 QString(" *.") + Trigger::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractEvent::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractAction::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractVariable::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractList::_GetFileSuffix().c_str() + 
						 QString(" *.") + PlayerControl::_GetFileSuffix().c_str() + 
						 QString(" *.") + FloatLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + VectorLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + QuaternionLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + Camera3D::_GetFileSuffix().c_str() + 
						 QString(" *.") + Texture2D::_GetFileSuffix().c_str() + 
						 QString(" *.") + Atlas2D::_GetFileSuffix().c_str() + 
						 QString(" *.") + Mesh::_GetFileSuffix().c_str() + 
						 QString(" *.") + Hitbox::_GetFileSuffix().c_str() + 
						 QString(" *.") + Timer::_GetFileSuffix().c_str() + 
						 QString(" *.") + TexturedAnimation::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory(AssetLibrary::_GetDir().c_str());

	
	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
		
		if(path.size() != 0)
		{ 
			if(!File::_IsExist(path.at(0).toAscii().data()))
			{
				return;
			}

			StringANSI name = File::_GetBaseName(path.at(0).toAscii().data());
			StringANSI suffix = File::_GetSuffix(path.at(0).toAscii().data());

			ReviewObject(QString(name.c_str()) + QString(".") + QString(suffix.c_str()));
		}
	}
}



void Editor::ReviewObject(QString _path)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + _path + QString(" review") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}
