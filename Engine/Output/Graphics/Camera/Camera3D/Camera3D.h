#ifndef CAMERA3D_H
#define CAMERA3D_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Matrix/Matrix4x4/Matrix4x4.h>
#include <Engine/Math/Quaternion/Quaternion.h>
#include <Engine/Core/Object/TranslatableObject/TranslatableObject.h>
#include <Engine/Core/Object/RotatableObject/RotatableObject.h>
#include <Engine/Core/Shape/Point/Point.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/Shape/Line/Line.h>
#include <Engine/Math/Matrix/Matrix3x3/Matrix3x3.h>
#include <Engine/Output/Graphics/Renderer/Renderer.h>
class Camera3D: public virtual TranslatableObject,
	            public virtual RotatableObject
{
	public: enum: int64
	{
		CAMERA_3D_CLASS = 32
	};
	public: enum
	{
		PICK_CAMERA_MESSAGE = 11,
		CHANGE_PROJECTION_MODE_MESSAGE = 82,
		GET_SELECT_RAY_MESSAGE = 115,
		CHANGE_VIEWING_POINT_MESSAGE = 197,
		CHANGE_DISTANCE_TO_VIEWING_POINT_MESSAGE = 198,
		CAMERA_LOOK_AT_MESSAGE = 199,
		CHECK_FRUSTUM_IN_QUAD_MESSAGE = 201,
		CHECK_POINT_IN_FRUSTUM_MESSAGE = 202,
		CHECK_OBJECT_IN_FRUSTUM_MESSAGE = 203,
		GET_VIEWING_POINT_MESSAGE = 181,
		CHANGE_ROTATION_MODE_MESSAGE = 297,
		CHANGE_MOVEMENT_MODE_MESSAGE = 298,
		CHANGE_ZOOM_FAR_MESSAGE = 299,
		CHANGE_ZOOM_NEAR_MESSAGE = 300,
		CHANGE_ZOOM_SPEED_MESSAGE = 301,
		ENABLE_ZOOM_MESSAGE = 302,
		CREATE_CAMERA_MESSAGE = 304,
		GET_CAMERA_LOOK_MESSAGE = 310,
		ENABLE_ROTATION_MESSAGE = 322,
		ENABLE_ROTATION_RANGE_MESSAGE = 323,
		CHANGE_ROTATION_RANGE_MESSAGE = 324
	};
	public: enum MovementMode
	{
		FLIGHT_MOVEMENT,
		SPECTATOR_MOVEMENT,
		OVERLAND_MOVEMENT
	};
	public: enum ProjectionMode
	{
		PERSPECTIVE_PROJECTION,
		ORTHOGRAPHIC_PROJECTION,
		SCREEN_PROJECTION
	};
	public: enum RotationMode
	{
	    FIRST_PERSON_ROTATION,
		OBJECT_VIEWER_ROTATION,
	    THIRD_PERSON_ROTATION,
	    SECOND_PERSON_ROTATION
	};
	public: enum
	{
		INVISIBLE_OBJECT,
		COMPLETELY_VISIBLE_OBJECT,
		PARTIALLY_VISIBLE_OBJECT
	};
	public: enum ActionType
	{
		MOVE_FORWARD = 0,
		MOVE_BACKWARD = 1,
		MOVE_RIGHT = 2,
		MOVE_LEFT = 3,
		MOVE_UP = 4,
		MOVE_DOWN = 5
	};
	public: class Define
	{
		public: ProjectionMode projectionMode;
		public: bool orthographicProjectionAutoEnable;
		public: float xLeft;
		public: float xRight;
		public: float yBottom;
		public: float yTop;
		public: float zNear;
	    public: float zFar;
        public: float aspectRatio;
		public: bool aspectRatioAutoEnable;
		public: float fov;
		public: float orthoScale;
		public: MovementMode movementType;
	    public: Vector3 position;
	    public: bool pitchClampEnable;
		public: float pitchMin;
	    public: float pitchMax;
		public: bool yawClampEnable;
		public: float yawMin;
		public: float yawMax;
		public: bool rollClampEnable;
		public: float rollMin;
		public: float rollMax;
	    public: bool zoomEnable;
		public: float zoomSpeed;
		public: float zoomNear;
		public: float zoomFar;
		public: Quaternion rotation;
		public: RotationMode rotationMode;
		public: float rotationSensitivity;
	    public: bool rotationSmoothingEnable;
	    public: int32 rotationHistorySize;
	    public: float rotationSmoothingModifier;
		public: bool rotationEnable;
		public: bool reverseScrollXEnable;
		public: bool reverseScrollYEnable;
	    public: float distanceToViewingPoint;
		public: Vector3 viewingPoint;
		public: Vector3 viewingPointYAxis;
		public: Define(void);
		public: void SetMiddleDistanceToViewingPoint(void);
		public: bool SaveToFile(File& _file);
		public: bool LoadFromFile(File& _file); 
	};
	friend class GraphicsScene;
	friend class Engine;
	private: ProjectionMode projectionMode;
	private: bool orthographicProjectionAutoEnable;
	private: float xLeft;
	private: float xRight;
	private: float yBottom;
	private: float yTop;
	private: float zNear;
	private: float zFar;
    private: float aspectRatio;
    private: bool aspectRatioAutoEnable;
    private: float fov;
	private: float orthoScale;
	private: Vector3 position;
	private: MovementMode movementType;
	private: bool pitchClampEnable;
	private: float pitchMin;
	private: float pitchMax;
	private: bool yawClampEnable;
	private: float yawMin;
	private: float yawMax;
	private: bool rollClampEnable;
	private: float rollMin;
	private: float rollMax;
	private: bool zoomEnable;
	private: float zoomSpeed;
	private: float zoomNear;
	private: float zoomFar;
	private: Quaternion rotation;
	private: RotationMode rotationMode;
	private: float rotationSensitivity;
	private: bool rotationSmoothingEnable;
	private: int32 rotationHistorySize;
	private: float rotationSmoothingModifier;
	private: bool rotationEnable;
	private: bool reverseScrollXEnable;
	private: bool reverseScrollYEnable;
	private: float distanceToViewingPoint;
    private: Vector3 viewingPoint;
	private: Vector3 viewingPointYAxis;
	private: Vector3 look;
	private: float pitch, yaw, roll;
    private: SimpleList<Vector2> rotationHistory;
    private: Vector3 nearPoint1, nearPoint2, nearPoint3, nearPoint4;
    private: Vector3 farPoint1, farPoint2, farPoint3, farPoint4;
    private: float frustum[6][4];
	private: Matrix4x4 viewMatrix;
	private: Matrix3x3<Vector3> rotationMatrix;
	private: Matrix4x4 projectionMatrix;
    private: static Camera3D* pickedCamera;
	private: static StringANSI camerasSuffix;
    private: static StringANSI camerasDir;
	public: Camera3D(void);
	public: virtual ~Camera3D(void);
    public: void Create(Camera3D::Define _define);
	public: Camera3D::Define GetState(void);
	public: void BeginTransform(Renderer::TransformType _type);
	public: void EndTransform(Renderer::TransformType _type);
	public: void Pick(void);
	public: bool IsPicked(void)const;
	public: static void _Unpick(void);
    public: static Camera3D* _GetPicked(void);
	private: void ClampRadians180(float& _radians);
	private: void AccumPitch(float _radians);
	private: void PitchClamp(float& _radians);
	public: void SetPitchMin(float _radians);
	public: float GetPitchMin(void)const;
	public: void SetPitchMax(float _radians);
	public: float GetPitchMax(void)const;
 	public: bool IsPitchClampEnabled(void)const;
 	public: void SetPitchClampEnable(bool _enable);
	private: void AccumYaw(float _radians);
	private: void YawClamp(float& _radians);
	public: void SetYawMin(float _radians);
	public: float GetYawMin(void)const;
	public: void SetYawMax(float _radians);
	public: float GetYawMax(void)const;
 	public: bool IsYawClampEnabled(void)const;
 	public: void SetYawClampEnable(bool _enable);
	private: void AccumRoll(float _radians);
	private: void RollClamp(float& _radians);
	public: void SetRollMin(float _radians);
	public: float GetRollMin(void)const;
	public: void SetRollMax(float _radians);
	public: float GetRollMax(void)const;
 	public: bool IsRollClampEnabled(void)const;
 	public: void SetRollClampEnable(bool _enable);
	public: void ResetRoll(void);
	public: virtual void SetRotation(Quaternion _rotation);
	public: virtual void DisplaceRotation(Quaternion _rotation);
	public: virtual Quaternion GetRotation(void);
	public: virtual Quaternion GetAbsoluteRotation(void);
	public: Vector3 GetLook(void)const;
	public: Matrix3x3<Vector3> GetRotationMatrix(void)const;
	public: Vector3 GetLocalX(void)const;
	public: Vector3 GetLocalY(void)const;
	public: Vector3 GetLocalZ(void)const;
	public: void LookAt(Vector3 _from, Vector3 _to, Vector3 _up);
	public: void LookAt(Vector3 _target);
	public: void ReverseLook(void);
	public: void SetReverseScrollXEnable(bool _enable);
	public: bool IsReverseScrollXEnabled(void)const;
	public: void SetReverseScrollYEnable(bool _enable);
	public: bool IsReverseScrollYEnabled(void)const;
	public: virtual void MouseEventFilter(Mouse::Event* _event);
	public: virtual void KeyboardEventFilter(Keyboard::Event* _event);
	private: void ResizeEventFilter(Vector2 _size);
    public: Matrix4x4 GetViewMatrix(void)const;
    private: void UpdateViewMatrix(void);
    private: void UpdateViewMatrixTranslation(void);
	public: void SetFov(float _fov);
	public: float GetFov(void)const;
	public: void SetAspectRatio(float _aspectRatio);
    public: float GetAspectRatio(void)const;
	public: bool IsAspectRatioAutoEnabled(void)const;
	public: void SetAspectRatioAutoEnable(bool _enable);
    public: void SetZFar(float _zFar);
	public: float GetZFar(void)const;
    public: void SetZNear(float _zNear);
	public: float GetZNear(void)const;
    public: void SetXLeft(float _xLeft);
	public: float GetXLeft(void)const;
    public: void SetXRight(float _xRight);
	public: float GetXRight(void)const;
    public: void SetYBottom(float _yBottom);
	public: float GetYBottom(void)const;
    public: void SetYTop(float _yTop);
	public: float GetYTop(void)const;
	private: void UpdateProjectionMatrix(void);
	private: void UpdatePerspective1Projection(void);
	private: void UpdateOrthographicProjection(void);
	private: void UpdateScreenProjection(void);
	public: void SetPerspective1Projection(float _fov, float _aspectRatio, float _zNear, float _zFar);
	public: bool IsOrthographicProjectionAutoEnabled(void)const;
	public: void SetOrthographicProjectionAutoEnable(bool _enable);
	public: void SetOrthographicProjection(float _xLeft, float _xRight, float _yBottom, float _yTop, float _zNear, float _zFar);
	public: float GetOrthoScale(void)const;
	public: void SetOrthoScale(float _scale);
	public: Matrix4x4 GetProjectionMatrix(void)const;
	public: void SetProjectionMode(ProjectionMode _projectionMode);
    public: ProjectionMode GetProjectionMode(void)const;
	public: void SetOrthographicProjection(void);
	public: void SetScreenProjection(void);
    public: void SetPerspectiveProjection(void);
	public: void SetMovementMode(MovementMode _movementType);
	public: MovementMode GetMovementMode(void)const;
	public: virtual void SetPosition(Vector3 _position);
	public: virtual void DisplacePosition(Vector3 _offset);
	public: virtual void MoveTo(Vector3 _direction, float _units);
	public: virtual Vector3 GetPosition(void);
	public: virtual Vector3 GetAbsolutePosition(void);
    public: void Strafe(float _units);
    public: void Fly(float _units);
    public: void Walk(float _units);
	public: void SetRotationSmoothingEnable(bool _enable);
	public: bool IsRotationSmoothingEnabled(void)const;
	private: Vector2 GetSmoothPosition(Vector2 _position);
	public: void SetRotationHistorySize(int32 _size);
	public: int32 GetRotationHistorySize(void)const;
    public: void SetRotationSmoothingModifier(float _value);
    public: float GetRotationSmoothingModifier(void)const;
	public: void SetRotationSensitivity(float _sensitivity);
	public: float GetRotationSensitivity(void)const;
	public: void SetRotationMode(RotationMode _rotationMode);
    public: RotationMode GetRotationMode(void)const;
	public: bool IsRotationEnabled(void)const;
	public: void SetRotationEnable(bool _enable);
	public: void UpdateTrackballFirstPersonWithOverland(float _pitch, float _yaw, float _roll);
	public: void UpdateTrackballFirstPersonWithFlight(float _pitch, float _yaw, float _roll);
	public: void UpdateTrackballFirstPersonWithSpectator(float _pitch, float _yaw, float _roll);
    public: void UpdateTrackballObjectViewer(float _pitch, float _yaw, float _roll);
	public: void SetViewingPoint(Vector3 _point);
	public: Vector3 GetViewingPoint(void)const;
	public: void SetViewingPointYAxis(Vector3 _axis);
	public: Vector3 GetViewingPointYAxis(void)const;
	public: void SetDistanceToViewingPoint(float _distance);
	public: float GetDistanceToViewingPoint(void)const;
	public: void Zooming(float _zoom);
	public: void SetZoomEnable(bool _enable);
	public: bool IsZoomEnabled(void)const;
    public: float GetZoomSpeed(void)const;
    public: void SetZoomSpeed(float _zoomSpeed);
    public: void SetZoomNear(float _zoomNear);
    public: float GetZoomNear(void)const;
    public: void SetZoomFar(float _zoomFar);
    public: float GetZoomFar(void)const;
	public: void Reject(Vector3 _windowPoint, Vector3& _worldPoint);
	public: Vector2 Project(Vector3 _worldPoint);
	public: Line GetSelectRay(Vector2 _windowPoint);
	public: Vector3 GetPointUnderPixel(Vector2 _windowPoint);
    public: bool IsFrustumInQuad(Quad _quad, Vector3* _point1 = NIL, Vector3* _point2 = NIL, Vector3* _point3 = NIL, Vector3* _point4 = NIL);
	public: float IsObjectInFrustum(GraphicsSurface* _object, int32* _visibleInfo = NIL, SimpleList<Vector3>* _visibleVertices = NIL);
	public: bool IsPointInFrustum(Vector3 _point);
	public: float IsSphereInFrustum(Sphere _sphere, int32* _visibleInfo = NIL);
	public: void GetFrustumPoints(Vector3& _nearPoint1, Vector3& _nearPoint2, Vector3& _nearPoint3, Vector3& _nearPoint4, Vector3& _farPoint1, Vector3& _farPoint2, Vector3& _farPoint3, Vector3& _farPoint4);
    private: void UpdateFrustum(void);
	private: void UpdateFrustumPoint1Point2Point3Point4Point5Point6Point7Point8(Matrix4x4& _matrix);
	private: void UpdateFrustumRightLeftBottomTopBackFrontPlanes(Matrix4x4& _matrix);
	private: void GetRay(Matrix4x4& _matrix, Vector2 _windowPoint, Vector3& _origin, Vector3& _end);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static Camera3D* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static Camera3D* _LoadFromFile(File& _file);
};




#endif 