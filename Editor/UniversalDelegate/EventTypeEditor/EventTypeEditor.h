#ifndef EVENTTYPEEDITOR_H
#define EVENTTYPEEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
class EventTypeEditor: public QComboBox
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

		RANDOMIZATION,
		COMBINATION,

		INPUT,
		TIME,
		TEXT,
		FILE,

		CONTAINER,

		COLLISION,

		SCRIPT
	};
	public: enum
	{
		ABSTRACT_OBJECT
	};
	public: enum
	{
		SCRIPT_TRIGGER
	};
	public: enum
	{
		TIME_TIMER
	};
	public: enum
	{
		INTERPOLATION_FLOAT_LERP
	};
	public: enum
	{
		CONTAINER_VARIABLE,
		CONTAINER_LAYER,
		CONTAINER_LOGIC_SCENE,
		CONTAINER_GRAPHICS_SCENE,
		CONTAINER_SCENE
	};
	public: enum
	{
		VIEW_CAMERA,
		VIEW_GRAPHICS_CONTAINER
	};
	public: enum
	{
		SHAPE_POLYGONAL_SURFACE
	};
	public: enum
	{
		COLLISION_POLYGONAL_SURFACE
	};
	public: enum
	{
		TRANSFORMATION_ABSTRACT_ANIMATION,
		TRANSFORMATION_TEXTURED_ANIMATION,
		TRANSFORMATION_LERPING_OBJECT
	};
	public: enum
	{
		PLAYBACK_PLAYING_OBJECT
	};
	public: enum
	{
		INPUT_PLAYER_CONTROL
	};
	public: enum
	{
		MATERIAL_TEXTURED_ANIMATION,
		MATERIAL_TEXTURING_SURFACE
	};
	public: enum
	{
		CATEGORY = 0,
		PROPERTY,
		EVENT_TYPE
	};
	public: enum
	{
		MAX_ROWS = 16,
		MAX_COLUMNS = 3,
		PROPERTIES_IN_CATEGORY = 10
	};
	public: QStandardItemModel* model;
	public: QTableView* view;
	private: int selectedCategory;
	private: int selectedProperty;
	private: int selectedEventType;
	private: SimpleList<StringANSI> eventTree[MAX_ROWS][PROPERTIES_IN_CATEGORY];
	private: SimpleList<StringANSI> propertyTree[MAX_ROWS];
	private: QStringList categoryLabels;
	public: EventTypeEditor(QWidget* _widget);
	private slots: void ItemPressed(QModelIndex _index);
	private slots: void ItemEntered(QModelIndex _index);
	public: bool SelectCategory(QString _eventType);
	public: bool SelectProperty(QString _eventType);
	public: void CategorySelectionReset(void);
	public: void PropertySelectionReset(void);
	public: void EventTypeSelectionReset(void);
	private: void UpdateCategorySelection(void);
	private: void UpdatePropertySelection(void);
	private: void UpdateEventTypeSelection(void);
	public: int GetSelectedCategory(void)const;
	public: int GetSelectedProperty(void)const;
	public: int GetSelectedEventType(void)const;
	public: bool IsEvent(QString _type);
	private: void AddCategory(int _column, QString _name);
	private: void AddItem(int _row, int _column);
	private: void ChangeItem(int _row, int _column, QString _name);
};


#endif 

