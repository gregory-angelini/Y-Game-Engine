#ifndef PIXELIMAGE_H
#define PIXELIMAGE_H
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Container/vector/vector.h>
#include <Engine/Output/File/File.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Core/Bitset/Bitset.h>
class PixelImage
{
	private: vector<RGBA> pixels;
	private: Vector2 size;
	public: PixelImage(void);
	public: PixelImage(Vector2 _size, RGBA _color);
	public: PixelImage(StringANSI _path);
	public: PixelImage(File& _file);
	public: virtual ~PixelImage(void);
	public: bool Resize(Vector2 _size, RGBA _color);
	public: void Destroy(void);
	public: RGBA& GetPixel(Vector2 _position);
	public: bool GetPixel(Vector2 _position, RGBA& _color);
	public: RGBA& GetPixel(int32 _index);
	public: void Fill(RGBA _color);
	public: bool SetImage(Vector2 _position, PixelImage& _image);
	public: const RGBA& GetPixel(Vector2 _position)const;
	public: const RGBA& GetPixel(int32 _index)const;
	public: bool IsExist(void)const;
	public: inline RGBA* GetData(void)
	{
		return &pixels[0];
	}
	public: Vector2 GetSize(void)const;
	public: float GetWidth(void)const;
	public: float GetHeight(void)const;
	public: bool LoadImageFromPNGFile(StringANSI _path);
	public: bool LoadImageFromPNGFile(File& _file);
	public: bool SaveImageToPNGFile(StringANSI _path)const;
	public: bool SaveImageToPNGFile(File& _file)const;
};

#endif 