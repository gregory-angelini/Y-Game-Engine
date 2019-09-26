#ifndef DESIGNTOOLBAR_H
#define DESIGNTOOLBAR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Editor/AssetManager/ObjectTreeWidget/ObjectTreeWidget.h>
#include <Editor/AssetManager/LogicTreeWidget/LogicTreeWidget.h>
#include <Editor/AssetManager/GraphicsTreeWidget/GraphicsTreeWidget.h>
class DesignToolBar: public QToolBar
{
	Q_OBJECT
	public: enum 
	{
		NO_TOOL,
		SELECT,
		SCALE,
		TRANSLATE,
		ROTATE,
		ERASE
	};
	private: QAction* selectAction;
    private: QAction* eraseAction;
    private: int currentTool;
	private: QAction* viewAction;
	private: QToolButton* viewToolButton;
	private: QMenu* viewContextMenu;
	private: bool viewToolEnable;

	private: QAction* viewToolHomeAct;
	private: QAction* viewToolShowSightAct;
	private: QAction* viewToolMovementEnableAct;
	private: QMenu* viewToolRotationModeMenu;
	private: QAction* viewToolFirstPersonRotateModeAct;
	private: QAction* viewToolObjectViewerRotateModeAct;

	private: QAction* viewZoomEnableAct;
	private: QAction* viewRotationEnableAct;
	private: QMenu* viewToolProjectionModeMenu;
	private: QAction* viewToolPerspectiveProjectionModeAct;
	private: QAction* viewToolOrthographicProjectionModeAct;
	private: QAction* viewToolScreenProjectionModeAct;
	private: QMenu* viewToolMovementModeMenu;
	private: QAction* viewToolSpectatorMovementModeAct;
	private: QAction* viewToolOverlandMovementModeAct;
	private: QAction* viewToolFlightMovementModeAct;
	private: QAction* scaleAction;
	private: QToolButton* scaleToolButton;
	private: QMenu* scaleContextMenu;
	private: QAction* scaleToolHomeAct;
	private: QAction* moveAction;
	private: QToolButton* moveToolButton;
	private: QMenu* moveContextMenu;
	private: QAction* moveToolHomeAct;
	private: QAction* rotateAction;
	private: QToolButton* rotateToolButton;
	private: QMenu* rotateContextMenu;
	private: QAction* rotateToolHomeAct;
	public: DesignToolBar(QWidget* _parent = 0);
	public: virtual ~DesignToolBar(void);
	public: bool IsViewToolEnabled(void);
	public: int GetCurrentTool(void)const;
    private: void CreateSelectTool(void);
    private: void CreateScaleTool(void);
	private: void CreateMoveTool(void);
    private: void CreateRotateTool(void);
    private: void CreateEraseTool(void);
    private: void CreateViewTool(void);
	public slots: void SetSelectTool(void);
	public slots: void SetScaleTool(void);
    public slots: void SetMoveTool(void);
    public slots: void SetRotateTool(void);
    public slots: void SetEraseTool(void);
	signals: void PickRotateTool(void);
	signals: void PickScaleTool(void);
	signals: void PickSelectTool(void);
	signals: void PickMoveTool(void);
	signals: void PickEraseTool(void);
	signals: void UnpickSelectTool(void);
	signals: void UnpickMoveTool(void);
	signals: void UnpickRotateTool(void);
	signals: void UnpickScaleTool(void);
	signals: void UnpickEraseTool(void);
	private slots: void UpdateViewContextMenu(const QPoint& _point);
	private: void UpdateViewContextMenu(void);
	private slots: void ViewToolHome(void);
	private: void CreateViewContextMenu(void);
	signals: void CameraHomeIsChecked(void);
	private slots: void ViewToolMovementEnable(void);
	public: void SetViewMovementEnable(bool _enable);
	private slots: void ViewToolShowSightEnable(void);
	public: void SetViewShowSightEnable(bool _enable);
	private slots: void ViewToolPerspectiveProjectionMode(void);
	private slots: void ViewToolOrthographicProjectionMode(void);
	private slots: void ViewToolScreenProjectionMode(void);
	signals: void CameraPerspectiveProjectionIsChecked(void);
	signals: void CameraOrthographicProjectionIsChecked(void);
	signals: void CameraScreenProjectionIsChecked(void);
	private slots: void ViewToolObjectViewerIsChecked(void);
	private slots: void ViewToolFirstPersonIsChecked(void);
	signals: void CameraFirstPersonIsChecked(void);
	signals: void CameraObjectViewerIsChecked(void);
	signals: void CameraShowSightIsChecked(bool _enable);
	signals: void CameraMovementEnableIsChecked(bool _enable);
	signals: void CameraZoomEnableIsChecked(void);
	signals: void CameraRotationEnableIsChecked(void);
	private slots: void ViewToolSpectatorMovementMode(void);
	private slots: void ViewToolOverlandMovementMode(void);
	private slots: void ViewToolFlightMovementMode(void);
	signals: void CameraSpectatorIsChecked(void);
	signals: void CameraOverlandIsChecked(void);
	signals: void CameraFlightIsChecked(void);
	private slots: void ScaleToolHome(void);
	private: void CreateScaleContextMenu(void);
	private slots: void ScaleContextMenu(const QPoint& _point);
	signals: void ScaleHomeIsChecked(void);
	private slots: void MoveToolHome(void);
	private: void CreateMoveContextMenu(void);
	private slots: void MoveContextMenu(const QPoint& _point);
	signals: void MoveHomeIsChecked(void);
	private slots: void RotateToolHome(void);
	private: void CreateRotateContextMenu(void);
	private slots: void RotateContextMenu(const QPoint& _point);
	signals: void RotateHomeIsChecked(void);
};


#endif 

