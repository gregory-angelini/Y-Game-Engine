#ifndef VECTOR3VARIABLE_H
#define VECTOR3VARIABLE_H
#include <Engine/Core/Parser/Variable/Variable.h>
template<>
class Variable<Vector3>: public AbstractVariable,
	                     public TranslatableObject,
						 public ScalableObject,
						 public ColorableObject
{
	public: enum: int64 
	{
		VECTOR_3_VARIABLE_CLASS = 4294967296
	};
	public: enum
	{
		GET_VECTOR_3_COMPONENT_MESSAGE = 204,
		CHANGE_VECTOR_3_COMPONENT_MESSAGE = 205,
		GET_NORMALIZED_VECTOR_3_MESSAGE = 264,
		GET_VECTOR_3_ANGLE_MESSAGE = 270,
		GET_VECTOR_3_LENGTH_MESSAGE = 271,
		VECTOR_3_TO_SPHERICAL_MESSAGE = 311
	};
	friend class AbstractVariable;
	private: Vector3 value;
	public: Variable(void);
	public: Variable(Vector3 _value);
	public: virtual ~Variable(void);
	public: virtual int32 GetValueType(void)const;
	public: Vector3 GetValue(void)const;
	public: virtual void SetValue(Vector3 _value);
	public: virtual void SetPosition(Vector3 _position);
	public: virtual void DisplacePosition(Vector3 _offset);
	public: virtual void MoveTo(Vector3 _direction, float _units);
	public: virtual Vector3 GetPosition(void);
	public: virtual Vector3 GetAbsolutePosition(void);
	public: virtual void SetScale(Vector3 _scale);
	public: virtual void DisplaceScale(Vector3 _scale);
	public: virtual Vector3 GetScale(void);
	public: virtual Vector3 GetAbsoluteScale(void);
	public: virtual void SetColorRGB(Vector3 _rgb);
	public: virtual void DisplaceColorRGB(Vector3 _offset);
	public: virtual Vector3 GetColorRGB(void);
	public: virtual void SetRed(float _red);
	public: virtual void SetGreen(float _green);
	public: virtual void SetBlue(float _blue);
	public: virtual void SetAlpha(float _alpha);
	public: virtual float GetAlpha(void);
	public: virtual void SetColorRGBA(Vector4 _rgba);
	public: virtual Vector4 GetColorRGBA(void);
	public: virtual void SetColor(RGBA _color);
	public: virtual void SetColorHSL(Vector3 _hsl);
	public: virtual Vector3 GetColorHSL(void);
	public: virtual void SetHue(float _hue);
	public: virtual void SetHue360(float _hue);
	public: virtual void SetSaturation(float _saturation);
	public: virtual void SetLightness(float _lightness);
	public: bool operator == (const Variable& _node);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void LoadDerivedFromFile(File& _file);
};

#endif 