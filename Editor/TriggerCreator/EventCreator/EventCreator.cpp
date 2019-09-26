#include "EventCreator.h"
#include <Editor/NameCreator/NameCreator.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Core/Trigger/Event/Events.h>



EventCreator::EventCreator(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), event(NIL), fastCreatingEnable(false)
{
	eventStateLabel = new QLabel(this);
    eventStateLabel->setText("Event state:");
	eventStateLabel->setGeometry((630+100+50+50+15+10-5+200-3+150+50+22-150-50)/2, 5, 100, 21);
	eventTable = new EventTableWidget(this);
	eventTable->move(20, 20+10);
	autoNameLabel = new QLabel(this);
    autoNameLabel->setGeometry(QRect(20+5+30-5+600+300+80, 5, 101, 21));
	autoNameLabel->setText("Auto name:");
	
	autoNameCheckBox = new QCheckBox(this);
    autoNameCheckBox->setGeometry(QRect(20+90+5+10-20+30-10-5+600+300+80, 5, 16, 21));
	autoNameCheckBox->setChecked(eventTable->IsAutoNameEnabled());
	connect(autoNameCheckBox, SIGNAL(stateChanged(int)), this, SLOT(AutoNameIsChanged(int)));
	line = new QFrame(this);
    line->setGeometry(QRect(20, 20+201+10-5+50+25-5-3+10+51, 400+400+400+60+40+20-150-70-40-10-10+5+50+20+3, 21));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
	assetsLibraryLabel = new QLabel(this);
    assetsLibraryLabel->setText("Assets library:");
	assetsLibraryLabel->setGeometry((630+100+50+50+15+10-5+200-3+150+50+22-150-50)/2, 20+270+10-5-2+17-10-7+20-20+20+51, 100, 21);
    assetsLibraryTree = new AssetsLibraryTree(this); 
	assetsLibraryTree->resize(630+100+50+50+15+10-5+200-3+150+50+22-150, 270+10-5-2+17-10-7+30+10-51);  
	assetsLibraryTree->move(20, 20+270+10-5-2+17-10-7+20+20+51);
	assetsLibraryTree->SetCreatingAssetsEnable(true);
	assetsLibraryTree->SetEditingAssetsEnable(true);
	connect(assetsLibraryTree, SIGNAL(RefreshIsFinished()), this, SLOT(RefreshEventTable()));
    connect(assetsLibraryTree, SIGNAL(AssetIsRemoved()), this, SLOT(RefreshEventTable()));
	refreshAssetsLibraryPushButton = new QPushButton(this);
    refreshAssetsLibraryPushButton->setText("Refresh"); 
	refreshAssetsLibraryPushButton->setFocusPolicy(Qt::NoFocus);
	connect(refreshAssetsLibraryPushButton, SIGNAL(clicked()), this, SLOT(RefreshAssetsLibrary()));
    refreshAssetsLibraryPushButton->setGeometry(QRect(250+500+100-10+140+70-400+120+20+60+100-30+40+70-50+30+50+50+15-150-70-40-10+50+20-500-380, 460-30+200-400-70+260-15-5-36-50-50+60+320+15-5, 81, 25));
	helpButton = new QToolButton(this);
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setAutoRaise(true);
	helpButton->setToolTip("Help");
	helpButton->setFocusPolicy(Qt::NoFocus);
	helpButton->setGeometry(QRect(250+500+100-10+140+70-400+120+20+60+100-30+40+70-50-50+30+50+50+15-150-70-40-10+50+20, 460-30+200-400-70+260-15-5-36-50-50+60+320+15, 32, 32));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));
    okButton = new QPushButton(this);
    okButton->setGeometry(QRect(350+500+100-10+140+70-400+120+20+60+100-30+40+70-50+30+50+50+15-150-70-40-10+50+20, 460-30+200-400-70+260-15-5-36-50-50+60+320+15, 81, 31));
	okButton->setText("Ok");
	okButton->setFocusPolicy(Qt::NoFocus);
	connect(okButton, SIGNAL(clicked()), this, SLOT(IsOk()));
    cancelButton = new QPushButton(this);
    cancelButton->setGeometry(QRect(250+500+100-10+140+70-400+120+20+60+100-30+40+70-50+30+50+50+15-150-70-40-10+50+20, 460-30+200-400-70+260-15-5-36-50-50+60+320+15, 81, 31));
	cancelButton->setText("Cancel");
	cancelButton->setFocusPolicy(Qt::NoFocus);
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	
	setFixedSize(493+500+100-10+150+70+10+10-400+120+55+100-30+40+70+80-130+15+50+50+15-150-70-40-10+50+20, 504-30+200-400-70+256+30-30-15-36-50-50+60+320+20-5);
	move(20+60, 5);
	setWindowTitle("Event Creator");
}



EventCreator::~EventCreator(void)
{
}



void EventCreator::RefreshEventTable(void)
{
	eventTable->UpdateStatus();
}


void EventCreator::RefreshAssetsLibrary(void)
{
	assetsLibraryTree->Refresh();
}


void EventCreator::AutoNameIsChanged(int _state)
{
	eventTable->SetAutoName(_state);
}



void EventCreator::SetObjectName(QString _name)
{
	eventTable->SetCellText(EventTableWidget::NAME_PROPERTY, EventTableWidget::VALUE, _name);
}


void EventCreator::SetNameDisable(bool _disable)
{
	if(_disable)
	{
		eventTable->SetCellDelegate(EventTableWidget::NAME_PROPERTY, UniversalDelegate::NO_EDIT);
	}
	else
	{
		eventTable->SetCellDelegate(EventTableWidget::NAME_PROPERTY, UniversalDelegate::TEXT_EDITOR);
	}
}


void EventCreator::ShowDoc(void)
{
	QString path = QString("editor\\docs\\") + "EventEditor.htm";

	QUrl url = QUrl(path);

	QDesktopServices::openUrl(url);
}




void EventCreator::SetFastCreating(bool _state)
{
	fastCreatingEnable = _state;
}



AbstractEvent* EventCreator::GetEvent(void)const
{
	return event;
}


QString EventCreator::Vector3ToString(Vector3 _vector)
{
	return QString::number(_vector.x) + ";" + QString::number(_vector.y) + ";" + QString::number(_vector.z);
}



QString EventCreator::Vector2ToString(Vector2 _vector)
{
	return QString::number(_vector.x) + ";" + QString::number(_vector.y);
}




QString EventCreator::QuaternionToString(Quaternion _quaternion)
{
	return QString::number(_quaternion.x) + ";" + QString::number(_quaternion.y) + ";" + QString::number(_quaternion.z) + ";" + QString::number(_quaternion.w);
}




Vector3 EventCreator::StringToVector3(QString _vector)
{
	SimpleList<StringANSI> list = SplitString(_vector.toStdString(), ";");

	if(list.GetSize() == 3)
	{
		float x = QString(list[0]->key.c_str()).toFloat();
		float y = QString(list[1]->key.c_str()).toFloat();
		float z = QString(list[2]->key.c_str()).toFloat();
		return Vector3(x, y, z);
	}
	return Vector3();
}


Vector2 EventCreator::StringToVector2(QString _vector)
{
	SimpleList<StringANSI> list = SplitString(_vector.toStdString(), ";");

	if(list.GetSize() == 2)
	{
		float x = QString(list[0]->key.c_str()).toFloat();
		float y = QString(list[1]->key.c_str()).toFloat();
		return Vector2(x, y);
	}
	return Vector2();
}


Quaternion EventCreator::StringToQuaternion(QString _quaternion)
{
	SimpleList<StringANSI> list = SplitString(_quaternion.toStdString(), ";");

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




QString EventCreator::StringOfDeactivationMode(int32 _mode)
{
	if(_mode == AbstractEvent::NEVER_SWITCH_OFF)
	{
		return "NEVER_SWITCH_OFF";
	}
	else if(_mode == AbstractEvent::AUTO_SWITCH_OFF)
	{
		return "AUTO_SWITCH_OFF";
	}
	else if(_mode == AbstractEvent::OVERLAY_SWITCH_OFF)
	{
		return "OVERLAY_SWITCH_OFF";
	}
	return "";
}


int32 EventCreator::IndexOfDeactivationMode(QString _text)
{
	if(_text == "NEVER_SWITCH_OFF")
	{
		return AbstractEvent::NEVER_SWITCH_OFF;
	}
	else if(_text == "AUTO_SWITCH_OFF")
	{
		return AbstractEvent::AUTO_SWITCH_OFF;
	}
	else if(_text == "OVERLAY_SWITCH_OFF")
	{
		return AbstractEvent::OVERLAY_SWITCH_OFF;
	}
	return -1;
}



void EventCreator::SetEvent(QString _name)
{
	eventTable->SetAutoName(false);

	if(event = AbstractEvent::_LoadFromFile(_name.toStdString()))
	{
		eventTable->SetCellText(EventTableWidget::NAME_PROPERTY, EventTableWidget::VALUE, event->GetName().c_str());
		eventTable->SetCellText(EventTableWidget::ACTIVATION_LIMIT_PROPERTY, EventTableWidget::VALUE, QString::number(event->GetActivationLimit()));
		eventTable->SetCellText(EventTableWidget::ARGS_LOADING_MODE_PROPERTY, EventTableWidget::VALUE, event->IsLoadArgsEnabled() ? "AUTO" : "MANUAL");
		eventTable->SetCellText(EventTableWidget::ACTIVATION_STATE_PROPERTY, EventTableWidget::VALUE, event->GetValue() ? "true" : "false");
		eventTable->SetCellText(EventTableWidget::DEACTIVATION_MODE_PROPERTY, EventTableWidget::VALUE, StringOfDeactivationMode(event->GetDeactivationMode()));
		CHANGE_NAME_SetEvent(event->GetType());
		SEND_MESSAGE_SetEvent(event->GetType());
		CREATE_OBJECT_SetEvent(event->GetType());
		DESTROY_OBJECT_SetEvent(event->GetType());
		TIMER_FINISH_SetEvent(event->GetType());
		FLOAT_LERP_FINISH_SetEvent(event->GetType());
		PICK_SCENE_SetEvent(event->GetType());
		ADD_TO_LAYER_SetEvent(event->GetType());
		REMOVE_FROM_LAYER_SetEvent(event->GetType());
		ADD_TO_UPDATE_LIST_SetEvent(event->GetType());
		REMOVE_FROM_UPDATE_LIST_SetEvent(event->GetType());
		ADD_TO_RENDER_LIST_SetEvent(event->GetType());
		REMOVE_FROM_RENDER_LIST_SetEvent(event->GetType());
		CHANGE_CAMERA_SetEvent(event->GetType());
		ENABLE_POSITION_OFFSET_SetEvent(event->GetType());
		CHANGE_FRONT_TEXTURE_SetEvent(event->GetType());
		CHANGE_BACK_TEXTURE_SetEvent(event->GetType());
		CHANGE_FRONT_TEXTURED_ANIMATION_SetEvent(event->GetType());
		CHANGE_BACK_TEXTURED_ANIMATION_SetEvent(event->GetType());
		CHANGE_ANIMATION_FRAME_SetEvent(event->GetType());
		ENABLE_PROPERTY_SetEvent(event->GetType());
		CHANGE_ATLAS_SetEvent(event->GetType());
		PLAYER_CONTROL_FINISH_SetEvent(event->GetType());
		TRIGGER_FINISH_SetEvent(event->GetType());
		PICK_CAMERA_SetEvent(event->GetType());
		CHANGE_MESH_SetEvent(event->GetType());
		CHANGE_FLOAT_LERP_SetEvent(event->GetType());
		CHANGE_PLAYING_STATE_SetEvent(event->GetType());
		CHANGE_PLAYING_MODE_SetEvent(event->GetType());
		CHANGE_VALUE_SetEvent(event->GetType());
		COLLISION_BETWEEN_OBJECTS_SetEvent(event->GetType());
		CHANGE_HITBOX_SetEvent(event->GetType());
		FLOAT_LERP_START_SetEvent(event->GetType());

		delete event;
		event = NIL; 
	}

	eventTable->SetAutoName(true);

	setWindowTitle("Event Editor");
}


 

void EventCreator::IsOk(void)
{ 
	int32 errorCode;

	if(!eventTable->IsValid())
	{
		ErrorDialog("Error", " Event is not valid", 130, 40);
		return;
	}
	if(eventTable->GetCellText(EventTableWidget::NAME_PROPERTY, EventTableWidget::VALUE).isEmpty())
	{
		ErrorDialog("Error", "Event name is not defined", 175, 40);
		return;
	}
	if((errorCode = BooleanExprParser::_IsNameValid(eventTable->GetCellText(EventTableWidget::NAME_PROPERTY, EventTableWidget::VALUE).toStdString())) >= 0)
	{
		ErrorDialog("Error", "Event name is not valid: '" + QString(eventTable->GetCellText(EventTableWidget::NAME_PROPERTY, EventTableWidget::VALUE).at(errorCode)) + "'", 190, 40);
		return;
	}
	CHANGE_NAME_Create();
	SEND_MESSAGE_Create();
	CREATE_OBJECT_Create();
	DESTROY_OBJECT_Create();
	TIMER_FINISH_Create();
	FLOAT_LERP_FINISH_Create();
	PICK_SCENE_Create();
	ADD_TO_LAYER_Create();
	REMOVE_FROM_LAYER_Create();
	ADD_TO_UPDATE_LIST_Create();
	REMOVE_FROM_UPDATE_LIST_Create();
	ADD_TO_RENDER_LIST_Create();
	REMOVE_FROM_RENDER_LIST_Create();
	CHANGE_CAMERA_Create();
	ENABLE_POSITION_OFFSET_Create();
	CHANGE_FRONT_TEXTURE_Create();
	CHANGE_BACK_TEXTURE_Create();
	CHANGE_FRONT_TEXTURED_ANIMATION_Create();
	CHANGE_BACK_TEXTURED_ANIMATION_Create();
	CHANGE_ANIMATION_FRAME_Create();
	ENABLE_PROPERTY_Create();
	CHANGE_ATLAS_Create();
	PLAYER_CONTROL_FINISH_Create();
	TRIGGER_FINISH_Create();
	PICK_CAMERA_Create();
	CHANGE_MESH_Create();
	CHANGE_FLOAT_LERP_Create();
	CHANGE_PLAYING_STATE_Create();
	CHANGE_PLAYING_MODE_Create();
	CHANGE_VALUE_Create();
	COLLISION_BETWEEN_OBJECTS_Create();
	CHANGE_HITBOX_Create();
	FLOAT_LERP_START_Create();

	if(event)
	{
		event->Rename(eventTable->GetCellText(EventTableWidget::NAME_PROPERTY, EventTableWidget::VALUE).toStdString());
		event->SetLoadArgsEnable(eventTable->GetCellText(EventTableWidget::ARGS_LOADING_MODE_PROPERTY, EventTableWidget::VALUE) == "AUTO");
		event->SetDeactivationMode(IndexOfDeactivationMode(eventTable->GetCellText(EventTableWidget::DEACTIVATION_MODE_PROPERTY, EventTableWidget::VALUE)));
		if(eventTable->GetCellText(EventTableWidget::ACTIVATION_STATE_PROPERTY, EventTableWidget::VALUE) == "true")
		{
			if(eventTable->GetCellText(EventTableWidget::ACTIVATION_LIMIT_PROPERTY, EventTableWidget::VALUE).toInt() > -1)
			{
				event->SetActivationLimit(eventTable->GetCellText(EventTableWidget::ACTIVATION_LIMIT_PROPERTY, EventTableWidget::VALUE).toInt() + 1);
			}
			else
			{
				event->SetActivationLimit(eventTable->GetCellText(EventTableWidget::ACTIVATION_LIMIT_PROPERTY, EventTableWidget::VALUE).toInt());
			}
			event->SetValue(true);
		}
		else
		{
			event->SetActivationLimit(eventTable->GetCellText(EventTableWidget::ACTIVATION_LIMIT_PROPERTY, EventTableWidget::VALUE).toInt());

			event->SetValue(false);
		}

		if(!fastCreatingEnable)
		{
			int assetType;
			bool ignoreEventIsEditedSignal = false;
			if(File::_IsExist(AssetLibrary::_GetDir() + AbstractEvent::_GetDir() + event->GetName() + "." + AbstractEvent::_GetFileSuffix()))
			{
				WarningDialog warning("Warning", "Event '" + QString(event->GetName().c_str()) + "' is already exist. Rewrite file?", 200, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					delete event;
					event = NIL;
					return;
				}
				ignoreEventIsEditedSignal = true;
			}
			else if((assetType = AssetLibrary::_IsAssetExist(event->GetName())) != AssetLibrary::UNKNOWN_ASSET)
			{
				WarningDialog warning("Warning", "Name '" + QString(event->GetName().c_str()) + "' is already reserved (" + QString(event->GetName().c_str()) + "." + QString(AssetLibrary::_GetFileSuffixOfAssetType(assetType).c_str()) + "). Ignore?", 280, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					delete event;
					event = NIL;
					return;
				}
			}

			event->SaveToFile();
			
			if(!ignoreEventIsEditedSignal) { emit EventIsEdited(QString(event->GetName().c_str())); }
			
			delete event;
			event = NIL;
		}
		else
		{
			if(AssetLibrary::_GetAssets().AddObject(event) == NIL)
			{
				ErrorDialog("Error", "Name '" + QString(event->GetName().c_str()) + "' is already reserved", 195, 40);
				delete event;
				event = NIL;
				return;
			} 

			emit EventIsEdited(QString(event->GetName().c_str()));
		}

		accept();
	}
}