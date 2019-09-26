#ifndef ACTIONTYPEEDITOR_H
#define ACTIONTYPEEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
class ActionTypeEditor: public QComboBox
{	
	Q_OBJECT
	public: enum
	{
		BASIC = 0,

		SHAPE,
		TRANSFORMATION,

		MATERIAL,
		VIEW,

		INTERPOLATION,
		PLAYBACK,
		ANIMATION,

		RANDOMIZATION,
		COMBINATION,

		INPUT,
		TIME,
		TEXT,
		FILE,
		SOUND,

		CONTAINER,

		COLLISION,

		SCRIPT
	};
	public: enum
	{
		BASIC_SYSTEM,
		BASIC_ABSTRACT_OBJECT
	};
	public: enum
	{
		SHAPE_POLYGONAL_SURFACE,
		SHAPE_MESH,
		SHAPE_GRAPHICS_STRIP,
		SHAPE_PLANE,
		SHAPE_LINE,
		SHAPE_CIRCLE
	};
	public: enum
	{
		TRANSFORMATION_SCALABLE_OBJECT,
		TRANSFORMATION_TRANSLATABLE_OBJECT,
		TRANSFORMATION_ROTATABLE_OBJECT,
		TRANSFORMATION_GRAPHICS_SURFACE,
		TRANSFORMATION_VARIABLE
	};
	public: enum
	{ 
		MATERIAL_GRAPHICS_OBJECT,
		MATERIAL_TEXTURE,
		MATERIAL_COLORABLE_OBJECT,
		MATERIAL_GRAPHICS_SURFACE,
		MATERIAL_TEXTURING_SURFACE,
		MATERIAL_GRAPHICS_CONTAINER
	};
	public: enum
	{
		VIEW_SYSTEM,
		VIEW_CAMERA,
		VIEW_GRAPHICS_CONTAINER,
		VIEW_SCENE
	};
	public: enum
	{
		INTERPOLATION_LERPING_OBJECT,
		INTERPOLATION_FLOAT_LERP,
		INTERPOLATION_QUATERNION_LERP,
		INTERPOLATION_VECTOR_LERP
	};
	public: enum
	{
		PLAYBACK_PLAYING_OBJECT,
		PLAYBACK_ACTIVATABLE_OBJECT
	};
	public: enum
	{
		ANIMATION_ABSTRACT_ANIMATION,
		ANIMATION_TEXTURED_ANIMATION
	};
	public: enum
	{
		RANDOMIZATION_SYSTEM,
		RANDOMIZATION_VARIABLE,
		RANDOMIZATION_LIST
	};
	public: enum
	{
		INPUT_KEYBOARD_DEVICE,
		INPUT_PLAYER_CONTROL
	};
	public: enum
	{
		TIME_SYSTEM,
		TIME_TIMER,
		TIME_LOGIC_OBJECT
	};
	public: enum
	{
		FILE_SYSTEM,
		FILE_ABSTRACT_OBJECT
	};
	public: enum
	{
		CONTAINER_VARIABLE,
		CONTAINER_LIST,
		CONTAINER_LAYER,
		CONTAINER_LOGIC_SCENE,
		CONTAINER_GRAPHICS_SCENE,
		CONTAINER_TILED_SPACE,
		CONTAINER_SET
	};
	public: enum
	{
		COLLISION_ABSTRACT_SURFACE,
		COLLISION_POLYGONAL_SURFACE,
		COLLISION_TEXTURING_SURFACE,
		COLLISION_CAMERA
	};
	public: enum
	{
		SCRIPT_SYSTEM,
		SCRIPT_TRIGGER
	};
	public: enum
	{
		CATEGORY = 0,
		PROPERTY,
		ACTION_TYPE
	};
	public: enum
	{
		MAX_ROWS = 18,
		MAX_COLUMNS = 3,
		PROPERTIES_IN_CATEGORY = 18
	};
	public: QStandardItemModel* model;
	public: QTableView* view;
	private: int selectedCategory;
	private: int selectedProperty;
	private: int selectedActionType;
	private: SimpleList<StringANSI> actionTree[MAX_ROWS][PROPERTIES_IN_CATEGORY];
	private: SimpleList<StringANSI> propertyTree[MAX_ROWS];
	private: QStringList categoryLabels;
	public: ActionTypeEditor(QWidget* _widget);
	private slots: void ItemPressed(QModelIndex _index);
	private slots: void ItemEntered(QModelIndex _index);
	public: bool SelectCategory(QString _actionType);
	public: bool SelectProperty(QString _actionType);
	public: void CategorySelectionReset(void);
	public: void PropertySelectionReset(void);
	public: void ActionTypeSelectionReset(void);
	private: void UpdateCategorySelection(void);
	private: void UpdatePropertySelection(void);
	private: void UpdateActionTypeSelection(void);
	public: int GetSelectedCategory(void)const;
	public: int GetSelectedProperty(void)const;
	public: int GetSelectedActionType(void)const;
	public: bool IsAction(QString _type);
	private: void AddCategory(int _column, QString _name);
	private: void AddItem(int _row, int _column);
	private: void ChangeItem(int _row, int _column, QString _name);
};


#endif 

