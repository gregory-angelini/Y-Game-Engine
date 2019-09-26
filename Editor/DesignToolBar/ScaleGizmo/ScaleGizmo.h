#ifndef SCALEGIZMO_H
#define SCALEGIZMO_H
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
#include <Engine/Core/Shape/Box/Box.h>
class ScaleGizmo: public GraphicsSurface
{
	public: enum
	{
		NO_SELECT = -1,

		X_POINT1 = 1,
		X_POINT2,

		Y_POINT1,
		Y_POINT2,

		Z_POINT1,
		Z_POINT2,
		
		XY_POINT1,
		XY_POINT2,
		XY_POINT3,
		XY_POINT4,

		XZ_POINT1,
		XZ_POINT2,
		XZ_POINT3,
		XZ_POINT4,

		YZ_POINT1,
		YZ_POINT2,
		YZ_POINT3,
		YZ_POINT4
	};
	public: enum
	{
		TOP_FACE = 1,
		BOTTOM_FACE,
		FRONT_FACE,
		BACK_FACE,
		LEFT_FACE,
		RIGHT_FACE
	};
	private: Vector3 intersectedPoint;
	private: float aabbScale;
	private: Box aabb;
	private: RGBA zColor, yColor, xColor, selectColor, cornerPointColor;

    private: float scale;
    private: bool startScaling;

    private: int32 selected;
	public: int32 selectedFace;

	private: GraphicsSurface* vFace1, *vFace2, *vFace3, *vFace4;
	private: GraphicsSurface* vBack1, *vBack2, *vBack3, *vBack4;
	private: GraphicsSurface* vY1, *vY2, *vY3, *vY4;
	private: GraphicsSurface* vX1, *vX2, *vX3, *vX4;
	private: GraphicsSurface* vZ1, *vZ2, *vZ3, *vZ4;
	private: Plane plane;
	private: Vector3 maxScale;
	private: float vertexSize;
	private: MagicPointer<GraphicsSurface> targetObject;
	private: float scaleTiny;

	private: MagicList<GraphicsSurface> selectedList;

	private: TexturingSurface* gizmo;
    private: Texture2D* texture;
	private: Vector3 previousCursorPosition;
	private: Vector3 startCursorPosition;
	private: Mesh* mesh;

	private: TexturingSurface* clenchedHandCursor;
	private: TexturingSurface* unclenchedHandCursor;
	private: TexturingSurface* arrowCursor;
	public: ScaleGizmo(void);
	public: virtual ~ScaleGizmo(void);
	public: void SetHome(void);
	public: GraphicsSurface* GetTargetObject(void)const;
	public: void Update(void);
	private: void ClampScale(GraphicsSurface* _object);
	public: void Reset(void);
	public: TexturingSurface* GetGizmoSprite(void)const;
	public: void Init(TexturingSurface* _arrowCursor, TexturingSurface* _unclenchedHandCursor, TexturingSurface* _clenchedHandCursor);
	public: void SetSelectedList(MagicList<GraphicsSurface>& _list);
	public: MagicList<GraphicsSurface>& GetSelectedList(void);
	public: void ClearSelectedList(void);
	public: virtual void Draw(void);
	private: void UpdateAABB(GraphicsSurface* _object);
	public: Mouse::Event* MouseEventFilter(Mouse::Event* _event);
	public: void CheckSelect(void);
};



#endif 
