
#include "..\..\..\..\..\Editor.h"


void Editor::CreateAtlasesMenu(void)
{
	atlasesAssetMenu = new QMenu(texturedAnimationsAssetMenu);
	atlasesAssetMenu->setTitle("Atlases");
	texturedAnimationsAssetMenu->addMenu(atlasesAssetMenu);
	
	CreateNewAtlasAct();
	CreateEditAtlasAct();
}



void Editor::CreateEditAtlasAct(void)
{
	editAtlasAct = new QAction("Edit", atlasesAssetMenu);
	connect(editAtlasAct, SIGNAL(triggered()), SLOT(EditAtlas()));
	atlasesAssetMenu->addAction(editAtlasAct);
}


void Editor::CreateNewAtlasAct(void)
{
	newAtlasAct = new QAction("New", atlasesAssetMenu);
	connect(newAtlasAct, SIGNAL(triggered()), SLOT(NewAtlas()));
	atlasesAssetMenu->addAction(newAtlasAct);
}


void Editor::NewAtlas(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TexturedAnimationEditor.exe").toAscii().data(), QString("TexturedAnimationEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable") + QString(" atlas")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void Editor::EditAtlas(void)
{
	QFileDialog dialog(this, "Choose atlas");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + Atlas2D::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() +  Atlas2D::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditAtlas(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}


void Editor::EditAtlas(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TexturedAnimationEditor.exe").toAscii().data(), (QString("TexturedAnimationEditor.exe ") + _name + QString(" edit") + QString(" nameEnable") + QString(" atlas")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}

