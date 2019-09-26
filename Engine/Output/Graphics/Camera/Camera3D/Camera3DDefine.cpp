#include "Camera3D.h"
#include <Engine/Math/Angle/Angle.h>



Camera3D::Define::Define(void)
{
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



void Camera3D::Define::SetMiddleDistanceToViewingPoint(void)
{
	distanceToViewingPoint = zoomNear + (zoomFar - zoomNear) * 0.5f;
}




bool Camera3D::Define::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(projectionMode);
		_file.Write(orthographicProjectionAutoEnable);
		_file.Write(xLeft);
		_file.Write(xRight);
		_file.Write(yBottom);
		_file.Write(yTop);
		_file.Write(zNear);
		_file.Write(zFar);
		_file.Write(aspectRatio);
		_file.Write(aspectRatioAutoEnable);
		_file.Write(fov);
		_file.Write(orthoScale);
		_file.Write(movementType);
		_file.Write(position);
		_file.Write(pitchClampEnable);
		_file.Write(pitchMin);
		_file.Write(pitchMax);
		_file.Write(yawClampEnable);
		_file.Write(yawMin);
		_file.Write(yawMax);
		_file.Write(rollClampEnable);
		_file.Write(rollMin);
		_file.Write(rollMax);
		_file.Write(zoomEnable);
		_file.Write(zoomSpeed);
		_file.Write(zoomNear);
		_file.Write(zoomFar);
		_file.Write(rotation);
		_file.Write(rotationMode);
		_file.Write(rotationSensitivity);
		_file.Write(rotationSmoothingEnable);
		_file.Write(rotationHistorySize);
		_file.Write(rotationSmoothingModifier);
		_file.Write(rotationEnable);
		_file.Write(reverseScrollXEnable);
		_file.Write(reverseScrollYEnable);

		_file.Write(viewingPoint);
		_file.Write(viewingPointYAxis);
		_file.Write(distanceToViewingPoint);
		return true;
	}
	return false;
}

bool Camera3D::Define::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		_file.Read(projectionMode);
		_file.Read(orthographicProjectionAutoEnable);
		_file.Read(xLeft);
		_file.Read(xRight);
		_file.Read(yBottom);
		_file.Read(yTop);
		_file.Read(zNear);
		_file.Read(zFar);
		_file.Read(aspectRatio);
		_file.Read(aspectRatioAutoEnable);
		_file.Read(fov);
		_file.Read(orthoScale);
		_file.Read(movementType);
		_file.Read(position);
		_file.Read(pitchClampEnable);
		_file.Read(pitchMin);
		_file.Read(pitchMax);
		_file.Read(yawClampEnable);
		_file.Read(yawMin);
		_file.Read(yawMax);
		_file.Read(rollClampEnable);
		_file.Read(rollMin);
		_file.Read(rollMax);
		_file.Read(zoomEnable);
		_file.Read(zoomSpeed);
		_file.Read(zoomNear);
		_file.Read(zoomFar);
		_file.Read(rotation);
		_file.Read(rotationMode);
		_file.Read(rotationSensitivity);
		_file.Read(rotationSmoothingEnable);
		_file.Read(rotationHistorySize);
		_file.Read(rotationSmoothingModifier);
		_file.Read(rotationEnable);
		_file.Read(reverseScrollXEnable);
		_file.Read(reverseScrollYEnable);

		_file.Read(viewingPoint);
		_file.Read(viewingPointYAxis);
		_file.Read(distanceToViewingPoint);
		return true;
	}
	return false;
}