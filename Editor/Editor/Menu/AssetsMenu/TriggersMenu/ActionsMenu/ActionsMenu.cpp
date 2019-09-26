
#include "..\..\..\..\Editor.h"
#include <Editor/TriggerCreator/ActionCreator/ActionCreator.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/WarningDialog/WarningDialog.h>



void Editor::CreateActionsMenu(void)
{
	actionsAssetMenu = new QMenu(triggersAssetMenu);
	actionsAssetMenu->setTitle("Actions");
	triggersAssetMenu->addMenu(actionsAssetMenu);

	CreateNewActionAct();
	CreateEditActionAct();
}



void Editor::CreateNewActionAct(void)
{
	actionNewAct = new QAction("New", actionsAssetMenu);
	connect(actionNewAct, SIGNAL(triggered()), SLOT(NewAction()));
	actionsAssetMenu->addAction(actionNewAct);
}


void Editor::NewAction(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ActionEditor.exe").toAscii().data(), QString("ActionEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void Editor::CreateEditActionAct(void)
{
	actionsEditAct = new QAction("Edit", actionsAssetMenu);
	connect(actionsEditAct, SIGNAL(triggered()), SLOT(EditAction()));
	actionsAssetMenu->addAction(actionsEditAct);
}




void Editor::EditAction(void)
{
	QFileDialog dialog(this, "Choose action");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + AbstractAction::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + AbstractAction::_GetDir()).c_str());

	
	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditAction(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}



void Editor::EditAction(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ActionEditor.exe").toAscii().data(), (QString("ActionEditor.exe ") + _name + QString(" edit") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}
