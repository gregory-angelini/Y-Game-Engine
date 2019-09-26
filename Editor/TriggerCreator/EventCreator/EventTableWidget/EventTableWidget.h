#ifndef EVENTTABLEWIDGET_H
#define EVENTTABLEWIDGET_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Trigger/Trigger.h>
class EventTableWidget: public QTableWidget
{
	Q_OBJECT
	public: class ItemEvent: public QTableWidgetItem
	{
		public: ItemEvent(void)
		{}
	};
	private: class Token
	{
		public: QString value;
		public: int position;
		public: QString previousOperation;
		public: Token(QString _value, QString _previousOperation, int _position)
		{
			value = _value;
			previousOperation = _previousOperation;
			position = _position;
		}
		public: bool IsLastOperation(QString _operation)
		{
			if(previousOperation == _operation)
			{
				return true;
			}
			return false;
		}
	};
	public: enum
	{
		MAX_ROWS = 12,
		MAX_COLUMNS = 4
	};
	public: enum
	{
		TYPE_PROPERTY = 0,
		SOURCE_PROPERTY,
		ARG1_PROPERTY,
		ARG2_PROPERTY,
		ARG3_PROPERTY,
		ARG4_PROPERTY,
		ARG5_PROPERTY,
		NAME_PROPERTY,
		DEACTIVATION_MODE_PROPERTY,
		ACTIVATION_STATE_PROPERTY,
		ACTIVATION_LIMIT_PROPERTY,
		ARGS_LOADING_MODE_PROPERTY
	};
	public: enum
	{
		VALUE = 0,
		CONTENT,
		DATA_TYPE,
		ARG_MODE
	};
	private: bool autoName;
	private: UniversalDelegate* delegate;

	private: QList<QString> createVariableArgsComboList;
    private: QList<QString> argMode_KEY_VARIABLE_TEMPLATE_ComboList;
	private: QList<QString> argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList;
    private: QList<QString> argMode_KEY_VARIABLE_EXPRESSION_EQUALITY_TEMPLATE_ComboList;
    private: QList<QString> argMode_VARIABLE_TEMPLATE_ComboList;
	private: QList<QString> deactivationMode_EVER_AUTO_OVERLAY_ComboList;
	private: QList<QString> deactivationMode_EVER_AUTO_ComboList;

	private: bool propertyValid[MAX_ROWS];
	private: bool propertyEnabled[MAX_ROWS];
	public: EventTableWidget(QWidget* _parent = 0);
	public: virtual ~EventTableWidget(void);
	private: void CreateConstantsForComboBox(void);
	public: void SetCellDelegate(int _row, int _editor);
	private: void CreateDictionary(void);
	private: QString GetFirstName(void);
	private: QString GetLastName(QString _type);
	public: void SetAutoName(bool _enable);
	private: bool IsAutoNameEnabled(int _row, int _column);
	public: bool IsAutoNameEnabled(void)const;
	private: void CreateAutoName(const QModelIndex& _index);
	public: int32 GetVariableType(QString _variableType);
	public: void SetDefaultState(void);
	public: void SetDefaultProperty(int _index);
	public: void SetCellText(int _row, int _column, QString _value);
	public: QString GetCellText(int _row, int _column);
	public: void SetPropertyEnable(int _row, bool _enable);
	public: void SetPropertyValid(int _row, bool _valid);
	public: bool IsPropertyValid(int _row);
	public: bool IsValid(void)const;
	public: void UpdateStatus(void);
	private: bool IsStringExprCompute(QString _text);
	private: bool IsArithmeticExprCompute(QString _text, int32 _variableType);
	private: bool IsBooleanExprCompute(QString _text);
	private: bool IsEqualityExprCompute(QString _text);
	private: void UpdateNamePropertyStatus(void);
	private: void UpdateTypePropertyStatus(void);
	private: void UpdateArg1PropertyStatus(void);
	private: void UpdateArg1KeyPropertyStatus(void);
	private: void UpdateArg1VariablePropertyStatus(void);
	private: void UpdateArg1TemplatePropertyStatus(void);
	private: void UpdateArg2PropertyStatus(void);
	private: void UpdateArg2KeyPropertyStatus(void);
	private: void UpdateArg2VariablePropertyStatus(void);
	private: void UpdateArg2ExpressionPropertyStatus(void);
	private: void UpdateArg2EqualityPropertyStatus(void);
	private: void UpdateArg2TemplatePropertyStatus(void);
	private: void UpdateArg3PropertyStatus(void);
	private: void UpdateArg3KeyPropertyStatus(void);
	private: void UpdateArg3VariablePropertyStatus(void);
	private: void UpdateArg3TemplatePropertyStatus(void);
	private: void UpdateArg4PropertyStatus(void);
	private: void UpdateArg4KeyPropertyStatus(void);
	private: void UpdateArg4VariablePropertyStatus(void);
	private: void UpdateArg4TemplatePropertyStatus(void);
	private: void UpdateArg5PropertyStatus(void);
	private: void UpdateArg5KeyPropertyStatus(void);
	private: void UpdateArg5VariablePropertyStatus(void);
	private: void UpdateArg5TemplatePropertyStatus(void);
	private: void UpdateSourcePropertyStatus(void);
	private: void UpdateSourceVariablePropertyStatus(void);
	private: void UpdateSourceTemplatePropertyStatus(void);
	private: void UpdateVariable(int _argIndex, int32 _variableType);
	private: void UpdateList(int _argIndex, int32 _listType);
	private: void UpdateAnyAsset(int _argIndex);
	private: void UpdateName(int _argIndex);
	private: void UpdateObject(int _argIndex);
	private: void UpdateTimer(int _argIndex);
	private: void UpdatePlayingObject(int _argIndex);
	private: void UpdateLerpingObject(int _argIndex);
	private: void UpdateFloatLerp(int _argIndex);
	private: void UpdateMesh(int _argIndex);
	private: void UpdateHitbox(int _argIndex);
	private: void UpdatePlayerControl(int _argIndex);
	private: void UpdateTrigger(int _argIndex);
	private: void UpdateAtlas(int _argIndex);
	private: void UpdateTexturedAnimation(int _argIndex);
	private: void UpdateAbstractAnimation(int _argIndex);
	private: void UpdateScene(int _argIndex);
	private: void UpdateLogicObject(int _argIndex);
	private: void UpdateGraphicsObject(int _argIndex);
	private: void UpdateCamera(int _argIndex);
	private: void UpdateTexture(int _argIndex);
	private: void UpdateBooleanExpr(int _argIndex);
	private: void UpdateEqualityExpr(int _argIndex);
	private: void UpdateStringExpr(int _argIndex);
	private: void UpdateArithmeticExpr(int _argIndex, int32 _variableType);
	public: bool IsEnabled(int _row)const;
	private slots: void CellIsClicked(int _row, int _column);
	protected: virtual void keyPressEvent(QKeyEvent* _event);
	protected: virtual void mouseDoubleClickEvent(QMouseEvent* _event);
	private: void UpdateItem(QString _eventType, QTableWidgetItem* _it, int _row);
	private: int GetEditorTypeForCell(QString _eventType, ItemEvent* _it);
	private slots: void UpdateRow(QTableWidgetItem* _it);
	protected: virtual void dragEnterEvent(QDragEnterEvent *_event);
	protected: virtual void dropEvent(QDropEvent *_event);
	protected: virtual void dragMoveEvent(QDragMoveEvent* _event);
	private: void VariableDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName, int32 _variableType);
	private: void ListDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName, int32 _listType);
	private: void MeshDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void HitboxDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void LerpingObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void PlayingObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void ObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void PlayerControlDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void TimerDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void TriggerDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void FloatLerpDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void SceneDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void LogicObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void GraphicsObjectDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void AnyAssetDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void AbstractAnimationDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void CameraDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void TexturedAnimationDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void AtlasDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: void TextureDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	public: bool IsEmpty(QString _expr);
	private: void EqualityExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	public: bool EqualityOperationIsLast(QString _expr);
	private: void ArithmeticExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName, QString _variableType);
	public: bool ArithmeticOperationIsLast(QString _expr);
	private: void StringExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	public: bool StringOperationIsLast(QString _expr);
	private: void BooleanExprDropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	public: bool BooleanOperationIsLast(QString _expr);
	private: void CHANGE_NAME_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_NAME_UpdateArg1KeyPropertyStatus(void);
	private: bool CHANGE_NAME_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_NAME_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_NAME_CreateAutoName(void);
	private: bool CHANGE_NAME_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_NAME_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void SEND_MESSAGE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool SEND_MESSAGE_UpdateArg1VariablePropertyStatus(void);
	private: bool SEND_MESSAGE_UpdateSourceVariablePropertyStatus(void);
	private: QString SEND_MESSAGE_CreateAutoName(void);
	private: bool SEND_MESSAGE_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void SEND_MESSAGE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CREATE_OBJECT_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CREATE_OBJECT_UpdateSourceVariablePropertyStatus(void);
	private: QString CREATE_OBJECT_CreateAutoName(void);
	private: bool CREATE_OBJECT_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CREATE_OBJECT_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void DESTROY_OBJECT_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool DESTROY_OBJECT_UpdateSourceVariablePropertyStatus(void);
	private: QString DESTROY_OBJECT_CreateAutoName(void);
	private: bool DESTROY_OBJECT_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void DESTROY_OBJECT_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void TIMER_FINISH_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool TIMER_FINISH_UpdateSourceVariablePropertyStatus(void);
	private: QString TIMER_FINISH_CreateAutoName(void);
	private: bool TIMER_FINISH_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void TIMER_FINISH_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void FLOAT_LERP_FINISH_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool FLOAT_LERP_FINISH_UpdateSourceVariablePropertyStatus(void);
	private: QString FLOAT_LERP_FINISH_CreateAutoName(void);
	private: bool FLOAT_LERP_FINISH_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void FLOAT_LERP_FINISH_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void PICK_SCENE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool PICK_SCENE_UpdateSourceVariablePropertyStatus(void);
	private: QString PICK_SCENE_CreateAutoName(void);
	private: bool PICK_SCENE_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void PICK_SCENE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void ADD_TO_LAYER_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool ADD_TO_LAYER_UpdateArg1VariablePropertyStatus(void);
	private: bool ADD_TO_LAYER_UpdateSourceVariablePropertyStatus(void);
	private: QString ADD_TO_LAYER_CreateAutoName(void);
	private: bool ADD_TO_LAYER_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void ADD_TO_LAYER_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void REMOVE_FROM_LAYER_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool REMOVE_FROM_LAYER_UpdateArg1VariablePropertyStatus(void);
	private: bool REMOVE_FROM_LAYER_UpdateSourceVariablePropertyStatus(void);
	private: QString REMOVE_FROM_LAYER_CreateAutoName(void);
	private: bool REMOVE_FROM_LAYER_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void REMOVE_FROM_LAYER_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void ADD_TO_UPDATE_LIST_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool ADD_TO_UPDATE_LIST_UpdateArg1VariablePropertyStatus(void);
	private: bool ADD_TO_UPDATE_LIST_UpdateSourceVariablePropertyStatus(void);
	private: QString ADD_TO_UPDATE_LIST_CreateAutoName(void);
	private: bool ADD_TO_UPDATE_LIST_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void ADD_TO_UPDATE_LIST_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void REMOVE_FROM_UPDATE_LIST_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool REMOVE_FROM_UPDATE_LIST_UpdateArg1VariablePropertyStatus(void);
	private: bool REMOVE_FROM_UPDATE_LIST_UpdateSourceVariablePropertyStatus(void);
	private: QString REMOVE_FROM_UPDATE_LIST_CreateAutoName(void);
	private: bool REMOVE_FROM_UPDATE_LIST_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void REMOVE_FROM_UPDATE_LIST_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void REMOVE_FROM_RENDER_LIST_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool REMOVE_FROM_RENDER_LIST_UpdateArg1VariablePropertyStatus(void);
	private: bool REMOVE_FROM_RENDER_LIST_UpdateSourceVariablePropertyStatus(void);
	private: QString REMOVE_FROM_RENDER_LIST_CreateAutoName(void);
	private: bool REMOVE_FROM_RENDER_LIST_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void REMOVE_FROM_RENDER_LIST_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void ADD_TO_RENDER_LIST_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool ADD_TO_RENDER_LIST_UpdateArg1VariablePropertyStatus(void);
	private: bool ADD_TO_RENDER_LIST_UpdateSourceVariablePropertyStatus(void);
	private: QString ADD_TO_RENDER_LIST_CreateAutoName(void);
	private: bool ADD_TO_RENDER_LIST_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void ADD_TO_RENDER_LIST_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_CAMERA_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_CAMERA_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_CAMERA_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_CAMERA_CreateAutoName(void);
	private: bool CHANGE_CAMERA_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_CAMERA_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void ENABLE_POSITION_OFFSET_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool ENABLE_POSITION_OFFSET_UpdateArg1VariablePropertyStatus(void);
	private: bool ENABLE_POSITION_OFFSET_UpdateSourceVariablePropertyStatus(void);
	private: QString ENABLE_POSITION_OFFSET_CreateAutoName(void);
	private: bool ENABLE_POSITION_OFFSET_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void ENABLE_POSITION_OFFSET_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_FRONT_TEXTURE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_FRONT_TEXTURE_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_FRONT_TEXTURE_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_FRONT_TEXTURE_CreateAutoName(void);
	private: bool CHANGE_FRONT_TEXTURE_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_FRONT_TEXTURE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_BACK_TEXTURE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_BACK_TEXTURE_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_BACK_TEXTURE_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_BACK_TEXTURE_CreateAutoName(void);
	private: bool CHANGE_BACK_TEXTURE_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_BACK_TEXTURE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_FRONT_TEXTURED_ANIMATION_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_FRONT_TEXTURED_ANIMATION_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_FRONT_TEXTURED_ANIMATION_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_FRONT_TEXTURED_ANIMATION_CreateAutoName(void);
	private: bool CHANGE_FRONT_TEXTURED_ANIMATION_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_FRONT_TEXTURED_ANIMATION_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_BACK_TEXTURED_ANIMATION_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_BACK_TEXTURED_ANIMATION_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_BACK_TEXTURED_ANIMATION_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_BACK_TEXTURED_ANIMATION_CreateAutoName(void);
	private: bool CHANGE_BACK_TEXTURED_ANIMATION_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_BACK_TEXTURED_ANIMATION_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_ANIMATION_FRAME_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_ANIMATION_FRAME_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_ANIMATION_FRAME_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_ANIMATION_FRAME_CreateAutoName(void);
	private: bool CHANGE_ANIMATION_FRAME_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_ANIMATION_FRAME_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void ENABLE_PROPERTY_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool ENABLE_PROPERTY_UpdateArg1VariablePropertyStatus(void);
	private: bool ENABLE_PROPERTY_UpdateArg2VariablePropertyStatus(void);
	private: bool ENABLE_PROPERTY_UpdateSourceVariablePropertyStatus(void);
	private: QString ENABLE_PROPERTY_CreateAutoName(void);
	private: bool ENABLE_PROPERTY_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void ENABLE_PROPERTY_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_ATLAS_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_ATLAS_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_ATLAS_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_ATLAS_CreateAutoName(void);
	private: bool CHANGE_ATLAS_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_ATLAS_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void PLAYER_CONTROL_FINISH_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool PLAYER_CONTROL_FINISH_UpdateSourceVariablePropertyStatus(void);
	private: QString PLAYER_CONTROL_FINISH_CreateAutoName(void);
	private: bool PLAYER_CONTROL_FINISH_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void PLAYER_CONTROL_FINISH_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void TRIGGER_FINISH_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool TRIGGER_FINISH_UpdateSourceVariablePropertyStatus(void);
	private: QString TRIGGER_FINISH_CreateAutoName(void);
	private: bool TRIGGER_FINISH_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void TRIGGER_FINISH_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void PICK_CAMERA_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool PICK_CAMERA_UpdateSourceVariablePropertyStatus(void);
	private: QString PICK_CAMERA_CreateAutoName(void);
	private: bool PICK_CAMERA_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void PICK_CAMERA_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_MESH_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_MESH_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_MESH_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_MESH_CreateAutoName(void);
	private: bool CHANGE_MESH_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_MESH_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_FLOAT_LERP_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_FLOAT_LERP_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_FLOAT_LERP_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_FLOAT_LERP_CreateAutoName(void);
	private: bool CHANGE_FLOAT_LERP_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_FLOAT_LERP_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_PLAYING_STATE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_PLAYING_STATE_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_PLAYING_STATE_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_PLAYING_STATE_CreateAutoName(void);
	private: bool CHANGE_PLAYING_STATE_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_PLAYING_STATE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_PLAYING_MODE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_PLAYING_MODE_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_PLAYING_MODE_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_PLAYING_MODE_CreateAutoName(void);
	private: bool CHANGE_PLAYING_MODE_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_PLAYING_MODE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void CHANGE_VALUE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_VALUE_UpdateArg1PropertyStatus(void);
	private: bool CHANGE_VALUE_UpdateArg2PropertyStatus(void);
	private: bool CHANGE_VALUE_UpdateArg2VariablePropertyStatus(void);
	private: bool CHANGE_VALUE_UpdateArg2ExpressionPropertyStatus(void);
	private: bool CHANGE_VALUE_UpdateArg2EqualityPropertyStatus(void);
	private: bool CHANGE_VALUE_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_VALUE_CreateAutoName(void);
	private: bool CHANGE_VALUE_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_VALUE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor);
	private: void COLLISION_BETWEEN_OBJECTS_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool COLLISION_BETWEEN_OBJECTS_UpdateArg1VariablePropertyStatus(void);
	private: bool COLLISION_BETWEEN_OBJECTS_UpdateArg2VariablePropertyStatus(void);
	private: bool COLLISION_BETWEEN_OBJECTS_UpdateArg2KeyPropertyStatus(void);
	private: bool COLLISION_BETWEEN_OBJECTS_UpdateArg3KeyPropertyStatus(void);
	private: bool COLLISION_BETWEEN_OBJECTS_UpdateArg3VariablePropertyStatus(void);
	private: bool COLLISION_BETWEEN_OBJECTS_UpdateArg4VariablePropertyStatus(void);
	private: bool COLLISION_BETWEEN_OBJECTS_UpdateArg5VariablePropertyStatus(void);
	private: bool COLLISION_BETWEEN_OBJECTS_UpdateSourceVariablePropertyStatus(void);
	private: QString COLLISION_BETWEEN_OBJECTS_CreateAutoName(void);
	private: bool COLLISION_BETWEEN_OBJECTS_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void COLLISION_BETWEEN_OBJECTS_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor);
	private: void CHANGE_HITBOX_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool CHANGE_HITBOX_UpdateArg1VariablePropertyStatus(void);
	private: bool CHANGE_HITBOX_UpdateSourceVariablePropertyStatus(void);
	private: QString CHANGE_HITBOX_CreateAutoName(void);
	private: bool CHANGE_HITBOX_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void CHANGE_HITBOX_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);
	private: void FLOAT_LERP_START_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName);
	private: bool FLOAT_LERP_START_UpdateSourceVariablePropertyStatus(void);
	private: QString FLOAT_LERP_START_CreateAutoName(void);
	private: bool FLOAT_LERP_START_UpdateRow(const QModelIndex& _index, const QString& _type);
	private: void FLOAT_LERP_START_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor);


};


#endif 

