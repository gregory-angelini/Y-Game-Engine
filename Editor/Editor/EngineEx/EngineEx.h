#ifndef ENGINEEX_H
#define ENGINEEX_H
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Output/Graphics/Surface/GraphicsSurface/GraphicsSurface.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Surface/TexturingSurface/TexturingSurface.h>
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Editor/DesignToolBar/RotateGizmo/RotateGizmo.h>
#include <Editor/DesignToolBar/TranslateGizmo/TranslateGizmo.h>
#include <Editor/DesignToolBar/ScaleGizmo/ScaleGizmo.h>
class Editor;
class EngineEx: public Engine,
		        public MessageHandler<int32>::Receiver
{
	private: Editor* editor;

    private: TexturingSurface* clenchedHandCursor;
	private: TexturingSurface* unclenchedHandCursor;
	private: TexturingSurface* arrowCursor;

    private: Font2D *font;
	private: Camera3D *camera2d;
	private: Scene *scene2d;
	private: float timeSpeed;
    private: TranslateGizmo* translateGizmo;
	private: Scene *translateGizmoScene3d;
    private: Scene *translateGizmoScene2d;
	private: RotateGizmo* rotateGizmo;
	private: Scene *rotateGizmoScene3d;
	private: Scene *rotateGizmoScene2d;
	private: ScaleGizmo* scaleGizmo;
	private: Scene *scaleGizmoScene3d;
	private: Scene *scaleGizmoScene2d;
	private: bool mouseRightButtonPressed;
	private: bool showCursorEnable;
    private: bool cursorShowEnable;

	private: float camera2dMoveSpeed;
	private: float camera3dMoveSpeed;

	private: float cameraMinScale;
	private: float camaraMaxScale;
	private: float ñamaraZoomSpeed;

	private: bool cameraMovementEnable;
    private: Keyboard::Key cameraMoveUpKey, cameraMoveDownKey, cameraMoveLeftKey, cameraMoveRightKey, cameraMoveForwardKey, cameraMoveBackwardKey;

    private: float cameraSightSize;
	private: RGBA cameraSightColor;
	private: bool cameraShowSightEnable;
	
	private: bool showCurrentTime;
	private: float currentTimeInSec;
	private: MagicList<GraphicsSurface> selectedList;
	public: EngineEx(Editor* _editor);
	public: virtual ~EngineEx(void);
	public: virtual bool Create(Define _define);
    private: void TranslateGizmoInit(void);
    private: void RotateGizmoInit(void);
    private: void ScaleGizmoInit(void);
    private: void CursorInit(void);
	public: void SetTimeSpeed(float _scale);
	public: void TimeShowEnable(bool _enable);
	public: void ResetTime(void);
	public: void ResetTool(int32 _toolId);
	public: void ResetDesignTools(void);
	public: void SetScaleHome(void);
	public: void SetMoveHome(void);
	public: void SetRotateHome(void);
	public: void SetViewZoomEnable(void);
	public: void SetViewRotationEnable(void);
	public: void SetViewHome(void);
	public: void SetViewShowSightEnable(bool _enable);
    public: bool GetViewShowSightEnable(void)const;
	public: void SetViewMovementEnable(bool _enable);
	public: bool GetViewMovementEnable(void)const;
	public: void SetViewFirstPerson(void);
	public: void SetViewObjectViewer(void);
	public: void SetViewPerspective(void);
	public: void SetViewOrthographic(void);
	public: void SetViewScreen(void);
	public: void SetViewSpectator(void);
	public: void SetViewOverland(void);
	public: void SetViewFlight(void);
	private: void RotateCameraBegin(void);
	private: void RotateCameraEnd(void);
	public: void SetCamera3dMoveSpeed(float _moveSpeed);
	public: float GetCamera3dMoveSpeed(void)const;
	public: void SetCameraMoveUp(Keyboard::Key _key);
	public: void SetCameraMoveDown(Keyboard::Key _key);
	public: void SetCameraMoveLeft(Keyboard::Key _key);
	public: void SetCameraMoveRight(Keyboard::Key _key);
	public: void SetCameraMoveForward(Keyboard::Key _key);
	public: void SetCameraMoveBackward(Keyboard::Key _key);
	public: void UnselectObjects(void);
	public: void SelectObject(GraphicsSurface* _object);
	public: void UnselectObject(GraphicsSurface* _object);
	public: MagicList<GraphicsSurface> CheckObjectsUnderCursor(void);
	private: void CheckObjectsUnderCursor(MagicList<GraphicsObject>::AddressDerived* _address, MagicList<GraphicsSurface>& _list)const;
	private:  void CheckObjectsUnderCursor(TiledSpace3D* _tiledSpace, MagicList<GraphicsSurface>& _list)const;
    private: void SelectObject(void);
	private: void DestroySelectedObjects(void);
	public: virtual void Update(float _delta);
	public: virtual void Draw(float _delta);
	private: void DrawSight(void);
    private: void DrawDesignTool(void);
	public: virtual LRESULT Proc(HWND _hWindow, UINT _message, WPARAM _wParam, LPARAM _lParam);
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event);
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event);
};



#endif 
