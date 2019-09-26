#include "AssetShortEditor.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Time/Timer/Timer.h>


AssetShortEditor::AssetShortEditor(QWidget* _parent): QWidget(_parent)
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
	connect(openFileButton, SIGNAL(clicked()), this, SLOT(CreateFinder()));
	editAssetButton = new QToolButton(this);
	editAssetButton->setGeometry(QRect(90+24-1-1, -1, 50+3, 24+2));
	editAssetButton->setText("create");
	editAssetButton->setEnabled(false);
}



void AssetShortEditor::FocusOut(void)
{
	emit FocusOut(this);
}


void AssetShortEditor::NameIsChanged(QString _name)
{ 
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTexturedAnimation()));
	
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditAtlas()));
 
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTexture()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTrigger()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditAction()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditEvent()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditVariable()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditList()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateObject()));
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditObject()));
		
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditPlayerControl()));
	
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditMesh()));
 	
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditHitbox()));
 
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditScene()));
	
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditFloatLerp()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditQuaternionLerp()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditVectorLerp()));
 
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditCamera()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTimer()));


	CheckArg();

	
	if(_name.isEmpty())
	{
		editAssetButton->setEnabled(false);
	}
	else
	{
		editAssetButton->setEnabled(true);
	}
	if(File::_IsExist(AssetLibrary::_GetDir() + TexturedAnimation::_GetDir() + _name.toStdString() + "." + TexturedAnimation::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTexturedAnimation()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + Atlas2D::_GetDir() + _name.toStdString() + "." + Atlas2D::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditAtlas()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + Texture2D::_GetDir() + _name.toStdString() + "." + Texture2D::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTexture()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + Trigger::_GetDir() + _name.toStdString() + "." + Trigger::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTrigger()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + AbstractAction::_GetDir() + _name.toStdString() + "." + AbstractAction::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditAction()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + AbstractEvent::_GetDir() + _name.toStdString() + "." + AbstractEvent::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditEvent()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + _name.toStdString() + "." + AbstractVariable::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditVariable()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + AbstractList::_GetDir() + _name.toStdString() + "." + AbstractList::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditList()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name.toStdString() + "." + AbstractObject::_GetFileSuffix()))
	{
		editAssetButton->setText("review");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditObject()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + PlayerControl::_GetDir() + _name.toStdString() + "." + PlayerControl::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditPlayerControl()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + Mesh::_GetDir() + _name.toStdString() + "." + Mesh::_GetFileSuffix()))
	{ 
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditMesh()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + Hitbox::_GetDir() + _name.toStdString() + "." + Hitbox::_GetFileSuffix()))
	{  
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditHitbox()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + ObjectScene::_GetDir() + _name.toStdString() + "." + ObjectScene::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditScene()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + FloatLerp::_GetDir() + _name.toStdString() + "." + FloatLerp::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditFloatLerp()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + QuaternionLerp::_GetDir() + _name.toStdString() + "." + QuaternionLerp::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditQuaternionLerp()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + VectorLerp::_GetDir() + _name.toStdString() + "." + VectorLerp::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditVectorLerp()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + Camera3D::_GetDir() + _name.toStdString() + "." + Camera3D::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditCamera()));
	}
	else if(File::_IsExist(AssetLibrary::_GetDir() + Timer::_GetDir() + _name.toStdString() + "." + Timer::_GetFileSuffix()))
	{
		editAssetButton->setText("edit");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditTimer()));
	}
	else
	{
		editAssetButton->setText("create");
		connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateObject()));
	}
}



void AssetShortEditor::EditTrigger(void)
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



void AssetShortEditor::EditVariable(void)
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



void AssetShortEditor::EditTimer(void)
{ 
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + QString("object") + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetShortEditor::EditCamera(void)
{ 
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + QString("object") + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetShortEditor::EditList(void)
{ 
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + QString("object") + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetShortEditor::EditScene(void)
{ 
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + QString("object") + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void AssetShortEditor::CreateObject(void)
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


void AssetShortEditor::EditObject(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + QString("object") + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}



void AssetShortEditor::EditAction(void)
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



void AssetShortEditor::EditEvent(void)
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



void AssetShortEditor::EditFloatLerp(void)
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




void AssetShortEditor::EditPlayerControl(void)
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



void AssetShortEditor::EditTexturedAnimation(void)
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



	
void AssetShortEditor::EditAtlas(void)
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


	
void AssetShortEditor::EditTexture(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), (QString("TextureEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable") + QString( " texture")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}


	
void AssetShortEditor::EditMesh(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("HitboxEditor.exe").toAscii().data(), (QString("HitboxEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable") + QString( " hitbox")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}



void AssetShortEditor::EditHitbox(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("HitboxEditor.exe").toAscii().data(), (QString("HitboxEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable") + QString( " hitbox")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}


	
void AssetShortEditor::EditVectorLerp(void)
{
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("VectorLerpEditor.exe").toAscii().data(), (QString("VectorLerpEditor.exe ") + QString(name.c_str()) + QString(" edit") + QString(" nameDisable") + QString( "vectorLerp")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	} 
}




void AssetShortEditor::EditQuaternionLerp(void)
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


void AssetShortEditor::CheckArg(void)
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
	if(AssetLibrary::_IsAssetExist(fileNameEdit->text().toStdString()) != AssetLibrary::UNKNOWN_ASSET)
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



void AssetShortEditor::CreateFinder(void)
{
	QFileDialog dialog(this, "Choose asset");
	dialog.setFileMode(QFileDialog::ExistingFile);

	dialog.setNameFilter(QString("*.") + AbstractObject::_GetFileSuffix().c_str() + 
		                 QString(" *.") + ObjectScene::_GetFileSuffix().c_str() + 
						 QString(" *.") + Trigger::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractEvent::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractAction::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractVariable::_GetFileSuffix().c_str() + 
						 QString(" *.") + AbstractList::_GetFileSuffix().c_str() + 
						 QString(" *.") + PlayerControl::_GetFileSuffix().c_str() + 
						 QString(" *.") + FloatLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + VectorLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + QuaternionLerp::_GetFileSuffix().c_str() + 
						 QString(" *.") + Camera3D::_GetFileSuffix().c_str() + 
						 QString(" *.") + Timer::_GetFileSuffix().c_str() + 
						 QString(" *.") + Mesh::_GetFileSuffix().c_str() + 
						 QString(" *.") + Hitbox::_GetFileSuffix().c_str() + 
						 QString(" *.") + Texture2D::_GetFileSuffix().c_str() + 
						 QString(" *.") + Atlas2D::_GetFileSuffix().c_str() + 
						 QString(" *.") + TexturedAnimation::_GetFileSuffix().c_str());
	
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory(AssetLibrary::_GetDir().c_str());

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



void AssetShortEditor::SetFileName(QString _fileName)
{
	fileNameEdit->setText(_fileName);
}


QString AssetShortEditor::GetFileName(void)const
{
	return fileNameEdit->text();
}


void AssetShortEditor::_SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats)
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



void AssetShortEditor::_ClearLineEditTextFormat(QLineEdit* _lineEdit)
{
    _SetLineEditTextFormat(_lineEdit, QList<QTextLayout::FormatRange>());
}




void AssetShortEditor::resizeEvent(QResizeEvent* _event)
{
	fileNameEdit->setGeometry(QRect(0, 0, _event->size().width() - 26 - 50+2, 24));
	openFileButton->setGeometry(QRect(_event->size().width() - 26 - 50, -1, 26, 24));
	editAssetButton->setGeometry(QRect(_event->size().width() - 50-1-1, -1, 50+3, 24+2));
	QWidget::resizeEvent(_event);
}
