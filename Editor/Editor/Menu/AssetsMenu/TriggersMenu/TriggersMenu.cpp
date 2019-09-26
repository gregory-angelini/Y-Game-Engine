
#include "..\..\..\Editor.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/WarningDialog/WarningDialog.h>


void Editor::CreateTriggersMenu(void)
{
	triggersAssetMenu = new QMenu(logicsAssetMenu);
	triggersAssetMenu->setTitle("Triggers");
	logicsAssetMenu->addMenu(triggersAssetMenu);

	CreateNewTriggerAct();
	CreateEditTriggerAct();
	CreateActionsMenu();
	CreateEventsMenu();
	CreateVariablesMenu();
}



void Editor::CreateNewTriggerAct(void)
{
	triggerNewAct = new QAction("New", triggersAssetMenu);
	connect(triggerNewAct, SIGNAL(triggered()), SLOT(NewTrigger()));
	triggersAssetMenu->addAction(triggerNewAct);
}




void Editor::CreateEditTriggerAct(void)
{
	triggersEditAct = new QAction("Edit", triggersAssetMenu);
	connect(triggersEditAct, SIGNAL(triggered()), SLOT(EditTrigger()));
	triggersAssetMenu->addAction(triggersEditAct);
}


void Editor::NewTrigger(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TriggerEditor.exe").toAscii().data(), QString("TriggerEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}




void Editor::EditTrigger(void)
{
	QFileDialog dialog(this, "Choose trigger");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + Trigger::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + Trigger::_GetDir()).c_str());

	
	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditTrigger(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}




void Editor::EditTrigger(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TriggerEditor.exe").toAscii().data(), (QString("TriggerEditor.exe ") + _name + QString(" edit") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}

