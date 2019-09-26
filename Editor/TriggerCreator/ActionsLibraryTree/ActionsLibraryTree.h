#ifndef ACTIONSLIBRARYTREE_H
#define ACTIONSLIBRARYTREE_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
class ActionsLibraryTree: public QTreeWidget
{
	Q_OBJECT
	private: class Item: public QTreeWidgetItem
	{
		public: enum 
		{
			ACTION_TYPE,
			GROUP_TYPE,
			CATEGORY_TYPE
		};
		private: int itemType;
		private: int actionType;
		private: QString name;
		public: Item(QTreeWidgetItem* _parent, int _actionType, int _itemType): QTreeWidgetItem(_parent, _itemType)
		{
			SetItemType(_itemType);
			SetActionType(_actionType);
		}
		private: void SetItemType(int _type)
		{
			itemType = _type;
		}
		public: int GetItemType(void)const
		{
			return itemType;
		}
		public: int GetActionType(void)const
		{
			return actionType;
		}
		public: void SetActionType(int _type)
		{
			actionType = _type;
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
		MAX_COLUMNS = 2
	};
	private: QPixmap folderIcon;
	private: QPixmap actionIcon;
    private: QMenu* contextMenu;
	private: QAction* editActionAct;
	private: QAction* removeItemAct;
	private: QAction* refreshAct;
	private: QPoint cursorPoint;

	private: Item* lastAction;
	private: Item* lastCategory;
	private: Item* lastGroup;
	private: QColor selectedBeginColor, categoryBeginColor, groupBeginColor;
	public: ActionsLibraryTree(QWidget* _parent = 0);
	public: virtual ~ActionsLibraryTree(void);
	public: void Refresh(void);
    private slots: void RefreshActionTree(void);
	public: void Refresh(QString _name);
	private: void mousePressEvent(QMouseEvent* _event);
	protected: virtual void mouseDoubleClickEvent(QMouseEvent* _event);
	protected: virtual void keyPressEvent(QKeyEvent* _event);
	private: virtual void drawRow(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index)const;
	private: void CreateContextMenu(void);
	private: void UpdateContextMenu(void);
	private slots: void RemoveSelectedActions(void);
	private: QTreeWidgetItem* FindLeaf(QList<QTreeWidgetItem*>& _list);
	signals: void ActionIsEdited(QString _name);
	private slots: void EditAction(void);
	public: void AddAction(QString _name, int _actionType);
	public: Item* AddCategory(QString _name, int _actionType);
	public: Item* AddGroup(Item* _parent, QString _name, int _actionType);
	private: QString GetGroupName(int _actionType);
	private: QString GetCategoryName(int _actionType);
	public: Item* FindAction(QString _name, int _actionType);
	private: Item* FindAction(QTreeWidgetItem* _it, QString _name, int _actionType);
	public: Item* FindAction(QString _name);
	private: Item* FindAction(QTreeWidgetItem* _it, QString _name);
	private: Item* FindCategory(int _actionType);
	private: Item* FindCategory(QTreeWidgetItem* _it, int _actionType);
	private: Item* FindGroup(int _actionType);
	private: Item* FindGroup(QTreeWidgetItem* _it, int _actionType);
	public: int GetAmountOfItems(QTreeWidgetItem* _it);
	protected: virtual void mouseMoveEvent(QMouseEvent *_event);		 
};


#endif 

