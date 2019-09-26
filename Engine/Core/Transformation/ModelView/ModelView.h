#ifndef MODELVIEW_H
#define MODELVIEW_H
#include <Engine/Core/Object/TranslatableObject/TranslatableObject.h>
#include <Engine/Core/Object/RotatableObject/RotatableObject.h>
#include <Engine/Core/Object/ScalableObject/ScalableObject.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Matrix/Matrix4x4/Matrix4x4.h>
class ModelView: public virtual TranslatableObject,
			     public virtual RotatableObject,
			     public virtual ScalableObject
{
	friend class GraphicsObject;
	friend class RenderList;
	friend class GraphicsScene;
	friend class GraphicsSurface;
	friend class PolygonalSurface;
	private: Vector3 position;
	private: Vector3 scale;
	private: Quaternion rotation;
	private: Matrix4x4 transformation;
	private: bool transformationValid;
	public: ModelView(void);
	public: virtual ~ModelView(void);
    public: ModelView& operator = (const ModelView& _object);
    public: virtual Matrix4x4 GetTransformation(void);
    public: void SetTransformation(Vector3 _scale, Quaternion _rotation, Vector3 _position);
	protected: virtual void ResetTransformationValid(void);
	protected: virtual void SetTransformationValid(void);
	public: virtual void SetPosition(Vector3 _position);
	public: virtual void DisplacePosition(Vector3 _offset);
	public: virtual void MoveTo(Vector3 _direction, float _units);
	public: virtual Vector3 GetPosition(void);
	public: virtual Vector3 GetAbsolutePosition(void);
	public: virtual void SetRotation(Quaternion _rotation);
	public: virtual void DisplaceRotation(Quaternion _rotation);
	public: virtual Quaternion GetRotation(void);
	public: virtual Quaternion GetAbsoluteRotation(void);
	public: virtual void SetScale(Vector3 _scale);
	public: virtual void DisplaceScale(Vector3 _scale);
	public: virtual Vector3 GetScale(void);
	public: virtual Vector3 GetAbsoluteScale(void);
	public: bool IsScaled(void);
	public: bool IsSymmetricalScaled(void);
};


#endif 