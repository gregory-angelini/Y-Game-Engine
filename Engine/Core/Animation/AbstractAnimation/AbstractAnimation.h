#ifndef ABSTRACTANIMATION_H
#define ABSTRACTANIMATION_H
#include <Engine/Core/Object/LogicObject/LogicObject.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Core/Object/PlayingObject/PlayingObject.h>
#include <Engine/Core/Surface/PolygonalSurface/Hitbox/Hitbox.h>
class AbstractAnimation: public PlayingObject,
	                     public LogicObject
{
	public: enum: int64 
	{
		ABSTRACT_ANIMATION_CLASS = 274877906944
	};
	public: enum
	{
		CHANGE_ANIMATION_FRAME_MESSAGE = 26,
		GET_AMOUNT_OF_FRAMES_MESSAGE = 71
	};
	public: class Frame: public MessageHandler<int32>
	{
		friend class Atlas2D;
		friend class TexturingSurface;
		
		private: StringANSI name;
		private: Vector3 positionOffset;
		private: MagicPointer<Hitbox> hitbox;
		public: Frame(void);
		public: virtual ~Frame(void);
		public: StringANSI GetName(void);
		public: void SetName(StringANSI _name);
	    public: void SetPositionOffset(Vector3 _offset);
	    public: Vector3 GetPositionOffset(void);
		public: void BindHitbox(Hitbox* _hitbox);
		public: Hitbox* GetHitbox(void)const;
		public: virtual bool SaveToFile(File& _file);
		public: virtual bool LoadFromFile(File& _file);
	};
	friend class TexturedAnimation;
    friend class TexturingSurface;
	private: float frameDurationInSec;
    private: int32 amountOfFrames;
	private: int32 currentFrame;
	private: float elapsedTimeInSec;
    private: bool firstFrame;
	private: float elapsedFrameTimeInSec;
	private: int32 playingDirection;
    private: static StringANSI animationsDir;
	public: AbstractAnimation(void);
	public: virtual ~AbstractAnimation(void);
    public: virtual void Stop(void);
	public: virtual void SetPlayingMode(int32 _playingMode);
	public: bool IsLastFrame(void);
	public: float GetPlayingFrameDurationInSec(void)const;
	public: void SetPlayingFrameDurationInSec(float _duration);
	public: virtual float GetPlayingDurationInSec(void);
	public: virtual void SetPlayingDurationInSec(float _duration);
	public: void SetFPS(float _fps);
	public: float GetFPS(void)const;
	public: virtual float GetElapsedTimeInSec(void)const;
	public: int32 GetCurrentFrame(void)const;
	public: void SetCurrentFrame(int32 _index);
	public: void SetCurrentFrame1(float _t);
	public: virtual void SetCurrentFrame(StringANSI _name);
	public: virtual StringANSI GetNameOfCurrentFrame(void)const;
	public: virtual void CurrentFrameIsChanged(void);
	public: int32 GetAmountOfFrames(void)const;
	protected: void SetAmountOfFrames(int32 _frames);
	public: virtual void Update(float _delta = 0.0f);
	public: static StringANSI _GetDir(void);
	protected: void SaveDerivedToFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
};

#endif 