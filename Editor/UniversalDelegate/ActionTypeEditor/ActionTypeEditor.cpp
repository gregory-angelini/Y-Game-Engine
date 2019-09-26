#include "ActionTypeEditor.h"
#include <Editor/Editor/Editor.h>



ActionTypeEditor::ActionTypeEditor(QWidget* _widget): QComboBox(_widget), selectedCategory(-1), selectedProperty(-1), selectedActionType(-1)
{
	model = new QStandardItemModel(MAX_ROWS, 1, this);
	QStringList columnLabels;
	columnLabels.push_back("CATEGORY");
	columnLabels.push_back("PROPERTY");
	columnLabels.push_back("ACTION_TYPE");
	model->setHorizontalHeaderLabels(columnLabels);
	QStringList rowLabels;
	for(int i = 0; i < MAX_ROWS; i++)
	{
		rowLabels.push_back("");
	}
	model->setVerticalHeaderLabels(rowLabels);

	model->verticalHeaderItem(BASIC)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/basic.png"));
	
	model->verticalHeaderItem(SHAPE)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/shape.png"));
	model->verticalHeaderItem(TRANSFORMATION)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/transformation.png"));

	model->verticalHeaderItem(MATERIAL)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/material.png"));
	model->verticalHeaderItem(VIEW)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/view.png"));

	model->verticalHeaderItem(INTERPOLATION)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/interpolation.png"));
	model->verticalHeaderItem(PLAYBACK)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/playback.png"));

	model->verticalHeaderItem(RANDOMIZATION)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/randomization.png"));
	model->verticalHeaderItem(COMBINATION)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/combination.png"));

	model->verticalHeaderItem(INPUT)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/input.png"));
	model->verticalHeaderItem(TIME)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/time.png"));
	model->verticalHeaderItem(TEXT)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/text.png"));
	model->verticalHeaderItem(FILE)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/file.png"));

	model->verticalHeaderItem(CONTAINER)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/container.png"));
	
	model->verticalHeaderItem(COLLISION)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/collision.png"));
	
	model->verticalHeaderItem(SCRIPT)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/script.png"));
	categoryLabels.push_back("BASIC");

	categoryLabels.push_back("SHAPE");
	categoryLabels.push_back("TRANSFORMATION");

	categoryLabels.push_back("MATERIAL");
	categoryLabels.push_back("VIEW");

	categoryLabels.push_back("INTERPOLATION");
	categoryLabels.push_back("PLAYBACK");
	categoryLabels.push_back("ANIMATION");

	categoryLabels.push_back("RANDOMIZATION");
	categoryLabels.push_back("COMBINATION");

	categoryLabels.push_back("INPUT");
	categoryLabels.push_back("TIME");
	categoryLabels.push_back("TEXT");
	categoryLabels.push_back("FILE");
	categoryLabels.push_back("SOUND");

	categoryLabels.push_back("CONTAINER");

	categoryLabels.push_back("COLLISION");

	categoryLabels.push_back("SCRIPT");
	for(int i = 0; i < categoryLabels.size(); i++)
	{
		AddCategory(i, categoryLabels.at(i));
	}
	propertyTree[BASIC].AddToTail("SYSTEM");
	propertyTree[BASIC].AddToTail("ABSTRACT_OBJECT");
	propertyTree[SHAPE].AddToTail("POLYGONAL_SURFACE");
	propertyTree[SHAPE].AddToTail("MESH");
	propertyTree[SHAPE].AddToTail("GRAPHICS_STRIP"); 
	propertyTree[SHAPE].AddToTail("PLANE");
	propertyTree[SHAPE].AddToTail("LINE");
	propertyTree[SHAPE].AddToTail("CIRCLE");
	propertyTree[TRANSFORMATION].AddToTail("SCALABLE_OBJECT");
	propertyTree[TRANSFORMATION].AddToTail("TRANSLATABLE_OBJECT");
	propertyTree[TRANSFORMATION].AddToTail("ROTATABLE_OBJECT");
	propertyTree[TRANSFORMATION].AddToTail("GRAPHICS_SURFACE");
	propertyTree[TRANSFORMATION].AddToTail("VARIABLE");
	propertyTree[MATERIAL].AddToTail("GRAPHICS_OBJECT");
	propertyTree[MATERIAL].AddToTail("TEXTURE");
	propertyTree[MATERIAL].AddToTail("COLORABLE_OBJECT");
	propertyTree[MATERIAL].AddToTail("GRAPHICS_SURFACE");
	propertyTree[MATERIAL].AddToTail("TEXTURING_SURFACE");
	propertyTree[MATERIAL].AddToTail("GRAPHICS_CONTAINER");
	propertyTree[VIEW].AddToTail("SYSTEM");
	propertyTree[VIEW].AddToTail("CAMERA");
	propertyTree[VIEW].AddToTail("GRAPHICS_CONTAINER");
	propertyTree[VIEW].AddToTail("SCENE");
	propertyTree[INTERPOLATION].AddToTail("LERPING_OBJECT");
	propertyTree[INTERPOLATION].AddToTail("FLOAT_LERP");
	propertyTree[INTERPOLATION].AddToTail("QUATERNION_LERP");
	propertyTree[INTERPOLATION].AddToTail("VECTOR_LERP");
	propertyTree[PLAYBACK].AddToTail("PLAYING_OBJECT");
	propertyTree[PLAYBACK].AddToTail("ACTIVATABLE_OBJECT");
	propertyTree[ANIMATION].AddToTail("ABSTRACT_ANIMATION");
	propertyTree[ANIMATION].AddToTail("TEXTURED_ANIMATION");
	propertyTree[RANDOMIZATION].AddToTail("SYSTEM");
	propertyTree[RANDOMIZATION].AddToTail("VARIABLE");
	propertyTree[RANDOMIZATION].AddToTail("LIST");
	propertyTree[INPUT].AddToTail("KEYBOARD");
	propertyTree[INPUT].AddToTail("PLAYER_CONTROL");
	propertyTree[TIME].AddToTail("SYSTEM");
	propertyTree[TIME].AddToTail("TIMER");
	propertyTree[TIME].AddToTail("LOGIC_OBJECT");
	propertyTree[FILE].AddToTail("SYSTEM");
	propertyTree[FILE].AddToTail("ABSTRACT_OBJECT");
	propertyTree[CONTAINER].AddToTail("VARIABLE");
	propertyTree[CONTAINER].AddToTail("LIST");
	propertyTree[CONTAINER].AddToTail("LAYER");
	propertyTree[CONTAINER].AddToTail("LOGIC_SCENE");
	propertyTree[CONTAINER].AddToTail("GRAPHICS_SCENE");
	propertyTree[CONTAINER].AddToTail("TILED_SPACE");
	propertyTree[CONTAINER].AddToTail("SET");
	propertyTree[COLLISION].AddToTail("ABSTRACT_SURFACE");
	propertyTree[COLLISION].AddToTail("POLYGONAL_SURFACE");
	propertyTree[COLLISION].AddToTail("TEXTURING_SURFACE");
	propertyTree[COLLISION].AddToTail("CAMERA");
	propertyTree[SCRIPT].AddToTail("SYSTEM");
	propertyTree[SCRIPT].AddToTail("TRIGGER");
	actionTree[BASIC][BASIC_SYSTEM].AddToTail("ENABLE_SHOW_CURSOR");
	
	actionTree[BASIC][BASIC_ABSTRACT_OBJECT].AddToTail("CHANGE_NAME");
	actionTree[BASIC][BASIC_ABSTRACT_OBJECT].AddToTail("GET_NAME");
	actionTree[BASIC][BASIC_ABSTRACT_OBJECT].AddToTail("CHECK_OBJECT_EXIST");
	actionTree[BASIC][BASIC_ABSTRACT_OBJECT].AddToTail("SEND_MESSAGE");
	actionTree[BASIC][BASIC_ABSTRACT_OBJECT].AddToTail("DESTROY_OBJECT");
	actionTree[BASIC][BASIC_ABSTRACT_OBJECT].AddToTail("SET_PRIVATE_ASSET");
	actionTree[BASIC][BASIC_ABSTRACT_OBJECT].AddToTail("CHECK_PROPERTY");
	actionTree[BASIC][BASIC_ABSTRACT_OBJECT].AddToTail("ENABLE_PROPERTY");
	actionTree[SHAPE][SHAPE_POLYGONAL_SURFACE].AddToTail("CHANGE_MESH");

	actionTree[SHAPE][SHAPE_MESH].AddToTail("CREATE_MESH");
	actionTree[SHAPE][SHAPE_MESH].AddToTail("CREATE_QUAD");
	actionTree[SHAPE][SHAPE_MESH].AddToTail("CREATE_BOX");

	actionTree[SHAPE][SHAPE_GRAPHICS_STRIP].AddToTail("CHANGE_STRIP_POINTS");

	actionTree[SHAPE][SHAPE_PLANE].AddToTail("CHANGE_PLANE_NORMAL");
	
	actionTree[SHAPE][SHAPE_LINE].AddToTail("GET_LINE_ANGLE_Z"); 
	actionTree[SHAPE][SHAPE_LINE].AddToTail("GET_LINE_LENGTH");
	actionTree[SHAPE][SHAPE_LINE].AddToTail("GET_LINE_DIRECTION");

	actionTree[SHAPE][SHAPE_CIRCLE].AddToTail("GET_CIRCLE_LENGTH");
	actionTree[SHAPE][SHAPE_CIRCLE].AddToTail("GET_RADIUS_FROM_2_POINTS"); 
	actionTree[SHAPE][SHAPE_CIRCLE].AddToTail("GET_RADIUS_FROM_3_POINTS");
	actionTree[TRANSFORMATION][TRANSFORMATION_SCALABLE_OBJECT].AddToTail("GET_SCALE");
	actionTree[TRANSFORMATION][TRANSFORMATION_SCALABLE_OBJECT].AddToTail("REPLACE_SCALE");
	actionTree[TRANSFORMATION][TRANSFORMATION_SCALABLE_OBJECT].AddToTail("DISPLACE_SCALE");

	actionTree[TRANSFORMATION][TRANSFORMATION_TRANSLATABLE_OBJECT].AddToTail("GET_POSITION");
	actionTree[TRANSFORMATION][TRANSFORMATION_TRANSLATABLE_OBJECT].AddToTail("REPLACE_POSITION");
	actionTree[TRANSFORMATION][TRANSFORMATION_TRANSLATABLE_OBJECT].AddToTail("DISPLACE_POSITION");
	actionTree[TRANSFORMATION][TRANSFORMATION_TRANSLATABLE_OBJECT].AddToTail("MOVE_AROUND_PIVOT");

	actionTree[TRANSFORMATION][TRANSFORMATION_ROTATABLE_OBJECT].AddToTail("GET_ROTATION");
	actionTree[TRANSFORMATION][TRANSFORMATION_ROTATABLE_OBJECT].AddToTail("REPLACE_ROTATION");
	actionTree[TRANSFORMATION][TRANSFORMATION_ROTATABLE_OBJECT].AddToTail("DISPLACE_ROTATION");
	actionTree[TRANSFORMATION][TRANSFORMATION_ROTATABLE_OBJECT].AddToTail("CREATE_ROTATION");
	actionTree[TRANSFORMATION][TRANSFORMATION_ROTATABLE_OBJECT].AddToTail("CREATE_ROTATION_FROM_TO");
	actionTree[TRANSFORMATION][TRANSFORMATION_ROTATABLE_OBJECT].AddToTail("ROTATE_AROUND_PIVOT");

	actionTree[TRANSFORMATION][TRANSFORMATION_GRAPHICS_SURFACE].AddToTail("ENABLE_STATIC_OBJECT");
	
	actionTree[TRANSFORMATION][TRANSFORMATION_VARIABLE].AddToTail("GET_NORMALIZED_VECTOR");
	actionTree[TRANSFORMATION][TRANSFORMATION_VARIABLE].AddToTail("GET_VECTOR_LENGTH");
	actionTree[TRANSFORMATION][TRANSFORMATION_VARIABLE].AddToTail("GET_VECTOR_ANGLE");
	actionTree[TRANSFORMATION][TRANSFORMATION_VARIABLE].AddToTail("VECTOR_TO_SPHERICAL");
	actionTree[MATERIAL][MATERIAL_GRAPHICS_OBJECT].AddToTail("ENABLE_SHOW");

	actionTree[MATERIAL][MATERIAL_TEXTURE].AddToTail("CREATE_TEXTURE");
	actionTree[MATERIAL][MATERIAL_TEXTURE].AddToTail("GET_TEXTURE_SIZE");
	actionTree[MATERIAL][MATERIAL_TEXTURE].AddToTail("CREATE_BOX_TEXTURE_MAP");
	actionTree[MATERIAL][MATERIAL_TEXTURE].AddToTail("CHANGE_BOX_TEXTURE_MAP");

	actionTree[MATERIAL][MATERIAL_COLORABLE_OBJECT].AddToTail("GET_COLOR");
	actionTree[MATERIAL][MATERIAL_COLORABLE_OBJECT].AddToTail("CHANGE_COLOR");
	actionTree[MATERIAL][MATERIAL_COLORABLE_OBJECT].AddToTail("CHANGE_COLOR_COMPONENT");
	actionTree[MATERIAL][MATERIAL_COLORABLE_OBJECT].AddToTail("GET_COLOR_COMPONENT");

	actionTree[MATERIAL][MATERIAL_GRAPHICS_SURFACE].AddToTail("ENABLE_BACK_FACE_CULLING");

	actionTree[MATERIAL][MATERIAL_TEXTURING_SURFACE].AddToTail("CHANGE_FRONT_TEXTURE");
	actionTree[MATERIAL][MATERIAL_TEXTURING_SURFACE].AddToTail("CHANGE_BACK_TEXTURE");
	actionTree[MATERIAL][MATERIAL_TEXTURING_SURFACE].AddToTail("CHANGE_FRONT_TEXTURED_ANIMATION");
	actionTree[MATERIAL][MATERIAL_TEXTURING_SURFACE].AddToTail("CHANGE_BACK_TEXTURED_ANIMATION");
	actionTree[MATERIAL][MATERIAL_TEXTURING_SURFACE].AddToTail("ENABLE_TEXTURE_FLIP");
	actionTree[MATERIAL][MATERIAL_TEXTURING_SURFACE].AddToTail("SET_PUBLIC_TEXTURE");
	
	actionTree[MATERIAL][MATERIAL_GRAPHICS_CONTAINER].AddToTail("CHANGE_RENDER_TARGET");
	actionTree[VIEW][VIEW_SYSTEM].AddToTail("CHANGE_BACKGROUND_COLOR");
	actionTree[VIEW][VIEW_SYSTEM].AddToTail("ENABLE_Z_BUFFER");

	actionTree[VIEW][VIEW_CAMERA].AddToTail("CREATE_CAMERA");
	actionTree[VIEW][VIEW_CAMERA].AddToTail("PICK_CAMERA");
	actionTree[VIEW][VIEW_CAMERA].AddToTail("CHANGE_PROJECTION_MODE");
	actionTree[VIEW][VIEW_CAMERA].AddToTail("CHANGE_MOVEMENT_MODE");
	actionTree[VIEW][VIEW_CAMERA].AddToTail("CHANGE_ROTATION_MODE");
	actionTree[VIEW][VIEW_CAMERA].AddToTail("CHANGE_VIEWING_POINT");
	actionTree[VIEW][VIEW_CAMERA].AddToTail("GET_VIEWING_POINT");
	actionTree[VIEW][VIEW_CAMERA].AddToTail("CHANGE_DISTANCE_TO_VIEWING_POINT"); 
	actionTree[VIEW][VIEW_CAMERA].AddToTail("CAMERA_LOOK_AT"); 
	actionTree[VIEW][VIEW_CAMERA].AddToTail("CHANGE_ZOOM_FAR"); 
	actionTree[VIEW][VIEW_CAMERA].AddToTail("CHANGE_ZOOM_NEAR"); 
	actionTree[VIEW][VIEW_CAMERA].AddToTail("CHANGE_ZOOM_SPEED"); 
	actionTree[VIEW][VIEW_CAMERA].AddToTail("ENABLE_ZOOM"); 
	actionTree[VIEW][VIEW_CAMERA].AddToTail("GET_CAMERA_LOOK"); 
	actionTree[VIEW][VIEW_CAMERA].AddToTail("ENABLE_ROTATION"); 
	actionTree[VIEW][VIEW_CAMERA].AddToTail("ENABLE_ROTATION_RANGE"); 
	actionTree[VIEW][VIEW_CAMERA].AddToTail("CHANGE_ROTATION_RANGE"); 

	actionTree[VIEW][VIEW_GRAPHICS_CONTAINER].AddToTail("CHANGE_CAMERA");

	actionTree[VIEW][VIEW_SCENE].AddToTail("PICK_SCENE");
	actionTree[INTERPOLATION][INTERPOLATION_LERPING_OBJECT].AddToTail("CHANGE_FLOAT_LERP");

	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("CREATE_FLOAT_LERP");
	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("CHANGE_FLOAT_LERP_MODE");
	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("CHANGE_FLOAT_LERP_TARGET");
	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("CHANGE_FLOAT_LERP_SPEED");
	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("CHANGE_FLOAT_LERP_T");
	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("GET_FLOAT_LERP_T");
	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("GET_FLOAT_LERP_CURRENT");
	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("GET_FLOAT_LERP_DELTA");
	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("STREAMING_FLOAT_T");
	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("STREAMING_FLOAT_CURRENT");
	actionTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("STREAMING_FLOAT_DELTA");
	
	actionTree[INTERPOLATION][INTERPOLATION_QUATERNION_LERP].AddToTail("CREATE_QUATERNION_LERP");
	actionTree[INTERPOLATION][INTERPOLATION_QUATERNION_LERP].AddToTail("SET_TWO_QUATERNIONS");
	actionTree[INTERPOLATION][INTERPOLATION_QUATERNION_LERP].AddToTail("CHANGE_CONTROL_QUATERNIONS");
	actionTree[INTERPOLATION][INTERPOLATION_QUATERNION_LERP].AddToTail("GET_CONTROL_QUATERNIONS");
	actionTree[INTERPOLATION][INTERPOLATION_QUATERNION_LERP].AddToTail("GET_DATA_FROM_CONTROL_QUATERNION");
	actionTree[INTERPOLATION][INTERPOLATION_QUATERNION_LERP].AddToTail("SET_DATA_TO_CONTROL_QUATERNION");
	actionTree[INTERPOLATION][INTERPOLATION_QUATERNION_LERP].AddToTail("GET_QUATERNION_LERP_PATH");
	actionTree[INTERPOLATION][INTERPOLATION_QUATERNION_LERP].AddToTail("GET_QUATERNION_LERP_T");
	actionTree[INTERPOLATION][INTERPOLATION_QUATERNION_LERP].AddToTail("STREAMING_QUATERNION_CURRENT");
	actionTree[INTERPOLATION][INTERPOLATION_QUATERNION_LERP].AddToTail("STREAMING_QUATERNION_DELTA");

	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("CREATE_VECTOR_LERP");
	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("CHANGE_VECTOR_LERP_MODE");
	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("SET_TWO_VECTORS");
	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("CHANGE_CONTROL_VECTORS");
	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("GET_CONTROL_VECTORS");
	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("GET_DATA_FROM_CONTROL_VECTOR");
	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("SET_DATA_TO_CONTROL_VECTOR");
	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("GET_VECTOR_LERP_PATH");
	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("GET_VECTOR_LERP_T");
	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("STREAMING_VECTOR_CURRENT");
	actionTree[INTERPOLATION][INTERPOLATION_VECTOR_LERP].AddToTail("STREAMING_VECTOR_DELTA");
	actionTree[PLAYBACK][PLAYBACK_PLAYING_OBJECT].AddToTail("CHANGE_PLAYING_MODE");
	actionTree[PLAYBACK][PLAYBACK_PLAYING_OBJECT].AddToTail("CHANGE_PLAYING_STATE");
	actionTree[PLAYBACK][PLAYBACK_PLAYING_OBJECT].AddToTail("CHANGE_PLAYING_DURATION");
	actionTree[PLAYBACK][PLAYBACK_PLAYING_OBJECT].AddToTail("REPLAY");

	actionTree[PLAYBACK][PLAYBACK_ACTIVATABLE_OBJECT].AddToTail("CHANGE_ACTIVATION_LIMIT");
	actionTree[PLAYBACK][PLAYBACK_ACTIVATABLE_OBJECT].AddToTail("CHANGE_DEACTIVATION_MODE");
	actionTree[ANIMATION][ANIMATION_ABSTRACT_ANIMATION].AddToTail("CHANGE_ANIMATION_FRAME");
	actionTree[ANIMATION][ANIMATION_ABSTRACT_ANIMATION].AddToTail("GET_AMOUNT_OF_FRAMES");

	actionTree[ANIMATION][ANIMATION_TEXTURED_ANIMATION].AddToTail("CREATE_TEXTURED_ANIMATION");
	actionTree[ANIMATION][ANIMATION_TEXTURED_ANIMATION].AddToTail("CHANGE_ATLAS");
	actionTree[ANIMATION][ANIMATION_TEXTURED_ANIMATION].AddToTail("GET_FRAME_SIZE");
	actionTree[ANIMATION][ANIMATION_TEXTURED_ANIMATION].AddToTail("ENABLE_POSITION_OFFSET");
	actionTree[RANDOMIZATION][RANDOMIZATION_SYSTEM].AddToTail("CHANGE_RANDOM_SEED");

	actionTree[RANDOMIZATION][RANDOMIZATION_VARIABLE].AddToTail("GET_RANDOM_VALUE");

	actionTree[RANDOMIZATION][RANDOMIZATION_LIST].AddToTail("RANDOMIZE_LIST");
	actionTree[INPUT][INPUT_KEYBOARD_DEVICE].AddToTail("CREATE_KEYBOARD_EVENT");
	
	actionTree[INPUT][INPUT_PLAYER_CONTROL].AddToTail("CREATE_PLAYER_CONTROL");
	actionTree[INPUT][INPUT_PLAYER_CONTROL].AddToTail("ADD_INPUT_EVENT");
	actionTree[INPUT][INPUT_PLAYER_CONTROL].AddToTail("SET_ONE_INPUT_EVENT");
	actionTree[INPUT][INPUT_PLAYER_CONTROL].AddToTail("CLEAR_INPUT_EVENTS");
	actionTree[INPUT][INPUT_PLAYER_CONTROL].AddToTail("CHANGE_INPUT_TIMING");
	actionTree[TIME][TIME_SYSTEM].AddToTail("GET_SYSTEM_TIME");
	
	actionTree[TIME][TIME_TIMER].AddToTail("CREATE_TIMER");

	actionTree[TIME][TIME_LOGIC_OBJECT].AddToTail("ENABLE_FREEZE");
	actionTree[TIME][TIME_LOGIC_OBJECT].AddToTail("CHANGE_DELTA_SCALE");
	actionTree[FILE][FILE_SYSTEM].AddToTail("LOAD_ASSET");  

	actionTree[FILE][FILE_ABSTRACT_OBJECT].AddToTail("SAVE_TO_FILE");
	actionTree[CONTAINER][CONTAINER_VARIABLE].AddToTail("CREATE_VARIABLE");
	actionTree[CONTAINER][CONTAINER_VARIABLE].AddToTail("CHANGE_VALUE");
	actionTree[CONTAINER][CONTAINER_VARIABLE].AddToTail("CHANGE_VALUE_RANGE");
	actionTree[CONTAINER][CONTAINER_VARIABLE].AddToTail("CHANGE_VECTOR_COMPONENT");
	actionTree[CONTAINER][CONTAINER_VARIABLE].AddToTail("GET_VECTOR_COMPONENT");

	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("CREATE_LIST");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("CHANGE_ITEM_VALUE"); 
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("CHANGE_LIST_VALUE");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("REMOVE_FROM_LIST");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("ADD_TO_LIST");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("GET_DATA_FROM_ITEM");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("GET_AMOUNT_OF_ITEMS");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("COPY_LIST");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("CLEAR_LIST");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("GET_LIST_LENGTH");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("CHECK_LIST_EMPTY");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("ROTATE_LIST");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("INVERSE_LIST");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("GET_ITEM_INDEX");
	actionTree[CONTAINER][CONTAINER_LIST].AddToTail("SWAP_ITEMS");

	actionTree[CONTAINER][CONTAINER_LAYER].AddToTail("ADD_TO_LAYER");
	actionTree[CONTAINER][CONTAINER_LAYER].AddToTail("REMOVE_FROM_LAYER");

	actionTree[CONTAINER][CONTAINER_LOGIC_SCENE].AddToTail("ADD_TO_UPDATE_LIST");
	actionTree[CONTAINER][CONTAINER_LOGIC_SCENE].AddToTail("REMOVE_FROM_UPDATE_LIST");

	actionTree[CONTAINER][CONTAINER_GRAPHICS_SCENE].AddToTail("ADD_TO_RENDER_LIST");
	actionTree[CONTAINER][CONTAINER_GRAPHICS_SCENE].AddToTail("REMOVE_FROM_RENDER_LIST");

	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("CREATE_TILED_SPACE");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("CHANGE_TILED_SPACE_SIZE");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("CREATE_BRUSH");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("REMOVE_BRUSH");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("GET_TILE_CENTER");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("TILED_SPACE_FILL");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("CREATE_SOLID_TILE");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("CREATE_EMPTY_TILE");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("SET_PUBLIC_TILE");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("MOVE_TILE_FROM_TO");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("COPY_TILE");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("SWAP_TWO_TILES");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("CHECK_TILE_EXIST");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("CHECK_TILE_SOLID");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("CHECK_TILE_PASS");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("CHANGE_TILE_PASS");
	actionTree[CONTAINER][CONTAINER_TILED_SPACE].AddToTail("GET_BRUSH_OF_TILE");

	actionTree[CONTAINER][CONTAINER_SET].AddToTail("CREATE_SET");
	actionTree[CONTAINER][CONTAINER_SET].AddToTail("RESIZE_SET");
	actionTree[CONTAINER][CONTAINER_SET].AddToTail("GET_AMOUNT_OF_SLOTS");
	actionTree[CONTAINER][CONTAINER_SET].AddToTail("GET_DATA_FROM_SLOT");
	actionTree[CONTAINER][CONTAINER_SET].AddToTail("SET_DATA_TO_SLOT");
	actionTree[COLLISION][COLLISION_ABSTRACT_SURFACE].AddToTail("CHANGE_INTERSECTION_MODE");
	actionTree[COLLISION][COLLISION_ABSTRACT_SURFACE].AddToTail("CHECK_LINE_IN_SURFACE");

	actionTree[COLLISION][COLLISION_POLYGONAL_SURFACE].AddToTail("CHECK_LINE_IN_POLYGON");
	actionTree[COLLISION][COLLISION_POLYGONAL_SURFACE].AddToTail("CHANGE_HITBOX");
	actionTree[COLLISION][COLLISION_POLYGONAL_SURFACE].AddToTail("ENABLE_COLLISION_POLYGONS_INFO");
	actionTree[COLLISION][COLLISION_POLYGONAL_SURFACE].AddToTail("ENABLE_COLLISION_CHECKING");
	actionTree[COLLISION][COLLISION_POLYGONAL_SURFACE].AddToTail("CHANGE_COLLISION_GROUP");
	actionTree[COLLISION][COLLISION_POLYGONAL_SURFACE].AddToTail("CHECK_COLLISION");

	actionTree[COLLISION][COLLISION_TEXTURING_SURFACE].AddToTail("CHECK_LINE_IN_IMAGE");

	actionTree[COLLISION][COLLISION_CAMERA].AddToTail("CHECK_FRUSTUM_IN_QUAD");
	actionTree[COLLISION][COLLISION_CAMERA].AddToTail("GET_SELECT_RAY");
	actionTree[SCRIPT][SCRIPT_SYSTEM].AddToTail("CHECK_EQUALITY");

	actionTree[SCRIPT][SCRIPT_TRIGGER].AddToTail("CREATE_TRIGGER");
	actionTree[SCRIPT][SCRIPT_TRIGGER].AddToTail("ENABLE_TRIGGERS_BLOCK");
	for(int i = 0; i < MAX_COLUMNS; i++)
	{
		for(int j = 0; j < MAX_ROWS; j++)
		{
			AddItem(j, i);
		}
	}

	view = new QTableView(this);
	view->setSelectionBehavior(QAbstractItemView::SelectItems);
	view->setSelectionMode(QAbstractItemView::SingleSelection);
	
	view->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");
	view->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");


	view->setFixedSize(MAX_COLUMNS * 210 + 100 - (20 * 5)+157+45, (MAX_ROWS) * 32 - 11);

	setView(view);
	setModel(model);

	view->setColumnWidth(CATEGORY, 255);
	view->setColumnWidth(PROPERTY, 255);
	view->setColumnWidth(ACTION_TYPE, 300);
	view->verticalHeader()->setResizeMode(QHeaderView::Fixed);


	connect(view, SIGNAL(pressed(QModelIndex)), this, SLOT(ItemPressed(QModelIndex)));
	connect(view, SIGNAL(entered(QModelIndex)), this, SLOT(ItemEntered(QModelIndex)));
}


int ActionTypeEditor::GetSelectedCategory(void)const
{
	return selectedCategory;
}


int ActionTypeEditor::GetSelectedProperty(void)const
{
	return selectedProperty;
}


int ActionTypeEditor::GetSelectedActionType(void)const
{
	return selectedActionType;
}


bool ActionTypeEditor::SelectCategory(QString _actionType)
{
	CategorySelectionReset();
	for(int i = 0; i < MAX_ROWS; i++)
	{
		for(int j = 0; j < PROPERTIES_IN_CATEGORY; j++)
		{
			if(actionTree[i][j].Find(_actionType.toStdString()))
			{
				selectedCategory = i;
			
				UpdateCategorySelection();
				return true;
			}
		}
	}
	return false;
}



bool ActionTypeEditor::SelectProperty(QString _actionType)
{
	if(SelectCategory(_actionType))
	{
		PropertySelectionReset();
		for(int j = 0; j < PROPERTIES_IN_CATEGORY; j++)
		{
			if(actionTree[selectedCategory][j].Find(_actionType.toStdString()))
			{
				selectedProperty = j;
				
				UpdatePropertySelection();
				return true;
			}
		}
	}
	return false;
}



void ActionTypeEditor::ItemPressed(QModelIndex _index)
{
	setModelColumn(_index.column());
}


void ActionTypeEditor::CategorySelectionReset(void)
{
	selectedCategory = -1;
	selectedProperty = -1;
	selectedActionType = -1;

	for(int i = 0; i < MAX_ROWS; i++)
	{
		QStandardItem* it = model->item(i, CATEGORY);

		if(it)
		{
			QFont font("Verdana", 9, QFont::Normal, false);
			it->setFont(font);

			it->setForeground(QColor(0,0,0));
			it->setBackground(QColor(255,255,255));
		}
		for(int i = 0; i < MAX_ROWS; i++)
		{
			QStandardItem* it = model->item(i, PROPERTY);
			if(it) { it->setText(""); }
		}
	}
	PropertySelectionReset();
}



void ActionTypeEditor::PropertySelectionReset(void)
{
	selectedProperty = -1;
	selectedActionType = -1;

	for(int i = 0; i < MAX_ROWS; i++)
	{
		QStandardItem* it = model->item(i, PROPERTY);

		if(it)
		{
			QFont font("Verdana", 9, QFont::Normal, false);
			it->setFont(font);

			it->setForeground(QColor(0,0,0));
			it->setBackground(QColor(255,255,255));
		}
		for(int i = 0; i < MAX_ROWS; i++)
		{
			QStandardItem* it = model->item(i, ACTION_TYPE);
			if(it) { it->setText(""); }
		}
	}    
}




void ActionTypeEditor::ActionTypeSelectionReset(void)
{
	selectedActionType = -1;

	for(int i = 0; i < MAX_ROWS; i++)
	{
		QStandardItem* it = model->item(i, ACTION_TYPE);

		if(it)
		{
			QFont font("Verdana", 9, QFont::Normal, false);
			it->setFont(font);

			it->setForeground(QColor(0,0,0));
			it->setBackground(QColor(255,255,255));
		}
	}
}



bool ActionTypeEditor::IsAction(QString _type)
{
	for(int x = 0; x < MAX_ROWS; x++)
	{
		for(int y = 0; y < PROPERTIES_IN_CATEGORY; y++)
		{
			for(int i = 0; i < actionTree[x][y].GetSize(); i++)
			{
				if(actionTree[x][y].Get(i)->key == _type.toStdString())
				{
					return true;
				}
			}
		}
	}
	return false;
}



void ActionTypeEditor::UpdatePropertySelection(void)
{
	if(selectedProperty != -1)
	{
		QStandardItem* it = model->item(selectedProperty, PROPERTY);
		
		if(it)
		{
			QFont font("Verdana", 10, QFont::Bold, false);
			it->setFont(font);

			it->setForeground(QColor(255,255,255));
			it->setBackground(QColor(51,94,168));
			for(int i = 0; i < actionTree[selectedCategory][selectedProperty].GetSize(); i++)
			{
				ChangeItem(i, ACTION_TYPE, actionTree[selectedCategory][selectedProperty].Get(i)->key.c_str());
			}
		}
	}
}



void ActionTypeEditor::UpdateActionTypeSelection(void)
{
	if(selectedActionType != -1)
	{
		QStandardItem* it = model->item(selectedActionType, ACTION_TYPE);
		
		if(it)
		{
			QFont font("Verdana", 10, QFont::Bold, false);
			it->setFont(font);

			it->setForeground(QColor(255,255,255));
			it->setBackground(QColor(51,94,168));
		}
	}
}


void ActionTypeEditor::UpdateCategorySelection(void)
{
	if(selectedCategory != -1)
	{
		QStandardItem* it = model->item(selectedCategory, CATEGORY);
		
		if(it)
		{
			QFont font("Verdana", 10, QFont::Bold, false);
			it->setFont(font);

			it->setForeground(QColor(255,255,255));
			it->setBackground(QColor(51,94,168));
			for(int i = 0; i < propertyTree[selectedCategory].GetSize(); i++)
			{
				ChangeItem(i, PROPERTY, propertyTree[selectedCategory].Get(i)->key.c_str());
			}
		}
	}
}



void ActionTypeEditor::ItemEntered(QModelIndex _index)
{
	if(_index.column() == CATEGORY)
	{
		CategorySelectionReset();

		selectedCategory = _index.row();

		UpdateCategorySelection();
	}
	else if(_index.column() == PROPERTY)
	{ 
		if(selectedCategory != -1)
		{ 
			PropertySelectionReset();

			selectedProperty = _index.row();
			
			UpdatePropertySelection();
		}
	}
	else if(_index.column() == ACTION_TYPE)
	{
		if(selectedProperty != -1)
		{
			ActionTypeSelectionReset();

			selectedActionType = _index.row();

			UpdateActionTypeSelection();
		}
	}
}


void ActionTypeEditor::ChangeItem(int _row, int _column, QString _name)
{
	QStandardItem* it = model->item(_row, _column);

	if(it)
	{	
		it->setText(_name);
	}
}


void ActionTypeEditor::AddItem(int _row, int _column)
{
	if(model->item(_row, _column) == 0)
	{
		QStandardItem* it = new QStandardItem("");
		it->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		QFont font("Verdana", 9, QFont::Normal, false);
		it->setFont(font);

		model->setItem(_row, _column, it);
	}
}


void ActionTypeEditor::AddCategory(int _row, QString _name)
{
	QStandardItem* it = new QStandardItem(_name);
	it->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	QFont font("Verdana", 9, QFont::Normal, false);
	it->setFont(font);

	model->setItem(_row, CATEGORY, it);
}