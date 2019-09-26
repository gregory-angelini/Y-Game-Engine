#include "ActionCreator.h"
#include <Editor/NameCreator/NameCreator.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h> 
#include <Editor/WarningDialog/WarningDialog.h> 
#include <Engine/Core/Control/PlayerControl/PlayerControl.h> 
#include <Engine/Core/Trigger/Action/Actions.h> 



ActionCreator::ActionCreator(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), action(NIL), fastCreatingEnable(false)
{
	actionStateLabel = new QLabel(this);
    actionStateLabel->setText("Action state:");
	actionStateLabel->setGeometry((630+100+50+50+15+10-5+200-3+150+50+22-150-50)/2, 5, 100, 21);
	actionTable = new ActionTableWidget(this);
	actionTable->move(20, 20+10);
	autoNameLabel = new QLabel(this);
    autoNameLabel->setGeometry(QRect(20+5+30-5+600+300+80, 5, 101, 21));
	autoNameLabel->setText("Auto name:");
	
	autoNameCheckBox = new QCheckBox(this);
    autoNameCheckBox->setGeometry(QRect(20+90+5+10-20+30-10-5+600+300+80, 5, 16, 21));
	autoNameCheckBox->setChecked(actionTable->IsAutoNameEnabled());
	connect(autoNameCheckBox, SIGNAL(stateChanged(int)), this, SLOT(AutoNameIsChanged(int)));
	line = new QFrame(this);
    line->setGeometry(QRect(20, 20+201+10-5+50+25-5-3+10, 400+400+400+60+40+20-150-70-40-10-10+5+50+20+3, 21));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
	assetsLibraryLabel = new QLabel(this);
    assetsLibraryLabel->setText("Assets library:");
	assetsLibraryLabel->setGeometry((630+100+50+50+15+10-5+200-3+150+50+22-150-50)/2, 20+270+10-5-2+17-10-7+20-20+20, 100, 21);
    assetsLibraryTree = new AssetsLibraryTree(this); 
	assetsLibraryTree->resize(630+100+50+50+15+10-5+200-3+150+50+22-150, 270+10-5-2+17-10-7+30+10);  
	assetsLibraryTree->move(20, 20+270+10-5-2+17-10-7+20+20);
	assetsLibraryTree->SetCreatingAssetsEnable(true);
	assetsLibraryTree->SetEditingAssetsEnable(true);
	connect(assetsLibraryTree, SIGNAL(RefreshIsFinished()), this, SLOT(RefreshActionTable()));
    connect(assetsLibraryTree, SIGNAL(AssetIsRemoved()), this, SLOT(RefreshActionTable()));
	refreshAssetsLibraryPushButton = new QPushButton(this);
    refreshAssetsLibraryPushButton->setText("Refresh"); 
	refreshAssetsLibraryPushButton->setFocusPolicy(Qt::NoFocus);
    refreshAssetsLibraryPushButton->setGeometry(QRect(250+500+100-10+140+70-400+120+20+60+100-30+40+70-50+30+50+50+15-150-70-40-10+50+20-500-380, 460-30+200-400-70+260-15-5-36-50-50+60+320+15-5, 81, 25));
    connect(refreshAssetsLibraryPushButton, SIGNAL(clicked()), this, SLOT(RefreshAssetsLibrary()));
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
	setWindowTitle("Action Creator");
}



ActionCreator::~ActionCreator(void)
{
}



void ActionCreator::RefreshActionTable(void)
{
	actionTable->UpdateStatus();
}


void ActionCreator::RefreshAssetsLibrary(void)
{
	assetsLibraryTree->Refresh();
}


void ActionCreator::AutoNameIsChanged(int _state)
{
	actionTable->SetAutoName(_state);
}



void ActionCreator::SetObjectName(QString _name)
{
	actionTable->SetCellText(ActionTableWidget::NAME_PROPERTY, ActionTableWidget::VALUE, _name);
}


void ActionCreator::SetNameDisable(bool _disable)
{
	if(_disable)
	{
		actionTable->SetCellDelegate(ActionTableWidget::NAME_PROPERTY, UniversalDelegate::NO_EDIT);
	}
	else
	{
		actionTable->SetCellDelegate(ActionTableWidget::NAME_PROPERTY, UniversalDelegate::TEXT_EDITOR);
	}
}




void ActionCreator::ShowDoc(void)
{
	QString path = QString("editor\\docs\\") + "ActionEditor.htm";

	QUrl url = QUrl(path);

	QDesktopServices::openUrl(url);
}





void ActionCreator::SetFastCreating(bool _state)
{
	fastCreatingEnable = _state;
}



QString ActionCreator::Vector3ToString(Vector3 _vector)
{
	return QString::number(_vector.x) + ";" + QString::number(_vector.y) + ";" + QString::number(_vector.z);
}



QString ActionCreator::Vector2ToString(Vector2 _vector)
{
	return QString::number(_vector.x) + ";" + QString::number(_vector.y);
}


QString ActionCreator::QuaternionToString(Quaternion _quaternion)
{
	return QString::number(_quaternion.x) + ";" + QString::number(_quaternion.y) + ";" + QString::number(_quaternion.z) + ";" + QString::number(_quaternion.w);
}



Vector3 ActionCreator::StringToVector3(QString _vector)
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


Vector2 ActionCreator::StringToVector2(QString _vector)
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


Quaternion ActionCreator::StringToQuaternion(QString _quaternion)
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



AbstractAction* ActionCreator::GetAction(void)const
{
	return action;
}



void ActionCreator::SetAction(QString _name)
{
	actionTable->SetAutoName(false);

	if(action = AbstractAction::_LoadFromFile(_name.toStdString()))
	{
		actionTable->SetCellText(ActionTableWidget::NAME_PROPERTY, ActionTableWidget::VALUE, action->GetName().c_str());
		actionTable->SetCellText(ActionTableWidget::ACTIVATION_LIMIT_PROPERTY, ActionTableWidget::VALUE, QString::number(action->GetActivationLimit()));
		actionTable->SetCellText(ActionTableWidget::ARGS_LOADING_MODE_PROPERTY, ActionTableWidget::VALUE, action->IsLoadArgsEnabled() ? "AUTO" : "MANUAL");
		CHANGE_NAME_SetAction(action->GetType());
		CREATE_TIMER_SetAction(action->GetType());
		CREATE_CAMERA_SetAction(action->GetType());
		CREATE_MESH_SetAction(action->GetType());
		CREATE_SET_SetAction(action->GetType());
		CREATE_PLAYER_CONTROL_SetAction(action->GetType());
		CREATE_TEXTURED_ANIMATION_SetAction(action->GetType());
		CREATE_TEXTURE_SetAction(action->GetType());
		GET_SYSTEM_TIME_SetAction(action->GetType());
		CREATE_TRIGGER_SetAction(action->GetType());
		CHANGE_VALUE_SetAction(action->GetType());
		LOAD_ASSET_SetAction(action->GetType());
		GET_NAME_SetAction(action->GetType());
		CHECK_OBJECT_EXIST_SetAction(action->GetType());
		CHECK_LIST_EMPTY_SetAction(action->GetType());
		SEND_MESSAGE_SetAction(action->GetType());
		DESTROY_OBJECT_SetAction(action->GetType());
		SAVE_TO_FILE_SetAction(action->GetType());
		SET_PRIVATE_ASSET_SetAction(action->GetType());
		CHANGE_ANIMATION_FRAME_SetAction(action->GetType());
		RESIZE_SET_SetAction(action->GetType());
		CHANGE_ACTIVATION_LIMIT_SetAction(action->GetType());
		CHANGE_DEACTIVATION_MODE_SetAction(action->GetType());
		CREATE_EMPTY_TILE_SetAction(action->GetType());
		REMOVE_BRUSH_SetAction(action->GetType());
		GET_AMOUNT_OF_FRAMES_SetAction(action->GetType());
		GET_FRAME_SIZE_SetAction(action->GetType());
		CHANGE_ATLAS_SetAction(action->GetType());
		GET_TEXTURE_SIZE_SetAction(action->GetType());
		CHANGE_RANDOM_SEED_SetAction(action->GetType());
		CHANGE_ITEM_VALUE_SetAction(action->GetType());
		CHANGE_LIST_VALUE_SetAction(action->GetType());
		REMOVE_FROM_LIST_SetAction(action->GetType());
		ENABLE_ROTATION_RANGE_SetAction(action->GetType());
		ADD_TO_LIST_SetAction(action->GetType());
		TILED_SPACE_FILL_SetAction(action->GetType());
		CHANGE_ROTATION_RANGE_SetAction(action->GetType());
		ADD_INPUT_EVENT_SetAction(action->GetType());
		SET_ONE_INPUT_EVENT_SetAction(action->GetType());
		GET_DATA_FROM_ITEM_SetAction(action->GetType());
		GET_ITEM_INDEX_SetAction(action->GetType());
		SET_DATA_TO_SLOT_SetAction(action->GetType());
		GET_DATA_FROM_SLOT_SetAction(action->GetType());
		RANDOMIZE_LIST_SetAction(action->GetType());
		GET_AMOUNT_OF_ITEMS_SetAction(action->GetType());
		GET_AMOUNT_OF_SLOTS_SetAction(action->GetType());
		COPY_LIST_SetAction(action->GetType());
		CLEAR_LIST_SetAction(action->GetType());
		CLEAR_INPUT_EVENTS_SetAction(action->GetType());
		REPLAY_SetAction(action->GetType());
		CREATE_LIST_SetAction(action->GetType());
		CREATE_VARIABLE_SetAction(action->GetType());
		CHANGE_BACKGROUND_COLOR_SetAction(action->GetType());
		GET_COLOR_SetAction(action->GetType());
		CHANGE_COLOR_SetAction(action->GetType());
		GET_SCALE_SetAction(action->GetType());
		DISPLACE_SCALE_SetAction(action->GetType());
		REPLACE_SCALE_SetAction(action->GetType());
		GET_POSITION_SetAction(action->GetType());
		REPLACE_POSITION_SetAction(action->GetType());
		DISPLACE_POSITION_SetAction(action->GetType());
		GET_ROTATION_SetAction(action->GetType());
		REPLACE_ROTATION_SetAction(action->GetType());
		DISPLACE_ROTATION_SetAction(action->GetType());
		CHANGE_PLANE_NORMAL_SetAction(action->GetType());
		CHANGE_MESH_SetAction(action->GetType());
		CREATE_QUAD_SetAction(action->GetType());
		CREATE_BOX_SetAction(action->GetType());
		CHANGE_INTERSECTION_MODE_SetAction(action->GetType());
		CHECK_PROPERTY_SetAction(action->GetType());
		ENABLE_PROPERTY_SetAction(action->GetType());
		ENABLE_BACK_FACE_CULLING_SetAction(action->GetType());
		ENABLE_ZOOM_SetAction(action->GetType());
		ENABLE_ROTATION_SetAction(action->GetType());
		ENABLE_STATIC_OBJECT_SetAction(action->GetType());
		ENABLE_POSITION_OFFSET_SetAction(action->GetType());
		CHANGE_FRONT_TEXTURE_SetAction(action->GetType());
		CHANGE_BACK_TEXTURE_SetAction(action->GetType());
		CHANGE_FRONT_TEXTURED_ANIMATION_SetAction(action->GetType());
		CHANGE_BACK_TEXTURED_ANIMATION_SetAction(action->GetType());
		ENABLE_SHOW_CURSOR_SetAction(action->GetType());
		ENABLE_Z_BUFFER_SetAction(action->GetType());
		ENABLE_TRIGGERS_BLOCK_SetAction(action->GetType());
		CREATE_KEYBOARD_EVENT_SetAction(action->GetType());
		PICK_SCENE_SetAction(action->GetType());
		ADD_TO_LAYER_SetAction(action->GetType());
		REMOVE_FROM_LAYER_SetAction(action->GetType());
		ADD_TO_UPDATE_LIST_SetAction(action->GetType());
		REMOVE_FROM_UPDATE_LIST_SetAction(action->GetType());
		ADD_TO_RENDER_LIST_SetAction(action->GetType());
		REMOVE_FROM_RENDER_LIST_SetAction(action->GetType());
		PICK_CAMERA_SetAction(action->GetType());
		INVERSE_LIST_SetAction(action->GetType());
		CHANGE_CAMERA_SetAction(action->GetType());
		CHANGE_RENDER_TARGET_SetAction(action->GetType());
		CHANGE_FLOAT_LERP_MODE_SetAction(action->GetType());
		CHANGE_PROJECTION_MODE_SetAction(action->GetType());
		CHANGE_MOVEMENT_MODE_SetAction(action->GetType());
		CHANGE_ROTATION_MODE_SetAction(action->GetType());
		GET_SELECT_RAY_SetAction(action->GetType());
		CREATE_FLOAT_LERP_SetAction(action->GetType());
		CREATE_QUATERNION_LERP_SetAction(action->GetType());
		CREATE_VECTOR_LERP_SetAction(action->GetType());
		CHANGE_FLOAT_LERP_SPEED_SetAction(action->GetType());
		STREAMING_FLOAT_T_SetAction(action->GetType());
		STREAMING_FLOAT_CURRENT_SetAction(action->GetType());
		CHANGE_FLOAT_LERP_TARGET_SetAction(action->GetType());
		STREAMING_FLOAT_DELTA_SetAction(action->GetType());
		CHANGE_VECTOR_LERP_MODE_SetAction(action->GetType());
		CHANGE_FLOAT_LERP_SetAction(action->GetType());
		CHANGE_PLAYING_MODE_SetAction(action->GetType());
		CHANGE_PLAYING_STATE_SetAction(action->GetType());
		CHANGE_PLAYING_DURATION_SetAction(action->GetType());
		CHANGE_INPUT_TIMING_SetAction(action->GetType());
		STREAMING_QUATERNION_CURRENT_SetAction(action->GetType());
		STREAMING_VECTOR_CURRENT_SetAction(action->GetType());
		STREAMING_QUATERNION_DELTA_SetAction(action->GetType());
		STREAMING_VECTOR_DELTA_SetAction(action->GetType());
		CHECK_LINE_IN_IMAGE_SetAction(action->GetType());
		CHECK_LINE_IN_POLYGON_SetAction(action->GetType());
		CHECK_LINE_IN_SURFACE_SetAction(action->GetType());
		SET_TWO_VECTORS_SetAction(action->GetType());
		SET_TWO_QUATERNIONS_SetAction(action->GetType());
		CHANGE_VALUE_RANGE_SetAction(action->GetType());
		CHANGE_CONTROL_VECTORS_SetAction(action->GetType());
		CHANGE_CONTROL_QUATERNIONS_SetAction(action->GetType());
		ENABLE_COLLISION_POLYGONS_INFO_SetAction(action->GetType());
		ENABLE_COLLISION_CHECKING_SetAction(action->GetType());
		CHANGE_COLLISION_GROUP_SetAction(action->GetType());
		CHECK_COLLISION_SetAction(action->GetType());
		SWAP_ITEMS_SetAction(action->GetType());
		GET_RANDOM_VALUE_SetAction(action->GetType());
		CREATE_ROTATION_FROM_TO_SetAction(action->GetType());
		CHANGE_COLOR_COMPONENT_SetAction(action->GetType());
		GET_COLOR_COMPONENT_SetAction(action->GetType());
		CREATE_ROTATION_SetAction(action->GetType());
		CREATE_BOX_TEXTURE_MAP_SetAction(action->GetType());
		CHANGE_BOX_TEXTURE_MAP_SetAction(action->GetType());
		GET_LINE_ANGLE_Z_SetAction(action->GetType());
		CHANGE_VIEWING_POINT_SetAction(action->GetType());
		CHANGE_DISTANCE_TO_VIEWING_POINT_SetAction(action->GetType());
		CHANGE_ZOOM_NEAR_SetAction(action->GetType());
		CHANGE_ZOOM_FAR_SetAction(action->GetType());
		CHANGE_ZOOM_SPEED_SetAction(action->GetType());
		CHANGE_HITBOX_SetAction(action->GetType());
		CAMERA_LOOK_AT_SetAction(action->GetType());
		CHANGE_VECTOR_COMPONENT_SetAction(action->GetType());
		GET_VECTOR_COMPONENT_SetAction(action->GetType());
		VECTOR_TO_SPHERICAL_SetAction(action->GetType());
		GET_VECTOR_ANGLE_SetAction(action->GetType());
		GET_NORMALIZED_VECTOR_SetAction(action->GetType());
		GET_VECTOR_LENGTH_SetAction(action->GetType());
		CHECK_FRUSTUM_IN_QUAD_SetAction(action->GetType());
		GET_DATA_FROM_CONTROL_VECTOR_SetAction(action->GetType());
		SET_DATA_TO_CONTROL_VECTOR_SetAction(action->GetType());
		GET_LINE_LENGTH_SetAction(action->GetType());
		GET_CIRCLE_LENGTH_SetAction(action->GetType());
		CHANGE_FLOAT_LERP_T_SetAction(action->GetType());
		GET_FLOAT_LERP_T_SetAction(action->GetType());
		GET_FLOAT_LERP_CURRENT_SetAction(action->GetType());
		GET_FLOAT_LERP_DELTA_SetAction(action->GetType());
		ENABLE_TEXTURE_FLIP_SetAction(action->GetType());
		GET_DATA_FROM_CONTROL_QUATERNION_SetAction(action->GetType());
		SET_DATA_TO_CONTROL_QUATERNION_SetAction(action->GetType());
		ENABLE_FREEZE_SetAction(action->GetType());
		ROTATE_LIST_SetAction(action->GetType());
		CHANGE_DELTA_SCALE_SetAction(action->GetType());
		ENABLE_SHOW_SetAction(action->GetType());
		GET_LINE_DIRECTION_SetAction(action->GetType());
		MOVE_AROUND_PIVOT_SetAction(action->GetType());
		ROTATE_AROUND_PIVOT_SetAction(action->GetType());
		CREATE_BRUSH_SetAction(action->GetType());
		CHECK_TILE_EXIST_SetAction(action->GetType());
		CHECK_TILE_SOLID_SetAction(action->GetType());
		CHECK_TILE_PASS_SetAction(action->GetType());
		CHANGE_TILE_PASS_SetAction(action->GetType());
		CREATE_SOLID_TILE_SetAction(action->GetType());
		GET_BRUSH_OF_TILE_SetAction(action->GetType());
		GET_TILE_CENTER_SetAction(action->GetType());
		MOVE_TILE_FROM_TO_SetAction(action->GetType());
		SWAP_TWO_TILES_SetAction(action->GetType());
		COPY_TILE_SetAction(action->GetType());
		CREATE_TILED_SPACE_SetAction(action->GetType());
		CHANGE_TILED_SPACE_SIZE_SetAction(action->GetType());
		SET_PUBLIC_TILE_SetAction(action->GetType());
		SET_PUBLIC_TEXTURE_SetAction(action->GetType());
		GET_VIEWING_POINT_SetAction(action->GetType());
		GET_CAMERA_LOOK_SetAction(action->GetType());
		GET_VECTOR_LERP_PATH_SetAction(action->GetType());
		CHANGE_STRIP_POINTS_SetAction(action->GetType());
		GET_VECTOR_LERP_T_SetAction(action->GetType());
		GET_QUATERNION_LERP_T_SetAction(action->GetType());
		GET_QUATERNION_LERP_PATH_SetAction(action->GetType());
		GET_LIST_LENGTH_SetAction(action->GetType());
		GET_RADIUS_FROM_2_POINTS_SetAction(action->GetType());
		GET_RADIUS_FROM_3_POINTS_SetAction(action->GetType());
		GET_CONTROL_VECTORS_SetAction(action->GetType());
		GET_CONTROL_QUATERNIONS_SetAction(action->GetType());
		CHECK_EQUALITY_SetAction(action->GetType());

		delete action;
		action = NIL; 
	}

	actionTable->SetAutoName(true);

	setWindowTitle("Action Editor");
}


 

void ActionCreator::IsOk(void)
{ 
	int32 errorCode;

	if(!actionTable->IsValid())
	{
		ErrorDialog("Error", " Action is not valid", 130, 40);
		return;
	}
	if(actionTable->GetCellText(ActionTableWidget::NAME_PROPERTY, ActionTableWidget::VALUE).isEmpty())
	{
		ErrorDialog("Error", "Action name is not defined", 175, 40);
		return;
	}
	if((errorCode = BooleanExprParser::_IsNameValid(actionTable->GetCellText(ActionTableWidget::NAME_PROPERTY, ActionTableWidget::VALUE).toStdString())) >= 0)
	{
		ErrorDialog("Error", "Action name is not valid: '" + QString(actionTable->GetCellText(ActionTableWidget::NAME_PROPERTY, ActionTableWidget::VALUE).at(errorCode)) + "'", 190, 40);
		return;
	}
	CHANGE_NAME_Create();
	CREATE_TIMER_Create();
	CREATE_CAMERA_Create();
	CREATE_MESH_Create();
	CREATE_SET_Create();
	CREATE_PLAYER_CONTROL_Create();
	CREATE_TEXTURED_ANIMATION_Create();
	CREATE_TEXTURE_Create();
	GET_SYSTEM_TIME_Create();
	CREATE_TRIGGER_Create();
	CHANGE_VALUE_Create();
	LOAD_ASSET_Create();
	GET_NAME_Create();
	CHECK_OBJECT_EXIST_Create();
	CHECK_LIST_EMPTY_Create();
	SEND_MESSAGE_Create();
	DESTROY_OBJECT_Create();
	SAVE_TO_FILE_Create();
	SET_PRIVATE_ASSET_Create();
	CHANGE_ANIMATION_FRAME_Create();
	RESIZE_SET_Create();
	CHANGE_ACTIVATION_LIMIT_Create();
	CHANGE_DEACTIVATION_MODE_Create();
	CREATE_EMPTY_TILE_Create();
	REMOVE_BRUSH_Create();
	GET_AMOUNT_OF_FRAMES_Create();
	GET_FRAME_SIZE_Create();
	CHANGE_ATLAS_Create();
	GET_TEXTURE_SIZE_Create();
	CHANGE_RANDOM_SEED_Create();
	CHANGE_ITEM_VALUE_Create();
	CHANGE_LIST_VALUE_Create();
	REMOVE_FROM_LIST_Create();
	ENABLE_ROTATION_RANGE_Create();
	ADD_TO_LIST_Create();
	TILED_SPACE_FILL_Create();
	CHANGE_ROTATION_RANGE_Create();
	ADD_INPUT_EVENT_Create();
	SET_ONE_INPUT_EVENT_Create();
	GET_DATA_FROM_ITEM_Create();
	GET_ITEM_INDEX_Create();
	SET_DATA_TO_SLOT_Create();
	GET_DATA_FROM_SLOT_Create();
	RANDOMIZE_LIST_Create();
	GET_AMOUNT_OF_ITEMS_Create();
	GET_AMOUNT_OF_SLOTS_Create();
	COPY_LIST_Create();
	CLEAR_LIST_Create();
	CLEAR_INPUT_EVENTS_Create();
	REPLAY_Create();
	CREATE_LIST_Create();
	CREATE_VARIABLE_Create();
	CHANGE_BACKGROUND_COLOR_Create();
	GET_COLOR_Create();
	CHANGE_COLOR_Create();
	GET_SCALE_Create();
	DISPLACE_SCALE_Create();
	REPLACE_SCALE_Create();
	GET_POSITION_Create();
	REPLACE_POSITION_Create();
	DISPLACE_POSITION_Create();
	GET_ROTATION_Create();
	REPLACE_ROTATION_Create();
	DISPLACE_ROTATION_Create();
	CHANGE_PLANE_NORMAL_Create();
	CHANGE_MESH_Create();
	CREATE_QUAD_Create();
	CREATE_BOX_Create();
	CHANGE_INTERSECTION_MODE_Create();
	CHECK_PROPERTY_Create();
	ENABLE_PROPERTY_Create();
	ENABLE_BACK_FACE_CULLING_Create();
	ENABLE_ZOOM_Create();
	ENABLE_ROTATION_Create();
	ENABLE_STATIC_OBJECT_Create();
	ENABLE_POSITION_OFFSET_Create();
	CHANGE_FRONT_TEXTURE_Create();
	CHANGE_BACK_TEXTURE_Create();
	CHANGE_FRONT_TEXTURED_ANIMATION_Create();
	CHANGE_BACK_TEXTURED_ANIMATION_Create();
	ENABLE_SHOW_CURSOR_Create();
	ENABLE_Z_BUFFER_Create();
	ENABLE_TRIGGERS_BLOCK_Create();
	CREATE_KEYBOARD_EVENT_Create();
	PICK_SCENE_Create();
	ADD_TO_LAYER_Create(); 
	REMOVE_FROM_LAYER_Create(); 
	ADD_TO_UPDATE_LIST_Create(); 
	REMOVE_FROM_UPDATE_LIST_Create(); 
	ADD_TO_RENDER_LIST_Create(); 
	REMOVE_FROM_RENDER_LIST_Create(); 
	PICK_CAMERA_Create(); 
	INVERSE_LIST_Create(); 
	CHANGE_CAMERA_Create(); 
	CHANGE_RENDER_TARGET_Create(); 
	CHANGE_FLOAT_LERP_MODE_Create(); 
	CHANGE_PROJECTION_MODE_Create(); 
	CHANGE_MOVEMENT_MODE_Create(); 
	CHANGE_ROTATION_MODE_Create(); 
	GET_SELECT_RAY_Create(); 
	CREATE_FLOAT_LERP_Create(); 
	CREATE_QUATERNION_LERP_Create(); 
	CREATE_VECTOR_LERP_Create(); 
	CHANGE_FLOAT_LERP_SPEED_Create(); 
	STREAMING_FLOAT_T_Create(); 
	STREAMING_FLOAT_CURRENT_Create(); 
	CHANGE_FLOAT_LERP_TARGET_Create(); 
	STREAMING_FLOAT_DELTA_Create();
	CHANGE_VECTOR_LERP_MODE_Create();
	CHANGE_FLOAT_LERP_Create();
	CHANGE_PLAYING_MODE_Create();
	CHANGE_PLAYING_STATE_Create();
	CHANGE_PLAYING_DURATION_Create();
	CHANGE_INPUT_TIMING_Create();
	STREAMING_QUATERNION_CURRENT_Create();
	STREAMING_VECTOR_CURRENT_Create();
	STREAMING_QUATERNION_DELTA_Create();
	STREAMING_VECTOR_DELTA_Create();
	CHECK_LINE_IN_IMAGE_Create();
	CHECK_LINE_IN_POLYGON_Create();
	CHECK_LINE_IN_SURFACE_Create();
	SET_TWO_VECTORS_Create();
	SET_TWO_QUATERNIONS_Create();
	CHANGE_VALUE_RANGE_Create();
	CHANGE_CONTROL_VECTORS_Create();
	CHANGE_CONTROL_QUATERNIONS_Create();
	ENABLE_COLLISION_POLYGONS_INFO_Create();
	ENABLE_COLLISION_CHECKING_Create();
	CHANGE_COLLISION_GROUP_Create();
	CHECK_COLLISION_Create();
	SWAP_ITEMS_Create();
	GET_RANDOM_VALUE_Create();
	CREATE_ROTATION_FROM_TO_Create();
	CHANGE_COLOR_COMPONENT_Create();
	GET_COLOR_COMPONENT_Create();
	CREATE_ROTATION_Create();
	CREATE_BOX_TEXTURE_MAP_Create();
	CHANGE_BOX_TEXTURE_MAP_Create();
	GET_LINE_ANGLE_Z_Create();
	CHANGE_VIEWING_POINT_Create();
	CHANGE_DISTANCE_TO_VIEWING_POINT_Create();
	CHANGE_ZOOM_NEAR_Create();
	CHANGE_ZOOM_FAR_Create();
	CHANGE_ZOOM_SPEED_Create();
	CHANGE_HITBOX_Create(); 
	CAMERA_LOOK_AT_Create(); 
	CHANGE_VECTOR_COMPONENT_Create(); 
	GET_VECTOR_COMPONENT_Create(); 
	VECTOR_TO_SPHERICAL_Create(); 
	GET_VECTOR_ANGLE_Create(); 
	GET_NORMALIZED_VECTOR_Create();
	GET_VECTOR_LENGTH_Create();
	CHECK_FRUSTUM_IN_QUAD_Create(); 
	GET_DATA_FROM_CONTROL_VECTOR_Create(); 
	SET_DATA_TO_CONTROL_VECTOR_Create(); 
	GET_LINE_LENGTH_Create(); 
	GET_CIRCLE_LENGTH_Create(); 
	CHANGE_FLOAT_LERP_T_Create(); 
	GET_FLOAT_LERP_T_Create(); 
	GET_FLOAT_LERP_CURRENT_Create(); 
	GET_FLOAT_LERP_DELTA_Create(); 
	ENABLE_TEXTURE_FLIP_Create(); 
	GET_DATA_FROM_CONTROL_QUATERNION_Create(); 
	SET_DATA_TO_CONTROL_QUATERNION_Create(); 
	ENABLE_FREEZE_Create(); 
	ROTATE_LIST_Create(); 
	CHANGE_DELTA_SCALE_Create(); 
	ENABLE_SHOW_Create(); 
	GET_LINE_DIRECTION_Create(); 
	MOVE_AROUND_PIVOT_Create();
	ROTATE_AROUND_PIVOT_Create();
	CREATE_BRUSH_Create();
	CHECK_TILE_EXIST_Create();
	CHECK_TILE_SOLID_Create();
	CHECK_TILE_PASS_Create();
	CHANGE_TILE_PASS_Create();
	CREATE_SOLID_TILE_Create();
	GET_BRUSH_OF_TILE_Create();
	GET_TILE_CENTER_Create();
	MOVE_TILE_FROM_TO_Create();
	SWAP_TWO_TILES_Create();
	COPY_TILE_Create();
	CREATE_TILED_SPACE_Create();
	CHANGE_TILED_SPACE_SIZE_Create();
	SET_PUBLIC_TILE_Create();
	SET_PUBLIC_TEXTURE_Create();
	GET_VIEWING_POINT_Create();
	GET_CAMERA_LOOK_Create();
	GET_VECTOR_LERP_PATH_Create();
	CHANGE_STRIP_POINTS_Create();
	GET_VECTOR_LERP_T_Create();
	GET_QUATERNION_LERP_T_Create();
	GET_QUATERNION_LERP_PATH_Create();
	GET_LIST_LENGTH_Create();
	GET_RADIUS_FROM_2_POINTS_Create();
	GET_RADIUS_FROM_3_POINTS_Create();
	GET_CONTROL_VECTORS_Create();
	GET_CONTROL_QUATERNIONS_Create();
	CHECK_EQUALITY_Create();


	if(action)
	{
		action->Rename(actionTable->GetCellText(ActionTableWidget::NAME_PROPERTY, ActionTableWidget::VALUE).toStdString());
		action->SetActivationLimit(actionTable->GetCellText(ActionTableWidget::ACTIVATION_LIMIT_PROPERTY, ActionTableWidget::VALUE).toInt());
		action->SetLoadArgsEnable(actionTable->GetCellText(ActionTableWidget::ARGS_LOADING_MODE_PROPERTY, ActionTableWidget::VALUE) == "AUTO");
	

		if(!fastCreatingEnable)
		{
			int assetType;
			bool ignoreActionIsEditedSignal = false;
			if(File::_IsExist(AssetLibrary::_GetDir() + AbstractAction::_GetDir() + action->GetName() + "." + AbstractAction::_GetFileSuffix()))
			{
				WarningDialog warning("Warning", "Action '" + QString(action->GetName().c_str()) + "' is already exist. Rewrite file?", 200, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					delete action;
					action = NIL;
					return;
				}
				ignoreActionIsEditedSignal = true;
			}
			else if((assetType = AssetLibrary::_IsAssetExist(action->GetName())) != AssetLibrary::UNKNOWN_ASSET)
			{
				WarningDialog warning("Warning", "Name '" + QString(action->GetName().c_str()) + "' is already reserved (" + QString(action->GetName().c_str()) + "." + QString(AssetLibrary::_GetFileSuffixOfAssetType(assetType).c_str()) + "). Ignore?", 280, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					delete action;
					action = NIL;
					return;
				}
			}

			action->SaveToFile();
			
			if(!ignoreActionIsEditedSignal) { emit ActionIsEdited(QString(action->GetName().c_str())); }
			
			delete action;
			action = NIL;
		}
		else
		{
			if(AssetLibrary::_GetAssets().AddObject(action) == NIL)
			{
				ErrorDialog("Error", "Name '" + QString(action->GetName().c_str()) + "' is already reserved", 195, 40);
				delete action;
				action = NIL;
				return;
			} 

			emit ActionIsEdited(QString(action->GetName().c_str()));
		}

		accept();
	}
}