#include "Camera3D.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Math/Angle/Angle.h> 
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Input/Mouse/Mouse.h>
#include <Engine/Input/Keyboard/Keyboard.h>
#include <Engine/Core/Shape/Plane/Plane.h>
Camera3D* Camera3D::pickedCamera = NIL;
StringANSI Camera3D::camerasSuffix = "camera";
StringANSI Camera3D::camerasDir = "cameras/";



Camera3D::Camera3D(void): pitch(0.0f), yaw(0.0f), roll(0.0f)
{
	AddClassProperty(CAMERA_3D_CLASS);

	projectionMode = PERSPECTIVE_PROJECTION;
	orthographicProjectionAutoEnable = true;
	xLeft = -1.0f;
	xRight = 1.0f;
	yBottom = -1.0f;
	yTop = 1.0f;
	zNear = 0.1f;
	zFar = 10000.0f;
	aspectRatio = 1.0f;
	aspectRatioAutoEnable = true;
	fov = (float)Angle::HALF_PI / 2.0f;
	orthoScale = 1.0f;
	movementType = SPECTATOR_MOVEMENT;
	pitchClampEnable = true;
	pitchMin = (float)-Angle::HALF_PI;
	pitchMax = (float)Angle::HALF_PI;
	yawClampEnable = false;
	yawMin = (float)-Angle::HALF_PI;
	yawMax = (float)Angle::HALF_PI;
	rollClampEnable = false;
	rollMin = (float)-Angle::HALF_PI;
	rollMax = (float)Angle::HALF_PI;
	zoomEnable = false;
	zoomSpeed = 0.3f;
	zoomNear = fov / 5.0f;
	zoomFar = fov * 5.0f;
	rotationMode = FIRST_PERSON_ROTATION;
	rotationSensitivity = 0.2f;
	rotationSmoothingEnable = true;
	rotationHistorySize = 10;
	rotationSmoothingModifier = 0.2f;
	rotationEnable = false;
	reverseScrollXEnable = false;
	reverseScrollYEnable = false;
	viewingPointYAxis = Vector3::UNIT_Y;
	distanceToViewingPoint = zoomNear + (zoomFar - zoomNear) * 0.5f;
}



Camera3D::~Camera3D(void)
{ 
	if(IsPicked()) { Camera3D::_Unpick(); }

	Destroying();
}



Vector3 Camera3D::GetLocalX(void)const
{
	return rotationMatrix.GetX();
}
	

Vector3 Camera3D::GetLocalY(void)const
{
	return rotationMatrix.GetY();
}


Vector3 Camera3D::GetLocalZ(void)const
{
	return rotationMatrix.GetZ();
}


bool Camera3D::IsOrthographicProjectionAutoEnabled(void)const
{
	return orthographicProjectionAutoEnable;
}


void Camera3D::SetOrthographicProjectionAutoEnable(bool _enable)
{
	if(orthographicProjectionAutoEnable = _enable)
	{
		UpdateOrthographicProjection();
	} 
}



void Camera3D::ResizeEventFilter(Vector2 _size)
{
	if(IsAspectRatioAutoEnabled())
	{
		SetAspectRatio(Ratio::_GetRatio(_size.x, _size.y));
	} 
	else
	{  
		UpdateScreenProjection();
	}
}


void Camera3D::Create(Camera3D::Define _define)
{
	SetProjectionMode(_define.projectionMode);
	SetOrthographicProjectionAutoEnable(_define.orthographicProjectionAutoEnable);
	SetXLeft(_define.xLeft);
	SetXRight(_define.xRight);
	SetYBottom(_define.yBottom);
	SetYTop(_define.yTop);
	SetZNear(_define.zNear);  
	SetZFar(_define.zFar); 
	SetAspectRatio(_define.aspectRatio);
	SetAspectRatioAutoEnable(_define.aspectRatioAutoEnable);
	
	SetFov(_define.fov);
	SetOrthoScale(_define.orthoScale);
    
	SetRotation(_define.rotation);

	SetPosition(_define.position);
	SetMovementMode(_define.movementType);
   
	SetPitchClampEnable(_define.pitchClampEnable);
	SetPitchMin(_define.pitchMin); 
	SetPitchMax(_define.pitchMax);
	SetYawClampEnable(_define.yawClampEnable);
	SetYawMin(_define.yawMin);
	SetYawMax(_define.yawMax);
	SetRollClampEnable(_define.rollClampEnable);
	SetRollMin(_define.rollMin);
	SetRollMax(_define.rollMax);
   
	SetZoomEnable(_define.zoomEnable);
	SetZoomSpeed(_define.zoomSpeed); 
	SetZoomNear(_define.zoomNear);
	SetZoomFar(_define.zoomFar);
      
	SetRotationMode(_define.rotationMode);
	SetRotationSensitivity(_define.rotationSensitivity);
	SetRotationSmoothingEnable(_define.rotationSmoothingEnable);
	SetRotationHistorySize(_define.rotationHistorySize); 
	SetRotationSmoothingModifier(_define.rotationSmoothingModifier);
	SetRotationEnable(_define.rotationEnable);
	SetReverseScrollXEnable(_define.reverseScrollXEnable);
	SetReverseScrollYEnable(_define.reverseScrollYEnable);

	distanceToViewingPoint = Numerical<float>::_GetMin(Numerical<float>::_GetMax(_define.distanceToViewingPoint, zoomNear), zoomFar);
	SetViewingPoint(_define.viewingPoint);  
	SetViewingPointYAxis(_define.viewingPointYAxis);
   
	rotationHistory.Clear();

	if(Renderer::_GetPicked())
	{
		Renderer::_GetPicked()->Connect(Window::RESIZE_WINDOW_MESSAGE, this, Caller<>(this, &Camera3D::ResizeEventFilter));

		ResizeEventFilter(Renderer::_GetPicked()->GetViewportSize());
	}
}  
 


float Camera3D::GetOrthoScale(void)const
{
	return orthoScale;
}


void Camera3D::SetOrthoScale(float _scale)
{
	if(_scale != 0.0f)
	{
		orthoScale = _scale;

		UpdateProjectionMatrix();
	}
}



Camera3D::Define Camera3D::GetState(void)
{
	Define define;
	define.projectionMode = projectionMode;
	define.orthographicProjectionAutoEnable = orthographicProjectionAutoEnable;
	define.xLeft = xLeft;
	define.xRight = xRight;
	define.yBottom = yBottom;
	define.yTop = yTop;
	define.zNear = zNear;
	define.zFar = zFar;
    define.aspectRatio = aspectRatio;
    define.aspectRatioAutoEnable = aspectRatioAutoEnable;

	define.fov = fov;
	define.orthoScale = orthoScale;

	define.rotation = rotation; 

	define.position = position; 
	define.movementType = movementType;
	
	define.pitchClampEnable = pitchClampEnable;
	define.pitchMin = pitchMin;
	define.pitchMax = pitchMax;
	define.yawClampEnable = yawClampEnable;
	define.yawMin = yawMin; 
	define.yawMax = yawMax; 
	define.rollClampEnable = rollClampEnable;
	define.rollMin = rollMin; 
	define.rollMax = rollMax; 

	define.zoomEnable = zoomEnable;
	define.zoomSpeed = zoomSpeed;
	define.zoomNear = zoomNear;
	define.zoomFar = zoomFar;
		 
	define.rotationMode = rotationMode;
	define.rotationSensitivity = rotationSensitivity;
	define.rotationSmoothingEnable = rotationSmoothingEnable;
	define.rotationHistorySize = rotationHistorySize;
	define.rotationSmoothingModifier = rotationSmoothingModifier;
	define.rotationEnable = rotationEnable;
	define.reverseScrollXEnable = reverseScrollXEnable;
	define.reverseScrollYEnable = reverseScrollYEnable;

	define.distanceToViewingPoint = distanceToViewingPoint;
	define.viewingPoint = viewingPoint;
	define.viewingPointYAxis = viewingPointYAxis; 
	return define;
}


void Camera3D::ClampRadians180(float& _radians)
{
	while(_radians < -Angle::PI)
	{
		_radians += (float)Angle::DOUBLE_PI;
	}

	while(_radians > (float)Angle::PI)
	{
		_radians -= (float)Angle::DOUBLE_PI;
	}
}


void Camera3D::YawClamp(float& _radians)
{
	if(yawClampEnable)
	{ 
		if(yaw < yawMin)
		{
			_radians = yawMin - (yaw - _radians);
			yaw = yawMin; 
		}
		else if(yaw > yawMax)
		{
			_radians = yawMax - (yaw - _radians);
			yaw = yawMax;
		}
	}
}
 


void Camera3D::SetYawMin(float _radians)
{
	yawMin = _radians;

	ClampRadians180(yawMin);
}


float Camera3D::GetYawMin(void)const
{
	return yawMin;
}


void Camera3D::SetYawMax(float _radians)
{
	yawMax = _radians;

	ClampRadians180(yawMax);
}


float Camera3D::GetYawMax(void)const
{
	return yawMax;
}


bool Camera3D::IsYawClampEnabled(void)const
{
	return yawClampEnable;
}

 

void Camera3D::SetYawClampEnable(bool _enable)
{
	yawClampEnable = _enable;
}


 

bool Camera3D::IsPitchClampEnabled(void)const
{
	return pitchClampEnable;
}


void Camera3D::SetPitchClampEnable(bool _enable)
{
	pitchClampEnable = _enable;
}



void Camera3D::SetPitchMin(float _radians)
{
	pitchMin = _radians;

	ClampRadians180(pitchMin);
}


void Camera3D::SetPitchMax(float _radians)
{
	pitchMax = _radians;

	ClampRadians180(pitchMax);
}


float Camera3D::GetPitchMin(void)const
{
	return pitchMin;
}

float Camera3D::GetPitchMax(void)const
{
	return pitchMax;
}



void Camera3D::PitchClamp(float& _radians)
{
	if(pitchClampEnable)
	{
		if(pitch < pitchMin)
		{
			_radians = pitchMin - (pitch - _radians);
			pitch = pitchMin; 
		}
		else if(pitch > pitchMax)
		{
			_radians = pitchMax - (pitch - _radians);
			pitch = pitchMax;
		}
	}
}



void Camera3D::RollClamp(float& _radians)
{
	if(rollClampEnable)
	{
		if(roll < rollMin)
		{
			_radians = rollMin - (roll - _radians);
			roll = rollMin; 
		}
		else if(roll > rollMax)
		{
			_radians = rollMax - (roll - _radians); 
			roll = rollMax;
		}
	}
}



void Camera3D::SetRollMin(float _radians)
{
	rollMin = _radians;

	ClampRadians180(rollMin);
}


float Camera3D::GetRollMin(void)const
{
	return rollMax;
}



void Camera3D::SetRollMax(float _radians)
{
	rollMax = _radians;

	ClampRadians180(rollMax);
}


float Camera3D::GetRollMax(void)const
{
	return rollMax;
}


bool Camera3D::IsRollClampEnabled(void)const
{
	return rollClampEnable;
}
 

void Camera3D::SetRollClampEnable(bool _enable)
{
	rollClampEnable = _enable;
}



void Camera3D::AccumPitch(float _radians)
{
	if(_radians != 0.0f)
	{
		pitch += _radians;

		ClampRadians180(pitch);
	}	
}

void Camera3D::AccumYaw(float _radians)
{
	if(_radians != 0.0f)
	{
		yaw += _radians;

		ClampRadians180(yaw);
	}	
}


void Camera3D::AccumRoll(float _radians)
{
	if(_radians != 0.0f)
	{
		roll += _radians;

		ClampRadians180(roll);
	}	
}


void Camera3D::KeyboardEventFilter(Keyboard::Event* _event)
{}
 

void Camera3D::MouseEventFilter(Mouse::Event* _event)
{  
	if(_event && Renderer::_GetPicked())
	{
		if(_event->GetEventType() == Mouse::MOVE)
		{
			if(rotationEnable)
			{
				Vector2 distanceFromClientAreaCenter = Mouse::_GetDistanceFrom(Renderer::_GetPicked()->GetClientAreaCenter());
						
				if(rotationSmoothingEnable)
				{
					distanceFromClientAreaCenter = GetSmoothPosition(distanceFromClientAreaCenter);
				} 

				if(rotationMode == FIRST_PERSON_ROTATION)
				{
					if(movementType == OVERLAND_MOVEMENT)
					{
						UpdateTrackballFirstPersonWithOverland(Angle::_DegreesToRadians(distanceFromClientAreaCenter.y * rotationSensitivity),
															   Angle::_DegreesToRadians(distanceFromClientAreaCenter.x * rotationSensitivity),
															   0.0f);
					}   
					if(movementType == SPECTATOR_MOVEMENT)
					{
						UpdateTrackballFirstPersonWithSpectator(Angle::_DegreesToRadians(distanceFromClientAreaCenter.y * rotationSensitivity),
																Angle::_DegreesToRadians(distanceFromClientAreaCenter.x * rotationSensitivity),
									              				0.0f);
					}
					else if(movementType == FLIGHT_MOVEMENT)
					{ 
						UpdateTrackballFirstPersonWithFlight(Angle::_DegreesToRadians(distanceFromClientAreaCenter.y * rotationSensitivity),
															 Angle::_DegreesToRadians(-look.x * 100.0f * Renderer::_GetPicked()->GetUpdateCallController().GetDelta()),
															 Angle::_DegreesToRadians(distanceFromClientAreaCenter.x * rotationSensitivity));
					}
				}
				else if(rotationMode == OBJECT_VIEWER_ROTATION)
				{
					UpdateTrackballObjectViewer(-Angle::_DegreesToRadians(distanceFromClientAreaCenter.y * rotationSensitivity),
												-Angle::_DegreesToRadians(distanceFromClientAreaCenter.x * rotationSensitivity),
												0.0f);
				}
				Mouse::_MoveCursorToCenter(Renderer::_GetPicked());
			} 
			else
			{}
		}
		else if(zoomEnable && _event->GetEventType() == Mouse::WHEEL)
		{
			Zooming((float)_event->GetWheel());
		}
	}
} 



Matrix4x4 Camera3D::GetViewMatrix(void)const
{
	return viewMatrix;
}



void Camera3D::UpdateViewMatrix(void)
{ 
	if(projectionMode == SCREEN_PROJECTION)
	{
		viewMatrix = Matrix4x4::_Translation(GetPosition()); 
	}
	else
	{
		rotationMatrix = GetRotation().ToMatrix3x3();

		look = -rotationMatrix.GetZ();
		viewMatrix.m11 = rotationMatrix.GetX().x;   viewMatrix.m21 = rotationMatrix.GetY().x;   viewMatrix.m31 = rotationMatrix.GetZ().x;
		viewMatrix.m12 = rotationMatrix.GetX().y;   viewMatrix.m22 = rotationMatrix.GetY().y;   viewMatrix.m32 = rotationMatrix.GetZ().y;
		viewMatrix.m13 = rotationMatrix.GetX().z;   viewMatrix.m23 = rotationMatrix.GetY().z;   viewMatrix.m33 = rotationMatrix.GetZ().z;

		if(rotationMode == OBJECT_VIEWER_ROTATION)
		{
		   position = viewingPoint + rotationMatrix.GetZ() * distanceToViewingPoint;
		}
		viewMatrix.m14 = -Vector3::_GetDot(rotationMatrix.GetX(), GetPosition());
		viewMatrix.m24 = -Vector3::_GetDot(rotationMatrix.GetY(), GetPosition());
		viewMatrix.m34 = -Vector3::_GetDot(rotationMatrix.GetZ(), GetPosition());
	}
	UpdateFrustum();
}



void Camera3D::UpdateViewMatrixTranslation(void)
{
	if(projectionMode == SCREEN_PROJECTION)
	{
		viewMatrix = Matrix4x4::_Translation(GetPosition());
	}
	else
	{
		if(rotationMode == OBJECT_VIEWER_ROTATION)
		{
			position = viewingPoint + rotationMatrix.GetZ() * distanceToViewingPoint;
		}
		viewMatrix.m14 = -Vector3::_GetDot(rotationMatrix.GetX(), GetPosition());
		viewMatrix.m24 = -Vector3::_GetDot(rotationMatrix.GetY(), GetPosition());
		viewMatrix.m34 = -Vector3::_GetDot(rotationMatrix.GetZ(), GetPosition());
	}
	UpdateFrustum();
}




void Camera3D::UpdateFrustumRightLeftBottomTopBackFrontPlanes(Matrix4x4& _matrix)
{
	float t;
	frustum[0][0] = _matrix.GetData()[3]  - _matrix.GetData()[0];
	frustum[0][1] = _matrix.GetData()[7]  - _matrix.GetData()[4];
	frustum[0][2] = _matrix.GetData()[11] - _matrix.GetData()[8];
	frustum[0][3] = _matrix.GetData()[15] - _matrix.GetData()[12];
	t = sqrtf(frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2]);
	frustum[0][0] /= t;
	frustum[0][1] /= t;
	frustum[0][2] /= t;
	frustum[0][3] /= t;
	frustum[1][0] = _matrix.GetData()[3]  + _matrix.GetData()[0];
	frustum[1][1] = _matrix.GetData()[7]  + _matrix.GetData()[4];
	frustum[1][2] = _matrix.GetData()[11] + _matrix.GetData()[8];
	frustum[1][3] = _matrix.GetData()[15] + _matrix.GetData()[12];
	t = sqrtf(frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2]);
	frustum[1][0] /= t;
	frustum[1][1] /= t;
	frustum[1][2] /= t;
	frustum[1][3] /= t;
	frustum[2][0] = _matrix.GetData()[3]  + _matrix.GetData()[1];
	frustum[2][1] = _matrix.GetData()[7]  + _matrix.GetData()[5];
	frustum[2][2] = _matrix.GetData()[11] + _matrix.GetData()[9];
	frustum[2][3] = _matrix.GetData()[15] + _matrix.GetData()[13];
	t = sqrtf(frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2]);
	frustum[2][0] /= t;
	frustum[2][1] /= t;
	frustum[2][2] /= t;
	frustum[2][3] /= t;
	frustum[3][0] = _matrix.GetData()[3]  - _matrix.GetData()[1];
	frustum[3][1] = _matrix.GetData()[7]  - _matrix.GetData()[5];
	frustum[3][2] = _matrix.GetData()[11] - _matrix.GetData()[9];
	frustum[3][3] = _matrix.GetData()[15] - _matrix.GetData()[13];
	t = sqrtf(frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2]);
	frustum[3][0] /= t;
	frustum[3][1] /= t;
	frustum[3][2] /= t;
	frustum[3][3] /= t;
	frustum[4][0] = _matrix.GetData()[3]  - _matrix.GetData()[2];
	frustum[4][1] = _matrix.GetData()[7]  - _matrix.GetData()[6];
	frustum[4][2] = _matrix.GetData()[11] - _matrix.GetData()[10];
	frustum[4][3] = _matrix.GetData()[15] - _matrix.GetData()[14];
	t = sqrtf(frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2]);
	frustum[4][0] /= t;
	frustum[4][1] /= t;
	frustum[4][2] /= t;
	frustum[4][3] /= t;
	frustum[5][0] = _matrix.GetData()[3]  + _matrix.GetData()[2];
	frustum[5][1] = _matrix.GetData()[7]  + _matrix.GetData()[6];
	frustum[5][2] = _matrix.GetData()[11] + _matrix.GetData()[10];
	frustum[5][3] = _matrix.GetData()[15] + _matrix.GetData()[14];
	t = sqrtf(frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2]);
	frustum[5][0] /= t;
	frustum[5][1] /= t;
	frustum[5][2] /= t;
	frustum[5][3] /= t;
}


	
void Camera3D::GetRay(Matrix4x4& _matrix, Vector2 _windowPoint, Vector3& _origin, Vector3& _end)	
{
	Vector4 in, out;
	in.x = _windowPoint.x;
	in.y = Renderer::_GetPicked()->GetClientAreaSize().y - _windowPoint.y - 1;
	in.z = -1.0f;
	in.w = 1.0f;
	in.x = (in.x - Renderer::_GetPicked()->GetViewportPosition().x) / Renderer::_GetPicked()->GetViewportSize().x;
	in.y = (in.y - Renderer::_GetPicked()->GetViewportPosition().y) / Renderer::_GetPicked()->GetViewportSize().y;
	in.x = 2.0f * in.x - 1.0f;
	in.y = 2.0f * in.y - 1.0f;
	in.z = 2.0f * in.z - 1.0f;

	out = _matrix * in;
	out.w = 1.0f / out.w;
	out.x *= out.w;
	out.y *= out.w;
	out.z *= out.w;

	_origin.x = out.x;
	_origin.y = out.y;
	_origin.z = out.z;
	in.x = _windowPoint.x;
	in.y = Renderer::_GetPicked()->GetClientAreaSize().y - _windowPoint.y - 1;
	in.z = 1.0f;
	in.w = 1.0f;
	in.x = (in.x - Renderer::_GetPicked()->GetViewportPosition().x) / Renderer::_GetPicked()->GetViewportSize().x;
	in.y = (in.y - Renderer::_GetPicked()->GetViewportPosition().y) / Renderer::_GetPicked()->GetViewportSize().y;
	in.x = 2.0f * in.x - 1.0f;
	in.y = 2.0f * in.y - 1.0f;
	in.z = 2.0f * in.z - 1.0f;

	out = _matrix * in;
	out.w = 1.0f / out.w;
	out.x *= out.w;
	out.y *= out.w;
	out.z *= out.w;

	_end.x = out.x;
	_end.y = out.y;
	_end.z = out.z; 
}
 

void Camera3D::UpdateFrustumPoint1Point2Point3Point4Point5Point6Point7Point8(Matrix4x4& _matrix)
{
	float width = Renderer::_GetPicked()->GetViewportSize().x;
	float height = Renderer::_GetPicked()->GetViewportSize().y;

	GetRay(_matrix, Vector2(0.0f, 0.0f),    nearPoint1, farPoint1);
	GetRay(_matrix, Vector2(width, 0.0f),   nearPoint2, farPoint2);
	GetRay(_matrix, Vector2(width, height), nearPoint3, farPoint3);
	GetRay(_matrix, Vector2(0.0f, height),  nearPoint4, farPoint4);
}


void Camera3D::UpdateFrustum(void)
{
	if(Renderer::_GetPicked())
	{
		Matrix4x4 matrix = GetProjectionMatrix() * GetViewMatrix();

		UpdateFrustumRightLeftBottomTopBackFrontPlanes(matrix);

		matrix.Inverse();

		UpdateFrustumPoint1Point2Point3Point4Point5Point6Point7Point8(matrix);
	}
} 


void Camera3D::GetFrustumPoints(Vector3& _nearPoint1, Vector3& _nearPoint2, Vector3& _nearPoint3, Vector3& _nearPoint4, Vector3& _farPoint1, Vector3& _farPoint2, Vector3& _farPoint3, Vector3& _farPoint4)
{
	_nearPoint1 = nearPoint1;
	_nearPoint2 = nearPoint2;
	_nearPoint3 = nearPoint3;
	_nearPoint4 = nearPoint4;
	
	_farPoint1 = farPoint1;
	_farPoint2 = farPoint2;
	_farPoint3 = farPoint3;
	_farPoint4 = farPoint4;
}
 

bool Camera3D::IsPointInFrustum(Vector3 _point)
{
	for(int32 i = 0; i < 6; i++)
	{
		if(frustum[i][0] * _point.x + frustum[i][1] * _point.y + frustum[i][2] * _point.z + frustum[i][3] < 0.0f)
		{
			return false;
		}
	}
	return true;
}





float Camera3D::IsSphereInFrustum(Sphere _sphere, int32* _visibleInfo)
{
	float d;
	int32 c = 0;
	for(int32 i = 0; i < 6; i++)
	{
		d = frustum[i][0] * _sphere.center.x + frustum[i][1] * _sphere.center.y + frustum[i][2] * _sphere.center.z + frustum[i][3];
		
		if(d <= -_sphere.radius)
		{
			if(_visibleInfo) { *_visibleInfo = INVISIBLE_OBJECT; }
			return 0.0f;
		}
		if(d > _sphere.radius)
		{
			c++;
		}
	}

	if(_visibleInfo) 
	{ 
		if(c == 6) { *_visibleInfo = COMPLETELY_VISIBLE_OBJECT; }
		else { *_visibleInfo = PARTIALLY_VISIBLE_OBJECT; }
	} 

	return d/* - _sphere.radius*/;
}


float Camera3D::IsObjectInFrustum(GraphicsSurface* _object, int32* _visibleInfo, SimpleList<Vector3>* _visibleVertices)
{ 
	if(_visibleInfo) { *_visibleInfo = INVISIBLE_OBJECT; }

	if(Renderer::_GetPicked())
	{
		if(_object && _object->IsShowEnabled() && _object->GetMesh() && _object->GetMesh()->IsExist())
		{
			float dist = IsSphereInFrustum(_object->GetTransformedSphere());

			if(dist > 0.0f)
			{ 
				
				if(_visibleInfo) { *_visibleInfo = PARTIALLY_VISIBLE_OBJECT; }

				if(_visibleInfo == NIL && _visibleVertices == NIL)
				{ 
					return dist; 
				}

				Mesh* mesh = _object->GetMesh();
				int32 amountOfVisibleVertices = 0;
				Vector3 point;
				for(int32 i = 0; i < mesh->GetAmountOfVertices(); i++)
				{
					point = _object->GetTransformation() * mesh->GetVertex(i);
					
					if(IsPointInFrustum(point))
					{
						amountOfVisibleVertices++;
							
						if(_visibleVertices) { _visibleVertices->AddToTail(point); }
					}
				}

				if(_visibleInfo) 
				{ 
					if(mesh->GetAmountOfVertices() == amountOfVisibleVertices)
					{
						*_visibleInfo = COMPLETELY_VISIBLE_OBJECT;
					}
				}
				return dist;
			}
		}
	}
	return 0.0f;
}
		

 
bool Camera3D::IsFrustumInQuad(Quad _quad, Vector3* _point1, Vector3* _point2, Vector3* _point3, Vector3* _point4)
{
	Vector3 p1, p2, p3, p4;
	if(_quad.IsIntersected(Line(nearPoint1, farPoint1), &p1))
	{
		if(_quad.IsIntersected(Line(nearPoint2, farPoint2), &p2))
		{
			if(_quad.IsIntersected(Line(nearPoint3, farPoint3), &p3))
			{
				if(_quad.IsIntersected(Line(nearPoint4, farPoint4), &p4))
				{
					if(_point1) { *_point1 = p1; } 
					if(_point2) { *_point2 = p2; }
					if(_point3) { *_point3 = p3; }
					if(_point4) { *_point4 = p4; }
					return true;
				}
			}
		}
	}
	return false;
}



void Camera3D::SetOrthographicProjection(void)
{
	projectionMode = ORTHOGRAPHIC_PROJECTION;
	
	UpdateOrthographicProjection();
}


void Camera3D::SetScreenProjection(void)
{
	projectionMode = SCREEN_PROJECTION;
	
	UpdateScreenProjection();
}
		


void Camera3D::SetPerspectiveProjection(void)
{
	projectionMode = PERSPECTIVE_PROJECTION;
	
	UpdatePerspective1Projection();
}



float Camera3D::GetFov(void)const
{
	return fov;
}


void Camera3D::SetFov(float _fov)
{
	if(_fov > Angle::PI || _fov < 0.0f) { return; }

	fov = _fov;
	
	UpdatePerspective1Projection();
}




void Camera3D::SetXLeft(float _xLeft)
{
	xLeft = _xLeft;
	
	UpdateProjectionMatrix();
}
	
			

float Camera3D::GetXLeft(void)const
{
	return xLeft;
}


void Camera3D::SetXRight(float _xRight)
{
	xRight = _xRight;
	
	UpdateProjectionMatrix();
}
	
			
float Camera3D::GetXRight(void)const
{
	return xRight;
}

 

float Camera3D::GetZFar(void)const
{
	return zFar;
}

			
void Camera3D::SetZFar(float _zFar)
{
	if(_zFar > 0.0f) 
	{
		zFar = _zFar;
		
		UpdateProjectionMatrix();
	}
}


float Camera3D::GetZNear(void)const
{
	return zNear;
}

			
void Camera3D::SetZNear(float _zNear)
{
	if(_zNear > 0.0f) 
	{
		zNear = _zNear;
		
		UpdateProjectionMatrix();
	}
}


void Camera3D::SetYBottom(float _yBottom)
{
	yBottom = _yBottom;
	
	UpdateProjectionMatrix();
}
	
			
float Camera3D::GetYBottom(void)const
{
	return yBottom;
}


void Camera3D::SetYTop(float _yTop)
{
	yTop = _yTop;
	
	UpdateProjectionMatrix();
}
	
			
float Camera3D::GetYTop(void)const
{
	return yTop;
}



void Camera3D::UpdateProjectionMatrix(void)
{ 
	if(projectionMode == PERSPECTIVE_PROJECTION)
	{
		UpdatePerspective1Projection();
	}
	else if(projectionMode == ORTHOGRAPHIC_PROJECTION)
	{
		UpdateOrthographicProjection();
	}
	else if(projectionMode == SCREEN_PROJECTION)
	{
		UpdateScreenProjection();
	}
} 



void Camera3D::BeginTransform(Renderer::TransformType _type)
{
	if(_type == Renderer::PROJECTION)
	{
		Renderer::_SetCurrentStack(Renderer::PROJECTION);
		Renderer::_PushMatrix();
		Renderer::_LoadMatrix(projectionMatrix);

	}
	else
	{
		Renderer::_SetCurrentStack(Renderer::MODELVIEW);
		Renderer::_PushMatrix();
		Renderer::_LoadMatrix(viewMatrix);
	}
}


void Camera3D::EndTransform(Renderer::TransformType _type)
{
	if(_type == Renderer::PROJECTION)
	{
		Renderer::_SetCurrentStack(Renderer::PROJECTION);
		Renderer::_PopMatrix();
	}
	else
	{
		Renderer::_SetCurrentStack(Renderer::MODELVIEW);
		Renderer::_PopMatrix();
	}
}


void Camera3D::UpdateScreenProjection(void)
{
	if(projectionMode == SCREEN_PROJECTION)
	{ 
		if(Renderer::_GetPicked())
		{
			projectionMatrix.OrthographicProjection(Renderer::_GetPicked()->GetViewportPosition().x, Renderer::_GetPicked()->GetViewportSize().x * distanceToViewingPoint, Renderer::_GetPicked()->GetViewportPosition().y, Renderer::_GetPicked()->GetViewportSize().y * distanceToViewingPoint, zNear, zFar);
			
			UpdateFrustum();
		}
	}
}


void Camera3D::UpdateOrthographicProjection(void)
{ 
	if(projectionMode == ORTHOGRAPHIC_PROJECTION)
	{ 
		if(orthographicProjectionAutoEnable)
		{ 
			projectionMatrix.OrthographicProjection(-aspectRatio * orthoScale, aspectRatio * orthoScale, -1.0f * orthoScale, 1.0f * orthoScale, zNear, zFar);
		}
		else
		{
			projectionMatrix.OrthographicProjection(xLeft, xRight, yBottom, yTop, zNear, zFar);
		}
		UpdateFrustum();
	}
}


void Camera3D::UpdatePerspective1Projection(void)
{
	if(projectionMode == PERSPECTIVE_PROJECTION)
	{ 
		projectionMatrix.Perspective1Projection(fov, aspectRatio, zNear, zFar);

		UpdateFrustum();
	}
}


void Camera3D::SetPerspective1Projection(float _fov, float _aspectRatio, float _zNear, float _zFar)
{
	if(_zNear > 0.0f && _zFar > 0.0f) 
	{
		if(_fov > Angle::PI || _fov < 0.0f) { return; }
		fov = _fov;
		zNear = _zNear;
		zFar = _zFar;
		aspectRatio = _aspectRatio;
		
		projectionMode = PERSPECTIVE_PROJECTION;
		
		UpdatePerspective1Projection();
	}
}




void Camera3D::SetOrthographicProjection(float _xLeft, float _xRight, float _yBottom, float _yTop, float _zNear, float _zFar)
{
	if(_zNear > 0.0f && _zFar > 0.0f) 
	{
		xLeft = _xLeft;
		xRight = _xRight;
		yBottom = _yBottom;
		yTop = _yTop;
		zNear = _zNear;
		zFar = _zFar;

		projectionMode = ORTHOGRAPHIC_PROJECTION;
		
		UpdateOrthographicProjection(); 
	}
}


Matrix4x4 Camera3D::GetProjectionMatrix(void)const
{
	return projectionMatrix;
}


void Camera3D::SetProjectionMode(ProjectionMode _projectionMode)
{
	projectionMode = _projectionMode;

	UpdateViewMatrix(); 
	UpdateProjectionMatrix();
}


Camera3D::ProjectionMode Camera3D::GetProjectionMode(void)const
{
	return projectionMode;
}


Camera3D::MovementMode Camera3D::GetMovementMode(void)const
{
	return movementType;
}


void Camera3D::SetMovementMode(MovementMode _movementType)
{
	movementType = _movementType;

	if(rotationMode == FIRST_PERSON_ROTATION)
	{
		if(movementType == OVERLAND_MOVEMENT || movementType == SPECTATOR_MOVEMENT)
		{
			ResetRoll();
		}
	}
} 


void Camera3D::SetPosition(Vector3 _position)
{  
	if(rotationMode == OBJECT_VIEWER_ROTATION)
	{
		viewingPoint = _position;
	}
	else
	{
		position = _position;
	}

	UpdateViewMatrixTranslation();
}



void Camera3D::DisplacePosition(Vector3 _offset)
{
	SetPosition(position + _offset);
}


Vector3 Camera3D::GetPosition(void)
{
	return position;
}

	
Vector3 Camera3D::GetAbsolutePosition(void)
{
	return position;
}



void Camera3D::MoveTo(Vector3 _direction, float _units)
{
	if(_units != 0.0f) { SetPosition(position + _direction * _units); }
}



void Camera3D::Walk(float _units)
{
	if(_units != 0.0f)
	{
		if(movementType == OVERLAND_MOVEMENT)
		{
			SetPosition(GetPosition() + Vector3(look.x, 0.0f, look.z) * _units);
		}
		else if(movementType == SPECTATOR_MOVEMENT)
		{  
			SetPosition(GetPosition() + look * _units);
		}
		else if(movementType == FLIGHT_MOVEMENT)
		{  
			SetPosition(GetPosition() + look * _units);
		}
	}
}


void Camera3D::Strafe(float _units)
{
	if(_units != 0.0f) 
	{
		if(movementType == OVERLAND_MOVEMENT)
		{ 
			SetPosition(GetPosition() + Vector3(rotationMatrix.GetX().x, 0.0f, rotationMatrix.GetX().z) * _units);
		}
		else if(movementType == SPECTATOR_MOVEMENT)
		{  
			SetPosition(GetPosition() + rotationMatrix.GetX() * _units);
		} 
		if(movementType == FLIGHT_MOVEMENT)
		{ 
			SetPosition(GetPosition() += rotationMatrix.GetX() * _units);
		}
	}
}


void Camera3D::Fly(float _units)
{
	if(_units != 0.0f) 
	{
		if(movementType == FLIGHT_MOVEMENT)
		{ 
			SetPosition(GetPosition() + rotationMatrix.GetY() * _units);
		} 
		else if(movementType == SPECTATOR_MOVEMENT)
		{  
			SetPosition(GetPosition() + rotationMatrix.GetY() * _units);
		}
	}
}


void Camera3D::SetRotation(Quaternion _rotation)
{
	rotation = _rotation;
	
	UpdateViewMatrix();
	pitch = yaw = roll = 0.0f;
}



void Camera3D::DisplaceRotation(Quaternion _rotation)
{
	SetRotation(rotation * _rotation);
}



Quaternion Camera3D::GetRotation(void)
{
	return rotation;
}


Quaternion Camera3D::GetAbsoluteRotation(void)
{
	return rotation;
}



Vector3 Camera3D::GetLook(void)const
{
	return look;
}


Matrix3x3<Vector3> Camera3D::GetRotationMatrix(void)const
{
	return rotationMatrix;
}



void Camera3D::ResetRoll(void)
{ 
	if(rotationMode == FIRST_PERSON_ROTATION)
	{
		LookAt(GetPosition(), GetPosition() + look, Vector3::UNIT_Y);
	}
	else if(rotationMode == OBJECT_VIEWER_ROTATION)
	{
		LookAt(GetPosition(), viewingPoint, Vector3::UNIT_Y); 
	} 
}


void Camera3D::LookAt(Vector3 _target)
{
    LookAt(GetPosition(), _target, rotationMatrix.GetY());
}


void Camera3D::LookAt(Vector3 _from, Vector3 _to, Vector3 _up)
{
	Matrix4x4 matrix(_to, _from, _up);
	
	position = _from;
 
	if(rotationMode == OBJECT_VIEWER_ROTATION)
	{
		viewingPoint = _to;
		distanceToViewingPoint = Line::_GetLength(_from, _to);
	}  

	SetRotation(Quaternion(matrix));
}


bool Camera3D::IsAspectRatioAutoEnabled(void)const
{
	return aspectRatioAutoEnable;
}


void Camera3D::SetAspectRatioAutoEnable(bool _enable)
{
	if(aspectRatioAutoEnable = _enable)
	{
		if(Renderer::_GetPicked())
		{
			aspectRatio = Ratio::_GetRatio(Renderer::_GetPicked()->GetViewportSize().x, Renderer::_GetPicked()->GetViewportSize().y);
			
			UpdateProjectionMatrix();
		}
	}
}



void Camera3D::SetAspectRatio(float _aspectRatio)
{
	aspectRatio = _aspectRatio;
	
	UpdateProjectionMatrix();
}



float Camera3D::GetAspectRatio(void)const
{
	return aspectRatio;
}



void Camera3D::SetRotationSmoothingEnable(bool _enable)
{
	if(!_enable)
	{
		rotationHistory.Clear();
	}

	rotationSmoothingEnable = _enable;
}


bool Camera3D::IsRotationSmoothingEnabled(void)const
{
	return rotationSmoothingEnable;
}


Vector2 Camera3D::GetSmoothPosition(Vector2 _position)
{
	if(rotationHistorySize >= 1)
	{
		while(rotationHistory.GetSize() >= rotationHistorySize) 
		{ 
			rotationHistory.RemoveTail();
		}
	 
		rotationHistory.AddToHead(_position);

		Vector2 smoothingPosition;
		float averageTotal = 0.0f;
		float currentWeight = 1.0f;
		SimpleList<Vector2>::Node* it = rotationHistory.GetHead();
		while(it)
		{
			smoothingPosition += it->key * currentWeight;
			averageTotal += currentWeight;
			currentWeight *= rotationSmoothingModifier;
			it = it->GetRight();
		}
		return smoothingPosition / averageTotal;
	}
	return _position;
}


void Camera3D::SetRotationHistorySize(int32 _size)
{ 
	if(_size >= 0)
	{
		rotationHistorySize = _size;
	}
}


int32 Camera3D::GetRotationHistorySize(void)const
{
	return rotationHistorySize;
}


void Camera3D::SetRotationSmoothingModifier(float _value)
{
	if(_value >= 0.0f && _value <= 1.0f)
	{
		rotationSmoothingModifier = _value;
	}
}

float Camera3D::GetRotationSmoothingModifier(void)const
{
	return rotationSmoothingModifier;
}



void Camera3D::SetRotationSensitivity(float _sensitivity)
{
	rotationSensitivity = _sensitivity;
}



float Camera3D::GetRotationSensitivity(void)const
{
	return rotationSensitivity;
}



void Camera3D::SetRotationMode(RotationMode _rotationMode)
{
	rotationMode = _rotationMode;

	if(rotationMode == OBJECT_VIEWER_ROTATION)
	{
		LookAt(viewingPoint + rotationMatrix.GetZ() * distanceToViewingPoint,
			   viewingPoint,
			   viewingPointYAxis);
	}
	else if(rotationMode == FIRST_PERSON_ROTATION)
	{  
		if(movementType == OVERLAND_MOVEMENT || movementType == SPECTATOR_MOVEMENT)
		{ 
			ResetRoll();
		}
	}
}
			

Camera3D::RotationMode Camera3D::GetRotationMode(void)const
{
	return rotationMode;
}


bool Camera3D::IsRotationEnabled(void)const
{
	return rotationEnable;
}



void Camera3D::SetRotationEnable(bool _enable)
{
	if(!rotationEnable && _enable) { rotationHistory.Clear(); }

	rotationEnable = _enable;
}


bool Camera3D::IsReverseScrollXEnabled(void)const
{
	return reverseScrollXEnable;
}


bool Camera3D::IsReverseScrollYEnabled(void)const
{
	return reverseScrollYEnable;
}


void Camera3D::SetReverseScrollXEnable(bool _enable)
{
	reverseScrollXEnable = _enable;
}
 

void Camera3D::SetReverseScrollYEnable(bool _enable)
{
	reverseScrollYEnable = _enable;
}



void Camera3D::ReverseLook(void)
{
	rotationMatrix.SetZ(-rotationMatrix.GetZ());
	rotationMatrix.SetX(-rotationMatrix.GetX());

	roll = -roll;
	pitch = -pitch;

	rotation = Quaternion(rotationMatrix);
	
	UpdateViewMatrix();
}



void Camera3D::UpdateTrackballObjectViewer(float _pitch, float _yaw, float _roll)
{
	if(rotationEnable)
	{ 
		if(reverseScrollXEnable) { _pitch *= -1; }
		if(reverseScrollYEnable) { _yaw *= -1; }

		AccumPitch(_pitch);
		PitchClamp(_pitch);
		AccumYaw(_yaw);
		YawClamp(_yaw);
		AccumRoll(_roll);
		RollClamp(_roll);
		if(_pitch != 0.0f)
		{
			rotation = GetRotation() * Quaternion(Matrix4x4::_RotationX(_pitch));
			
			UpdateViewMatrix();
		}
		if(_yaw != 0.0f)
		{
			rotation = Quaternion(AxisAngle(viewingPointYAxis, _yaw)) * GetRotation();
			
			UpdateViewMatrix();
		}
		if(_roll != 0.0f)
		{  
			rotation = GetRotation() * Quaternion(Matrix4x4::_RotationZ(_roll));
			
			UpdateViewMatrix();
		}
	}
} 


void Camera3D::UpdateTrackballFirstPersonWithSpectator(float _pitch, float _yaw, float _roll)
{
	if(rotationEnable)
	{ 
		if(reverseScrollXEnable) { _pitch *= -1; }
		if(reverseScrollYEnable) { _yaw *= -1; }

		AccumPitch(_pitch);
		PitchClamp(_pitch);
		AccumYaw(_yaw);
		YawClamp(_yaw);
		AccumRoll(_roll);
		RollClamp(_roll);
		if(_pitch != 0.0f)
		{
			rotation = GetRotation() * Quaternion(Matrix4x4::_RotationX(_pitch));
			
			UpdateViewMatrix();
		}
		if(_yaw != 0.0f)
		{
			rotation = Quaternion(Matrix4x4::_RotationY(_yaw)) * GetRotation();
			
			UpdateViewMatrix();
		}
		if(_roll != 0.0f)
		{ 
			rotation = GetRotation() * Quaternion(Matrix4x4::_RotationZ(_roll));
			
			UpdateViewMatrix();
		}
	}
}

  

void Camera3D::UpdateTrackballFirstPersonWithOverland(float _pitch, float _yaw, float _roll)
{
	if(rotationEnable)
	{ 
		if(reverseScrollXEnable) { _pitch *= -1; }
		if(reverseScrollYEnable) { _yaw *= -1; }

		AccumPitch(_pitch);
		PitchClamp(_pitch);
		AccumYaw(_yaw);
		YawClamp(_yaw);
		AccumRoll(_roll);
		RollClamp(_roll);
		if(_pitch != 0.0f)
		{ 
			rotation = GetRotation() * Quaternion(Matrix4x4::_RotationX(_pitch));
			
			UpdateViewMatrix();
		}
		if(_yaw != 0.0f)
		{ 
			rotation = Quaternion(Matrix4x4::_RotationY(_yaw)) * GetRotation();
			
			UpdateViewMatrix();
		}
		if(_roll != 0.0f)
		{ 
			rotation = GetRotation() * Quaternion(Matrix4x4::_RotationZ(_roll));
			
			UpdateViewMatrix();
		}
	}
}
 

void Camera3D::UpdateTrackballFirstPersonWithFlight(float _pitch, float _yaw, float _roll)
{
	if(rotationEnable)
	{
		if(reverseScrollXEnable) { _pitch *= -1; }
		if(reverseScrollYEnable) { _yaw *= -1; }

		AccumPitch(_pitch);
		AccumYaw(_yaw);
		AccumRoll(_roll);

		if(_pitch != 0.0f || _yaw != 0.0f || _roll != 0.0f)
		{ 
			rotation = GetRotation() *= Quaternion(Euler(_pitch, _yaw, _roll));
			
			UpdateViewMatrix();
		}
	}	
}



void Camera3D::SetViewingPoint(Vector3 _point)
{
	viewingPoint = _point;

	if(rotationMode == OBJECT_VIEWER_ROTATION)
	{ 
		UpdateViewMatrix();
	}
}


Vector3 Camera3D::GetViewingPoint(void)const
{
	return viewingPoint;
}


void Camera3D::SetViewingPointYAxis(Vector3 _axis)
{
	viewingPointYAxis = _axis;
		
	if(rotationMode == OBJECT_VIEWER_ROTATION)
	{ 
		UpdateViewMatrix();
	}
}


Vector3 Camera3D::GetViewingPointYAxis(void)const
{
	return viewingPointYAxis;
}



float Camera3D::GetDistanceToViewingPoint(void)const
{
	return distanceToViewingPoint;
}


void Camera3D::SetDistanceToViewingPoint(float _distance)
{
	if(projectionMode == SCREEN_PROJECTION)
	{ 
		float previousZoom = distanceToViewingPoint;
		 
		distanceToViewingPoint = Numerical<float>::_GetMin(Numerical<float>::_GetMax(_distance, zoomNear), zoomFar);

		UpdateScreenProjection();

		float previousWidth = Renderer::_GetPicked()->GetViewportSize().x * previousZoom;
		float previousHeight = Renderer::_GetPicked()->GetViewportSize().y * previousZoom;

		float newWidth = Renderer::_GetPicked()->GetViewportSize().x * distanceToViewingPoint;
		float newHeight = Renderer::_GetPicked()->GetViewportSize().y * distanceToViewingPoint;

		DisplacePosition(Vector3((newWidth - previousWidth) * 0.5f, (newHeight - previousHeight) * 0.5f, 0.0f));   
	}
	else
	{
		distanceToViewingPoint = Numerical<float>::_GetMin(Numerical<float>::_GetMax(_distance, zoomNear), zoomFar);

		if(rotationMode == OBJECT_VIEWER_ROTATION)
		{ 
			UpdateViewMatrix();
		}
	}
}


void Camera3D::Zooming(float _zoom)
{
	if(projectionMode == SCREEN_PROJECTION)
	{  
		if(Renderer::_GetPicked())
		{
			float previousZoom = distanceToViewingPoint;
			distanceToViewingPoint = Numerical<float>::_GetMin(Numerical<float>::_GetMax(distanceToViewingPoint + (-_zoom * zoomSpeed), zoomNear), zoomFar);
			
			UpdateScreenProjection();

			float previousWidth = Renderer::_GetPicked()->GetViewportSize().x * previousZoom;
			float previousHeight = Renderer::_GetPicked()->GetViewportSize().y * previousZoom;

			float newWidth = Renderer::_GetPicked()->GetViewportSize().x * distanceToViewingPoint;
			float newHeight = Renderer::_GetPicked()->GetViewportSize().y * distanceToViewingPoint;

			DisplacePosition(Vector3((newWidth - previousWidth) * 0.5f, (newHeight - previousHeight) * 0.5f, 0.0f));
		} 
	}
	else
	{
		if(rotationMode == OBJECT_VIEWER_ROTATION)
		{
			distanceToViewingPoint += -_zoom * zoomSpeed;
			distanceToViewingPoint = Numerical<float>::_GetMin(Numerical<float>::_GetMax(distanceToViewingPoint, zoomNear), zoomFar);
			UpdateViewMatrix();
		}
		else if(rotationMode == FIRST_PERSON_ROTATION)
		{
			float newFov = fov + Angle::_DegreesToRadians(-_zoom * zoomSpeed);
			newFov = Numerical<float>::_GetMin(Numerical<float>::_GetMax(newFov, zoomNear), zoomFar);
			SetFov(newFov);
		}
	}
}


bool Camera3D::IsZoomEnabled(void)const
{
	return zoomEnable;
}


void Camera3D::SetZoomEnable(bool _enable)
{
	zoomEnable = _enable;
}


		
float Camera3D::GetZoomSpeed(void)const
{
	return zoomSpeed;
}

		
void Camera3D::SetZoomSpeed(float _zoomSpeed)
{
	zoomSpeed = _zoomSpeed;
}



float Camera3D::GetZoomNear(void)const
{
	return zoomNear;
}



void Camera3D::SetZoomNear(float _zoomNear)
{
	zoomNear = _zoomNear; 
	
	SetDistanceToViewingPoint(distanceToViewingPoint);
}


float Camera3D::GetZoomFar(void)const
{
	return zoomFar;
}
		

		
void Camera3D::SetZoomFar(float _zoomFar)
{
	zoomFar = _zoomFar;
	
	SetDistanceToViewingPoint(distanceToViewingPoint);
}




bool Camera3D::IsPicked(void)const
{
	return (Camera3D::pickedCamera == this);
}


void Camera3D::Pick(void)
{
	if(Camera3D::pickedCamera != this)
	{ 
		Camera3D::_Unpick();

		Camera3D::pickedCamera = this;

		if(IsReceiversExist(PICK_CAMERA_MESSAGE)) { SendPackage(PICK_CAMERA_MESSAGE, this); }
	}
}


void Camera3D::_Unpick(void)
{
	if(Camera3D::pickedCamera)
	{
		Camera3D::pickedCamera = NIL;
	}
}


Camera3D* Camera3D::_GetPicked(void)
{
	return Camera3D::pickedCamera;
}




Line Camera3D::GetSelectRay(Vector2 _windowPoint)
{
	if(Renderer::_GetPicked())
	{
		Line ray;
		Reject(Vector3(_windowPoint.x, _windowPoint.y, -1.0f), ray.origin);
		Reject(Vector3(_windowPoint.x, _windowPoint.y, 1.0f), ray.end);
		
		return ray;
	}
	return Line();
}



Vector3 Camera3D::GetPointUnderPixel(Vector2 _windowPoint)
{
	if(Renderer::_GetPicked())
	{
		if(Renderer::_GetPicked()->IsZBufferEnabled())
		{
			float vz;
			Vector3 worldPoint;

			glReadPixels(_windowPoint.x, _windowPoint.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &vz);

			Reject(Vector3(_windowPoint.x, _windowPoint.y, vz), worldPoint);
			
			return worldPoint;
		}
	}
	return Vector3();
}


void Camera3D::Reject(Vector3 _windowPoint, Vector3& _worldPoint)
{
	if(Renderer::_GetPicked())
	{
		Matrix4x4 matrix;
		Vector4 in, out;
		
		matrix = GetProjectionMatrix() * GetViewMatrix();

		matrix.Inverse();

		in.x = _windowPoint.x;
		in.y = Renderer::_GetPicked()->GetClientAreaSize().y - _windowPoint.y - 1;
		in.z = _windowPoint.z;
		in.w = 1.0f;
		in.x = (in.x - Renderer::_GetPicked()->GetViewportPosition().x) / Renderer::_GetPicked()->GetViewportSize().x;
		in.y = (in.y - Renderer::_GetPicked()->GetViewportPosition().y) / Renderer::_GetPicked()->GetViewportSize().y;
		in.x = 2.0f * in.x - 1.0f;
		in.y = 2.0f * in.y - 1.0f;
		in.z = 2.0f * in.z - 1.0f;

		out = matrix * in;
		out.w = 1.0f / out.w;
		out.x *= out.w;
		out.y *= out.w;
		out.z *= out.w;

		_worldPoint.x = out.x;
		_worldPoint.y = out.y;
		_worldPoint.z = out.z;
	}
}

 
Vector2 Camera3D::Project(Vector3 _worldPoint)
{
	if(Renderer::_GetPicked())
	{
		Vector4 in(_worldPoint.x, _worldPoint.y, _worldPoint.z, 1.0f);

		Vector4 out = GetViewMatrix() * in;
		in = GetProjectionMatrix() * out;
		in.w = 1.0f / in.w;
		in.x *= in.w;
		in.y *= in.w;
		in.z *= in.w;
		in.x = in.x * 0.5f + 0.5f;
		in.y = in.y * 0.5f + 0.5f;
		in.z = in.z * 0.5f + 0.5f;
		in.x = in.x * Renderer::_GetPicked()->GetViewportSize().x + Renderer::_GetPicked()->GetViewportPosition().x;
		in.y = in.y * Renderer::_GetPicked()->GetViewportSize().y + Renderer::_GetPicked()->GetViewportPosition().y;
			
		return Vector2(in.x, in.y);
	}
	return Vector2();
}



StringANSI Camera3D::_GetFileSuffix(void)
{
	return camerasSuffix;
}


void Camera3D::_SetFileSuffix(StringANSI _suffix)
{
	camerasSuffix = _suffix;
}

 

StringANSI Camera3D::_GetDir(void)
{
	return camerasDir;
}


bool Camera3D::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + camerasDir + GetName() + "." + camerasSuffix;
	}

	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true;
}



bool Camera3D::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		GetState().SaveToFile(_file);
		_file.Write(IsPicked());
		return true;
	}
	return false;
}



bool Camera3D::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + Camera3D::_GetDir() + _name + "." + Camera3D::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Camera3D::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		GetState().SaveToFile(_file);
		_file.Write(IsPicked());
		return true;
	}
	return false;
}



bool Camera3D::LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + camerasDir + _path + "." + camerasSuffix;
	}

	File file(_path, File::READ);

	return LoadFromFile(file);
} 


bool Camera3D::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		int32 version;
		_file.Read(version);
		_file.Read(properties);
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		Rename(name);
		Define define;
		define.LoadFromFile(_file);
		Create(define);
		bool isPickedCamera;
		_file.Read(isPickedCamera);
				
		if(isPickedCamera)
		{
			Pick();
		}
		return true;
	}
	return false;
}



Camera3D* Camera3D::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + camerasDir + _path + "." + camerasSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


Camera3D* Camera3D::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		Camera3D* camera = new Camera3D();

		if(camera->LoadFromFile(_file))
		{
			return camera;
		}
		delete camera;
	}
	return NIL;
}
