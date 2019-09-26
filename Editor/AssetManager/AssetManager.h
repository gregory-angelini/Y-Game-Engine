#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Editor/AssetManager/ObjectTreeWidget/ObjectTreeWidget.h>
#include <Editor/AssetManager/LogicTreeWidget/LogicTreeWidget.h>
#include <Editor/AssetManager/GraphicsTreeWidget/GraphicsTreeWidget.h>
#include <Editor/AssetManager/EventTreeWidget/EventTreeWidget.h>
class AssetManager: public QTabWidget
{
	Q_OBJECT
	private: ObjectTreeWidget* objectTreeWidget;
	private: GraphicsTreeWidget* drawTreeWidget;
    private: LogicTreeWidget* updateTreeWidget;
	private: EventTreeWidget* eventTreeWidget;
    private: QMenu *contextMenu;
	private: QAction *eventLogSettingsAct;
	private: QAction *eventLogClearAct;
	public: AssetManager(QWidget* _parent = 0);
	public: void UnselectObject(GraphicsSurface* _object);
	public: void SelectObject(GraphicsSurface* _object);
	public: ObjectTreeWidget* GetObjectTreeWidget(void)const;
	public: LogicTreeWidget* GetUpdateTreeWidget(void)const;
	public: GraphicsTreeWidget* GetDrawTreeWidget(void)const;
	public: EventTreeWidget* GetEventTreeWidget(void)const;
	public: void ExpandRoot(void);
	private: void CreateContextMenu(void);
	private: void ShowEventLogContextMenu(QPoint _mousePos);
	public: bool IsEventLogPostponeShowModeEnabled(void)const;
	public: bool IsEventLogRealTimeShowModeEnabled(void)const;
	protected: virtual bool eventFilter(QObject* _object, QEvent* _event);
};


#endif 

