#ifndef EVENTSLIBRARYTREE_H
#define EVENTSLIBRARYTREE_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
class EventsLibraryTree: public QTreeWidget
{
	Q_OBJECT
	private: class Item: public QTreeWidgetItem
	{
		public: enum 
		{
			EVENT_TYPE,
			TRIGGER_TYPE,
			VARIABLE_TYPE,
			GROUP_TYPE,
			CATEGORY_TYPE
		};
		private: int itemType;
		private: int eventType;
		private: QString name;
		public: Item(QTreeWidgetItem* _parent, int _eventType, int _itemType): QTreeWidgetItem(_parent, _itemType)
		{
			SetItemType(_itemType);
			SetEventType(_eventType);
		}
		private: void SetItemType(int _type)
		{
			itemType = _type;
		}
		public: int GetItemType(void)const
		{
			return itemType;
		}
		public: int GetEventType(void)const
		{
			return eventType;
		}
		public: void SetEventType(int _type)
		{
			eventType = _type;
		}
		public: QString GetName(void)const
		{
			return name;
		}
		public: void SetName(QString _name)
		{
			name = _name;
			
			setText(NAME_PROPERTY, name);
		}
		public: void RemoveChildItems(void)
		{
			while(child(0))
			{
				Item* it = dynamic_cast<Item*>(child(0));
				
				if(it) 
				{ 
					it->RemoveChildItems(); 
				} 
				delete child(0);
			}
		}
		public: bool IsEmpty(void)const
		{
			return child(0) == 0;
		}
		public: SimpleList<QString> GetChildNamesList(void)const 
		{
			SimpleList<QString> names;

			int index = 0;

			while(child(index))
			{
				Item* it = dynamic_cast<Item*>(child(index));
				if(it) { names.AddToTail(it->GetName()); }
				index++;
			}
			return names;
		}
	};
	public: enum
	{
		ICON_PROPERTY = 0,
		NAME_PROPERTY,
		TYPE_PROPERTY
	};
	private: enum
	{
		TRIGGER_CATEGORY = -1,
		VARIABLE_CATEGORY = -2
	};
	private: enum
	{
		MAX_COLUMNS = 2
	};
	private: QPixmap folderIcon;
	private: QPixmap eventIcon;
	private: QPixmap triggerIcon;
	private: QPixmap variableIcon;
    private: QMenu* contextMenu;
	private: QAction* editEventAct;
	private: QAction* removeItemAct;
	private: QAction* refreshAct;
	private: QPoint cursorPoint;

	private: Item* lastEvent;
	private: Item* lastCategory;
	private: Item* lastGroup;
	private: QColor selectedBeginColor, categoryBeginColor, groupBeginColor, variablesBeginColor, triggersBeginColor;
	public: EventsLibraryTree(QWidget* _parent = 0);
	public: virtual ~EventsLibraryTree(void);
	public: void Refresh(void);
    private slots: void RefreshEventTree(void);
	public: void Refresh(QString _name);
	private: void UpdateEventsTree(void);
	private: void UpdateTriggersTree(void);
	private: void UpdateVariablesTree(void);
	public: void RefreshEvent(QString _name);
	public: void RefreshTrigger(QString _name);
	public: void RefreshVariable(QString _name);
	private: void mousePressEvent(QMouseEvent* _event);
	protected: virtual void mouseDoubleClickEvent(QMouseEvent* _event);
	protected: virtual void keyPressEvent(QKeyEvent* _event);
	private: virtual void drawRow(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index)const;
	private: void CreateContextMenu(void);
	private: void UpdateContextMenu(void);
	private slots: void RemoveSelectedItems(void);
	private: QTreeWidgetItem* FindLeaf(QList<QTreeWidgetItem*>& _list);
	signals: void EventIsEdited(QString _name);
	signals: void TriggerIsEdited(QString _name);
	signals: void VariableIsEdited(QString _name);
	private slots: void EditItem(void);
	private slots: void EditEvent(void);
	private slots: void EditTrigger(void);
	private slots: void EditVariable(void);
	public: void AddTrigger(QString _name);
	public: void AddEvent(QString _name, int _eventType);
	public: void AddVariable(QString _name);
	public: Item* AddCategory(QString _name, int _eventType);
	public: Item* AddGroup(Item* _parent, QString _name, int _eventType);
	private: QString GetGroupName(int _eventType);
	private: QString GetCategoryName(int _eventType);
	public: Item* FindEvent(QString _name, int _eventType);
	private: Item* FindEvent(QTreeWidgetItem* _it, QString _name, int _eventType);
	public: Item* FindEvent(QString _name);
	private: Item* FindEvent(QTreeWidgetItem* _it, QString _name);
	public: Item* FindTrigger(QString _name);
	private: Item* FindTrigger(QTreeWidgetItem* _it, QString _name);
	public: Item* FindVariable(QString _name);
	private: Item* FindVariable(QTreeWidgetItem* _it, QString _name);
	private: Item* FindCategory(int _eventType);
	private: Item* FindCategory(QTreeWidgetItem* _it, int _eventType);
	private: Item* FindCategory(QString _name);
	private: Item* FindCategory(QTreeWidgetItem* _it, QString _name);
	private: Item* FindGroup(int _eventType);
	private: Item* FindGroup(QTreeWidgetItem* _it, int _eventType);
	public: int GetAmountOfItems(QTreeWidgetItem* _it);
	protected: virtual void mouseMoveEvent(QMouseEvent *_event);	
};


#endif 

