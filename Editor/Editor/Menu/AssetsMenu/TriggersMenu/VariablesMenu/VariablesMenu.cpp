
#include "..\..\..\..\Editor.h"
#include <Editor/TriggerCreator/VariableCreator/VariableCreator.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/WarningDialog/WarningDialog.h>


void Editor::CreateVariablesMenu(void)
{
	variablesAssetMenu = new QMenu(triggersAssetMenu);
	variablesAssetMenu->setTitle("Variables");
	triggersAssetMenu->addMenu(variablesAssetMenu);

	CreateNewVariableAct();
	CreateEditVariableAct();
}



void Editor::CreateNewVariableAct(void)
{
	variableNewAct = new QAction("New", variablesAssetMenu);
	connect(variableNewAct, SIGNAL(triggered()), SLOT(NewVariable()));
	variablesAssetMenu->addAction(variableNewAct);
}


void Editor::NewVariable(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("VariableEditor.exe").toAscii().data(), QString("VariableEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void Editor::CreateEditVariableAct(void)
{
	variablesEditAct = new QAction("Edit", variablesAssetMenu);
	connect(variablesEditAct, SIGNAL(triggered()), SLOT(EditVariable()));
	variablesAssetMenu->addAction(variablesEditAct);
}




void Editor::EditVariable(void)
{
	QFileDialog dialog(this, "Choose variable");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + AbstractVariable::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + AbstractVariable::_GetDir()).c_str());

	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditVariable(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}




void Editor::EditVariable(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("VariableEditor.exe").toAscii().data(), (QString("VariableEditor.exe ") + _name + QString(" edit") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}

