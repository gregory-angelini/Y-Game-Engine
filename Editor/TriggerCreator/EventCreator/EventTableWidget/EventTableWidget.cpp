#include "EventTableWidget.h"
#include <Editor/Editor/Editor.h>
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Editor/TriggerCreator/EnumerationExprLineEdit/EnumerationExprLineEdit.h>
#include <Editor/TriggerCreator/BooleanExprLineEdit/BooleanExprLineEdit.h>
#include <Editor/TriggerCreator/EqualityExprLineEdit/EqualityExprLineEdit.h>
#include <Editor/TriggerCreator/StringExprLineEdit/StringExprLineEdit.h>
#include <Editor/TriggerCreator/ArithmeticExprLineEdit/ArithmeticExprLineEdit.h>




EventTableWidget::EventTableWidget(QWidget* _parent): QTableWidget(MAX_ROWS, MAX_COLUMNS, _parent), autoName(true)
{
	blockSignals(true);
 
	CreateDictionary();

	CreateConstantsForComboBox();
	QStringList columnLabels;
	columnLabels.push_back("EVENT TYPE");
	columnLabels.push_back("SOURCE");
	columnLabels.push_back("ARG1");
	columnLabels.push_back("ARG2");
	columnLabels.push_back("ARG3");
	columnLabels.push_back("ARG4");
	columnLabels.push_back("ARG5");
	columnLabels.push_back("NAME");
	columnLabels.push_back("DEACTIVATION MODE");
	columnLabels.push_back("ACTIVATION STATE");
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
			ItemEvent* it = new ItemEvent;
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
	item(DEACTIVATION_MODE_PROPERTY, VALUE)->setToolTip("mode of deactivation"); 
	SetCellText(DEACTIVATION_MODE_PROPERTY, VALUE, "");
	item(ACTIVATION_STATE_PROPERTY, VALUE)->setToolTip("current state"); 
	SetCellText(ACTIVATION_STATE_PROPERTY, VALUE, "false");
	SetCellText(ACTIVATION_STATE_PROPERTY, CONTENT, "Enable"); 
	SetCellText(ACTIVATION_STATE_PROPERTY, DATA_TYPE, "Boolean");
	item(ARGS_LOADING_MODE_PROPERTY, VALUE)->setToolTip("mode of loading args"); 
	SetCellText(ARGS_LOADING_MODE_PROPERTY, VALUE, "AUTO");
	item(ACTIVATION_LIMIT_PROPERTY, VALUE)->setToolTip("limit of activation"); 
	SetCellText(ACTIVATION_LIMIT_PROPERTY, VALUE, "-1"); 
	SetCellText(ACTIVATION_LIMIT_PROPERTY, CONTENT, "Count"); 
	SetCellText(ACTIVATION_LIMIT_PROPERTY, DATA_TYPE, "Integer");
	delegate = new UniversalDelegate(this);
	setItemDelegate(delegate);
	
	delegate->SetCellDelegate(indexFromItem(item(NAME_PROPERTY, VALUE)), UniversalDelegate::TEXT_EDITOR);
	
	delegate->SetCellDelegate(indexFromItem(item(ACTIVATION_STATE_PROPERTY, VALUE)), UniversalDelegate::BOOL_EDITOR);

	delegate->SetCellDelegate(indexFromItem(item(ACTIVATION_LIMIT_PROPERTY, VALUE)), UniversalDelegate::LIMIT_EDITOR);

	delegate->SetCellDelegate(indexFromItem(item(ARGS_LOADING_MODE_PROPERTY, VALUE)), UniversalDelegate::LOADING_MODE_EDITOR);

	delegate->SetCellDelegate(indexFromItem(item(TYPE_PROPERTY, VALUE)), UniversalDelegate::EVENT_TYPE_EDITOR);
	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(UpdateRow(QTableWidgetItem*)));
    connect(this, SIGNAL(cellClicked(int, int)), this, SLOT(CellIsClicked(int, int)));

	setFixedSize(630+100+50+50+15+10-5+200-3+150+50+22-150, 270+10-5-2+17-10-7-25+75);

	blockSignals(false);
}
 

EventTableWidget::~EventTableWidget(void)
{
	delete delegate;
}


void EventTableWidget::CellIsClicked(int _row, int _column)
{
	if(_row == SOURCE_PROPERTY && _column == VALUE)
	{
		UpdateSourcePropertyStatus();
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


void EventTableWidget::dragMoveEvent(QDragMoveEvent* _event)
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
			else if(index.row() == SOURCE_PROPERTY)
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





bool EventTableWidget::EqualityOperationIsLast(QString _expr)
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


void EventTableWidget::EqualityExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
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


void EventTableWidget::dragEnterEvent(QDragEnterEvent *_event)
{
	if(_event->mimeData()->hasFormat("text/uri-list")) 
	{ 	
		_event->acceptProposedAction();
	}
}


bool EventTableWidget::IsEmpty(QString _expr)
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



void EventTableWidget::dropEvent(QDropEvent *_event)
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
					SEND_MESSAGE_DropEvent(it, index, suffix, fileName);
					CREATE_OBJECT_DropEvent(it, index, suffix, fileName);
					DESTROY_OBJECT_DropEvent(it, index, suffix, fileName);
					TIMER_FINISH_DropEvent(it, index, suffix, fileName);
					FLOAT_LERP_FINISH_DropEvent(it, index, suffix, fileName);
					PICK_SCENE_DropEvent(it, index, suffix, fileName);
					ADD_TO_LAYER_DropEvent(it, index, suffix, fileName);
					REMOVE_FROM_LAYER_DropEvent(it, index, suffix, fileName);
					ADD_TO_UPDATE_LIST_DropEvent(it, index, suffix, fileName);
					REMOVE_FROM_UPDATE_LIST_DropEvent(it, index, suffix, fileName);
					ADD_TO_RENDER_LIST_DropEvent(it, index, suffix, fileName);
					REMOVE_FROM_RENDER_LIST_DropEvent(it, index, suffix, fileName);
					CHANGE_CAMERA_DropEvent(it, index, suffix, fileName);
					ENABLE_POSITION_OFFSET_DropEvent(it, index, suffix, fileName);
					CHANGE_FRONT_TEXTURE_DropEvent(it, index, suffix, fileName);
					CHANGE_BACK_TEXTURE_DropEvent(it, index, suffix, fileName);
					CHANGE_FRONT_TEXTURED_ANIMATION_DropEvent(it, index, suffix, fileName);
					CHANGE_BACK_TEXTURED_ANIMATION_DropEvent(it, index, suffix, fileName);
					CHANGE_ANIMATION_FRAME_DropEvent(it, index, suffix, fileName);
					ENABLE_PROPERTY_DropEvent(it, index, suffix, fileName);
					CHANGE_ATLAS_DropEvent(it, index, suffix, fileName);
					PLAYER_CONTROL_FINISH_DropEvent(it, index, suffix, fileName);
					TRIGGER_FINISH_DropEvent(it, index, suffix, fileName);
					PICK_CAMERA_DropEvent(it, index, suffix, fileName);
					CHANGE_MESH_DropEvent(it, index, suffix, fileName);
					CHANGE_FLOAT_LERP_DropEvent(it, index, suffix, fileName);
					CHANGE_PLAYING_STATE_DropEvent(it, index, suffix, fileName);
					CHANGE_PLAYING_MODE_DropEvent(it, index, suffix, fileName);
					CHANGE_VALUE_DropEvent(it, index, suffix, fileName);
					COLLISION_BETWEEN_OBJECTS_DropEvent(it, index, suffix, fileName);
					CHANGE_HITBOX_DropEvent(it, index, suffix, fileName);
					FLOAT_LERP_START_DropEvent(it, index, suffix, fileName);
				}
			}
		}
	}	
	_event->acceptProposedAction();
}




void EventTableWidget::SetCellDelegate(int _row, int _editor)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		QModelIndex index = indexFromItem(item(_row, VALUE));

		delegate->SetCellDelegate(index, _editor);
	}

}

 
void EventTableWidget::CreateConstantsForComboBox(void)
{
	createVariableArgsComboList.push_back("INTEGER"); 
	createVariableArgsComboList.push_back("FLOAT");
	createVariableArgsComboList.push_back("BOOLEAN");
	createVariableArgsComboList.push_back("STRING");
	createVariableArgsComboList.push_back("VECTOR_2");
	createVariableArgsComboList.push_back("VECTOR_3");
	createVariableArgsComboList.push_back("QUATERNION");
	argMode_KEY_VARIABLE_TEMPLATE_ComboList.push_back("KEY");
	argMode_KEY_VARIABLE_TEMPLATE_ComboList.push_back("VARIABLE");
	argMode_KEY_VARIABLE_TEMPLATE_ComboList.push_back("TEMPLATE");

	argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList.push_back("KEY");
	argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList.push_back("VARIABLE");
	argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList.push_back("EXPRESSION");
	argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList.push_back("TEMPLATE");

	argMode_KEY_VARIABLE_EXPRESSION_EQUALITY_TEMPLATE_ComboList.push_back("KEY");
	argMode_KEY_VARIABLE_EXPRESSION_EQUALITY_TEMPLATE_ComboList.push_back("VARIABLE");
	argMode_KEY_VARIABLE_EXPRESSION_EQUALITY_TEMPLATE_ComboList.push_back("EXPRESSION");
	argMode_KEY_VARIABLE_EXPRESSION_EQUALITY_TEMPLATE_ComboList.push_back("EQUALITY");
	argMode_KEY_VARIABLE_EXPRESSION_EQUALITY_TEMPLATE_ComboList.push_back("TEMPLATE");

	argMode_VARIABLE_TEMPLATE_ComboList.push_back("VARIABLE");
	argMode_VARIABLE_TEMPLATE_ComboList.push_back("TEMPLATE");
	deactivationMode_EVER_AUTO_OVERLAY_ComboList.push_back("NEVER_SWITCH_OFF");	
	deactivationMode_EVER_AUTO_OVERLAY_ComboList.push_back("AUTO_SWITCH_OFF");	
	deactivationMode_EVER_AUTO_OVERLAY_ComboList.push_back("OVERLAY_SWITCH_OFF");	

	deactivationMode_EVER_AUTO_ComboList.push_back("NEVER_SWITCH_OFF");	
	deactivationMode_EVER_AUTO_ComboList.push_back("AUTO_SWITCH_OFF");	
}



void EventTableWidget::CreateDictionary(void)
{
	StringANSI path = QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/dictionary.txt").toAscii().data();

	File::_SetStringKey(path, true, "FIRST_NAME", "EVENT", "event");
	File::_SetStringKey(path, true, "LAST_NAME", "SEND_MESSAGE", "send_message");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_NAME", "change_name");
	File::_SetStringKey(path, true, "LAST_NAME", "DESTROY_OBJECT", "destroy_object");
	File::_SetStringKey(path, true, "LAST_NAME", "CREATE_OBJECT", "create_object");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_PROPERTY", "enable_property");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_PLAYING_STATE", "change_playing_state");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_PLAYING_MODE", "change_playing_mode");
	File::_SetStringKey(path, true, "LAST_NAME", "TIMER_FINISH", "timer_finish");
	File::_SetStringKey(path, true, "LAST_NAME", "PLAYER_CONTROL_FINISH", "player_control_finish");
	File::_SetStringKey(path, true, "LAST_NAME", "ADD_TO_UPDATE_LIST", "add_to_update_list");
	File::_SetStringKey(path, true, "LAST_NAME", "REMOVE_FROM_UPDATE_LIST", "remove_from_update_list");
	File::_SetStringKey(path, true, "LAST_NAME", "ADD_TO_RENDER_LIST", "add_to_render_list");
	File::_SetStringKey(path, true, "LAST_NAME", "REMOVE_FROM_RENDER_LIST", "remove_from_render_list");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_CAMERA", "change_camera");
	File::_SetStringKey(path, true, "LAST_NAME", "ADD_TO_LAYER", "add_to_layer");
	File::_SetStringKey(path, true, "LAST_NAME", "REMOVE_FROM_LAYER", "remove_from_layer");
	File::_SetStringKey(path, true, "LAST_NAME", "PICK_SCENE", "pick_scene");
	File::_SetStringKey(path, true, "LAST_NAME", "TRIGGER_FINISH", "trigger_finish");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ANIMATION_FRAME", "change_animation_frame");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_ATLAS", "change_atlas");
	File::_SetStringKey(path, true, "LAST_NAME", "ENABLE_POSITION_OFFSET", "enable_position_offset");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_FLOAT_LERP", "change_float_lerp");
	File::_SetStringKey(path, true, "LAST_NAME", "FLOAT_LERP_FINISH", "float_lerp_finish");
	File::_SetStringKey(path, true, "LAST_NAME", "FLOAT_LERP_START", "float_lerp_start");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_VALUE", "change_value");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_MESH", "change_mesh");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_HITBOX", "change_hitbox");
	File::_SetStringKey(path, true, "LAST_NAME", "COLLISION_BETWEEN_OBJECTS", "collision_between_objects");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_FRONT_TEXTURED_ANIMATION", "change_front_textured_animation");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_BACK_TEXTURED_ANIMATION", "change_back_textured_animation");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_FRONT_TEXTURE", "change_front_texture");
	File::_SetStringKey(path, true, "LAST_NAME", "CHANGE_BACK_TEXTURE", "change_back_texture");
	File::_SetStringKey(path, true, "LAST_NAME", "PICK_CAMERA", "pick_camera");
}



QString EventTableWidget::GetFirstName(void)
{
	StringANSI result;

	StringANSI path = QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/dictionary.txt").toAscii().data();
	if(File::_GetStringKey(path, true, "FIRST_NAME", "EVENT", result))
	{ 
		return result.c_str();
	}
	return "";
}


QString EventTableWidget::GetLastName(QString _type)
{
	StringANSI result;

	StringANSI path = QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/dictionary.txt").toAscii().data();
	if(File::_GetStringKey(path, true, "LAST_NAME", _type.toAscii().data(), result))
	{ 
		return result.c_str();
	}
	return "";
}




bool EventTableWidget::StringOperationIsLast(QString _expr)
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





void EventTableWidget::HitboxDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::HITBOX_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}


void EventTableWidget::MeshDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::MESH_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void EventTableWidget::CameraDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::CAMERA_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void EventTableWidget::FloatLerpDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::FLOAT_LERP_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}




void EventTableWidget::AnyAssetDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString()))
		{
			_it->setText(_fileName);
		}
	}
}



void EventTableWidget::TexturedAnimationDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TEXTURED_ANIMATION_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}




void EventTableWidget::TextureDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TEXTURE_2D_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}


void EventTableWidget::AbstractAnimationDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TEXTURED_ANIMATION_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void EventTableWidget::AtlasDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::ATLAS_2D_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void EventTableWidget::PlayingObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
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



void EventTableWidget::LerpingObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
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




void EventTableWidget::GraphicsObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
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


void EventTableWidget::LogicObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
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


void EventTableWidget::SceneDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::SCENE_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}


void EventTableWidget::ObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::OBJECT_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void EventTableWidget::PlayerControlDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::PLAYER_CONTROL_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}


void EventTableWidget::TimerDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TIMER_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}


void EventTableWidget::TriggerDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
	{
		if(AssetLibrary::_IsAssetExist(_fileName.toStdString(), AssetLibrary::TRIGGER_ASSET))
		{
			_it->setText(_fileName);
		}
	}
}



void EventTableWidget::ListDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName, int32 _listType)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
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



void EventTableWidget::VariableDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName, int32 _variableType)
{ 
	if(_index.row() == ARG1_PROPERTY ||
	   _index.row() == ARG2_PROPERTY ||
	   _index.row() == ARG3_PROPERTY ||
	   _index.row() == ARG4_PROPERTY ||
	   _index.row() == ARG5_PROPERTY ||
	   _index.row() == SOURCE_PROPERTY)
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


void EventTableWidget::StringExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	bool isVariable = false;

	if(_suffix == AbstractVariable::_GetFileSuffix().c_str())
	{
		AbstractVariable* variable = AbstractVariable::_LoadFromFile(_fileName.toStdString());

		if(variable)
		{
			if(variable->GetValueType() == Variable<int32>::INTEGER_TYPE ||
			   variable->GetValueType() == Variable<StringANSI>::STRING_TYPE)
			{
				isVariable = true;
			}
			delete variable;
		}
	}

	if(IsEmpty(_it->text()))
	{
		if(!isVariable)
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

			if(!isVariable)
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

			if(!isVariable)
			{
				_fileName = StringExprLineEdit::_GetCONST() + _fileName;
			}
			_it->setText(_it->text() + operation + _fileName);
		}
	} 
}



void EventTableWidget::SetAutoName(bool _enable)
{
	autoName = _enable;
}


bool EventTableWidget::IsAutoNameEnabled(void)const
{
	return autoName;
}


bool EventTableWidget::IsAutoNameEnabled(int _row, int _column)
{
	if(autoName)
	{ 
		if(_row == TYPE_PROPERTY && _column == VALUE ||
		   _row == SOURCE_PROPERTY && _column == VALUE ||
		   _row == ARG1_PROPERTY && _column == VALUE ||
		   _row == ARG2_PROPERTY && _column == VALUE ||
		   _row == ARG3_PROPERTY && _column == VALUE ||
		   _row == ARG4_PROPERTY && _column == VALUE ||
		   _row == ARG5_PROPERTY && _column == VALUE)
		{		
			return true;
		}
	}
	return false;
}



bool EventTableWidget::BooleanOperationIsLast(QString _expr)
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


void EventTableWidget::BooleanExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
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



void EventTableWidget::ArithmeticExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName, QString _variableType)
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



bool EventTableWidget::ArithmeticOperationIsLast(QString _expr)
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




bool EventTableWidget::IsEqualityExprCompute(QString _text)
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



void EventTableWidget::UpdateEqualityExpr(int _argIndex)
{  
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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


void EventTableWidget::UpdateArithmeticExpr(int _argIndex, int32 _variableType)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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


bool EventTableWidget::IsArithmeticExprCompute(QString _text, int32 _variableType)
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



bool EventTableWidget::IsBooleanExprCompute(QString _text)
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



void EventTableWidget::UpdateStringExpr(int _argIndex)
{ 
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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


void EventTableWidget::UpdateBooleanExpr(int _argIndex)
{ 
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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


int32 EventTableWidget::GetVariableType(QString _variableType)
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




void EventTableWidget::SetCellText(int _row, int _column, QString _value)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		if(_column >= 0 && _column < MAX_COLUMNS)
		{
			item(_row, _column)->setText(_value);
		}
	}
}



QString EventTableWidget::GetCellText(int _row, int _column)
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




void EventTableWidget::SetDefaultState(void)
{
	propertyValid[ARG1_PROPERTY] = false;
	propertyValid[ARG2_PROPERTY] = false;
	propertyValid[ARG3_PROPERTY] = false;
	propertyValid[ARG4_PROPERTY] = false;
	propertyValid[ARG5_PROPERTY] = false;
	propertyValid[SOURCE_PROPERTY] = false;
	propertyEnabled[ARG1_PROPERTY] = true;
	propertyEnabled[ARG2_PROPERTY] = true;
	propertyEnabled[ARG3_PROPERTY] = true;
	propertyEnabled[ARG4_PROPERTY] = true;
	propertyEnabled[ARG5_PROPERTY] = true;
	propertyEnabled[SOURCE_PROPERTY] = true;
	item(ARG1_PROPERTY, VALUE)->setToolTip("");
	item(ARG2_PROPERTY, VALUE)->setToolTip("");
	item(ARG3_PROPERTY, VALUE)->setToolTip("");
	item(ARG4_PROPERTY, VALUE)->setToolTip("");
	item(ARG5_PROPERTY, VALUE)->setToolTip("");
	item(SOURCE_PROPERTY, VALUE)->setToolTip("");
	
	item(ARG1_PROPERTY, CONTENT)->setToolTip("");
	item(ARG2_PROPERTY, CONTENT)->setToolTip("");
	item(ARG3_PROPERTY, CONTENT)->setToolTip("");
	item(ARG4_PROPERTY, CONTENT)->setToolTip("");
	item(ARG5_PROPERTY, CONTENT)->setToolTip("");
	item(SOURCE_PROPERTY, CONTENT)->setToolTip("");
	for(int i = 0; i < MAX_COLUMNS; i++)
	{
		SetCellText(ARG1_PROPERTY, i, "");
		SetCellText(ARG2_PROPERTY, i, "");
		SetCellText(ARG3_PROPERTY, i, "");
		SetCellText(ARG4_PROPERTY, i, "");
		SetCellText(ARG5_PROPERTY, i, "");
		SetCellText(SOURCE_PROPERTY, i, ""); 
	}
}


 
void EventTableWidget::SetDefaultProperty(int _index)
{
	if(_index == ARG1_PROPERTY ||
	   _index == ARG2_PROPERTY ||
	   _index == ARG3_PROPERTY ||
	   _index == ARG4_PROPERTY ||
	   _index == ARG5_PROPERTY ||
	   _index == SOURCE_PROPERTY)
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
	else if(_index == ACTIVATION_STATE_PROPERTY)
	{
		SetCellText(ACTIVATION_STATE_PROPERTY, VALUE, "false"); 
	}
	else if(_index == DEACTIVATION_MODE_PROPERTY)
	{
		SetCellText(DEACTIVATION_MODE_PROPERTY, VALUE, ""); 
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


void EventTableWidget::SetPropertyEnable(int _row, bool _enable)
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


bool EventTableWidget::IsPropertyValid(int _row)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		return propertyValid[_row];
	}
	return false;
}


void EventTableWidget::SetPropertyValid(int _row, bool _valid)
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


bool EventTableWidget::IsValid(void)const
{
	if(propertyEnabled[TYPE_PROPERTY] && !propertyValid[TYPE_PROPERTY]) { return false; }
	if(propertyEnabled[NAME_PROPERTY] && !propertyValid[NAME_PROPERTY]) { return false; }
	if(propertyEnabled[ARG1_PROPERTY] && !propertyValid[ARG1_PROPERTY]) { return false; }
	if(propertyEnabled[ARG2_PROPERTY] && !propertyValid[ARG2_PROPERTY]) { return false; }
	if(propertyEnabled[ARG3_PROPERTY] && !propertyValid[ARG3_PROPERTY]) { return false; }
	if(propertyEnabled[ARG4_PROPERTY] && !propertyValid[ARG4_PROPERTY]) { return false; }
	if(propertyEnabled[ARG5_PROPERTY] && !propertyValid[ARG5_PROPERTY]) { return false; }
	if(propertyEnabled[SOURCE_PROPERTY] && !propertyValid[SOURCE_PROPERTY]) { return false; }
	
	return true;
}




void EventTableWidget::UpdateGraphicsObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdateList(int _argIndex, int32 _listType)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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


void EventTableWidget::UpdateVariable(int _argIndex, int32 _variableType)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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

void EventTableWidget::UpdateName(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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





void EventTableWidget::UpdateLogicObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdateScene(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdateCamera(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdateTexturedAnimation(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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


void EventTableWidget::UpdateTexture(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdateAbstractAnimation(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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





void EventTableWidget::UpdateAtlas(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdateHitbox(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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


void EventTableWidget::UpdateMesh(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdateFloatLerp(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdateLerpingObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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





void EventTableWidget::UpdatePlayingObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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


void EventTableWidget::UpdateObject(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdatePlayerControl(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdateTimer(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::TIMER_ASSET))
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



void EventTableWidget::UpdateTrigger(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
	{
		QString fileName = item(_argIndex, VALUE)->text();
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(fileName.toStdString(), AssetLibrary::TRIGGER_ASSET))
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


void EventTableWidget::UpdateAnyAsset(int _argIndex)
{
	if(_argIndex == ARG1_PROPERTY ||
	   _argIndex == ARG2_PROPERTY ||
	   _argIndex == ARG3_PROPERTY ||
	   _argIndex == ARG4_PROPERTY ||
	   _argIndex == ARG5_PROPERTY ||
	   _argIndex == SOURCE_PROPERTY)
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



void EventTableWidget::UpdateNamePropertyStatus(void)
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


void EventTableWidget::UpdateTypePropertyStatus(void)
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


void EventTableWidget::UpdateArg1KeyPropertyStatus(void)
{
	if(CHANGE_NAME_UpdateArg1KeyPropertyStatus())
	{}
	else
	{
		SetPropertyValid(ARG1_PROPERTY, true);
	}
}


void EventTableWidget::UpdateArg1VariablePropertyStatus(void)
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
		   SEND_MESSAGE_UpdateArg1VariablePropertyStatus() ||
		   ADD_TO_LAYER_UpdateArg1VariablePropertyStatus() ||
		   REMOVE_FROM_LAYER_UpdateArg1VariablePropertyStatus() ||
		   ADD_TO_UPDATE_LIST_UpdateArg1VariablePropertyStatus() ||
		   REMOVE_FROM_UPDATE_LIST_UpdateArg1VariablePropertyStatus() ||
		   ADD_TO_RENDER_LIST_UpdateArg1VariablePropertyStatus() ||
		   REMOVE_FROM_RENDER_LIST_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_CAMERA_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_POSITION_OFFSET_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_FRONT_TEXTURE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_BACK_TEXTURE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_FRONT_TEXTURED_ANIMATION_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_BACK_TEXTURED_ANIMATION_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ANIMATION_FRAME_UpdateArg1VariablePropertyStatus() ||
		   ENABLE_PROPERTY_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_ATLAS_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_MESH_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_PLAYING_STATE_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_PLAYING_MODE_UpdateArg1VariablePropertyStatus() ||
		   COLLISION_BETWEEN_OBJECTS_UpdateArg1VariablePropertyStatus() ||
		   CHANGE_HITBOX_UpdateArg1VariablePropertyStatus())
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


void EventTableWidget::UpdateArg1TemplatePropertyStatus(void)
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



bool EventTableWidget::IsStringExprCompute(QString _text)
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



void EventTableWidget::UpdateArg1PropertyStatus(void)
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
			else
			{ 
				CHANGE_VALUE_UpdateArg1PropertyStatus();
			}
		} 
		else 
		{ 
			if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE")
			{
				SetPropertyValid(ARG1_PROPERTY, false);
			}
			else
			{ 
				SetPropertyValid(ARG1_PROPERTY, true);
			}
		}
	}
	else
	{
		item(ARG1_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
}


void EventTableWidget::UpdateArg2KeyPropertyStatus(void)
{
	if(COLLISION_BETWEEN_OBJECTS_UpdateArg2KeyPropertyStatus())
	{}
	else
	{
		SetPropertyValid(ARG2_PROPERTY, true);
	}
}


void EventTableWidget::UpdateArg2VariablePropertyStatus(void)
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
		if(CHANGE_VALUE_UpdateArg2VariablePropertyStatus() ||
		   COLLISION_BETWEEN_OBJECTS_UpdateArg2VariablePropertyStatus() ||
		   ENABLE_PROPERTY_UpdateArg2VariablePropertyStatus())
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


void EventTableWidget::UpdateArg2ExpressionPropertyStatus(void)
{
	if(CHANGE_VALUE_UpdateArg2ExpressionPropertyStatus())
	{}
}

void EventTableWidget::UpdateArg2EqualityPropertyStatus(void)
{
	if(CHANGE_VALUE_UpdateArg2EqualityPropertyStatus())
	{}
}


void EventTableWidget::UpdateArg2TemplatePropertyStatus(void)
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



void EventTableWidget::UpdateArg2PropertyStatus(void)
{
	if(IsEnabled(ARG2_PROPERTY))
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
			else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
			{ 
				UpdateArg2EqualityPropertyStatus();
			}
			else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
			{
				UpdateArg2TemplatePropertyStatus();
			}
		}
		else 
		{ 
			SetPropertyValid(ARG2_PROPERTY, true);
		}
	}
	else
	{
		item(ARG2_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
}



void EventTableWidget::UpdateArg3KeyPropertyStatus(void)
{
	if(COLLISION_BETWEEN_OBJECTS_UpdateArg3KeyPropertyStatus())
	{}
	else
	{
		SetPropertyValid(ARG3_PROPERTY, true);
	}
}


void EventTableWidget::UpdateArg3VariablePropertyStatus(void)
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
		if(COLLISION_BETWEEN_OBJECTS_UpdateArg3VariablePropertyStatus())
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


void EventTableWidget::UpdateArg3TemplatePropertyStatus(void)
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



void EventTableWidget::UpdateArg3PropertyStatus(void)
{
	if(IsEnabled(ARG3_PROPERTY))
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
			else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
			{
				UpdateArg3TemplatePropertyStatus();
			}
		}
		else 
		{ 
			SetPropertyValid(ARG3_PROPERTY, true);
		}
	}
	else
	{
		item(ARG3_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
} 




void EventTableWidget::UpdateArg4KeyPropertyStatus(void)
{
	SetPropertyValid(ARG4_PROPERTY, true);
}


void EventTableWidget::UpdateArg4VariablePropertyStatus(void)
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
		if(COLLISION_BETWEEN_OBJECTS_UpdateArg4VariablePropertyStatus())
		{}
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


void EventTableWidget::UpdateArg4TemplatePropertyStatus(void)
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



void EventTableWidget::UpdateArg4PropertyStatus(void)
{
	if(IsEnabled(ARG4_PROPERTY))
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
			SetPropertyValid(ARG4_PROPERTY, true);
		}
	}
	else
	{
		item(ARG4_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
} 



void EventTableWidget::UpdateArg5KeyPropertyStatus(void)
{
	SetPropertyValid(ARG5_PROPERTY, true);
}


void EventTableWidget::UpdateArg5VariablePropertyStatus(void)
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
		if(COLLISION_BETWEEN_OBJECTS_UpdateArg5VariablePropertyStatus())
		{}
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


void EventTableWidget::UpdateArg5TemplatePropertyStatus(void)
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



void EventTableWidget::UpdateArg5PropertyStatus(void)
{
	if(IsEnabled(ARG5_PROPERTY))
	{
		if(item(ARG5_PROPERTY, VALUE)->text().length() > 0)
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
			SetPropertyValid(ARG5_PROPERTY, true);
		}
	}
	else
	{
		item(ARG5_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
} 


void EventTableWidget::UpdateSourceVariablePropertyStatus(void)
{
	int32 errorCode;

	if((errorCode = BooleanExprParser::_IsNameValid(item(SOURCE_PROPERTY, VALUE)->text().toStdString())) >= 0)
	{ 
		SetPropertyValid(SOURCE_PROPERTY, false);
			
		QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(item(SOURCE_PROPERTY, VALUE)->text().at(errorCode)) + "'";
		item(SOURCE_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{
		if(CHANGE_NAME_UpdateSourceVariablePropertyStatus() ||
		   SEND_MESSAGE_UpdateSourceVariablePropertyStatus() ||
		   CREATE_OBJECT_UpdateSourceVariablePropertyStatus() ||
		   DESTROY_OBJECT_UpdateSourceVariablePropertyStatus() ||
		   TIMER_FINISH_UpdateSourceVariablePropertyStatus() ||
		   FLOAT_LERP_FINISH_UpdateSourceVariablePropertyStatus() ||
		   PICK_SCENE_UpdateSourceVariablePropertyStatus() ||
		   ADD_TO_LAYER_UpdateSourceVariablePropertyStatus() ||
		   REMOVE_FROM_LAYER_UpdateSourceVariablePropertyStatus() ||
		   ADD_TO_UPDATE_LIST_UpdateSourceVariablePropertyStatus() ||
		   REMOVE_FROM_UPDATE_LIST_UpdateSourceVariablePropertyStatus() ||
		   ADD_TO_RENDER_LIST_UpdateSourceVariablePropertyStatus() ||
		   REMOVE_FROM_RENDER_LIST_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_CAMERA_UpdateSourceVariablePropertyStatus() ||
		   ENABLE_POSITION_OFFSET_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_FRONT_TEXTURE_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_BACK_TEXTURE_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_FRONT_TEXTURED_ANIMATION_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_BACK_TEXTURED_ANIMATION_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_ANIMATION_FRAME_UpdateSourceVariablePropertyStatus() ||
		   ENABLE_PROPERTY_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_ATLAS_UpdateSourceVariablePropertyStatus() ||
		   PLAYER_CONTROL_FINISH_UpdateSourceVariablePropertyStatus() ||
		   TRIGGER_FINISH_UpdateSourceVariablePropertyStatus() ||
		   PICK_CAMERA_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_MESH_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_FLOAT_LERP_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_PLAYING_STATE_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_PLAYING_MODE_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_VALUE_UpdateSourceVariablePropertyStatus() ||
		   COLLISION_BETWEEN_OBJECTS_UpdateSourceVariablePropertyStatus() ||
		   CHANGE_HITBOX_UpdateSourceVariablePropertyStatus() ||
		   FLOAT_LERP_START_UpdateSourceVariablePropertyStatus())
		{}
		else
		{
			if(AssetLibrary::_IsAssetExist(item(SOURCE_PROPERTY, VALUE)->text().toStdString()))
			{
				SetPropertyValid(SOURCE_PROPERTY, true);
			}
			else
			{
				SetPropertyValid(SOURCE_PROPERTY, true);
				QString text = "Asset does not exist";
				item(SOURCE_PROPERTY, VALUE)->setToolTip(text);
				item(SOURCE_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
			}
		}
	}
}



void EventTableWidget::UpdateSourceTemplatePropertyStatus(void)
{
	StringExprParser parser;

	if(parser.Parse(item(SOURCE_PROPERTY, VALUE)->text().toStdString()) != StringExprParser::NO_ERRORS)
	{
		SetPropertyValid(SOURCE_PROPERTY, false);
		
		QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): '" + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
		item(SOURCE_PROPERTY, VALUE)->setToolTip(text);
	}
	else
	{
		SetPropertyValid(SOURCE_PROPERTY, true);
		if(!IsStringExprCompute(item(SOURCE_PROPERTY, VALUE)->text()))
		{
			QString text = "One or more args does not exist";
			item(SOURCE_PROPERTY, VALUE)->setToolTip(text);
			item(SOURCE_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));	
		}
	}
}



void EventTableWidget::UpdateSourcePropertyStatus(void)
{
	if(IsEnabled(SOURCE_PROPERTY))
	{
		if(item(SOURCE_PROPERTY, VALUE)->text().length() > 0)
		{
			if(item(SOURCE_PROPERTY, ARG_MODE)->text() == "VARIABLE")
			{
				UpdateSourceVariablePropertyStatus();
			}
			else if(item(SOURCE_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
			{
				UpdateSourceTemplatePropertyStatus();
			}
		}
		else 
		{ 
			SetPropertyValid(SOURCE_PROPERTY, false);
		}
	}
	else
	{
		item(SOURCE_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
	}
}




void EventTableWidget::UpdateStatus(void) 
{ 
	UpdateNamePropertyStatus();
	
	UpdateTypePropertyStatus();

	UpdateArg1PropertyStatus();

	UpdateArg2PropertyStatus();

	UpdateArg3PropertyStatus();

	UpdateArg4PropertyStatus();

	UpdateArg5PropertyStatus();

	UpdateSourcePropertyStatus();
}


bool EventTableWidget::IsEnabled(int _row)const
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		return propertyEnabled[_row];
	}
	return false;
}


void EventTableWidget::keyPressEvent(QKeyEvent* _event)
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



void EventTableWidget::mouseDoubleClickEvent(QMouseEvent* _event)
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

void EventTableWidget::UpdateItem(QString _eventType, QTableWidgetItem* _it, int _row)
{ 
	if(_row == ARG1_PROPERTY || 
	   _row == ARG2_PROPERTY || 
	   _row == ARG3_PROPERTY || 
	   _row == ARG4_PROPERTY || 
	   _row == ARG5_PROPERTY || 
	   _row == SOURCE_PROPERTY ||
	   _row == DEACTIVATION_MODE_PROPERTY)
	{
		QModelIndex index = indexFromItem(_it);
		delegate->SetCellDelegate(index, GetEditorTypeForCell(_eventType, static_cast<ItemEvent*>(_it)));
	} 
}



void EventTableWidget::CreateAutoName(const QModelIndex& _index)
{
	if(IsAutoNameEnabled(_index.row(), _index.column()))
	{
		QString newName = GetFirstName();
		
		newName += "_" + GetLastName(item(TYPE_PROPERTY, VALUE)->text());
	
		newName += CHANGE_NAME_CreateAutoName();
		newName += SEND_MESSAGE_CreateAutoName();
		newName += CREATE_OBJECT_CreateAutoName();
		newName += DESTROY_OBJECT_CreateAutoName();
		newName += TIMER_FINISH_CreateAutoName();
		newName += FLOAT_LERP_FINISH_CreateAutoName();
		newName += PICK_SCENE_CreateAutoName();
		newName += ADD_TO_LAYER_CreateAutoName();
		newName += REMOVE_FROM_LAYER_CreateAutoName();
		newName += ADD_TO_UPDATE_LIST_CreateAutoName();
		newName += REMOVE_FROM_UPDATE_LIST_CreateAutoName();
		newName += ADD_TO_RENDER_LIST_CreateAutoName();
		newName += REMOVE_FROM_RENDER_LIST_CreateAutoName();
		newName += CHANGE_CAMERA_CreateAutoName();
		newName += ENABLE_POSITION_OFFSET_CreateAutoName();
		newName += CHANGE_FRONT_TEXTURE_CreateAutoName();
		newName += CHANGE_BACK_TEXTURE_CreateAutoName();
		newName += CHANGE_FRONT_TEXTURED_ANIMATION_CreateAutoName();
		newName += CHANGE_BACK_TEXTURED_ANIMATION_CreateAutoName();
		newName += CHANGE_ANIMATION_FRAME_CreateAutoName();
		newName += ENABLE_PROPERTY_CreateAutoName();
		newName += CHANGE_ATLAS_CreateAutoName();
		newName += PLAYER_CONTROL_FINISH_CreateAutoName();
		newName += TRIGGER_FINISH_CreateAutoName();
		newName += PICK_CAMERA_CreateAutoName();
		newName += CHANGE_MESH_CreateAutoName();
		newName += CHANGE_FLOAT_LERP_CreateAutoName();
		newName += CHANGE_PLAYING_STATE_CreateAutoName();
		newName += CHANGE_PLAYING_MODE_CreateAutoName();
		newName += CHANGE_VALUE_CreateAutoName();
		newName += COLLISION_BETWEEN_OBJECTS_CreateAutoName();
		newName += CHANGE_HITBOX_CreateAutoName();
		newName += FLOAT_LERP_START_CreateAutoName();

		item(NAME_PROPERTY, VALUE)->setText(newName);
	}
}



void EventTableWidget::UpdateRow(QTableWidgetItem* _it)
{
	ItemEvent* it = static_cast<ItemEvent*>(_it);
	
	QModelIndex index = indexFromItem(_it); 

	
	CreateAutoName(index);
	QString type = item(TYPE_PROPERTY, VALUE)->text();
	

	CHANGE_NAME_UpdateRow(index, type);
	SEND_MESSAGE_UpdateRow(index, type);
	CREATE_OBJECT_UpdateRow(index, type);
	DESTROY_OBJECT_UpdateRow(index, type);
	TIMER_FINISH_UpdateRow(index, type);
	FLOAT_LERP_FINISH_UpdateRow(index, type);
	PICK_SCENE_UpdateRow(index, type);
	ADD_TO_LAYER_UpdateRow(index, type);
	REMOVE_FROM_LAYER_UpdateRow(index, type);
	ADD_TO_UPDATE_LIST_UpdateRow(index, type);
	REMOVE_FROM_UPDATE_LIST_UpdateRow(index, type);
	ADD_TO_RENDER_LIST_UpdateRow(index, type);
	REMOVE_FROM_RENDER_LIST_UpdateRow(index, type);
	CHANGE_CAMERA_UpdateRow(index, type);
	ENABLE_POSITION_OFFSET_UpdateRow(index, type);
	CHANGE_FRONT_TEXTURE_UpdateRow(index, type);
	CHANGE_BACK_TEXTURE_UpdateRow(index, type);
	CHANGE_FRONT_TEXTURED_ANIMATION_UpdateRow(index, type);
	CHANGE_BACK_TEXTURED_ANIMATION_UpdateRow(index, type);
	CHANGE_ANIMATION_FRAME_UpdateRow(index, type);
	ENABLE_PROPERTY_UpdateRow(index, type);
	CHANGE_ATLAS_UpdateRow(index, type);
	PLAYER_CONTROL_FINISH_UpdateRow(index, type);
	TRIGGER_FINISH_UpdateRow(index, type);
	PICK_CAMERA_UpdateRow(index, type);
	CHANGE_MESH_UpdateRow(index, type);
	CHANGE_FLOAT_LERP_UpdateRow(index, type);
	CHANGE_PLAYING_STATE_UpdateRow(index, type);
	CHANGE_PLAYING_MODE_UpdateRow(index, type);
	CHANGE_VALUE_UpdateRow(index, type);
	COLLISION_BETWEEN_OBJECTS_UpdateRow(index, type);
	CHANGE_HITBOX_UpdateRow(index, type);
	FLOAT_LERP_START_UpdateRow(index, type);


	blockSignals(true);
 
	if(index.row() == TYPE_PROPERTY && index.column() == VALUE)
	{
		UpdateStatus();
	}
	else if(index.row() == SOURCE_PROPERTY && (index.column() == VALUE || index.column() == ARG_MODE))
	{
		UpdateSourcePropertyStatus();
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





int EventTableWidget::GetEditorTypeForCell(QString _eventType, ItemEvent* _it)
{
	QModelIndex index = indexFromItem(_it);

	int editor = UniversalDelegate::NO_EDIT;

	CHANGE_NAME_GetEditorTypeForCell(index, _eventType, editor);
	SEND_MESSAGE_GetEditorTypeForCell(index, _eventType, editor);
	CREATE_OBJECT_GetEditorTypeForCell(index, _eventType, editor);
	DESTROY_OBJECT_GetEditorTypeForCell(index, _eventType, editor);
	TIMER_FINISH_GetEditorTypeForCell(index, _eventType, editor);
	FLOAT_LERP_FINISH_GetEditorTypeForCell(index, _eventType, editor);
	PICK_SCENE_GetEditorTypeForCell(index, _eventType, editor);
	ADD_TO_LAYER_GetEditorTypeForCell(index, _eventType, editor);
	REMOVE_FROM_LAYER_GetEditorTypeForCell(index, _eventType, editor);
	ADD_TO_UPDATE_LIST_GetEditorTypeForCell(index, _eventType, editor);
	REMOVE_FROM_UPDATE_LIST_GetEditorTypeForCell(index, _eventType, editor);
	ADD_TO_RENDER_LIST_GetEditorTypeForCell(index, _eventType, editor);
	REMOVE_FROM_RENDER_LIST_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_CAMERA_GetEditorTypeForCell(index, _eventType, editor);
	ENABLE_POSITION_OFFSET_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_FRONT_TEXTURE_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_BACK_TEXTURE_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_FRONT_TEXTURED_ANIMATION_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_BACK_TEXTURED_ANIMATION_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_ANIMATION_FRAME_GetEditorTypeForCell(index, _eventType, editor);
	ENABLE_PROPERTY_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_ATLAS_GetEditorTypeForCell(index, _eventType, editor);
	PLAYER_CONTROL_FINISH_GetEditorTypeForCell(index, _eventType, editor);
	TRIGGER_FINISH_GetEditorTypeForCell(index, _eventType, editor);
	PICK_CAMERA_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_MESH_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_FLOAT_LERP_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_PLAYING_STATE_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_PLAYING_MODE_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_VALUE_GetEditorTypeForCell(index, _eventType, editor); 
	COLLISION_BETWEEN_OBJECTS_GetEditorTypeForCell(index, _eventType, editor);
	CHANGE_HITBOX_GetEditorTypeForCell(index, _eventType, editor);
	FLOAT_LERP_START_GetEditorTypeForCell(index, _eventType, editor);

	return editor;
}
