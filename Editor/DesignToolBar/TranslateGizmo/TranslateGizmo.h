#ifndef TRANSLATEGIZMO_H
#define TRANSLATEGIZMO_H
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
class TranslateGizmo: public GraphicsSurface
{
	public: enum
	{
		NO_SELECT,
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
		XZ_PLANE,
		XY_PLANE,
		YZ_PLANE
	};
	private: GraphicsSurface *XZ, *XY, *YZ;
	private: GraphicsSurface *xDir, *yDir, *zDir;
    private: RGBA zColor, yColor, xColor;
	private: RGBA selectColor;
	private: RGBA xyColor, xzColor, yzColor;
	private: Plane* xyPlane, *xzPlane, *yzPlane;
	private: float dirLength;

	private: MagicPointer<GraphicsSurface> targetObject;
	private: MagicList<GraphicsSurface> selectedList;
	private: bool translatingEnable;
	private: Vector3 intersectedPoint;
	private: Vector3 previousIntersectedPoint;
    private: int32 selected;
	private: Line selectRay;
	private: float gizmoSize;
  
	private: TexturingSurface* clenchedHandCursor;
	private: TexturingSurface* unclenchedHandCursor;
	private: TexturingSurface* arrowCursor;

	private: TexturingSurface* gizmo;
    private: Texture2D* texture;
	public: TranslateGizmo(void);
	public: virtual ~TranslateGizmo(void);
	public: void Reset(void);
	public: void SetHome(void);
	public: void Init(TexturingSurface* _arrowCursor, TexturingSurface* _unclenchedHandCursor, TexturingSurface* _clenchedHandCursor);
	public: void SetSelectedList(MagicList<GraphicsSurface>& _list);
	public: void ClearSelectedList(void);
	private: void CheckSelect(void);
	public: Mouse::Event* MouseEventFilter(Mouse::Event* _event);
	public: virtual void Draw(void);
	public: virtual void SetPosition(Vector3 _position);
	public: bool IsSelected(void)const;
	public: TexturingSurface* GetGizmoSprite(void)const;
	public: GraphicsSurface* GetTargetObject(void)const;
	public: MagicList<GraphicsSurface>& GetSelectedList(void);

};



#endif 
