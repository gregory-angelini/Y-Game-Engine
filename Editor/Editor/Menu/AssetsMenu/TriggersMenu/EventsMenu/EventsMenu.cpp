
#include "..\..\..\..\Editor.h"
#include <Editor/TriggerCreator/EventCreator/EventCreator.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/WarningDialog/WarningDialog.h>


void Editor::CreateEventsMenu(void)
{
	eventsAssetMenu = new QMenu(triggersAssetMenu);
	eventsAssetMenu->setTitle("Events");
	triggersAssetMenu->addMenu(eventsAssetMenu);

	CreateNewEventAct();
	CreateEditEventAct();
}



void Editor::CreateNewEventAct(void)
{
	eventNewAct = new QAction("New", eventsAssetMenu);
	connect(eventNewAct, SIGNAL(triggered()), SLOT(NewEvent()));
	eventsAssetMenu->addAction(eventNewAct);
}


void Editor::NewEvent(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("EventEditor.exe").toAscii().data(), QString("EventEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void Editor::CreateEditEventAct(void)
{
	eventsEditAct = new QAction("Edit", eventsAssetMenu);
	connect(eventsEditAct, SIGNAL(triggered()), SLOT(EditEvent()));
	eventsAssetMenu->addAction(eventsEditAct);
}




void Editor::EditEvent(void)
{
	QFileDialog dialog(this, "Choose event");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + AbstractEvent::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + AbstractEvent::_GetDir()).c_str());

	
	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditEvent(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}




void Editor::EditEvent(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("EventEditor.exe").toAscii().data(), (QString("EventEditor.exe ") + _name + QString(" edit") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}

