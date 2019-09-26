#ifndef RGBA_H
#define RGBA_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Math/Vector/Vector4/Vector4.h>
#define GET_RGBA(_red, _green, _blue, _alpha) ((uint32(_red) << 24) + (uint32(_green) << 16) + (uint32(_blue) << 8) + uint32(_alpha))
#define GET_RED(_color) (((_color) >> 24) & 0xFF)
#define GET_GREEN(_color) (((_color) >> 16) & 0xFF)
#define GET_BLUE(_color) (((_color) >> 8) & 0xFF)
#define GET_ALPHA(_color) ((_color) & 0xFF)
#define SET_RED(_color, _red) (((_color) & 0x00FFFFFF) + (uint32(_red) << 24))
#define SET_GREEN(_color, _green) (((_color) & 0xFF00FFFF) + (uint32(_green) << 16))
#define SET_BLUE(_color, _blue) (((_color) & 0xFFFF00FF) + (uint32(_blue) << 8))
#define SET_ALPHA(_color, _blue) (((_color) & 0xFFFFFF00) + uint32(_alpha))
class RGBA
{
	public: enum
	{
		RED_CHANNEL = 0,
		GREEN_CHANNEL = 1,
		BLUE_CHANNEL = 2,
		ALPHA_CHANNEL = 3
	};
	public: enum
	{ 
		WHITE_COLOR       = GET_RGBA(255,   255,   255,   255),
		BLACK_COLOR       = GET_RGBA(0,     0,     0,     255),
		GRAY_COLOR        = GET_RGBA(255/2, 255/2, 255/2, 255),
		
		RED_COLOR         = GET_RGBA(255,   0,     0,     255),
		GREEN_COLOR       = GET_RGBA(0,     255,   0,     255),
		BLUE_COLOR        = GET_RGBA(0,     0,     255,   255),
		
		YELLOW_COLOR      = GET_RGBA(255,   255,   0,     255),
		CYAN_COLOR        = GET_RGBA(0,     255,   255,   255),
		MAGENTA_COLOR     = GET_RGBA(255,   0,     255,   255),
			
		ROSE_COLOR        = GET_RGBA(255,   255/4, 255/2, 255),
		ORANGE_COLOR      = GET_RGBA(255,   255/2, 255/4, 255),
		VIOLET_COLOR      = GET_RGBA(255/2, 255/4, 255,   255),
		CABBAGE_COLOR     = GET_RGBA(255/2, 255,   255/4, 255),
		SKY_COLOR         = GET_RGBA(255/4, 255/2, 255,   255),
		SPRING_COLOR      = GET_RGBA(255/4, 255,   255/2, 255),

		DARK_RED_COLOR    = GET_RGBA(255/2, 0,     0,     255),
		DARK_GREEN_COLOR  = GET_RGBA(0,     255/2, 0,     255),
		DARK_BLUE_COLOR   = GET_RGBA(0,     0,     255/2, 255),

		INVISIBLE_COLOR   = GET_RGBA(0,     0,     0,     0)
	};
	public: uint8 red;
	public: uint8 green;
	public: uint8 blue;
	public: uint8 alpha;
	public: static const int32 SIZE;
    public: RGBA(void);
	public: explicit RGBA(uint32 _key);
	public: RGBA(float _red, float _green, float _blue, float _alpha);
	public: RGBA(int32 _red, int32 _green, int32 _blue, int32 _alpha);
	public: RGBA(Vector3 _rgb, float _alpha);
	public: RGBA(Vector4 _rgba);
    public: void SetRGBAb(uint8 _red, uint8 _green, uint8 _blue, uint8 _alpha);
	public: inline uint8* GetData(void)
	{
		return &red;
	}
	public: uint32 GetKey(void)const;
	public: void SetRGBAk(uint32 _key);
	public: Vector3 GetRGBf(void)const;
	public: float GetAf(void)const;
	public: Vector4 GetRGBAf(void)const;
    public: void SetRGBAf(float _red, float _green, float _blue, float _alpha);
	public: static Vector3 _HSLToRGB(Vector3 _hsl);
	public: static Vector3 _RGBToHSL(Vector3 _rgb);
	public: uint8& operator [] (int32 _component);
	public: inline const uint8& operator [] (int32 _component)const
	{
		return *(&red + _component);
	}
	public: RGBA operator + (const RGBA& _color)const;
	public: RGBA& operator += (RGBA _color);
	public: RGBA operator - (const RGBA& _color)const;
	public: RGBA& operator -= (RGBA _color);
	public: bool operator == (const RGBA& _color)const;
	public: bool operator != (const RGBA& _color)const;
	public: void Randomize(uint8 _lowerBound = 0, uint8 _upperBound = 255);
	public: static RGBA _GetRandom(uint8 _lowerBound = 0, uint8 _upperBound = 255);
	public: static uint32 _RGBAfToRGBAk(float _red, float _green, float _blue, float _alpha);
	public: static uint8 _FloatToByte(float _intensity);
	public: static float _ByteToFloat(uint8 _intensity);
};


#endif 