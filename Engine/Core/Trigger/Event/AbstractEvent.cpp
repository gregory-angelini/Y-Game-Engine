#include "AbstractEvent.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/Graphics/Surface/TexturingSurface/TexturingSurface.h>
#include <Engine/Output/Graphics/Scene/GraphicsScene/GraphicsScene.h>
#include <Engine/Core/Scene/LogicScene/LogicScene.h>
#include <Engine/Core/Time/Timer/Timer.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Core/Trigger/Event/Events.h>
StringANSI AbstractEvent::eventsSuffix = "event";
StringANSI AbstractEvent::eventsDir = "triggers/events/";



AbstractEvent::AbstractEvent(void): type(0), loadArgsEnable(true), argsEnable(true)
{
	for(int32 i = 0; i < WIDTH; i++)
	{
		for(int32 j = 0; j < HEIGHT; j++)
		{
			state[i][j] = FALSE;
		}

	}

	AddClassProperty(EVENT_CLASS);
}


AbstractEvent::AbstractEvent(int32 _type): type(_type), loadArgsEnable(true), argsEnable(true)
{
	for(int32 i = 0; i < WIDTH; i++)
	{
		for(int32 j = 0; j < HEIGHT; j++)
		{
			state[i][j] = FALSE;
		}
	}

	AddClassProperty(EVENT_CLASS);
}



AbstractEvent::~AbstractEvent(void)
{
	Destroying();
}




void AbstractEvent::SetArgsEnable(bool _enable)
{
	argsEnable = _enable;
}



void AbstractEvent::SetArgMode(int32 _index, int32 _mode)
{
	if(_index >= 0 && _index < HEIGHT)
	{
		if(_mode >= 0 && _mode < WIDTH)
		{
			state[TEMPLATE][_index] = FALSE;
			state[EXPRESSION][_index] = FALSE;
			state[VARIABLE][_index] = FALSE;
			state[KEY][_index] = FALSE;
			state[EQUALITY][_index] = FALSE;

			state[_mode][_index] = TRUE;
		}
	}
}

bool AbstractEvent::IsArgsEnabled(void)const
{
	return argsEnable;
}


void AbstractEvent::SetArgAssetType(int32 _index, int32 _type)
{
	if(_index >= 0 && _index < HEIGHT)
	{
		state[ASSET_TYPE][_index] = _type;
	}
}



int32 AbstractEvent::GetArgState(int32 _property, int32 _index)
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



void AbstractEvent::SetExpressionArg(int32 _index, StringANSI _expr)
{}



void AbstractEvent::SetVariableArg(int32 _index, StringANSI _name)
{}



void AbstractEvent::SetTemplateArg(int32 _index, StringANSI _expr)
{}




void AbstractEvent::SetStringKeyArg(int32 _index, StringANSI _value)
{}


void AbstractEvent::SetIntegerKeyArg(int32 _index, int32 _value)
{}


void AbstractEvent::SetFloatKeyArg(int32 _index, float _value)
{}


void AbstractEvent::SetBooleanKeyArg(int32 _index, bool _value)
{}


void AbstractEvent::SetVector2KeyArg(int32 _index, Vector2 _value)
{}


void AbstractEvent::SetVector3KeyArg(int32 _index, Vector3 _value)
{}



void AbstractEvent::SetQuaternionKeyArg(int32 _index, Quaternion _value)
{}



void AbstractEvent::SetEqualityArg(int32 _index, StringANSI _expr)
{}



StringANSI AbstractEvent::GetExpressionArg(int32 _index)
{
	return "";
}


StringANSI AbstractEvent::GetEqualityArg(int32 _index)
{
	return "";
}



StringANSI AbstractEvent::GetTemplateArg(int32 _index)
{
	return "";
}



StringANSI AbstractEvent::GetVariableArg(int32 _index)
{
	return "";
}




StringANSI AbstractEvent::GetStringKeyArg(int32 _index)
{
	return "";
}


int32 AbstractEvent::GetIntegerKeyArg(int32 _index)
{
	return -1;
}


float AbstractEvent::GetFloatKeyArg(int32 _index)
{
	return -1.0f;
}


bool AbstractEvent::GetBooleanKeyArg(int32 _index)
{
	return false;
}



Vector2 AbstractEvent::GetVector2KeyArg(int32 _index)
{
	return Vector2();
}


Vector3 AbstractEvent::GetVector3KeyArg(int32 _index)
{
	return Vector3();
}


Quaternion AbstractEvent::GetQuaternionKeyArg(int32 _index)
{
	return Quaternion();
}



bool AbstractEvent::IsLoadArgsEnabled(void)const
{
	return loadArgsEnable;
}


void AbstractEvent::SetLoadArgsEnable(bool _enable)
{
	loadArgsEnable = _enable;
}


int32 AbstractEvent::GetType(void)const
{
	return type;
}



void AbstractEvent::SetValue(bool _value)
{
	if(_value)
	{
		if(activationLimit == 0) { return; }
		else if(activationLimit > 0)
		{
			activationLimit--;
		}

		if(Trigger::_IsEventLogEnabled())
		{
			Trigger::_GetEventLogList().AddToTail(Trigger::EventInfo(GetName(), Trigger::EventInfo::EVENT_CATEGORY, type, Renderer::_GetPicked()->GetWorkingTimeInMs()));
		}

		Variable::SetValue(_value);
	}
	else
	{
		if(!_value && !value) { return; }
		Variable::SetValue(_value);
	}
}


StringANSI AbstractEvent::_GetStringType(int32 _type)
{
	switch(_type)
	{
		case AbstractObject::SEND_MESSAGE:
		{
			return "SEND_MESSAGE";
		}

		case AbstractObject::CHANGE_NAME_MESSAGE:
		{
			return "CHANGE_NAME";
		}

		case AbstractObject::ENABLE_PROPERTY_MESSAGE:
		{
			return "ENABLE_PROPERTY";
		}

		case AbstractObject::DESTROY_OBJECT_MESSAGE:
		{
			return "DESTROY_OBJECT";
		}

		case AssetLibrary::CREATE_OBJECT_MESSAGE:
		{
			return "CREATE_OBJECT";
		}

		case PlayingObject::CHANGE_PLAYING_STATE_MESSAGE:
		{
			return "CHANGE_PLAYING_STATE";
		}

		case PlayingObject::CHANGE_PLAYING_MODE_MESSAGE:
		{
			return "CHANGE_PLAYING_MODE";
		}

		case Timer::TIMER_FINISH_MESSAGE:
		{
			return "TIMER_FINISH";
		}

		case PlayerControl::PLAYER_CONTROL_FINISH_MESSAGE:
		{
			return "PLAYER_CONTROL_FINISH";
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

		case ObjectScene::ADD_TO_LAYER_MESSAGE:
		{
			return "ADD_TO_LAYER";
		}

		case ObjectScene::REMOVE_FROM_LAYER_MESSAGE:
		{
			return "REMOVE_FROM_LAYER";
		}

		case Scene::PICK_SCENE_MESSAGE:
		{
			return "PICK_SCENE";
		}

		case Trigger::TRIGGER_FINISH_MESSAGE:
		{
			return "TRIGGER_FINISH";
		}

		case AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE:
		{
			return "CHANGE_ANIMATION_FRAME";
		}

		case TexturedAnimation::CHANGE_ATLAS_MESSAGE:
		{
			return "CHANGE_ATLAS";
		}

		case TexturedAnimation::ENABLE_POSITION_OFFSET_MESSAGE:
		{
			return "ENABLE_POSITION_OFFSET";
		} 

		case LerpingObject::CHANGE_FLOAT_LERP_MESSAGE:
		{
			return "CHANGE_FLOAT_LERP";
		}

		case FloatLerp::FLOAT_LERP_FINISH_MESSAGE:
		{
			return "FLOAT_LERP_FINISH";
		}

		case FloatLerp::FLOAT_LERP_START_MESSAGE:
		{
			return "FLOAT_LERP_START";
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
	
		case PolygonalSurface::CHANGE_MESH_MESSAGE:
		{
			return "CHANGE_MESH";
		}

		case PolygonalSurface::CHANGE_HITBOX_MESSAGE:
		{
			return "CHANGE_HITBOX";
		}

		case PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE:
		{
			return "COLLISION_BETWEEN_OBJECTS";
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

		case Camera3D::PICK_CAMERA_MESSAGE:
		{
			return "PICK_CAMERA";
		}
	}
	return "";
}



StringANSI AbstractEvent::_GetDir(void)
{
	return eventsDir;
}



void AbstractEvent::_SetFileSuffix(StringANSI _suffix)
{
	eventsSuffix = _suffix;
}


StringANSI AbstractEvent::_GetFileSuffix(void)
{
	return eventsSuffix;
}




AbstractEvent* AbstractEvent::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + eventsDir + _path + "." + eventsSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


AbstractEvent* AbstractEvent::_LoadFromFile(File& _file) 
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		int32 type;
		_file.Read(type);

		switch(type)
		{
			case AbstractObject::CHANGE_NAME_MESSAGE:
			{
				return ChangeNameEvent::_LoadFromFile(_file);
			}

			case AbstractObject::ENABLE_PROPERTY_MESSAGE:
			{
				return EnablePropertyEvent::_LoadFromFile(_file);
			}

			case AbstractObject::DESTROY_OBJECT_MESSAGE:
			{
				return DestroyObjectEvent::_LoadFromFile(_file);
			}

			case PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE:
			{
				return CollisionBetweenObjectsEvent::_LoadFromFile(_file);
			}

			case AssetLibrary::CREATE_OBJECT_MESSAGE:
			{
				return CreateObjectEvent::_LoadFromFile(_file);
			}

			case MessageHandler<int32>::SEND_MESSAGE:
			{
				return SendMessageEvent::_LoadFromFile(_file);
			}

			case Timer::TIMER_FINISH_MESSAGE:
			{
				return TimerFinishEvent::_LoadFromFile(_file);
			}

			case FloatLerp::FLOAT_LERP_FINISH_MESSAGE:
			{
				return FloatLerpFinishEvent::_LoadFromFile(_file);
			}

			case FloatLerp::FLOAT_LERP_START_MESSAGE:
			{
				return FloatLerpStartEvent::_LoadFromFile(_file);
			}

			case Scene::PICK_SCENE_MESSAGE:
			{
				return PickSceneEvent::_LoadFromFile(_file);
			}

			case ObjectScene::ADD_TO_LAYER_MESSAGE:
			{
				return AddToLayerEvent::_LoadFromFile(_file);
			}
				
			case ObjectScene::REMOVE_FROM_LAYER_MESSAGE:
			{
				return RemoveFromLayerEvent::_LoadFromFile(_file);
			}

			case LogicScene::ADD_TO_UPDATE_LIST_MESSAGE:
			{
				return AddToUpdateListEvent::_LoadFromFile(_file);
			}

			case LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE:
			{
				return RemoveFromUpdateListEvent::_LoadFromFile(_file);
			}

			case GraphicsScene::ADD_TO_RENDER_LIST_MESSAGE:
			{
				return AddToRenderListEvent::_LoadFromFile(_file);
			}

			case GraphicsScene::REMOVE_FROM_RENDER_LIST_MESSAGE:
			{
				return RemoveFromRenderListEvent::_LoadFromFile(_file);
			}

			case GraphicsContainer::CHANGE_CAMERA_MESSAGE:
			{
				return ChangeCameraEvent::_LoadFromFile(_file);
			}

			case TexturedAnimation::ENABLE_POSITION_OFFSET_MESSAGE:
			{
				return EnablePositionOffsetEvent::_LoadFromFile(_file);
			}

			case TexturingSurface::CHANGE_BACK_TEXTURE_MESSAGE:
			{
				return ChangeBackTextureEvent::_LoadFromFile(_file);
			}

			case TexturingSurface::CHANGE_FRONT_TEXTURE_MESSAGE:
			{
				return ChangeFrontTextureEvent::_LoadFromFile(_file);
			}

			case TexturingSurface::CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE:
			{
				return ChangeFrontTexturedAnimationEvent::_LoadFromFile(_file);
			}

			case TexturingSurface::CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE:
			{
				return ChangeBackTexturedAnimationEvent::_LoadFromFile(_file);
			}

			case AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE:
			{
				return ChangeAnimationFrameEvent::_LoadFromFile(_file);
			}

			case TexturedAnimation::CHANGE_ATLAS_MESSAGE:
			{
				return ChangeAtlasEvent::_LoadFromFile(_file);
			}

			case PlayerControl::PLAYER_CONTROL_FINISH_MESSAGE:
			{
				return PlayerControlFinishEvent::_LoadFromFile(_file);
			}

			case Trigger::TRIGGER_FINISH_MESSAGE:
			{
				return TriggerFinishEvent::_LoadFromFile(_file);
			}

			case Camera3D::PICK_CAMERA_MESSAGE:
			{
				return PickCameraEvent::_LoadFromFile(_file);
			}

			case LerpingObject::CHANGE_FLOAT_LERP_MESSAGE:
			{
				return ChangeFloatLerpEvent::_LoadFromFile(_file);
			}

			case PolygonalSurface::CHANGE_MESH_MESSAGE:
			{
				return ChangeMeshEvent::_LoadFromFile(_file);
			}

			case PolygonalSurface::CHANGE_HITBOX_MESSAGE:
			{
				return ChangeHitboxEvent::_LoadFromFile(_file);
			}
			
			case PlayingObject::CHANGE_PLAYING_STATE_MESSAGE:
			{
				return ChangePlayingStateEvent::_LoadFromFile(_file);
			}

			case PlayingObject::CHANGE_PLAYING_MODE_MESSAGE:
			{
				return ChangePlayingModeEvent::_LoadFromFile(_file);
			}

			case Variable<int32>::CHANGE_INTEGER_VALUE_MESSAGE:
			{
				return ChangeIntegerValueEvent::_LoadFromFile(_file);
			}

			case Variable<float>::CHANGE_FLOAT_VALUE_MESSAGE:
			{
				return ChangeFloatValueEvent::_LoadFromFile(_file);
			}

			case Variable<bool>::CHANGE_BOOLEAN_VALUE_MESSAGE:
			{
				return ChangeBooleanValueEvent::_LoadFromFile(_file);
			}

			case Variable<StringANSI>::CHANGE_STRING_VALUE_MESSAGE:
			{
				return ChangeStringValueEvent::_LoadFromFile(_file);
			}

			case Variable<Vector2>::CHANGE_VECTOR_2_VALUE_MESSAGE:
			{
				return ChangeVector2ValueEvent::_LoadFromFile(_file);
			}

			case Variable<Vector3>::CHANGE_VECTOR_3_VALUE_MESSAGE:
			{
				return ChangeVector3ValueEvent::_LoadFromFile(_file);
			}

			case Variable<Quaternion>::CHANGE_QUATERNION_VALUE_MESSAGE:
			{
				return ChangeQuaternionValueEvent::_LoadFromFile(_file);
			}
		} 
	}
	return NIL;
}
 

bool AbstractEvent::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}

				
bool AbstractEvent::LoadFromFile(File& _file)
{
	return false;
}


