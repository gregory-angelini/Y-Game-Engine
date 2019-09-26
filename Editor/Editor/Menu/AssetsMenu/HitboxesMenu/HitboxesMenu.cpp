
#include "..\..\..\Editor.h"
#include <Editor/TextureCreator/TextureCreator.h>



void Editor::CreateHitboxesMenu(void)
{
	hitboxesAssetMenu = new QMenu(logicsAssetMenu);
	hitboxesAssetMenu->setTitle("Hitboxes");
	logicsAssetMenu->addMenu(hitboxesAssetMenu);
	
	CreateNewHitboxAct();
	CreateEditHitboxAct();
}



void Editor::CreateEditHitboxAct(void)
{
	hitboxEditAct = new QAction("Edit", hitboxesAssetMenu);
	connect(hitboxEditAct, SIGNAL(triggered()), SLOT(EditHitbox()));
	hitboxesAssetMenu->addAction(hitboxEditAct);
}


void Editor::CreateNewHitboxAct(void)
{
	hitboxNewAct = new QAction("New", hitboxesAssetMenu);
	connect(hitboxNewAct, SIGNAL(triggered()), SLOT(NewHitbox()));
	hitboxesAssetMenu->addAction(hitboxNewAct);
}


void Editor::NewHitbox(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("HitboxEditor.exe").toAscii().data(), QString("HitboxEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable") + QString(" hitbox")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void Editor::EditHitbox(void)
{
	QFileDialog dialog(this, "Choose hitbox");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + Hitbox::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() +  Hitbox::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditHitbox(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}


void Editor::EditHitbox(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("HitboxEditor.exe").toAscii().data(), (QString("HitboxEditor.exe ") + _name + QString(" edit") + QString(" nameEnable") + QString(" hitbox")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}

