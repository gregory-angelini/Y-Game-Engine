#ifndef ROTATEGIZMO_H
#define ROTATEGIZMO_H
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
class RotateGizmo: public GraphicsSurface
{
	public: enum
	{
		DEGREES,
		RADIANS
	};
	public: enum
	{
		NO_SELECT,
		X_AXIS,
		Y_AXIS,
		Z_AXIS
	};
	private: GraphicsSurface* sphere;
	private: int32 segments;
	private: float step;

	private: Quaternion qX;
	private: Quaternion qY;
	private: Quaternion qZ;

	private: Vector3 xAxis;
	private: Vector3 yAxis;
	private: Vector3 zAxis;
	
    private: float scale;
    private: bool startRotation;

    private: RGBA zColor, yColor, xColor, selectColor;
    private: int32 selected;

    private: Vector3 intersectedPoint;
    private: Vector3 point1, point2;
	private: MagicList<GraphicsSurface> selectedList;
	
	private: MagicPointer<GraphicsSurface> targetObject;
	private: TexturingSurface* gizmo;
    private: Texture2D* texture;
	private: Vector3 previousCursorPosition;
	private: Vector3 startCursorPosition;

	private: TexturingSurface* clenchedHandCursor;
	private: TexturingSurface* unclenchedHandCursor;
	private: TexturingSurface* arrowCursor;
	public: RotateGizmo(void);
	public: virtual ~RotateGizmo(void);
	public: void SetHome(void);
	public: GraphicsSurface* GetTargetObject(void)const;
	public: void Reset(void);
	public: TexturingSurface* GetGizmoSprite(void)const;
	public: void Init(TexturingSurface* _arrowCursor, TexturingSurface* _unclenchedHandCursor, TexturingSurface* _clenchedHandCursor);
	public: void SetSelectedList(MagicList<GraphicsSurface>& _list);
	public: MagicList<GraphicsSurface>& GetSelectedList(void);
	public: void ClearSelectedList(void);
	public: virtual void Draw(void);
	public: virtual void SetPosition(Vector3 _position);
	public: Mouse::Event* MouseEventFilter(Mouse::Event* _event);
	public: void CheckSelect(void);
};




#endif 
