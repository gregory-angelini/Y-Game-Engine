#ifndef PLAYERCONTROLEVENTLISTWIDGET_H
#define PLAYERCONTROLEVENTLISTWIDGET_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
class PlayerControlEventListWidget: public QTreeWidget
{
	public: class ItemInputEvent: public QTreeWidgetItem
	{
		public: enum
		{
			ICON_PROPERTY = 0,
			DEVICE_PROPERTY = 1,
			TYPE_PROPERTY = 2,
			KEY_PROPERTY = 3
		};
		private: enum 
		{
			SIZE = 4
		};
		public: QList<QString> toolTipList;
		private: bool propertyValid[SIZE];
		public: ItemInputEvent(QTreeWidgetItem* _parent);
		private: virtual QVariant data(int _column, int _role)const;
		public: void SetDefaultState(void);
		public: bool IsValid(void)const;
		public: bool IsKeyValid(void)const;
		public: void SetPropertyValid(int _index, bool _valid);
	};


	Q_OBJECT
	private: QPixmap keyboardIcon;
	private: QPixmap mouseIcon;
	private: QPixmap gamepadIcon;
    
	private: QList<QString> deviceList;
	private: QList<QString> keyboardEventTypeList;
	private: QList<QString> mouseEventTypeList;
    private: UniversalDelegate* delegate;
	private: QList<QString> keyMouseEventTypeList;
    private: QPoint cursorPoint;
	public: PlayerControlEventListWidget(QWidget* _parent = 0);
	public: virtual ~PlayerControlEventListWidget(void);
	private: void UpdateItem(QString _deviceType, QTreeWidgetItem* _it, int _column);
	private: int GetEditorTypeForColumn(QString _deviceType, ItemInputEvent* _it, int _column);
	private: void SetDefaultEventValid(ItemInputEvent* _it);
	private: void UpdateEventValid(ItemInputEvent* _it);
	private slots: void UpdateRow(QTreeWidgetItem* _it, int _column);
	private slots: void ItemClicked(QTreeWidgetItem* _it, int _column);
	private: virtual void drawBranches(QPainter* _painter, const QRect& _rect, const QModelIndex& _index)const;
	protected: virtual void mouseDoubleClickEvent(QMouseEvent* _event);
	protected: virtual void keyPressEvent(QKeyEvent* _event);
	protected: virtual void mousePressEvent(QMouseEvent* _event);
	public slots: void RemoveSelectedItems(void);
	public: void NewEvent(InputEvent* _event);
	private slots: void NewEvent(void);
	signals: void UpdateEvents(void);
	private: QString GetKeyboardEventType(int32 _eventType);
	private: QString GetMouseEventType(int32 _eventType);
	private: QString GetMouseEventKey(int32 _eventKey);
	public: void GetEventList(QList<QTreeWidgetItem*>& _list);
	private: void GetItemList(QTreeWidgetItem* _it, QList<QTreeWidgetItem*>& _list);
	protected: virtual void dropEvent(QDropEvent * _event);
	private: QTreeWidgetItem* GetTopItem(QList<QTreeWidgetItem*> &_selectedList);
	private: void BackDrop(QList<QTreeWidgetItem*> &_selectedList, QTreeWidgetItem* _parent, int _indicator);
};


#endif 

