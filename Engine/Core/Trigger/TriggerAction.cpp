#include "Trigger.h"
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/Trigger/Action/Actions.h>



AbstractAction* Trigger::_CreateAction(int32 _type)
{
	switch(_type)
	{
		case AbstractVariable::CHECK_EQUALITY_MESSAGE:
		{
			return new CheckEqualityAction(_type);
		}

		case QuaternionLerp::GET_CONTROL_QUATERNIONS_MESSAGE:
		{
			return new GetControlQuaternionsAction(_type);
		}

		case VectorLerp::GET_CONTROL_VECTORS_MESSAGE:
		{
			return new GetControlVectorsAction(_type);
		}

		case Circle::GET_RADIUS_FROM_2_POINTS_MESSAGE:
		{
			return new GetRadiusFrom2PointsAction(_type);
		}

		case Circle::GET_RADIUS_FROM_3_POINTS_MESSAGE:
		{
			return new GetRadiusFrom3PointsAction(_type);
		}

		case QuaternionLerp::GET_QUATERNION_LERP_PATH_MESSAGE:
		{
			return new GetQuaternionLerpPathAction(_type);
		}

		case AbstractList::GET_LIST_LENGTH_MESSAGE:
		{
			return new GetListLengthAction(_type);
		}

		case QuaternionLerp::GET_QUATERNION_LERP_T_MESSAGE:
		{
			return new GetQuaternionLerpTAction(_type);
		} 

		case QuaternionLerp::GET_ROTATABLE_LERP_T_MESSAGE:
		{
			return new GetRotatableLerpTAction(_type);
		} 
		
		case VectorLerp::GET_VECTOR_LERP_T_MESSAGE:
		{
			return new GetVectorLerpTAction(_type);
		} 

		case VectorLerp::GET_TRANSLATABLE_LERP_T_MESSAGE:
		{
			return new GetTranslatableLerpTAction(_type);
		} 

		case VectorLerp::GET_SCALABLE_LERP_T_MESSAGE:
		{
			return new GetScalableLerpTAction(_type);
		}

		case VectorLerp::GET_COLORABLE_LERP_T_MESSAGE:
		{
			return new GetColorableLerpTAction(_type);
		}
		
		case GraphicsStrip::CHANGE_STRIP_COLORABLES_MESSAGE:
		{
			return new ChangeStripColorablesAction(_type);
		} 
		
		case GraphicsStrip::CHANGE_STRIP_SCALABLES_MESSAGE: 
		{
			return new ChangeStripScalablesAction(_type);
		} 
		
		case GraphicsStrip::CHANGE_STRIP_TRANSLATABLES_MESSAGE:
		{
			return new ChangeStripTranslatablesAction(_type);
		} 
	    
		case GraphicsStrip::CHANGE_STRIP_VECTORS_MESSAGE:
		{
			return new ChangeStripVectorsAction(_type);
		} 

		case VectorLerp::GET_VECTOR_LERP_PATH_MESSAGE:
		{
			return new GetVectorLerpPathAction(_type);
		} 

		case Camera3D::GET_VIEWING_POINT_MESSAGE:
		{
			return new GetViewingPointAction(_type);
		} 

		case Camera3D::ENABLE_ZOOM_MESSAGE:
		{
			return new EnableZoomAction(_type);
		} 

		case Camera3D::ENABLE_ROTATION_RANGE_MESSAGE:
		{
			return new EnableRotationRangeAction(_type);
		} 

		case Camera3D::CHANGE_ROTATION_RANGE_MESSAGE:
		{
			return new ChangeRotationRangeAction(_type);
		} 

		case Camera3D::ENABLE_ROTATION_MESSAGE:
		{
			return new EnableRotationAction(_type);
		} 

		case Camera3D::CHANGE_ZOOM_SPEED_MESSAGE:
		{
			return new ChangeZoomSpeedAction(_type);
		} 

		case Camera3D::CHANGE_ZOOM_FAR_MESSAGE:
		{
			return new ChangeZoomFarAction(_type);
		} 

		case Camera3D::CHANGE_ZOOM_NEAR_MESSAGE:
		{
			return new ChangeZoomNearAction(_type);
		} 

		case AbstractObject::CHANGE_NAME_MESSAGE:
		{
			return new ChangeNameAction(_type);
		} 

		case TiledSpace3D::TILED_SPACE_FILL_MESSAGE:
		{
			return new TiledSpaceFillAction(_type); 
		}

		case TiledSpace3D::CHECK_TILE_EXIST_MESSAGE:
		{
			return new CheckTileExistAction(_type); 
		}

		case TiledSpace3D::CHANGE_TILE_PASS_MESSAGE:
		{
			return new ChangeTilePassAction(_type); 
		}

		case TiledSpace3D::CHECK_TILE_PASS_MESSAGE:
		{
			return new CheckTilePassAction(_type); 
		}

		case TiledSpace3D::CHECK_TILE_SOLID_MESSAGE:
		{
			return new CheckTileSolidAction(_type); 
		}

		case TiledSpace3D::SET_PUBLIC_TILE_MESSAGE:
		{
			return new SetPublicTileAction(_type); 
		}

		case TexturingSurface::SET_PUBLIC_TEXTURE_MESSAGE:
		{
			return new SetPublicTextureAction(_type); 
		}

		case AssetLibrary::SET_PRIVATE_ASSET_MESSAGE:
		{
			return new SetPrivateAssetAction(_type); 
		}

		case Texture2D::CREATE_BOX_TEXTURE_MAP_MESSAGE:
		{
			return new CreateBoxTextureMapAction(_type); 
		}

		case Texture2D::CHANGE_BOX_TEXTURE_MAP_MESSAGE:
		{
			return new ChangeBoxTextureMapAction(_type); 
		}

		case Timer::CREATE_TIMER_MESSAGE:
		{
			return new CreateTimerAction(_type); 
		}

		case Texture2D::CREATE_TEXTURE_MESSAGE:
		{
			return new CreateTextureAction(_type); 
		}
		
		case TexturedAnimation::CREATE_TEXTURED_ANIMATION_MESSAGE:
		{
			return new CreateTexturedAnimationAction(_type); 
		}

		case Mesh::CREATE_MESH_MESSAGE:
		{
			return new CreateMeshAction(_type); 
		}

		case Set::GET_DATA_FROM_SLOT_MESSAGE:
		{
			return new GetDataFromSlotAction(_type); 
		}

		case Set::SET_DATA_TO_SLOT_MESSAGE:
		{
			return new SetDataToSlotAction(_type); 
		}

		case Set::RESIZE_SET_MESSAGE:
		{
			return new ResizeSetAction(_type); 
		}

		case Set::GET_AMOUNT_OF_SLOTS_MESSAGE:
		{
			return new GetAmountOfSlotsAction(_type); 
		}

		case Set::CREATE_SET_MESSAGE:
		{
			return new CreateSetAction(_type); 
		}

		case PlayerControl::CHANGE_INPUT_TIMING_MESSAGE:
		{
			return new ChangeInputTimingAction(_type); 
		}

		case PlayingObject::REPLAY_MESSAGE:
		{
			return new ReplayAction(_type); 
		}

		case PlayerControl::SET_ONE_INPUT_EVENT_MESSAGE:
		{
			return new SetOneInputEventAction(_type); 
		}

		case PlayerControl::CREATE_PLAYER_CONTROL_MESSAGE:
		{
			return new CreatePlayerControlAction(_type); 
		}

		case TiledSpace3D::MOVE_TILE_FROM_TO_MESSAGE:
		{
			return new MoveTileFromToAction(_type); 
		}

		case TiledSpace3D::GET_BRUSH_OF_TILE_MESSAGE:
		{
			return new GetBrushOfTileAction(_type); 
		}

		case TiledSpace3D::SWAP_TWO_TILES_MESSAGE:
		{
			return new SwapTwoTilesAction(_type); 
		}

		case TiledSpace3D::COPY_TILE_MESSAGE:
		{
			return new CopyTileAction(_type); 
		}

		case Camera3D::CREATE_CAMERA_MESSAGE:
		{
			return new CreateCameraAction(_type); 
		}

		case SystemTime::GET_SYSTEM_TIME_MESSAGE: 
		{
			return new GetSystemTimeAction(_type); 
		}

		case Trigger::CREATE_TRIGGER_MESSAGE:
		{
			return new CreateTriggerAction(_type);
		}

		case AbstractVariable::CHANGE_INTEGER_VALUE_MESSAGE: 
		{ 
			return new ChangeIntegerValueAction(_type);
		}

		case AbstractVariable::CHANGE_FLOAT_VALUE_MESSAGE: 
		{ 
			return new ChangeFloatValueAction(_type);
		}
	
		case AbstractVariable::CHANGE_BOOLEAN_VALUE_MESSAGE: 
		{ 
			return new ChangeBooleanValueAction(_type);
		}

		case AbstractVariable::CHANGE_STRING_VALUE_MESSAGE: 
		{ 
			return new ChangeStringValueAction(_type);
		}

		case AbstractVariable::CHANGE_VECTOR_2_VALUE_MESSAGE: 
		{ 
			return new ChangeVector2ValueAction(_type);
		}

		case AbstractVariable::CHANGE_VECTOR_3_VALUE_MESSAGE: 
		{ 
			return new ChangeVector3ValueAction(_type);
		}

		case AbstractVariable::CHANGE_QUATERNION_VALUE_MESSAGE: 
		{ 
			return new ChangeQuaternionValueAction(_type);
		}

		case AssetLibrary::LOAD_ASSET_MESSAGE:
		{ 
			return new LoadAssetAction(_type);
		}

		case AbstractObject::GET_NAME_MESSAGE:
		{ 
			return new GetNameAction(_type);
		}

		case Variable<Vector3>::GET_NORMALIZED_VECTOR_3_MESSAGE:
		{ 
			return new GetNormalizedVector3Action(_type);
		}
		
		case Variable<Vector2>::GET_NORMALIZED_VECTOR_2_MESSAGE:
		{ 
			return new GetNormalizedVector2Action(_type);
		}

		case Variable<Quaternion>::CREATE_ROTATION_FROM_VECTOR_3_TO_VECTOR_3_MESSAGE: 
		{ 
			return new CreateRotationFromVector3ToVector3Action(_type);
		}

		case Variable<Quaternion>::CREATE_ROTATION_FROM_QUATERNION_TO_QUATERNION_MESSAGE:
		{
			return new CreateRotationFromQuaternionToQuaternionAction(_type);
		}

		case Camera3D::GET_CAMERA_LOOK_MESSAGE:
		{
			return new GetCameraLookAction(_type);
		}

		case Variable<Vector3>::GET_VECTOR_3_COMPONENT_MESSAGE:
		{ 
			return new GetVector3ComponentAction(_type);
		}

		case Variable<Vector2>::VECTOR_2_TO_SPHERICAL_MESSAGE:
		{ 
			return new Vector2ToSphericalAction(_type);
		}

		case Variable<Vector3>::VECTOR_3_TO_SPHERICAL_MESSAGE:
		{ 
			return new Vector3ToSphericalAction(_type);
		}

		case Variable<Vector3>::CHANGE_VECTOR_3_COMPONENT_MESSAGE:
		{ 
			return new ChangeVector3ComponentAction(_type);
		}

		case Variable<Vector2>::GET_VECTOR_2_COMPONENT_MESSAGE:
		{ 
			return new GetVector2ComponentAction(_type);
		}

		case Variable<Vector2>::CHANGE_VECTOR_2_COMPONENT_MESSAGE:
		{ 
			return new ChangeVector2ComponentAction(_type);
		}
		
		case AssetLibrary::CHECK_OBJECT_EXIST_MESSAGE:
		{
			return new CheckObjectExistAction(_type);
		}

		case AbstractObject::CHECK_PROPERTY_MESSAGE:
		{
			return new CheckPropertyAction(_type);
		}

		case AbstractObject::ENABLE_PROPERTY_MESSAGE:
		{
			return new EnablePropertyAction(_type);
		}

		case MessageHandler<int32>::SEND_MESSAGE:
		{
			return new SendMessageAction(_type);
		}

		case AbstractObject::DESTROY_OBJECT_MESSAGE:
		{
			return new DestroyObjectAction(_type); 
		}

		case AbstractObject::SAVE_TO_FILE_MESSAGE:
		{
			return new SaveToFileAction(_type); 
		}

		case AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE:
		{
			return new ChangeAnimationFrameAction(_type);
		}

		case PolygonalSurface::ENABLE_COLLISION_POLYGONS_INFO_MESSAGE:
		{
			return new EnableCollisionPolygonsInfoAction(_type);
		}

		case GraphicsSurface::ENABLE_STATIC_OBJECT_MESSAGE:
		{
			return new EnableStaticObjectAction(_type);
		}
		
		case PolygonalSurface::ENABLE_COLLISION_CHECKING_MESSAGE: 
		{
			return new EnableCollisionCheckingAction(_type);
		}

		case PolygonalSurface::CHECK_COLLISION_OBJECT_GROUP_MESSAGE:
		{
			return new CheckCollisionObjectGroupAction(_type);
		}

		case PolygonalSurface::CHECK_COLLISION_OBJECT_OBJECT_MESSAGE:
		{
			return new CheckCollisionObjectObjectAction(_type);
		}

		case PolygonalSurface::CHECK_COLLISION_GROUP_GROUP_MESSAGE:
		{
			return new CheckCollisionGroupGroupAction(_type);
		}

		case PolygonalSurface::CHANGE_COLLISION_GROUP_MESSAGE: 
		{
			return new ChangeCollisionGroupAction(_type);
		}

		case AbstractAnimation::GET_AMOUNT_OF_FRAMES_MESSAGE:
		{
			return new GetAmountOfFramesAction(_type);
		}
				
		case TexturedAnimation::GET_FRAME_SIZE_MESSAGE: 
		{ 
			return new GetFrameSizeAction(_type);
		}

		case TexturedAnimation::CHANGE_ATLAS_MESSAGE:
		{
			return new ChangeAtlasAction(_type);
		}

		case Texture2D::GET_TEXTURE_SIZE_MESSAGE: 
		{ 
			return new GetTextureSizeAction(_type);
		}

		case Variable<float>::CHANGE_FLOAT_RANGE_MESSAGE:
		{
			return new ChangeFloatValueRangeAction(_type);
		}

		case Variable<int32>::CHANGE_INTEGER_RANGE_MESSAGE:
		{
			return new ChangeIntegerValueRangeAction(_type);
		}

		case Random::CHANGE_RANDOM_SEED_MESSAGE:
		{
			return new ChangeRandomSeedAction(_type);
		}

		case Random::GET_RANDOM_BOOLEAN_MESSAGE:
		{
			return new GetRandomBooleanAction(_type); 
		}

		case Random::GET_RANDOM_INTEGER_MESSAGE:
		{
			return new GetRandomIntegerAction(_type);
		}

		case Random::GET_RANDOM_FLOAT_MESSAGE:
		{
			return new GetRandomFloatAction(_type);
		}

		case AbstractList::CHANGE_ITEM_INTEGER_MESSAGE:
		{
			return new ChangeItemIntegerAction(_type);
		}

		case AbstractList::CHANGE_ITEM_FLOAT_MESSAGE:
		{
			return new ChangeItemFloatAction(_type);
		}

		case AbstractList::CHANGE_ITEM_BOOLEAN_MESSAGE:
		{
			return new ChangeItemBooleanAction(_type);
		}

		case AbstractList::CHANGE_ITEM_STRING_MESSAGE:
		{
			return new ChangeItemStringAction(_type);
		}

		case AbstractList::CHANGE_ITEM_VECTOR_2_MESSAGE:
		{
			return new ChangeItemVector2Action(_type);
		}

		case AbstractList::CHANGE_ITEM_VECTOR_3_MESSAGE:
		{
			return new ChangeItemVector3Action(_type);
		}

		case AbstractList::CHANGE_ITEM_QUATERNION_MESSAGE:
		{
			return new ChangeItemQuaternionAction(_type);
		}

		case AbstractList::CHANGE_ITEM_COLORABLE_MESSAGE:
		{
			return new ChangeItemColorableAction(_type);
		}

		case AbstractList::CHANGE_ITEM_TRANSLATABLE_MESSAGE:
		{
			return new ChangeItemTranslatableAction(_type);
		}

		case AbstractList::CHANGE_ITEM_ROTATABLE_MESSAGE:
		{
			return new ChangeItemRotatableAction(_type);
		}

		case AbstractList::CHANGE_ITEM_SCALABLE_MESSAGE:
		{
			return new ChangeItemScalableAction(_type);
		}

		case AbstractList::CHANGE_LIST_INTEGER_MESSAGE:
		{
			return new ChangeListIntegerAction(_type);
		}

		case AbstractList::CHANGE_LIST_FLOAT_MESSAGE:
		{
			return new ChangeListFloatAction(_type);
		}

		case AbstractList::CHANGE_LIST_BOOLEAN_MESSAGE:
		{
			return new ChangeListBooleanAction(_type);
		}

		case AbstractList::CHANGE_LIST_STRING_MESSAGE:
		{
			return new ChangeListStringAction(_type);
		}

		case AbstractList::CHANGE_LIST_VECTOR_2_MESSAGE:
		{
			return new ChangeListVector2Action(_type);
		}

		case AbstractList::CHANGE_LIST_VECTOR_3_MESSAGE:
		{
			return new ChangeListVector3Action(_type);
		}

		case AbstractList::CHANGE_LIST_QUATERNION_MESSAGE:
		{
			return new ChangeListQuaternionAction(_type);
		}

		case AbstractList::CHANGE_LIST_COLORABLE_MESSAGE:
		{
			return new ChangeListColorableAction(_type);
		}

		case AbstractList::CHANGE_LIST_TRANSLATABLE_MESSAGE:
		{
			return new ChangeListTranslatableAction(_type);
		}

		case AbstractList::CHANGE_LIST_ROTATABLE_MESSAGE:
		{
			return new ChangeListRotatableAction(_type);
		}

		case AbstractList::CHANGE_LIST_SCALABLE_MESSAGE:
		{
			return new ChangeListScalableAction(_type);
		}

		case AbstractList::REMOVE_INDEX_FROM_LIST_MESSAGE:
		{
			return new RemoveIndexFromListAction(_type);
		}
		
		case AbstractList::REMOVE_ITEM_FROM_LIST_MESSAGE:
		{
			return new RemoveItemFromListAction(_type);
		}

		case AbstractList::ADD_TO_LIST_MESSAGE:
		{
			return new AddToListAction(_type);
		}

		case AbstractList::GET_DATA_FROM_ITEM_MESSAGE:
		{
			return new GetDataFromItemAction(_type);
		}

		case AbstractList::GET_ITEM_INDEX_MESSAGE:
		{
			return new GetItemIndexAction(_type);
		}

		case AbstractList::RANDOMIZE_LIST_MESSAGE: 
		{
			return new RandomizeListAction(_type);
		}

		case AbstractList::SWAP_LIST_MESSAGE: 
		{
			return new SwapListAction(_type);
		}

		case AbstractList::SWAP_ITEM_MESSAGE: 
		{
			return new SwapItemAction(_type);
		}

		case AbstractList::SWAP_INDEX_MESSAGE: 
		{
			return new SwapIndexAction(_type);
		}

		case AbstractList::GET_AMOUNT_OF_ITEMS_MESSAGE:
		{
			return new GetAmountOfItemsAction(_type);
		}

		case ActivatableObject::CHANGE_DEACTIVATION_MODE_MESSAGE:
		{
			return new ChangeDeactivationModeAction(_type);
		}

		case ActivatableObject::CHANGE_ACTIVATION_LIMIT_MESSAGE:
		{
			return new ChangeActivationLimitAction(_type);
		}

		case AbstractList::CHECK_LIST_EMPTY_MESSAGE:
		{
			return new CheckListEmptyAction(_type);
		}

		case AbstractList::ROTATE_LIST_MESSAGE:
		{
			return new RotateListAction(_type);
		}

		case AbstractList::COPY_LIST_MESSAGE:
		{
			return new CopyListAction(_type);
		}

		case AbstractList::CLEAR_LIST_MESSAGE:
		{
			return new ClearListAction(_type);
		}

		case AbstractList::INVERSE_LIST_MESSAGE:
		{
			return new InverseListAction(_type);
		}

		case PlayerControl::ADD_INPUT_EVENT_MESSAGE:
		{
			return new AddInputEventAction(_type);
		}

		case PlayerControl::CLEAR_INPUT_EVENTS_MESSAGE:
		{
			return new ClearInputEventsAction(_type);
		}

		case AbstractList::CREATE_LIST_MESSAGE:
		{ 
			return new CreateListAction(_type);
		}

		case AbstractVariable::CREATE_INTEGER_VARIABLE_MESSAGE:
		{ 
			return new CreateIntegerVariableAction(_type);			
		}

		case AbstractVariable::CREATE_FLOAT_VARIABLE_MESSAGE:
		{ 
			return new CreateFloatVariableAction(_type);			
		}

		case AbstractVariable::CREATE_STRING_VARIABLE_MESSAGE: 
		{ 
			return new CreateStringVariableAction(_type);			
		}

		case AbstractVariable::CREATE_BOOLEAN_VARIABLE_MESSAGE:
		{ 
			return new CreateBooleanVariableAction(_type);			
		}

		case AbstractVariable::CREATE_VECTOR_2_VARIABLE_MESSAGE: 
		{ 
			return new CreateVector2VariableAction(_type);			
		}

		case AbstractVariable::CREATE_VECTOR_3_VARIABLE_MESSAGE: 
		{ 
			return new CreateVector3VariableAction(_type);			
		}

		case AbstractVariable::CREATE_QUATERNION_VARIABLE_MESSAGE: 
		{ 
			return new CreateQuaternionVariableAction(_type);			
		}

		case ColorableObject::CHANGE_BACKGROUND_COLOR_MESSAGE:
		{
			return new ChangeBackgroundColorAction(_type); 
		}

		case ColorableObject::GET_COLOR_RGB_MESSAGE: 
		{
			return new GetColorRGBAction(_type);
		} 

		case ColorableObject::GET_COLOR_HSL_MESSAGE: 
		{
			return new GetColorHSLAction(_type);
		} 

		case ColorableObject::GET_ALPHA_MESSAGE: 
		{
			return new GetAlphaAction(_type);
		} 

		case ColorableObject::GET_RED_MESSAGE: 
		{
			return new GetRedAction(_type);
		} 

		case ColorableObject::GET_GREEN_MESSAGE: 
		{
			return new GetGreenAction(_type);
		}

		case ColorableObject::GET_BLUE_MESSAGE:
		{
			return new GetBlueAction(_type);
		}
		
		case ColorableObject::GET_HUE_MESSAGE: 
		{
			return new GetHueAction(_type);
		} 

		case ColorableObject::GET_SATURATION_MESSAGE:
		{
			return new GetSaturationAction(_type);
		}

		case ColorableObject::GET_LIGHTNESS_MESSAGE:
		{
			return new GetLightnessAction(_type);
		}
		
		case ColorableObject::CHANGE_COLOR_RGB_MESSAGE:
		{ 
			return new ChangeColorRGBAction(_type);
		}

		case ColorableObject::CHANGE_COLOR_HSL_MESSAGE:
		{ 
			return new ChangeColorHSLAction(_type);
		} 

		case ColorableObject::CHANGE_ALPHA_MESSAGE:
		{ 
			return new ChangeAlphaAction(_type);
		} 

		case ColorableObject::CHANGE_RED_MESSAGE:
		{ 
			return new ChangeRedAction(_type);
		}

		case ColorableObject::CHANGE_GREEN_MESSAGE:
		{ 
			return new ChangeGreenAction(_type);
		} 

		case ColorableObject::CHANGE_BLUE_MESSAGE:
		{ 
			return new ChangeBlueAction(_type);
		}

		case ColorableObject::CHANGE_HUE_MESSAGE:
		{ 
			return new ChangeHueAction(_type);
		}

		case ColorableObject::CHANGE_SATURATION_MESSAGE:
		{ 
			return new ChangeSaturationAction(_type);
		} 

		case ColorableObject::CHANGE_LIGHTNESS_MESSAGE:
		{ 
			return new ChangeLightnessAction(_type);
		}
		
		case ScalableObject::GET_SCALE_MESSAGE: 
		{ 
			return new GetScaleAction(_type);
		} 

		case ScalableObject::DISPLACE_SCALE_MESSAGE: 
		{ 
			return new DisplaceScaleAction(_type);
		} 

		case ScalableObject::REPLACE_SCALE_MESSAGE:
		{
			return new ReplaceScaleAction(_type);
		}

		case TranslatableObject::GET_POSITION_MESSAGE:
		{
			return new GetPositionAction(_type);
		} 

		case TranslatableObject::MOVE_AROUND_PIVOT_MESSAGE:
		{
			return new MoveAroundPivotAction(_type);
		} 

		case RotatableObject::ROTATE_AROUND_PIVOT_MESSAGE:
		{
			return new RotateAroundPivotAction(_type);
		} 

		case RotatableObject::GET_ROTATION_MESSAGE: 
		{ 
			return new GetRotationAction(_type);
		}

		case TranslatableObject::DISPLACE_POSITION_MESSAGE:
		{ 
			return new DisplacePositionAction(_type);
		}

		case TranslatableObject::REPLACE_POSITION_MESSAGE:
		{ 
			return new ReplacePositionAction(_type);
		} 

		case TiledSpace3D::GET_TILE_CENTER_MESSAGE:
		{ 
			return new GetTileCenterAction(_type);
		} 
		
		case RotatableObject::REPLACE_ROTATION_MESSAGE:
		{
			return new ReplaceRotationAction(_type);
		}
				
		case RotatableObject::DISPLACE_ROTATION_MESSAGE:
		{ 
			return new DisplaceRotationAction(_type);
		}

		case Plane::CHANGE_PLANE_NORMAL_MESSAGE:
		{ 
			return new ChangePlaneNormalAction(_type);
		} 
		
		case Camera3D::CHANGE_VIEWING_POINT_MESSAGE:
		{ 
			return new ChangeViewingPointAction(_type);
		} 

		case Camera3D::CAMERA_LOOK_AT_MESSAGE:
		{ 
			return new CameraLookAtAction(_type);
		} 

		case PolygonalSurface::CHANGE_MESH_MESSAGE:
		{
			return new ChangeMeshAction(_type);
		}

		case PolygonalSurface::CHANGE_HITBOX_MESSAGE:
		{
			return new ChangeHitboxAction(_type);
		}
	
		case Mesh::CREATE_QUAD_MESSAGE:
		{ 
			return new CreateQuadAction(_type);
		}

		case Mesh::CREATE_BOX_MESSAGE:
		{ 
			return new CreateBoxAction(_type);
		}

		case AbstractSurface::CHANGE_INTERSECTION_MODE_MESSAGE:
		{ 
			return new ChangeIntersectionModeAction(_type);
		}

		case GraphicsSurface::ENABLE_BACK_FACE_CULLING_MESSAGE:
		{
			return new EnableBackFaceCullingAction(_type);
		}

		case TexturedAnimation::ENABLE_POSITION_OFFSET_MESSAGE: 
		{
			return new EnablePositionOffsetAction(_type);
		}

		case LogicObject::ENABLE_FREEZE_MESSAGE:
		{
			return new EnableFreezeAction(_type);
		}

		case GraphicsObject::ENABLE_SHOW_MESSAGE:
		{
			return new EnableShowAction(_type);
		}

		case LogicObject::CHANGE_DELTA_SCALE_MESSAGE:
		{
			return new ChangeDeltaScaleAction(_type);
		}

		case TexturingSurface::CHANGE_FRONT_TEXTURE_MESSAGE:
		{
			return new ChangeFrontTextureAction(_type);
		}
		
		case TexturingSurface::CHANGE_BACK_TEXTURE_MESSAGE:
		{
			return new ChangeBackTextureAction(_type);
		}

		case TexturingSurface::CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE:
		{
			return new ChangeFrontTexturedAnimationAction(_type);
		}

		case TexturingSurface::CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE:
		{
			return new ChangeBackTexturedAnimationAction(_type);
		}

		case Mouse::ENABLE_SHOW_CURSOR_MESSAGE:
		{
			return new EnableShowCursorAction(_type); 
		}

		case Renderer::ENABLE_Z_BUFFER_MESSAGE:
		{
			return new EnableZBufferAction(_type); 
		}
		
		case Trigger::ENABLE_TRIGGERS_BLOCK_MESSAGE:
		{
			return new EnableTriggersBlockAction(_type); 
		}

		case Keyboard::CREATE_KEYBOARD_EVENT_MESSAGE:
		{
			return new CreateKeyboardEventAction(_type);
		} 

		case Scene::PICK_SCENE_MESSAGE:
		{
			return new PickSceneAction(_type); 
		}

		case ObjectScene::ADD_TO_LAYER_MESSAGE:
		{
			return new AddToLayerAction(_type);
		}

		case ObjectScene::REMOVE_FROM_LAYER_MESSAGE:
		{
			return new RemoveFromLayerAction(_type);
		}
				
		case LogicScene::ADD_TO_UPDATE_LIST_MESSAGE:
		{
			return new AddToUpdateListAction(_type);
		}

		case LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE:
		{
			return new RemoveFromUpdateListAction(_type);
		}

		case GraphicsScene::ADD_TO_RENDER_LIST_MESSAGE:
		{ 
			return new AddToRenderListAction(_type);
		}

		case GraphicsScene::REMOVE_FROM_RENDER_LIST_MESSAGE:
		{
			return new RemoveFromRenderListAction(_type);
		}

		case GraphicsContainer::CHANGE_CAMERA_MESSAGE:
		{
			return new ChangeCameraAction(_type);
		}

		case GraphicsContainer::CHANGE_RENDER_TARGET_MESSAGE:
		{
			return new ChangeRenderTargetAction(_type);
		}

		case Camera3D::GET_SELECT_RAY_MESSAGE:
		{
			return new GetSelectRayAction(_type);
		}

		case TexturingSurface::ENABLE_TEXTURE_FLIP_MESSAGE:
		{
			return new EnableTextureFlipAction(_type);
		}

		case FloatLerp::CHANGE_FLOAT_LERP_T_MESSAGE:
		{
			return new ChangeFloatLerpTAction(_type);
		}

		case FloatLerp::GET_FLOAT_LERP_T_MESSAGE:
		{
			return new GetFloatLerpTAction(_type);
		}

		case FloatLerp::GET_FLOAT_LERP_CURRENT_MESSAGE:
		{
			return new GetFloatLerpCurrentAction(_type);
		}

		case FloatLerp::GET_FLOAT_LERP_DELTA_MESSAGE:
		{
			return new GetFloatLerpDeltaAction(_type);
		}

		case Variable<Vector3>::GET_VECTOR_3_ANGLE_MESSAGE:
		{
			return new GetVector3AngleAction(_type);
		}

		case Variable<Vector2>::GET_VECTOR_2_ANGLE_MESSAGE:
		{
			return new GetVector2AngleAction(_type);
		}
		
		case Variable<Vector3>::GET_VECTOR_3_LENGTH_MESSAGE:
		{
			return new GetVector3LengthAction(_type);
		}

		case Variable<Vector2>::GET_VECTOR_2_LENGTH_MESSAGE:
		{
			return new GetVector2LengthAction(_type);
		}
		
		case Line::GET_LINE_DIRECTION_MESSAGE:
		{
			return new GetLineDirectionAction(_type);
		}

		case Line::GET_LINE_ANGLE_Z_MESSAGE:
		{
			return new GetLineAngleZAction(_type);
		}

		case Circle::GET_CIRCLE_LENGTH_MESSAGE:
		{
			return new GetCircleLengthAction(_type);
		}

		case Line::GET_LINE_LENGTH_MESSAGE:
		{
			return new GetLineLengthAction(_type);
		}

		case QuaternionLerp::GET_DATA_FROM_CONTROL_QUATERNION_MESSAGE:
		{
			return new GetDataFromControlQuaternionAction(_type);
		}

		case QuaternionLerp::GET_DATA_FROM_CONTROL_ROTATABLE_MESSAGE:
		{
			return new GetDataFromControlRotatableAction(_type);
		}

		case VectorLerp::GET_DATA_FROM_CONTROL_VECTOR_MESSAGE:
		{
			return new GetDataFromControlVectorAction(_type);
		}

		case VectorLerp::GET_DATA_FROM_CONTROL_TRANSLATABLE_MESSAGE:
		{
			return new GetDataFromControlTranslatableAction(_type);
		}

		case VectorLerp::GET_DATA_FROM_CONTROL_SCALABLE_MESSAGE:
		{
			return new GetDataFromControlScalableAction(_type);
		}
	
		case VectorLerp::GET_DATA_FROM_CONTROL_COLORABLE_MESSAGE:
		{
			return new GetDataFromControlColorableAction(_type);
		}

		case RotatableObject::CREATE_ROTATION_MESSAGE:
		{
			return new CreateRotationAction(_type);
		}

		case Camera3D::CHANGE_PROJECTION_MODE_MESSAGE:
		{
			return new ChangeProjectionModeAction(_type);
		}
			
		case Camera3D::CHANGE_MOVEMENT_MODE_MESSAGE:
		{
			return new ChangeMovementModeAction(_type);
		}

		case Camera3D::CHANGE_ROTATION_MODE_MESSAGE:
		{
			return new ChangeRotationModeAction(_type);
		}
		
		case Camera3D::PICK_CAMERA_MESSAGE:
		{
			return new PickCameraAction(_type); 
		}

		case FloatLerp::CHANGE_FLOAT_LERP_MODE_MESSAGE:
		{
			return new ChangeFloatLerpModeAction(_type);
		} 

		case FloatLerp::CHANGE_FLOAT_LERP_SPEED_MESSAGE:
		{
			return new ChangeFloatLerpSpeedAction(_type);
		} 

		case FloatLerp::CREATE_FLOAT_LERP_MESSAGE:
		{
			return new CreateFloatLerpAction(_type); 
		}

		case TiledSpace3D::CREATE_TILED_SPACE_MESSAGE:
		{
			return new CreateTiledSpaceAction(_type); 
		}

		case TiledSpace3D::CHANGE_TILED_SPACE_SIZE_MESSAGE:
		{
			return new ChangeTiledSpaceSizeAction(_type); 
		}

		case TiledSpace3D::CREATE_SOLID_TILE_MESSAGE:
		{
			return new CreateSolidTileAction(_type); 
		}

		case TiledSpace3D::CREATE_EMPTY_TILE_MESSAGE:
		{
			return new CreateEmptyTileAction(_type); 
		}

		case TiledSpace3D::CREATE_BRUSH_MESSAGE:
		{
			return new CreateBrushAction(_type); 
		}

		case TiledSpace3D::REMOVE_BRUSH_MESSAGE:
		{
			return new RemoveBrushAction(_type); 
		}

		case QuaternionLerp::CREATE_QUATERNION_LERP_MESSAGE:
		{
			return new CreateQuaternionLerpAction(_type); 
		}

		case VectorLerp::CREATE_VECTOR_LERP_MESSAGE:
		{
			return new CreateVectorLerpAction(_type); 
		}
				
		case FloatLerp::STREAMING_FLOAT_T_MESSAGE:
		{
			return new StreamingFloatTAction(_type);
		}

		case FloatLerp::STREAMING_FLOAT_T_ALPHA_MESSAGE:
		{
			return new StreamingFloatTAlphaAction(_type);
		}

		case FloatLerp::STREAMING_FLOAT_T_ANIMATION_FRAME_MESSAGE:
		{
			return new StreamingFloatTAnimationFrameAction(_type);
		}
		
		case FloatLerp::STREAMING_FLOAT_T_RED_MESSAGE:
		{
			return new StreamingFloatTRedAction(_type);
		}

		case FloatLerp::STREAMING_FLOAT_T_GREEN_MESSAGE:
		{
			return new StreamingFloatTGreenAction(_type);
		}

		case FloatLerp::STREAMING_FLOAT_T_BLUE_MESSAGE:
		{
			return new StreamingFloatTBlueAction(_type);
		}

		case FloatLerp::STREAMING_FLOAT_T_HUE_MESSAGE:
		{
			return new StreamingFloatTHueAction(_type);
		}

		case FloatLerp::STREAMING_FLOAT_T_SATURATION_MESSAGE:
		{
			return new StreamingFloatTSaturationAction(_type);
		}

		case FloatLerp::STREAMING_FLOAT_T_LIGHTNESS_MESSAGE:
		{
			return new StreamingFloatTLightnessAction(_type);
		}

		case FloatLerp::STREAMING_FLOAT_T_DELTA_SCALE_MESSAGE:
		{
			return new StreamingFloatTDeltaScaleAction(_type);
		}

		case FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE:
		{
			return new StreamingFloatCurrentAction(_type);
		}  

		case FloatLerp::CHANGE_FLOAT_LERP_FLOAT_TARGET_MESSAGE:
		{
			return new ChangeFloatLerpFloatTargetAction(_type);
		}

		case FloatLerp::CHANGE_FLOAT_LERP_INTEGER_TARGET_MESSAGE:
		{
			return new ChangeFloatLerpIntegerTargetAction(_type);
		}

		case FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE:
		{
			return new StreamingFloatDeltaAction(_type);
		}

		case VectorLerp::CHANGE_VECTOR_LERP_MODE_MESSAGE:
		{
			return new ChangeVectorLerpModeAction(_type);
		} 

		case LerpingObject::CHANGE_FLOAT_LERP_MESSAGE:
		{
			return new ChangeFloatLerpAction(_type);
		}

		case PlayingObject::CHANGE_PLAYING_MODE_MESSAGE:
		{
			return new ChangePlayingModeAction(_type);
		} 

		case PlayingObject::CHANGE_PLAYING_STATE_MESSAGE:
		{
			return new ChangePlayingStateAction(_type);
		} 

		case PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE:
		{
			return new ChangePlayingDurationInSecAction(_type);
		} 

		case Camera3D::CHANGE_DISTANCE_TO_VIEWING_POINT_MESSAGE: 
		{
			return new ChangeDistanceToViewingPointAction(_type);
		} 

		case QuaternionLerp::STREAMING_QUATERNION_CURRENT_MESSAGE:
		{
			return new StreamingQuaternionCurrentAction(_type);
		}

		case QuaternionLerp::STREAMING_ROTATABLE_CURRENT_MESSAGE:
		{
			return new StreamingRotatableCurrentAction(_type);
		}

		case QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE:
		{
			return new StreamingQuaternionDeltaAction(_type);
		}
			
		case QuaternionLerp::STREAMING_ROTATABLE_DELTA_MESSAGE:
		{
			return new StreamingRotatableDeltaAction(_type);
		}

		case VectorLerp::STREAMING_COLORABLE_CURRENT_MESSAGE:
		{
			return new StreamingColorableCurrentAction(_type);
		}

		case VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE:
		{
			return new StreamingVectorDeltaAction(_type);
		}

		case VectorLerp::STREAMING_TRANSLATABLE_DELTA_MESSAGE:
		{
			return new StreamingTranslatableDeltaAction(_type);
		}

		case VectorLerp::STREAMING_SCALABLE_DELTA_MESSAGE:
		{
			return new StreamingScalableDeltaAction(_type);
		}

		case VectorLerp::STREAMING_COLORABLE_DELTA_MESSAGE:
		{
			return new StreamingColorableDeltaAction(_type);
		}

		case VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE:
		{
			return new StreamingVectorCurrentAction(_type);
		}

		case VectorLerp::STREAMING_TRANSLATABLE_CURRENT_MESSAGE:
		{
			return new StreamingTranslatableCurrentAction(_type);
		}

		case VectorLerp::STREAMING_SCALABLE_CURRENT_MESSAGE:
		{
			return new StreamingScalableCurrentAction(_type);
		}

		case TexturingSurface::CHECK_LINE_IN_IMAGE_MESSAGE:
		{
			return new CheckLineInImageAction(_type);
		}

		case Camera3D::CHECK_FRUSTUM_IN_QUAD_MESSAGE: 
		{
			return new CheckFrustumInQuadAction(_type);
		}

		case PolygonalSurface::CHECK_LINE_IN_POLYGON_MESSAGE:
		{
			return new CheckLineInPolygonAction(_type);
		}

		case AbstractSurface::CHECK_LINE_IN_SURFACE_MESSAGE:
		{
			return new CheckLineInSurfaceAction(_type);
		}

		case VectorLerp::SET_TWO_VECTORS_MESSAGE: 
		{
			return new SetTwoVectorsAction(_type);
		}

		case VectorLerp::SET_TWO_TRANSLATABLES_MESSAGE: 
		{
			return new SetTwoTranslatablesAction(_type);
		}

		case VectorLerp::SET_TWO_SCALABLES_MESSAGE: 
		{
			return new SetTwoScalablesAction(_type);
		}

		case VectorLerp::SET_TWO_COLORABLES_MESSAGE:
		{
			return new SetTwoColorablesAction(_type);
		}

		case VectorLerp::CHANGE_CONTROL_VECTORS_MESSAGE:
		{
			return new ChangeControlVectorsAction(_type);
		}

		case VectorLerp::CHANGE_CONTROL_TRANSLATABLES_MESSAGE:
		{
			return new ChangeControlTranslatablesAction(_type);
		}

		case VectorLerp::CHANGE_CONTROL_SCALABLES_MESSAGE:
		{
			return new ChangeControlScalablesAction(_type);
		}

		case VectorLerp::CHANGE_CONTROL_COLORABLES_MESSAGE:
		{
			return new ChangeControlColorablesAction(_type);
		}

		case QuaternionLerp::CHANGE_CONTROL_QUATERNIONS_MESSAGE:
		{
			return new ChangeControlQuaternionsAction(_type);
		}

		case QuaternionLerp::CHANGE_CONTROL_ROTATABLES_MESSAGE:
		{
			return new ChangeControlRotatablesAction(_type);
		}

		case QuaternionLerp::SET_TWO_QUATERNIONS_MESSAGE: 
		{
			return new SetTwoQuaternionsAction(_type);
		}

		case QuaternionLerp::SET_TWO_ROTATABLES_MESSAGE:
		{
			return new SetTwoRotatablesAction(_type);
		}
		
		case VectorLerp::SET_DATA_TO_CONTROL_VECTOR_MESSAGE:
		{
			return new SetDataToControlVectorAction(_type);
		}

		case VectorLerp::SET_DATA_TO_CONTROL_TRANSLATABLE_MESSAGE:
		{
			return new SetDataToControlTranslatableAction(_type);
		}

		case VectorLerp::SET_DATA_TO_CONTROL_SCALABLE_MESSAGE:
		{
			return new SetDataToControlScalableAction(_type);
		}
	
		case VectorLerp::SET_DATA_TO_CONTROL_COLORABLE_MESSAGE:
		{
			return new SetDataToControlColorableAction(_type);
		}

		case QuaternionLerp::SET_DATA_TO_CONTROL_QUATERNION_MESSAGE:
		{
			return new SetDataToControlQuaternionAction(_type);
		}

		case QuaternionLerp::SET_DATA_TO_CONTROL_ROTATABLE_MESSAGE:
		{
			return new SetDataToControlRotatableAction(_type);
		}
	} 
	return NIL;
}
