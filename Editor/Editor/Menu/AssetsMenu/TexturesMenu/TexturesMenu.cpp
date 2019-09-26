
#include "..\..\..\Editor.h"
#include <Editor/TextureCreator/TextureCreator.h>


void Editor::CreateTexturesMenu(void)
{
	texturesAssetMenu = new QMenu(graphicsAssetMenu);
	texturesAssetMenu->setTitle("Textures");
	graphicsAssetMenu->addMenu(texturesAssetMenu);
	
	CreateNewTextureAct();
	CreateEditTextureAct();
}



void Editor::CreateEditTextureAct(void)
{
	editTextureAct = new QAction("Edit", texturesAssetMenu);
	connect(editTextureAct, SIGNAL(triggered()), SLOT(EditTexture()));
	texturesAssetMenu->addAction(editTextureAct);
}


void Editor::CreateNewTextureAct(void)
{
	newTextureAct = new QAction("New", texturesAssetMenu);
	connect(newTextureAct, SIGNAL(triggered()), SLOT(NewTexture()));
	texturesAssetMenu->addAction(newTextureAct);
}



void Editor::NewTexture(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), QString("TextureEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable") + QString(" texture")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void Editor::EditTexture(void)
{
	QFileDialog dialog(this, "Choose texture");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + Texture2D::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + Texture2D::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditTexture(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}


void Editor::EditTexture(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), (QString("TextureEditor.exe ") + _name + QString(" edit") + QString(" nameEnable") + QString(" texture")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}

