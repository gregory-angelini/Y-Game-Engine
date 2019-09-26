#include "RGBA.h"
#include <Engine/Core/Random/Random.h>
#include <Engine/Core/Common/Common.h>



const int32 RGBA::SIZE = 4;



RGBA::RGBA(void)
{
	red = green = blue = alpha = 255;
}


RGBA::RGBA(uint32 _key)
{
	SetRGBAk(_key);
}


RGBA::RGBA(float _red, float _green, float _blue, float _alpha)
{
	SetRGBAf(_red, _green, _blue, _alpha);
}


RGBA::RGBA(int32 _red, int32 _green, int32 _blue, int32 _alpha)
{
	SetRGBAb(_red, _green, _blue, _alpha);
}


RGBA::RGBA(Vector3 _rgb, float _alpha)
{
	red = (uint8)(_rgb.x * 255.0f);
	green = (uint8)(_rgb.y * 255.0f); 
	blue = (uint8)(_rgb.z * 255.0f);
	alpha = (uint8)(_alpha * 255.0f);
}
	
 
RGBA::RGBA(Vector4 _rgba)
{
	red = (uint8)(_rgba.x * 255.0f);
	green = (uint8)(_rgba.y * 255.0f); 
	blue = (uint8)(_rgba.z * 255.0f);
	alpha = (uint8)(_rgba.w * 255.0f);
}
		


Vector3 RGBA::_HSLToRGB(Vector3 _hsl)
{
	Vector3 rgb;

	float chroma = (1.0f - fabs(2.0f * _hsl.z - 1.0f)) * _hsl.y;
	float h1 = _hsl.x / 60.0f;
	float x = chroma * (1.0f - fabs(fmodf(h1, 2.0f) - 1.0f));
	float m = _hsl.z - 0.5f * chroma;

	if((0.0f <= h1) && (h1 < 1.0f)) 
	{
		rgb.x = chroma;
		rgb.y = x;
		rgb.z = 0.0f;
	} 
	else if((1.0f <= h1) && (h1 < 2.0f)) 
	{
		rgb.x = x;
		rgb.y = chroma;
		rgb.z = 0.0f;
	} 
	else if((2.0f <= h1) && (h1 < 3.0f)) 
	{
		rgb.x = 0.0f;
		rgb.y = chroma;
		rgb.z = x;
	} 
	else if((3.0f <= h1) && (h1 < 4.0f)) 
	{
		rgb.x = 0.0f;
		rgb.y = x;
		rgb.z = chroma;
	} 
	else if((4.0f <= h1) && (h1 < 5.0f)) 
	{
		rgb.x = x;
		rgb.y = 0.0f;
		rgb.z = chroma;
	} 
	else if((5.0f <= h1) && (h1 < 6.0f)) 
	{
		rgb.x = chroma;
		rgb.y = 0.0f;
		rgb.z = x;
	} 
	else 
	{
		rgb.x = 0.0f;
		rgb.y = 0.0f;
		rgb.z = 0.0f;
	}

	rgb.x += m; 
	rgb.y += m; 
	rgb.z += m;

	return rgb;
}



Vector3 RGBA::_RGBToHSL(Vector3 _rgb)
{
	Vector3 hsl;

	float max = Numerical<float>::_GetMax(_rgb.x, _rgb.y, _rgb.z);
    float  min = Numerical<float>::_GetMin(_rgb.x, _rgb.y, _rgb.z);
    float c = max - min;
    hsl.z = 0.5f * (max + min);
    
	if(c != 0.0f)
    {
		if(max == _rgb.x)
        {
           hsl.x = fmod(((_rgb.y - _rgb.z) / c), 6.0f);
        }
        else if(max == _rgb.y)
        {
           hsl.x = ((_rgb.z - _rgb.x) / c) + 2.0f;
        }
        else/*if(max == _rgb.z)*/
        {
           hsl.x = ((_rgb.x - _rgb.y) / c) + 4.0f;
        }
		
		hsl.x *= 60.0f;

        hsl.y = c / (1.0f - fabs(2.0f * hsl.z - 1.0f));
    }
    return hsl;
}



void RGBA::Randomize(uint8 _lowerBound, uint8 _upperBound)
{
	red = Random::_RandomInt32(_lowerBound, _upperBound);
	green = Random::_RandomInt32(_lowerBound, _upperBound);
	blue = Random::_RandomInt32(_lowerBound, _upperBound);
}


RGBA RGBA::_GetRandom(uint8 _lowerBound, uint8 _upperBound)
{
	return RGBA(Random::_RandomInt32(_lowerBound, _upperBound), 
				Random::_RandomInt32(_lowerBound, _upperBound),
				Random::_RandomInt32(_lowerBound, _upperBound),
				255);
}



void RGBA::SetRGBAf(float _red, float _green, float _blue, float _alpha)
{
	red = (uint8)(_red * 255.0f);
	green = (uint8)(_green * 255.0f); 
	blue = (uint8)(_blue * 255.0f);
	alpha = (uint8)(_alpha * 255.0f);
}


void RGBA::SetRGBAb(uint8 _red, uint8 _green, uint8 _blue, uint8 _alpha)
{
	red = _red;
	green = _green; 
	blue = _blue;
	alpha = _alpha;
}


void RGBA::SetRGBAk(uint32 _key)
{
	red = GET_RED(_key);
	green = GET_GREEN(_key); 
	blue = GET_BLUE(_key);
	alpha = GET_ALPHA(_key);
}



uint32 RGBA::_RGBAfToRGBAk(float _red, float _green, float _blue, float _alpha)
{
	return GET_RGBA(_red * 255, 
		            _green * 255, 
			    	_blue * 255, 
				    _alpha * 255);
} 


	

Vector3 RGBA::GetRGBf(void)const
{
	return Vector3((float)red / 255, (float)green / 255, (float)blue / 255);
}


float RGBA::GetAf(void)const
{
	return (float)alpha / 255;
}




Vector4 RGBA::GetRGBAf(void)const
{
	return Vector4((float)red / 255, (float)green / 255, (float)blue / 255, (float)alpha / 255);
}


float RGBA::_ByteToFloat(uint8 _intensity)
{
	return (float)_intensity / 255;
}


uint8 RGBA::_FloatToByte(float _intensity)
{
	return (uint8)(_intensity * 255);
}




uint32 RGBA::GetKey(void)const
{
	return GET_RGBA(red, green, blue, alpha);
}


uint8& RGBA::operator [] (int32 _component)
{
	return *(&red + _component);
}



RGBA RGBA::operator + (const RGBA& _color)const
{
	return RGBA(Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(red + _color.red, 0), 255), 
                Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(green + _color.green, 0), 255),
				Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(blue + _color.blue, 0), 255),
				Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(alpha + _color.alpha, 0), 255));
}


RGBA& RGBA::operator += (RGBA _color)
{
	red = Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(red + _color.red, 0), 255);
	green = Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(green + _color.green, 0), 255);
	blue = Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(blue + _color.blue, 0), 255);
	alpha = Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(alpha + _color.alpha, 0), 255);
	return *this;	
}


RGBA RGBA::operator - (const RGBA& _color)const
{
	return RGBA(Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(red - _color.red, 0), 255), 
                Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(green - _color.green, 0), 255),
				Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(blue - _color.blue, 0), 255),
				Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(alpha - _color.alpha, 0), 255));
}


RGBA& RGBA::operator -= (RGBA _color)
{
	red = Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(red - _color.red, 0), 255);
	green = Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(green - _color.green, 0), 255);
	blue = Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(blue - _color.blue, 0), 255);
	alpha = Numerical<uint8>::_GetMin(Numerical<uint8>::_GetMax(alpha - _color.alpha, 0), 255);
	return *this;	
}



bool RGBA::operator == (const RGBA& _color)const
{
	if(red == _color.red &&
	   green == _color.green &&
	   blue == _color.blue &&
	   alpha == _color.alpha)
	{
		return true;
	}
	return false;
}


bool RGBA::operator != (const RGBA& _color)const
{
	return !(*this == _color);
}

