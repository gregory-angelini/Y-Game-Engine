
#include "..\..\..\..\Editor.h"
#include <Editor/PlayerControlCreator/PlayerControlCreator.h>


void Editor::CreatePlayerControlsMenu(void)
{
	playerControlsAssetMenu = new QMenu(controlsAssetMenu);
	playerControlsAssetMenu->setTitle("Player");
	controlsAssetMenu->addMenu(playerControlsAssetMenu);


	CreateNewPlayerControlAct();
	CreateEditPlayerControlAct();
}


void Editor::CreateNewPlayerControlAct(void)
{
	playerControlNewAct = new QAction("New", playerControlsAssetMenu);
	connect(playerControlNewAct, SIGNAL(triggered()), SLOT(NewPlayerControl()));
	playerControlsAssetMenu->addAction(playerControlNewAct);
}


void Editor::CreateEditPlayerControlAct(void)
{
	playerControlEditAct = new QAction("Edit", playerControlsAssetMenu);
	connect(playerControlEditAct, SIGNAL(triggered()), SLOT(EditPlayerControl()));
	playerControlsAssetMenu->addAction(playerControlEditAct);
}




void Editor::NewPlayerControl(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("PlayerControlEditor.exe").toAscii().data(), QString("PlayerControlEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}


void Editor::EditPlayerControl(void)
{ 
	QFileDialog dialog(this, "Choose player control");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + PlayerControl::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + PlayerControl::_GetDir()).c_str());

	
	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditPlayerControl(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}


void Editor::EditPlayerControl(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("PlayerControlEditor.exe").toAscii().data(), (QString("PlayerControlEditor.exe ") + _name + QString(" edit") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}