#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Output/Graphics/Image/PixelImage/PixelImage.h>
#include <Engine/Core/Surface/PolygonalSurface/Hitbox/Hitbox.h>
class PixelBuffer;
class Texture2D: public AbstractObject
{
	private: enum
	{
		DESTROY_TEXTURE_MESSAGE = 1,
		INIT_TEXTURE_MESSAGE = 2,
		CHANGE_RENDER_TARGET_ROLE_MESSAGE = 86
	};
	public: enum
	{
		GET_TEXTURE_SIZE_MESSAGE = 68,
		CREATE_BOX_TEXTURE_MAP_MESSAGE = 281,
		CREATE_QUAD_TEXTURE_MAP_MESSAGE = 282,
		CHANGE_BOX_TEXTURE_MAP_MESSAGE = 283,
		CHANGE_QUAD_TEXTURE_MAP_MESSAGE = 284,
		CREATE_TEXTURE_MESSAGE = 291
	};
	public: enum: int64 
	{
		TEXTURE_2D_CLASS = 34359738368
	};
	public: enum 
	{
		POINT_FILTERING,
		BILINEAR_FILTERING,
		TRILINEAR_FILTERING,
		ANISOTROPY_FILTERING
	};
	public: enum
	{
		QUAD_TEXTURE_MAP,
		BOX_TEXTURE_MAP
	};
	public: enum
	{
		BOX_FRONT_FACE,
		BOX_BACK_FACE,
		BOX_LEFT_FACE,
		BOX_RIGHT_FACE,
		BOX_TOP_FACE,
		BOX_BOTTOM_FACE
	};
	friend class GraphicsPoint;
	friend class GraphicsLine;
	friend class TexturingSurface;
	friend class GraphicsScene;
	friend class TiledSpace3D;
	friend class GraphicsContainer;
	private: Vector2 imageSize;
	private: Vector2 textureSize;
	private: uint32 textureId;
	private: bool cachingEnable;
	private: MagicPointer<Hitbox> hitbox;
    private: bool hitboxEnable;
	private: int32 filteringMode;
	private: int32 magFilter;
	private: int32 minFilter;

	private: int32 typeOfTextureMap;
	private: Vector3 boxImageSize;
	private: PixelBuffer* pixelBuffer;
	private: PixelImage pixelImageCache;
    private: static StringANSI texturesSuffix;
    private: static StringANSI texturesDir;
	public: Texture2D(void);
	public: virtual ~Texture2D(void);
	private: void SetRenderTarget(PixelBuffer* _pixelBuffer);
	public: bool IsRenderTarget(void)const;
	public: void BindHitbox(Hitbox* _hitbox);
	public: Hitbox* GetHitbox(void)const;
	public: bool IsHitboxEnabled(void)const;
	public: void SetHitboxEnable(bool _enable);
	private: bool CreateRenderTarget(Vector2 _imageSize);
	public: bool CreateQuadTextureMap(Vector2 _imageSize, RGBA _color);
	public: bool Create(int32 _typeOfTextureMap, StringANSI _path);
	public: bool Create(int32 _typeOfTextureMap, PixelImage& _image);
	public:	void CreateBoxTextureMap(Vector3 _size, RGBA _color);
	public: bool SetBoxTextureMap(int32 _faceId, StringANSI _path);
	public: bool SetBoxTextureMap(int32 _faceId, Texture2D* _texture);
	public: virtual void Destroy(void);
	public: bool FromScreen(Vector2 _position, Vector2 _size, Vector2 _offset = Vector2());
	private: bool CreateMipMap(void);
	public: void SetFilteringMode(int32 _mode);
    public: int32 GetFilteringMode(void);
	private: void ApplyFilteringMode(void);
	public: bool IsCachingEnabled(void)const;
	public: void SetCachingEnable(bool _enable);
	public: Vector3 GetBoxTextureMapSize(void)const;
	public: Vector2 GetQuadTextureMapSize(void)const;
	public: int32 GetTypeOfTextureMap(void)const;
	public: Vector2 GetImageSize(void)const;
	public: Vector2 GetTextureSize(void)const;
	public: bool IsExist(void)const;
	public: uint32 GetTextureId(void)const;
	public: static int32 _GetTextureMaxSize(void);
	public: bool SetImage(Vector2 _position, PixelImage& _image);
	public: bool SetImage(PixelImage& _image);
	public: bool SetPixel(Vector2 _position, RGBA _color);
	public: bool GetImage(PixelImage& _image);
	public: bool GetImage(Vector2 _position, Vector2 _size, PixelImage& _image);
	public: bool GetPixel(Vector2 _position, RGBA& _color);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static Texture2D* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static Texture2D* _LoadFromFile(File& _file);
};

#endif 