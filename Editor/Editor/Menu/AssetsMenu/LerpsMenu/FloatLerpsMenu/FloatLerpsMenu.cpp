
#include "..\..\..\..\Editor.h"
#include <Editor/FloatLerpCreator/FloatLerpCreator.h>


void Editor::CreateFloatLerpsMenu(void)
{
	floatLerpsAssetMenu = new QMenu(lerpsAssetMenu);
	floatLerpsAssetMenu->setTitle("Float");
	lerpsAssetMenu->addMenu(floatLerpsAssetMenu);


	CreateNewFloatLerpAct();
	CreateEditFloatLerpAct();
}


void Editor::CreateNewFloatLerpAct(void)
{
	floatLerpNewAct = new QAction("New", floatLerpsAssetMenu);
	connect(floatLerpNewAct, SIGNAL(triggered()), SLOT(NewFloatLerp()));
	floatLerpsAssetMenu->addAction(floatLerpNewAct);
}


void Editor::CreateEditFloatLerpAct(void)
{
	floatLerpEditAct = new QAction("Edit", floatLerpsAssetMenu);
	connect(floatLerpEditAct, SIGNAL(triggered()), SLOT(EditFloatLerp()));
	floatLerpsAssetMenu->addAction(floatLerpEditAct);
}




void Editor::NewFloatLerp(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("FloatLerpEditor.exe").toAscii().data(), QString("FloatLerpEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable") + QString(" floatLerp")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void Editor::EditFloatLerp(void)
{ 
	QFileDialog dialog(this, "Choose float lerp");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + FloatLerp::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + FloatLerp::_GetDir()).c_str());

	
	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditFloatLerp(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}


void Editor::EditFloatLerp(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("FloatLerpEditor.exe").toAscii().data(), (QString("FloatLerpEditor.exe ") + _name + QString(" edit") + QString(" nameEnable") + QString(" floatLerp")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}