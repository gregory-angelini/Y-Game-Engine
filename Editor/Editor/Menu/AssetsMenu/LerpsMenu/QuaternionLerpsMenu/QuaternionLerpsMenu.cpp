
#include "..\..\..\..\Editor.h"
#include <Editor/QuaternionLerpCreator/QuaternionLerpCreator.h>


void Editor::CreateQuaternionLerpsMenu(void)
{
	quaternionLerpsAssetMenu = new QMenu(lerpsAssetMenu);
	quaternionLerpsAssetMenu->setTitle("Quaternion");
	lerpsAssetMenu->addMenu(quaternionLerpsAssetMenu);


	CreateNewQuaternionLerpAct();
	CreateEditQuaternionLerpAct();
}


void Editor::CreateNewQuaternionLerpAct(void)
{
	quaternionLerpNewAct = new QAction("New", quaternionLerpsAssetMenu);
	connect(quaternionLerpNewAct, SIGNAL(triggered()), SLOT(NewQuaternionLerp()));
	quaternionLerpsAssetMenu->addAction(quaternionLerpNewAct);
}


void Editor::CreateEditQuaternionLerpAct(void)
{
	quaternionLerpEditAct = new QAction("Edit", quaternionLerpsAssetMenu);
	connect(quaternionLerpEditAct, SIGNAL(triggered()), SLOT(EditQuaternionLerp()));
	quaternionLerpsAssetMenu->addAction(quaternionLerpEditAct);
}




void Editor::NewQuaternionLerp(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("QuaternionLerpEditor.exe").toAscii().data(), QString("QuaternionLerpEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void Editor::EditQuaternionLerp(void)
{ 
	QFileDialog dialog(this, "Choose quaternion lerp");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + QuaternionLerp::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + QuaternionLerp::_GetDir()).c_str());

	
	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditQuaternionLerp(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}


void Editor::EditQuaternionLerp(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("QuaternionLerpEditor.exe").toAscii().data(), (QString("QuaternionLerpEditor.exe ") + _name + QString(" edit") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}