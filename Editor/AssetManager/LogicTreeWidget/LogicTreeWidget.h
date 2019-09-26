#ifndef LOGICTREEWIDGET_H
#define LOGICTREEWIDGET_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/AssetManager/LogicTreeWidget/LogicSceneItem.h>
#include <Engine/Core/Object/LogicObject/LogicObject.h>
class LogicTreeWidget: public QTreeWidget,
			           public MessageHandler<int32>::Receiver
{
	Q_OBJECT
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
	friend class LogicSceneItem;
	private: QPixmap sceneIcon;
	private: QPixmap objectIcon;
	private: QPixmap floatLerpIcon;
	private: QPixmap timerIcon;
	private: QPixmap texturedAnimationIcon;
    private: QMenu* contextMenu;
    private: QAction* destroyItemAct;
	private: QAction* renameItemAct;
	private: bool ignoreSelection;
	private: QAction* copyItemAct;
	private: QAction* pickItemAct;
	private: QAction* saveToFileItemAct;
	private: QAction* importItemAct;
	private: QAction* saveAllAssetsAct;
	private: QAction* saveLayersStructureAct;
	private: QAction* reviewItemAct;
	private: QAction* removeFromSceneAct;

	private: QPoint cursorPoint;
	private: LogicSceneItem* root;
	private: int scrollTimerId;
    private: QTreeWidgetItem* dropItem;
    private: bool autoParentEnable;
	public: LogicTreeWidget(QWidget* _parent = 0);
	public: virtual ~LogicTreeWidget(void);
	public slots: void ImportAsset(void);
	public slots: void ReviewAsset(void);
	private: LogicSceneItem::Type GetAssetType(AbstractObject* _object);
	private slots: void RenameCurrentItem(void);
	private slots: void PickCurrentItem(void);
	private slots: void CopyCurrentItem(void);
	private slots: void SaveToFileSelectedItems(void);
	private: void ObjectIsCreated(AbstractObject* _object);
	private: void ObjectIsRemoved(AbstractObject* _object);
	private: void ObjectIsRenamed(AbstractObject::NameInfo* _info);
	protected: virtual void mousePressEvent(QMouseEvent* _event);
	protected: virtual void mouseReleaseEvent(QMouseEvent* _event);
	protected: virtual void mouseDoubleClickEvent(QMouseEvent* _event);
	protected: virtual void keyPressEvent(QKeyEvent* _event);
	protected: void virtual keyReleaseEvent(QKeyEvent* _event);
	public: QPoint GetCursorPoint(void)const;
	protected: virtual void mouseMoveEvent(QMouseEvent* _event);
	public slots: void SaveAllAssets(void);
	private: void SaveAllAssets(QTreeWidgetItem* _it);
	public slots: void SaveLayersStructure(void);
	private: void SaveLayersStructure(QTreeWidgetItem* _it);
	private: void CreateContextMenu(void);
	private slots: void DestroySelectedItems(void);
	private slots: void RemoveFromSceneSelectedItems(void);
	private: void UpdateContextMenu(void);
	public slots: void NewAsset(void);
	public: LogicSceneItem* AddAsset(QString _name, LogicSceneItem::Type _assetType, int64 _objectClass, LogicObject* _object, LogicSceneItem* _parent = 0);
	public: QPixmap GetIcon(int _assetType);
	public: QString GetStringOfAssetType(LogicSceneItem::Type _assetType)const;
	public: void CreateRoot(void);
	public: LogicSceneItem* FindItem(QString _name);
	private: LogicSceneItem* FindItem(QTreeWidgetItem* _it, QString _name);
	public: LogicSceneItem* FindItem(QString _name, QString _parent);
	private: LogicSceneItem* FindItem(QTreeWidgetItem* _it, QString _name, QString _parent);
	private: LogicSceneItem* FindLeaf(QTreeWidgetItem* _item);
	private: QTreeWidgetItem* FindLeaf(QList<QTreeWidgetItem*>& _list);
	protected: virtual void timerEvent(QTimerEvent* _event);
	private: void Scroll(void);
	public: void ExpandRoot(void);
	protected: virtual void dropEvent(QDropEvent* _event);
	protected: virtual void dragMoveEvent(QDragMoveEvent* _event);
	protected: virtual void dragEnterEvent(QDragEnterEvent* _event);
};


#endif 

