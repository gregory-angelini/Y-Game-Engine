#ifndef TEXTURESHAPELISTWIDGET_H
#define TEXTURESHAPELISTWIDGET_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
class TextureShapeListWidget: public QTreeWidget
{
	public: class ItemShape: public QTreeWidgetItem
	{
		public: enum
		{
			NAME_PROPERTY = 1,
			COLLISION_GROUP_PROPERTY = 2,
			COLLISION_ENABLE_PROPERTY = 3,
			COLLISION_POLYGONS_INFO_PROPERTY = 4,
			BOUNDING_VOLUME_TYPE_PROPERTY = 5
		};
		private: enum 
		{
			SIZE = 4
		};
		public: ItemShape(QTreeWidgetItem* _parent = NIL);
	};


	Q_OBJECT
    private: UniversalDelegate* delegate;
    private: QPoint cursorPoint;
	private: QList<QString> boundingVolumeComboList;
    private: QMenu* contextMenu;
    private: QAction* importShapeAct;
	private: QAction* saveShapeAsAct;
	private: QAction* removeShapeAct;
	public: TextureShapeListWidget(QWidget* _parent = 0);
	public: virtual ~TextureShapeListWidget(void);
	public: QString GetStringBoundingVolumeType(int _shapeType);
	public: void AddShape(QString _name, int _collisionGroup = 0, bool _collisionCheck = true, bool _collisionPolygonsInfo = false, int _shapeType = Mesh::BOUNDING_CIRCLE);
	public slots: void RemoveSelectedShape(void);
	public: int32 GetSelectedShapeIndex(void)const;
	public: QString GetSelectedShapeName(void)const;
	public: ItemShape* GetSelectedShape(void)const;
    public: void ResetSelectedShape(void);
	public: void SelectShape(int32 _index);
	private: void CreateContextMenu(void);
	private: void UpdateContextMenu(void);
    signals: void ImportShape(void);
    signals: void SaveShapeAs(void);
    signals: void RemoveShape(void);
	protected: virtual void mousePressEvent(QMouseEvent* _event);
};


#endif 

