#include "AbstractAction.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Core/Object/TranslatableObject/TranslatableObject.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/Parser/ArithmeticExprParser/ArithmeticExprParser.h>
#include <Engine/Core/Trigger/StringExprParser/StringExprParser.h>
#include <Engine/Core/Object/RotatableObject/RotatableObject.h>
#include <Engine/Math/Quaternion/Quaternion.h>
#include <Engine/Output/Graphics/Surface/TexturingSurface/TexturingSurface.h>
#include <Engine/Output/Graphics/Scene/GraphicsScene/GraphicsScene.h>
#include <Engine/Core/Time/Timer/Timer.h>
#include <Engine/Core/Object/ScalableObject/ScalableObject.h>
#include <Engine/Core/Scene/LogicScene/LogicScene.h>
#include <Engine/Core/Trigger/Action/Actions.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>
StringANSI AbstractAction::actionsSuffix = "action";
StringANSI AbstractAction::actionsDir = "triggers/actions/";


 
AbstractAction::AbstractAction(void): type(0), activationLimit(-1), loadArgsEnable(true), valid(false)
{
	for(int32 i = 0; i < WIDTH; i++)
	{
		for(int32 j = 0; j < HEIGHT; j++)
		{
			state[i][j] = FALSE;
		}
	}

	AddClassProperty(ACTION_CLASS);
}


AbstractAction::AbstractAction(int32 _type): type(_type), activationLimit(-1), loadArgsEnable(true), valid(false)
{
	for(int32 i = 0; i < WIDTH; i++)
	{
		for(int32 j = 0; j < HEIGHT; j++)
		{
			state[i][j] = FALSE;
		}
	}

	AddClassProperty(ACTION_CLASS);
}


AbstractAction::~AbstractAction(void)
{
	Destroying();
}


void AbstractAction::SetArgMode(int32 _index, int32 _mode)
{
	if(_index >= 0 && _index < HEIGHT)
	{
		if(_mode >= 0 && _mode < WIDTH)
		{
			state[TEMPLATE][_index] = FALSE;
			state[EXPRESSION][_index] = FALSE;
			state[VARIABLE][_index] = FALSE;
			state[KEY][_index] = FALSE;

			state[_mode][_index] = TRUE;
		}
	}
}




void AbstractAction::SetArgAssetType(int32 _index, int32 _type)
{
	if(_index >= 0 && _index < HEIGHT)
	{
		state[ASSET_TYPE][_index] = _type;
	}
}


int32 AbstractAction::GetArgState(int32 _property, int32 _index)
{
	if(_property >= 0 && _property < WIDTH)
	{
		if(_index >= 0 && _index < HEIGHT)
		{
			return state[_property][_index];
		}
	}
	return FALSE;
}



void AbstractAction::SetVariableArg(int32 _index, StringANSI _name)
{}



void AbstractAction::SetTemplateArg(int32 _index, StringANSI _expr)
{}


void AbstractAction::SetExpressionArg(int32 _index, StringANSI _expr)
{}


void AbstractAction::SetStringKeyArg(int32 _index, StringANSI _value)
{}


void AbstractAction::SetIntegerKeyArg(int32 _index, int32 _value)
{}


void AbstractAction::SetFloatKeyArg(int32 _index, float _value)
{}


void AbstractAction::SetBooleanKeyArg(int32 _index, bool _value)
{}


void AbstractAction::SetVector2KeyArg(int32 _index, Vector2 _value)
{}


void AbstractAction::SetVector3KeyArg(int32 _index, Vector3 _value)
{}



void AbstractAction::SetQuaternionKeyArg(int32 _index, Quaternion _value)
{}



StringANSI AbstractAction::GetTemplateArg(int32 _index)
{
	return "";
}


StringANSI AbstractAction::GetExpressionArg(int32 _index)
{
	return "";
}



StringANSI AbstractAction::GetVariableArg(int32 _index)
{
	return "";
}



StringANSI AbstractAction::GetStringKeyArg(int32 _index)
{
	return "";
}


int32 AbstractAction::GetIntegerKeyArg(int32 _index)
{
	return -1;
}


float AbstractAction::GetFloatKeyArg(int32 _index)
{
	return -1.0f;
}


bool AbstractAction::GetBooleanKeyArg(int32 _index)
{
	return false;
}



Vector2 AbstractAction::GetVector2KeyArg(int32 _index)
{
	return Vector2();
}


Vector3 AbstractAction::GetVector3KeyArg(int32 _index)
{
	return Vector3();
}


Quaternion AbstractAction::GetQuaternionKeyArg(int32 _index)
{
	return Quaternion();
}



int32 AbstractAction::GetActivationLimit(void)const
{
	return activationLimit;
}


void AbstractAction::SetActivationLimit(int32 _activationLimit)
{
	activationLimit = _activationLimit;
}


bool AbstractAction::IsLoadArgsEnabled(void)const
{
	return loadArgsEnable;
}


void AbstractAction::SetLoadArgsEnable(bool _enable)
{
	loadArgsEnable = _enable;
}



int32 AbstractAction::GetType(void)const
{
	return type;
} 


bool AbstractAction::IsValid(void)
{
	if(valid)
	{
		if(activationLimit > 0)
		{
			activationLimit--;

			if(Trigger::_IsEventLogEnabled())
			{
				Trigger::_GetEventLogList().AddToTail(Trigger::EventInfo(GetName(), Trigger::EventInfo::ACTION_CATEGORY, type, Renderer::_GetPicked()->GetWorkingTimeInMs()));
			}
			return true;
		}
		else if(activationLimit < 0)
		{
			if(Trigger::_IsEventLogEnabled())
			{
				Trigger::_GetEventLogList().AddToTail(Trigger::EventInfo(GetName(), Trigger::EventInfo::ACTION_CATEGORY, type, Renderer::_GetPicked()->GetWorkingTimeInMs()));
			}
			return true;
		}
	}
	return false;
}

				
StringANSI AbstractAction::_GetStringType(int32 _type)
{ 
	switch(_type)
	{ 
		case MessageHandler<int32>::SEND_MESSAGE:
		{
			return "SEND_MESSAGE";
		}

		case AbstractObject::CHANGE_NAME_MESSAGE:
		{
			return "CHANGE_NAME";
		} 

		case AbstractObject::DESTROY_OBJECT_MESSAGE:
		{
			return "DESTROY_OBJECT";
		}

		case AssetLibrary::CHECK_OBJECT_EXIST_MESSAGE:
		{
			return "CHECK_OBJECT_EXIST";
		}

		case AbstractObject::CHECK_PROPERTY_MESSAGE:
		{
			return "CHECK_PROPERTY";
		}

		case AbstractObject::ENABLE_PROPERTY_MESSAGE:
		{
			return "ENABLE_PROPERTY";
		}

		case AbstractObject::GET_NAME_MESSAGE:
		{
			return "GET_NAME";
		}

		case AssetLibrary::LOAD_ASSET_MESSAGE:
		{
			return "LOAD_ASSET";
		}

		case AbstractObject::SAVE_TO_FILE_MESSAGE:
		{
			return "SAVE_TO_FILE";
		}

		case TiledSpace3D::TILED_SPACE_FILL_MESSAGE:
		{
			return "TILED_SPACE_FILL";
		}

		case TiledSpace3D::CHECK_TILE_EXIST_MESSAGE:
		{
			return "CHECK_TILE_EXIST";
		}

		case TiledSpace3D::CHANGE_TILE_PASS_MESSAGE:
		{
			return "CHANGE_TILE_PASS";
		}

		case TiledSpace3D::CHECK_TILE_PASS_MESSAGE:
		{
			return "CHECK_TILE_PASS";
		}

		case TiledSpace3D::CHECK_TILE_SOLID_MESSAGE:
		{
			return "CHECK_TILE_SOLID";
		}

		case TiledSpace3D::SET_PUBLIC_TILE_MESSAGE:
		{
			return "SET_PUBLIC_TILE";
		}

		case TexturingSurface::SET_PUBLIC_TEXTURE_MESSAGE:
		{
			return "SET_PUBLIC_TEXTURE";
		}

		case AssetLibrary::SET_PRIVATE_ASSET_MESSAGE:
		{
			return "SET_PRIVATE_ASSET";
		}

		case Texture2D::CREATE_BOX_TEXTURE_MAP_MESSAGE:
		{		
			return "CREATE_BOX_TEXTURE_MAP";
		}

		case Texture2D::CHANGE_BOX_TEXTURE_MAP_MESSAGE:
		{		
			return "CHANGE_BOX_TEXTURE_MAP";
		}

		case Timer::CREATE_TIMER_MESSAGE:
		{
			return "CREATE_TIMER";
		}

		case Texture2D::CREATE_TEXTURE_MESSAGE:
		{
			return "CREATE_TEXTURE";
		}

		case TexturedAnimation::CREATE_TEXTURED_ANIMATION_MESSAGE:
		{
			return "CREATE_TEXTURED_ANIMATION";
		}

		case Mesh::CREATE_MESH_MESSAGE:
		{
			return "CREATE_MESH";
		}

		case Set::GET_DATA_FROM_SLOT_MESSAGE:
		{
			return "GET_DATA_FROM_SLOT";
		}

		case Set::SET_DATA_TO_SLOT_MESSAGE:
		{
			return "SET_DATA_TO_SLOT";
		}

		case Set::RESIZE_SET_MESSAGE:
		{
			return "RESIZE_SET";
		}

		case Set::GET_AMOUNT_OF_SLOTS_MESSAGE:
		{
			return "GET_AMOUNT_OF_SLOTS";
		}

		case Set::CREATE_SET_MESSAGE:
		{
			return "CREATE_SET";
		}

		case PlayerControl::CHANGE_INPUT_TIMING_MESSAGE:
		{
			return "CHANGE_INPUT_TIMING";
		}

		case PlayingObject::REPLAY_MESSAGE:
		{
			return "REPLAY";
		}

		case PlayerControl::SET_ONE_INPUT_EVENT_MESSAGE:
		{
			return "SET_ONE_INPUT_EVENT";
		}

		case PlayerControl::CREATE_PLAYER_CONTROL_MESSAGE:
		{
			return "CREATE_PLAYER_CONTROL";
		}

		case TiledSpace3D::MOVE_TILE_FROM_TO_MESSAGE:
		{
			return "MOVE_TILE_FROM_TO";
		}

		case TiledSpace3D::GET_BRUSH_OF_TILE_MESSAGE:
		{
			return "GET_BRUSH_OF_TILE";
		}

		case TiledSpace3D::SWAP_TWO_TILES_MESSAGE:
		{
			return "SWAP_TWO_TILES";
		}

		case TiledSpace3D::COPY_TILE_MESSAGE:
		{
			return "COPY_TILE";
		}

		case Camera3D::CREATE_CAMERA_MESSAGE:
		{
			return "CREATE_CAMERA";
		}

		case SystemTime::GET_SYSTEM_TIME_MESSAGE:
		{
			return "GET_SYSTEM_TIME";
		}

		case PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE:
		{
			return "CHANGE_PLAYING_DURATION";
		}

		case PlayingObject::CHANGE_PLAYING_STATE_MESSAGE:
		{
			return "CHANGE_PLAYING_STATE";
		}

		case PlayingObject::CHANGE_PLAYING_MODE_MESSAGE:
		{
			return "CHANGE_PLAYING_MODE";
		}

		case Keyboard::CREATE_KEYBOARD_EVENT_MESSAGE:
		{
			return "CREATE_KEYBOARD_EVENT";
		}

		case Mouse::ENABLE_SHOW_CURSOR_MESSAGE:
		{
			return "ENABLE_SHOW_CURSOR";
		}

		case Renderer::ENABLE_Z_BUFFER_MESSAGE:
		{
			return "ENABLE_Z_BUFFER";
		}

		case Trigger::ENABLE_TRIGGERS_BLOCK_MESSAGE:
		{
			return "ENABLE_TRIGGERS_BLOCK";
		}

		case Plane::CHANGE_PLANE_NORMAL_MESSAGE:
		{
			return "CHANGE_PLANE_NORMAL";
		}

		case Variable<Vector3>::GET_VECTOR_3_ANGLE_MESSAGE:
		case Variable<Vector2>::GET_VECTOR_2_ANGLE_MESSAGE:
		{
			return "GET_VECTOR_ANGLE";
		}

		case Line::GET_LINE_ANGLE_Z_MESSAGE:
		{
			return "GET_LINE_ANGLE_Z";
		}

		case Line::GET_LINE_LENGTH_MESSAGE:
		{
			return "GET_LINE_LENGTH";
		}

		case Line::GET_LINE_DIRECTION_MESSAGE:
		{
			return "GET_LINE_DIRECTION";
		}

		case Circle::GET_CIRCLE_LENGTH_MESSAGE:
		{
			return "GET_CIRCLE_LENGTH";
		}

		case Circle::GET_RADIUS_FROM_2_POINTS_MESSAGE:
		{
			return "GET_RADIUS_FROM_2_POINTS";
		}

		case Circle::GET_RADIUS_FROM_3_POINTS_MESSAGE:
		{
			return "GET_RADIUS_FROM_3_POINTS";
		}

		case FloatLerp::CREATE_FLOAT_LERP_MESSAGE:
		{
			return "CREATE_FLOAT_LERP";
		}
		
		case TiledSpace3D::CREATE_TILED_SPACE_MESSAGE:
		{
			return "CREATE_TILED_SPACE";
		}

		case TiledSpace3D::CHANGE_TILED_SPACE_SIZE_MESSAGE:
		{
			return "CHANGE_TILED_SPACE_SIZE";
		}

		case TiledSpace3D::CREATE_SOLID_TILE_MESSAGE:
		{
			return "CREATE_SOLID_TILE";
		}

		case TiledSpace3D::CREATE_EMPTY_TILE_MESSAGE:
		{
			return "CREATE_EMPTY_TILE";
		}

		case TiledSpace3D::CREATE_BRUSH_MESSAGE:
		{
			return "CREATE_BRUSH";
		}

		case TiledSpace3D::REMOVE_BRUSH_MESSAGE:
		{
			return "REMOVE_BRUSH";
		}

		case FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE:
		{
			return "STREAMING_FLOAT_CURRENT";
		}

		case FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE:
		{
			return "STREAMING_FLOAT_DELTA";
		}
	
		case FloatLerp::STREAMING_FLOAT_T_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_T_ALPHA_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_T_RED_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_T_GREEN_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_T_BLUE_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_T_HUE_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_T_SATURATION_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_T_LIGHTNESS_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_T_DELTA_SCALE_MESSAGE:
		case FloatLerp::STREAMING_FLOAT_T_ANIMATION_FRAME_MESSAGE:
		{
			return "STREAMING_FLOAT_T";
		}

		case FloatLerp::CHANGE_FLOAT_LERP_SPEED_MESSAGE:
		{
			return "CHANGE_FLOAT_LERP_SPEED";
		}

		case FloatLerp::CHANGE_FLOAT_LERP_FLOAT_TARGET_MESSAGE:
		case FloatLerp::CHANGE_FLOAT_LERP_INTEGER_TARGET_MESSAGE:
		{
			return "CHANGE_FLOAT_LERP_TARGET";
		}

		case FloatLerp::CHANGE_FLOAT_LERP_MODE_MESSAGE:
		{
			return "CHANGE_FLOAT_LERP_MODE";
		}

		case FloatLerp::CHANGE_FLOAT_LERP_T_MESSAGE:
		{
			return "CHANGE_FLOAT_LERP_T";
		}

		case FloatLerp::GET_FLOAT_LERP_T_MESSAGE:
		{
			return "GET_FLOAT_LERP_T";
		}

		case FloatLerp::GET_FLOAT_LERP_CURRENT_MESSAGE:
		{
			return "GET_FLOAT_LERP_CURRENT";
		}

		case FloatLerp::GET_FLOAT_LERP_DELTA_MESSAGE:
		{
			return "GET_FLOAT_LERP_DELTA";
		}

		case QuaternionLerp::CREATE_QUATERNION_LERP_MESSAGE:
		{
			return "CREATE_QUATERNION_LERP";
		}

		case QuaternionLerp::STREAMING_QUATERNION_CURRENT_MESSAGE:
		case QuaternionLerp::STREAMING_ROTATABLE_CURRENT_MESSAGE:
		{
			return "STREAMING_QUATERNION_CURRENT";
		}

		case QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE:
		case QuaternionLerp::STREAMING_ROTATABLE_DELTA_MESSAGE:
		{
			return "STREAMING_QUATERNION_DELTA";
		}

		case QuaternionLerp::GET_DATA_FROM_CONTROL_QUATERNION_MESSAGE:
		case QuaternionLerp::GET_DATA_FROM_CONTROL_ROTATABLE_MESSAGE:
		{
			return "GET_DATA_FROM_CONTROL_QUATERNION";
		}

		case QuaternionLerp::SET_TWO_QUATERNIONS_MESSAGE:
		case QuaternionLerp::SET_TWO_ROTATABLES_MESSAGE:
		{
			return "SET_TWO_QUATERNIONS";
		}

		case QuaternionLerp::CHANGE_CONTROL_QUATERNIONS_MESSAGE:
		case QuaternionLerp::CHANGE_CONTROL_ROTATABLES_MESSAGE:
		{
			return "CHANGE_CONTROL_QUATERNIONS";
		}

		case QuaternionLerp::GET_QUATERNION_LERP_PATH_MESSAGE:
		{
			return "GET_QUATERNION_LERP_PATH";
		}

		case QuaternionLerp::GET_QUATERNION_LERP_T_MESSAGE:
		case QuaternionLerp::GET_ROTATABLE_LERP_T_MESSAGE:
		{
			return "GET_QUATERNION_LERP_T";
		}

		case QuaternionLerp::GET_CONTROL_QUATERNIONS_MESSAGE:
		{
			return "GET_CONTROL_QUATERNIONS";
		}

		case VectorLerp::CREATE_VECTOR_LERP_MESSAGE:
		{
			return "CREATE_VECTOR_LERP";
		}
		
		case VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE:
		case VectorLerp::STREAMING_TRANSLATABLE_CURRENT_MESSAGE:
		case VectorLerp::STREAMING_SCALABLE_CURRENT_MESSAGE:
		case VectorLerp::STREAMING_COLORABLE_CURRENT_MESSAGE:
		{
			return "STREAMING_VECTOR_CURRENT";
		}

		case VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE:
		case VectorLerp::STREAMING_TRANSLATABLE_DELTA_MESSAGE:
		case VectorLerp::STREAMING_SCALABLE_DELTA_MESSAGE:
		case VectorLerp::STREAMING_COLORABLE_DELTA_MESSAGE:
		{
			return "STREAMING_VECTOR_DELTA";
		}

		case VectorLerp::GET_DATA_FROM_CONTROL_VECTOR_MESSAGE:
		case VectorLerp::GET_DATA_FROM_CONTROL_TRANSLATABLE_MESSAGE:
		case VectorLerp::GET_DATA_FROM_CONTROL_SCALABLE_MESSAGE:
		case VectorLerp::GET_DATA_FROM_CONTROL_COLORABLE_MESSAGE:
		{
			return "GET_DATA_FROM_CONTROL_VECTOR";
		}

		case VectorLerp::SET_TWO_VECTORS_MESSAGE:
		case VectorLerp::SET_TWO_TRANSLATABLES_MESSAGE:
		case VectorLerp::SET_TWO_SCALABLES_MESSAGE:
		case VectorLerp::SET_TWO_COLORABLES_MESSAGE:
		{
			return "SET_TWO_VECTORS";
		}

		case VectorLerp::CHANGE_CONTROL_VECTORS_MESSAGE:
		case VectorLerp::CHANGE_CONTROL_TRANSLATABLES_MESSAGE:
		case VectorLerp::CHANGE_CONTROL_SCALABLES_MESSAGE:
		case VectorLerp::CHANGE_CONTROL_COLORABLES_MESSAGE:
		{
			return "CHANGE_CONTROL_VECTORS";
		}

		case VectorLerp::CHANGE_VECTOR_LERP_MODE_MESSAGE:
		{
			return "CHANGE_VECTOR_LERP_MODE";
		}

		case VectorLerp::GET_VECTOR_LERP_PATH_MESSAGE:
		{
			return "GET_VECTOR_LERP_PATH";
		}

		case VectorLerp::GET_VECTOR_LERP_T_MESSAGE:
		case VectorLerp::GET_TRANSLATABLE_LERP_T_MESSAGE:
		case VectorLerp::GET_SCALABLE_LERP_T_MESSAGE:
		case VectorLerp::GET_COLORABLE_LERP_T_MESSAGE:
		{
			return "GET_VECTOR_LERP_T";
		}

		case VectorLerp::GET_CONTROL_VECTORS_MESSAGE:
		{
			return "GET_CONTROL_VECTORS";
		}

		case GraphicsStrip::CHANGE_STRIP_VECTORS_MESSAGE:
		case GraphicsStrip::CHANGE_STRIP_TRANSLATABLES_MESSAGE:
		case GraphicsStrip::CHANGE_STRIP_COLORABLES_MESSAGE:
		case GraphicsStrip::CHANGE_STRIP_SCALABLES_MESSAGE:
		{
			return "CHANGE_STRIP_POINTS";
		}

		case RotatableObject::REPLACE_ROTATION_MESSAGE:
		{
			return "REPLACE_ROTATION";
		}

		case RotatableObject::DISPLACE_ROTATION_MESSAGE:
		{
			return "DISPLACE_ROTATION";
		}

		case RotatableObject::GET_ROTATION_MESSAGE:
		{
			return "GET_ROTATION";
		}

		case RotatableObject::CREATE_ROTATION_MESSAGE:
		{
			return "CREATE_ROTATION";
		}

		case ScalableObject::REPLACE_SCALE_MESSAGE:
		{
			return "REPLACE_SCALE";
		}

		case ScalableObject::DISPLACE_SCALE_MESSAGE:
		{
			return "DISPLACE_SCALE";
		}

		case ScalableObject::GET_SCALE_MESSAGE:
		{
			return "GET_SCALE";
		}

		case TranslatableObject::REPLACE_POSITION_MESSAGE:
		{
			return "REPLACE_POSITION";
		}

		case TiledSpace3D::GET_TILE_CENTER_MESSAGE:
		{
			return "GET_TILE_CENTER";
		}

		case TranslatableObject::DISPLACE_POSITION_MESSAGE:
		{
			return "DISPLACE_POSITION";
		}

		case TranslatableObject::GET_POSITION_MESSAGE:
		{
			return "GET_POSITION";
		}

		case TranslatableObject::MOVE_AROUND_PIVOT_MESSAGE:
		{
			return "MOVE_AROUND_PIVOT";
		}

		case RotatableObject::ROTATE_AROUND_PIVOT_MESSAGE:
		{
			return "ROTATE_AROUND_PIVOT";
		}

		case ColorableObject::CHANGE_COLOR_RGB_MESSAGE:
		case ColorableObject::CHANGE_COLOR_HSL_MESSAGE:
		{
			return "CHANGE_COLOR";
		}

		case ColorableObject::GET_COLOR_RGB_MESSAGE:
		case ColorableObject::GET_COLOR_HSL_MESSAGE:
		{
			return "GET_COLOR";
		}

		case ColorableObject::CHANGE_ALPHA_MESSAGE:
		case ColorableObject::CHANGE_RED_MESSAGE:
		case ColorableObject::CHANGE_GREEN_MESSAGE:
		case ColorableObject::CHANGE_BLUE_MESSAGE:
		case ColorableObject::CHANGE_HUE_MESSAGE:
		case ColorableObject::CHANGE_SATURATION_MESSAGE:
		case ColorableObject::CHANGE_LIGHTNESS_MESSAGE:
		{
			return "CHANGE_COLOR_COMPONENT";
		}

		case ColorableObject::GET_ALPHA_MESSAGE:
		case ColorableObject::GET_RED_MESSAGE:
		case ColorableObject::GET_GREEN_MESSAGE:
		case ColorableObject::GET_BLUE_MESSAGE:
		case ColorableObject::GET_HUE_MESSAGE:
		case ColorableObject::GET_SATURATION_MESSAGE:
		case ColorableObject::GET_LIGHTNESS_MESSAGE:
		{ 
			return "GET_COLOR_COMPONENT";
		}

		case Mesh::CREATE_QUAD_MESSAGE:
		{
			return "CREATE_QUAD";
		}

		case Mesh::CREATE_BOX_MESSAGE:
		{
			return "CREATE_BOX";
		}
		
		case PolygonalSurface::CHECK_LINE_IN_POLYGON_MESSAGE:
		{
			return "CHECK_LINE_IN_POLYGON";
		}

		case PolygonalSurface::CHANGE_MESH_MESSAGE:
		{
			return "CHANGE_MESH";
		}

		case PolygonalSurface::CHANGE_HITBOX_MESSAGE:
		{
			return "CHANGE_HITBOX";
		}

		case PolygonalSurface::ENABLE_COLLISION_POLYGONS_INFO_MESSAGE:
		{
			return "ENABLE_COLLISION_POLYGONS_INFO";
		}

		case GraphicsSurface::ENABLE_STATIC_OBJECT_MESSAGE:
		{
			return "ENABLE_STATIC_OBJECT";
		}

		case PolygonalSurface::ENABLE_COLLISION_CHECKING_MESSAGE:
		{
			return "ENABLE_COLLISION_CHECKING";
		}

		case PolygonalSurface::CHANGE_COLLISION_GROUP_MESSAGE:
		{
			return "CHANGE_COLLISION_GROUP";
		}

		case PolygonalSurface::CHECK_COLLISION_OBJECT_OBJECT_MESSAGE:
		case PolygonalSurface::CHECK_COLLISION_GROUP_GROUP_MESSAGE:
		case PolygonalSurface::CHECK_COLLISION_OBJECT_GROUP_MESSAGE: 
		{
			return "CHECK_COLLISION";
		}

		case AbstractSurface::CHECK_LINE_IN_SURFACE_MESSAGE:
		{
			return "CHECK_LINE_IN_SURFACE";
		}

		case AbstractSurface::CHANGE_INTERSECTION_MODE_MESSAGE:
		{
			return "CHANGE_INTERSECTION_MODE";
		}

		case GraphicsSurface::ENABLE_BACK_FACE_CULLING_MESSAGE:
		{
			return "ENABLE_BACK_FACE_CULLING";
		}

		case Variable<float>::CHANGE_FLOAT_RANGE_MESSAGE:
		case Variable<int32>::CHANGE_INTEGER_RANGE_MESSAGE:
		{
			return "CHANGE_VALUE_RANGE"; 
		}

		case Random::GET_RANDOM_INTEGER_MESSAGE:
		case Random::GET_RANDOM_FLOAT_MESSAGE:
		case Random::GET_RANDOM_BOOLEAN_MESSAGE:
		{
			return "GET_RANDOM_VALUE";
		}

		case Random::CHANGE_RANDOM_SEED_MESSAGE:
		{
			return "CHANGE_RANDOM_SEED";
		}

		case AbstractList::ADD_TO_LIST_MESSAGE:
		{
			return "ADD_TO_LIST";
		}

		case AbstractList::REMOVE_INDEX_FROM_LIST_MESSAGE:
		case AbstractList::REMOVE_ITEM_FROM_LIST_MESSAGE:
		{
			return "REMOVE_FROM_LIST";
		}

		case AbstractList::GET_DATA_FROM_ITEM_MESSAGE:
		{
			return "GET_DATA_FROM_ITEM";
		}

		case AbstractList::GET_ITEM_INDEX_MESSAGE:
		{
			return "GET_ITEM_INDEX";
		}

		case AbstractList::COPY_LIST_MESSAGE:
		{
			return "COPY_LIST";
		}

		case AbstractList::RANDOMIZE_LIST_MESSAGE:
		{
			return "RANDOMIZE_LIST";
		}

		case AbstractList::SWAP_LIST_MESSAGE: 
		case AbstractList::SWAP_INDEX_MESSAGE: 
		case AbstractList::SWAP_ITEM_MESSAGE: 
		{
			return "SWAP_ITEMS";
		}

		case AbstractList::CLEAR_LIST_MESSAGE:
		{
			return "CLEAR_LIST";
		}

		case AbstractList::INVERSE_LIST_MESSAGE:
		{
			return "INVERSE_LIST";
		}

		case PlayerControl::ADD_INPUT_EVENT_MESSAGE:
		{
			return "ADD_INPUT_EVENT";
		}

		case PlayerControl::CLEAR_INPUT_EVENTS_MESSAGE:
		{
			return "CLEAR_INPUT_EVENTS";
		}

		case AbstractList::CREATE_LIST_MESSAGE:
		{
			return "CREATE_LIST";
		}

		case AbstractList::GET_AMOUNT_OF_ITEMS_MESSAGE:
		{
			return "GET_AMOUNT_OF_ITEMS";
		}

		case ActivatableObject::CHANGE_DEACTIVATION_MODE_MESSAGE:
		{
			return "CHANGE_DEACTIVATION_MODE";
		}

		case ActivatableObject::CHANGE_ACTIVATION_LIMIT_MESSAGE:
		{
			return "CHANGE_ACTIVATION_LIMIT";
		}

		case AbstractList::CHECK_LIST_EMPTY_MESSAGE:
		{
			return "CHECK_LIST_EMPTY";
		}

		case AbstractList::ROTATE_LIST_MESSAGE:
		{
			return "ROTATE_LIST";
		}

		case AbstractList::CHANGE_LIST_INTEGER_MESSAGE:
		case AbstractList::CHANGE_LIST_FLOAT_MESSAGE:
		case AbstractList::CHANGE_LIST_STRING_MESSAGE:
		case AbstractList::CHANGE_LIST_BOOLEAN_MESSAGE:
		case AbstractList::CHANGE_LIST_VECTOR_2_MESSAGE:
		case AbstractList::CHANGE_LIST_VECTOR_3_MESSAGE:
		case AbstractList::CHANGE_LIST_QUATERNION_MESSAGE:
		case AbstractList::CHANGE_LIST_ROTATABLE_MESSAGE:
		case AbstractList::CHANGE_LIST_SCALABLE_MESSAGE:
		case AbstractList::CHANGE_LIST_TRANSLATABLE_MESSAGE:
		case AbstractList::CHANGE_LIST_COLORABLE_MESSAGE:
		{
			return "CHANGE_LIST_VALUE";
		}

		case AbstractList::CHANGE_ITEM_INTEGER_MESSAGE:
		case AbstractList::CHANGE_ITEM_FLOAT_MESSAGE:
		case AbstractList::CHANGE_ITEM_STRING_MESSAGE:
		case AbstractList::CHANGE_ITEM_BOOLEAN_MESSAGE:
		case AbstractList::CHANGE_ITEM_VECTOR_2_MESSAGE:
		case AbstractList::CHANGE_ITEM_VECTOR_3_MESSAGE:
		case AbstractList::CHANGE_ITEM_QUATERNION_MESSAGE:
		case AbstractList::CHANGE_ITEM_ROTATABLE_MESSAGE:
		case AbstractList::CHANGE_ITEM_SCALABLE_MESSAGE:
		case AbstractList::CHANGE_ITEM_TRANSLATABLE_MESSAGE:
		case AbstractList::CHANGE_ITEM_COLORABLE_MESSAGE:
		{
			return "CHANGE_ITEM_VALUE";
		}

		case AbstractList::GET_LIST_LENGTH_MESSAGE:
		{
			return "GET_LIST_LENGTH";
		}

		case LogicScene::ADD_TO_UPDATE_LIST_MESSAGE:
		{
			return "ADD_TO_UPDATE_LIST";
		}

		case LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE:
		{
			return "REMOVE_FROM_UPDATE_LIST";
		}

		case GraphicsScene::ADD_TO_RENDER_LIST_MESSAGE:
		{
			return "ADD_TO_RENDER_LIST";
		}

		case GraphicsScene::REMOVE_FROM_RENDER_LIST_MESSAGE:
		{
			return "REMOVE_FROM_RENDER_LIST";
		}

		case GraphicsContainer::CHANGE_CAMERA_MESSAGE:
		{
			return "CHANGE_CAMERA";
		}

		case GraphicsContainer::CHANGE_RENDER_TARGET_MESSAGE:
		{
			return "CHANGE_RENDER_TARGET";
		}

		case Scene::PICK_SCENE_MESSAGE:
		{
			return "PICK_SCENE";
		}

		case Camera3D::PICK_CAMERA_MESSAGE:
		{
			return "PICK_CAMERA";
		}

		case Camera3D::CHANGE_PROJECTION_MODE_MESSAGE:
		{
			return "CHANGE_PROJECTION_MODE";
		}

		case Camera3D::CHANGE_MOVEMENT_MODE_MESSAGE:
		{
			return "CHANGE_MOVEMENT_MODE";
		}

		case Camera3D::CHANGE_ROTATION_MODE_MESSAGE:
		{
			return "CHANGE_ROTATION_MODE";
		}

		case Camera3D::GET_SELECT_RAY_MESSAGE:
		{
			return "GET_SELECT_RAY";
		}

		case Camera3D::CHANGE_VIEWING_POINT_MESSAGE:
		{
			return "CHANGE_VIEWING_POINT";
		}

		case Camera3D::CHANGE_DISTANCE_TO_VIEWING_POINT_MESSAGE:
		{
			return "CHANGE_DISTANCE_TO_VIEWING_POINT";
		}

		case Camera3D::CAMERA_LOOK_AT_MESSAGE:
		{
			return "CAMERA_LOOK_AT";
		}

		case Camera3D::CHECK_FRUSTUM_IN_QUAD_MESSAGE:
		{
			return "CHECK_FRUSTUM_IN_QUAD";
		}

		case Camera3D::GET_VIEWING_POINT_MESSAGE:
		{
			return "GET_VIEWING_POINT";
		}

		case Camera3D::ENABLE_ZOOM_MESSAGE:
		{
			return "ENABLE_ZOOM";
		}

		case Camera3D::ENABLE_ROTATION_RANGE_MESSAGE:
		{
			return "ENABLE_ROTATION_RANGE";
		}

		case Camera3D::CHANGE_ROTATION_RANGE_MESSAGE:
		{
			return "CHANGE_ROTATION_RANGE";
		}

		case Camera3D::ENABLE_ROTATION_MESSAGE:
		{
			return "ENABLE_ROTATION";
		}

		case Camera3D::CHANGE_ZOOM_SPEED_MESSAGE:
		{
			return "CHANGE_ZOOM_SPEED";
		}

		case Camera3D::CHANGE_ZOOM_FAR_MESSAGE:
		{
			return "CHANGE_ZOOM_FAR";
		}

		case Camera3D::CHANGE_ZOOM_NEAR_MESSAGE:
		{
			return "CHANGE_ZOOM_NEAR";
		}

		case ObjectScene::ADD_TO_LAYER_MESSAGE:
		{
			return "ADD_TO_LAYER";
		}

		case ObjectScene::REMOVE_FROM_LAYER_MESSAGE:
		{
			return "REMOVE_FROM_LAYER";
		}

		case AbstractVariable::CREATE_INTEGER_VARIABLE_MESSAGE:
		case AbstractVariable::CREATE_FLOAT_VARIABLE_MESSAGE:
		case AbstractVariable::CREATE_STRING_VARIABLE_MESSAGE:
		case AbstractVariable::CREATE_BOOLEAN_VARIABLE_MESSAGE:
		case AbstractVariable::CREATE_VECTOR_2_VARIABLE_MESSAGE:
		case AbstractVariable::CREATE_VECTOR_3_VARIABLE_MESSAGE:
		case AbstractVariable::CREATE_QUATERNION_VARIABLE_MESSAGE:
		{
			return "CREATE_VARIABLE";
		}

		case AbstractVariable::CHANGE_INTEGER_VALUE_MESSAGE:
		case AbstractVariable::CHANGE_FLOAT_VALUE_MESSAGE:
		case AbstractVariable::CHANGE_STRING_VALUE_MESSAGE:
		case AbstractVariable::CHANGE_BOOLEAN_VALUE_MESSAGE:
		case AbstractVariable::CHANGE_VECTOR_2_VALUE_MESSAGE:
		case AbstractVariable::CHANGE_VECTOR_3_VALUE_MESSAGE:
		case AbstractVariable::CHANGE_QUATERNION_VALUE_MESSAGE:
		{
			return "CHANGE_VALUE";
		}

		case Variable<Vector3>::CHANGE_VECTOR_3_COMPONENT_MESSAGE:
		case Variable<Vector2>::CHANGE_VECTOR_2_COMPONENT_MESSAGE:
		{
			return "CHANGE_VECTOR_COMPONENT";
		}

		case Variable<Quaternion>::CREATE_ROTATION_FROM_VECTOR_3_TO_VECTOR_3_MESSAGE:
		case Variable<Quaternion>::CREATE_ROTATION_FROM_QUATERNION_TO_QUATERNION_MESSAGE:
		{
			return "CREATE_ROTATION_FROM_TO";
		}
 
		case Camera3D::GET_CAMERA_LOOK_MESSAGE:
		{
			return "GET_CAMERA_LOOK";
		}

		case Variable<Vector3>::GET_VECTOR_3_COMPONENT_MESSAGE:
		case Variable<Vector2>::GET_VECTOR_2_COMPONENT_MESSAGE:
		{
			return "GET_VECTOR_COMPONENT";
		}

		case Variable<Vector2>::VECTOR_2_TO_SPHERICAL_MESSAGE:
		case Variable<Vector3>::VECTOR_3_TO_SPHERICAL_MESSAGE:
		{
			return "VECTOR_TO_SPHERICAL";
		}

		case Variable<Vector3>::GET_VECTOR_3_LENGTH_MESSAGE:
		case Variable<Vector2>::GET_VECTOR_2_LENGTH_MESSAGE:
		{
			return "GET_VECTOR_LENGTH";
		}

		case Variable<Vector3>::GET_NORMALIZED_VECTOR_3_MESSAGE:
		case Variable<Vector2>::GET_NORMALIZED_VECTOR_2_MESSAGE:
		{
			return "GET_NORMALIZED_VECTOR";
		}
		
		case Trigger::CREATE_TRIGGER_MESSAGE:
		{
			return "CREATE_TRIGGER";
		}

		case AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE:
		{
			return "CHANGE_ANIMATION_FRAME";
		}

		case AbstractAnimation::GET_AMOUNT_OF_FRAMES_MESSAGE:
		{
			return "GET_AMOUNT_OF_FRAMES";
		}

		case TexturedAnimation::CHANGE_ATLAS_MESSAGE:
		{
			return "CHANGE_ATLAS";
		}

		case TexturedAnimation::GET_FRAME_SIZE_MESSAGE:
		{
			return "GET_FRAME_SIZE";
		}

		case TexturedAnimation::ENABLE_POSITION_OFFSET_MESSAGE:
		{
			return "ENABLE_POSITION_OFFSET";
		}

		case GraphicsObject::ENABLE_SHOW_MESSAGE:
		{
			return "ENABLE_SHOW";
		}

		case AbstractVariable::CHECK_EQUALITY_MESSAGE:
		{
			return "CHECK_EQUALITY";
		}

		case LogicObject::ENABLE_FREEZE_MESSAGE:
		{
			return "ENABLE_FREEZE";
		}
		
		case LogicScene::CHANGE_DELTA_SCALE_MESSAGE:
		{
			return "CHANGE_DELTA_SCALE";
		}

		case Texture2D::GET_TEXTURE_SIZE_MESSAGE:
		{
			return "GET_TEXTURE_SIZE";
		}
	
		case LerpingObject::CHANGE_FLOAT_LERP_MESSAGE:
		{
			return "CHANGE_FLOAT_LERP";
		}

		case ColorableObject::CHANGE_BACKGROUND_COLOR_MESSAGE:
		{
			return "CHANGE_BACKGROUND_COLOR";
		}

		case TexturingSurface::CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE:
		{
			return "CHANGE_FRONT_TEXTURED_ANIMATION";
		}

		case TexturingSurface::CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE:
		{
			return "CHANGE_BACK_TEXTURED_ANIMATION";
		}

		case TexturingSurface::CHANGE_FRONT_TEXTURE_MESSAGE:
		{
			return "CHANGE_FRONT_TEXTURE";
		}

		case TexturingSurface::CHANGE_BACK_TEXTURE_MESSAGE:
		{
			return "CHANGE_BACK_TEXTURE";
		}

		case TexturingSurface::CHECK_LINE_IN_IMAGE_MESSAGE:
		{
			return "CHECK_LINE_IN_IMAGE";
		}

		case TexturingSurface::ENABLE_TEXTURE_FLIP_MESSAGE:
		{
			return "ENABLE_TEXTURE_FLIP";
		}

		case VectorLerp::SET_DATA_TO_CONTROL_VECTOR_MESSAGE:
		case VectorLerp::SET_DATA_TO_CONTROL_TRANSLATABLE_MESSAGE:
		case VectorLerp::SET_DATA_TO_CONTROL_SCALABLE_MESSAGE:
		case VectorLerp::SET_DATA_TO_CONTROL_COLORABLE_MESSAGE:
		{
			return "SET_DATA_TO_CONTROL_VECTOR";
		}

		case QuaternionLerp::SET_DATA_TO_CONTROL_QUATERNION_MESSAGE:
		case QuaternionLerp::SET_DATA_TO_CONTROL_ROTATABLE_MESSAGE:
		{
			return "SET_DATA_TO_CONTROL_QUATERNION";
		}
	}
	return "";
}


StringANSI AbstractAction::_GetDir(void)
{
	return actionsDir;
}


void AbstractAction::_SetFileSuffix(StringANSI _suffix)
{
	actionsSuffix = _suffix;
}


StringANSI AbstractAction::_GetFileSuffix(void)
{
	return actionsSuffix;
}


 
AbstractAction* AbstractAction::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + actionsDir + _path + "." + actionsSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}



AbstractAction* AbstractAction::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		int32 type;
		_file.Read(type);

		switch(type)
		{
			case AbstractVariable::CHECK_EQUALITY_MESSAGE:
			{
				return CheckEqualityAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::GET_CONTROL_QUATERNIONS_MESSAGE:
			{
				return GetControlQuaternionsAction::_LoadFromFile(_file);
			}

			case VectorLerp::GET_CONTROL_VECTORS_MESSAGE:
			{
				return GetControlVectorsAction::_LoadFromFile(_file);
			}

			case Circle::GET_RADIUS_FROM_2_POINTS_MESSAGE:
			{
				return GetRadiusFrom2PointsAction::_LoadFromFile(_file);
			}

			case Circle::GET_RADIUS_FROM_3_POINTS_MESSAGE:
			{
				return GetRadiusFrom3PointsAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::GET_QUATERNION_LERP_PATH_MESSAGE:
			{
				return GetQuaternionLerpPathAction::_LoadFromFile(_file);
			}

			case AbstractList::GET_LIST_LENGTH_MESSAGE:
			{
				return GetListLengthAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::GET_QUATERNION_LERP_T_MESSAGE:
			{
				return GetQuaternionLerpTAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::GET_ROTATABLE_LERP_T_MESSAGE:
			{
				return GetRotatableLerpTAction::_LoadFromFile(_file);
			}
			
			case VectorLerp::GET_VECTOR_LERP_T_MESSAGE:
			{
				return GetVectorLerpTAction::_LoadFromFile(_file);
			}

			case VectorLerp::GET_TRANSLATABLE_LERP_T_MESSAGE:
			{
				return GetTranslatableLerpTAction::_LoadFromFile(_file);
			}

			case VectorLerp::GET_SCALABLE_LERP_T_MESSAGE:
			{
				return GetScalableLerpTAction::_LoadFromFile(_file);
			}

			case VectorLerp::GET_COLORABLE_LERP_T_MESSAGE:
			{
				return GetColorableLerpTAction::_LoadFromFile(_file);
			}
			
			case GraphicsStrip::CHANGE_STRIP_COLORABLES_MESSAGE: 
			{
				return ChangeStripColorablesAction::_LoadFromFile(_file);
			}
			
			case GraphicsStrip::CHANGE_STRIP_SCALABLES_MESSAGE:
            {
				return ChangeStripScalablesAction::_LoadFromFile(_file);
			}
            
			case GraphicsStrip::CHANGE_STRIP_TRANSLATABLES_MESSAGE: 
			{
				return ChangeStripTranslatablesAction::_LoadFromFile(_file);
			}
			
			case GraphicsStrip::CHANGE_STRIP_VECTORS_MESSAGE:
			{
				return ChangeStripVectorsAction::_LoadFromFile(_file);
			}

			case VectorLerp::GET_VECTOR_LERP_PATH_MESSAGE:
			{
				return GetVectorLerpPathAction::_LoadFromFile(_file);
			}

			case Camera3D::GET_VIEWING_POINT_MESSAGE:
			{
				return GetViewingPointAction::_LoadFromFile(_file);
			}

			case Camera3D::ENABLE_ZOOM_MESSAGE:
			{
				return EnableZoomAction::_LoadFromFile(_file);
			}

			case Camera3D::ENABLE_ROTATION_RANGE_MESSAGE:
			{
				return EnableRotationRangeAction::_LoadFromFile(_file);
			}

			case Camera3D::CHANGE_ROTATION_RANGE_MESSAGE:
			{
				return ChangeRotationRangeAction::_LoadFromFile(_file);
			}

			case Camera3D::ENABLE_ROTATION_MESSAGE:
			{
				return EnableRotationAction::_LoadFromFile(_file);
			}

			case Camera3D::CHANGE_ZOOM_SPEED_MESSAGE:
			{
				return ChangeZoomSpeedAction::_LoadFromFile(_file);
			}

			case Camera3D::CHANGE_ZOOM_FAR_MESSAGE:
			{
				return ChangeZoomFarAction::_LoadFromFile(_file);
			}

			case Camera3D::CHANGE_ZOOM_NEAR_MESSAGE:
			{
				return ChangeZoomNearAction::_LoadFromFile(_file);
			}

			case AbstractObject::CHANGE_NAME_MESSAGE:
			{
				return ChangeNameAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::TILED_SPACE_FILL_MESSAGE:
			{
				return TiledSpaceFillAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::CHECK_TILE_EXIST_MESSAGE:
			{
				return CheckTileExistAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::CHANGE_TILE_PASS_MESSAGE:
			{
				return ChangeTilePassAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::CHECK_TILE_PASS_MESSAGE:
			{
				return CheckTilePassAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::CHECK_TILE_SOLID_MESSAGE:
			{
				return CheckTileSolidAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::SET_PUBLIC_TILE_MESSAGE:
			{
				return SetPublicTileAction::_LoadFromFile(_file);
			}

			case TexturingSurface::SET_PUBLIC_TEXTURE_MESSAGE:
			{
				return SetPublicTextureAction::_LoadFromFile(_file);
			}

			case AssetLibrary::SET_PRIVATE_ASSET_MESSAGE:
			{
				return SetPrivateAssetAction::_LoadFromFile(_file);
			}

			case Texture2D::CREATE_BOX_TEXTURE_MAP_MESSAGE:
			{
				return CreateBoxTextureMapAction::_LoadFromFile(_file);
			}

			case Texture2D::CHANGE_BOX_TEXTURE_MAP_MESSAGE:
			{
				return ChangeBoxTextureMapAction::_LoadFromFile(_file);
			}

			case Timer::CREATE_TIMER_MESSAGE:
			{
				return CreateTimerAction::_LoadFromFile(_file);
			}

			case Texture2D::CREATE_TEXTURE_MESSAGE:
			{
				return CreateTextureAction::_LoadFromFile(_file);
			}

			case TexturedAnimation::CREATE_TEXTURED_ANIMATION_MESSAGE:
			{
				return CreateTexturedAnimationAction::_LoadFromFile(_file);
			}

			case Mesh::CREATE_MESH_MESSAGE:
			{
				return CreateMeshAction::_LoadFromFile(_file);
			}

			case Set::GET_DATA_FROM_SLOT_MESSAGE:
			{
				return GetDataFromSlotAction::_LoadFromFile(_file);
			}

			case Set::SET_DATA_TO_SLOT_MESSAGE:
			{
				return SetDataToSlotAction::_LoadFromFile(_file);
			}

			case Set::RESIZE_SET_MESSAGE:
			{
				return ResizeSetAction::_LoadFromFile(_file);
			}

			case Set::GET_AMOUNT_OF_SLOTS_MESSAGE:
			{
				return GetAmountOfSlotsAction::_LoadFromFile(_file);
			}

			case Set::CREATE_SET_MESSAGE:
			{
				return CreateSetAction::_LoadFromFile(_file);
			}

			case PlayerControl::CHANGE_INPUT_TIMING_MESSAGE:
			{
				return ChangeInputTimingAction::_LoadFromFile(_file);
			}

			case PlayingObject::REPLAY_MESSAGE:
			{
				return ReplayAction::_LoadFromFile(_file);
			}

			case PlayerControl::SET_ONE_INPUT_EVENT_MESSAGE:
			{
				return SetOneInputEventAction::_LoadFromFile(_file);
			}

			case PlayerControl::CREATE_PLAYER_CONTROL_MESSAGE:
			{
				return CreatePlayerControlAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::MOVE_TILE_FROM_TO_MESSAGE:
			{
				return MoveTileFromToAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::GET_BRUSH_OF_TILE_MESSAGE:
			{
				return GetBrushOfTileAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::SWAP_TWO_TILES_MESSAGE:
			{
				return SwapTwoTilesAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::COPY_TILE_MESSAGE:
			{
				return CopyTileAction::_LoadFromFile(_file);
			}

			case Camera3D::CREATE_CAMERA_MESSAGE:
			{
				return CreateCameraAction::_LoadFromFile(_file);
			}

			case SystemTime::GET_SYSTEM_TIME_MESSAGE: 
			{
				return GetSystemTimeAction::_LoadFromFile(_file);
			}

			case Trigger::CREATE_TRIGGER_MESSAGE:
			{
				return CreateTriggerAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CHANGE_INTEGER_VALUE_MESSAGE: 
			{
				return ChangeIntegerValueAction::_LoadFromFile(_file);
			}
			
			case AbstractVariable::CHANGE_FLOAT_VALUE_MESSAGE: 
			{
				return ChangeFloatValueAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CHANGE_BOOLEAN_VALUE_MESSAGE: 
			{
				return ChangeBooleanValueAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CHANGE_STRING_VALUE_MESSAGE:
			{
				return ChangeStringValueAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CHANGE_VECTOR_2_VALUE_MESSAGE: 
			{
				return ChangeVector2ValueAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CHANGE_VECTOR_3_VALUE_MESSAGE: 
			{
				return ChangeVector3ValueAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CHANGE_QUATERNION_VALUE_MESSAGE: 
			{
				return ChangeQuaternionValueAction::_LoadFromFile(_file);
			}

			case AssetLibrary::LOAD_ASSET_MESSAGE:
			{ 
				return LoadAssetAction::_LoadFromFile(_file);
			}

			case AbstractObject::GET_NAME_MESSAGE: 
			{
				return GetNameAction::_LoadFromFile(_file);
			}

			case Variable<Vector3>::GET_NORMALIZED_VECTOR_3_MESSAGE:
			{
				return GetNormalizedVector3Action::_LoadFromFile(_file);
			}
			
			case Variable<Vector2>::GET_NORMALIZED_VECTOR_2_MESSAGE:
			{
				return GetNormalizedVector2Action::_LoadFromFile(_file);
			}

			case Variable<Quaternion>::CREATE_ROTATION_FROM_VECTOR_3_TO_VECTOR_3_MESSAGE: 
			{
				return CreateRotationFromVector3ToVector3Action::_LoadFromFile(_file);
			}

			case Variable<Quaternion>::CREATE_ROTATION_FROM_QUATERNION_TO_QUATERNION_MESSAGE: 
			{
				return CreateRotationFromQuaternionToQuaternionAction::_LoadFromFile(_file);
			}

			case Camera3D::GET_CAMERA_LOOK_MESSAGE:
			{
				return GetCameraLookAction::_LoadFromFile(_file);
			}

			case Variable<Vector3>::GET_VECTOR_3_COMPONENT_MESSAGE:
			{
				return GetVector3ComponentAction::_LoadFromFile(_file);
			}

			case Variable<Vector2>::VECTOR_2_TO_SPHERICAL_MESSAGE:
			{
				return Vector2ToSphericalAction::_LoadFromFile(_file);
			}

			case Variable<Vector3>::VECTOR_3_TO_SPHERICAL_MESSAGE:
			{
				return Vector3ToSphericalAction::_LoadFromFile(_file);
			}

			case Variable<Vector3>::CHANGE_VECTOR_3_COMPONENT_MESSAGE:
			{
				return ChangeVector3ComponentAction::_LoadFromFile(_file);
			}

			case Variable<Vector2>::GET_VECTOR_2_COMPONENT_MESSAGE:
			{
				return GetVector2ComponentAction::_LoadFromFile(_file);
			}

			case Variable<Vector2>::CHANGE_VECTOR_2_COMPONENT_MESSAGE:
			{
				return ChangeVector2ComponentAction::_LoadFromFile(_file);
			}
			
			case AssetLibrary::CHECK_OBJECT_EXIST_MESSAGE:
			{
				return CheckObjectExistAction::_LoadFromFile(_file);
			}

			case AbstractObject::CHECK_PROPERTY_MESSAGE:
			{
				return CheckPropertyAction::_LoadFromFile(_file);
			}

			case AbstractObject::ENABLE_PROPERTY_MESSAGE:
			{
				return EnablePropertyAction::_LoadFromFile(_file);
			}

			case MessageHandler<int32>::SEND_MESSAGE:
			{
				return SendMessageAction::_LoadFromFile(_file);
			}

			case AbstractObject::DESTROY_OBJECT_MESSAGE:
			{
				return DestroyObjectAction::_LoadFromFile(_file);
			}

			case AbstractObject::SAVE_TO_FILE_MESSAGE:
			{
				return SaveToFileAction::_LoadFromFile(_file);
			}

			case AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE:
			{
				return ChangeAnimationFrameAction::_LoadFromFile(_file);
			}

			case PolygonalSurface::ENABLE_COLLISION_POLYGONS_INFO_MESSAGE:
			{
				return EnableCollisionPolygonsInfoAction::_LoadFromFile(_file);
			}

			case GraphicsSurface::ENABLE_STATIC_OBJECT_MESSAGE:
			{
				return EnableStaticObjectAction::_LoadFromFile(_file);
			}

			case PolygonalSurface::ENABLE_COLLISION_CHECKING_MESSAGE: 
			{
				return EnableCollisionCheckingAction::_LoadFromFile(_file);
			}

			case PolygonalSurface::CHECK_COLLISION_OBJECT_GROUP_MESSAGE:
			{
				return CheckCollisionObjectGroupAction::_LoadFromFile(_file);
			}

			case PolygonalSurface::CHECK_COLLISION_OBJECT_OBJECT_MESSAGE:
			{
				return CheckCollisionObjectObjectAction::_LoadFromFile(_file);
			}

			case PolygonalSurface::CHECK_COLLISION_GROUP_GROUP_MESSAGE:
			{
				return CheckCollisionGroupGroupAction::_LoadFromFile(_file);
			}

			case PolygonalSurface::CHANGE_COLLISION_GROUP_MESSAGE: 
			{
				return ChangeCollisionGroupAction::_LoadFromFile(_file);
			}

			case AbstractAnimation::GET_AMOUNT_OF_FRAMES_MESSAGE: 
			{
				return GetAmountOfFramesAction::_LoadFromFile(_file);
			}

			case TexturedAnimation::GET_FRAME_SIZE_MESSAGE: 
			{
				return GetFrameSizeAction::_LoadFromFile(_file);
			}

			case TexturedAnimation::CHANGE_ATLAS_MESSAGE:
			{
				return ChangeAtlasAction::_LoadFromFile(_file);
			}

			case Texture2D::GET_TEXTURE_SIZE_MESSAGE: 
			{
				return GetTextureSizeAction::_LoadFromFile(_file);
			}

			case Variable<float>::CHANGE_FLOAT_RANGE_MESSAGE:
			{
				return ChangeFloatValueRangeAction::_LoadFromFile(_file);
			}

			case Variable<int32>::CHANGE_INTEGER_RANGE_MESSAGE:
			{
				return ChangeIntegerValueRangeAction::_LoadFromFile(_file);
			}

			case Random::CHANGE_RANDOM_SEED_MESSAGE:
			{
				return ChangeRandomSeedAction::_LoadFromFile(_file);
			}

			case Random::GET_RANDOM_BOOLEAN_MESSAGE:
			{
				return GetRandomBooleanAction::_LoadFromFile(_file);
			}

			case Random::GET_RANDOM_INTEGER_MESSAGE:
			{
				return GetRandomIntegerAction::_LoadFromFile(_file);
			}

			case Random::GET_RANDOM_FLOAT_MESSAGE:
			{
				return GetRandomFloatAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_ITEM_INTEGER_MESSAGE:
			{
				return ChangeItemIntegerAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_ITEM_FLOAT_MESSAGE:
			{
				return ChangeItemFloatAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_ITEM_BOOLEAN_MESSAGE:
			{
				return ChangeItemBooleanAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_ITEM_STRING_MESSAGE:
			{
				return ChangeItemStringAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_ITEM_VECTOR_2_MESSAGE:
			{
				return ChangeItemVector2Action::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_ITEM_VECTOR_3_MESSAGE:
			{
				return ChangeItemVector3Action::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_ITEM_QUATERNION_MESSAGE:
			{
				return ChangeItemQuaternionAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_ITEM_COLORABLE_MESSAGE:
			{
				return ChangeItemColorableAction::_LoadFromFile(_file);
			}
	
			case AbstractList::CHANGE_ITEM_TRANSLATABLE_MESSAGE:
			{
				return ChangeItemTranslatableAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_ITEM_SCALABLE_MESSAGE:
			{
				return ChangeItemScalableAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_ITEM_ROTATABLE_MESSAGE:
			{
				return ChangeItemRotatableAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_LIST_INTEGER_MESSAGE:
			{
				return ChangeListIntegerAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_LIST_FLOAT_MESSAGE:
			{
				return ChangeListFloatAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_LIST_BOOLEAN_MESSAGE:
			{
				return ChangeListBooleanAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_LIST_STRING_MESSAGE:
			{
				return ChangeListStringAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_LIST_VECTOR_2_MESSAGE:
			{
				return ChangeListVector2Action::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_LIST_VECTOR_3_MESSAGE:
			{
				return ChangeListVector3Action::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_LIST_QUATERNION_MESSAGE:
			{
				return ChangeListQuaternionAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_LIST_COLORABLE_MESSAGE:
			{
				return ChangeListColorableAction::_LoadFromFile(_file);
			}
	
			case AbstractList::CHANGE_LIST_TRANSLATABLE_MESSAGE:
			{
				return ChangeListTranslatableAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_LIST_SCALABLE_MESSAGE:
			{
				return ChangeListScalableAction::_LoadFromFile(_file);
			}

			case AbstractList::CHANGE_LIST_ROTATABLE_MESSAGE:
			{
				return ChangeListRotatableAction::_LoadFromFile(_file);
			}

			case AbstractList::REMOVE_INDEX_FROM_LIST_MESSAGE:
			{
				return RemoveIndexFromListAction::_LoadFromFile(_file);
			} 

			case AbstractList::REMOVE_ITEM_FROM_LIST_MESSAGE:
			{
				return RemoveItemFromListAction::_LoadFromFile(_file);
			} 

			case AbstractList::ADD_TO_LIST_MESSAGE:
			{
				return AddToListAction::_LoadFromFile(_file);
			}

			case AbstractList::GET_DATA_FROM_ITEM_MESSAGE:
			{
				return GetDataFromItemAction::_LoadFromFile(_file);
			}

			case AbstractList::GET_ITEM_INDEX_MESSAGE:
			{
				return GetItemIndexAction::_LoadFromFile(_file);
			}
						
			case AbstractList::RANDOMIZE_LIST_MESSAGE: 
			{
				return RandomizeListAction::_LoadFromFile(_file);
			}

			case AbstractList::SWAP_LIST_MESSAGE: 
			{
				return SwapListAction::_LoadFromFile(_file);
			}

			case AbstractList::SWAP_ITEM_MESSAGE: 
			{
				return SwapItemAction::_LoadFromFile(_file);
			}

			case AbstractList::SWAP_INDEX_MESSAGE: 
			{
				return SwapIndexAction::_LoadFromFile(_file);
			}

			case AbstractList::GET_AMOUNT_OF_ITEMS_MESSAGE: 
			{
				return GetAmountOfItemsAction::_LoadFromFile(_file);
			}

			case ActivatableObject::CHANGE_DEACTIVATION_MODE_MESSAGE:
			{
				return ChangeDeactivationModeAction::_LoadFromFile(_file);
			}

			case ActivatableObject::CHANGE_ACTIVATION_LIMIT_MESSAGE:
			{
				return ChangeActivationLimitAction::_LoadFromFile(_file);
			}

			case AbstractList::CHECK_LIST_EMPTY_MESSAGE:
			{
				return CheckListEmptyAction::_LoadFromFile(_file);
			}

			case AbstractList::ROTATE_LIST_MESSAGE:
			{
				return RotateListAction::_LoadFromFile(_file);
			}

			case AbstractList::COPY_LIST_MESSAGE:
			{
				return CopyListAction::_LoadFromFile(_file);
			}

			case AbstractList::CLEAR_LIST_MESSAGE:
			{
				return ClearListAction::_LoadFromFile(_file);
			}

			case AbstractList::INVERSE_LIST_MESSAGE:
			{
				return InverseListAction::_LoadFromFile(_file);
			}

			case PlayerControl::ADD_INPUT_EVENT_MESSAGE:
			{
				return AddInputEventAction::_LoadFromFile(_file);
			}

			case PlayerControl::CLEAR_INPUT_EVENTS_MESSAGE:
			{
				return ClearInputEventsAction::_LoadFromFile(_file);
			}

			case AbstractList::CREATE_LIST_MESSAGE:
			{
				return CreateListAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CREATE_INTEGER_VARIABLE_MESSAGE:
			{ 
				return CreateIntegerVariableAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CREATE_FLOAT_VARIABLE_MESSAGE:
			{ 
				return CreateFloatVariableAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CREATE_BOOLEAN_VARIABLE_MESSAGE:
			{ 
				return CreateBooleanVariableAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CREATE_STRING_VARIABLE_MESSAGE:
			{ 
				return CreateStringVariableAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CREATE_VECTOR_2_VARIABLE_MESSAGE:
			{ 
				return CreateVector2VariableAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CREATE_VECTOR_3_VARIABLE_MESSAGE:
			{ 
				return CreateVector3VariableAction::_LoadFromFile(_file);
			}

			case AbstractVariable::CREATE_QUATERNION_VARIABLE_MESSAGE:
			{ 
				return CreateQuaternionVariableAction::_LoadFromFile(_file);
			}

			case ColorableObject::CHANGE_BACKGROUND_COLOR_MESSAGE:
			{
				return ChangeBackgroundColorAction::_LoadFromFile(_file);
			}

			case ColorableObject::GET_COLOR_RGB_MESSAGE: 
			{
				return GetColorRGBAction::_LoadFromFile(_file);
			}

			case ColorableObject::GET_COLOR_HSL_MESSAGE: 
			{
				return GetColorHSLAction::_LoadFromFile(_file);
			}

			case ColorableObject::GET_ALPHA_MESSAGE: 
			{
				return GetAlphaAction::_LoadFromFile(_file);
			}

			case ColorableObject::GET_RED_MESSAGE: 
			{
				return GetRedAction::_LoadFromFile(_file);
			}

			case ColorableObject::GET_GREEN_MESSAGE: 
			{
				return GetGreenAction::_LoadFromFile(_file);
			}

			case ColorableObject::GET_BLUE_MESSAGE: 
			{
				return GetBlueAction::_LoadFromFile(_file);
			}
			
			case ColorableObject::GET_HUE_MESSAGE: 
			{
				return GetHueAction::_LoadFromFile(_file);
			}

			case ColorableObject::GET_SATURATION_MESSAGE: 
			{
				return GetSaturationAction::_LoadFromFile(_file);
			}

			case ColorableObject::GET_LIGHTNESS_MESSAGE: 
			{
				return GetLightnessAction::_LoadFromFile(_file);
			}
			
			case ColorableObject::CHANGE_COLOR_RGB_MESSAGE:
			{
				return ChangeColorRGBAction::_LoadFromFile(_file);
			}

			case ColorableObject::CHANGE_COLOR_HSL_MESSAGE:
			{
				return ChangeColorHSLAction::_LoadFromFile(_file);
			}

			case ColorableObject::CHANGE_ALPHA_MESSAGE:
			{
				return ChangeAlphaAction::_LoadFromFile(_file);
			}
			
			case ColorableObject::CHANGE_RED_MESSAGE:
			{
				return ChangeRedAction::_LoadFromFile(_file);
			}

			case ColorableObject::CHANGE_GREEN_MESSAGE:
			{
				return ChangeGreenAction::_LoadFromFile(_file);
			}

			case ColorableObject::CHANGE_BLUE_MESSAGE:
			{
				return ChangeBlueAction::_LoadFromFile(_file);
			}

			case ColorableObject::CHANGE_HUE_MESSAGE:
			{
				return ChangeHueAction::_LoadFromFile(_file);
			}

			case ColorableObject::CHANGE_SATURATION_MESSAGE:
			{
				return ChangeSaturationAction::_LoadFromFile(_file);
			}

			case ColorableObject::CHANGE_LIGHTNESS_MESSAGE:
			{
				return ChangeLightnessAction::_LoadFromFile(_file);
			}
			
			case ScalableObject::GET_SCALE_MESSAGE: 
			{
				return GetScaleAction::_LoadFromFile(_file);
			}

			case ScalableObject::DISPLACE_SCALE_MESSAGE: 
			{
				return DisplaceScaleAction::_LoadFromFile(_file);
			}

			case ScalableObject::REPLACE_SCALE_MESSAGE:
			{
				return ReplaceScaleAction::_LoadFromFile(_file);
			}

			case TranslatableObject::GET_POSITION_MESSAGE: 
			{
				return GetPositionAction::_LoadFromFile(_file);
			}

			case TranslatableObject::MOVE_AROUND_PIVOT_MESSAGE:
			{
				return MoveAroundPivotAction::_LoadFromFile(_file);
			}

			case RotatableObject::ROTATE_AROUND_PIVOT_MESSAGE:
			{
				return RotateAroundPivotAction::_LoadFromFile(_file);
			}

			case RotatableObject::GET_ROTATION_MESSAGE:
			{
				return GetRotationAction::_LoadFromFile(_file);
			}

			case TranslatableObject::DISPLACE_POSITION_MESSAGE:
			{
				return DisplacePositionAction::_LoadFromFile(_file);
			}

			case TranslatableObject::REPLACE_POSITION_MESSAGE:
			{
				return ReplacePositionAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::GET_TILE_CENTER_MESSAGE:
			{
				return GetTileCenterAction::_LoadFromFile(_file);
			}

			case RotatableObject::REPLACE_ROTATION_MESSAGE:
			{
				return ReplaceRotationAction::_LoadFromFile(_file);
			}

			case RotatableObject::DISPLACE_ROTATION_MESSAGE: 
			{
				return DisplaceRotationAction::_LoadFromFile(_file);
			}

			case Plane::CHANGE_PLANE_NORMAL_MESSAGE:
			{
				return ChangePlaneNormalAction::_LoadFromFile(_file);
			}

			case Camera3D::CHANGE_VIEWING_POINT_MESSAGE:
			{
				return ChangeViewingPointAction::_LoadFromFile(_file);
			}

			case Camera3D::CAMERA_LOOK_AT_MESSAGE:
			{
				return CameraLookAtAction::_LoadFromFile(_file);
			}

			case PolygonalSurface::CHANGE_MESH_MESSAGE:
			{
				return ChangeMeshAction::_LoadFromFile(_file);
			}

			case PolygonalSurface::CHANGE_HITBOX_MESSAGE:
			{
				return ChangeHitboxAction::_LoadFromFile(_file);
			}
			
			case Mesh::CREATE_QUAD_MESSAGE:
			{
				return CreateQuadAction::_LoadFromFile(_file);
			}

			case Mesh::CREATE_BOX_MESSAGE:
			{
				return CreateBoxAction::_LoadFromFile(_file);
			}

			case AbstractSurface::CHANGE_INTERSECTION_MODE_MESSAGE:
			{
				return ChangeIntersectionModeAction::_LoadFromFile(_file);
			}

			case GraphicsSurface::ENABLE_BACK_FACE_CULLING_MESSAGE:
			{
				return EnableBackFaceCullingAction::_LoadFromFile(_file);
			}

			case TexturedAnimation::ENABLE_POSITION_OFFSET_MESSAGE: 
			{
				return EnablePositionOffsetAction::_LoadFromFile(_file);
			}

			case LogicObject::ENABLE_FREEZE_MESSAGE:
			{
				return EnableFreezeAction::_LoadFromFile(_file);
			}

			case GraphicsObject::ENABLE_SHOW_MESSAGE:
			{
				return EnableShowAction::_LoadFromFile(_file);
			}

			case LogicObject::CHANGE_DELTA_SCALE_MESSAGE:
			{
				return ChangeDeltaScaleAction::_LoadFromFile(_file);
			}

			case TexturingSurface::CHANGE_FRONT_TEXTURE_MESSAGE:
			{
				return ChangeFrontTextureAction::_LoadFromFile(_file);
			}

			case TexturingSurface::CHANGE_BACK_TEXTURE_MESSAGE:
			{
				return ChangeBackTextureAction::_LoadFromFile(_file);
			}
						
			case TexturingSurface::CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE:
			{
				return ChangeFrontTexturedAnimationAction::_LoadFromFile(_file);
			}

			case TexturingSurface::CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE:
			{
				return ChangeBackTexturedAnimationAction::_LoadFromFile(_file);
			}

			case Mouse::ENABLE_SHOW_CURSOR_MESSAGE:
			{
				return EnableShowCursorAction::_LoadFromFile(_file);
			}

			case Renderer::ENABLE_Z_BUFFER_MESSAGE:
			{
				return EnableZBufferAction::_LoadFromFile(_file);
			}
			
			case Trigger::ENABLE_TRIGGERS_BLOCK_MESSAGE:
			{
				return EnableTriggersBlockAction::_LoadFromFile(_file);
			}

			case Keyboard::CREATE_KEYBOARD_EVENT_MESSAGE:
			{
				return CreateKeyboardEventAction::_LoadFromFile(_file);
			}

			case Scene::PICK_SCENE_MESSAGE:
			{
				return PickSceneAction::_LoadFromFile(_file);
			}
			
			case ObjectScene::ADD_TO_LAYER_MESSAGE:
			{
				return AddToLayerAction::_LoadFromFile(_file);
			}

			case ObjectScene::REMOVE_FROM_LAYER_MESSAGE:
			{
				return RemoveFromLayerAction::_LoadFromFile(_file);
			}

			case LogicScene::ADD_TO_UPDATE_LIST_MESSAGE:
			{
				return AddToUpdateListAction::_LoadFromFile(_file);
			}

			case LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE:
			{
				return RemoveFromUpdateListAction::_LoadFromFile(_file);
			}

			case GraphicsScene::ADD_TO_RENDER_LIST_MESSAGE:
			{
				return AddToRenderListAction::_LoadFromFile(_file);
			}

			case GraphicsScene::REMOVE_FROM_RENDER_LIST_MESSAGE:
			{
				return RemoveFromRenderListAction::_LoadFromFile(_file);
			}

			case GraphicsContainer::CHANGE_CAMERA_MESSAGE:
			{
				return ChangeCameraAction::_LoadFromFile(_file);
			}
						
			case GraphicsContainer::CHANGE_RENDER_TARGET_MESSAGE:
			{
				return ChangeRenderTargetAction::_LoadFromFile(_file);
			}

			case Camera3D::GET_SELECT_RAY_MESSAGE: 
			{
				return GetSelectRayAction::_LoadFromFile(_file);
			}

			case TexturingSurface::ENABLE_TEXTURE_FLIP_MESSAGE:
			{
				return EnableTextureFlipAction::_LoadFromFile(_file);
			}

			case FloatLerp::CHANGE_FLOAT_LERP_T_MESSAGE:
			{
				return ChangeFloatLerpTAction::_LoadFromFile(_file);
			}

			case FloatLerp::GET_FLOAT_LERP_T_MESSAGE:
			{
				return GetFloatLerpTAction::_LoadFromFile(_file);
			}

			case FloatLerp::GET_FLOAT_LERP_CURRENT_MESSAGE:
			{
				return GetFloatLerpCurrentAction::_LoadFromFile(_file);
			}

			case FloatLerp::GET_FLOAT_LERP_DELTA_MESSAGE:
			{
				return GetFloatLerpDeltaAction::_LoadFromFile(_file);
			}

			case Variable<Vector3>::GET_VECTOR_3_ANGLE_MESSAGE:
			{
				return GetVector3AngleAction::_LoadFromFile(_file);
			}

			case Variable<Vector2>::GET_VECTOR_2_ANGLE_MESSAGE:
			{
				return GetVector2AngleAction::_LoadFromFile(_file);
			}
			
			case Variable<Vector3>::GET_VECTOR_3_LENGTH_MESSAGE:
			{
				return GetVector3LengthAction::_LoadFromFile(_file);
			}

			case Variable<Vector2>::GET_VECTOR_2_LENGTH_MESSAGE:
			{
				return GetVector2LengthAction::_LoadFromFile(_file);
			}
			
			case Line::GET_LINE_DIRECTION_MESSAGE:
			{
				return GetLineDirectionAction::_LoadFromFile(_file);
			}

			case Line::GET_LINE_ANGLE_Z_MESSAGE:
			{
				return GetLineAngleZAction::_LoadFromFile(_file);
			}
			
			case Circle::GET_CIRCLE_LENGTH_MESSAGE:
			{ 
				return GetCircleLengthAction::_LoadFromFile(_file);
			}

			case Line::GET_LINE_LENGTH_MESSAGE:
			{ 
				return GetLineLengthAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::GET_DATA_FROM_CONTROL_QUATERNION_MESSAGE:
			{ 
				return GetDataFromControlQuaternionAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::GET_DATA_FROM_CONTROL_ROTATABLE_MESSAGE:
			{ 
				return GetDataFromControlRotatableAction::_LoadFromFile(_file);
			}

			case VectorLerp::GET_DATA_FROM_CONTROL_VECTOR_MESSAGE:
			{ 
				return GetDataFromControlVectorAction::_LoadFromFile(_file);
			}

			case VectorLerp::GET_DATA_FROM_CONTROL_TRANSLATABLE_MESSAGE:
			{ 
				return GetDataFromControlTranslatableAction::_LoadFromFile(_file);
			}
			
			case VectorLerp::GET_DATA_FROM_CONTROL_SCALABLE_MESSAGE:
			{ 
				return GetDataFromControlScalableAction::_LoadFromFile(_file);
			}

			case VectorLerp::GET_DATA_FROM_CONTROL_COLORABLE_MESSAGE:
			{ 
				return GetDataFromControlColorableAction::_LoadFromFile(_file);
			}

			case RotatableObject::CREATE_ROTATION_MESSAGE:
			{
				return CreateRotationAction::_LoadFromFile(_file);
			}

			case Camera3D::CHANGE_PROJECTION_MODE_MESSAGE: 
			{
				return ChangeProjectionModeAction::_LoadFromFile(_file);
			}

			case Camera3D::CHANGE_MOVEMENT_MODE_MESSAGE:
			{
				return ChangeMovementModeAction::_LoadFromFile(_file);
			}

			case Camera3D::CHANGE_ROTATION_MODE_MESSAGE:
			{
				return ChangeRotationModeAction::_LoadFromFile(_file);
			}

			case Camera3D::PICK_CAMERA_MESSAGE:
			{
				return PickCameraAction::_LoadFromFile(_file);
			}

			case FloatLerp::CHANGE_FLOAT_LERP_MODE_MESSAGE:
			{
				return ChangeFloatLerpModeAction::_LoadFromFile(_file);
			}

			case FloatLerp::CHANGE_FLOAT_LERP_SPEED_MESSAGE:
			{
				return ChangeFloatLerpSpeedAction::_LoadFromFile(_file);
			}

			case FloatLerp::CREATE_FLOAT_LERP_MESSAGE:
			{
				return CreateFloatLerpAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::CREATE_TILED_SPACE_MESSAGE:
			{
				return CreateTiledSpaceAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::CHANGE_TILED_SPACE_SIZE_MESSAGE:
			{
				return ChangeTiledSpaceSizeAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::CREATE_SOLID_TILE_MESSAGE:
			{
				return CreateSolidTileAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::CREATE_EMPTY_TILE_MESSAGE:
			{
				return CreateEmptyTileAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::CREATE_BRUSH_MESSAGE:
			{
				return CreateBrushAction::_LoadFromFile(_file);
			}

			case TiledSpace3D::REMOVE_BRUSH_MESSAGE:
			{
				return RemoveBrushAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::CREATE_QUATERNION_LERP_MESSAGE:
			{
				return CreateQuaternionLerpAction::_LoadFromFile(_file);
			}

			case VectorLerp::CREATE_VECTOR_LERP_MESSAGE:
			{
				return CreateVectorLerpAction::_LoadFromFile(_file);
			}

			case FloatLerp::STREAMING_FLOAT_T_MESSAGE:
			{
				return StreamingFloatTAction::_LoadFromFile(_file);
			}
			
			case FloatLerp::STREAMING_FLOAT_T_ANIMATION_FRAME_MESSAGE:
			{
				return StreamingFloatTAnimationFrameAction::_LoadFromFile(_file);
			}

			case FloatLerp::STREAMING_FLOAT_T_RED_MESSAGE:
			{
				return StreamingFloatTRedAction::_LoadFromFile(_file);
			}

			case FloatLerp::STREAMING_FLOAT_T_GREEN_MESSAGE:
			{
				return StreamingFloatTGreenAction::_LoadFromFile(_file);
			}

			case FloatLerp::STREAMING_FLOAT_T_BLUE_MESSAGE:
			{
				return StreamingFloatTBlueAction::_LoadFromFile(_file);
			}

			case FloatLerp::STREAMING_FLOAT_T_HUE_MESSAGE:
			{
				return StreamingFloatTHueAction::_LoadFromFile(_file);
			}

			case FloatLerp::STREAMING_FLOAT_T_SATURATION_MESSAGE:
			{
				return StreamingFloatTSaturationAction::_LoadFromFile(_file);
			}

			case FloatLerp::STREAMING_FLOAT_T_LIGHTNESS_MESSAGE:
			{
				return StreamingFloatTLightnessAction::_LoadFromFile(_file);
			}
			
			case FloatLerp::STREAMING_FLOAT_T_ALPHA_MESSAGE:
			{
				return StreamingFloatTAlphaAction::_LoadFromFile(_file);
			}

			case FloatLerp::STREAMING_FLOAT_T_DELTA_SCALE_MESSAGE:
			{
				return StreamingFloatTDeltaScaleAction::_LoadFromFile(_file);
			}

			case FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE:
			{
				return StreamingFloatCurrentAction::_LoadFromFile(_file);
			}

			case FloatLerp::CHANGE_FLOAT_LERP_FLOAT_TARGET_MESSAGE:
			{
				return ChangeFloatLerpFloatTargetAction::_LoadFromFile(_file);
			}
	
			case FloatLerp::CHANGE_FLOAT_LERP_INTEGER_TARGET_MESSAGE:
			{
				return ChangeFloatLerpIntegerTargetAction::_LoadFromFile(_file);
			}

			case FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE:
			{
				return StreamingFloatDeltaAction::_LoadFromFile(_file);
			}

			case VectorLerp::CHANGE_VECTOR_LERP_MODE_MESSAGE:
			{
				return ChangeVectorLerpModeAction::_LoadFromFile(_file);
			}

			case LerpingObject::CHANGE_FLOAT_LERP_MESSAGE:
			{
				return ChangeFloatLerpAction::_LoadFromFile(_file);
			}

			case PlayingObject::CHANGE_PLAYING_MODE_MESSAGE:
			{
				return ChangePlayingModeAction::_LoadFromFile(_file);
			}

			case PlayingObject::CHANGE_PLAYING_STATE_MESSAGE:
			{
				return ChangePlayingStateAction::_LoadFromFile(_file);
			}

			case PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE:
			{
				return ChangePlayingDurationInSecAction::_LoadFromFile(_file);
			}

			case Camera3D::CHANGE_DISTANCE_TO_VIEWING_POINT_MESSAGE:
			{
				return ChangeDistanceToViewingPointAction::_LoadFromFile(_file);
			}
			
			case QuaternionLerp::STREAMING_QUATERNION_CURRENT_MESSAGE:
			{
				return StreamingQuaternionCurrentAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::STREAMING_ROTATABLE_CURRENT_MESSAGE:
			{
				return StreamingRotatableCurrentAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE:
			{
				return StreamingQuaternionDeltaAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::STREAMING_ROTATABLE_DELTA_MESSAGE:
			{
				return StreamingRotatableDeltaAction::_LoadFromFile(_file);
			}

			case VectorLerp::STREAMING_COLORABLE_CURRENT_MESSAGE:
			{
				return StreamingColorableCurrentAction::_LoadFromFile(_file);
			}

			case VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE:
			{
				return StreamingVectorDeltaAction::_LoadFromFile(_file);
			}

			case VectorLerp::STREAMING_TRANSLATABLE_DELTA_MESSAGE:
			{
				return StreamingTranslatableDeltaAction::_LoadFromFile(_file);
			}

			case VectorLerp::STREAMING_SCALABLE_DELTA_MESSAGE:
			{
				return StreamingScalableDeltaAction::_LoadFromFile(_file);
			}

			case VectorLerp::STREAMING_COLORABLE_DELTA_MESSAGE:
			{
				return StreamingColorableDeltaAction::_LoadFromFile(_file);
			}

			case VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE:
			{
				return StreamingVectorCurrentAction::_LoadFromFile(_file);
			}

			case VectorLerp::STREAMING_TRANSLATABLE_CURRENT_MESSAGE:
			{
				return StreamingTranslatableCurrentAction::_LoadFromFile(_file);
			}

			case VectorLerp::STREAMING_SCALABLE_CURRENT_MESSAGE:
			{
				return StreamingScalableCurrentAction::_LoadFromFile(_file);
			}

			case TexturingSurface::CHECK_LINE_IN_IMAGE_MESSAGE:
			{
				return CheckLineInImageAction::_LoadFromFile(_file);
			}

			case Camera3D::CHECK_FRUSTUM_IN_QUAD_MESSAGE: 
			{
				return CheckFrustumInQuadAction::_LoadFromFile(_file);
			}

			case PolygonalSurface::CHECK_LINE_IN_POLYGON_MESSAGE:
			{
				return CheckLineInPolygonAction::_LoadFromFile(_file);
			}

			case AbstractSurface::CHECK_LINE_IN_SURFACE_MESSAGE:
			{
				return CheckLineInSurfaceAction::_LoadFromFile(_file);
			}

			case VectorLerp::SET_TWO_VECTORS_MESSAGE:
			{
				return SetTwoVectorsAction::_LoadFromFile(_file);
			}

			case VectorLerp::SET_TWO_TRANSLATABLES_MESSAGE: 
			{
				return SetTwoTranslatablesAction::_LoadFromFile(_file);
			}

			case VectorLerp::SET_TWO_SCALABLES_MESSAGE:
			{
				return SetTwoScalablesAction::_LoadFromFile(_file);
			}

			case VectorLerp::SET_TWO_COLORABLES_MESSAGE:
			{
				return SetTwoColorablesAction::_LoadFromFile(_file);
			}

			case VectorLerp::CHANGE_CONTROL_VECTORS_MESSAGE:
			{
				return ChangeControlVectorsAction::_LoadFromFile(_file);
			}

			case VectorLerp::CHANGE_CONTROL_TRANSLATABLES_MESSAGE:
			{
				return ChangeControlTranslatablesAction::_LoadFromFile(_file);
			}

			case VectorLerp::CHANGE_CONTROL_SCALABLES_MESSAGE:
			{
				return ChangeControlScalablesAction::_LoadFromFile(_file);
			}

			case VectorLerp::CHANGE_CONTROL_COLORABLES_MESSAGE:
			{
				return ChangeControlColorablesAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::CHANGE_CONTROL_QUATERNIONS_MESSAGE:
			{
				return ChangeControlQuaternionsAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::CHANGE_CONTROL_ROTATABLES_MESSAGE:
			{
				return ChangeControlRotatablesAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::SET_TWO_QUATERNIONS_MESSAGE: 
			{
				return SetTwoQuaternionsAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::SET_TWO_ROTATABLES_MESSAGE: 
			{
				return SetTwoRotatablesAction::_LoadFromFile(_file);
			}
			
			case VectorLerp::SET_DATA_TO_CONTROL_VECTOR_MESSAGE:
			{ 
				return SetDataToControlVectorAction::_LoadFromFile(_file);
			}

			case VectorLerp::SET_DATA_TO_CONTROL_TRANSLATABLE_MESSAGE:
			{ 
				return SetDataToControlTranslatableAction::_LoadFromFile(_file);
			}
			
			case VectorLerp::SET_DATA_TO_CONTROL_SCALABLE_MESSAGE:
			{ 
				return SetDataToControlScalableAction::_LoadFromFile(_file);
			}

			case VectorLerp::SET_DATA_TO_CONTROL_COLORABLE_MESSAGE:
			{ 
				return SetDataToControlColorableAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::SET_DATA_TO_CONTROL_QUATERNION_MESSAGE:
			{ 
				return SetDataToControlQuaternionAction::_LoadFromFile(_file);
			}

			case QuaternionLerp::SET_DATA_TO_CONTROL_ROTATABLE_MESSAGE:
			{ 
				return SetDataToControlRotatableAction::_LoadFromFile(_file);
			}
		}
	} 
	return NIL;
}



bool AbstractAction::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}

				
bool AbstractAction::LoadFromFile(File& _file)
{
	return false;
}

