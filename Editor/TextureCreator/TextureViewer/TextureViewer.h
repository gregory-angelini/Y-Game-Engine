#ifndef TEXTUREVIEWER_H
#define TEXTUREVIEWER_H
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/Surface/TexturingSurface/TexturingSurface.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Output/Graphics/Shape/Circle/GraphicsCircle/GraphicsCircle.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
class TextureCreator;
class TextureViewer: public Engine,
	                 public MessageHandler<int32>::Receiver
{
	public: class GraphicsPointEx: public GraphicsPoint
	{
		private: bool isSelected;
		private: RGBA normalColor;
		private: RGBA selectColor;
		private: RGBA convexColor;
		private: bool isConvex;
		public: GraphicsPointEx(void);
		public: virtual ~GraphicsPointEx(void);
		public: bool IsSelected(void)const;
		public: void SetSelected(bool _enable);
		public: bool IsConvex(void)const;
		public: void SetConvex(bool _enable);
	    public: void RestoreColor(void);
		public: virtual void Draw(void);
	};
	public: class GraphicsLineEx: public GraphicsLine
	{
		private: bool isIntersected;
		private: RGBA normalColor;
		private: RGBA intersectColor;
		public: GraphicsLineEx(void);
		public: virtual ~GraphicsLineEx(void);
	    public: void RestoreColor(void);
		public: bool IsIntersected(void)const;
		public: void SetIntersected(bool _enable);
	};
	public: class GraphicsSurfaceEx: public GraphicsSurface
	{
		private: bool isCurrent;
		private: int32 frameIndex;
		public: GraphicsSurfaceEx(void);
		public: virtual ~GraphicsSurfaceEx(void);
		public: bool IsCurrent(void)const;
		public: void SetCurrent(bool _enable);
		public: void SetFrameIndex(int32 _index);
		public: int32 GetFrameIndex(void)const;
		public: virtual void Draw(void);
	};
    private: static MagicPointer<Camera3D> camera3d;
	private: Camera3D* camera2d;
    private: MagicPointer<Scene> scene3d;
	private: MagicList<GraphicsObject> decorations;
	private: Scene *scene2d;
	private: TexturingSurface* sprite;
	private: Mesh* mesh;
	private: GraphicsPointEx* vectorLerpCurrentVertex;
	private: Font2D* font;

	private: VectorLerp* vectorLerp;
	private: FloatLerp* vectorLerpFloatLerp;
	private: SimpleList<Vector3> vectorLerpCurve;

	private: TextureCreator* textureCreator;
    private: TexturingSurface* clenchedHandCursor;
	private: TexturingSurface* unclenchedHandCursor;
	private: TexturingSurface* arrowCursor;

	private: Vector2 worldCursorPosition;
	private: Line selectRay;
	private: Vector3 intersectionPoint;
	private: Plane* plane;
	private: GraphicsPointEx* fantomVertex;
	private: GraphicsLineEx* fantomEdge;
	private: bool fantomVertexDrawing;
	private: bool fantomEdgeDrawing;
	private: Mesh* fantomMesh;
	private: GraphicsSurfaceEx* fantomObject;

	private: GraphicsPointEx* currentVertex;
	private: Vector3 startCurrentVertexPosition;
	private: Vector3 previousCurrentVertexPosition;

	private: float vectorLerpCurrentVertexSize;
	private: static float pointSizeInUnits;
	private: int32 pixelsPerMeter;
	private: float pointsSizeInPixels;
	private: float lineWidthInPixels;
	private: int32 gridSize;

    private: float cameraMoveSpeed;
	private: float cameraNearScale;
	private: float cameraFarScale;
	private: float ñamaraZoomSpeed;
	private: float sightSize;
	private: float circleRadius;
	private: Vector2 quadSize;

    private: MagicList<GraphicsPointEx> vertices;
	private: MagicList<Mesh> shapes;
	private: MagicList<GraphicsSurfaceEx> objects;
	private: SimpleList<int32> intersectedLines;
	private: GraphicsSurface* selectArea;
	private: Quad selectAreaQuad;
	private: bool selectAreaDrawing;
	private: Vector3 startSelectedAreaPosition;
	private: MagicList<GraphicsPointEx> selectedPoints;

	private: bool pivotShowEnable;
	public: TextureViewer(void);
	public: TextureViewer(TextureCreator* _textureCreator);
	public: virtual ~TextureViewer(void);
	public: virtual bool Create(Define _define);
	public: virtual void Update(float _delta);
	public: virtual void Draw(float _delta);
	private: void DrawPreviousAnimationFrame(void);
	private: void CursorSightDraw(void);
	private: void UpdateAnimationStatus(void);
	public: void SetHitbox(Hitbox* _hitbox);
	public: int GetIntBoundingVolumeType(StringANSI _shapeType);
	public: bool IsVertexOverlay(Vector3 _position);
	public: void CreateMesh(int32 _index = -1);
	public: void CreateMesh(Mesh* _mesh, StringANSI _name, int32 _collisionGroupIndex, bool _collisionEnable, bool _collisionPolygonsInfoEnable, int _boundingVolumeType);
	public: void CreateMesh(Mesh* _mesh);
	public: void RelockSelectedShape(void);
	private: void CheckConvex(void);
	private: void CheckEdgeIntersection(void);
	private: void UpdateFantomMesh(void);
	public: void DestroyPoints(void);
	public: void DestroySelectedPoints(void);
	public: void UnlockShape(int32 _index);
	public: void ChangePivot(void);
	public: void CreateSubPoints(void);
	private: GraphicsPointEx* CreateNewVertex(Vector3 _position);
	public: void HitboxSaveShapeAsMesh(StringANSI _name);
	public: void HitboxSaveShapeAsObject(StringANSI _name);
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event);
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event);
	private: void VectorLerpLeftButtonPressed(void);
	private: void HitboxLeftButtonPressed(void);
	private: void UpdatePivot(void);
	public: void CurrentBuildModeIsChanged(StringANSI _name);
	private: void VectorLerpRightButtonPressed(void);
	private: void HitboxRightButtonPressed(void);
	private: void HitboxMouseMove(void);
	private: void VectorLerpMouseMove(void);
	private: void VectorLerpLeftButtonReleased(void);
	private: void HitboxLeftButtonReleased(void);
	private: void VectorLerpRightButtonReleased(void);
	private: void HitboxRightButtonReleased(void);
	private: void UpdateCameraZoom(void);
	public: void CurrentTabIsChanged(StringANSI _name);
	public: void LoadShape(StringANSI _name);
	public: void LoadDecoration(StringANSI _name);
	public: void ClearDecorations(void);
	public: MagicList<GraphicsObject>& GetDecorations(void);
	public: float GetPixelsPerMeter(void)const;
	public: void SetPixelsPerMeter(int32 _units);
	public: int32 GetGridSize(void)const;
	public: void ShowCursor(void);
	public: void HideCursor(void);
	private: void UpdateCursorPosition(void);
	private: void ShowArrowCursor(void);
	private: void ShowClenchedCursor(void);
	private: void ShowUnclenchedCursor(void);
	private: void MoveCamera(void);
	public: float GetCameraMoveSpeed(void);
	public: void SetCameraMoveSpeed(float _speed);
	public: float GetCameraNearScale(void)const;
	public: float GetCameraFarScale(void)const;
	public: float GetCameraZoomSpeed(void)const;
	public: static Camera3D* _GetCamera(void);
	public: void VectorLerpFloatLerpIsFinish(void);
	public: void VectorLerpFloatLerpIsStart(void);
	public: GraphicsPointEx* GetVectorLerpCurrentVertex(void);
	public: void UpdateVectorLerpCurve(void);
	public: VectorLerp* GetVectorLerp(void)const;
	public: SimpleList<Vector3>& GetVectorLerpCurve(void);
	public: void VectorLerpSaveCurveAsList(StringANSI _name);
	public: void VectorLerpSaveCurveAsObject(StringANSI _name);
	public: void SavePoints(StringANSI _name);
	public: void LoadVectorLerp(StringANSI _name);
	public: void LoadVectorLerpFloatLerp(StringANSI _name);
	public: void LoadPoints(StringANSI _name);
	private: void VectorLerpFloatLerpCurrent(float _value);
	private: void VectorLerpFloatLerpDelta(float _value);
	private: void VectorLerpFloatLerpT(float _value);
	private: void VectorLerpCurrent(Vector3 _value);
	private: void VectorLerpDelta(Vector3 _value);
	private: void UpdateVectorLerpFloatLerpStatus(void);
	public: static float _GetPointSizeInUnits(void);
	public: GraphicsSurfaceEx* GetEditingObject(void)const;
	public: MagicList<GraphicsPointEx>& GetPoints(void);
	public: MagicList<GraphicsPointEx>& GetSelectedPoints(void);
	public: MagicList<Mesh>& GetShapes(void);
	public: MagicList<GraphicsSurfaceEx>& GetObjects(void);
	public: Camera3D* GetCamera(void)const;
	public: Scene* GetScene(void);
	public: TexturingSurface* GetSprite(void);
	 private: void DrawSelectArea(void);
	 private: void DrawMetricGrid(void);
    private: void DrawGizmoXY(void);
    private: void DrawQuad(void);
    private: void DrawHitboxPoints(void);
    private: void DrawVectorLerpPoints(void);
    private: void DrawHitboxFantomEdge(void);
    private: void DrawVectorLerpFantomEdge(void);
    private: void DrawVectorLerpCurve(void);
};


#endif 
