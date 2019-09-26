#ifndef EVENTTREEWIDGET_H
#define EVENTTREEWIDGET_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Object/LogicObject/LogicObject.h>
#include <Engine/Core/Container/Stack/SimpleStack/SimpleStack.h>
#include <Engine/Core/Container/Tree/SimpleBinaryTree/SimpleBinaryTree.h>
class EventTreeWidget: public QTreeWidget
{
	Q_OBJECT
	public: enum 
	{
		ICON_PROPERTY = 0,
		TYPE_PROPERTY,
		NAME_PROPERTY,
		TIME_PROPERTY
	};
	private: enum
	{
		MAX_COLUMNS = 3 
	};
	private: QPixmap triggerIcon;
	private: QPixmap eventIcon;
	private: QPixmap actionIcon;
	private: QPixmap warningIcon;

	private: SimpleStack<QTreeWidgetItem*> eventStack;

	private: int logShowMode;
	private: bool triggersShowEnable;
	private: bool eventsShowEnable;
	private: bool actionsShowEnable;
	private: QList<QPair<int, QString>> eventIgnoreList;
	private: SimpleBinaryTree<QString> eventIgnoreTree;
	private: QPoint cursorPoint;
    private: QMenu *contextMenu;
	private: QAction *ignoreAct;
	private: QAction *hideAct;
	public: EventTreeWidget(QWidget* _parent = 0);
	public: void FindItem(QString _name, QList<QTreeWidgetItem*>& _list);
	private: void FindItem(QTreeWidgetItem* _it, QString _name, QList<QTreeWidgetItem*>& _list);
	public slots: void ClearLogList(void);
	private slots: void EventHideIsClicked(void);
	public slots: void EventLogSettings(void);
	public: int GetLogShowMode(void)const;
	private: void CreateContextMenu(void);
	public slots: void EventIgnoreIsClicked(void);
	private: void UpdateContextMenu(void);
	public: void AddEvent(int _category, int _type, QString _name, uint32 _time);
	public slots: void UpdateEventTree(void);
	public: QPixmap GetIcon(int _assetType);
	public: QString GetStringCategory(int _category);
	public: QString GetStringType(int _category, int _type);
	protected: virtual void mousePressEvent(QMouseEvent* _event);

};


#endif 

