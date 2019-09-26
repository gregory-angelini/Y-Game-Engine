#ifndef UNIVERSALDELEGATE_H
#define UNIVERSALDELEGATE_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/FloatSpinBox/FloatSpinBox.h>
#include <Editor/IntSpinBox/IntSpinBox.h>
class UniversalDelegate : public QStyledItemDelegate
{
	Q_OBJECT
	public: enum
	{
		NO_EDIT,
		BOOL_EDITOR,
		TEXT_EDITOR,
		COMBO_BOX_EDITOR,
		INT_EDITOR,
		INT_EX_EDITOR,
		FLOAT_EDITOR,
		FLOAT_0_1_EDITOR,
		FLOAT_0_360_EDITOR,
		FLOAT_EX_EDITOR,

		STRING_EXPR_EDITOR,
		   INTEGER_ARITHMETIC_EXPR_EDITOR,
		   FLOAT_ARITHMETIC_EXPR_EDITOR,
		   VECTOR_2_ARITHMETIC_EXPR_EDITOR,
		   VECTOR_3_ARITHMETIC_EXPR_EDITOR,
		   QUATERNION_ARITHMETIC_EXPR_EDITOR,
		BOOLEAN_EXPR_EDITOR,
		EQUALITY_EXPR_EDITOR,
		ENUMERATION_EXPR_EDITOR,

		ACTION_TYPE_EDITOR,
		EVENT_TYPE_EDITOR,
		LIMIT_EDITOR,
		LOADING_MODE_EDITOR,

		VIRUAL_KEY_EDITOR,
		NAME_EDITOR,

		VECTOR_2_EDITOR,
		VECTOR_3_EDITOR,
		VECTOR_3_RGB_EDITOR,
		VECTOR_3_HSL_EDITOR,
		QUATERNION_EDITOR,

		ANY_ASSET_EDITOR,
		
		SCALABLE_OBJECT_EDITOR,
		COLORABLE_OBJECT_EDITOR,
		TRANSLATABLE_OBJECT_EDITOR,
		ROTATABLE_OBJECT_EDITOR,
		PLAYING_OBJECT_EDITOR,
		LERPING_OBJECT_EDITOR,
		LOGIC_OBJECT_EDITOR,
		GRAPHICS_OBJECT_EDITOR,
		ACTIVATABLE_OBJECT_EDITOR,

		ABSTRACT_ANIMATION_EDITOR,

		OBJECT_EDITOR,
		SCENE_EDITOR,
		PLAYER_CONTROL_EDITOR,
		ABSTRACT_VARIABLE_EDITOR,
			BOOLEAN_VARIABLE_EDITOR,
			INTEGER_VARIABLE_EDITOR,
			STRING_VARIABLE_EDITOR,
			FLOAT_VARIABLE_EDITOR,
			VECTOR_2_VARIABLE_EDITOR,
			VECTOR_3_VARIABLE_EDITOR,
			QUATERNION_VARIABLE_EDITOR,
		ABSTRACT_LIST_EDITOR,
			INTEGER_LIST_EDITOR,
			FLOAT_LIST_EDITOR,
			BOOLEAN_LIST_EDITOR,
			STRING_LIST_EDITOR,
			VECTOR_2_LIST_EDITOR,
			VECTOR_3_LIST_EDITOR,
			QUATERNION_LIST_EDITOR,
			TRANSLATABLE_LIST_EDITOR,
			COLORABLE_LIST_EDITOR,
			ROTATABLE_LIST_EDITOR,
			SCALABLE_LIST_EDITOR,
			SET_LIST_EDITOR,
		MESH_EDITOR,
		HITBOX_EDITOR,
		TRIGGER_EDITOR,
		ACTION_EDITOR,
		EVENT_EDITOR,
		TEXTURED_ANIMATION_EDITOR,
		ATLAS_EDITOR,
		TEXTURE_EDITOR,
		CAMERA_EDITOR,
		FLOAT_LERP_EDITOR,
		QUATERNION_LERP_EDITOR,
		VECTOR_LERP_EDITOR,
		TIMER_EDITOR
	};
	private: QMap <QModelIndex, QList<QString>> valuesOfComboForCell;
    private: QMap <int, QList<QString>> valuesOfComboForColumn;
    private: QMap <int, int> columnDelegate;
    private: QMap <QModelIndex, int> cellDelegate;
    public: UniversalDelegate(QObject* _parent = 0);
    public: virtual ~UniversalDelegate(void);
	public slots: void CloseEditor(QWidget* _widget);
    public: void SetColumnDelegate(int _column, int _editorType);
    public: void SetCellDelegate(const QModelIndex _index, int _editorType);
    protected: virtual QWidget* createEditor(QWidget* _parent, const QStyleOptionViewItem& _option, const QModelIndex& _index)const;
	protected: virtual void setEditorData(QWidget* _editor, const QModelIndex& _index)const;
	protected: virtual void setModelData(QWidget* _editor, QAbstractItemModel* _model, const QModelIndex& _index)const;
	protected: virtual void updateEditorGeometry(QWidget* _editor, const QStyleOptionViewItem& _option, const QModelIndex& _index)const;
    public: void SetComboValuesForCell(QModelIndex _index, QList<QString> _values);
	public: void SetComboValuesForColumn(int _column, QList<QString> _values);
	public: QList<QString> GetComboValuesForCell(QModelIndex _index)const;
	public: int GetCurrentDelegate(QModelIndex _index)const;
	private: void Clear(void);
};





#endif 

