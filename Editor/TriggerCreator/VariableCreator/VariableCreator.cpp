#include "VariableCreator.h"
#include <Editor/NameCreator/NameCreator.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Editor/Editor/Editor.h>



VariableCreator::VariableCreator(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), variable(NIL), fastCreatingEnable(false)
{
	variableTable = new VariableTableWidget(this);
	variableTable->move(20, 20);
	importButton = new QPushButton(this);
    importButton->setGeometry(QRect(20+240, 100+50+30, 61-5+10, 22)); 
	importButton->setText("Import");  
	connect(importButton, SIGNAL(clicked()), this, SLOT(Import()));
	saveAsButton = new QPushButton(this);
    saveAsButton->setGeometry(QRect(20+75+240, 100+50+30, 61-5+10, 22)); 
	saveAsButton->setText("Save as...");  
	connect(saveAsButton, SIGNAL(clicked()), this, SLOT(SaveAs()));
	helpButton = new QToolButton(this);
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setAutoRaise(true);
	helpButton->setToolTip("Help");
	helpButton->setGeometry(QRect(20+250-75, 100+50+25+50-5+35, 32, 32));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));
    okButton = new QPushButton(this);
    okButton->setGeometry(QRect(20+40+250-75, 100+50+25+50+35, 81, 23));
	okButton->setText("Ok");
	connect(okButton, SIGNAL(clicked()), this, SLOT(IsOk()));
    cancelButton = new QPushButton(this);
    cancelButton->setGeometry(QRect(20+100+40+250-5-75, 100+50+25+50+35, 81, 23));
	cancelButton->setText("Cancel");
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	cancelButton->setFocus();


	setFixedSize(430, 134+100+65);
	move(460, 220);
	setWindowTitle("Variable Creator");
}


VariableCreator::~VariableCreator(void)
{
}


void VariableCreator::Import(void)
{
	QFileDialog dialog(this, "Choose variable");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + AbstractVariable::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + AbstractVariable::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}
		StringANSI name = File::_GetBaseName(assets.at(0).toStdString());

		SetVariable(name.c_str()); 
	}
}







void VariableCreator::SetNameDisable(bool _disable)
{
	variableTable->SetCellDelegate(VariableTableWidget::NAME_PROPERTY, UniversalDelegate::NO_EDIT);
}


void VariableCreator::ShowDoc(void)
{
	QString path = (Editor::_GetWorkDir() + Editor::_GetDocsDir() + "VariableEditor.doc");

	QUrl url = QUrl::fromLocalFile(path);

	QDesktopServices::openUrl(url);
}


AbstractVariable* VariableCreator::GetVariable(void)const
{
	return variable;
}


QString VariableCreator::GetVariableStringType(int32 _type)
{
	switch(_type)
	{
		case Variable<int32>::INTEGER_TYPE:
		{
			return "INTEGER";
		}

		case Variable<float>::FLOAT_TYPE:
		{
			return "FLOAT";
		}

		case Variable<bool>::BOOLEAN_TYPE:
		{
			return "BOOLEAN";
		}

		case Variable<StringANSI>::STRING_TYPE:
		{
			return "STRING";
		}

		case Variable<Vector2>::VECTOR_2_TYPE:
		{
			return "VECTOR_2";
		}

		case Variable<Vector3>::VECTOR_3_TYPE:
		{
			return "VECTOR_3";
		}

		case Variable<Quaternion>::QUATERNION_TYPE:
		{
			return "QUATERNION";
		}
	}
	return "";
} 


void VariableCreator::SetFastCreating(bool _state)
{
	fastCreatingEnable = _state;
}



void VariableCreator::SetVariable(QString _name)
{ 
	if(variable = AbstractVariable::_LoadFromFile(_name.toAscii().data()))
	{ 
		variableTable->SetDefaultState();

		switch(variable->GetValueType())
		{
			case Variable<int32>::INTEGER_TYPE:
			{ 
				Variable<int32>* derived = dynamic_cast<Variable<int32>*>(variable);
				variableTable->SetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE, GetVariableStringType(variable->GetValueType()));
				QString value = QString::number(derived->GetValue());
				variableTable->SetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE, value);
				value = QString::number(derived->GetLowerBound());
				variableTable->SetCellText(VariableTableWidget::LOWER_BOUND_PROPERTY, VariableTableWidget::VALUE, value);
				value = QString::number(derived->GetUpperBound());
				variableTable->SetCellText(VariableTableWidget::UPPER_BOUND_PROPERTY, VariableTableWidget::VALUE, value);
				break;
			}

			case Variable<float>::FLOAT_TYPE:
			{
				Variable<float>* derived = dynamic_cast<Variable<float>*>(variable);
				variableTable->SetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE, GetVariableStringType(variable->GetValueType()));
				QString value = QString::number(derived->GetValue());
				variableTable->SetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE, value);
				value = QString::number(derived->GetLowerBound());
				variableTable->SetCellText(VariableTableWidget::LOWER_BOUND_PROPERTY, VariableTableWidget::VALUE, value);
				value = QString::number(derived->GetUpperBound());
				variableTable->SetCellText(VariableTableWidget::UPPER_BOUND_PROPERTY, VariableTableWidget::VALUE, value);
				break;
			}

			case Variable<bool>::BOOLEAN_TYPE:
			{
				Variable<bool>* derived = dynamic_cast<Variable<bool>*>(variable);
				variableTable->SetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE, GetVariableStringType(variable->GetValueType()));
				QString value = derived->GetValue() ? "true" : "false";
				variableTable->SetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE, value);
				break;
			}
						
			case Variable<StringANSI>::STRING_TYPE:
			{ 
				Variable<StringANSI>* derived = dynamic_cast<Variable<StringANSI>*>(variable);
				variableTable->SetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE, GetVariableStringType(variable->GetValueType()));
				QString value = derived->GetValue().c_str();
				variableTable->SetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE, value);
				break;
			}

			case Variable<Vector2>::VECTOR_2_TYPE:
			{
				Variable<Vector2>* derived = dynamic_cast<Variable<Vector2>*>(variable);
				variableTable->SetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE, GetVariableStringType(variable->GetValueType()));
				Vector2 value = derived->GetValue();
				variableTable->SetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE, Vector2ToString(value));
				break;
			}

			case Variable<Vector3>::VECTOR_3_TYPE:
			{
				Variable<Vector3>* derived = dynamic_cast<Variable<Vector3>*>(variable);
				variableTable->SetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE, GetVariableStringType(variable->GetValueType()));
				Vector3 value = derived->GetValue();
				variableTable->SetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE, Vector3ToString(value));
				break;
			}

			case Variable<Quaternion>::QUATERNION_TYPE:
			{
				Variable<Quaternion>* derived = dynamic_cast<Variable<Quaternion>*>(variable);
				variableTable->SetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE, GetVariableStringType(variable->GetValueType()));
				Quaternion value = derived->GetValue();
				variableTable->SetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE, QuaternionToString(value));
				break;
			}
		}
		variableTable->SetCellText(VariableTableWidget::NAME_PROPERTY, VariableTableWidget::VALUE, variable->GetName().c_str());
		
		delete variable;
		variable = NIL;
	}

	setWindowTitle("Variable Editor");
}


void VariableCreator::SetObjectName(QString _name)
{
	variableTable->SetCellText(VariableTableWidget::NAME_PROPERTY, VariableTableWidget::VALUE, _name);
}




void VariableCreator::SaveAs(void)
{ 
	if(!CreateVariable())
	{
		return;
	}

	QString pathFile = QFileDialog::getSaveFileName(this,
													"Save variable as...",
													(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + variable->GetName()).c_str(),
													QString("*") + AbstractVariable::_GetFileSuffix().c_str());

	if(!pathFile.isEmpty())
	{	
		QString name = File::_GetBaseName(pathFile.toStdString()).c_str();
		if(File::_IsExist(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + name.toStdString() + "." + AbstractVariable::_GetFileSuffix()))
		{ 
			WarningDialog warning("Warning", "Variable '" + name + "' is already exist. Rewrite file?", 200, 90);
			
			if(warning.exec() == QDialog::Rejected)
			{
				return;
			}
		}
	
		variable->SaveToFile();
		emit VariableIsEdited(QString(variable->GetName().c_str()));
		delete variable;
		variable = NIL;
	}
}



QString VariableCreator::GetObjectName(void)const
{
	return variableTable->GetCellText(VariableTableWidget::NAME_PROPERTY, VariableTableWidget::VALUE);
}



void VariableCreator::SetType(int _type)
{
	variableTable->SetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE, GetVariableStringType(_type));
}




Vector3 VariableCreator::StringToVector3(QString _vector)
{
	SimpleList<StringANSI> list = SplitString(_vector.toAscii().data(), ";");

	if(list.GetSize() == 3)
	{
		float x = QString(list[0]->key.c_str()).toFloat();
		float y = QString(list[1]->key.c_str()).toFloat();
		float z = QString(list[2]->key.c_str()).toFloat();
		return Vector3(x, y, z);
	}
	return Vector3();
}


Vector2 VariableCreator::StringToVector2(QString _vector)
{
	SimpleList<StringANSI> list = SplitString(_vector.toAscii().data(), ";");

	if(list.GetSize() == 2)
	{
		float x = QString(list[0]->key.c_str()).toFloat();
		float y = QString(list[1]->key.c_str()).toFloat();
		return Vector2(x, y);
	}
	return Vector2();
}


Quaternion VariableCreator::StringToQuaternion(QString _quaternion)
{
	SimpleList<StringANSI> list = SplitString(_quaternion.toAscii().data(), ";");

	if(list.GetSize() == 4)
	{
		float x = QString(list[0]->key.c_str()).toFloat();
		float y = QString(list[1]->key.c_str()).toFloat();
		float z = QString(list[2]->key.c_str()).toFloat();
		float w = QString(list[3]->key.c_str()).toFloat();
		return Quaternion(x, y, z, w);
	}
	return Quaternion();
}





QString VariableCreator::Vector3ToString(Vector3 _vector)
{
	return QString::number(_vector.x) + ";" + QString::number(_vector.y) + ";" + QString::number(_vector.z);
}



QString VariableCreator::Vector2ToString(Vector2 _vector)
{
	return QString::number(_vector.x) + ";" + QString::number(_vector.y);
}




QString VariableCreator::QuaternionToString(Quaternion _quaternion)
{
	return QString::number(_quaternion.x) + ";" + QString::number(_quaternion.y) + ";" + QString::number(_quaternion.z) + ";" + QString::number(_quaternion.w);
}





bool VariableCreator::CreateVariable(void)
{
	int32 errorCode;
	QString invalidVariableName = variableTable->IsOk();
	
	if(invalidVariableName.length())
	{
		ErrorDialog("Error", " Variable is not valid", 130, 40);
		return false;
	}
	if(variableTable->GetCellText(VariableTableWidget::NAME_PROPERTY, VariableTableWidget::VALUE).isEmpty())
	{
		ErrorDialog("Error", "Variable name is not defined", 175, 40);
		return false;
	}
	if((errorCode = BooleanExprParser::_IsNameValid(variableTable->GetCellText(VariableTableWidget::NAME_PROPERTY, VariableTableWidget::VALUE).toAscii().data())) >= 0)
	{
		ErrorDialog("Error", "Variable name is not valid: '" + QString(variableTable->GetCellText(VariableTableWidget::NAME_PROPERTY, VariableTableWidget::VALUE).at(errorCode)) + "'", 190, 40);
		return false;
	}

	
	if(variableTable->GetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE) == "INTEGER")
	{
		variable = new Variable<int32>();
		int32 value = variableTable->GetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE).toInt();
		dynamic_cast<Variable<int32>*>(variable)->SetValue(value);
		int lowerBound = variableTable->GetCellText(VariableTableWidget::LOWER_BOUND_PROPERTY, VariableTableWidget::VALUE).toInt();
		dynamic_cast<Variable<int32>*>(variable)->SetLowerBound(lowerBound);

		int upperBound = variableTable->GetCellText(VariableTableWidget::UPPER_BOUND_PROPERTY, VariableTableWidget::VALUE).toInt();
		dynamic_cast<Variable<int32>*>(variable)->SetUpperBound(upperBound);

	} 
	else if(variableTable->GetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE) == "FLOAT")
	{
		variable = new Variable<float>();
		float value = variableTable->GetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE).toFloat();
		dynamic_cast<Variable<float>*>(variable)->SetValue(value);
		float lowerBound = variableTable->GetCellText(VariableTableWidget::LOWER_BOUND_PROPERTY, VariableTableWidget::VALUE).toFloat();
		dynamic_cast<Variable<float>*>(variable)->SetLowerBound(lowerBound);

		float upperBound = variableTable->GetCellText(VariableTableWidget::UPPER_BOUND_PROPERTY, VariableTableWidget::VALUE).toFloat();
		dynamic_cast<Variable<float>*>(variable)->SetUpperBound(upperBound);
	}
	else if(variableTable->GetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE) == "BOOLEAN")
	{	
		variable = new Variable<bool>();
		bool value = variableTable->GetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE) == "true" ? true : false;
		dynamic_cast<Variable<bool>*>(variable)->SetValue(value);
	}
	else if(variableTable->GetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE) == "STRING")
	{
		variable = new Variable<StringANSI>();
		StringANSI value = variableTable->GetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE).toAscii().data();
		dynamic_cast<Variable<StringANSI>*>(variable)->SetValue(value);
	}
	else if(variableTable->GetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE) == "VECTOR_3")
	{ 	
		variable = new Variable<Vector3>();
		Vector3 value = StringToVector3(variableTable->GetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE));
		dynamic_cast<Variable<Vector3>*>(variable)->SetValue(value);
	}
	else if(variableTable->GetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE) == "VECTOR_2")
	{ 	
		variable = new Variable<Vector2>();
		Vector2 value = StringToVector2(variableTable->GetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE));
		dynamic_cast<Variable<Vector2>*>(variable)->SetValue(value);
	} 
	else if(variableTable->GetCellText(VariableTableWidget::TYPE_PROPERTY, VariableTableWidget::VALUE) == "QUATERNION")
	{ 	
		variable = new Variable<Quaternion>();
		Quaternion value = StringToQuaternion(variableTable->GetCellText(VariableTableWidget::VALUE_PROPERTY, VariableTableWidget::VALUE));
		dynamic_cast<Variable<Quaternion>*>(variable)->SetValue(value);
	}
	variable->Rename(variableTable->GetCellText(VariableTableWidget::NAME_PROPERTY, VariableTableWidget::VALUE).toAscii().data());

	return true;
}



void VariableCreator::IsOk(void)
{ 
	if(!CreateVariable())
	{
		return;
	}
	if(File::_IsExist(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + StringANSI(variableTable->GetCellText(VariableTableWidget::NAME_PROPERTY, VariableTableWidget::VALUE).toAscii().data()) + "." + AbstractVariable::_GetFileSuffix()))
	{
		WarningDialog warning("Warning", "Variable '" + QString(variableTable->GetCellText(VariableTableWidget::NAME_PROPERTY, VariableTableWidget::VALUE).toAscii().data()) + "' is already exist. Rewrite file?", 200, 90);
		
		if(warning.exec() == QDialog::Rejected)
		{
			delete variable;
			variable = NIL;
			return;
		}
	}

	if(!fastCreatingEnable)
	{
		variable->SaveToFile();
		emit VariableIsEdited(QString(variable->GetName().c_str()));
		delete variable;
		variable = NIL;
	}
	else
	{
		if(AssetLibrary::_GetAssets().AddObject(variable) == NIL)
		{
			ErrorDialog("Error", "Name '" + variableTable->GetCellText(VariableTableWidget::NAME_PROPERTY, VariableTableWidget::VALUE) + "' is already reserved", 195, 40);
			delete variable;
			variable = NIL;
			return;
		}
	}

	accept();
}
