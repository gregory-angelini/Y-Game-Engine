#ifndef QUATERNIONVIEWER_H
#define QUATERNIONVIEWER_H
#include <Engine/Helpers/Engine/Engine.h>
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
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
class QuaternionCreator;
class QuaternionGizmo: public GraphicsObject
{
	public: enum RotationMode
	{
		EULER_SCROLLBAR,
		SPHERICAL_SWING_TWIST,
		SPHERICAL_TWO_POLE,
		SPHERICAL_FROM_TO
	};
	private: Vector3 xAxis;
	private: Vector3 yAxis;
	private: Vector3 zAxis;
	private: RGBA xColor;
	private: RGBA yColor;
    private: RGBA zColor;
	private: GraphicsSurface *xAxisArrow, *yAxisArrow, *zAxisArrow;

    private: int32 segments;
	private: Vector3 point1, point2;
	private: float scale;
	private: float step;
	private: Quaternion qX;
	private: Quaternion qY;
	private: Quaternion qZ;

	private: Vector3 xAxisBegin, xAxisEnd, yAxisBegin, yAxisEnd, zAxisBegin, zAxisEnd;
	private: Font2D* font;
	private: Quaternion quaternion;
	private: RotationMode rotationMode;

	private: bool showGlobalSpins;
	private: bool showGlobalAxises;
	public: QuaternionGizmo(void);
	public: virtual ~QuaternionGizmo(void);
	public: void ShowGlobalSpins(int _state);
	public: void ShowGlobalAxises(int _state);
	public: RGBA GetXColor(void)const;
	public: RGBA GetYColor(void)const;
	public: RGBA GetZColor(void)const;
	public: void SetRotationMode(RotationMode _mode);
	public: RotationMode GetRotationMode(void)const;
	public: StringANSI GetRotationModeString(void)const;
	public: Quaternion GetQuaternion(void)const;
	private: void DrawXAxis(void);
	private: void DrawYAxis(void);
	private: void DrawZAxis(void);
	public: virtual void Draw(void);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);

};
class QuaternionSphere: public GraphicsSurface,
	                    public LogicObject
{
	public: enum
	{
		NO_SELECTED,
		CIRCLE_X,
		CIRCLE_Y,
		CIRCLE_Z
	};
	private: Vector3 xAxis;
	private: Vector3 yAxis;
	private: Vector3 zAxis;
	private: RGBA xColor;
	private: RGBA yColor;
    private: RGBA zColor;
	private: int32 segments;
	private: Vector3 point1, point2;
	private: float scale;
	private: float step;
	private: Quaternion qX;
	private: Quaternion qY;
	private: Quaternion qZ;
	private: GraphicsSurface* face;
	private: RGBA faceColor;
	private: int32 selected;
	public: QuaternionSphere(void);
	public: virtual ~QuaternionSphere(void);
	public: virtual void Update(float _delta);
	public: virtual void Draw(void);
	public: RGBA GetXColor(void)const;
	public: RGBA GetYColor(void)const;
	public: RGBA GetZColor(void)const;
	public: void SelectX(void);
	public: void SelectY(void);
	public: void SelectZ(void);
	public: int32 GetSelected(void)const;
	public: void Unselect(void);
	public: virtual bool SaveToFile(File& _file);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
};
class QuaternionViewer: public Engine
{
	private: QuaternionCreator* quaternionCreator;

	private: Camera3D* camera, *camera2d;
    private: Font2D *font;
	private: Scene* scene, *scene2d;
	private: float zoom100;
	
	private: bool rightPressed;
	private: QuaternionSphere* sphere;
	private: QuaternionGizmo* gizmo;
	private: RGBA pixelColorUnderCursor;

	private: Vector3 previousCursorPosition;
	private: Vector3 startCursorPosition;
	private: Vector3 startSpherePosition;
	private: bool startSphereRotation;
    private: TexturingSurface* clenchedHandCursor;
	private: TexturingSurface* unclenchedHandCursor;
	private: TexturingSurface* arrowCursor;
	private: StringANSI relativeRotationString;
	public: QuaternionViewer(QuaternionCreator* _quaternionCreator);
	public: virtual ~QuaternionViewer(void);
	public: QuaternionSphere* GetSphere(void)const;
	public: StringANSI GetRelativeRotationString(void);
	public: bool Create(Engine::Define _define);
	public: virtual void Update(float _delta);
	public: virtual void Draw(float _delta);
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event);
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event);
	public: void ShowCursor(void);
	public: void HideCursor(void);
	public: void SetRightView(void);
	public: void SetTopDownView(void);
	public: void SetFrontView(void);
	public: void SetEulerScrollbar(void);
	public: void SetSphericalSwingTwist(void);
	public: void SetSphericalTwoPole(void);
	public: void SetSphericalFromTo(void);
	public: void SetZoom100(void);
	public: void ShowGlobalSpins(int _state);
	public: void ShowGlobalAxises(int _state);
	private: void UpdateRelativeRotation(Vector3 _currentCursorPosition);
};




#endif 
