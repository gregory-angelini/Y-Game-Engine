#include "Trigger.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Event/Events.h>



AbstractEvent* Trigger::_CreateEvent(int32 _type)
{
	switch(_type)
	{
		case AbstractObject::CHANGE_NAME_MESSAGE:
		{
			return new ChangeNameEvent(_type);
		}

		case AbstractObject::ENABLE_PROPERTY_MESSAGE:
		{
			return new EnablePropertyEvent(_type);
		}

		case AbstractObject::DESTROY_OBJECT_MESSAGE:
		{
			return new DestroyObjectEvent(_type);
		}

		case PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE:
		{
			return new CollisionBetweenObjectsEvent(_type);
		}

		case AssetLibrary::CREATE_OBJECT_MESSAGE:
		{ 
			return new CreateObjectEvent(_type);
		}

		case MessageHandler<int32>::SEND_MESSAGE:
		{
			return new SendMessageEvent(_type);
		}

		case Timer::TIMER_FINISH_MESSAGE:
		{
			return new TimerFinishEvent(_type);
		}

		case FloatLerp::FLOAT_LERP_FINISH_MESSAGE:
		{
			return new FloatLerpFinishEvent(_type);
		}

		case FloatLerp::FLOAT_LERP_START_MESSAGE:
		{
			return new FloatLerpStartEvent(_type);
		}

		case Scene::PICK_SCENE_MESSAGE:
		{
			return new PickSceneEvent(_type);
		}

		case ObjectScene::ADD_TO_LAYER_MESSAGE:
		{
			return new AddToLayerEvent(_type);
		}

		case ObjectScene::REMOVE_FROM_LAYER_MESSAGE:
		{
			return new RemoveFromLayerEvent(_type);
		}

		case LogicScene::ADD_TO_UPDATE_LIST_MESSAGE:
		{
			return new AddToUpdateListEvent(_type);
		}

		case LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE:
		{
			return new RemoveFromUpdateListEvent(_type);
		}

		case GraphicsScene::ADD_TO_RENDER_LIST_MESSAGE:
		{
			return new AddToRenderListEvent(_type);
		}

		case GraphicsScene::REMOVE_FROM_RENDER_LIST_MESSAGE:
		{
			return new RemoveFromRenderListEvent(_type);
		}

		case GraphicsContainer::CHANGE_CAMERA_MESSAGE:
		{
			return new ChangeCameraEvent(_type);
		}

		case TexturedAnimation::ENABLE_POSITION_OFFSET_MESSAGE:
		{
			return new EnablePositionOffsetEvent(_type);
		}

		case TexturingSurface::CHANGE_BACK_TEXTURE_MESSAGE:
		{
			return new ChangeBackTextureEvent(_type);
		}

		case TexturingSurface::CHANGE_FRONT_TEXTURE_MESSAGE:
		{
			return new ChangeFrontTextureEvent(_type);
		}

		case TexturingSurface::CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE:
		{
			return new ChangeFrontTexturedAnimationEvent(_type);
		}

		case TexturingSurface::CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE:
		{
			return new ChangeBackTexturedAnimationEvent(_type);
		} 

		case AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE:
		{
			return new ChangeAnimationFrameEvent(_type);
		}

		case TexturedAnimation::CHANGE_ATLAS_MESSAGE:
		{
			return new ChangeAtlasEvent(_type);
		}

		case PlayerControl::PLAYER_CONTROL_FINISH_MESSAGE:
		{
			return new PlayerControlFinishEvent(_type);
		}

		case Trigger::TRIGGER_FINISH_MESSAGE:
		{
			return new TriggerFinishEvent(_type);
		}

		case Camera3D::PICK_CAMERA_MESSAGE:
		{
			return new PickCameraEvent(_type);
		}

		case LerpingObject::CHANGE_FLOAT_LERP_MESSAGE:
		{
			return new ChangeFloatLerpEvent(_type);
		}

		case PolygonalSurface::CHANGE_MESH_MESSAGE:
		{
			return new ChangeMeshEvent(_type);
		}
		
		case PolygonalSurface::CHANGE_HITBOX_MESSAGE:
		{
			return new ChangeHitboxEvent(_type);
		}
		
		case PlayingObject::CHANGE_PLAYING_STATE_MESSAGE:
		{
			return new ChangePlayingStateEvent(_type);
		}

		case PlayingObject::CHANGE_PLAYING_MODE_MESSAGE:
		{
			return new ChangePlayingModeEvent(_type);
		}

		case Variable<int32>::CHANGE_INTEGER_VALUE_MESSAGE:
		{
			return new ChangeIntegerValueEvent(_type);
		}

		case Variable<float>::CHANGE_FLOAT_VALUE_MESSAGE:
		{
			return new ChangeFloatValueEvent(_type);
		}

		case Variable<bool>::CHANGE_BOOLEAN_VALUE_MESSAGE:
		{
			return new ChangeBooleanValueEvent(_type);
		}

		case Variable<StringANSI>::CHANGE_STRING_VALUE_MESSAGE:
		{
			return new ChangeStringValueEvent(_type);
		}

		case Variable<Vector2>::CHANGE_VECTOR_2_VALUE_MESSAGE:
		{
			return new ChangeVector2ValueEvent(_type);
		}

		case Variable<Vector3>::CHANGE_VECTOR_3_VALUE_MESSAGE:
		{
			return new ChangeVector3ValueEvent(_type);
		}

		case Variable<Quaternion>::CHANGE_QUATERNION_VALUE_MESSAGE:
		{
			return new ChangeQuaternionValueEvent(_type);
		}
	}

	return NIL;
}
