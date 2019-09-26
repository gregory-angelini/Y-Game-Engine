#ifndef ASSETSLIBRARYTREE_H
#define ASSETSLIBRARYTREE_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
class AssetsLibraryTree: public QTreeWidget
{
	Q_OBJECT
	private: class Item: public QTreeWidgetItem
	{
		public: enum
		{
			ASSET_TYPE,
			CATEGORY_TYPE
		};
		private: int itemType;
		private: int assetType;
		private: QString name;
		private: bool nameReserved;
		public: Item(QTreeWidgetItem* _parent, int _assetType, int _itemType): QTreeWidgetItem(_parent, _itemType), nameReserved(false)
		{
			SetItemType(_itemType);
			SetAssetType(_assetType);
		}
		private: void SetItemType(int _type)
		{
			itemType = _type;
		}
		public: int GetItemType(void)const
		{
			return itemType;
		}
		public: void SetAssetType(int _type)
		{
			assetType = _type;
		}
		public: int GetAssetType(void)const
		{
			return assetType;
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
				delete child(0);
			}
		}
		public: bool IsEmpty(void)const
		{
			return child(0) == 0;
		}
		public: bool IsNameReserved(void)const
		{
			return nameReserved;
		}
		public: void SetNameReserved(bool _enable)
		{
			nameReserved = _enable;
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
    private: QMenu* contextMenu;
	private: QAction* newAssetAct;
	private: QAction* refreshAct;
	private: QAction* editAssetAct;
	private: QAction* removeItemAct;
	private: QPoint cursorPoint;

	private: QColor categoryBeginColor;
	private: bool creatingAssetsEnable;
	private: bool editingAssetsEnable;
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
    private: QPixmap hitboxIcon;
	
	private: QPixmap floatLerpIcon;
	private: QPixmap quaternionLerpIcon;
	private: QPixmap vectorLerpIcon;

	private: QPixmap cameraIcon;
	
	private: QPixmap folderIcon;
	public: AssetsLibraryTree(QWidget* _parent = 0);
	public: virtual ~AssetsLibraryTree(void);
	protected: void mousePressEvent(QMouseEvent* _event);
	protected: virtual void mouseDoubleClickEvent(QMouseEvent* _event);
	protected: virtual void keyPressEvent(QKeyEvent* _event);
	public: void Refresh(void);
    private slots: void RefreshAssetTree(void);
	private: void CreateCategory(int _assetType);
	signals: void RefreshIsFinished(void);
	signals: void AssetIsRemoved(void);
	public: void AddAsset(QString _name, int _assetType);
	public: Item* AddCategory(int _assetType);
	public: QPixmap GetIcon(int _assetType);
	public: QString GetStringOfAssetType(int _assetType)const;
	public: int GetStringOfAssetType(QString _assetType)const;
	public slots: void NewAsset(void);
	public slots: void EditAsset(void);
	public: void SetCreatingAssetsEnable(bool _enable);
	public: void SetEditingAssetsEnable(bool _enable);
	private slots: void RemoveSelectedItem(void);
	private: QTreeWidgetItem* FindLeaf(QList<QTreeWidgetItem*>& _list);
	public: Item* FindAsset(QString _name);
	private: Item* FindAsset(QTreeWidgetItem* _it, QString _name);
	private: Item* FindCategory(QString _name);
	private: Item* FindCategory(QTreeWidgetItem* _it, QString _name);
	public: int GetAmountOfItems(QTreeWidgetItem* _it);
	private: virtual void drawRow(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index)const;
	private: void CreateContextMenu(void);
	private: void UpdateContextMenu(void);
	protected: virtual void mouseMoveEvent(QMouseEvent *_event); 
};


#endif 

