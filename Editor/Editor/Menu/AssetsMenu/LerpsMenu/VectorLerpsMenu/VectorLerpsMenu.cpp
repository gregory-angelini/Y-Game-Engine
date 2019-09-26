
#include "..\..\..\..\Editor.h"
#include <Editor/TextureCreator/TextureCreator.h>


void Editor::CreateVectorLerpsMenu(void)
{
	vectorLerpsAssetMenu = new QMenu(lerpsAssetMenu);
	vectorLerpsAssetMenu->setTitle("Vector");
	lerpsAssetMenu->addMenu(vectorLerpsAssetMenu);
	
	CreateNewVectorLerpAct();
	CreateEditVectorLerpAct();
}



void Editor::CreateEditVectorLerpAct(void)
{
	vectorLerpEditAct = new QAction("Edit", vectorLerpsAssetMenu);
	connect(vectorLerpEditAct, SIGNAL(triggered()), SLOT(EditVectorLerp()));
	vectorLerpsAssetMenu->addAction(vectorLerpEditAct);
}


void Editor::CreateNewVectorLerpAct(void)
{
	vectorLerpNewAct = new QAction("New", vectorLerpsAssetMenu);
	connect(vectorLerpNewAct, SIGNAL(triggered()), SLOT(NewVectorLerp()));
	vectorLerpsAssetMenu->addAction(vectorLerpNewAct);
}


void Editor::NewVectorLerp(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("VectorLerpEditor.exe").toAscii().data(), QString("VectorLerpEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable") + QString(" vectorLerp")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void Editor::EditVectorLerp(void)
{
	QFileDialog dialog(this, "Choose vector lerp");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + VectorLerp::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() +  VectorLerp::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditVectorLerp(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}


void Editor::EditVectorLerp(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("VectorLerpEditor.exe").toAscii().data(), (QString("VectorLerpEditor.exe ") + _name + QString(" edit") + QString(" nameEnable") + QString(" vectorLerp")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}

