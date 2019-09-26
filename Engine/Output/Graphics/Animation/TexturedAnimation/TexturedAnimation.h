#ifndef TEXTUREDANIMATION_H
#define TEXTUREDANIMATION_H
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
class TexturedAnimation: public AbstractAnimation
{
	private: enum
	{
		DESTROY_TEXTURED_ANIMATION_MESSAGE = 5,
		INIT_TEXTURED_ANIMATION_MESSAGE = 6
	};
	public: enum
	{
		CHANGE_ATLAS_MESSAGE = 7,
		GET_FRAME_SIZE_MESSAGE = 69,
		ENABLE_POSITION_OFFSET_MESSAGE = 77,
		CREATE_TEXTURED_ANIMATION_MESSAGE = 296
	};
	public: enum: int64 
	{
		TEXTURED_ANIMATION_CLASS = 137438953472
	};
	friend class TexturingSurface;
	private: MagicPointer<Atlas2D> atlas;
	private: Vector2 currentFrameSize;
    private: Atlas2D::Frame* currentFrameInfo;
	private: bool positionOffsetEnable;
    private: bool hitboxEnable;
    private: static StringANSI texturedAnimationsSuffix;
    private: static StringANSI texturedAnimationsDir;
	public: TexturedAnimation(void);
	public: virtual ~TexturedAnimation(void);
	public: void BindAtlas(Atlas2D* _atlas);
	private: void AtlasIsDestroyed(void);
	private: void AtlasIsCreated(void);
	public: Atlas2D* GetAtlas(void);
	public: bool IsExist(void)const;
	public: virtual void Destroy(void);
	public: Hitbox* GetHitbox(void)const;
	public: bool IsHitboxEnabled(void)const;
	public: void SetHitboxEnable( bool _enable);
	private: void HitboxIsChanged(StringANSI _name);
	public: bool IsPositionOffsetEnabled(void)const;
	public: void SetPositionOffsetEnable(bool _enable);
	public: virtual void CurrentFrameIsChanged(void);
	public: Vector2 GetCurrentFrameSize(void)const;
	public: Atlas2D::Frame* GetCurrentFrameInfo(void)const;
	public: using AbstractAnimation::SetCurrentFrame;
	public: virtual void SetCurrentFrame(StringANSI _frameName);
	public: virtual StringANSI GetNameOfCurrentFrame(void)const;
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static TexturedAnimation* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static TexturedAnimation* _LoadFromFile(File& _file);
};

#endif 