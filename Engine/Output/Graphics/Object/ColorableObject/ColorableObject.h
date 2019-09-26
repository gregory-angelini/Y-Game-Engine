#ifndef COLORABLEOBJECT_H
#define COLORABLEOBJECT_H
#include <Engine/Core/Random/Random.h>  
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Math/Vector/Vector4/Vector4.h>
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>
class ColorableObject: public virtual AbstractObject
{
	public: enum: int64 
	{
		COLORABLE_OBJECT_CLASS = 1024
	};
	public: enum
	{
		   CHANGE_COLOR_RGB_MESSAGE = 24,
		   CHANGE_COLOR_HSL_MESSAGE = 229,
		   GET_COLOR_RGB_MESSAGE = 78,
		   GET_COLOR_HSL_MESSAGE = 230,
		CHANGE_BACKGROUND_COLOR_MESSAGE = 108,
			CHANGE_ALPHA_MESSAGE = 132,
			CHANGE_RED_MESSAGE = 217,
			CHANGE_GREEN_MESSAGE = 218,
			CHANGE_BLUE_MESSAGE = 219,
			CHANGE_HUE_MESSAGE = 220,
			CHANGE_SATURATION_MESSAGE = 221,
			CHANGE_LIGHTNESS_MESSAGE = 222,
			GET_ALPHA_MESSAGE = 141,
			GET_RED_MESSAGE = 223,
			GET_GREEN_MESSAGE = 224,
			GET_BLUE_MESSAGE = 225,
			GET_HUE_MESSAGE = 226,
			GET_SATURATION_MESSAGE = 227,
			GET_LIGHTNESS_MESSAGE = 228
	};
	public: ColorableObject(void);
	public: virtual ~ColorableObject(void);
	public: virtual void SetColorRGB(Vector3 _rgb) = 0;
	public: virtual void DisplaceColorRGB(Vector3 _offset) = 0;
	public: virtual Vector3 GetColorRGB(void) = 0;
	public: virtual void SetRed(float _red) = 0;
	public: virtual void SetGreen(float _green) = 0;
	public: virtual void SetBlue(float _blue) = 0;
	public: virtual void SetAlpha(float _alpha) = 0;
	public: virtual float GetAlpha(void) = 0;
	public: virtual void SetColorRGBA(Vector4 _rgba) = 0;
	public: virtual Vector4 GetColorRGBA(void) = 0;
	public: virtual void SetColor(RGBA _rgba) = 0;
	public: virtual void SetColorHSL(Vector3 _hsl) = 0;
	public: virtual Vector3 GetColorHSL(void) = 0;
	public: virtual void SetHue(float _hue) = 0;
	public: virtual void SetHue360(float _hue) = 0;
	public: virtual void SetSaturation(float _saturation) = 0;
	public: virtual void SetLightness(float _lightness) = 0;
};






#endif 