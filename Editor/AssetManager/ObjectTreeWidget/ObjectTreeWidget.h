#ifndef OBJECTTREEWIDGET_H
#define OBJECTTREEWIDGET_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/AssetManager/ObjectTreeWidget/ObjectSceneItem.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Output/Graphics/Surface/GraphicsSurface/GraphicsSurface.h>
class ObjectTreeWidget: public QTreeWidget,
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
	friend class ObjectSceneItem;
	private: QPixmap texturedAnimationIcon;
	private: QPixmap atlasIcon;
	private: QPixmap textureIcon;

	private: QPixmap triggerIcon;
	private: QPixmap variableIcon;
	private: QPixmap listIcon;
	private: QPixmap actionIcon;
	private: QPixmap eventIcon;

	private: QPixmap sceneIcon;
	private: QPixmap objectIcon;
	private: QPixmap playerControlIcon;
	private: QPixmap meshIcon;
	private: QPixmap timerIcon;
	private: QPixmap hitboxIcon;
	
	private: QPixmap floatLerpIcon;
	private: QPixmap quaternionLerpIcon;
	private: QPixmap vectorLerpIcon;

	private: QPixmap cameraIcon;
    private: QMenu* contextMenu;
    private: QAction* destroyItemAct;
	private: QAction* removeFromSceneAct;
	private: QAction* renameItemAct;
	private: QAction* saveToFileItemAct;
    private: QAction* reviewItemAct;

	
	private: QAction* copyItemAct;
	private: QAction* activateItemAct;
	private: QAction* pickItemAct;

	private: QAction* importItemAct;
	private: QAction* saveAllAssetsAct;
	private: QAction* saveLayersStructureAct;

	private: bool ignoreSelection;


	private: QPoint cursorPoint;
	private: ObjectSceneItem* root;
	private: int scrollTimerId;
	private: MagicList<GraphicsSurface> selectList;
    private: QTreeWidgetItem* dropItem;
	private: bool autoParentEnable;
	private: bool altPressed;
	public: ObjectTreeWidget(QWidget* _parent = 0);
	public: virtual ~ObjectTreeWidget(void);
	public: void Select(GraphicsSurface* _object, bool _ignoreSelection = true);
	public: void Unselect(GraphicsSurface* _object, bool _ignoreSelection = true);
	private slots: void ChangeSelectList(void);
	private: void ObjectIsCreated(AbstractObject* _object);
	private: void ObjectIsRemoved(AbstractObject* _object);
	private: void ObjectIsRenamed(AbstractObject::NameInfo* _info);
	private slots: void RenameCurrentItem(void);
	private slots: void CopyCurrentItem(void);
	private slots: void RemoveFromSceneSelectedItems(void);
	private slots: void PickCurrentItem(void);
	private slots: void ActivateCurrentItem(void);
	public: ObjectSceneItem* FindItem(AbstractObject* _object);
	private: ObjectSceneItem* FindItem(QTreeWidgetItem* _it, AbstractObject* _object);
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
	private slots: void SaveToFileSelectedItems(void);
	private: void UpdateContextMenu(void);
	public slots: void NewAsset(void);
	public slots: void ImportAsset(void);
	public slots: void ReviewAsset(void);
	public: ObjectSceneItem* AddAsset(QString _name, ObjectSceneItem::Type _assetType, int64 _objectClass, AbstractObject* _object, ObjectSceneItem* _parent = 0);
	public: QPixmap GetIcon(int _assetType);
	public: QString GetStringOfAssetType(ObjectSceneItem::Type _assetType)const;
	public: void CreateRoot(void);
	public: ObjectSceneItem* FindItem(QString _name);
	private: ObjectSceneItem* FindItem(QTreeWidgetItem* _it, QString _name);
	public: ObjectSceneItem* FindItem(QString _name, QString _parent);
	private: ObjectSceneItem* FindItem(QTreeWidgetItem* _it, QString _name, QString _parent);
	private: ObjectSceneItem* FindLeaf(QTreeWidgetItem* _item);
	private: QTreeWidgetItem* FindLeaf(QList<QTreeWidgetItem*>& _list);
	protected: virtual void timerEvent(QTimerEvent* _event);
	private: void Scroll(void);
	public: void ExpandRoot(void);
	signals: void SelectObject(GraphicsSurface* _object);
	signals: void UnselectObject(GraphicsSurface* _object);
	protected: virtual void dropEvent(QDropEvent* _event);
	protected: virtual void dragMoveEvent(QDragMoveEvent* _event);
	protected: virtual void dragEnterEvent(QDragEnterEvent* _event);
};


#endif 

