
#include "..\..\..\..\Editor.h"
#include <Editor/TextureCreator/TextureCreator.h>



void Editor::CreateTexturedAnimationsMenu(void)
{
	texturedAnimationsAssetMenu = new QMenu(animationsAssetMenu);
	texturedAnimationsAssetMenu->setTitle("Textured");
	animationsAssetMenu->addMenu(texturedAnimationsAssetMenu);

	CreateNewTexturedAnimationAct();
	CreateEditTexturedAnimationAct();
	CreateAtlasesMenu();
}


void Editor::CreateNewTexturedAnimationAct(void)
{
	texturedAnimationNewAct = new QAction("New", texturedAnimationsAssetMenu);
	connect(texturedAnimationNewAct, SIGNAL(triggered()), this, SLOT(NewTexturedAnimation()));
	texturedAnimationsAssetMenu->addAction(texturedAnimationNewAct);
}



void Editor::CreateEditTexturedAnimationAct(void)
{
	texturedAnimationEditAct = new QAction("Edit", texturedAnimationsAssetMenu);
	connect(texturedAnimationEditAct, SIGNAL(triggered()), this, SLOT(EditTexturedAnimation()));
	texturedAnimationsAssetMenu->addAction(texturedAnimationEditAct);
}




void Editor::NewTexturedAnimation(void)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TexturedAnimationEditor.exe").toAscii().data(), QString("TexturedAnimationEditor.exe" + QString(" unnamed") + QString(" new") + QString(" nameEnable") + QString(" texturedAnimation")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void Editor::EditTexturedAnimation(void)
{
	QFileDialog dialog(this, "Choose textured animation");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + TexturedAnimation::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + TexturedAnimation::_GetDir()).c_str());

	
	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
	
		if(path.size() != 0)
		{
			EditTexturedAnimation(File::_GetBaseName(path.at(0).toAscii().data()).c_str());
		}
	}
}



void Editor::EditTexturedAnimation(QString _name)
{
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TexturedAnimationEditor.exe").toAscii().data(), (QString("TexturedAnimationEditor.exe ") + _name + QString(" edit") + QString(" nameEnable") + QString(" texturedAnimation")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}

