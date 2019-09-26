#ifndef FLOATLERPVIEWER_H
#define FLOATLERPVIEWER_H
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
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
class FloatLerpCreator;
class FloatLerpViewer: public Engine,
         	           public MessageHandler<int32>::Receiver
{
	private: FloatLerpCreator* floatLerpCreator;
	private: Camera3D* camera, *camera2d;
    private: Font2D *font;
	private: Scene* scene, *scene2d;
	private: bool lerpMarker;
	private: Graph* graph;
	private: bool rightPressed;
	private: int32 index;
    private: FloatLerp* lerp;
    private: TexturingSurface* clenchedHandCursor;
	private: TexturingSurface* unclenchedHandCursor;
	private: TexturingSurface* arrowCursor;
	private: Graph::Function *graphLerp;
	private: int32 selectedMarker;
	private: bool startMarkerMoving;
	private: Vector3 startMarkerPosition;
    private: RGBA pixelColorUnderCursor;
	private: float minCameraScale;
	private: float maxCamaraScale;
	private: float ñamaraZoomSpeed;
	public: FloatLerpViewer(FloatLerpCreator* _floatLerpCreator);
	public: virtual ~FloatLerpViewer(void);
	public: void ShowCursor(void);
	public: void HideCursor(void);
	public: bool Create(Engine::Define _define);
	public: virtual void Update(float _delta);
	public: virtual void Draw(float _delta);
	public: Camera3D* GetCamera(void)const;
	public: void CheckCollisionWithMarkers(void);
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event);
	public: void Select(void);
    public: void Unselect(void);
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event);
	public: FloatLerp* GetFloatLerp(void)const;
	public: float GetPlayingDurationInSec(void);
	public: Graph* GetGraph(void)const;
	public: void UpdateMark(void);
    public: void SetTarget(float _target);
    public: void SetPlayingSpeed(float _speed);
    public: void SetPlayingDurationInSec(float _duration);
	public: void ClearHistory(void);
	public: void PlayLerp(void);
	public: void PauseLerp(void);
	public: void StopLerp(void);
	public: void ShowCurveForAllLerps(void);
	public: void ShowCurveFromStartToCurrent(void);
	float GetParabolaPlayingDurationInSec(void);
	void SetParabolaPlayingDurationInSec(float _duration);
	float GetMagneticPlayingDurationInSec(void);
	void SetMagneticPlayingDurationInSec(float _duration);
	float GetDampingPlayingDurationInSec(void);
	void SetDampingPlayingDurationInSec(float _duration);
};




#endif 
