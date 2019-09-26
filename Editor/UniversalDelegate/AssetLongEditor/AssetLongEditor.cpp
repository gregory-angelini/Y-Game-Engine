
#include "AssetLongEditor.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Time/Timer/Timer.h>


AssetLongEditor::AssetLongEditor(QWidget* _parent): QWidget(_parent)
{
	standardFormat.setForeground(Qt::black);
	standardFormat.setFontPointSize(10);
	unknownArgFormat.setForeground(Qt::black);
	unknownArgFormat.setBackground(QColor(231,186,188));
	fileNameEdit = new StringLineEdit(this);
	fileNameEdit->setGeometry(QRect(0, 0, 90, 24));
	connect(fileNameEdit, SIGNAL(FocusOut()), this, SLOT(FocusOut()));
	connect(fileNameEdit, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
	openFileButton = new QToolButton(this);
	openFileButton->setGeometry(QRect(90, 0, 26, 26));
	openFileButton->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/finder.png"));
	openFileButton->setFixedSize(QSize(26, 26));
	openFileButton->setIconSize(QSize(26, 26));
	openFileButton->setToolTip("finder");
	connect(openFileButton, SIGNAL(clicked()), this, SLOT(OpenFinder()));
	editAssetButton = new QToolButton(this);
	editAssetButton->setGeometry(QRect(90+24-1-1, -1, 50+3, 24+2));
	editAssetButton->setText("create");
	editAssetButton->setEnabled(false);
}


void AssetLongEditor::FocusOut(void)
{
	emit FocusOut(this);
}



void AssetLongEditor::SetFileName(QString _fileName)
{
	fileNameEdit->setText(_fileName);
}


QString AssetLongEditor::GetFileName(void)const
{
	return fileNameEdit->text();
}



void AssetLongEditor::NameIsChanged(QString _name)
{  
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateTexturedAnimation()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTexturedAnimation()));
	
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateAtlas()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditAtlas()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateTexture()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTexture()));


	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateTrigger()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTrigger()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateAction()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditAction()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateNewEvent()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditEvent()));


	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateVariable()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditVariable()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateList()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditList()));


	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateObject()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditObject()));
	

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreatePlayerControl()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditPlayerControl()));
	
		
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateMesh()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditMesh()));
 
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateHitbox()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditHitbox()));
 

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateScene()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditScene()));
	
	
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateFloatLerp()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditFloatLerp()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateQuaternionLerp()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditQuaternionLerp()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateVectorLerp()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditVectorLerp()));
 

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateCamera()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditCamera()));


	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateTimer()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTimer()));


	CheckArg();
	
 
	if(fileType == TexturedAnimation::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + TexturedAnimation::_GetDir() + _name.toStdString() + "." + TexturedAnimation::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTexturedAnimation()));
			}
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateTexturedAnimation()));
			} 
		}
	}  
	else if(fileType == Atlas2D::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + Atlas2D::_GetDir() + _name.toStdString() + "." + Atlas2D::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditAtlas()));
			}
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateAtlas()));
			} 
		}
	}  
	else if(fileType == Texture2D::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + Texture2D::_GetDir() + _name.toStdString() + "." + Texture2D::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTexture()));
			}
			else 
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateTexture()));
			}
		}
	}  
	else if(fileType == Trigger::_GetFileSuffix().c_str())
	{ 
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + Trigger::_GetDir() + _name.toStdString() + "." + Trigger::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTrigger()));
			} 
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateTrigger()));
			} 
		}
	} 
	else if(fileType == AbstractAction::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + AbstractAction::_GetDir() + _name.toStdString() + "." + AbstractAction::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditAction()));
			}
			else
			{ 
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateAction()));
			}
		}
	}  
	else if(fileType == AbstractEvent::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + AbstractEvent::_GetDir() + _name.toStdString() + "." + AbstractEvent::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditEvent()));
			}
			else  
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateNewEvent()));
			}
		}
	}  
	else if(fileType == AbstractVariable::_GetFileSuffix().c_str())
	{ 
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + _name.toStdString() + "." + AbstractVariable::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditVariable()));
			} 
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateVariable()));
			} 
		}
	}   
	else if(fileType == AbstractList::_GetFileSuffix().c_str())
	{ 
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else
		{ 
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + AbstractList::_GetDir() + _name.toStdString() + "." + AbstractList::_GetFileSuffix()))
			{
				editAssetButton->setText("review");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditList()));
			}
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateList()));
			}
		}
	}
	else if(fileType == AbstractObject::_GetFileSuffix().c_str())
	{ 
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else
		{ 
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name.toStdString() + "." + AbstractObject::_GetFileSuffix()))
			{
				editAssetButton->setText("review");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditObject()));
			}
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateObject()));
			}
		}
	}  
	else if(fileType == PlayerControl::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + PlayerControl::_GetDir() + _name.toStdString() + "." + PlayerControl::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditPlayerControl()));
			}
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreatePlayerControl()));
			} 
		}
	}
	else if(fileType == Mesh::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{ 
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + Mesh::_GetDir() + _name.toStdString() + "." + Mesh::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditMesh()));
			}
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateMesh()));
			} 
		} 
	} 
	else if(fileType == Hitbox::_GetFileSuffix().c_str())
	{ 
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + Hitbox::_GetDir() + _name.toStdString() + "." + Hitbox::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditHitbox()));
			}
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateHitbox()));
			} 
		}
	}
	else if(fileType == ObjectScene::_GetFileSuffix().c_str())
	{ 
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else
		{ 
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + ObjectScene::_GetDir() + _name.toStdString() + "." + ObjectScene::_GetFileSuffix()))
			{
				editAssetButton->setText("review");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditScene()));
			}
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateScene()));
			}
		}
	}  
	else if(fileType == FloatLerp::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + FloatLerp::_GetDir() + _name.toStdString() + "." + FloatLerp::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditFloatLerp()));
			}
			else
			{ 
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateFloatLerp()));
			}
		}
	}  
	else if(fileType == QuaternionLerp::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + QuaternionLerp::_GetDir() + _name.toStdString() + "." + QuaternionLerp::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditQuaternionLerp()));
			}
			else 
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateQuaternionLerp()));
			}
		}
	}  
	else if(fileType == VectorLerp::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else  
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + VectorLerp::_GetDir() + _name.toStdString() + "." + VectorLerp::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditVectorLerp()));
			}
			else  
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateVectorLerp()));
			}
		}
	}
	else if(fileType == Camera3D::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + Camera3D::_GetDir() + _name.toStdString() + "." + Camera3D::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditCamera()));
			}
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateCamera()));
			}
		} 
	}
	else if(fileType == Timer::_GetFileSuffix().c_str())
	{
		if(_name.isEmpty())
		{
			editAssetButton->setEnabled(false);
		}
		else 
		{
			editAssetButton->setEnabled(true);
			if(File::_IsExist(AssetLibrary::_GetDir() + Timer::_GetDir() + _name.toStdString() + "." + Timer::_GetFileSuffix()))
			{
				editAssetButton->setText("edit");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTimer()));
			}
			else
			{
				editAssetButton->setText("create");
				connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateTimer()));
			}
		} 
	}  
}

 
void AssetLongEditor::CreateTrigger(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TriggerEditor.exe").toAscii().data(), (QString("TriggerEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditTrigger(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TriggerEditor.exe").toAscii().data(), (QString("TriggerEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::CreateVariable(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo; 
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("VariableEditor.exe").toAscii().data(), (QString("VariableEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditVariable(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("VariableEditor.exe").toAscii().data(), (QString("VariableEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::CreateObject(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditObject(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + fileType + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetLongEditor::CreateScene(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditScene(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + fileType + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetLongEditor::CreateCamera(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditCamera(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + fileType + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::CreateTimer(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditTimer(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + fileType + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetLongEditor::CreateList(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditList(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + fileType + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetLongEditor::CreateAction(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ActionEditor.exe").toAscii().data(), (QString("ActionEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditAction(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ActionEditor.exe").toAscii().data(), (QString("ActionEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}




void AssetLongEditor::CreateNewEvent(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("EventEditor.exe").toAscii().data(), (QString("EventEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditEvent(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("EventEditor.exe").toAscii().data(), (QString("EventEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetLongEditor::CreateFloatLerp(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("FloatLerpEditor.exe").toAscii().data(), (QString("FloatLerpEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable") + QString(" floatLerp")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditFloatLerp(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("FloatLerpEditor.exe").toAscii().data(), (QString("FloatLerpEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable") + QString(" floatLerp")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetLongEditor::CreatePlayerControl(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("PlayerControlEditor.exe").toAscii().data(), (QString("PlayerControlEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditPlayerControl(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("PlayerControlEditor.exe").toAscii().data(), (QString("PlayerControlEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetLongEditor::CreateTexturedAnimation(void)
{ 
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TexturedAnimationEditor.exe").toAscii().data(), (QString("TexturedAnimationEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable") + QString(" texturedAnimation")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditTexturedAnimation(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TexturedAnimationEditor.exe").toAscii().data(), (QString("TexturedAnimationEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable") + QString(" texturedAnimation")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}



void AssetLongEditor::CreateAtlas(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TexturedAnimationEditor.exe").toAscii().data(), (QString("TexturedAnimationEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable") + QString(" atlas")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}
	
	
void AssetLongEditor::EditAtlas(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TexturedAnimationEditor.exe").toAscii().data(), (QString("TexturedAnimationEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable") + QString(" atlas")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}



void AssetLongEditor::CreateVectorLerp(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("VectorLerpEditor.exe").toAscii().data(), (QString("VectorLerpEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable") + QString(" vectorLerp")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditVectorLerp(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("VectorLerpEditor.exe").toAscii().data(), (QString("VectorLerpEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable")  + QString(" vectorLerp")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}




void AssetLongEditor::CreateMesh(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), (QString("TextureEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable") + QString(" hitbox")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditMesh(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), (QString("TextureEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable")  + QString(" hitbox")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}




void AssetLongEditor::CreateHitbox(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), (QString("TextureEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable") + QString(" hitbox")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetLongEditor::EditHitbox(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), (QString("TextureEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable")  + QString(" hitbox")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}



void AssetLongEditor::CreateTexture(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), (QString("TextureEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable") + QString(" texture")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}
	
	
void AssetLongEditor::EditTexture(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), (QString("TextureEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable") + QString(" texture")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}


void AssetLongEditor::CreateQuaternionLerp(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("QuaternionLerpEditor.exe").toAscii().data(), (QString("QuaternionLerpEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}
	
	
void AssetLongEditor::EditQuaternionLerp(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("QuaternionLerpEditor.exe").toAscii().data(), (QString("QuaternionLerpEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetLongEditor::OpenFinder(void)
{
	if(fileType == TexturedAnimation::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose textured animation");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + TexturedAnimation::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == Atlas2D::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose atlas");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() +  Atlas2D::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == Texture2D::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose texture");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() +  Texture2D::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	} 
	else if(fileType == Trigger::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose trigger");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + Trigger::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == AbstractEvent::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose event");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + AbstractEvent::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == AbstractAction::_GetFileSuffix().c_str())
	{  
		QFileDialog dialog(this, "Choose action");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + AbstractAction::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == AbstractVariable::_GetFileSuffix().c_str())
	{ 
		QFileDialog dialog(this, "Choose variable");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + AbstractVariable::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == AbstractList::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose list");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + AbstractList::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == AbstractObject::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose object");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + AbstractObject::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			} 
		}
	}
	else if(fileType == PlayerControl::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose player control");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + PlayerControl::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == Mesh::_GetFileSuffix().c_str())
	{ 
		QFileDialog dialog(this, "Choose mesh");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + Mesh::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == Hitbox::_GetFileSuffix().c_str())
	{ 
		QFileDialog dialog(this, "Choose hitbox");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + Hitbox::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == ObjectScene::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose scene");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() + ObjectScene::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == FloatLerp::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose float lerp");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() +  FloatLerp::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == VectorLerp::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose vector lerp");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() +  VectorLerp::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == QuaternionLerp::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose quaternion lerp");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() +  QuaternionLerp::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
	else if(fileType == Camera3D::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose camera");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() +  Camera3D::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	} 
	else if(fileType == Timer::_GetFileSuffix().c_str())
	{
		QFileDialog dialog(this, "Choose timer");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(QString("*.") + fileType.toAscii().data());
		dialog.setViewMode(QFileDialog::List);
		dialog.setDirectory((AssetLibrary::_GetDir() +  Timer::_GetDir()).c_str());

		if(dialog.exec())
		{
			QStringList path = dialog.selectedFiles();
					
			if(path.size())
			{
				QFileInfo fileInfo(path.at(0));

				QString fileName = fileInfo.completeBaseName();
	
				fileNameEdit->setText(fileName);
			}
		}
	}
}
 


void AssetLongEditor::SetFileType(QString _fileType)
{
	fileType = _fileType;
}

	

void AssetLongEditor::CheckArg(void)
{
	_ClearLineEditTextFormat(fileNameEdit);

	QList<QTextLayout::FormatRange> formats;
	QString string = fileNameEdit->text();
	int index = 0;

	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = string.length();
		format.format = standardFormat;
		formats.append(format);
	}
	int32 errorCode;
	if((errorCode = BooleanExprParser::_IsNameValid(string.toStdString())) >= 0)
	{
		QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(string.at(errorCode)) + "'";
		fileNameEdit->setToolTip(text);

		QTextLayout::FormatRange format;
		format.start = errorCode;
		format.length = 1;
		format.format = unknownArgFormat;
		formats.append(format);
	}
	else { fileNameEdit->setToolTip(""); } 


	index = 0;
	bool exist = false;
	if(AssetLibrary::_IsAssetExist(fileNameEdit->text().toStdString(), AssetLibrary::_GetAssetTypeOfFileSuffix(fileType.toStdString())))
	{
		exist = true;
	}

	if(!exist)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = string.length();
		format.format = unknownArgFormat;
		formats.append(format);

		if(errorCode < 0)
		{
			QString text = "Asset does not exist";
			fileNameEdit->setToolTip(text);
		}
	}
	else 
	{ 
		if(errorCode < 0)
		{
			fileNameEdit->setToolTip(""); 
		}
	} 

	_SetLineEditTextFormat(fileNameEdit, formats);
}



void AssetLongEditor::_SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats)
{
    if(_lineEdit)
	{
		QList<QInputMethodEvent::Attribute> attributes;

		foreach(const QTextLayout::FormatRange& formatRange, _formats)
		{
			QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
			int start = formatRange.start - _lineEdit->cursorPosition();
			int length = formatRange.length;
			QVariant value = formatRange.format;
			attributes.append(QInputMethodEvent::Attribute(type, start, length, value));
		}

		QInputMethodEvent event(QString(), attributes);
		QCoreApplication::sendEvent(_lineEdit, &event);
	}
}



void AssetLongEditor::_ClearLineEditTextFormat(QLineEdit* _lineEdit)
{
    _SetLineEditTextFormat(_lineEdit, QList<QTextLayout::FormatRange>());
}



void AssetLongEditor::resizeEvent(QResizeEvent* _event)
{
	fileNameEdit->setGeometry(QRect(0, 0, _event->size().width() - 26 - 50+2, 24));
	openFileButton->setGeometry(QRect(_event->size().width() - 26 - 50, -1, 26, 24));
	editAssetButton->setGeometry(QRect(_event->size().width() - 50-1-1, -1, 50+3, 24+2));
	QWidget::resizeEvent(_event);
}

