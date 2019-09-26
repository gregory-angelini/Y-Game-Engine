#ifndef ATLAS2D_H
#define ATLAS2D_H
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Output/Graphics/Texture/Texture2D/Texture2D.h>
#include <Engine/Core/Surface/PolygonalSurface/Hitbox/Hitbox.h>
class Atlas2D: public AbstractObject
{
	private: enum
	{
		DESTROY_ATLAS_MESSAGE = 3,
		INIT_ATLAS_MESSAGE = 4
	};
	public: enum: int64 
	{
		ATLAS_2D_CLASS = 16384
	};
	public: class Frame: public AbstractAnimation::Frame
	{
		friend class TexturingSurface;
		friend class TexturedAnimation;
		friend class Atlas2D;
	    private: Vector2 normalizedPosition;
		private: Vector2 normalizedSize;
		private: Vector2 positionInPixels;
		private: Vector2 sizeInPixels;
		private: Texture2D* texture;
		private: MagicPointer<Hitbox> hitbox;
		public: Frame(void);
		public: virtual ~Frame(void);
		public: Vector2 GetNormalizedPosition(void);
		public: Vector2 GetNormalizedSize(void);
	    public: Vector2 GetPositionInPixels(void);
		public: Vector2 GetSizeInPixels(void);
		public: Texture2D& GetTexture(void);
		public: void BindHitbox(Hitbox* _hitbox);
		public: Hitbox* GetHitbox(void)const;
		public: virtual bool SaveToFile(File& _file);
		public: virtual bool LoadFromFile(File& _file);
	};
	friend class TexturedAnimation;
	private: MagicList<Texture2D> textures;
    private: vector<Frame> frames;
	private: bool alignedFrameSize;
    private: int32 filteringMode;
	private: static int32 maxFrames;
	private: static StringANSI atlasesSuffix;
    private: static StringANSI atlasesDir;
	public: Atlas2D(void);
	public: virtual ~Atlas2D(void);
	public: virtual void Destroy(void);
	public: bool LoadImagesFromFolder(StringANSI _path);
	private: void CreateTexturesForAnimation(Vector2 _frameSize, int32 _maxFramesInRow, int32 _maxFramesInColumn, SimpleList<PixelImage>& _frames, SimpleList<StringANSI>& _nameList);
	public: void RenameFrame(int32 _index, StringANSI _name);
    public: int32 GetFilteringMode(void);
	public: void SetFilteringMode(int32 _mode);
	public: Frame* GetFrameInfo(int32 _index);
	public: Frame* GetFrameInfo(StringANSI _name);
	public: int32 GetIndexOfFrame(StringANSI _name);
	public: bool GetImage(int32 _index, PixelImage& _image);
	public: MagicList<Texture2D>& GetTextures(void);
	public: int32 GetAmountOfFrames(void)const;
	public: bool IsExist(void)const;
	public: bool IsFrameSizeAligned(void)const;
	public: void BindHitbox(int32 _index, Hitbox* _hitbox);
	public: Hitbox* GetHitbox(int32 _index)const;
	public: static void _SetMaxFrames(int32 _value);
	public: static int32 _GetMaxFrames(void);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static Atlas2D* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static Atlas2D* _LoadFromFile(File& _file);
};


#endif 