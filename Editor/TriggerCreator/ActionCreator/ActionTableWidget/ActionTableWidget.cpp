#include "ActionTableWidget.h"
#include <Editor/Editor/Editor.h>
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Editor/TriggerCreator/EnumerationExprLineEdit/EnumerationExprLineEdit.h>
#include <Editor/TriggerCreator/BooleanExprLineEdit/BooleanExprLineEdit.h>
#include <Editor/TriggerCreator/EqualityExprLineEdit/EqualityExprLineEdit.h>
#include <Editor/TriggerCreator/StringExprLineEdit/StringExprLineEdit.h>
#include <Editor/TriggerCreator/ArithmeticExprLineEdit/ArithmeticExprLineEdit.h>
#include <Engine/Core/Parser/List/IntegerList/IntegerList.h>
#include <Engine/Core/Parser/List/FloatList/FloatList.h>
#include <Engine/Core/Parser/List/BooleanList/BooleanList.h>
#include <Engine/Core/Parser/List/StringList/StringList.h>
#include <Engine/Core/Parser/List/Vector2List/Vector2List.h>
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h>
#include <Engine/Core/Parser/List/QuaternionList/QuaternionList.h>
#include <Engine/Core/Parser/List/TranslatableObjectList/TranslatableObjectList.h>
#include <Engine/Core/Parser/List/RotatableObjectList/RotatableObjectList.h>
#include <Engine/Core/Parser/List/ScalableObjectList/ScalableObjectList.h>
#include <Engine/Core/Parser/List/ColorableObjectList/ColorableObjectList.h>
#include <Engine/Core/Parser/List/AbstractObjectList/AbstractObjectList.h>



ActionTableWidget::ActionTableWidget(QWidget* _parent): QTableWidget(MAX_ROWS, MAX_COLUMNS, _parent), autoName(true)
{
	blockSignals(true);
 
	CreateDictionary();

	CreateConstantsForComboBox();
	QStringList columnLabels;
	columnLabels.push_back("ACTION TYPE");
	columnLabels.push_back("TARGET");
	columnLabels.push_back("ARG1");
	columnLabels.push_back("ARG2");
	columnLabels.push_back("ARG3");
	columnLabels.push_back("ARG4");
	columnLabels.push_back("ARG5");
	columnLabels.push_back("NAME");
	columnLabels.push_back("ACTIVATION LIMIT");
	columnLabels.push_back("ARGS LOADING MODE");

	setVerticalHeaderLabels(columnLabels);
	QStringList rowLabels;
	rowLabels.push_back("VALUE");
	rowLabels.push_back("CONTENT");
	rowLabels.push_back("DATA TYPE");
	rowLabels.push_back("ARG MODE");

	setHorizontalHeaderLabels(rowLabels);

	
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	for(int i = 0; i < MAX_ROWS; ++i)
	{
		for(int j = 0; j < MAX_COLUMNS; ++j)
		{
			ItemAction* it = new ItemAction;
			it->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			QFont font("Verdana", 10, QFont::Normal, false);
			it->setFont(font);
			if(j == VALUE)
			{
				it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsDropEnabled);
			}
			else if(j == ARG_MODE)
			{
				it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
			}
			else
			{
				it->setFlags(Qt::ItemIsEnabled);
			}
			setItem(i, j, it);
		}
	}
	for(int i = 0; i < MAX_ROWS; i++)
	{
		setRowHeight(i, 25);
	}

	setSelectionMode(QAbstractItemView::NoSelection);

	verticalHeader()->setResizeMode(QHeaderView::Fixed);
	horizontalHeader()->setResizeMode(QHeaderView::Fixed);

	verticalHeader()->setStyleSheet("QHeaderView::section { background-color: gray; color: white; }");
	horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: gray; color: white; }");
	setColumnWidth(VALUE, 310+50+15+30);
	setColumnWidth(CONTENT, 150+57);
	setColumnWidth(DATA_TYPE, 150+58); 
	setColumnWidth(ARG_MODE, 150);


	SetDefaultState();
	item(ARGS_LOADING_MODE_PROPERTY, VALUE)->setToolTip("mode of loading args"); 
	SetCellText(ARGS_LOADING_MODE_PROPERTY, VALUE, "AUTO");
	item(ACTIVATION_LIMIT_PROPERTY, VALUE)->setToolTip("limit of activation"); 
	SetCellText(ACTIVATION_LIMIT_PROPERTY, VALUE, "-1"); 
	SetCellText(ACTIVATION_LIMIT_PROPERTY, CONTENT, "Count"); 
	SetCellText(ACTIVATION_LIMIT_PROPERTY, DATA_TYPE, "Integer");
	delegate = new UniversalDelegate(this);
	setItemDelegate(delegate);
	
	delegate->SetCellDelegate(indexFromItem(item(NAME_PROPERTY, VALUE)), UniversalDelegate::TEXT_EDITOR);
	
	delegate->SetCellDelegate(indexFromItem(item(ACTIVATION_LIMIT_PROPERTY, VALUE)), UniversalDelegate::LIMIT_EDITOR);

	delegate->SetCellDelegate(indexFromItem(item(ARGS_LOADING_MODE_PROPERTY, VALUE)), UniversalDelegate::LOADING_MODE_EDITOR);

	delegate->SetCellDelegate(indexFromItem(item(TYPE_PROPERTY, VALUE)), UniversalDelegate::ACTION_TYPE_EDITOR);
	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(UpdateRow(QTableWidgetItem*)));
    connect(this, SIGNAL(cellClicked(int, int)), this, SLOT(CellIsClicked(int, int)));

	setFixedSize(630+100+50+50+15+10-5+200-3+150+50+22-150, 270+10-5-2+17-10-7);

	blockSignals(false);
}


ActionTableWidget::~ActionTableWidget(void)
{
	delete delegate;
}


void ActionTableWidget::CellIsClicked(int _row, int _column)
{
	if(_row == TARGET_PROPERTY && _column == VALUE)
	{
		UpdateTargetPropertyStatus();
	}
	else if(_row == ARG1_PROPERTY && _column == VALUE)
	{
		UpdateArg1PropertyStatus();
	}
	else if(_row == ARG2_PROPERTY && _column == VALUE)
	{
		UpdateArg2PropertyStatus();
	}
	else if(_row == ARG3_PROPERTY && _column == VALUE)
	{
		UpdateArg3PropertyStatus();
	}
	else if(_row == ARG4_PROPERTY && _column == VALUE)
	{
		UpdateArg4PropertyStatus();
	}
	else if(_row == ARG5_PROPERTY && _column == VALUE)
	{
		UpdateArg5PropertyStatus();
	} 
	else if(_row == NAME_PROPERTY && _column == VALUE)
	{
		UpdateNamePropertyStatus();
	}
}


void ActionTableWidget::dragMoveEvent(QDragMoveEvent* _event)
{
	QModelIndex index = indexAt(_event->pos());

	QTableWidgetItem* it = itemFromIndex(index);

	if(it)
	{
		if(index.column() == VALUE)
		{
			if(index.row() == ARG1_PROPERTY)
			{
				if(IsEnabled(index.row()))
				{
					_event->accept();
					return;
				}
			}
			else if(index.row() == ARG2_PROPERTY)
			{
				if(IsEnabled(index.row()))
				{
					_event->accept();
					return;
				}
			}
			else if(index.row() == ARG3_PROPERTY)
			{
				if(IsEnabled(index.row()))
				{
					_event->accept();
					return;
				}
			}
			else if(index.row() == ARG4_PROPERTY)
			{
				if(IsEnabled(index.row()))
				{
					_event->accept();
					return;
				}
			}
			else if(index.row() == ARG5_PROPERTY)
			{
				if(IsEnabled(index.row()))
				{
					_event->accept();
					return;
				}
			}
			else if(index.row() == TARGET_PROPERTY)
			{
				if(IsEnabled(index.row()))
				{
					_event->accept();
					return;
				}
			}
		}
	}
	_event->ignore();
}


void ActionTableWidget::dragEnterEvent(QDragEnterEvent *_event)
{
	if(_event->mimeData()->hasFormat("text/uri-list")) 
	{ 	
		_event->acceptProposedAction();
	}
}
	

bool ActionTableWidget::IsEmpty(QString _expr)
{
	for(int i = 0; i < _expr.length(); i++)
	{
		if(QString(_expr.at(i)) != QString(" "))
		{
			return false;
		}
	}
	return true;
}



void ActionTableWidget::dropEvent(QDropEvent *_event)
{
	QList<QUrl> urlList;
	QString fileName;
	QFileInfo fileInfo;
	QString suffix;

	if(_event->mimeData()->hasUrls())
	{
		urlList = _event->mimeData()->urls();
		
		if(!urlList.isEmpty())
		{
			for(int i = 0; i < urlList.size(); i++)
			{
				fileName = urlList[i].toLocalFile();
					
				fileInfo.setFile(fileName);

				if(fileInfo.isFile())
				{
					fileName = fileInfo.completeBaseName();
					suffix = fileInfo.suffix();
				}
				else
				{
					suffix = AssetLibrary::_GetFileSuffixOfAssetType(AssetLibrary::_IsAssetExist(fileName.toStdString())).c_str();
				}

				QModelIndex index = indexAt(_event->pos());

				QTableWidgetItem* it = itemFromIndex(index);

				if(it)
				{
					CHANGE_NAME_DropEvent(it, index, suffix, fileName);
					CREATE_TIMER_DropEvent(it, index, suffix, fileName);
					CREATE_CAMERA_DropEvent(it, index, suffix, fileName);
					CREATE_MESH_DropEvent(it, index, suffix, fileName);
					CREATE_SET_DropEvent(it, index, suffix, fileName);
					CREATE_PLAYER_CONTROL_DropEvent(it, index, suffix, fileName);
					CREATE_TEXTURED_ANIMATION_DropEvent(it, index, suffix, fileName);
					CREATE_TEXTURE_DropEvent(it, index, suffix, fileName);
					GET_SYSTEM_TIME_DropEvent(it, index, suffix, fileName);
					CREATE_TRIGGER_DropEvent(it, index, suffix, fileName);
					CHANGE_VALUE_DropEvent(it, index, suffix, fileName);
					LOAD_ASSET_DropEvent(it, index, suffix, fileName);
					GET_NAME_DropEvent(it, index, suffix, fileName);
					CHECK_OBJECT_EXIST_DropEvent(it, index, suffix, fileName);
					CHECK_LIST_EMPTY_DropEvent(it, index, suffix, fileName);
					SEND_MESSAGE_DropEvent(it, index, suffix, fileName);
					DESTROY_OBJECT_DropEvent(it, index, suffix, fileName);
					SAVE_TO_FILE_DropEvent(it, index, suffix, fileName);
					SET_PRIVATE_ASSET_DropEvent(it, index, suffix, fileName);
					CHANGE_ANIMATION_FRAME_DropEvent(it, index, suffix, fileName);
					RESIZE_SET_DropEvent(it, index, suffix, fileName);
					CHANGE_ACTIVATION_LIMIT_DropEvent(it, index, suffix, fileName);
					CHANGE_DEACTIVATION_MODE_DropEvent(it, index, suffix, fileName);
					CREATE_EMPTY_TILE_DropEvent(it, index, suffix, fileName);
					REMOVE_BRUSH_DropEvent(it, index, suffix, fileName);
					GET_AMOUNT_OF_FRAMES_DropEvent(it, index, suffix, fileName);
					GET_FRAME_SIZE_DropEvent(it, index, suffix, fileName);
					CHANGE_ATLAS_DropEvent(it, index, suffix, fileName);
					GET_TEXTURE_SIZE_DropEvent(it, index, suffix, fileName);
					CHANGE_RANDOM_SEED_DropEvent(it, index, suffix, fileName);
					CHANGE_ITEM_VALUE_DropEvent(it, index, suffix, fileName);
					CHANGE_LIST_VALUE_DropEvent(it, index, suffix, fileName);
					ENABLE_ROTATION_RANGE_DropEvent(it, index, suffix, fileName);
					ADD_TO_LIST_DropEvent(it, index, suffix, fileName);
					TILED_SPACE_FILL_DropEvent(it, index, suffix, fileName);
					CHANGE_ROTATION_RANGE_DropEvent(it, index, suffix, fileName);
					ADD_INPUT_EVENT_DropEvent(it, index, suffix, fileName);
					SET_ONE_INPUT_EVENT_DropEvent(it, index, suffix, fileName);
					CREATE_TILED_SPACE_DropEvent(it, index, suffix, fileName);
					GET_DATA_FROM_ITEM_DropEvent(it, index, suffix, fileName);
					GET_ITEM_INDEX_DropEvent(it, index, suffix, fileName);
					SET_DATA_TO_SLOT_DropEvent(it, index, suffix, fileName);
					GET_DATA_FROM_SLOT_DropEvent(it, index, suffix, fileName);
					RANDOMIZE_LIST_DropEvent(it, index, suffix, fileName);
					GET_AMOUNT_OF_ITEMS_DropEvent(it, index, suffix, fileName);
					GET_AMOUNT_OF_SLOTS_DropEvent(it, index, suffix, fileName);
					COPY_LIST_DropEvent(it, index, suffix, fileName);
					CLEAR_LIST_DropEvent(it, index, suffix, fileName);
					CLEAR_INPUT_EVENTS_DropEvent(it, index, suffix, fileName);
					REPLAY_DropEvent(it, index, suffix, fileName);
					CREATE_LIST_DropEvent(it, index, suffix, fileName);
					CREATE_VARIABLE_DropEvent(it, index, suffix, fileName);
					CHANGE_BACKGROUND_COLOR_DropEvent(it, index, suffix, fileName);
					GET_SCALE_DropEvent(it, index, suffix, fileName);
					DISPLACE_SCALE_DropEvent(it, index, suffix, fileName);
					REPLACE_SCALE_DropEvent(it, index, suffix, fileName);
					GET_POSITION_DropEvent(it, index, suffix, fileName);
					REPLACE_POSITION_DropEvent(it, index, suffix, fileName);
					DISPLACE_POSITION_DropEvent(it, index, suffix, fileName);
					GET_ROTATION_DropEvent(it, index, suffix, fileName);
					REPLACE_ROTATION_DropEvent(it, index, suffix, fileName);
					DISPLACE_ROTATION_DropEvent(it, index, suffix, fileName);
					CHANGE_PLANE_NORMAL_DropEvent(it, index, suffix, fileName);
					CHANGE_MESH_DropEvent(it, index, suffix, fileName);
					CREATE_QUAD_DropEvent(it, index, suffix, fileName);
					CREATE_BOX_DropEvent(it, index, suffix, fileName);
					CHANGE_INTERSECTION_MODE_DropEvent(it, index, suffix, fileName);
					CHECK_PROPERTY_DropEvent(it, index, suffix, fileName);
					ENABLE_PROPERTY_DropEvent(it, index, suffix, fileName);
					ENABLE_BACK_FACE_CULLING_DropEvent(it, index, suffix, fileName);
					ENABLE_ZOOM_DropEvent(it, index, suffix, fileName);
					ENABLE_ROTATION_DropEvent(it, index, suffix, fileName);
					ENABLE_STATIC_OBJECT_DropEvent(it, index, suffix, fileName);
					ENABLE_POSITION_OFFSET_DropEvent(it, index, suffix, fileName);
					CHANGE_FRONT_TEXTURE_DropEvent(it, index, suffix, fileName);
					CHANGE_BACK_TEXTURE_DropEvent(it, index, suffix, fileName);
					CHANGE_FRONT_TEXTURED_ANIMATION_DropEvent(it, index, suffix, fileName);
					CHANGE_BACK_TEXTURED_ANIMATION_DropEvent(it, index, suffix, fileName);
					ENABLE_SHOW_CURSOR_DropEvent(it, index, suffix, fileName);
					ENABLE_Z_BUFFER_DropEvent(it, index, suffix, fileName);
					ENABLE_TRIGGERS_BLOCK_DropEvent(it, index, suffix, fileName);
					CREATE_KEYBOARD_EVENT_DropEvent(it, index, suffix, fileName);
					PICK_SCENE_DropEvent(it, index, suffix, fileName);
					ADD_TO_LAYER_DropEvent(it, index, suffix, fileName);
					REMOVE_FROM_LAYER_DropEvent(it, index, suffix, fileName);
					ADD_TO_UPDATE_LIST_DropEvent(it, index, suffix, fileName);
					REMOVE_FROM_UPDATE_LIST_DropEvent(it, index, suffix, fileName);
					ADD_TO_RENDER_LIST_DropEvent(it, index, suffix, fileName);
					REMOVE_FROM_RENDER_LIST_DropEvent(it, index, suffix, fileName);
					PICK_CAMERA_DropEvent(it, index, suffix, fileName);
					INVERSE_LIST_DropEvent(it, index, suffix, fileName);
					CHANGE_CAMERA_DropEvent(it, index, suffix, fileName);
					CHANGE_RENDER_TARGET_DropEvent(it, index, suffix, fileName);
					CHANGE_FLOAT_LERP_MODE_DropEvent(it, index, suffix, fileName);
					CHANGE_PROJECTION_MODE_DropEvent(it, index, suffix, fileName);
					CHANGE_MOVEMENT_MODE_DropEvent(it, index, suffix, fileName);
					CHANGE_ROTATION_MODE_DropEvent(it, index, suffix, fileName);
					GET_SELECT_RAY_DropEvent(it, index, suffix, fileName);
					CREATE_FLOAT_LERP_DropEvent(it, index, suffix, fileName);
					CREATE_QUATERNION_LERP_DropEvent(it, index, suffix, fileName);
					CREATE_VECTOR_LERP_DropEvent(it, index, suffix, fileName);
					CHANGE_FLOAT_LERP_SPEED_DropEvent(it, index, suffix, fileName);
					STREAMING_FLOAT_CURRENT_DropEvent(it, index, suffix, fileName);
					CHANGE_FLOAT_LERP_TARGET_DropEvent(it, index, suffix, fileName);
					STREAMING_FLOAT_DELTA_DropEvent(it, index, suffix, fileName);
					CHANGE_VECTOR_LERP_MODE_DropEvent(it, index, suffix, fileName);
					CHANGE_FLOAT_LERP_DropEvent(it, index, suffix, fileName);
					CHANGE_PLAYING_MODE_DropEvent(it, index, suffix, fileName);
					CHANGE_PLAYING_STATE_DropEvent(it, index, suffix, fileName);
					CHANGE_PLAYING_DURATION_DropEvent(it, index, suffix, fileName);
					CHANGE_INPUT_TIMING_DropEvent(it, index, suffix, fileName);
					STREAMING_VECTOR_DELTA_DropEvent(it, index, suffix, fileName);
					STREAMING_QUATERNION_DELTA_DropEvent(it, index, suffix, fileName);
					STREAMING_VECTOR_CURRENT_DropEvent(it, index, suffix, fileName);
					STREAMING_QUATERNION_CURRENT_DropEvent(it, index, suffix, fileName);
					STREAMING_FLOAT_T_DropEvent(it, index, suffix, fileName);
					CHECK_LINE_IN_IMAGE_DropEvent(it, index, suffix, fileName);
					CHECK_LINE_IN_POLYGON_DropEvent(it, index, suffix, fileName);
					ROTATE_AROUND_PIVOT_DropEvent(it, index, suffix, fileName);
					CHECK_LINE_IN_SURFACE_DropEvent(it, index, suffix, fileName);
					SET_TWO_VECTORS_DropEvent(it, index, suffix, fileName);
					SET_TWO_QUATERNIONS_DropEvent(it, index, suffix, fileName);
					CHANGE_VALUE_RANGE_DropEvent(it, index, suffix, fileName);
					CHANGE_CONTROL_VECTORS_DropEvent(it, index, suffix, fileName);
					CHANGE_CONTROL_QUATERNIONS_DropEvent(it, index, suffix, fileName);
					ENABLE_COLLISION_POLYGONS_INFO_DropEvent(it, index, suffix, fileName);
					ENABLE_COLLISION_CHECKING_DropEvent(it, index, suffix, fileName);
					CHANGE_COLLISION_GROUP_DropEvent(it, index, suffix, fileName);
					CHECK_COLLISION_DropEvent(it, index, suffix, fileName);
					SWAP_ITEMS_DropEvent(it, index, suffix, fileName);
					REMOVE_FROM_LIST_DropEvent(it, index, suffix, fileName);
					GET_RANDOM_VALUE_DropEvent(it, index, suffix, fileName);
					CREATE_ROTATION_FROM_TO_DropEvent(it, index, suffix, fileName);
					CREATE_ROTATION_DropEvent(it, index, suffix, fileName); 
					CHANGE_BOX_TEXTURE_MAP_DropEvent(it, index, suffix, fileName); 
					CREATE_BOX_TEXTURE_MAP_DropEvent(it, index, suffix, fileName); 
					GET_LINE_ANGLE_Z_DropEvent(it, index, suffix, fileName); 
					CHANGE_VIEWING_POINT_DropEvent(it, index, suffix, fileName); 
					CHANGE_DISTANCE_TO_VIEWING_POINT_DropEvent(it, index, suffix, fileName);
					CHANGE_ZOOM_NEAR_DropEvent(it, index, suffix, fileName);
					CHANGE_ZOOM_FAR_DropEvent(it, index, suffix, fileName);
					CHANGE_ZOOM_SPEED_DropEvent(it, index, suffix, fileName);
					CHANGE_HITBOX_DropEvent(it, index, suffix, fileName); 
					CAMERA_LOOK_AT_DropEvent(it, index, suffix, fileName); 
					CHANGE_VECTOR_COMPONENT_DropEvent(it, index, suffix, fileName);
					GET_VECTOR_COMPONENT_DropEvent(it, index, suffix, fileName);
					VECTOR_TO_SPHERICAL_DropEvent(it, index, suffix, fileName);
					GET_NORMALIZED_VECTOR_DropEvent(it, index, suffix, fileName);
					GET_VECTOR_ANGLE_DropEvent(it, index, suffix, fileName);
					GET_VECTOR_LENGTH_DropEvent(it, index, suffix, fileName);
					CHECK_FRUSTUM_IN_QUAD_DropEvent(it, index, suffix, fileName);
					GET_DATA_FROM_CONTROL_VECTOR_DropEvent(it, index, suffix, fileName);
					SET_DATA_TO_CONTROL_VECTOR_DropEvent(it, index, suffix, fileName);
					GET_LINE_LENGTH_DropEvent(it, index, suffix, fileName);
					GET_CIRCLE_LENGTH_DropEvent(it, index, suffix, fileName);
					CHANGE_COLOR_DropEvent(it, index, suffix, fileName);
					GET_COLOR_DropEvent(it, index, suffix, fileName); 
					CHANGE_COLOR_COMPONENT_DropEvent(it, index, suffix, fileName); 
					GET_COLOR_COMPONENT_DropEvent(it, index, suffix, fileName); 
					CHANGE_FLOAT_LERP_T_DropEvent(it, index, suffix, fileName); 
					GET_FLOAT_LERP_T_DropEvent(it, index, suffix, fileName); 
					GET_FLOAT_LERP_CURRENT_DropEvent(it, index, suffix, fileName); 
					GET_FLOAT_LERP_DELTA_DropEvent(it, index, suffix, fileName); 
					ENABLE_TEXTURE_FLIP_DropEvent(it, index, suffix, fileName); 
					GET_DATA_FROM_CONTROL_QUATERNION_DropEvent(it, index, suffix, fileName);
					SET_DATA_TO_CONTROL_QUATERNION_DropEvent(it, index, suffix, fileName);
					ENABLE_FREEZE_DropEvent(it, index, suffix, fileName);
					ROTATE_LIST_DropEvent(it, index, suffix, fileName);
					CHANGE_DELTA_SCALE_DropEvent(it, index, suffix, fileName);
					ENABLE_SHOW_DropEvent(it, index, suffix, fileName);
					GET_LINE_DIRECTION_DropEvent(it, index, suffix, fileName);
					MOVE_AROUND_PIVOT_DropEvent(it, index, suffix, fileName);
					CREATE_BRUSH_DropEvent(it, index, suffix, fileName);
					CHECK_TILE_EXIST_DropEvent(it, index, suffix, fileName);
					CHECK_TILE_SOLID_DropEvent(it, index, suffix, fileName);
					CHECK_TILE_PASS_DropEvent(it, index, suffix, fileName);
					CHANGE_TILE_PASS_DropEvent(it, index, suffix, fileName);
					CREATE_SOLID_TILE_DropEvent(it, index, suffix, fileName);
					GET_BRUSH_OF_TILE_DropEvent(it, index, suffix, fileName);
					CHANGE_TILED_SPACE_SIZE_DropEvent(it, index, suffix, fileName);
					SET_PUBLIC_TILE_DropEvent(it, index, suffix, fileName);
					SET_PUBLIC_TEXTURE_DropEvent(it, index, suffix, fileName);
					GET_TILE_CENTER_DropEvent(it, index, suffix, fileName);
					MOVE_TILE_FROM_TO_DropEvent(it, index, suffix, fileName);
					SWAP_TWO_TILES_DropEvent(it, index, suffix, fileName);
					COPY_TILE_DropEvent(it, index, suffix, fileName);
					GET_VIEWING_POINT_DropEvent(it, index, suffix, fileName);
					GET_CAMERA_LOOK_DropEvent(it, index, suffix, fileName);
					GET_VECTOR_LERP_PATH_DropEvent(it, index, suffix, fileName);
					CHANGE_STRIP_POINTS_DropEvent(it, index, suffix, fileName);
					GET_VECTOR_LERP_T_DropEvent(it, index, suffix, fileName);
					GET_QUATERNION_LERP_T_DropEvent(it, index, suffix, fileName);
					GET_QUATERNION_LERP_PATH_DropEvent(it, index, suffix, fileName);
					GET_LIST_LENGTH_DropEvent(it, index, suffix, fileName);
					GET_RADIUS_FROM_2_POINTS_DropEvent(it, index, suffix, fileName);
					GET_RADIUS_FROM_3_POINTS_DropEvent(it, index, suffix, fileName);
					GET_CONTROL_VECTORS_DropEvent(it, index, suffix, fileName);
					GET_CONTROL_QUATERNIONS_DropEvent(it, index, suffix, fileName);
					CHECK_EQUALITY_DropEvent(it, index, suffix, fileName);
				}
			}
		}
	}	
	_event->acceptProposedAction();
}



void ActionTableWidget::AbstractAnimationDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TEXTURED_ANIMATION_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}




void ActionTableWidget::TextureDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TEXTURE_2D_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}


void ActionTableWidget::TexturedAnimationDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TEXTURED_ANIMATION_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}


void ActionTableWidget::PlayerControlDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::PLAYER_CONTROL_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}

void ActionTableWidget::AtlasDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::ATLAS_2D_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void ActionTableWidget::MeshDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::MESH_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}




void ActionTableWidget::HitboxDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::HITBOX_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void ActionTableWidget::ListDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName, int32 _listType)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		AbstractList* list = AbstractList::_LoadFromFile(_fileName.toStdString());

		if(list)
		{
			if(list->GetValueType() == _listType)
			{
				_it->setText(_fileName);
			}
			delete list;
		}
	}
}



void ActionTableWidget::AbstractListDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::LIST_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}




void ActionTableWidget::AbstractVariableDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::VARIABLE_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void ActionTableWidget::ObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::OBJECT_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void ActionTableWidget::AnyAssetDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString()))
		{
			_it->setText(_fileName);
		}
	}
}

void ActionTableWidget::SceneDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::SCENE_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}




void ActionTableWidget::FloatLerpDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::FLOAT_LERP_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}




void ActionTableWidget::QuaternionLerpDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::QUATERNION_LERP_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}


void ActionTableWidget::VectorLerpDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::VECTOR_LERP_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void ActionTableWidget::CameraDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::CAMERA_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void ActionTableWidget::ActionDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::ACTION_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}


void ActionTableWidget::VariableDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName, int32 _variableType)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		if(_variableType == Variable<bool>::BOOLEAN_TYPE)
		{
			bool exist = false;
			if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TRIGGER_ASSET) ||
			   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::EVENT_ASSET))
			{
				exist = true;
			}
			else if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::VARIABLE_ASSET))
			{
				AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toStdString());

				if(variable)
				{
					if(variable->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
					{
						exist = true;
					}
					delete variable;
				}
			}

			if(exist)
			{
				_it->setText(_fileName);
			}
		}
		else
		{
			AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toStdString());

			if(variable)
			{
				if(variable->GetValueType() == _variableType)
				{
					_it->setText(_fileName);
				}
				delete variable;
			}
		}
	}
}


void ActionTableWidget::TranslatableObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::OBJECT_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::SCENE_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::CAMERA_ASSET))
		{
			exist = true;
		}
		else if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::VARIABLE_ASSET))
		{
			AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toStdString());

			if(variable)
			{
				if(variable->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
				{
					exist = true;
				}
				delete variable;
			}
		}

		if(exist)
		{
			_it->setText(_fileName);
		}
	}
}



void ActionTableWidget::ActivatableObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::EVENT_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TRIGGER_ASSET))
		{
			exist = true;
		}

		if(exist)
		{
			_it->setText(_fileName);
		}
	}
}


void ActionTableWidget::RotatableObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::OBJECT_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::SCENE_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::CAMERA_ASSET))
		{
			exist = true;
		}
		else if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::VARIABLE_ASSET))
		{
			AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toStdString());

			if(variable)
			{
				if(variable->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE)
				{
					exist = true;
				}
				delete variable;
			}
		}

		if(exist)
		{
			_it->setText(_fileName);
		}
	}
}



void ActionTableWidget::ColorableObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::OBJECT_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::SCENE_ASSET))
		{
			exist = true;
		}
		else if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::VARIABLE_ASSET))
		{
			AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toStdString());

			if(variable)
			{
				if(variable->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
				{
					exist = true;
				}
				delete variable;
			}
		}

		if(exist)
		{
			_it->setText(_fileName);
		}
	}
}




void ActionTableWidget::LerpingObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::QUATERNION_LERP_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::VECTOR_LERP_ASSET))
		{
			exist = true;
		}

		if(exist)
		{
			_it->setText(_fileName);
		}
	}
}



void ActionTableWidget::LogicObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::OBJECT_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TEXTURED_ANIMATION_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::FLOAT_LERP_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::SCENE_ASSET))
		{
			exist = true;
		}

		if(exist)
		{
			_it->setText(_fileName);
		}
	}
}




void ActionTableWidget::GraphicsObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::OBJECT_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::SCENE_ASSET))
		{
			exist = true;
		}

		if(exist)
		{
			_it->setText(_fileName);
		}
	}
}



void ActionTableWidget::PlayingObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TIMER_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TEXTURED_ANIMATION_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::FLOAT_LERP_ASSET))
		{
			exist = true;
		}

		if(exist)
		{
			_it->setText(_fileName);
		}
	}
}


void ActionTableWidget::ScalableObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == TARGET_PROPERTY)
	{
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::OBJECT_ASSET) ||
		   AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::SCENE_ASSET))
		{
			exist = true;
		}
		else if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::VARIABLE_ASSET))
		{
			AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toStdString());

			if(variable)
			{
				if(variable->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
				{
					exist = true;
				}
				delete variable;
			}
		}

		if(exist)
		{
			_it->setText(_fileName);
		}
	}
}


void ActionTableWidget::UpdateName(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		int32 errorCode;

		if((errorCode = BooleanExprParser::_IsNameValid(item(_argIndex, VALUE)->text().toStdString())) >= 0)
		{ 
			SetPropertyValid(_argIndex, false);
				
			QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(item(_argIndex, VALUE)->text().at(errorCode)) + "'";
			item(_argIndex, VALUE)->setToolTip(text);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}



void ActionTableWidget::UpdateAnyAsset(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString()))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}


 

void ActionTableWidget::UpdateAbstractAnimation(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::TEXTURED_ANIMATION_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}



void ActionTableWidget::UpdateTexture(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::TEXTURE_2D_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}




void ActionTableWidget::UpdateTexturedAnimation(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::TEXTURED_ANIMATION_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}




void ActionTableWidget::UpdateAtlas(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::ATLAS_2D_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}



void ActionTableWidget::UpdatePlayerControl(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::PLAYER_CONTROL_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}

void ActionTableWidget::UpdateHitbox(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::HITBOX_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}



void ActionTableWidget::UpdateMesh(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::MESH_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}


void ActionTableWidget::UpdateScalableObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::VARIABLE_ASSET))
		{
			UpdateVariable(_argIndex, Variable<Vector3>::VECTOR_3_TYPE);
		}
		else
		{
			if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::OBJECT_ASSET) ||
			   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::SCENE_ASSET))
			{
				SetPropertyValid(_argIndex, true);
			}
			else
			{
				SetPropertyValid(_argIndex, true);
				QString text = "Asset does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}



void ActionTableWidget::UpdateColorableObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::VARIABLE_ASSET))
		{
			UpdateVariable(_argIndex, Variable<Vector3>::VECTOR_3_TYPE); 
		}
		else
		{
			if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::OBJECT_ASSET) ||
			   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::SCENE_ASSET))
			{
				SetPropertyValid(_argIndex, true);
			}
			else
			{
				SetPropertyValid(_argIndex, true);
				QString text = "Asset does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}



void ActionTableWidget::UpdateLerpingObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::QUATERNION_LERP_ASSET) ||
		   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::VECTOR_LERP_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}



void ActionTableWidget::UpdateLogicObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::OBJECT_ASSET) ||
		   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::TEXTURED_ANIMATION_ASSET) ||
		   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::FLOAT_LERP_ASSET) ||
		   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::SCENE_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}



void ActionTableWidget::UpdatePlayingObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::TIMER_ASSET) ||
		   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::TEXTURED_ANIMATION_ASSET) ||
		   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::FLOAT_LERP_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}


void ActionTableWidget::UpdateGraphicsObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::OBJECT_ASSET) ||
		   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::SCENE_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}


void ActionTableWidget::UpdateTranslatableObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::VARIABLE_ASSET))
		{
			UpdateVariable(_argIndex, Variable<Vector3>::VECTOR_3_TYPE);
		}
		else
		{
			if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::OBJECT_ASSET) ||
			   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::SCENE_ASSET) ||
			   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::CAMERA_ASSET))
			{
				SetPropertyValid(_argIndex, true);
			}
			else
			{
				SetPropertyValid(_argIndex, true);
				QString text = "Asset does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}



void ActionTableWidget::UpdateActivatableObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::EVENT_ASSET) ||
		   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::TRIGGER_ASSET))
		{
			SetPropertyValid(_argIndex, true);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}



void ActionTableWidget::UpdateRotatableObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::VARIABLE_ASSET))
		{
			UpdateVariable(_argIndex, Variable<Quaternion>::QUATERNION_TYPE);
		}
		else
		{
			if(AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::OBJECT_ASSET) ||
			   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::SCENE_ASSET) ||
			   AssetLibrary::_IsAssetExist(item(_argIndex, VALUE)->text().toStdString(), AssetLibrary::CAMERA_ASSET))
			{
				SetPropertyValid(_argIndex, true);
			}
			else
			{
				SetPropertyValid(_argIndex, true);
				QString text = "Asset does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}


void ActionTableWidget::UpdateArithmeticExpr(int _argIndex, int32 _variableType)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		ArithmeticExprParser<int32> parser; 

		if(parser.Parse(item(_argIndex, VALUE)->text().toStdString()) != ArithmeticExprParser<int32>::NO_ERRORS)
		{
			SetPropertyValid(_argIndex, false);  
				
			QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
			item(_argIndex, VALUE)->setToolTip(text);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			if(!IsArithmeticExprCompute(item(_argIndex, VALUE)->text(), _variableType))
			{
				QString text = "One or more args does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
			} 
		}
	}
}



void ActionTableWidget::UpdateList(int _argIndex, int32 _listType)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::LIST_ASSET))
		{
			AbstractList* list = AbstractList::_LoadFromFile(fileName.toStdString());

			if(list)
			{
				if(list->GetValueType() == _listType)
				{
					exist = true;
				}
				delete list;
			}
		}
		
		if(!exist)
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}



void ActionTableWidget::UpdateAbstractList(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::LIST_ASSET))
		{
			exist = true;
		} 
		
		if(!exist)
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}


void ActionTableWidget::UpdateAbstractVariable(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::VARIABLE_ASSET))
		{
			exist = true;
		} 
		
		if(!exist)
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}


void ActionTableWidget::UpdateObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::OBJECT_ASSET))
		{
			exist = true;
		} 
		
		if(!exist)
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}



void ActionTableWidget::UpdateFloatLerp(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::FLOAT_LERP_ASSET))
		{
			exist = true;
		} 
		
		if(!exist)
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}



void ActionTableWidget::UpdateQuaternionLerp(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::QUATERNION_LERP_ASSET))
		{
			exist = true;
		} 
		
		if(!exist)
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}


void ActionTableWidget::UpdateVectorLerp(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::VECTOR_LERP_ASSET))
		{
			exist = true;
		} 
		
		if(!exist)
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}



void ActionTableWidget::UpdateScene(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::SCENE_ASSET))
		{
			exist = true;
		} 
		
		if(!exist)
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}




void ActionTableWidget::UpdateCamera(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::CAMERA_ASSET))
		{
			exist = true;
		} 
		
		if(!exist)
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}



void ActionTableWidget::UpdateAction(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::ACTION_ASSET))
		{
			exist = true;
		} 
		
		if(!exist)
		{
			SetPropertyValid(_argIndex, true);
			QString text = "Asset does not exist";
			item(_argIndex, VALUE)->setToolTip(text);
			item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			SetPropertyValid(_argIndex, true);
		}
	}
}


void ActionTableWidget::UpdateVariable(int _argIndex, int32 _variableType)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		if(_variableType == Variable<bool>::BOOLEAN_TYPE)
		{
			QString fileName = item(_argIndex, VALUE)->text();
			bool exist = false;
			if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::TRIGGER_ASSET) ||
			   AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::EVENT_ASSET))
			{
				exist = true;
			}
			else if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::VARIABLE_ASSET))
			{
				AbstractVariable* variable = AbstractVariable::_LoadFromFile(fileName.toStdString());

				if(variable)
				{
					if(variable->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
					{
						exist = true;
					}
					delete variable;
				}
			}
			
			if(!exist)
			{
				SetPropertyValid(_argIndex, true);
				QString text = "Asset does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
			else
			{
				SetPropertyValid(_argIndex, true);
			}
		}
		else
		{
			QString fileName = item(_argIndex, VALUE)->text();
			bool exist = false;
			if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::VARIABLE_ASSET))
			{
				AbstractVariable* variable = AbstractVariable::_LoadFromFile(fileName.toStdString());

				if(variable)
				{ 
					if(variable->GetValueType() == _variableType)
					{
						exist = true;
					}
					delete variable;
				}
			}
			
			if(!exist)
			{
				SetPropertyValid(_argIndex, true);
				QString text = "Asset does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
			else
			{
				SetPropertyValid(_argIndex, true);
			}
		}
	}
}


void ActionTableWidget::UpdateStringExpr(int _argIndex)
{ 
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		StringExprParser parser; 

		if(parser.Parse(item(_argIndex, VALUE)->text().toStdString()) != StringExprParser::NO_ERRORS)
		{
			SetPropertyValid(_argIndex, false);  
				
			QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
			item(_argIndex, VALUE)->setToolTip(text);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			if(!IsStringExprCompute(item(_argIndex, VALUE)->text()))
			{
				QString text = "One or more args does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
			} 
		}
	}
}


void ActionTableWidget::UpdateEnumerationExpr(int _argIndex)
{ 
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		EnumerationExprParser parser; 

		if(parser.Parse(item(_argIndex, VALUE)->text().toStdString()) != EnumerationExprParser::NO_ERRORS)
		{
			SetPropertyValid(_argIndex, false);  
				
			QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
			item(_argIndex, VALUE)->setToolTip(text);
		}  
		else
		{
			SetPropertyValid(_argIndex, true);
			if(!IsEnumerationExprCompute(item(_argIndex, VALUE)->text()))
			{
				QString text = "One or more args does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
			} 
		}
	}
}


void ActionTableWidget::UpdateBooleanExpr(int _argIndex)
{ 
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		BooleanExprParser parser; 

		if(parser.Parse(item(_argIndex, VALUE)->text().toStdString()) != BooleanExprParser::NO_ERRORS)
		{
			SetPropertyValid(_argIndex, false);  
				
			QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
			item(_argIndex, VALUE)->setToolTip(text);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			if(!IsBooleanExprCompute(item(_argIndex, VALUE)->text()))
			{
				QString text = "One or more args does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
			} 
		}
	}
}


void ActionTableWidget::UpdateEqualityExpr(int _argIndex)
{ 
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == TARGET_PROPERTY)
	{
		EqualityExprParser parser; 

		if(parser.Parse(item(_argIndex, VALUE)->text().toStdString()) != EqualityExprParser::NO_ERRORS)
		{
			SetPropertyValid(_argIndex, false);  
				
			QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
			item(_argIndex, VALUE)->setToolTip(text);
		}
		else
		{
			SetPropertyValid(_argIndex, true);
			if(!IsEqualityExprCompute(item(_argIndex, VALUE)->text()))
			{
				QString text = "One or more args does not exist";
				item(_argIndex, VALUE)->setToolTip(text);
				item(_argIndex, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
			} 
		}
	}
}



bool ActionTableWidget::EqualityOperationIsLast(QString _expr)
{
	if(!IsEmpty(_expr))
	{
		for(int i = _expr.length() - 1; i >= 0; i--)
		{
			if(QString(_expr.at(i)) != QString(" "))
			{ 
				if(QString(_expr.at(i)) == EqualityExprLineEdit::_GetAND() ||
				   QString(_expr.at(i)) == EqualityExprLineEdit::_GetOR() ||
				   QString(_expr.at(i)) == EqualityExprLineEdit::_GetXOR() ||
				   QString(_expr.at(i)) == EqualityExprLineEdit::_GetNOT() ||
				   QString(_expr.at(i)) == EqualityExprLineEdit::_GetLEFT_BRACKET() ||
				   QString(_expr.at(i)) == EqualityExprLineEdit::_GetEQUAL() ||
				   QString(_expr.at(i)) == EqualityExprLineEdit::_GetLESS() ||
				   QString(_expr.at(i)) == EqualityExprLineEdit::_GetGREATER())
				{
					return true;
				}
				else
				{ 
					return false; 
				} 
			}
		}
	}
	return false;
}


void ActionTableWidget::EqualityExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	bool exist = false;

	if(_suffix == AbstractVariable::_GetFileSuffix().c_str())
	{
		AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toStdString());

		if(variable)
		{
			if(variable->GetValueType() == Variable<int32>::INTEGER_TYPE ||
			   variable->GetValueType() == Variable<float>::FLOAT_TYPE ||
			   variable->GetValueType() == Variable<float>::BOOLEAN_TYPE)
			{
				exist = true;
			}
			delete variable;
		}
	}
	else if(_suffix == AbstractEvent::_GetFileSuffix().c_str() ||
		    _suffix == Trigger::_GetFileSuffix().c_str())
	{
		exist = true;
	}

	if(exist)
	{ 
		if(IsEmpty(_it->text()))
		{ 
			_it->setText(_fileName);
		}
		else
		{
			if(EqualityOperationIsLast(_it->text()))
			{
				while(QString(_it->text().at(_it->text().length() - 1)) == QString(" "))
				{
					QString str = _it->text();
					str.chop(1);
					_it->setText(str);
				}

				_it->setText(_it->text() + QString(" ") + _fileName);
			}
			else
			{
				while(QString(_it->text().at(_it->text().length() - 1)) == QString(" "))
				{
					QString str = _it->text();
					str.chop(1);
					_it->setText(str);
				}

				QString operation = QString(" ") + EqualityExprLineEdit::_GetEQUAL() + QString(" ");

				_it->setText(_it->text() + operation + _fileName);
			}
		} 
	}
}



bool ActionTableWidget::EnumerationOperationIsLast(QString _expr)
{
	if(!IsEmpty(_expr))
	{
		for(int i = _expr.length() - 1; i >= 0; i--)
		{
			if(QString(_expr.at(i)) != QString(" "))
			{
				if(QString(_expr.at(i)) == EnumerationExprLineEdit::_GetNEXT())
				{
					return true;
				}
				else
				{ 
					return false; 
				} 
			}
		}
	}
	return false;
}


void ActionTableWidget::EnumerationExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	bool exist = false;

	if(_suffix == AbstractAction::_GetFileSuffix().c_str())
	{
		exist = true;
	}
	if(exist)
	{
		if(IsEmpty(_it->text()))
		{ 
			_it->setText(_fileName);
		} 
		else
		{
			if(EnumerationOperationIsLast(_it->text()))
			{
				while(QString(_it->text().at(_it->text().length() - 1)) == QString(" "))
				{
					QString str = _it->text();
					str.chop(1);
					_it->setText(str);
				}

				_it->setText(_it->text() + QString(" ") + _fileName);
			}
			else
			{
				while(QString(_it->text().at(_it->text().length() - 1)) == QString(" "))
				{
					QString str = _it->text();
					str.chop(1);
					_it->setText(str);
				}

				QString operation = EnumerationExprLineEdit::_GetNEXT() + QString(" ");

				_it->setText(_it->text() + operation + _fileName);
			}
		}
	}
}


bool ActionTableWidget::BooleanOperationIsLast(QString _expr)
{
	if(!IsEmpty(_expr))
	{
		for(int i = _expr.length() - 1; i >= 0; i--)
		{
			if(QString(_expr.at(i)) != QString(" "))
			{ 
				if(QString(_expr.at(i)) == BooleanExprLineEdit::_GetAND() ||
				   QString(_expr.at(i)) == BooleanExprLineEdit::_GetOR() ||
				   QString(_expr.at(i)) == BooleanExprLineEdit::_GetXOR() ||
				   QString(_expr.at(i)) == BooleanExprLineEdit::_GetNOT() ||
				   QString(_expr.at(i)) == BooleanExprLineEdit::_GetLEFT_BRACKET())
				{
					return true;
				}
				else
				{ 
					return false; 
				} 
			}
		}
	}
	return false;
}


void ActionTableWidget::BooleanExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	bool exist = false;

	if(_suffix == AbstractEvent::_GetFileSuffix().c_str() ||
	   _suffix == Trigger::_GetFileSuffix().c_str())
	{
		exist = true;
	}
	else if(_suffix == AbstractVariable::_GetFileSuffix().c_str())
	{
		AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toAscii().data());
		
		if(variable)
		{
			if(variable->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				exist = true;
			}
			delete variable;
		}
	}
	if(exist)
	{
		if(IsEmpty(_it->text()))
		{ 
			_it->setText(_fileName);
		} 
		else
		{
			if(BooleanOperationIsLast(_it->text()))
			{
				while(QString(_it->text().at(_it->text().length() - 1)) == QString(" "))
				{
					QString str = _it->text();
					str.chop(1);
					_it->setText(str);
				}

				_it->setText(_it->text() + QString(" ") + _fileName);
			}
			else
			{
				while(QString(_it->text().at(_it->text().length() - 1)) == QString(" "))
				{
					QString str = _it->text();
					str.chop(1);
					_it->setText(str);
				}

				QString operation = QString(" ") + BooleanExprLineEdit::_GetAND() + QString(" ");

				_it->setText(_it->text() + operation + _fileName);
			}
		}
	}
}



bool ActionTableWidget::StringOperationIsLast(QString _expr)
{
	if(!IsEmpty(_expr))
	{
		for(int i = _expr.length() - 1; i >= 0; i--)
		{
			if(QString(_expr.at(i)) != QString(" "))
			{
				if(QString(_expr.at(i)) == StringExprLineEdit::_GetPLUS() ||
				   QString(_expr.at(i)) == StringExprLineEdit::_GetCONST())
				{
					return true;
				}
				else
				{ 
					return false; 
				} 
			}
		}
	}
	return false;
}



void ActionTableWidget::StringExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	bool exist = false;

	if(_suffix == AbstractVariable::_GetFileSuffix().c_str())
	{
		AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toStdString());

		if(variable)
		{
			if(variable->GetValueType() == Variable<int32>::INTEGER_TYPE ||
			   variable->GetValueType() == Variable<StringANSI>::STRING_TYPE)
			{
				exist = true;
			}
			delete variable;
		}
	}

	if(IsEmpty(_it->text()))
	{
		if(!exist)
		{
			_fileName = StringExprLineEdit::_GetCONST() + _fileName;
		}
		_it->setText(_fileName);
	}
	else
	{
		if(StringOperationIsLast(_it->text()))
		{
			while(QString(_it->text().at(_it->text().length() - 1)) == QString(" "))
			{
				QString str = _it->text();
				str.chop(1);
				_it->setText(str);
			}

			if(!exist)
			{
				_fileName = StringExprLineEdit::_GetCONST() + _fileName;
			}
			_it->setText(_it->text() + QString(" ") + _fileName);
		}
		else
		{
			while(QString(_it->text().at(_it->text().length() - 1)) == QString(" "))
			{
				QString str = _it->text();
				str.chop(1);
				_it->setText(str);
			}

			QString operation = QString(" ") + StringExprLineEdit::_GetPLUS() + QString(" ");

			if(!exist)
			{
				_fileName = StringExprLineEdit::_GetCONST() + _fileName;
			}
			_it->setText(_it->text() + operation + _fileName);
		}
	} 
}


void ActionTableWidget::SetCellDelegate(int _row, int _editor)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		QModelIndex index = indexFromItem(item(_row, VALUE));

		delegate->SetCellDelegate(index, _editor);
	}

}

 
void ActionTableWidget::CreateConstantsForComboBox(void)
{
	createVariableArgsComboList.push_back("INTEGER"); 
	createVariableArgsComboList.push_back("FLOAT");
	createVariableArgsComboList.push_back("BOOLEAN");
	createVariableArgsComboList.push_back("STRING");
	createVariableArgsComboList.push_back("VECTOR_2");
	createVariableArgsComboList.push_back("VECTOR_3");
	createVariableArgsComboList.push_back("QUATERNION");
	changeVectorArgsComboList.push_back("VECTOR_2");
	changeVectorArgsComboList.push_back("VECTOR_3");
	changeValueRangeArgsComboList.push_back("FLOAT");
	changeValueRangeArgsComboList.push_back("INTEGER");
	getRotationFromToArgsComboList.push_back("VECTOR_3");
	getRotationFromToArgsComboList.push_back("QUATERNION");
	createListArgsComboList.push_back("INTEGER"); 
	createListArgsComboList.push_back("FLOAT");
	createListArgsComboList.push_back("BOOLEAN");
	createListArgsComboList.push_back("STRING");
	createListArgsComboList.push_back("VECTOR_2");
	createListArgsComboList.push_back("VECTOR_3");
	createListArgsComboList.push_back("QUATERNION");
	createListArgsComboList.push_back("ABSTRACT_OBJECT");
	createListArgsComboList.push_back("TRANSLATABLE_OBJECT");
	createListArgsComboList.push_back("SCALABLE_OBJECT");
	createListArgsComboList.push_back("ROTATABLE_OBJECT");
	createListArgsComboList.push_back("COLORABLE_OBJECT");
	createListArgsComboList.push_back("SET");
	vectorLerpArgsComboList.push_back("VECTOR_3");
	vectorLerpArgsComboList.push_back("TRANSLATABLE_OBJECT");
	vectorLerpArgsComboList.push_back("SCALABLE_OBJECT");
	vectorLerpArgsComboList.push_back("COLORABLE_OBJECT");
	quaternionLerpArgsComboList.push_back("QUATERNION");
	quaternionLerpArgsComboList.push_back("ROTATABLE_OBJECT");
	floatLerpArgsComboList1.push_back("FLOAT");
	floatLerpArgsComboList1.push_back("INTEGER");
	colorTypeArgsComboList.push_back("RGB_COLOR");
	colorTypeArgsComboList.push_back("HSL_COLOR");
	floatLerpArgsComboList2.push_back("VARIABLE");
	floatLerpArgsComboList2.push_back("ALPHA_COLOR");
	floatLerpArgsComboList2.push_back("RED_COLOR");
	floatLerpArgsComboList2.push_back("GREEN_COLOR");
	floatLerpArgsComboList2.push_back("BLUE_COLOR");
	floatLerpArgsComboList2.push_back("HUE_COLOR");
	floatLerpArgsComboList2.push_back("SATURATION_COLOR");
	floatLerpArgsComboList2.push_back("LIGHTNESS_COLOR");
	floatLerpArgsComboList2.push_back("ANIMATION_FRAME");
	floatLerpArgsComboList2.push_back("DELTA_SCALE");
	colorComponentArgsComboList.push_back("ALPHA_COLOR");
	colorComponentArgsComboList.push_back("RED_COLOR");
	colorComponentArgsComboList.push_back("GREEN_COLOR");
	colorComponentArgsComboList.push_back("BLUE_COLOR");
	colorComponentArgsComboList.push_back("HUE_COLOR");
	colorComponentArgsComboList.push_back("SATURATION_COLOR");
	colorComponentArgsComboList.push_back("LIGHTNESS_COLOR");
	checkCollisionArgsComboList.push_back("GROUP-GROUP");
	checkCollisionArgsComboList.push_back("OBJECT-OBJECT");
	checkCollisionArgsComboList.push_back("OBJECT-GROUP");
	swapItemsArgsComboList.push_back("INDEX");
	swapItemsArgsComboList.push_back("ITEM");
	swapItemsArgsComboList.push_back("LIST");
	removeItemArgsComboList.push_back("INDEX");
	removeItemArgsComboList.push_back("ITEM");
	randomValueArgsComboList.push_back("FLOAT");
	randomValueArgsComboList.push_back("INTEGER");
	randomValueArgsComboList.push_back("BOOLEAN");
	argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList.push_back("KEY");
	argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList.push_back("VARIABLE");
	argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList.push_back("EXPRESSION");
	argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList.push_back("TEMPLATE");
	argMode_VARIABLE_EQUALITY_ComboList.push_back("VARIABLE");
	argMode_VARIABLE_EQUALITY_ComboList.push_back("EQUALITY");
	argMode_KEY_VARIABLE_TEMPLATE_ComboList.push_back("KEY");
	argMode_KEY_VARIABLE_TEMPLATE_ComboList.push_back("VARIABLE");
	argMode_KEY_VARIABLE_TEMPLATE_ComboList.push_back("TEMPLATE");
	argMode_VARIABLE_TEMPLATE_ComboList.push_back("VARIABLE");
	argMode_VARIABLE_TEMPLATE_ComboList.push_back("TEMPLATE");
	argMode_VARIABLE_EXPRESSION_ComboList.push_back("VARIABLE");
	argMode_VARIABLE_EXPRESSION_ComboList.push_back("EXPRESSION");
}



void ActionTableWidget::CreateDictionary(void)
{
	StringANSI path = QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/dictionary.txt").toAscii().data();
	
	File::_SetStringKey(path, true, "FIRST_NAME", "ACTION", "action");
	File::_SetStringKey(path, true, "LAST_NAME", "SEND_MESSAGE", "send_message");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_NAME", "change_name");
	File::_SetStringKey(path, true, "LAST_NAME", "DESTROY_OBJECT", "destroy_object");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_OBJECT_EXIST", "check_object_exist");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_NAME", "get_name");
	File::_SetStringKey(path, true, "LAST_NAME", "SET_PRIVATE_ASSET", "set_private_asset");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_PROPERTY", "check_property");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_PROPERTY", "enable_property");
	File::_SetStringKey(path, true, "LAST_NAME", "LOAD_ASSET", "load_asset");
	File::_SetStringKey(path, true, "LAST_NAME", "SAVE_TO_FILE", "save_to_file");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_TIMER", "create_timer");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_SYSTEM_TIME", "get_system_time");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_PLAYING_DURATION", "change_playing_duration");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_PLAYING_STATE", "change_playing_state");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_PLAYING_MODE", "change_playing_mode");
	File::_SetStringKey(path, true, "LAST_NAME", "REPLAY", "replay");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_KEYBOARD_EVENT", "create_keyboard_event");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_PLANE_NORMAL", "change_plane_normal");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_LINE_ANGLE_Z", "get_line_angle_z");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_LINE_LENGTH", "get_line_length");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_LINE_DIRECTION", "get_line_direction");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_CIRCLE_LENGTH", "get_circle_length");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_RADIUS_FROM_2_POINTS", "get_radius_from_2_points");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_RADIUS_FROM_3_POINTS", "get_radius_from_3_points");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_FLOAT_LERP", "create_float_lerp");
	File::_SetStringKey(path, true, "LAST_NAME", "STREAMING_FLOAT_CURRENT", "streaming_float_current");
	File::_SetStringKey(path, true, "LAST_NAME", "STREAMING_FLOAT_DELTA", "streaming_float_delta");
	File::_SetStringKey(path, true, "LAST_NAME", "STREAMING_FLOAT_T", "streaming_float_t");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_FLOAT_LERP_SPEED", "change_float_lerp_speed");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_FLOAT_LERP_TARGET", "change_float_lerp_target");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_FLOAT_LERP_MODE", "change_float_lerp_mode");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_FLOAT_LERP_T", "change_float_lerp_t");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_FLOAT_LERP_T", "get_float_lerp_t");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_FLOAT_LERP_CURRENT", "get_float_lerp_current");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_FLOAT_LERP_DELTA", "get_float_lerp_delta");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_QUATERNION_LERP", "create_quaternion_lerp");
	File::_SetStringKey(path, true, "LAST_NAME", "STREAMING_QUATERNION_CURRENT", "streaming_quaternion_current");
	File::_SetStringKey(path, true, "LAST_NAME", "STREAMING_QUATERNION_DELTA", "streaming_quaternion_delta");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_DATA_FROM_CONTROL_QUATERNION", "get_data_from_control_quaternion");
	File::_SetStringKey(path, true, "LAST_NAME", "SET_DATA_TO_CONTROL_QUATERNION", "set_data_to_control_quaternion");
	File::_SetStringKey(path, true, "LAST_NAME", "SET_TWO_QUATERNIONS", "set_two_quaternions");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_CONTROL_QUATERNIONS", "change_control_quaternions");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_QUATERNION_LERP_PATH", "get_quaternion_lerp_path");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_QUATERNION_LERP_T", "get_quaternion_lerp_t");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_CONTROL_QUATERNIONS", "get_control_quaternions");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_VECTOR_LERP", "create_vector_lerp");
	File::_SetStringKey(path, true, "LAST_NAME", "STREAMING_VECTOR_CURRENT", "streaming_vector_current");
	File::_SetStringKey(path, true, "LAST_NAME", "STREAMING_VECTOR_DELTA", "streaming_vector_delta");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_DATA_FROM_CONTROL_VECTOR", "get_data_from_control_vector");
	File::_SetStringKey(path, true, "LAST_NAME", "SET_DATA_TO_CONTROL_VECTOR", "set_data_to_control_vector");
	File::_SetStringKey(path, true, "LAST_NAME", "SET_TWO_VECTORS", "set_two_vectors");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_CONTROL_VECTORS", "change_control_vectors");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_VECTOR_LERP_MODE", "change_vector_lerp_mode");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_VECTOR_LERP_PATH", "get_vector_lerp_path");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_VECTOR_LERP_T", "get_vector_lerp_t");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_CONTROL_VECTORS", "get_control_vectors");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_STRIP_POINTS", "change_strip_points");
	File::_SetStringKey(path, true, "LAST_NAME", "REPLACE_ROTATION", "replace_rotation");
	File::_SetStringKey(path, true, "LAST_NAME", "DISPLACE_ROTATION", "displace_rotation");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_ROTATION", "get_rotation");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_ROTATION", "create_rotation");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_ROTATION_FROM_TO", "create_rotation_from_to");
	File::_SetStringKey(path, true, "LAST_NAME", "ROTATE_AROUND_PIVOT", "rotate_around_pivot");
	File::_SetStringKey(path, true, "LAST_NAME", "REPLACE_SCALE", "replace_scale");
	File::_SetStringKey(path, true, "LAST_NAME", "DISPLACE_SCALE", "displace_scale");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_SCALE", "get_scale");
	File::_SetStringKey(path, true, "LAST_NAME", "REPLACE_POSITION", "replace_position");
	File::_SetStringKey(path, true, "LAST_NAME", "DISPLACE_POSITION", "displace_position");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_POSITION", "get_position");
	File::_SetStringKey(path, true, "LAST_NAME", "MOVE_AROUND_PIVOT", "move_around_pivot");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_COLOR", "change_color");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_COLOR", "get_color");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_COLOR_COMPONENT", "change_color_component");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_COLOR_COMPONENT", "get_color_component");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_QUAD", "create_quad");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_BOX", "create_box");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_MESH", "create_mesh");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_SET", "create_set");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_AMOUNT_OF_SLOTS", "get_amount_of_slots");
	File::_SetStringKey(path, true, "LAST_NAME", "RESIZE_SET", "resize_set");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_DATA_FROM_SLOT", "get_data_from_slot");
	File::_SetStringKey(path, true, "LAST_NAME", "SET_DATA_TO_SLOT", "set_data_to_slot");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_LINE_IN_POLYGON", "check_line_in_polygon");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_MESH", "change_mesh");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_HITBOX", "change_hitbox");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_COLLISION_POLYGONS_INFO", "enable_collision_polygons_info");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_COLLISION_CHECKING", "enable_collision_checking");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_COLLISION_GROUP", "change_collision_group");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_COLLISION", "check_collision");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_LINE_IN_SURFACE", "check_line_in_surface");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_INTERSECTION_MODE", "change_intersection_mode");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_BACK_FACE_CULLING", "enable_back_face_culling");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_STATIC_OBJECT", "enable_static_object");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_RANDOM_VALUE", "get_random_value");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_RANDOM_SEED", "change_random_seed");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_TILED_SPACE", "create_tiled_space");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_BRUSH", "create_brush");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_SOLID_TILE", "create_solid_tile");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_TILED_SPACE_SIZE", "change_tiled_space_size");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_TILE_CENTER", "get_tile_center");
	File::_SetStringKey(path, true, "LAST_NAME", "TILED_SPACE_FILL", "tiled_space_fill");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_EMPTY_TILE", "create_empty_tile");
	File::_SetStringKey(path, true, "LAST_NAME", "SET_PUBLIC_TILE", "set_public_tile");
	File::_SetStringKey(path, true, "LAST_NAME", "REMOVE_BRUSH", "remove_brush");
	File::_SetStringKey(path, true, "LAST_NAME", "MOVE_TILE_FROM_TO", "move_tile_from_to");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_TILE_EXIST", "check_tile_exist");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_TILE_PASS", "check_tile_pass");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_TILE_PASS", "change_tile_pass");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_TILE_SOLID", "check_tile_solid");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_BRUSH_OF_TILE", "get_brush_of_tile");
	File::_SetStringKey(path, true, "LAST_NAME", "COPY_TILE", "copy_tile");
	File::_SetStringKey(path, true, "LAST_NAME", "SWAP_TWO_TILES", "swap_two_tiles");
	File::_SetStringKey(path, true, "LAST_NAME", "ADD_INPUT_EVENT", "add_input_event");
	File::_SetStringKey(path, true, "LAST_NAME", "SET_ONE_INPUT_EVENT", "set_one_input_event");
	File::_SetStringKey(path, true, "LAST_NAME", "CLEAR_INPUT_EVENTS", "clear_input_events");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_PLAYER_CONTROL", "create_player_control");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_INPUT_TIMING", "change_input_timing");
	File::_SetStringKey(path, true, "LAST_NAME", "ADD_TO_LIST", "add_to_list");
	File::_SetStringKey(path, true, "LAST_NAME", "REMOVE_FROM_LIST", "remove_from_list");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_DATA_FROM_ITEM", "get_data_from_item");
	File::_SetStringKey(path, true, "LAST_NAME", "COPY_LIST", "copy_list");
	File::_SetStringKey(path, true, "LAST_NAME", "RANDOMIZE_LIST", "randomize_list");
	File::_SetStringKey(path, true, "LAST_NAME", "CLEAR_LIST", "clear_list");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_LIST", "create_list");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_AMOUNT_OF_ITEMS", "get_amount_of_items");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_LIST_VALUE", "change_list_value");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ITEM_VALUE", "change_item_value");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_LIST_LENGTH", "get_list_length");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_LIST_EMPTY", "check_list_empty");
	File::_SetStringKey(path, true, "LAST_NAME", "ROTATE_LIST", "rotate_list");
	File::_SetStringKey(path, true, "LAST_NAME", "INVERSE_LIST", "inverse_list");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_ITEM_INDEX", "get_item_index");
	File::_SetStringKey(path, true, "LAST_NAME", "SWAP_ITEMS", "swap_items");
	File::_SetStringKey(path, true, "LAST_NAME", "ADD_TO_UPDATE_LIST", "add_to_update_list");
	File::_SetStringKey(path, true, "LAST_NAME", "REMOVE_FROM_UPDATE_LIST", "remove_from_update_list");
	File::_SetStringKey(path, true, "LAST_NAME", "ADD_TO_RENDER_LIST", "add_to_render_list");
	File::_SetStringKey(path, true, "LAST_NAME", "REMOVE_FROM_RENDER_LIST", "remove_from_render_list");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_CAMERA", "change_camera");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_RENDER_TARGET", "change_render_target");
	File::_SetStringKey(path, true, "LAST_NAME", "PICK_SCENE", "pick_scene");
	File::_SetStringKey(path, true, "LAST_NAME", "PICK_CAMERA", "pick_camera");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_PROJECTION_MODE", "change_projection_mode");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_MOVEMENT_MODE", "change_movement_mode");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ROTATION_MODE", "change_rotation_mode");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_SELECT_RAY", "get_select_ray");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_VIEWING_POINT", "change_viewing_point");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_DISTANCE_TO_VIEWING_POINT", "change_distance_to_viewing_point");
	File::_SetStringKey(path, true, "LAST_NAME", "CAMERA_LOOK_AT", "camera_look_at");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_FRUSTUM_IN_QUAD", "check_frustum_in_quad");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_VIEWING_POINT", "get_viewing_point");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ZOOM_FAR", "change_zoom_far");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ZOOM_NEAR", "change_zoom_near");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ZOOM_SPEED", "change_zoom_speed");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_ZOOM", "enable_zoom");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_CAMERA", "create_camera");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_CAMERA_LOOK", "get_camera_look");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_ROTATION", "enable_rotation");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_ROTATION_RANGE", "enable_rotation_range");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ROTATION_RANGE", "change_rotation_range");
	File::_SetStringKey(path, true, "LAST_NAME", "ADD_TO_LAYER", "add_to_layer");
	File::_SetStringKey(path, true, "LAST_NAME", "REMOVE_FROM_LAYER", "remove_from_layer");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_VARIABLE", "create_variable");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_VALUE", "change_value");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_VECTOR_COMPONENT", "change_vector_component");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_VECTOR_COMPONENT", "get_vector_component"); 
	File::_SetStringKey(path, true, "LAST_NAME", "GET_NORMALIZED_VECTOR", "get_normalized_vector"); 
	File::_SetStringKey(path, true, "LAST_NAME", "GET_VECTOR_ANGLE", "get_vector_angle"); 
	File::_SetStringKey(path, true, "LAST_NAME", "GET_VECTOR_LENGTH", "get_vector_length"); 
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_VALUE_RANGE", "change_value_range"); 
	File::_SetStringKey(path, true, "LAST_NAME", "VECTOR_TO_SPHERICAL", "vector_to_spherical"); 
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_EQUALITY", "check_equality");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_TRIGGER", "create_trigger");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_TRIGGERS_BLOCK", "enable_triggers_block");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ANIMATION_FRAME", "change_animation_frame");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_AMOUNT_OF_FRAMES", "get_amount_of_frames");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ACTIVATION_LIMIT", "change_activation_limit");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_DEACTIVATION_MODE", "change_deactivation_mode");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ATLAS", "change_atlas");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_FRAME_SIZE", "get_frame_size");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_POSITION_OFFSET", "enable_position_offset");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_TEXTURED_ANIMATION", "create_textured_animation");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_SHOW", "enable_show");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_FREEZE", "enable_freeze");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_DELTA_SCALE", "change_delta_scale");
	File::_SetStringKey(path, true, "LAST_NAME", "GET_TEXTURE_SIZE", "get_texture_size"); 
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_BOX_TEXTURE_MAP", "create_box_texture_map"); 
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_BOX_TEXTURE_MAP", "change_box_texture_map"); 
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_TEXTURE", "create_texture");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_FLOAT_LERP", "change_float_lerp");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_BACKGROUND_COLOR", "change_background_color");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_SHOW_CURSOR", "enable_show_cursor"); 
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_Z_BUFFER", "enable_z_buffer");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_FRONT_TEXTURED_ANIMATION", "change_front_textured_animation");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_BACK_TEXTURED_ANIMATION", "change_back_textured_animation");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_FRONT_TEXTURE", "change_front_texture");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_BACK_TEXTURE", "change_back_texture");
	File::_SetStringKey(path, true, "LAST_NAME", "CHECK_LINE_IN_IMAGE", "check_line_in_image");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_TEXTURE_FLIP", "enable_texturing_surface");
	File::_SetStringKey(path, true, "LAST_NAME", "SET_PUBLIC_TEXTURE", "set_public_texture");
}



QString ActionTableWidget::GetFirstName(void)
{
	StringANSI result;

	StringANSI path = QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/dictionary.txt").toAscii().data();
	if(File::_GetStringKey(path, true, "FIRST_NAME", "ACTION", result))
	{ 
		return result.c_str();
	}
	return "";
}


QString ActionTableWidget::GetLastName(QString _type)
{
	StringANSI result;

	StringANSI path = QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "ActionCreator/dictionary.txt").toAscii().data();
	if(File::_GetStringKey(path, true, "LAST_NAME", _type.toAscii().data(), result))
	{ 
		return result.c_str();
	}
	return "";
}



void ActionTableWidget::SetAutoName(bool _enable)
{
	autoName = _enable;
}


bool ActionTableWidget::IsAutoNameEnabled(void)const
{
	return autoName;
}


bool ActionTableWidget::IsAutoNameEnabled(int _row, int _column)
{
	if(autoName)
	{ 
		if(_row == TYPE_PROPERTY && _column == VALUE ||
		   _row == TARGET_PROPERTY && _column  == VALUE ||
		   _row == ARG1_PROPERTY && _column  == VALUE ||
		   _row == ARG2_PROPERTY && _column  == VALUE ||
		   _row == ARG3_PROPERTY && _column  == VALUE ||
		   _row == ARG4_PROPERTY && _column  == VALUE ||
		   _row == ARG5_PROPERTY && _column  == VALUE)
		{		
			return true;
		}
	}
	return false;
}


void ActionTableWidget::SetCellText(int _row, int _column, QString _value)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		if(_column >= 0 && _column < MAX_COLUMNS)
		{
			item(_row, _column)->setText(_value);
		}
	}
}



QString ActionTableWidget::GetCellText(int _row, int _column)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		if(_column >= 0 && _column < MAX_COLUMNS)
		{
			return item(_row, _column)->text();
		}
	}
	return "";
}




void ActionTableWidget::SetDefaultState(void)
{
	propertyValid[ARG1_PROPERTY] = false;
	propertyValid[ARG2_PROPERTY] = false;
	propertyValid[ARG3_PROPERTY] = false;
	propertyValid[ARG4_PROPERTY] = false;
	propertyValid[ARG5_PROPERTY] = false;
	propertyValid[TARGET_PROPERTY] = false;
	propertyEnabled[ARG1_PROPERTY] = true;
	propertyEnabled[ARG2_PROPERTY] = true;
	propertyEnabled[ARG3_PROPERTY] = true;
	propertyEnabled[ARG4_PROPERTY] = true;
	propertyEnabled[ARG5_PROPERTY] = true;
	propertyEnabled[TARGET_PROPERTY] = true;
	item(ARG1_PROPERTY, VALUE)->setToolTip("");
	item(ARG2_PROPERTY, VALUE)->setToolTip("");
	item(ARG3_PROPERTY, VALUE)->setToolTip("");
	item(ARG4_PROPERTY, VALUE)->setToolTip("");
	item(ARG5_PROPERTY, VALUE)->setToolTip("");
	item(TARGET_PROPERTY, VALUE)->setToolTip("");
	
	item(ARG1_PROPERTY, CONTENT)->setToolTip("");
	item(ARG2_PROPERTY, CONTENT)->setToolTip("");
	item(ARG3_PROPERTY, CONTENT)->setToolTip("");
	item(ARG4_PROPERTY, CONTENT)->setToolTip("");
	item(ARG5_PROPERTY, CONTENT)->setToolTip("");
	item(TARGET_PROPERTY, CONTENT)->setToolTip("");
	for(int i = 0; i < MAX_COLUMNS; i++)
	{
		SetCellText(ARG1_PROPERTY, i, "");
		SetCellText(ARG2_PROPERTY, i, "");
		SetCellText(ARG3_PROPERTY, i, "");
		SetCellText(ARG4_PROPERTY, i, "");
		SetCellText(ARG5_PROPERTY, i, "");
		SetCellText(TARGET_PROPERTY, i, ""); 
	}
}


 
void ActionTableWidget::SetDefaultProperty(int _index)
{
	if(_index == ARG1_PROPERTY ||
	   _index == ARG2_PROPERTY ||
	   _index == ARG3_PROPERTY ||
	   _index == ARG4_PROPERTY ||
	   _index == ARG5_PROPERTY ||
	   _index == TARGET_PROPERTY)
	{
		propertyValid[_index] = false;
		propertyEnabled[_index] = true;
		item(_index, CONTENT)->setToolTip(""); 
		item(_index, VALUE)->setToolTip("");
		for(int i = 0; i < MAX_COLUMNS - 1; i++)
		{
			SetCellText(_index, i, "");
		}
		
		item(_index, VALUE)->setBackground(QBrush(QColor(231,186,188)));
	} 
	else if(_index == ARGS_LOADING_MODE_PROPERTY)
	{
		SetCellText(ARGS_LOADING_MODE_PROPERTY, VALUE, "AUTO"); 
	}
	else if(_index == ACTIVATION_LIMIT_PROPERTY)
	{
		SetCellText(ACTIVATION_LIMIT_PROPERTY, VALUE, "-1"); 
	}
	else if(_index == NAME_PROPERTY)
	{
		SetCellText(NAME_PROPERTY, VALUE, ""); 
	}
	else if(_index == TYPE_PROPERTY)
	{
		SetCellText(TYPE_PROPERTY, VALUE, ""); 
	}
}


void ActionTableWidget::SetPropertyEnable(int _row, bool _enable)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		propertyEnabled[_row] = _enable;
		
		if(!_enable)
		{ 
			item(_row, CONTENT)->setToolTip(""); 
			item(_row, VALUE)->setToolTip("");
			SetCellText(_row, VALUE, "---"); 
			SetCellText(_row, ARG_MODE, "");
		}
	}
}


bool ActionTableWidget::IsPropertyValid(int _row)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		return propertyValid[_row];
	}
	return false;
}


void ActionTableWidget::SetPropertyValid(int _row, bool _valid)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		propertyValid[_row] = _valid;
		
		if(_valid)
		{
			item(_row, CONTENT)->setToolTip(""); 
			item(_row, VALUE)->setToolTip(""); 
			item(_row, VALUE)->setBackground(QBrush(QColor(255,255,255)));
		}
		else 
		{
			item(_row, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}


bool ActionTableWidget::IsValid(void)const
{
	if(propertyEnabled[TYPE_PROPERTY] && !propertyValid[TYPE_PROPERTY]) { return false; }
	if(propertyEnabled[NAME_PROPERTY] && !propertyValid[NAME_PROPERTY]) { return false; }
	if(propertyEnabled[ARG1_PROPERTY] && !propertyValid[ARG1_PROPERTY]) { return false; }
	if(propertyEnabled[ARG2_PROPERTY] && !propertyValid[ARG2_PROPERTY]) { return false; }
	if(propertyEnabled[ARG3_PROPERTY] && !propertyValid[ARG3_PROPERTY]) { return false; }
	if(propertyEnabled[ARG4_PROPERTY] && !propertyValid[ARG4_PROPERTY]) { return false; }
	if(propertyEnabled[ARG5_PROPERTY] && !propertyValid[ARG5_PROPERTY]) { return false; }
	if(propertyEnabled[TARGET_PROPERTY] && !propertyValid[TARGET_PROPERTY]) { return false; }
	
	return true;
}



bool ActionTableWidget::IsStringExprCompute(QString _text)
{
	StringExprParser parser;
	SimpleList<StringExprParser::Token> tokens;
	
	if(parser.Parse(_text.toStdString()) == StringExprParser::NO_ERRORS)
	{
		tokens = parser.GetTokens();
	}
	else { return false; }

	if(tokens.IsNotEmpty())
	{
		for(int i = 0; i < tokens.GetSize(); i++)
		{  
			SimpleList<StringExprParser::Token>::Node* token = tokens.Get(i);

			if(token->key.GetType() == StringExprParser::Token::STRING)
			{
				if(AssetLibrary::_IsAssetExist(token->key.GetValue()) != AssetLibrary::UNKNOWN_ASSET)
				{}
				else { return false; } 
			}
			if(token->key.GetType() == StringExprParser::Token::VARIABLE)
			{
				AbstractVariable* variable = AbstractVariable::_LoadFromFile(token->key.GetValue());

				if(variable)
				{
					int type = variable->GetValueType();
					delete variable;

					if(type == Variable<int32>::INTEGER_TYPE ||
					   type == Variable<StringANSI>::STRING_TYPE)
					{}
					else { return false; } 
				}
				else { return false; }
			}
		}
	}
	return true;
}



bool ActionTableWidget::IsBooleanExprCompute(QString _text)
{
	BooleanExprParser parser;
	SimpleList<BooleanExprParser::Token> tokens;
	
	if(parser.Parse(_text.toStdString()) == BooleanExprParser::NO_ERRORS)
	{
		tokens = parser.GetTokens();
	}
	else { return false; }

	if(tokens.IsNotEmpty())
	{
		for(int i = 0; i < tokens.GetSize(); i++)
		{
			SimpleList<BooleanExprParser::Token>::Node* token = tokens.Get(i);

			if(token->key.GetType() == BooleanExprParser::Token::VARIABLE)
			{
				if(AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::EVENT_ASSET) ||
				   AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::TRIGGER_ASSET))
				{}
				else if(AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::VARIABLE_ASSET))
				{
					AbstractVariable* variable = AbstractVariable::_LoadFromFile(token->key.GetValue());
						
					if(variable)
					{
						int type = variable->GetValueType();
						delete variable;

						if(type == Variable<bool>::BOOLEAN_TYPE)
						{}
						else { return false; } 
					}
					else { return false; }  
				}
				else { return false; } 
			}
		}
	}
	return true;
}


bool ActionTableWidget::IsEnumerationExprCompute(QString _text)
{
	EnumerationExprParser parser;
	SimpleList<EnumerationExprParser::Token> tokens;
	
	if(parser.Parse(_text.toStdString()) == EnumerationExprParser::NO_ERRORS)
	{
		tokens = parser.GetTokens();
	}
	else { return false; }

	if(tokens.IsNotEmpty())
	{
		for(int i = 0; i < tokens.GetSize(); i++)
		{
			SimpleList<EnumerationExprParser::Token>::Node* token = tokens.Get(i);

			if(token->key.GetType() == EnumerationExprParser::Token::VARIABLE)
			{
				if(AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::ACTION_ASSET))
				{}
				else { return false; } 
			}
		}
	}
	return true;
}


bool ActionTableWidget::IsEqualityExprCompute(QString _text)
{
	EqualityExprParser parser;
	SimpleList<EqualityExprParser::Token> tokens;
	
	if(parser.Parse(_text.toStdString()) == EqualityExprParser::NO_ERRORS)
	{
		tokens = parser.GetTokens();
	}
	else { return false; }

	if(tokens.IsNotEmpty())
	{
		for(int i = 0; i < tokens.GetSize(); i++)
		{
			SimpleList<EqualityExprParser::Token>::Node* token = tokens.Get(i);

			if(token->key.GetType() == EqualityExprParser::Token::VARIABLE)
			{
				if(AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::VARIABLE_ASSET) ||
				   AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::EVENT_ASSET) ||
				   AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::TRIGGER_ASSET))
				{}
				else { return false; } 
			}
		}
	}
	return true;
}




int32 ActionTableWidget::GetVariableType(QString _variableType)
{
	if(_variableType == "INTEGER")
	{
		return Variable<int32>::INTEGER_TYPE;
	}
	else if(_variableType == "FLOAT")
	{
		return Variable<float>::FLOAT_TYPE;
	}
	else if(_variableType == "VECTOR_2")
	{
		return Variable<Vector2>::VECTOR_2_TYPE;
	}
	else if(_variableType == "VECTOR_3")
	{
		return Variable<Vector3>::VECTOR_3_TYPE;
	}
	else if(_variableType == "QUATERNION")
	{
		return Variable<Quaternion>::QUATERNION_TYPE;
	}
	else if(_variableType == "STRING")
	{
		return Variable<StringANSI>::STRING_TYPE;
	}
	else if(_variableType == "BOOLEAN")
	{
		return Variable<bool>::BOOLEAN_TYPE;
	}
	return 0;
}




int32 ActionTableWidget::GetListType(QString _listType)
{
	if(_listType == "INTEGER")
	{
		return List<int32>::INTEGER_TYPE;
	}
	else if(_listType == "FLOAT")
	{
		return List<float>::FLOAT_TYPE;
	}
	else if(_listType == "VECTOR_2")
	{
		return List<Vector2>::VECTOR_2_TYPE;
	}
	else if(_listType == "VECTOR_3")
	{
		return List<Vector3>::VECTOR_3_TYPE;
	}
	else if(_listType == "QUATERNION")
	{
		return List<Quaternion>::QUATERNION_TYPE;
	}
	else if(_listType == "STRING")
	{
		return List<StringANSI>::STRING_TYPE;
	}
	else if(_listType == "BOOLEAN")
	{
		return List<bool>::BOOLEAN_TYPE;
	}
	else if(_listType == "TRANSLATABLE_OBJECT")
	{
		return List<TranslatableObject>::TRANSLATABLE_OBJECT_TYPE;
	}
	else if(_listType == "SCALABLE_OBJECT")
	{
		return List<ScalableObject>::SCALABLE_OBJECT_TYPE;
	}
	else if(_listType == "ROTATABLE_OBJECT")
	{
		return List<RotatableObject>::ROTATABLE_OBJECT_TYPE;
	}
	else if(_listType == "COLORABLE_OBJECT")
	{
		return List<ColorableObject>::COLORABLE_OBJECT_TYPE;
	}
	else if(_listType == "SET")
	{
		return List<Set>::SET_TYPE;
	}
	return 0;
}


void ActionTableWidget::ArithmeticExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName, QString _variableType)
{
	bool exist = false;
	int32 variableType = 0;
    AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toStdString());

	if(variable)
	{
		variableType = variable->GetValueType();
		delete variable;
	}

	if(_variableType == "INTEGER" ||
	   _variableType == "FLOAT")
	{
		if(variableType == Variable<int32>::INTEGER_TYPE ||
		   variableType == Variable<float>::FLOAT_TYPE)
		{
			exist = true;
		}
	}
	else if(_variableType == "VECTOR_2")
	{
		if(variableType == Variable<Vector2>::VECTOR_2_TYPE ||
	       variableType == Variable<int32>::INTEGER_TYPE ||
		   variableType == Variable<float>::FLOAT_TYPE)
		{
			exist = true;
		}
	}
	else if(_variableType == "VECTOR_3")
	{
		if(variableType == Variable<Vector3>::VECTOR_3_TYPE ||
		   variableType == Variable<int32>::INTEGER_TYPE ||
		   variableType == Variable<float>::FLOAT_TYPE)
		{
			exist = true;
		}
	}
	else if(_variableType == "QUATERNION")
	{
		if(variableType == Variable<Quaternion>::QUATERNION_TYPE ||
		   variableType == Variable<Vector3>::VECTOR_3_TYPE)
		{
			exist = true;
		}
	}

	if(exist)
	{ 
		if(IsEmpty(_it->text()))
		{ 
			_it->setText(_fileName);
		}
		else
		{
			if(ArithmeticOperationIsLast(_it->text()))
			{
				while(QString(_it->text().at(_it->text().length() - 1)) == QString(" "))
				{
					QString str = _it->text();
					str.chop(1);
					_it->setText(str);
				}

				_it->setText(_it->text() + QString(" ") + _fileName);
			}
			else
			{
				while(QString(_it->text().at(_it->text().length() - 1)) == QString(" "))
				{
					QString str = _it->text();
					str.chop(1);
					_it->setText(str);
				}

				QString operation = QString(" ") + ArithmeticExprLineEdit::_GetPLUS() + QString(" ");

				_it->setText(_it->text() + operation + _fileName);
			}
		} 
	}
}



bool ActionTableWidget::ArithmeticOperationIsLast(QString _expr)
{
	if(!IsEmpty(_expr))
	{
		for(int i = _expr.length() - 1; i >= 0; i--)
		{
			if(QString(_expr.at(i)) != QString(" "))
			{ 
				if(QString(_expr.at(i)) == ArithmeticExprLineEdit::_GetDIVIDE() ||
				   QString(_expr.at(i)) == ArithmeticExprLineEdit::_GetMULTIPLY() ||
				   QString(_expr.at(i)) == ArithmeticExprLineEdit::_GetPLUS() ||
				   QString(_expr.at(i)) == ArithmeticExprLineEdit::_GetMINUS() ||
				   QString(_expr.at(i)) == ArithmeticExprLineEdit::_GetLEFT_BRACKET())
				{
					return true;
				}
				else
				{ 
					return false; 
				} 
			}
		}
	}
	return false;
}


bool ActionTableWidget::IsArithmeticExprCompute(QString _text, int32 _variableType)
{
	ArithmeticExprParser<int32> parser;
	SimpleList<ArithmeticExprParser<int32>::Token> tokens;
	
	if(parser.Parse(_text.toStdString()) == ArithmeticExprParser<int32>::NO_ERRORS)
	{
		tokens = parser.GetTokens();
	}
	else { return false; }

	if(tokens.IsNotEmpty())
	{
		for(int i = 0; i < tokens.GetSize(); i++)
		{
			SimpleList<ArithmeticExprParser<int32>::Token>::Node* token = tokens.Get(i);

			if(token->key.GetType() == ArithmeticExprParser<int32>::Token::VARIABLE)
			{
				AbstractVariable* variable = AbstractVariable::_LoadFromFile(token->key.GetValue());

				if(variable)
				{
					int type = variable->GetValueType();
					delete variable;

					if(type == _variableType)
					{}
					else if(_variableType == Variable<int32>::INTEGER_TYPE)
					{
						if(type == Variable<float>::FLOAT_TYPE)
						{}
						else { return false; } 
					}
					else if(_variableType == Variable<float>::FLOAT_TYPE)
					{
						if(type == Variable<int32>::INTEGER_TYPE)
						{}
						else { return false; } 
					}
					else if(_variableType == Variable<Vector2>::VECTOR_2_TYPE)
					{
						if(type == Variable<int32>::INTEGER_TYPE ||
						   type == Variable<float>::FLOAT_TYPE)
						{}
						else { return false; } 
					}
					else if(_variableType == Variable<Vector3>::VECTOR_3_TYPE)
					{
						if(type == Variable<int32>::INTEGER_TYPE ||
						   type == Variable<float>::FLOAT_TYPE)
						{}
						else { return false; } 
					}
					else if(_variableType == Variable<Quaternion>::QUATERNION_TYPE)
					{
						if(type == Variable<Vector3>::VECTOR_3_TYPE)
						{}
						else { return false; } 
					}
					else { return false; } 
				}
				else { return false; }
			}
		}
	}

	return true;
}





void ActionTableWidget::UpdateNamePropertyStatus(void)
{
	if(IsEnabled(NAME_PROPERTY))
	{
		if(item(NAME_PROPERTY, VALUE)->text().length() > 0)
		{
			int32 errorCode;
			if((errorCode = BooleanExprParser::_IsNameValid(item(NAME_PROPERTY, VALUE)->text().toStdString())) >= 0)
			{				
				QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(item(NAME_PROPERTY, VALUE)->text().at(errorCode)) + "'";
				item(NAME_PROPERTY, VALUE)->setToolTip(text); 
				
				SetPropertyValid(NAME_PROPERTY, false);
				item(NAME_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
			else
			{
				item(NAME_PROPERTY, VALUE)->setToolTip("");

				SetPropertyValid(NAME_PROPERTY, true);
				item(NAME_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
			}
		}
		else
		{ 
			SetPropertyValid(NAME_PROPERTY, false);
			item(NAME_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}


void ActionTableWidget::UpdateTypePropertyStatus(void)
{
	if(IsEnabled(TYPE_PROPERTY))
	{
		if(item(TYPE_PROPERTY, VALUE)->text().length() > 0)
		{
			SetPropertyValid(TYPE_PROPERTY, true);
		}
		else 
		{ 
			SetPropertyValid(TYPE_PROPERTY, false);
		}
	}
}


void ActionTableWidget::UpdateArg1KeyPropertyStatus(void)
{
	if(CHANGE_NAME_UpdateArg1KeyPropertyStatus() ||
	   CREATE_TIMER_UpdateArg1KeyPropertyStatus() ||
	   CREATE_CAMERA_UpdateArg1KeyPropertyStatus() ||
	   CREATE_MESH_UpdateArg1KeyPropertyStatus() ||
	   CREATE_SET_UpdateArg1KeyPropertyStatus() ||
	   CREATE_PLAYER_CONTROL_UpdateArg1KeyPropertyStatus() ||
	   CREATE_TEXTURED_ANIMATION_UpdateArg1KeyPropertyStatus() ||
	   CREATE_TEXTURE_UpdateArg1KeyPropertyStatus() ||
	   CREATE_TILED_SPACE_UpdateArg1KeyPropertyStatus() ||
	   CREATE_TRIGGER_UpdateArg1KeyPropertyStatus() ||
	   LOAD_ASSET_UpdateArg1KeyPropertyStatus() ||
	   CREATE_LIST_UpdateArg1KeyPropertyStatus() ||
	   CREATE_VARIABLE_UpdateArg1KeyPropertyStatus() ||
	   CREATE_FLOAT_LERP_UpdateArg1KeyPropertyStatus() ||
	   CREATE_QUATERNION_LERP_UpdateArg1KeyPropertyStatus() ||
	   CREATE_VECTOR_LERP_UpdateArg1KeyPropertyStatus())
	{}
	else
	{
		SetPropertyValid(ARG1_PROPERTY, true);
	}
}


void ActionTableWidget::UpdateArg1VariablePropertyStatus(void)
{
	int32 errorCode;

	if((errorCode = BooleanExprParser::_IsNameValid(item(ARG1_PROPERTY, VALUE)->text().toStdString())) >= 0)
	{ 
		SetPropertyValid(ARG1_PROPERTY, false);
				
		QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(item(ARG1_PROPERTY, VALUE)->text().at(errorCode)) + "'";
		item(ARG1_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{ 
		if(CHANGE_NAME_UpdateArg1VariablePropertyStatus() ||
		   CREATE_TIMER_UpdateArg1VariablePropertyStatus() ||
		   CREATE_CAMERA_UpdateArg1VariablePropertyStatus() ||
		   CREATE_MESH_UpdateArg1VariablePropertyStatus() ||
		   CREATE_SET_UpdateArg1VariablePropertyStatus() ||
		   CREATE_PLAYER_CONTROL_UpdateArg1VariablePropertyStatus() ||
		   CREATE_TEXTURED_ANIMATION_UpdateArg1VariablePropertyStatus() ||
		   CREATE_TEXTURE_UpdateArg1VariablePropertyStatus() ||
		   GET_SYSTEM_TIME_UpdateArg1VariablePropertyStatus() ||
		   CREATE_TRIGGER_UpdateArg1VariablePropertyStatus() ||
		   LOAD_ASSET_UpdateArg1VariablePropertyStatus() ||
		   GET_NAME_UpdateArg1VariablePropertyStatus() ||
		   CHECK_OBJECT_EXIST_UpdateArg1VariablePropertyStatus() ||
		   CHECK_LIST_EMPTY_UpdateArg1VariablePropertyStatus() ||
		   SEND_MESSAGE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ANIMATION_FRAME_UpdateArg1VariablePropertyStatus() ||
		   RESIZE_SET_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ACTIVATION_LIMIT_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_DEACTIVATION_MODE_UpdateArg1VariablePropertyStatus() ||
		   CREATE_EMPTY_TILE_UpdateArg1VariablePropertyStatus() ||
		   REMOVE_BRUSH_UpdateArg1VariablePropertyStatus() ||
		   GET_AMOUNT_OF_FRAMES_UpdateArg1VariablePropertyStatus() ||
		   GET_FRAME_SIZE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ATLAS_UpdateArg1VariablePropertyStatus() ||
		   GET_TEXTURE_SIZE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_RANDOM_SEED_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ITEM_VALUE_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_ROTATION_RANGE_UpdateArg1VariablePropertyStatus() ||
		   ADD_TO_LIST_UpdateArg1VariablePropertyStatus() ||
		   TILED_SPACE_FILL_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ROTATION_RANGE_UpdateArg1VariablePropertyStatus() ||
		   ADD_INPUT_EVENT_UpdateArg1VariablePropertyStatus() ||
		   SET_ONE_INPUT_EVENT_UpdateArg1VariablePropertyStatus() ||
		   CREATE_TILED_SPACE_UpdateArg1VariablePropertyStatus() ||
		   GET_DATA_FROM_ITEM_UpdateArg1VariablePropertyStatus() ||
		   GET_ITEM_INDEX_UpdateArg1VariablePropertyStatus() ||
		   SET_DATA_TO_SLOT_UpdateArg1VariablePropertyStatus() ||
		   GET_DATA_FROM_SLOT_UpdateArg1VariablePropertyStatus() ||
		   RANDOMIZE_LIST_UpdateArg1VariablePropertyStatus() ||
		   GET_AMOUNT_OF_ITEMS_UpdateArg1VariablePropertyStatus() ||
		   GET_AMOUNT_OF_SLOTS_UpdateArg1VariablePropertyStatus() ||
		   COPY_LIST_UpdateArg1VariablePropertyStatus() ||
		   CLEAR_LIST_UpdateArg1VariablePropertyStatus() ||
		   CREATE_LIST_UpdateArg1VariablePropertyStatus() ||
		   CREATE_VARIABLE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_BACKGROUND_COLOR_UpdateArg1VariablePropertyStatus() ||
		   GET_SCALE_UpdateArg1VariablePropertyStatus() ||
		   DISPLACE_SCALE_UpdateArg1VariablePropertyStatus() ||
		   REPLACE_SCALE_UpdateArg1VariablePropertyStatus() ||
		   GET_POSITION_UpdateArg1VariablePropertyStatus() ||
		   REPLACE_POSITION_UpdateArg1VariablePropertyStatus() ||
		   DISPLACE_POSITION_UpdateArg1VariablePropertyStatus() ||
		   GET_ROTATION_UpdateArg1VariablePropertyStatus() ||
		   REPLACE_ROTATION_UpdateArg1VariablePropertyStatus() ||
		   DISPLACE_ROTATION_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_PLANE_NORMAL_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_MESH_UpdateArg1VariablePropertyStatus() ||
		   CREATE_QUAD_UpdateArg1VariablePropertyStatus() ||
		   CREATE_BOX_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_INTERSECTION_MODE_UpdateArg1VariablePropertyStatus() ||
		   CHECK_PROPERTY_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_PROPERTY_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_BACK_FACE_CULLING_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_ZOOM_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_ROTATION_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_STATIC_OBJECT_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_POSITION_OFFSET_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_FRONT_TEXTURE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_BACK_TEXTURE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_FRONT_TEXTURED_ANIMATION_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_BACK_TEXTURED_ANIMATION_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_SHOW_CURSOR_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_Z_BUFFER_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_TRIGGERS_BLOCK_UpdateArg1VariablePropertyStatus() ||
		   CREATE_KEYBOARD_EVENT_UpdateArg1VariablePropertyStatus() ||
		   ADD_TO_LAYER_UpdateArg1VariablePropertyStatus() ||
		   REMOVE_FROM_LAYER_UpdateArg1VariablePropertyStatus() ||
		   ADD_TO_UPDATE_LIST_UpdateArg1VariablePropertyStatus() ||
		   REMOVE_FROM_UPDATE_LIST_UpdateArg1VariablePropertyStatus() ||
		   ADD_TO_RENDER_LIST_UpdateArg1VariablePropertyStatus() ||
		   REMOVE_FROM_RENDER_LIST_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_CAMERA_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_RENDER_TARGET_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_MODE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_PROJECTION_MODE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_MOVEMENT_MODE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ROTATION_MODE_UpdateArg1VariablePropertyStatus() ||
		   GET_SELECT_RAY_UpdateArg1VariablePropertyStatus() ||
		   CREATE_FLOAT_LERP_UpdateArg1VariablePropertyStatus() ||
		   CREATE_QUATERNION_LERP_UpdateArg1VariablePropertyStatus() ||
		   CREATE_VECTOR_LERP_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_SPEED_UpdateArg1VariablePropertyStatus() ||
		   STREAMING_FLOAT_DELTA_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_VECTOR_LERP_MODE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_PLAYING_MODE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_PLAYING_STATE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_PLAYING_DURATION_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_INPUT_TIMING_UpdateArg1VariablePropertyStatus() ||
		   STREAMING_VECTOR_DELTA_UpdateArg1VariablePropertyStatus() || 
		   STREAMING_QUATERNION_DELTA_UpdateArg1VariablePropertyStatus() ||
		   STREAMING_VECTOR_CURRENT_UpdateArg1VariablePropertyStatus() ||
		   STREAMING_QUATERNION_CURRENT_UpdateArg1VariablePropertyStatus() ||
		   STREAMING_FLOAT_T_UpdateArg1VariablePropertyStatus() ||
		   STREAMING_FLOAT_CURRENT_UpdateArg1VariablePropertyStatus() ||
		   CHECK_LINE_IN_IMAGE_UpdateArg1VariablePropertyStatus() ||
		   CHECK_LINE_IN_POLYGON_UpdateArg1VariablePropertyStatus() ||
		   ROTATE_AROUND_PIVOT_UpdateArg1VariablePropertyStatus() ||
		   CHECK_LINE_IN_SURFACE_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_COLLISION_POLYGONS_INFO_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_COLLISION_CHECKING_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_COLLISION_GROUP_UpdateArg1VariablePropertyStatus() ||
		   CREATE_ROTATION_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_BOX_TEXTURE_MAP_UpdateArg1VariablePropertyStatus() ||
		   CREATE_BOX_TEXTURE_MAP_UpdateArg1VariablePropertyStatus() ||
		   GET_LINE_ANGLE_Z_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_VIEWING_POINT_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_DISTANCE_TO_VIEWING_POINT_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ZOOM_NEAR_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ZOOM_FAR_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ZOOM_SPEED_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_HITBOX_UpdateArg1VariablePropertyStatus() ||
		   CAMERA_LOOK_AT_UpdateArg1VariablePropertyStatus() ||
		   CHECK_FRUSTUM_IN_QUAD_UpdateArg1VariablePropertyStatus() ||
		   GET_DATA_FROM_CONTROL_VECTOR_UpdateArg1VariablePropertyStatus() ||
		   SET_DATA_TO_CONTROL_VECTOR_UpdateArg1VariablePropertyStatus() ||
		   GET_LINE_LENGTH_UpdateArg1VariablePropertyStatus() ||
		   GET_CIRCLE_LENGTH_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_T_UpdateArg1VariablePropertyStatus() ||
		   GET_FLOAT_LERP_T_UpdateArg1VariablePropertyStatus() ||
		   GET_FLOAT_LERP_CURRENT_UpdateArg1VariablePropertyStatus() ||
		   GET_FLOAT_LERP_DELTA_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_TEXTURE_FLIP_UpdateArg1VariablePropertyStatus() ||
		   GET_DATA_FROM_CONTROL_QUATERNION_UpdateArg1VariablePropertyStatus() ||
		   SET_DATA_TO_CONTROL_QUATERNION_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_FREEZE_UpdateArg1VariablePropertyStatus() ||
		   ROTATE_LIST_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_DELTA_SCALE_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_SHOW_UpdateArg1VariablePropertyStatus() ||
		   GET_LINE_DIRECTION_UpdateArg1VariablePropertyStatus() ||
		   MOVE_AROUND_PIVOT_UpdateArg1VariablePropertyStatus() ||
		   CREATE_BRUSH_UpdateArg1VariablePropertyStatus() ||
		   CHECK_TILE_EXIST_UpdateArg1VariablePropertyStatus() ||
		   CHECK_TILE_SOLID_UpdateArg1VariablePropertyStatus() ||
		   CHECK_TILE_PASS_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_TILE_PASS_UpdateArg1VariablePropertyStatus() ||
		   CREATE_SOLID_TILE_UpdateArg1VariablePropertyStatus() ||
		   GET_BRUSH_OF_TILE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_TILED_SPACE_SIZE_UpdateArg1VariablePropertyStatus() ||
		   SET_PUBLIC_TILE_UpdateArg1VariablePropertyStatus() ||
		   SET_PUBLIC_TEXTURE_UpdateArg1VariablePropertyStatus() ||
		   GET_TILE_CENTER_UpdateArg1VariablePropertyStatus() ||
		   MOVE_TILE_FROM_TO_UpdateArg1VariablePropertyStatus() ||
		   SWAP_TWO_TILES_UpdateArg1VariablePropertyStatus() ||
		   COPY_TILE_UpdateArg1VariablePropertyStatus() ||
		   GET_VIEWING_POINT_UpdateArg1VariablePropertyStatus() ||
		   GET_CAMERA_LOOK_UpdateArg1VariablePropertyStatus() ||
		   GET_VECTOR_LERP_PATH_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_STRIP_POINTS_UpdateArg1VariablePropertyStatus() ||
		   GET_VECTOR_LERP_T_UpdateArg1VariablePropertyStatus() ||
		   GET_QUATERNION_LERP_T_UpdateArg1VariablePropertyStatus() ||
		   GET_QUATERNION_LERP_PATH_UpdateArg1VariablePropertyStatus() ||
		   GET_LIST_LENGTH_UpdateArg1VariablePropertyStatus() ||
		   GET_RADIUS_FROM_2_POINTS_UpdateArg1VariablePropertyStatus() ||
		   GET_RADIUS_FROM_3_POINTS_UpdateArg1VariablePropertyStatus() ||
		   GET_CONTROL_VECTORS_UpdateArg1VariablePropertyStatus() ||
		   GET_CONTROL_QUATERNIONS_UpdateArg1VariablePropertyStatus() ||
		   CHECK_EQUALITY_UpdateArg1VariablePropertyStatus())
		{}
		else
		{
			if(AssetLibrary::_IsAssetExist(item(ARG1_PROPERTY, VALUE)->text().toStdString()))
			{
				SetPropertyValid(ARG1_PROPERTY, true);
			}
			else
			{
				SetPropertyValid(ARG1_PROPERTY, true);
				QString text = "Asset does not exist";
				item(ARG1_PROPERTY, VALUE)->setToolTip(text);
				item(ARG1_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}


void ActionTableWidget::UpdateArg1TemplatePropertyStatus(void)
{  
	StringExprParser parser; 

	if(parser.Parse(item(ARG1_PROPERTY, VALUE)->text().toStdString()) != StringExprParser::NO_ERRORS)
	{ 
		SetPropertyValid(ARG1_PROPERTY, false);  
			
		QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
		item(ARG1_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{
		SetPropertyValid(ARG1_PROPERTY, true);
		if(!IsStringExprCompute(item(ARG1_PROPERTY, VALUE)->text()))
		{
			QString text = "One or more args does not exist";
			item(ARG1_PROPERTY, VALUE)->setToolTip(text);
			item(ARG1_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
		} 
	}
}


void ActionTableWidget::UpdateArg1PropertyStatus(void)
{
	if(IsEnabled(ARG1_PROPERTY))
	{
		if(item(ARG1_PROPERTY, VALUE)->text().length() > 0)
		{
			if(item(ARG1_PROPERTY, ARG_MODE)->text() == "KEY")
			{
				UpdateArg1KeyPropertyStatus();
			}
			else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
			{
				UpdateArg1VariablePropertyStatus();
			}
			else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
			{
				UpdateArg1TemplatePropertyStatus(); 
			} 
			else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
			{
				UpdateArg1ExpressionPropertyStatus(); 
			} 
			else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "EQUALITY")
			{
				UpdateArg1EqualityPropertyStatus(); 
			} 
			else
			{ 
				CHANGE_VALUE_UpdateArg1PropertyStatus();
				CHANGE_LIST_VALUE_UpdateArg1PropertyStatus();
				CHANGE_FLOAT_LERP_TARGET_UpdateArg1PropertyStatus();
				SET_TWO_VECTORS_UpdateArg1PropertyStatus();
				SET_TWO_QUATERNIONS_UpdateArg1PropertyStatus();
				CHANGE_VALUE_RANGE_UpdateArg1PropertyStatus();
				CHANGE_CONTROL_VECTORS_UpdateArg1PropertyStatus();
				CHANGE_CONTROL_QUATERNIONS_UpdateArg1PropertyStatus();
				CHECK_COLLISION_UpdateArg1PropertyStatus();
				SWAP_ITEMS_UpdateArg1PropertyStatus();
				REMOVE_FROM_LIST_UpdateArg1PropertyStatus();
				GET_RANDOM_VALUE_UpdateArg1PropertyStatus();
				CREATE_ROTATION_FROM_TO_UpdateArg1PropertyStatus();
				CHANGE_VECTOR_COMPONENT_UpdateArg1PropertyStatus();
				GET_VECTOR_COMPONENT_UpdateArg1PropertyStatus();
				VECTOR_TO_SPHERICAL_UpdateArg1PropertyStatus();
				GET_NORMALIZED_VECTOR_UpdateArg1PropertyStatus();
				GET_VECTOR_ANGLE_UpdateArg1PropertyStatus();
				GET_VECTOR_LENGTH_UpdateArg1PropertyStatus();
				CHANGE_COLOR_UpdateArg1PropertyStatus();
				GET_COLOR_UpdateArg1PropertyStatus();
				CHANGE_COLOR_COMPONENT_UpdateArg1PropertyStatus();
				GET_COLOR_COMPONENT_UpdateArg1PropertyStatus();
			}
		}
	}
	else
	{
		item(ARG1_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
} 



void ActionTableWidget::UpdateArg1ExpressionPropertyStatus(void)
{
}


void ActionTableWidget::UpdateArg1EqualityPropertyStatus(void)
{
	if(CHECK_EQUALITY_UpdateArg1EqualityPropertyStatus())
	{}
}


void ActionTableWidget::UpdateArg2KeyPropertyStatus(void)
{ 
	if(CREATE_TRIGGER_UpdateArg2KeyPropertyStatus() ||
	   SET_PUBLIC_TILE_UpdateArg2KeyPropertyStatus() ||
	   SET_PUBLIC_TEXTURE_UpdateArg2KeyPropertyStatus())
	{}
	else
	{
		SetPropertyValid(ARG2_PROPERTY, true);
	}
}


void ActionTableWidget::UpdateArg2VariablePropertyStatus(void)
{
	int32 errorCode;

	if((errorCode = BooleanExprParser::_IsNameValid(item(ARG2_PROPERTY, VALUE)->text().toStdString())) >= 0)
	{ 
		SetPropertyValid(ARG2_PROPERTY, false);
			
		QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(item(ARG2_PROPERTY, VALUE)->text().at(errorCode)) + "'";
		item(ARG2_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{
		if(CREATE_TRIGGER_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_VALUE_UpdateArg2VariablePropertyStatus() || 
		   LOAD_ASSET_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_LIST_VALUE_UpdateArg2VariablePropertyStatus() ||
		   ENABLE_ROTATION_RANGE_UpdateArg2VariablePropertyStatus() ||
		   ADD_TO_LIST_UpdateArg2VariablePropertyStatus() ||
		   TILED_SPACE_FILL_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_ROTATION_RANGE_UpdateArg2VariablePropertyStatus() ||
		   ADD_INPUT_EVENT_UpdateArg2VariablePropertyStatus() ||
		   SET_ONE_INPUT_EVENT_UpdateArg2VariablePropertyStatus() ||
		   GET_DATA_FROM_ITEM_UpdateArg2VariablePropertyStatus() ||
		   GET_ITEM_INDEX_UpdateArg2VariablePropertyStatus() ||
		   SET_DATA_TO_SLOT_UpdateArg2VariablePropertyStatus() ||
		   GET_DATA_FROM_SLOT_UpdateArg2VariablePropertyStatus() ||
		   COPY_LIST_UpdateArg2VariablePropertyStatus() ||
		   CREATE_LIST_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_INTERSECTION_MODE_UpdateArg2VariablePropertyStatus() ||
		   CHECK_PROPERTY_UpdateArg2VariablePropertyStatus() ||
		   ENABLE_PROPERTY_UpdateArg2VariablePropertyStatus() ||
		   GET_POSITION_UpdateArg2VariablePropertyStatus() ||
		   GET_SCALE_UpdateArg2VariablePropertyStatus() ||
		   GET_ROTATION_UpdateArg2VariablePropertyStatus() ||
		   CREATE_KEYBOARD_EVENT_UpdateArg2VariablePropertyStatus() ||
		   GET_SELECT_RAY_UpdateArg2VariablePropertyStatus() ||
		   STREAMING_FLOAT_CURRENT_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_TARGET_UpdateArg2VariablePropertyStatus() ||
		   STREAMING_FLOAT_DELTA_UpdateArg2VariablePropertyStatus() ||
		   CHECK_LINE_IN_IMAGE_UpdateArg2VariablePropertyStatus() ||
		   CHECK_LINE_IN_POLYGON_UpdateArg2VariablePropertyStatus() ||
		   ROTATE_AROUND_PIVOT_UpdateArg2VariablePropertyStatus() ||
		   CHECK_LINE_IN_SURFACE_UpdateArg2VariablePropertyStatus() ||
		   SET_TWO_VECTORS_UpdateArg2VariablePropertyStatus() ||
		   SET_TWO_QUATERNIONS_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_VALUE_RANGE_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_CONTROL_VECTORS_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_CONTROL_QUATERNIONS_UpdateArg2VariablePropertyStatus() ||
		   CHECK_COLLISION_UpdateArg2VariablePropertyStatus() ||
		   SWAP_ITEMS_UpdateArg2VariablePropertyStatus() ||
		   REMOVE_FROM_LIST_UpdateArg2VariablePropertyStatus() ||
		   GET_RANDOM_VALUE_UpdateArg2VariablePropertyStatus() ||
		   CREATE_ROTATION_FROM_TO_UpdateArg2VariablePropertyStatus() ||
		   CREATE_ROTATION_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_BOX_TEXTURE_MAP_UpdateArg2VariablePropertyStatus() ||
		   CREATE_BOX_TEXTURE_MAP_UpdateArg2VariablePropertyStatus() ||
		   GET_LINE_ANGLE_Z_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_VECTOR_COMPONENT_UpdateArg2VariablePropertyStatus() ||
		   GET_VECTOR_COMPONENT_UpdateArg2VariablePropertyStatus() ||
		   VECTOR_TO_SPHERICAL_UpdateArg2VariablePropertyStatus() ||
		   GET_NORMALIZED_VECTOR_UpdateArg2VariablePropertyStatus() ||
		   GET_VECTOR_ANGLE_UpdateArg2VariablePropertyStatus() ||
		   GET_VECTOR_LENGTH_UpdateArg2VariablePropertyStatus() ||
		   CHECK_FRUSTUM_IN_QUAD_UpdateArg2VariablePropertyStatus() ||
		   GET_LINE_LENGTH_UpdateArg2VariablePropertyStatus() ||
		   GET_CIRCLE_LENGTH_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_COLOR_UpdateArg2VariablePropertyStatus() ||
		   GET_COLOR_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_COLOR_COMPONENT_UpdateArg2VariablePropertyStatus() ||
		   GET_COLOR_COMPONENT_UpdateArg2VariablePropertyStatus() ||
		   STREAMING_VECTOR_DELTA_UpdateArg2VariablePropertyStatus() ||
		   STREAMING_QUATERNION_DELTA_UpdateArg2VariablePropertyStatus() ||
		   ENABLE_TEXTURE_FLIP_UpdateArg2VariablePropertyStatus() ||
		   GET_LINE_DIRECTION_UpdateArg2VariablePropertyStatus() ||
		   MOVE_AROUND_PIVOT_UpdateArg2VariablePropertyStatus() ||
		   CREATE_BRUSH_UpdateArg2VariablePropertyStatus() ||
		   CHECK_TILE_EXIST_UpdateArg2VariablePropertyStatus() ||
		   CHECK_TILE_SOLID_UpdateArg2VariablePropertyStatus() ||
		   CHECK_TILE_PASS_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_TILE_PASS_UpdateArg2VariablePropertyStatus() ||
		   CREATE_SOLID_TILE_UpdateArg2VariablePropertyStatus() ||
		   GET_BRUSH_OF_TILE_UpdateArg2VariablePropertyStatus() ||
		   CHANGE_TILED_SPACE_SIZE_UpdateArg2VariablePropertyStatus() ||
		   SET_PUBLIC_TILE_UpdateArg2VariablePropertyStatus() ||
		   SET_PUBLIC_TEXTURE_UpdateArg2VariablePropertyStatus() ||
		   GET_TILE_CENTER_UpdateArg2VariablePropertyStatus() ||
		   MOVE_TILE_FROM_TO_UpdateArg2VariablePropertyStatus() ||
		   SWAP_TWO_TILES_UpdateArg2VariablePropertyStatus() ||
		   COPY_TILE_UpdateArg2VariablePropertyStatus() ||
		   GET_VECTOR_LERP_PATH_UpdateArg2VariablePropertyStatus() ||
		   GET_QUATERNION_LERP_PATH_UpdateArg2VariablePropertyStatus() ||
		   GET_RADIUS_FROM_2_POINTS_UpdateArg2VariablePropertyStatus() ||
		   GET_RADIUS_FROM_3_POINTS_UpdateArg2VariablePropertyStatus() ||
		   GET_CONTROL_VECTORS_UpdateArg2VariablePropertyStatus() ||
		   GET_CONTROL_QUATERNIONS_UpdateArg2VariablePropertyStatus() ||
		   CHECK_EQUALITY_UpdateArg2VariablePropertyStatus())
		{}
		else
		{
			if(AssetLibrary::_IsAssetExist(item(ARG2_PROPERTY, VALUE)->text().toStdString()))
			{ 
				SetPropertyValid(ARG2_PROPERTY, true);
			}
			else
			{
				SetPropertyValid(ARG2_PROPERTY, true);
				QString text = "Asset does not exist";
				item(ARG2_PROPERTY, VALUE)->setToolTip(text);
				item(ARG2_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}


void ActionTableWidget::UpdateArg2TemplatePropertyStatus(void)
{
	StringExprParser parser;
 
	if(parser.Parse(item(ARG2_PROPERTY, VALUE)->text().toStdString()) != StringExprParser::NO_ERRORS)
	{
		SetPropertyValid(ARG2_PROPERTY, false);
			
		QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
		item(ARG2_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{
		SetPropertyValid(ARG2_PROPERTY, true);
		if(!IsStringExprCompute(item(ARG2_PROPERTY, VALUE)->text()))
		{
			QString text = "One or more args does not exist";
			item(ARG2_PROPERTY, VALUE)->setToolTip(text);
			item(ARG2_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
		} 
	}
}



void ActionTableWidget::UpdateArg2ExpressionPropertyStatus(void)
{
	if(CHANGE_VALUE_UpdateArg2ExpressionPropertyStatus() ||
	   CHANGE_LIST_VALUE_UpdateArg2ExpressionPropertyStatus())
	{}
}


void ActionTableWidget::UpdateArg2PropertyStatus(void)
{
	if(IsEnabled(ARG2_PROPERTY))
	{ 
		if((item(TYPE_PROPERTY, VALUE)->text() == "GET_RANDOM_VALUE" && item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")) 
		{}
		else 
		{
			if(item(ARG2_PROPERTY, VALUE)->text().length() > 0)
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					UpdateArg2KeyPropertyStatus();
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
				{
					UpdateArg2VariablePropertyStatus();
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					UpdateArg2ExpressionPropertyStatus();
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
				{
					UpdateArg2TemplatePropertyStatus();
				}
				else
				{ 
					CHANGE_ITEM_VALUE_UpdateArg2PropertyStatus();
					CREATE_VARIABLE_UpdateArg2PropertyStatus();
					STREAMING_VECTOR_CURRENT_UpdateArg2PropertyStatus();
					STREAMING_QUATERNION_CURRENT_UpdateArg2PropertyStatus();
					STREAMING_FLOAT_T_UpdateArg2PropertyStatus();
					GET_DATA_FROM_CONTROL_VECTOR_UpdateArg2PropertyStatus();
					SET_DATA_TO_CONTROL_VECTOR_UpdateArg2PropertyStatus();
					GET_DATA_FROM_CONTROL_QUATERNION_UpdateArg2PropertyStatus();
					SET_DATA_TO_CONTROL_QUATERNION_UpdateArg2PropertyStatus();
					CHANGE_STRIP_POINTS_UpdateArg2PropertyStatus();
					GET_VECTOR_LERP_T_UpdateArg2PropertyStatus();
					GET_QUATERNION_LERP_T_UpdateArg2PropertyStatus();
				}  
			}
			else 
			{ 
				if(item(TYPE_PROPERTY, VALUE)->text() == "CREATE_TRIGGER" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "CHECK_EQUALITY" || 
				   item(TYPE_PROPERTY, VALUE)->text() == "ADD_TO_RENDER_LIST" || 
				   item(TYPE_PROPERTY, VALUE)->text() == "GET_VECTOR_COMPONENT" || 
				   item(TYPE_PROPERTY, VALUE)->text() == "VECTOR_TO_SPHERICAL" || 
				   item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VECTOR_COMPONENT" || 
				   (item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE" && item(ARG1_PROPERTY, VALUE)->text() == "STRING" && item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY") ||
				   (item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_LIST_VALUE" && item(ARG1_PROPERTY, VALUE)->text() == "STRING" && item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY"))
				{
					SetPropertyValid(ARG2_PROPERTY, true);
				}
				else
				{
					SetPropertyValid(ARG2_PROPERTY, false);
				}
			}
		}
	}
	else
	{
		item(ARG2_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
}


void ActionTableWidget::UpdateArg3KeyPropertyStatus(void)
{
	if(CREATE_TRIGGER_UpdateArg3KeyPropertyStatus())
	{}
	else
	{
		SetPropertyValid(ARG3_PROPERTY, true);
	}
}



void ActionTableWidget::UpdateArg3VariablePropertyStatus(void)
{
	int32 errorCode;

	if((errorCode = BooleanExprParser::_IsNameValid(item(ARG3_PROPERTY, VALUE)->text().toStdString())) >= 0)
	{ 
		SetPropertyValid(ARG3_PROPERTY, false);
			
		QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(item(ARG3_PROPERTY, VALUE)->text().at(errorCode)) + "'";
		item(ARG3_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{ 
		if(CREATE_TRIGGER_UpdateArg3VariablePropertyStatus() ||
		   CHANGE_ITEM_VALUE_UpdateArg3VariablePropertyStatus() ||
		   ADD_TO_LIST_UpdateArg3VariablePropertyStatus() ||
		   TILED_SPACE_FILL_UpdateArg3VariablePropertyStatus() ||
		   CHANGE_ROTATION_RANGE_UpdateArg3VariablePropertyStatus() ||
		   ADD_INPUT_EVENT_UpdateArg3VariablePropertyStatus() ||
		   SET_ONE_INPUT_EVENT_UpdateArg3VariablePropertyStatus() ||
		   CREATE_BRUSH_UpdateArg3VariablePropertyStatus() ||
		   CREATE_VARIABLE_UpdateArg3VariablePropertyStatus() ||
		   STREAMING_VECTOR_CURRENT_UpdateArg3VariablePropertyStatus() ||
		   STREAMING_QUATERNION_CURRENT_UpdateArg3VariablePropertyStatus() ||
		   STREAMING_FLOAT_T_UpdateArg3VariablePropertyStatus() ||
		   CHECK_LINE_IN_IMAGE_UpdateArg3VariablePropertyStatus() ||
		   MOVE_TILE_FROM_TO_UpdateArg3VariablePropertyStatus() ||
		   SWAP_TWO_TILES_UpdateArg3VariablePropertyStatus() ||
		   COPY_TILE_UpdateArg3VariablePropertyStatus() ||
		   CHECK_LINE_IN_POLYGON_UpdateArg3VariablePropertyStatus() ||
		   ROTATE_AROUND_PIVOT_UpdateArg3VariablePropertyStatus() ||
		   CHECK_LINE_IN_SURFACE_UpdateArg3VariablePropertyStatus() ||
		   SET_TWO_VECTORS_UpdateArg3VariablePropertyStatus() ||
		   SET_TWO_QUATERNIONS_UpdateArg3VariablePropertyStatus() ||
		   CHANGE_VALUE_RANGE_UpdateArg3VariablePropertyStatus() ||
		   CHECK_COLLISION_UpdateArg3VariablePropertyStatus() ||
		   SWAP_ITEMS_UpdateArg3VariablePropertyStatus() ||
		   REMOVE_FROM_LIST_UpdateArg3VariablePropertyStatus() ||
		   GET_RANDOM_VALUE_UpdateArg3VariablePropertyStatus() ||
		   CREATE_ROTATION_FROM_TO_UpdateArg3VariablePropertyStatus() ||
		   GET_LINE_ANGLE_Z_UpdateArg3VariablePropertyStatus() ||
		   CHANGE_VECTOR_COMPONENT_UpdateArg3VariablePropertyStatus() ||
		   GET_VECTOR_COMPONENT_UpdateArg3VariablePropertyStatus() ||
		   VECTOR_TO_SPHERICAL_UpdateArg3VariablePropertyStatus() ||
		   GET_VECTOR_ANGLE_UpdateArg3VariablePropertyStatus() ||
		   STREAMING_FLOAT_DELTA_UpdateArg3VariablePropertyStatus() ||
		   CHECK_FRUSTUM_IN_QUAD_UpdateArg3VariablePropertyStatus() ||
		   GET_DATA_FROM_CONTROL_VECTOR_UpdateArg3VariablePropertyStatus() ||
		   SET_DATA_TO_CONTROL_VECTOR_UpdateArg3VariablePropertyStatus() ||
		   GET_LINE_LENGTH_UpdateArg3VariablePropertyStatus() ||
		   GET_DATA_FROM_CONTROL_QUATERNION_UpdateArg3VariablePropertyStatus() ||
		   SET_DATA_TO_CONTROL_QUATERNION_UpdateArg3VariablePropertyStatus() ||
		   GET_LINE_DIRECTION_UpdateArg3VariablePropertyStatus() ||
		   CHANGE_STRIP_POINTS_UpdateArg3VariablePropertyStatus() ||
		   GET_VECTOR_LERP_T_UpdateArg3VariablePropertyStatus() ||
		   GET_QUATERNION_LERP_T_UpdateArg3VariablePropertyStatus() ||
		   GET_RADIUS_FROM_2_POINTS_UpdateArg3VariablePropertyStatus() ||
		   GET_RADIUS_FROM_3_POINTS_UpdateArg3VariablePropertyStatus() ||
		   CHECK_EQUALITY_UpdateArg3VariablePropertyStatus())
		{} 
		else
		{
			if(AssetLibrary::_IsAssetExist(item(ARG3_PROPERTY, VALUE)->text().toStdString()))
			{
				SetPropertyValid(ARG3_PROPERTY, true);
			}
			else
			{
				SetPropertyValid(ARG3_PROPERTY, true);
				QString text = "Asset does not exist";
				item(ARG3_PROPERTY, VALUE)->setToolTip(text);
				item(ARG3_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}


void ActionTableWidget::UpdateArg3ExpressionPropertyStatus(void)
{
	if(CHANGE_ITEM_VALUE_UpdateArg3ExpressionPropertyStatus() ||
	   CREATE_VARIABLE_UpdateArg3ExpressionPropertyStatus())
	{}
}


void ActionTableWidget::UpdateArg3TemplatePropertyStatus(void)
{
	StringExprParser parser;

	if(parser.Parse(item(ARG3_PROPERTY, VALUE)->text().toStdString()) != StringExprParser::NO_ERRORS)
	{
		SetPropertyValid(ARG3_PROPERTY, false);
					
		QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
		item(ARG3_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{
		SetPropertyValid(ARG3_PROPERTY, true);
		if(!IsStringExprCompute(item(ARG3_PROPERTY, VALUE)->text()))
		{
			QString text = "One or more args does not exist";
			item(ARG3_PROPERTY, VALUE)->setToolTip(text);
			item(ARG3_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
		} 
	}
}


void ActionTableWidget::UpdateArg3PropertyStatus(void)
{
	if(IsEnabled(ARG3_PROPERTY))
	{ 
		if((item(TYPE_PROPERTY, VALUE)->text() == "GET_RANDOM_VALUE" && item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN"))
		{}
		else
		{
			if(item(ARG3_PROPERTY, VALUE)->text().length() > 0)
			{
				if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					UpdateArg3KeyPropertyStatus();
				}
				else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
				{
					UpdateArg3VariablePropertyStatus();
				}
				else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					UpdateArg3ExpressionPropertyStatus();
				}
				else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
				{
					UpdateArg3TemplatePropertyStatus();
				}
				else
				{ 
					STREAMING_VECTOR_DELTA_UpdateArg3PropertyStatus();
					STREAMING_QUATERNION_DELTA_UpdateArg3PropertyStatus();
				} 
			}
			else 
			{ 
				if(item(TYPE_PROPERTY, VALUE)->text() == "CREATE_TRIGGER" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "CHECK_EQUALITY" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "GET_VECTOR_COMPONENT" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "VECTOR_TO_SPHERICAL" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VECTOR_COMPONENT" ||   
				   item(TYPE_PROPERTY, VALUE)->text() == "MOVE_TILE_FROM_TO" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "SWAP_TWO_TILES" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "COPY_TILE" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_POLYGON" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_SURFACE" ||
				  (item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_ITEM_VALUE" && item(ARG2_PROPERTY, VALUE)->text() == "STRING" && item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY") ||
				  (item(TYPE_PROPERTY, VALUE)->text() == "CREATE_VARIABLE" && item(ARG2_PROPERTY, VALUE)->text() == "STRING" && item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY"))
				{
					SetPropertyValid(ARG3_PROPERTY, true);
				}
				else
				{
					SetPropertyValid(ARG3_PROPERTY, false);
				}
			}
		}
	}
	else
	{
		item(ARG3_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
}


void ActionTableWidget::UpdateArg4KeyPropertyStatus(void)
{
	SetPropertyValid(ARG4_PROPERTY, true);
}



void ActionTableWidget::UpdateArg4VariablePropertyStatus(void)
{ 
	if(CHECK_LINE_IN_IMAGE_UpdateArg4VariablePropertyStatus() ||
	   SWAP_ITEMS_UpdateArg4VariablePropertyStatus() ||
	   CHECK_LINE_IN_POLYGON_UpdateArg4VariablePropertyStatus() ||
	   ROTATE_AROUND_PIVOT_UpdateArg4VariablePropertyStatus() ||
	   CHECK_LINE_IN_SURFACE_UpdateArg4VariablePropertyStatus() ||
	   STREAMING_VECTOR_DELTA_UpdateArg4VariablePropertyStatus() ||
	   STREAMING_QUATERNION_DELTA_UpdateArg4VariablePropertyStatus() ||
	   GET_RADIUS_FROM_3_POINTS_UpdateArg4VariablePropertyStatus() ||
	   CHANGE_VECTOR_COMPONENT_UpdateArg4VariablePropertyStatus() ||
	   GET_VECTOR_COMPONENT_UpdateArg4VariablePropertyStatus() ||
	   VECTOR_TO_SPHERICAL_UpdateArg4VariablePropertyStatus() ||
	   GET_VECTOR_ANGLE_UpdateArg4VariablePropertyStatus())
	{}
	else
	{ 
		int32 errorCode;

		if((errorCode = BooleanExprParser::_IsNameValid(item(ARG4_PROPERTY, VALUE)->text().toStdString())) >= 0)
		{ 
			SetPropertyValid(ARG4_PROPERTY, false);
			
			QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(item(ARG4_PROPERTY, VALUE)->text().at(errorCode)) + "'";
			item(ARG4_PROPERTY, VALUE)->setToolTip(text);
		}
		else
		{
			if(AssetLibrary::_IsAssetExist(item(ARG4_PROPERTY, VALUE)->text().toStdString()))
			{
				SetPropertyValid(ARG4_PROPERTY, true);
			}
			else
			{
				SetPropertyValid(ARG4_PROPERTY, true);
				QString text = "Asset does not exist";
				item(ARG4_PROPERTY, VALUE)->setToolTip(text);
				item(ARG4_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}


void ActionTableWidget::UpdateArg4TemplatePropertyStatus(void)
{
	StringExprParser parser;

	if(parser.Parse(item(ARG4_PROPERTY, VALUE)->text().toStdString()) != StringExprParser::NO_ERRORS)
	{
		SetPropertyValid(ARG4_PROPERTY, false);
		
		QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
		item(ARG4_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{
		SetPropertyValid(ARG4_PROPERTY, true);
		if(!IsStringExprCompute(item(ARG4_PROPERTY, VALUE)->text()))
		{
			QString text = "One or more args does not exist";
			item(ARG4_PROPERTY, VALUE)->setToolTip(text);
			item(ARG4_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
		}
	}
}


void ActionTableWidget::UpdateArg4PropertyStatus(void)
{
	if(IsEnabled(ARG4_PROPERTY))
	{ 
		if((item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VECTOR_COMPONENT" && item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2") ||
		   (item(TYPE_PROPERTY, VALUE)->text() == "VECTOR_TO_SPHERICAL" && item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2") ||
		   (item(TYPE_PROPERTY, VALUE)->text() == "GET_VECTOR_COMPONENT" && item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2") ||
		   (item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS" && (item(ARG1_PROPERTY, VALUE)->text() == "INDEX" || item(ARG1_PROPERTY, VALUE)->text() == "LIST")))
		{}
		else
		{
			if(item(ARG4_PROPERTY, VALUE)->text().length() > 0)
			{
				if(item(ARG4_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					UpdateArg4KeyPropertyStatus();
				}
				else if(item(ARG4_PROPERTY, ARG_MODE)->text() == "VARIABLE")
				{
					UpdateArg4VariablePropertyStatus(); 
				}
				else if(item(ARG4_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
				{
					UpdateArg4TemplatePropertyStatus();
				}
			}
			else 
			{ 
				if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_POLYGON" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_SURFACE" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "GET_VECTOR_COMPONENT" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "VECTOR_TO_SPHERICAL" ||
				   item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VECTOR_COMPONENT")
				{
					SetPropertyValid(ARG4_PROPERTY, true);
				}
				else
				{
					SetPropertyValid(ARG4_PROPERTY, false);
				}
			}
		}
	}
	else
	{
		item(ARG4_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
}


void ActionTableWidget::UpdateArg5KeyPropertyStatus(void)
{
	SetPropertyValid(ARG5_PROPERTY, true);
}


void ActionTableWidget::UpdateArg5VariablePropertyStatus(void)
{ 
	if(CHECK_LINE_IN_IMAGE_UpdateArg5VariablePropertyStatus() ||
	   SWAP_ITEMS_UpdateArg5VariablePropertyStatus() ||
	   CHECK_LINE_IN_POLYGON_UpdateArg5VariablePropertyStatus() ||
	   ROTATE_AROUND_PIVOT_UpdateArg5VariablePropertyStatus() ||
	   CHECK_LINE_IN_SURFACE_UpdateArg5VariablePropertyStatus())
	{}  
	else
	{
		int32 errorCode;

		if((errorCode = BooleanExprParser::_IsNameValid(item(ARG5_PROPERTY, VALUE)->text().toStdString())) >= 0)
		{ 
			SetPropertyValid(ARG5_PROPERTY, false);
				
			QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(item(ARG5_PROPERTY, VALUE)->text().at(errorCode)) + "'";
			item(ARG5_PROPERTY, VALUE)->setToolTip(text);
		}
		else
		{
			if(AssetLibrary::_IsAssetExist(item(ARG5_PROPERTY, VALUE)->text().toStdString()))
			{
				SetPropertyValid(ARG5_PROPERTY, true);
			}
			else
			{
				SetPropertyValid(ARG5_PROPERTY, true);
				QString text = "Asset does not exist";
				item(ARG5_PROPERTY, VALUE)->setToolTip(text);
				item(ARG5_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}


void ActionTableWidget::UpdateArg5TemplatePropertyStatus(void)
{
	StringExprParser parser;

	if(parser.Parse(item(ARG5_PROPERTY, VALUE)->text().toStdString()) != StringExprParser::NO_ERRORS)
	{
		SetPropertyValid(ARG5_PROPERTY, false);
			
		QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
		item(ARG5_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{
		SetPropertyValid(ARG5_PROPERTY, true);
		if(!IsStringExprCompute(item(ARG5_PROPERTY, VALUE)->text()))
		{
			QString text = "One or more args does not exist";
			item(ARG5_PROPERTY, VALUE)->setToolTip(text);
			item(ARG5_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
		}
	}
}


void ActionTableWidget::UpdateArg5PropertyStatus(void)
{
	if(IsEnabled(ARG5_PROPERTY))
	{ 
		if(item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS" && (item(ARG1_PROPERTY, VALUE)->text() == "INDEX" || item(ARG1_PROPERTY, VALUE)->text() == "LIST"))
		{}
		else if(item(ARG5_PROPERTY, VALUE)->text().length() > 0)
		{
			if(item(ARG5_PROPERTY, ARG_MODE)->text() == "KEY")
			{
				UpdateArg5KeyPropertyStatus();
			}
			else if(item(ARG5_PROPERTY, ARG_MODE)->text() == "VARIABLE")
			{
				UpdateArg5VariablePropertyStatus();
			}
			else if(item(ARG5_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
			{
				UpdateArg5TemplatePropertyStatus();
			}
		}
		else 
		{ 
			if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE" ||
			   item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_SURFACE")
			{
				SetPropertyValid(ARG5_PROPERTY, true);
			}
			else
			{
				SetPropertyValid(ARG5_PROPERTY, false);
			}
		}
	} 
	else
	{
		item(ARG5_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
}



void ActionTableWidget::UpdateTargetVariablePropertyStatus(void)
{
	int32 errorCode;

	if((errorCode = BooleanExprParser::_IsNameValid(item(TARGET_PROPERTY, VALUE)->text().toStdString())) >= 0)
	{ 
		SetPropertyValid(TARGET_PROPERTY, false);
			
		QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(item(TARGET_PROPERTY, VALUE)->text().at(errorCode)) + "'";
		item(TARGET_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{
		if(CHANGE_NAME_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_VALUE_UpdateTargetVariablePropertyStatus() ||
		   GET_NAME_UpdateTargetVariablePropertyStatus() ||
		   CHECK_OBJECT_EXIST_UpdateTargetVariablePropertyStatus() ||
		   CHECK_LIST_EMPTY_UpdateTargetVariablePropertyStatus() ||
		   SEND_MESSAGE_UpdateTargetVariablePropertyStatus() ||
		   DESTROY_OBJECT_UpdateTargetVariablePropertyStatus() ||
		   SAVE_TO_FILE_UpdateTargetVariablePropertyStatus() ||
		   SET_PRIVATE_ASSET_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_ANIMATION_FRAME_UpdateTargetVariablePropertyStatus() ||
		   RESIZE_SET_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_ACTIVATION_LIMIT_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_DEACTIVATION_MODE_UpdateTargetVariablePropertyStatus() ||
		   CREATE_EMPTY_TILE_UpdateTargetVariablePropertyStatus() ||
		   REMOVE_BRUSH_UpdateTargetVariablePropertyStatus() ||
		   GET_AMOUNT_OF_FRAMES_UpdateTargetVariablePropertyStatus() ||
		   GET_FRAME_SIZE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_ATLAS_UpdateTargetVariablePropertyStatus() ||
		   GET_TEXTURE_SIZE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_ITEM_VALUE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_LIST_VALUE_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_ROTATION_RANGE_UpdateTargetVariablePropertyStatus() ||
		   ADD_TO_LIST_UpdateTargetVariablePropertyStatus() ||
		   TILED_SPACE_FILL_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_ROTATION_RANGE_UpdateTargetVariablePropertyStatus() ||
		   ADD_INPUT_EVENT_UpdateTargetVariablePropertyStatus() ||
		   SET_ONE_INPUT_EVENT_UpdateTargetVariablePropertyStatus() ||
		   GET_DATA_FROM_ITEM_UpdateTargetVariablePropertyStatus() ||
		   GET_ITEM_INDEX_UpdateTargetVariablePropertyStatus() ||
		   SET_DATA_TO_SLOT_UpdateTargetVariablePropertyStatus() ||
		   GET_DATA_FROM_SLOT_UpdateTargetVariablePropertyStatus() ||
		   RANDOMIZE_LIST_UpdateTargetVariablePropertyStatus() ||
		   GET_AMOUNT_OF_ITEMS_UpdateTargetVariablePropertyStatus() ||
		   GET_AMOUNT_OF_SLOTS_UpdateTargetVariablePropertyStatus() ||
		   COPY_LIST_UpdateTargetVariablePropertyStatus() ||
		   CLEAR_LIST_UpdateTargetVariablePropertyStatus() ||
		   CLEAR_INPUT_EVENTS_UpdateTargetVariablePropertyStatus() ||
		   REPLAY_UpdateTargetVariablePropertyStatus() ||
		   GET_SCALE_UpdateTargetVariablePropertyStatus() ||
		   DISPLACE_SCALE_UpdateTargetVariablePropertyStatus() ||
		   REPLACE_SCALE_UpdateTargetVariablePropertyStatus() ||
		   GET_POSITION_UpdateTargetVariablePropertyStatus() ||
		   REPLACE_POSITION_UpdateTargetVariablePropertyStatus() ||
		   DISPLACE_POSITION_UpdateTargetVariablePropertyStatus() ||
		   GET_ROTATION_UpdateTargetVariablePropertyStatus() ||
		   REPLACE_ROTATION_UpdateTargetVariablePropertyStatus() ||
		   DISPLACE_ROTATION_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_PLANE_NORMAL_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_MESH_UpdateTargetVariablePropertyStatus() ||
		   CREATE_QUAD_UpdateTargetVariablePropertyStatus() ||
		   CREATE_BOX_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_INTERSECTION_MODE_UpdateTargetVariablePropertyStatus() ||
		   CHECK_PROPERTY_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_PROPERTY_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_BACK_FACE_CULLING_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_ZOOM_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_ROTATION_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_STATIC_OBJECT_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_POSITION_OFFSET_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_FRONT_TEXTURE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_BACK_TEXTURE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_FRONT_TEXTURED_ANIMATION_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_BACK_TEXTURED_ANIMATION_UpdateTargetVariablePropertyStatus() ||
		   PICK_SCENE_UpdateTargetVariablePropertyStatus() ||
		   ADD_TO_LAYER_UpdateTargetVariablePropertyStatus() ||
		   REMOVE_FROM_LAYER_UpdateTargetVariablePropertyStatus() ||
		   ADD_TO_UPDATE_LIST_UpdateTargetVariablePropertyStatus() ||
		   REMOVE_FROM_UPDATE_LIST_UpdateTargetVariablePropertyStatus() ||
		   ADD_TO_RENDER_LIST_UpdateTargetVariablePropertyStatus() ||
		   REMOVE_FROM_RENDER_LIST_UpdateTargetVariablePropertyStatus() ||
		   PICK_CAMERA_UpdateTargetVariablePropertyStatus() ||
		   INVERSE_LIST_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_CAMERA_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_RENDER_TARGET_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_MODE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_PROJECTION_MODE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_MOVEMENT_MODE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_ROTATION_MODE_UpdateTargetVariablePropertyStatus() ||
		   GET_SELECT_RAY_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_SPEED_UpdateTargetVariablePropertyStatus() ||
		   STREAMING_FLOAT_CURRENT_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_TARGET_UpdateTargetVariablePropertyStatus() ||
		   STREAMING_FLOAT_DELTA_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_VECTOR_LERP_MODE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_PLAYING_MODE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_PLAYING_STATE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_PLAYING_DURATION_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_INPUT_TIMING_UpdateTargetVariablePropertyStatus() ||
		   STREAMING_VECTOR_DELTA_UpdateTargetVariablePropertyStatus() ||
		   STREAMING_QUATERNION_DELTA_UpdateTargetVariablePropertyStatus() ||
		   STREAMING_VECTOR_CURRENT_UpdateTargetVariablePropertyStatus() ||
		   STREAMING_QUATERNION_CURRENT_UpdateTargetVariablePropertyStatus() ||
		   STREAMING_FLOAT_T_UpdateTargetVariablePropertyStatus() ||
		   CHECK_LINE_IN_IMAGE_UpdateTargetVariablePropertyStatus() ||
		   CHECK_LINE_IN_POLYGON_UpdateTargetVariablePropertyStatus() ||
		   ROTATE_AROUND_PIVOT_UpdateTargetVariablePropertyStatus() ||
		   CHECK_LINE_IN_SURFACE_UpdateTargetVariablePropertyStatus() ||
		   SET_TWO_VECTORS_UpdateTargetVariablePropertyStatus() ||
		   SET_TWO_QUATERNIONS_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_VALUE_RANGE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_CONTROL_VECTORS_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_CONTROL_QUATERNIONS_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_COLLISION_POLYGONS_INFO_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_COLLISION_CHECKING_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_COLLISION_GROUP_UpdateTargetVariablePropertyStatus() ||
		   CREATE_ROTATION_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_BOX_TEXTURE_MAP_UpdateTargetVariablePropertyStatus() ||
		   CREATE_BOX_TEXTURE_MAP_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_VIEWING_POINT_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_DISTANCE_TO_VIEWING_POINT_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_ZOOM_NEAR_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_ZOOM_FAR_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_ZOOM_SPEED_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_HITBOX_UpdateTargetVariablePropertyStatus() ||
		   CAMERA_LOOK_AT_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_VECTOR_COMPONENT_UpdateTargetVariablePropertyStatus() ||
		   GET_VECTOR_COMPONENT_UpdateTargetVariablePropertyStatus() ||
		   VECTOR_TO_SPHERICAL_UpdateTargetVariablePropertyStatus() ||
		   GET_NORMALIZED_VECTOR_UpdateTargetVariablePropertyStatus() ||
		   GET_VECTOR_LENGTH_UpdateTargetVariablePropertyStatus() ||
		   CHECK_FRUSTUM_IN_QUAD_UpdateTargetVariablePropertyStatus() ||
		   GET_DATA_FROM_CONTROL_VECTOR_UpdateTargetVariablePropertyStatus() ||
		   SET_DATA_TO_CONTROL_VECTOR_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_COLOR_UpdateTargetVariablePropertyStatus() ||
		   GET_COLOR_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_COLOR_COMPONENT_UpdateTargetVariablePropertyStatus() ||
		   GET_COLOR_COMPONENT_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_T_UpdateTargetVariablePropertyStatus() ||
		   GET_FLOAT_LERP_T_UpdateTargetVariablePropertyStatus() ||
		   GET_FLOAT_LERP_CURRENT_UpdateTargetVariablePropertyStatus() ||
		   GET_FLOAT_LERP_DELTA_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_TEXTURE_FLIP_UpdateTargetVariablePropertyStatus() ||
		   GET_DATA_FROM_CONTROL_QUATERNION_UpdateTargetVariablePropertyStatus() ||
		   SET_DATA_TO_CONTROL_QUATERNION_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_FREEZE_UpdateTargetVariablePropertyStatus() ||
		   ROTATE_LIST_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_DELTA_SCALE_UpdateTargetVariablePropertyStatus() ||
		   ENABLE_SHOW_UpdateTargetVariablePropertyStatus() ||
		   MOVE_AROUND_PIVOT_UpdateTargetVariablePropertyStatus() ||
		   CREATE_BRUSH_UpdateTargetVariablePropertyStatus() ||
		   CHECK_TILE_EXIST_UpdateTargetVariablePropertyStatus() ||
		   CHECK_TILE_SOLID_UpdateTargetVariablePropertyStatus() ||
		   CHECK_TILE_PASS_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_TILE_PASS_UpdateTargetVariablePropertyStatus() ||
		   CREATE_SOLID_TILE_UpdateTargetVariablePropertyStatus() ||
		   GET_BRUSH_OF_TILE_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_TILED_SPACE_SIZE_UpdateTargetVariablePropertyStatus() ||
		   SET_PUBLIC_TILE_UpdateTargetVariablePropertyStatus() ||
		   SET_PUBLIC_TEXTURE_UpdateTargetVariablePropertyStatus() ||
		   GET_TILE_CENTER_UpdateTargetVariablePropertyStatus() ||
		   MOVE_TILE_FROM_TO_UpdateTargetVariablePropertyStatus() ||
		   SWAP_TWO_TILES_UpdateTargetVariablePropertyStatus() ||
		   COPY_TILE_UpdateTargetVariablePropertyStatus() ||
		   GET_VIEWING_POINT_UpdateTargetVariablePropertyStatus() ||
		   GET_CAMERA_LOOK_UpdateTargetVariablePropertyStatus() ||
		   GET_VECTOR_LERP_PATH_UpdateTargetVariablePropertyStatus() ||
		   CHANGE_STRIP_POINTS_UpdateTargetVariablePropertyStatus() ||
		   GET_VECTOR_LERP_T_UpdateTargetVariablePropertyStatus() ||
		   GET_QUATERNION_LERP_T_UpdateTargetVariablePropertyStatus() ||
		   GET_QUATERNION_LERP_PATH_UpdateTargetVariablePropertyStatus() ||
		   GET_LIST_LENGTH_UpdateTargetVariablePropertyStatus() ||
		   GET_CONTROL_VECTORS_UpdateTargetVariablePropertyStatus() ||
		   GET_CONTROL_QUATERNIONS_UpdateTargetVariablePropertyStatus() ||
		   CHECK_COLLISION_UpdateTargetVariablePropertyStatus() || 
		   SWAP_ITEMS_UpdateTargetVariablePropertyStatus() || 
		   REMOVE_FROM_LIST_UpdateTargetVariablePropertyStatus() ||
		   GET_RANDOM_VALUE_UpdateTargetVariablePropertyStatus() ||
		   CREATE_ROTATION_FROM_TO_UpdateTargetVariablePropertyStatus())
		{}  
		else 
		{ 
			if(AssetLibrary::_IsAssetExist(item(TARGET_PROPERTY, VALUE)->text().toStdString()))
			{
				SetPropertyValid(TARGET_PROPERTY, true);
			}
			else
			{
				SetPropertyValid(TARGET_PROPERTY, true);
				QString text = "Asset does not exist";
				item(TARGET_PROPERTY, VALUE)->setToolTip(text);
				item(TARGET_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}



void ActionTableWidget::UpdateTargetTemplatePropertyStatus(void)
{
	StringExprParser parser;

	if(parser.Parse(item(TARGET_PROPERTY, VALUE)->text().toStdString()) != StringExprParser::NO_ERRORS)
	{
		SetPropertyValid(TARGET_PROPERTY, false);
		
		QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
		item(TARGET_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{
		SetPropertyValid(TARGET_PROPERTY, true);
		if(!IsStringExprCompute(item(TARGET_PROPERTY, VALUE)->text()))
		{
			QString text = "One or more args does not exist";
			item(TARGET_PROPERTY, VALUE)->setToolTip(text);
			item(TARGET_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
		}
	}
}



void ActionTableWidget::UpdateTargetPropertyStatus(void)
{
	if(IsEnabled(TARGET_PROPERTY))
	{ 
		if((item(TYPE_PROPERTY, VALUE)->text() == "CHECK_COLLISION" && (item(ARG1_PROPERTY, VALUE)->text() == "OBJECT-OBJECT" || item(ARG1_PROPERTY, VALUE)->text() == "GROUP-GROUP")) ||
		   (item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS" && (item(ARG1_PROPERTY, VALUE)->text() == "ITEM" || item(ARG1_PROPERTY, VALUE)->text() == "LIST")))
		{}
		else
		{
			if(item(TARGET_PROPERTY, VALUE)->text().length() > 0)
			{
				if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
				{
					UpdateTargetVariablePropertyStatus();
				}
				else if(item(TARGET_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
				{
					UpdateTargetTemplatePropertyStatus();
				}
			}
			else 
			{ 
				SetPropertyValid(TARGET_PROPERTY, false);
			}
		}
	}
	else
	{
		item(TARGET_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
}




void ActionTableWidget::UpdateStatus(void) 
{ 
	UpdateNamePropertyStatus();
	
	UpdateTypePropertyStatus();

	UpdateArg1PropertyStatus();

	UpdateArg2PropertyStatus();

	UpdateArg3PropertyStatus();
	
	UpdateArg4PropertyStatus();

	UpdateArg5PropertyStatus();

	UpdateTargetPropertyStatus();
}


bool ActionTableWidget::IsEnabled(int _row)const
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		return propertyEnabled[_row];
	}
	return false;
}


void ActionTableWidget::keyPressEvent(QKeyEvent* _event)
{
	if(_event->key() == Qt::Key_Return)
	{
		QTableWidgetItem* it = currentItem();
		
		if(it)
		{
			QModelIndex index = indexFromItem(it);
			int row = index.row();

			QString type = item(TYPE_PROPERTY, VALUE)->text();
			
			UpdateItem(type, it, row);
			
			editItem(it);
		}
	}
}



void ActionTableWidget::mouseDoubleClickEvent(QMouseEvent* _event)
{
	if(_event->button() == Qt::LeftButton)
	{
		QTableWidgetItem* it = currentItem();

		if(it)
		{
			QModelIndex index = indexFromItem(it);
			int row = index.row();

			QString type = item(TYPE_PROPERTY, VALUE)->text();
			
			UpdateItem(type, it, row);
			
			editItem(it);
		}
	}
} 

void ActionTableWidget::UpdateItem(QString _actionType, QTableWidgetItem* _it, int _row)
{ 
	if(_row == ARG1_PROPERTY || 
	   _row == ARG2_PROPERTY || 
	   _row == ARG3_PROPERTY || 
	   _row == ARG4_PROPERTY || 
	   _row == ARG5_PROPERTY || 
	   _row == TARGET_PROPERTY)
	{
		QModelIndex index = indexFromItem(_it);
		delegate->SetCellDelegate(index, GetEditorTypeForCell(_actionType, static_cast<ItemAction*>(_it)));
	} 
}



void ActionTableWidget::CreateAutoName(const QModelIndex& _index)
{
	if(IsAutoNameEnabled(_index.row(), _index.column()))
	{
		QString newName = GetFirstName();
		
		newName += "_" + GetLastName(item(TYPE_PROPERTY, VALUE)->text());
	
		newName += CHANGE_NAME_CreateAutoName();
		newName += CREATE_TIMER_CreateAutoName();
		newName += CREATE_CAMERA_CreateAutoName();
		newName += CREATE_MESH_CreateAutoName();
		newName += CREATE_SET_CreateAutoName();
		newName += CREATE_PLAYER_CONTROL_CreateAutoName();
		newName += CREATE_TEXTURED_ANIMATION_CreateAutoName();
		newName += CREATE_TEXTURE_CreateAutoName();
		newName += GET_SYSTEM_TIME_CreateAutoName();
		newName += CREATE_TRIGGER_CreateAutoName();
		newName += CHANGE_VALUE_CreateAutoName();
		newName += LOAD_ASSET_CreateAutoName();
		newName += GET_NAME_CreateAutoName();
		newName += CHECK_OBJECT_EXIST_CreateAutoName();
		newName += CHECK_LIST_EMPTY_CreateAutoName();
		newName += SEND_MESSAGE_CreateAutoName();
		newName += DESTROY_OBJECT_CreateAutoName();
		newName += SAVE_TO_FILE_CreateAutoName();
		newName += SET_PRIVATE_ASSET_CreateAutoName();
		newName += CHANGE_ANIMATION_FRAME_CreateAutoName();
		newName += RESIZE_SET_CreateAutoName();
		newName += CHANGE_ACTIVATION_LIMIT_CreateAutoName();
		newName += CHANGE_DEACTIVATION_MODE_CreateAutoName();
		newName += CREATE_EMPTY_TILE_CreateAutoName();
		newName += REMOVE_BRUSH_CreateAutoName();
		newName += GET_AMOUNT_OF_FRAMES_CreateAutoName();
		newName += GET_FRAME_SIZE_CreateAutoName();
		newName += CHANGE_ATLAS_CreateAutoName();
		newName += GET_TEXTURE_SIZE_CreateAutoName();
		newName += CHANGE_RANDOM_SEED_CreateAutoName();
		newName += CHANGE_ITEM_VALUE_CreateAutoName();
		newName += CHANGE_LIST_VALUE_CreateAutoName();
		newName += ENABLE_ROTATION_RANGE_CreateAutoName();
		newName += ADD_TO_LIST_CreateAutoName();
		newName += TILED_SPACE_FILL_CreateAutoName();
		newName += CHANGE_ROTATION_RANGE_CreateAutoName();
		newName += ADD_INPUT_EVENT_CreateAutoName();
		newName += SET_ONE_INPUT_EVENT_CreateAutoName();
		newName += CREATE_TILED_SPACE_CreateAutoName();
		newName += GET_DATA_FROM_ITEM_CreateAutoName();
		newName += GET_ITEM_INDEX_CreateAutoName();
		newName += SET_DATA_TO_SLOT_CreateAutoName();
		newName += GET_DATA_FROM_SLOT_CreateAutoName();
		newName += RANDOMIZE_LIST_CreateAutoName();
		newName += GET_AMOUNT_OF_ITEMS_CreateAutoName();
		newName += GET_AMOUNT_OF_SLOTS_CreateAutoName();
	    newName += COPY_LIST_CreateAutoName();
		newName += CLEAR_LIST_CreateAutoName();
		newName += CLEAR_INPUT_EVENTS_CreateAutoName();
		newName += REPLAY_CreateAutoName();
		newName += CREATE_LIST_CreateAutoName();
		newName += CREATE_VARIABLE_CreateAutoName();
		newName += CHANGE_BACKGROUND_COLOR_CreateAutoName();
		newName += GET_SCALE_CreateAutoName();
		newName += DISPLACE_SCALE_CreateAutoName();
		newName += REPLACE_SCALE_CreateAutoName();
		newName += GET_POSITION_CreateAutoName();
		newName += REPLACE_POSITION_CreateAutoName();
		newName += DISPLACE_POSITION_CreateAutoName();
		newName += GET_ROTATION_CreateAutoName();
		newName += REPLACE_ROTATION_CreateAutoName();
		newName += DISPLACE_ROTATION_CreateAutoName();
		newName += CHANGE_PLANE_NORMAL_CreateAutoName();
		newName += CHANGE_MESH_CreateAutoName();
		newName += CREATE_QUAD_CreateAutoName();
		newName += CREATE_BOX_CreateAutoName();
		newName += CHANGE_INTERSECTION_MODE_CreateAutoName();
		newName += CHECK_PROPERTY_CreateAutoName();
		newName += ENABLE_PROPERTY_CreateAutoName();
		newName += ENABLE_BACK_FACE_CULLING_CreateAutoName();
		newName += ENABLE_ZOOM_CreateAutoName();
		newName += ENABLE_ROTATION_CreateAutoName();
		newName += ENABLE_STATIC_OBJECT_CreateAutoName();
		newName += ENABLE_POSITION_OFFSET_CreateAutoName();
		newName += CHANGE_FRONT_TEXTURE_CreateAutoName();
		newName += CHANGE_BACK_TEXTURE_CreateAutoName();
		newName += CHANGE_FRONT_TEXTURED_ANIMATION_CreateAutoName();
		newName += CHANGE_BACK_TEXTURED_ANIMATION_CreateAutoName();
		newName += ENABLE_SHOW_CURSOR_CreateAutoName();
		newName += ENABLE_Z_BUFFER_CreateAutoName();
		newName += ENABLE_TRIGGERS_BLOCK_CreateAutoName();
		newName += CREATE_KEYBOARD_EVENT_CreateAutoName();
		newName += PICK_SCENE_CreateAutoName();
		newName += ADD_TO_LAYER_CreateAutoName();
		newName += REMOVE_FROM_LAYER_CreateAutoName();
		newName += ADD_TO_UPDATE_LIST_CreateAutoName();
		newName += REMOVE_FROM_UPDATE_LIST_CreateAutoName();
		newName += ADD_TO_RENDER_LIST_CreateAutoName();
		newName += REMOVE_FROM_RENDER_LIST_CreateAutoName();
		newName += PICK_CAMERA_CreateAutoName();
		newName += INVERSE_LIST_CreateAutoName();
		newName += CHANGE_CAMERA_CreateAutoName();
		newName += CHANGE_RENDER_TARGET_CreateAutoName();
		newName += CHANGE_FLOAT_LERP_MODE_CreateAutoName();
		newName += CHANGE_PROJECTION_MODE_CreateAutoName();
		newName += CHANGE_MOVEMENT_MODE_CreateAutoName();
		newName += CHANGE_ROTATION_MODE_CreateAutoName();
		newName += GET_SELECT_RAY_CreateAutoName();
		newName += CREATE_FLOAT_LERP_CreateAutoName();
		newName += CREATE_QUATERNION_LERP_CreateAutoName();
		newName += CREATE_VECTOR_LERP_CreateAutoName();
		newName += CHANGE_FLOAT_LERP_SPEED_CreateAutoName();
		newName += STREAMING_FLOAT_CURRENT_CreateAutoName();
		newName += CHANGE_FLOAT_LERP_TARGET_CreateAutoName();
		newName += STREAMING_FLOAT_DELTA_CreateAutoName();
		newName += CHANGE_VECTOR_LERP_MODE_CreateAutoName();
	    newName += CHANGE_FLOAT_LERP_CreateAutoName();
		newName += CHANGE_PLAYING_MODE_CreateAutoName();
		newName += CHANGE_PLAYING_STATE_CreateAutoName();
		newName += CHANGE_PLAYING_DURATION_CreateAutoName();
		newName += CHANGE_INPUT_TIMING_CreateAutoName();
		newName += STREAMING_VECTOR_DELTA_CreateAutoName();
		newName += STREAMING_QUATERNION_DELTA_CreateAutoName();
		newName += STREAMING_VECTOR_CURRENT_CreateAutoName();
		newName += STREAMING_QUATERNION_CURRENT_CreateAutoName();
		newName += STREAMING_FLOAT_T_CreateAutoName();
		newName += CHECK_LINE_IN_IMAGE_CreateAutoName();
		newName += CHECK_LINE_IN_POLYGON_CreateAutoName();
		newName += ROTATE_AROUND_PIVOT_CreateAutoName();
		newName += CHECK_LINE_IN_SURFACE_CreateAutoName();
		newName += SET_TWO_VECTORS_CreateAutoName();
		newName += SET_TWO_QUATERNIONS_CreateAutoName();
		newName += CHANGE_VALUE_RANGE_CreateAutoName();
		newName += CHANGE_CONTROL_VECTORS_CreateAutoName();
		newName += CHANGE_CONTROL_QUATERNIONS_CreateAutoName();
		newName += ENABLE_COLLISION_POLYGONS_INFO_CreateAutoName();
		newName += ENABLE_COLLISION_CHECKING_CreateAutoName();
		newName += CHANGE_COLLISION_GROUP_CreateAutoName();
		newName += CHECK_COLLISION_CreateAutoName();
		newName += SWAP_ITEMS_CreateAutoName();
		newName += REMOVE_FROM_LIST_CreateAutoName();
		newName += GET_RANDOM_VALUE_CreateAutoName();
		newName += CREATE_ROTATION_FROM_TO_CreateAutoName();
		newName += CREATE_ROTATION_CreateAutoName();
		newName += CHANGE_BOX_TEXTURE_MAP_CreateAutoName();
		newName += CREATE_BOX_TEXTURE_MAP_CreateAutoName();
		newName += GET_LINE_ANGLE_Z_CreateAutoName();
		newName += CHANGE_VIEWING_POINT_CreateAutoName();
		newName += CHANGE_DISTANCE_TO_VIEWING_POINT_CreateAutoName();
		newName += CHANGE_ZOOM_NEAR_CreateAutoName();
		newName += CHANGE_ZOOM_FAR_CreateAutoName();
		newName += CHANGE_ZOOM_SPEED_CreateAutoName();
		newName += CHANGE_HITBOX_CreateAutoName();
		newName += CAMERA_LOOK_AT_CreateAutoName();
		newName += CHANGE_VECTOR_COMPONENT_CreateAutoName();
		newName += GET_VECTOR_COMPONENT_CreateAutoName();
		newName += VECTOR_TO_SPHERICAL_CreateAutoName();
		newName += GET_VECTOR_ANGLE_CreateAutoName();
		newName += GET_NORMALIZED_VECTOR_CreateAutoName();
		newName += GET_VECTOR_LENGTH_CreateAutoName();
		newName += CHECK_FRUSTUM_IN_QUAD_CreateAutoName();
		newName += GET_DATA_FROM_CONTROL_VECTOR_CreateAutoName();
		newName += SET_DATA_TO_CONTROL_VECTOR_CreateAutoName();
		newName += GET_LINE_LENGTH_CreateAutoName();
		newName += GET_CIRCLE_LENGTH_CreateAutoName();
		newName += CHANGE_COLOR_CreateAutoName();
		newName += GET_COLOR_CreateAutoName();
		newName += CHANGE_COLOR_COMPONENT_CreateAutoName();
		newName += GET_COLOR_COMPONENT_CreateAutoName();
		newName += CHANGE_FLOAT_LERP_T_CreateAutoName();
		newName += GET_FLOAT_LERP_T_CreateAutoName();
		newName += GET_FLOAT_LERP_CURRENT_CreateAutoName();
		newName += GET_FLOAT_LERP_DELTA_CreateAutoName();
		newName += ENABLE_TEXTURE_FLIP_CreateAutoName();
		newName += GET_DATA_FROM_CONTROL_QUATERNION_CreateAutoName();
		newName += SET_DATA_TO_CONTROL_QUATERNION_CreateAutoName();
		newName += ENABLE_FREEZE_CreateAutoName();
		newName += ROTATE_LIST_CreateAutoName();
		newName += CHANGE_DELTA_SCALE_CreateAutoName();
		newName += ENABLE_SHOW_CreateAutoName();
		newName += GET_LINE_DIRECTION_CreateAutoName();
		newName += MOVE_AROUND_PIVOT_CreateAutoName();
		newName += CREATE_BRUSH_CreateAutoName();
		newName += CHECK_TILE_EXIST_CreateAutoName();
		newName += CHECK_TILE_SOLID_CreateAutoName();
		newName += CHECK_TILE_PASS_CreateAutoName();
		newName += CHANGE_TILE_PASS_CreateAutoName();
		newName += CREATE_SOLID_TILE_CreateAutoName();
		newName += GET_BRUSH_OF_TILE_CreateAutoName();
		newName += CHANGE_TILED_SPACE_SIZE_CreateAutoName();
		newName += SET_PUBLIC_TILE_CreateAutoName();
		newName += SET_PUBLIC_TEXTURE_CreateAutoName();
		newName += GET_TILE_CENTER_CreateAutoName();
		newName += MOVE_TILE_FROM_TO_CreateAutoName();
		newName += SWAP_TWO_TILES_CreateAutoName();
		newName += COPY_TILE_CreateAutoName();
		newName += GET_VIEWING_POINT_CreateAutoName();
		newName += GET_CAMERA_LOOK_CreateAutoName();
		newName += GET_VECTOR_LERP_PATH_CreateAutoName();
		newName += CHANGE_STRIP_POINTS_CreateAutoName();
		newName += GET_VECTOR_LERP_T_CreateAutoName();
		newName += GET_QUATERNION_LERP_T_CreateAutoName();
		newName += GET_QUATERNION_LERP_PATH_CreateAutoName();
		newName += GET_LIST_LENGTH_CreateAutoName();
		newName += GET_RADIUS_FROM_2_POINTS_CreateAutoName();
		newName += GET_RADIUS_FROM_3_POINTS_CreateAutoName();
		newName += GET_CONTROL_VECTORS_CreateAutoName();
		newName += GET_CONTROL_QUATERNIONS_CreateAutoName();
		newName += CHECK_EQUALITY_CreateAutoName();

		
		item(NAME_PROPERTY, VALUE)->setText(newName);
	}
}



void ActionTableWidget::UpdateRow(QTableWidgetItem* _it)
{
	ItemAction* it = static_cast<ItemAction*>(_it);
	
	QModelIndex index = indexFromItem(_it); 

	
	CreateAutoName(index);
	QString type = item(TYPE_PROPERTY, VALUE)->text();
	
	CHANGE_NAME_UpdateRow(index, type);
	CREATE_TIMER_UpdateRow(index, type);
	CREATE_CAMERA_UpdateRow(index, type);
	CREATE_MESH_UpdateRow(index, type);
	CREATE_SET_UpdateRow(index, type);
	CREATE_PLAYER_CONTROL_UpdateRow(index, type);
	CREATE_TEXTURED_ANIMATION_UpdateRow(index, type);
	CREATE_TEXTURE_UpdateRow(index, type);
	GET_SYSTEM_TIME_UpdateRow(index, type);
	CREATE_TRIGGER_UpdateRow(index, type);
	CHANGE_VALUE_UpdateRow(index, type);
	LOAD_ASSET_UpdateRow(index, type);
	GET_NAME_UpdateRow(index, type);
	CHECK_OBJECT_EXIST_UpdateRow(index, type);
	CHECK_LIST_EMPTY_UpdateRow(index, type);
	SEND_MESSAGE_UpdateRow(index, type);
	DESTROY_OBJECT_UpdateRow(index, type);
	SAVE_TO_FILE_UpdateRow(index, type);
	SET_PRIVATE_ASSET_UpdateRow(index, type);
	CHANGE_ANIMATION_FRAME_UpdateRow(index, type);
	RESIZE_SET_UpdateRow(index, type);
	CHANGE_ACTIVATION_LIMIT_UpdateRow(index, type);
	CHANGE_DEACTIVATION_MODE_UpdateRow(index, type);
	CREATE_EMPTY_TILE_UpdateRow(index, type);
	REMOVE_BRUSH_UpdateRow(index, type);
	GET_AMOUNT_OF_FRAMES_UpdateRow(index, type);
	GET_FRAME_SIZE_UpdateRow(index, type);
	CHANGE_ATLAS_UpdateRow(index, type);
	GET_TEXTURE_SIZE_UpdateRow(index, type);
	CHANGE_RANDOM_SEED_UpdateRow(index, type);
	CHANGE_ITEM_VALUE_UpdateRow(index, type);
	CHANGE_LIST_VALUE_UpdateRow(index, type);
	ENABLE_ROTATION_RANGE_UpdateRow(index, type);
	ADD_TO_LIST_UpdateRow(index, type);
	TILED_SPACE_FILL_UpdateRow(index, type);
	CHANGE_ROTATION_RANGE_UpdateRow(index, type);
	ADD_INPUT_EVENT_UpdateRow(index, type);
	SET_ONE_INPUT_EVENT_UpdateRow(index, type);
	CREATE_TILED_SPACE_UpdateRow(index, type);
	GET_DATA_FROM_ITEM_UpdateRow(index, type);
	GET_ITEM_INDEX_UpdateRow(index, type);
	SET_DATA_TO_SLOT_UpdateRow(index, type);
	GET_DATA_FROM_SLOT_UpdateRow(index, type);
	RANDOMIZE_LIST_UpdateRow(index, type);
	GET_AMOUNT_OF_ITEMS_UpdateRow(index, type);
	GET_AMOUNT_OF_SLOTS_UpdateRow(index, type);
	COPY_LIST_UpdateRow(index, type);
	CLEAR_LIST_UpdateRow(index, type);
	CLEAR_INPUT_EVENTS_UpdateRow(index, type);
	REPLAY_UpdateRow(index, type);
	CREATE_LIST_UpdateRow(index, type);
	CREATE_VARIABLE_UpdateRow(index, type);
	CHANGE_BACKGROUND_COLOR_UpdateRow(index, type);
	GET_SCALE_UpdateRow(index, type);
	DISPLACE_SCALE_UpdateRow(index, type);
	REPLACE_SCALE_UpdateRow(index, type);
	GET_POSITION_UpdateRow(index, type);
	REPLACE_POSITION_UpdateRow(index, type);
	DISPLACE_POSITION_UpdateRow(index, type);
	GET_ROTATION_UpdateRow(index, type);
	REPLACE_ROTATION_UpdateRow(index, type);
	DISPLACE_ROTATION_UpdateRow(index, type);
	CHANGE_PLANE_NORMAL_UpdateRow(index, type);
	CHANGE_MESH_UpdateRow(index, type);
	CREATE_QUAD_UpdateRow(index, type);
	CREATE_BOX_UpdateRow(index, type);
	CHANGE_INTERSECTION_MODE_UpdateRow(index, type);
	CHECK_PROPERTY_UpdateRow(index, type);
	ENABLE_PROPERTY_UpdateRow(index, type);
	ENABLE_BACK_FACE_CULLING_UpdateRow(index, type);
	ENABLE_ZOOM_UpdateRow(index, type);
	ENABLE_ROTATION_UpdateRow(index, type);
	ENABLE_STATIC_OBJECT_UpdateRow(index, type);
	ENABLE_POSITION_OFFSET_UpdateRow(index, type);
	CHANGE_FRONT_TEXTURE_UpdateRow(index, type);
	CHANGE_BACK_TEXTURE_UpdateRow(index, type);
	CHANGE_FRONT_TEXTURED_ANIMATION_UpdateRow(index, type);
	CHANGE_BACK_TEXTURED_ANIMATION_UpdateRow(index, type);
	ENABLE_SHOW_CURSOR_UpdateRow(index, type);
	ENABLE_Z_BUFFER_UpdateRow(index, type);
	ENABLE_TRIGGERS_BLOCK_UpdateRow(index, type);
	CREATE_KEYBOARD_EVENT_UpdateRow(index, type);
	PICK_SCENE_UpdateRow(index, type);
	ADD_TO_LAYER_UpdateRow(index, type);
	REMOVE_FROM_LAYER_UpdateRow(index, type);
	ADD_TO_UPDATE_LIST_UpdateRow(index, type);
	REMOVE_FROM_UPDATE_LIST_UpdateRow(index, type);
	ADD_TO_RENDER_LIST_UpdateRow(index, type);
	REMOVE_FROM_RENDER_LIST_UpdateRow(index, type);
	PICK_CAMERA_UpdateRow(index, type);
	INVERSE_LIST_UpdateRow(index, type);
	CHANGE_CAMERA_UpdateRow(index, type);
	CHANGE_RENDER_TARGET_UpdateRow(index, type);
	CHANGE_FLOAT_LERP_MODE_UpdateRow(index, type);
	CHANGE_PROJECTION_MODE_UpdateRow(index, type);
	CHANGE_MOVEMENT_MODE_UpdateRow(index, type);
	CHANGE_ROTATION_MODE_UpdateRow(index, type);
	GET_SELECT_RAY_UpdateRow(index, type);
	CREATE_FLOAT_LERP_UpdateRow(index, type);
	CREATE_QUATERNION_LERP_UpdateRow(index, type);
	CREATE_VECTOR_LERP_UpdateRow(index, type);
	CHANGE_FLOAT_LERP_SPEED_UpdateRow(index, type);
	STREAMING_FLOAT_CURRENT_UpdateRow(index, type);
	CHANGE_FLOAT_LERP_TARGET_UpdateRow(index, type);
	STREAMING_FLOAT_DELTA_UpdateRow(index, type);
	CHANGE_VECTOR_LERP_MODE_UpdateRow(index, type);
	CHANGE_FLOAT_LERP_UpdateRow(index, type);
	CHANGE_PLAYING_MODE_UpdateRow(index, type);
	CHANGE_PLAYING_STATE_UpdateRow(index, type);
	CHANGE_PLAYING_DURATION_UpdateRow(index, type);
	CHANGE_INPUT_TIMING_UpdateRow(index, type);
	STREAMING_VECTOR_DELTA_UpdateRow(index, type);
	STREAMING_QUATERNION_DELTA_UpdateRow(index, type);
	STREAMING_VECTOR_CURRENT_UpdateRow(index, type);
	STREAMING_QUATERNION_CURRENT_UpdateRow(index, type);
	STREAMING_FLOAT_T_UpdateRow(index, type);
	CHECK_LINE_IN_IMAGE_UpdateRow(index, type);
	CHECK_LINE_IN_POLYGON_UpdateRow(index, type);
	ROTATE_AROUND_PIVOT_UpdateRow(index, type);
	CHECK_LINE_IN_SURFACE_UpdateRow(index, type);
	SET_TWO_VECTORS_UpdateRow(index, type);
	SET_TWO_QUATERNIONS_UpdateRow(index, type);
	CHANGE_VALUE_RANGE_UpdateRow(index, type);
	CHANGE_CONTROL_VECTORS_UpdateRow(index, type);
	CHANGE_CONTROL_QUATERNIONS_UpdateRow(index, type);
	ENABLE_COLLISION_POLYGONS_INFO_UpdateRow(index, type);
	ENABLE_COLLISION_CHECKING_UpdateRow(index, type);
	CHANGE_COLLISION_GROUP_UpdateRow(index, type);
	CHECK_COLLISION_UpdateRow(index, type);
	SWAP_ITEMS_UpdateRow(index, type);
	REMOVE_FROM_LIST_UpdateRow(index, type);
	GET_RANDOM_VALUE_UpdateRow(index, type);
	CREATE_ROTATION_FROM_TO_UpdateRow(index, type);
	CREATE_ROTATION_UpdateRow(index, type);
	CHANGE_BOX_TEXTURE_MAP_UpdateRow(index, type);
	CREATE_BOX_TEXTURE_MAP_UpdateRow(index, type);
	GET_LINE_ANGLE_Z_UpdateRow(index, type);
	CHANGE_VIEWING_POINT_UpdateRow(index, type);
	CHANGE_DISTANCE_TO_VIEWING_POINT_UpdateRow(index, type);
	CHANGE_ZOOM_NEAR_UpdateRow(index, type);
	CHANGE_ZOOM_FAR_UpdateRow(index, type);
	CHANGE_ZOOM_SPEED_UpdateRow(index, type);
	CHANGE_HITBOX_UpdateRow(index, type);
	CAMERA_LOOK_AT_UpdateRow(index, type);
	CHANGE_VECTOR_COMPONENT_UpdateRow(index, type);
	GET_VECTOR_COMPONENT_UpdateRow(index, type);
	VECTOR_TO_SPHERICAL_UpdateRow(index, type);
	GET_VECTOR_ANGLE_UpdateRow(index, type);
	GET_NORMALIZED_VECTOR_UpdateRow(index, type);
	GET_VECTOR_LENGTH_UpdateRow(index, type);
	CHECK_FRUSTUM_IN_QUAD_UpdateRow(index, type);
	GET_DATA_FROM_CONTROL_VECTOR_UpdateRow(index, type);
	SET_DATA_TO_CONTROL_VECTOR_UpdateRow(index, type);
	GET_LINE_LENGTH_UpdateRow(index, type);
	GET_CIRCLE_LENGTH_UpdateRow(index, type);
	CHANGE_COLOR_UpdateRow(index, type);
	GET_COLOR_UpdateRow(index, type);
	CHANGE_COLOR_COMPONENT_UpdateRow(index, type);
	GET_COLOR_COMPONENT_UpdateRow(index, type);
	CHANGE_FLOAT_LERP_T_UpdateRow(index, type);
	GET_FLOAT_LERP_T_UpdateRow(index, type);
	GET_FLOAT_LERP_CURRENT_UpdateRow(index, type);
	GET_FLOAT_LERP_DELTA_UpdateRow(index, type);
	ENABLE_TEXTURE_FLIP_UpdateRow(index, type);
	GET_DATA_FROM_CONTROL_QUATERNION_UpdateRow(index, type);
	SET_DATA_TO_CONTROL_QUATERNION_UpdateRow(index, type);
	ENABLE_FREEZE_UpdateRow(index, type);
	ROTATE_LIST_UpdateRow(index, type);
	CHANGE_DELTA_SCALE_UpdateRow(index, type);
	ENABLE_SHOW_UpdateRow(index, type);
	GET_LINE_DIRECTION_UpdateRow(index, type);
	MOVE_AROUND_PIVOT_UpdateRow(index, type);
	CREATE_BRUSH_UpdateRow(index, type);
	CHECK_TILE_EXIST_UpdateRow(index, type);
	CHECK_TILE_SOLID_UpdateRow(index, type);
	CHECK_TILE_PASS_UpdateRow(index, type);
	CHANGE_TILE_PASS_UpdateRow(index, type);
	CREATE_SOLID_TILE_UpdateRow(index, type);
	GET_BRUSH_OF_TILE_UpdateRow(index, type);
	CHANGE_TILED_SPACE_SIZE_UpdateRow(index, type);
	SET_PUBLIC_TILE_UpdateRow(index, type);
	SET_PUBLIC_TEXTURE_UpdateRow(index, type);
	GET_TILE_CENTER_UpdateRow(index, type);
	MOVE_TILE_FROM_TO_UpdateRow(index, type);
	SWAP_TWO_TILES_UpdateRow(index, type);
	COPY_TILE_UpdateRow(index, type);
	GET_VIEWING_POINT_UpdateRow(index, type);
	GET_CAMERA_LOOK_UpdateRow(index, type);
	GET_VECTOR_LERP_PATH_UpdateRow(index, type);
	CHANGE_STRIP_POINTS_UpdateRow(index, type);
	GET_VECTOR_LERP_T_UpdateRow(index, type);
	GET_QUATERNION_LERP_T_UpdateRow(index, type);
	GET_QUATERNION_LERP_PATH_UpdateRow(index, type);
	GET_LIST_LENGTH_UpdateRow(index, type);
	GET_RADIUS_FROM_2_POINTS_UpdateRow(index, type);
	GET_RADIUS_FROM_3_POINTS_UpdateRow(index, type);
	GET_CONTROL_VECTORS_UpdateRow(index, type);
	GET_CONTROL_QUATERNIONS_UpdateRow(index, type);
	CHECK_EQUALITY_UpdateRow(index, type);


	blockSignals(true);
 
	if(index.row() == TYPE_PROPERTY && index.column() == VALUE)
	{
		UpdateStatus();
	}
	else if(index.row() == TARGET_PROPERTY && (index.column() == VALUE || index.column() == ARG_MODE))
	{
		UpdateTargetPropertyStatus();
	}
	else if(index.row() == ARG1_PROPERTY && (index.column() == VALUE || index.column() == ARG_MODE))
	{
		UpdateArg1PropertyStatus();
	}
	else if(index.row() == ARG2_PROPERTY && (index.column() == VALUE || index.column() == ARG_MODE))
	{
		UpdateArg2PropertyStatus();
	}
	else if(index.row() == ARG3_PROPERTY && (index.column() == VALUE || index.column() == ARG_MODE))
	{
		UpdateArg3PropertyStatus();
	}
	else if(index.row() == ARG4_PROPERTY && (index.column() == VALUE || index.column() == ARG_MODE))
	{
		UpdateArg4PropertyStatus();
	}
	else if(index.row() == ARG5_PROPERTY && (index.column() == VALUE || index.column() == ARG_MODE))
	{
		UpdateArg5PropertyStatus();
	} 
	else if(index.row() == NAME_PROPERTY && index.column() == VALUE)
	{
		UpdateNamePropertyStatus();
	}

	blockSignals(false);
}





int ActionTableWidget::GetEditorTypeForCell(QString _actionType, ItemAction* _it)
{
	QModelIndex index = indexFromItem(_it);

	int editor = UniversalDelegate::NO_EDIT;

	CHANGE_NAME_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_TIMER_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_CAMERA_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_MESH_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_SET_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_PLAYER_CONTROL_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_TEXTURED_ANIMATION_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_TEXTURE_GetEditorTypeForCell(index, _actionType, editor);
	GET_SYSTEM_TIME_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_TRIGGER_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_VALUE_GetEditorTypeForCell(index, _actionType, editor); 
	LOAD_ASSET_GetEditorTypeForCell(index, _actionType, editor); 
	GET_NAME_GetEditorTypeForCell(index, _actionType, editor); 
	CHECK_OBJECT_EXIST_GetEditorTypeForCell(index, _actionType, editor); 
	CHECK_LIST_EMPTY_GetEditorTypeForCell(index, _actionType, editor); 
	SEND_MESSAGE_GetEditorTypeForCell(index, _actionType, editor);
	DESTROY_OBJECT_GetEditorTypeForCell(index, _actionType, editor);
	SAVE_TO_FILE_GetEditorTypeForCell(index, _actionType, editor);
	SET_PRIVATE_ASSET_GetEditorTypeForCell(index, _actionType, editor);
    CHANGE_ANIMATION_FRAME_GetEditorTypeForCell(index, _actionType, editor);
	RESIZE_SET_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_ACTIVATION_LIMIT_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_DEACTIVATION_MODE_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_EMPTY_TILE_GetEditorTypeForCell(index, _actionType, editor);
	REMOVE_BRUSH_GetEditorTypeForCell(index, _actionType, editor);
	GET_AMOUNT_OF_FRAMES_GetEditorTypeForCell(index, _actionType, editor);
	GET_FRAME_SIZE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_ATLAS_GetEditorTypeForCell(index, _actionType, editor);
	GET_TEXTURE_SIZE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_RANDOM_SEED_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_ITEM_VALUE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_LIST_VALUE_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_ROTATION_RANGE_GetEditorTypeForCell(index, _actionType, editor);
	ADD_TO_LIST_GetEditorTypeForCell(index, _actionType, editor);
	TILED_SPACE_FILL_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_ROTATION_RANGE_GetEditorTypeForCell(index, _actionType, editor);
	ADD_INPUT_EVENT_GetEditorTypeForCell(index, _actionType, editor);
	SET_ONE_INPUT_EVENT_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_TILED_SPACE_GetEditorTypeForCell(index, _actionType, editor);
	GET_DATA_FROM_ITEM_GetEditorTypeForCell(index, _actionType, editor);
	GET_ITEM_INDEX_GetEditorTypeForCell(index, _actionType, editor);
	SET_DATA_TO_SLOT_GetEditorTypeForCell(index, _actionType, editor);
	GET_DATA_FROM_SLOT_GetEditorTypeForCell(index, _actionType, editor);
	RANDOMIZE_LIST_GetEditorTypeForCell(index, _actionType, editor);
	GET_AMOUNT_OF_ITEMS_GetEditorTypeForCell(index, _actionType, editor);
	GET_AMOUNT_OF_SLOTS_GetEditorTypeForCell(index, _actionType, editor);
	COPY_LIST_GetEditorTypeForCell(index, _actionType, editor);
	CLEAR_LIST_GetEditorTypeForCell(index, _actionType, editor);
	CLEAR_INPUT_EVENTS_GetEditorTypeForCell(index, _actionType, editor);
	REPLAY_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_LIST_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_VARIABLE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_BACKGROUND_COLOR_GetEditorTypeForCell(index, _actionType, editor);
	GET_SCALE_GetEditorTypeForCell(index, _actionType, editor);
	DISPLACE_SCALE_GetEditorTypeForCell(index, _actionType, editor);
	REPLACE_SCALE_GetEditorTypeForCell(index, _actionType, editor);
	GET_POSITION_GetEditorTypeForCell(index, _actionType, editor);
	REPLACE_POSITION_GetEditorTypeForCell(index, _actionType, editor);
	DISPLACE_POSITION_GetEditorTypeForCell(index, _actionType, editor);
	GET_ROTATION_GetEditorTypeForCell(index, _actionType, editor);
	REPLACE_ROTATION_GetEditorTypeForCell(index, _actionType, editor);
	DISPLACE_ROTATION_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_PLANE_NORMAL_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_MESH_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_QUAD_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_BOX_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_INTERSECTION_MODE_GetEditorTypeForCell(index, _actionType, editor);
	CHECK_PROPERTY_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_PROPERTY_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_BACK_FACE_CULLING_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_ZOOM_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_ROTATION_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_STATIC_OBJECT_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_POSITION_OFFSET_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_FRONT_TEXTURE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_BACK_TEXTURE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_FRONT_TEXTURED_ANIMATION_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_BACK_TEXTURED_ANIMATION_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_SHOW_CURSOR_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_Z_BUFFER_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_TRIGGERS_BLOCK_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_KEYBOARD_EVENT_GetEditorTypeForCell(index, _actionType, editor);
	PICK_SCENE_GetEditorTypeForCell(index, _actionType, editor);
	ADD_TO_LAYER_GetEditorTypeForCell(index, _actionType, editor);
	REMOVE_FROM_LAYER_GetEditorTypeForCell(index, _actionType, editor);
	ADD_TO_UPDATE_LIST_GetEditorTypeForCell(index, _actionType, editor);
	REMOVE_FROM_UPDATE_LIST_GetEditorTypeForCell(index, _actionType, editor);
	ADD_TO_RENDER_LIST_GetEditorTypeForCell(index, _actionType, editor);
	REMOVE_FROM_RENDER_LIST_GetEditorTypeForCell(index, _actionType, editor);
	PICK_CAMERA_GetEditorTypeForCell(index, _actionType, editor);
	INVERSE_LIST_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_CAMERA_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_RENDER_TARGET_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_FLOAT_LERP_MODE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_PROJECTION_MODE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_MOVEMENT_MODE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_ROTATION_MODE_GetEditorTypeForCell(index, _actionType, editor);
	GET_SELECT_RAY_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_FLOAT_LERP_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_QUATERNION_LERP_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_VECTOR_LERP_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_FLOAT_LERP_SPEED_GetEditorTypeForCell(index, _actionType, editor);
	STREAMING_FLOAT_CURRENT_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_FLOAT_LERP_TARGET_GetEditorTypeForCell(index, _actionType, editor);
	STREAMING_FLOAT_DELTA_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_VECTOR_LERP_MODE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_FLOAT_LERP_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_PLAYING_MODE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_PLAYING_STATE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_PLAYING_DURATION_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_INPUT_TIMING_GetEditorTypeForCell(index, _actionType, editor);
	STREAMING_VECTOR_DELTA_GetEditorTypeForCell(index, _actionType, editor);
	STREAMING_QUATERNION_DELTA_GetEditorTypeForCell(index, _actionType, editor);
	STREAMING_VECTOR_CURRENT_GetEditorTypeForCell(index, _actionType, editor);
	STREAMING_QUATERNION_CURRENT_GetEditorTypeForCell(index, _actionType, editor);
	STREAMING_FLOAT_T_GetEditorTypeForCell(index, _actionType, editor);
	CHECK_LINE_IN_IMAGE_GetEditorTypeForCell(index, _actionType, editor);
	CHECK_LINE_IN_POLYGON_GetEditorTypeForCell(index, _actionType, editor);
	ROTATE_AROUND_PIVOT_GetEditorTypeForCell(index, _actionType, editor);
	CHECK_LINE_IN_SURFACE_GetEditorTypeForCell(index, _actionType, editor);
	SET_TWO_VECTORS_GetEditorTypeForCell(index, _actionType, editor);
	SET_TWO_QUATERNIONS_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_VALUE_RANGE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_CONTROL_VECTORS_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_CONTROL_QUATERNIONS_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_COLLISION_POLYGONS_INFO_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_COLLISION_CHECKING_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_COLLISION_GROUP_GetEditorTypeForCell(index, _actionType, editor);
	CHECK_COLLISION_GetEditorTypeForCell(index, _actionType, editor);
	SWAP_ITEMS_GetEditorTypeForCell(index, _actionType, editor);
	REMOVE_FROM_LIST_GetEditorTypeForCell(index, _actionType, editor);
	GET_RANDOM_VALUE_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_ROTATION_FROM_TO_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_ROTATION_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_BOX_TEXTURE_MAP_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_BOX_TEXTURE_MAP_GetEditorTypeForCell(index, _actionType, editor);
	GET_LINE_ANGLE_Z_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_VIEWING_POINT_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_DISTANCE_TO_VIEWING_POINT_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_ZOOM_NEAR_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_ZOOM_FAR_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_ZOOM_SPEED_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_HITBOX_GetEditorTypeForCell(index, _actionType, editor);
	CAMERA_LOOK_AT_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_VECTOR_COMPONENT_GetEditorTypeForCell(index, _actionType, editor);
	GET_VECTOR_COMPONENT_GetEditorTypeForCell(index, _actionType, editor);
	VECTOR_TO_SPHERICAL_GetEditorTypeForCell(index, _actionType, editor);
	GET_VECTOR_ANGLE_GetEditorTypeForCell(index, _actionType, editor);
	GET_NORMALIZED_VECTOR_GetEditorTypeForCell(index, _actionType, editor);
	GET_VECTOR_LENGTH_GetEditorTypeForCell(index, _actionType, editor);
	CHECK_FRUSTUM_IN_QUAD_GetEditorTypeForCell(index, _actionType, editor);
	GET_DATA_FROM_CONTROL_VECTOR_GetEditorTypeForCell(index, _actionType, editor);
	SET_DATA_TO_CONTROL_VECTOR_GetEditorTypeForCell(index, _actionType, editor);
	GET_LINE_LENGTH_GetEditorTypeForCell(index, _actionType, editor);
	GET_CIRCLE_LENGTH_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_COLOR_GetEditorTypeForCell(index, _actionType, editor);
	GET_COLOR_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_COLOR_COMPONENT_GetEditorTypeForCell(index, _actionType, editor);
	GET_COLOR_COMPONENT_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_FLOAT_LERP_T_GetEditorTypeForCell(index, _actionType, editor);
	GET_FLOAT_LERP_T_GetEditorTypeForCell(index, _actionType, editor);
	GET_FLOAT_LERP_CURRENT_GetEditorTypeForCell(index, _actionType, editor);
	GET_FLOAT_LERP_DELTA_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_TEXTURE_FLIP_GetEditorTypeForCell(index, _actionType, editor);
	GET_DATA_FROM_CONTROL_QUATERNION_GetEditorTypeForCell(index, _actionType, editor);
	SET_DATA_TO_CONTROL_QUATERNION_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_FREEZE_GetEditorTypeForCell(index, _actionType, editor);
	ROTATE_LIST_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_DELTA_SCALE_GetEditorTypeForCell(index, _actionType, editor);
	ENABLE_SHOW_GetEditorTypeForCell(index, _actionType, editor);
	GET_LINE_DIRECTION_GetEditorTypeForCell(index, _actionType, editor);
	MOVE_AROUND_PIVOT_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_BRUSH_GetEditorTypeForCell(index, _actionType, editor);
	CHECK_TILE_EXIST_GetEditorTypeForCell(index, _actionType, editor);
	CHECK_TILE_SOLID_GetEditorTypeForCell(index, _actionType, editor);
	CHECK_TILE_PASS_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_TILE_PASS_GetEditorTypeForCell(index, _actionType, editor);
	CREATE_SOLID_TILE_GetEditorTypeForCell(index, _actionType, editor);
	GET_BRUSH_OF_TILE_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_TILED_SPACE_SIZE_GetEditorTypeForCell(index, _actionType, editor);
	SET_PUBLIC_TILE_GetEditorTypeForCell(index, _actionType, editor);
	SET_PUBLIC_TEXTURE_GetEditorTypeForCell(index, _actionType, editor);
	GET_TILE_CENTER_GetEditorTypeForCell(index, _actionType, editor);
	MOVE_TILE_FROM_TO_GetEditorTypeForCell(index, _actionType, editor);
	SWAP_TWO_TILES_GetEditorTypeForCell(index, _actionType, editor);
	COPY_TILE_GetEditorTypeForCell(index, _actionType, editor);
	GET_VIEWING_POINT_GetEditorTypeForCell(index, _actionType, editor);
	GET_CAMERA_LOOK_GetEditorTypeForCell(index, _actionType, editor);
	GET_VECTOR_LERP_PATH_GetEditorTypeForCell(index, _actionType, editor);
	CHANGE_STRIP_POINTS_GetEditorTypeForCell(index, _actionType, editor);
	GET_VECTOR_LERP_T_GetEditorTypeForCell(index, _actionType, editor);
	GET_QUATERNION_LERP_T_GetEditorTypeForCell(index, _actionType, editor);
	GET_QUATERNION_LERP_PATH_GetEditorTypeForCell(index, _actionType, editor);
	GET_LIST_LENGTH_GetEditorTypeForCell(index, _actionType, editor);
	GET_RADIUS_FROM_2_POINTS_GetEditorTypeForCell(index, _actionType, editor);
	GET_RADIUS_FROM_3_POINTS_GetEditorTypeForCell(index, _actionType, editor);
	GET_CONTROL_VECTORS_GetEditorTypeForCell(index, _actionType, editor);
	GET_CONTROL_QUATERNIONS_GetEditorTypeForCell(index, _actionType, editor);
	CHECK_EQUALITY_GetEditorTypeForCell(index, _actionType, editor);

	return editor;
}
