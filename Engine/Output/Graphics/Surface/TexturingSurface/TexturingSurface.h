#ifndef TEXTURINGSURFACE_H
#define TEXTURINGSURFACE_H
#include <Engine/Output/Graphics/Animation/TexturedAnimation/TexturedAnimation.h>
#include <Engine/Output/Graphics/Surface/GraphicsSurface/GraphicsSurface.h>
#include <algorithm>
class TexturingSurface: public GraphicsSurface
{
	public: enum
	{
		TEXTURED_SURFACE_GRAPHICS_TYPE = 8,
		BACK_TEXTURING_GRAPHICS_TYPE = 16
	};
	public: enum
	{
		CHANGE_FRONT_TEXTURE_MESSAGE = 85,
		CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE = 21,
		CHANGE_BACK_TEXTURE_MESSAGE = 88,
		CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE = 89,
		CHECK_LINE_IN_IMAGE_MESSAGE = 117,
		ENABLE_TEXTURE_FLIP_MESSAGE = 216,
		SET_PUBLIC_TEXTURE_MESSAGE = 335
	};
	public: enum 
	{
		CHECK_ONLY_COLORED_PIXEL = 4
	};
	public: enum: int64
	{
		TEXTURING_SURFACE_CLASS = 2048
	};
	friend class GraphicsSurface;
	private: MagicPointer<Texture2D> frontTexture;
	private: MagicPointer<Texture2D> backTexture;
	private: MagicPointer<TexturedAnimation> frontTexturedAnimation;
	private: MagicPointer<TexturedAnimation> backTexturedAnimation;
	private: bool flipXEnable;
    private: bool flipYEnable;
    private: vector<vector<Vector2>> frontTexels;
    private: vector<vector<Vector2>> backTexels;
    private: vector<uint32> backFaceIndices;
	public: TexturingSurface(void);
	public: virtual ~TexturingSurface(void);
	public: virtual void Destroy(void);
    public: TexturingSurface& operator = (const TexturingSurface& _object);
	public: void SetFlipXEnable(bool _flipXEnable);
	public: void SetFlipYEnable(bool _flipYEnable);
	public: bool IsXFlipEnable(void)const;
    public: bool IsYFlipEnable(void)const;
	public: vector<vector<Vector2>>& GetFrontTexels(void);
	public: vector<vector<Vector2>>& GetBackTexels(void);
	public: void ResizeFrontTexels(int32 _rows, int32 _columns);
	public: void ResizeBackTexels(int32 _rows, int32 _columns);
	public: void DestroyFrontTexels(void);
	public: void DestroyBackTexels(void);
	private: void CalculateFrontTexelsForBox(void);
	private: void CalculateFrontTexelsForQuad(void);
	private: void CalculateBackTexelsForQuad(void);
	private: void FrontTextureRenderTargetRoleIsChanged(void);
	private: void BackTextureRenderTargetRoleIsChanged(void);
	private: void CreateBackIndices(void);
	private: void DestroyBackIndices(void);
	public: virtual void BindMesh(Mesh* _mesh);
	private: void MeshIsDestroyed(void);
	private: void MeshIsCreated(void);
	protected: virtual void ModifyGraphicsType(int64 _property, bool _enable);
	private: void DrawFrontBackTexturingShape(void);
	private: void DrawFrontTexturingShape(void);
	private: void DrawBackTexturingShape(void);
	private: void DrawFrontBackTexturingShapeWithBoundingVolume(void);
	private: void DrawCulledTexturingShapeWithBoundingVolume(void);
	private: void DrawTexturingShapeWithBoundingVolume(void);
	private: void DrawCulledTexturingShape(void);
	private: void DrawTexturingShape(void);
	public: virtual void SetIntersectionMode(int32 _mode, bool _enable);
	public: using GraphicsSurface::IsIntersected;
	public: virtual bool IsIntersected(Line _line, Vector2* _pixelPosition = NIL, RGBA* _pixelColor = NIL);
	public: TexturedAnimation* GetFrontTexturedAnimation(void)const;
	public: void BindFrontTexturedAnimation(TexturedAnimation* _texturedAnimation);
	private: void FrontTexturedAnimationHitboxEnableIsChanged(bool _enable);
	private: void FrontTexturedAnimationHitboxIsChanged(StringANSI _name);
	private: void FrontTexturedAnimationIsDestroyed(void);
	private: void FrontTexturedAnimationIsCreated(void);
	public: TexturedAnimation* GetBackTexturedAnimation(void)const;
	public: void BindBackTexturedAnimation(TexturedAnimation* _texturedAnimation);
	private: void BackTexturedAnimationIsDestroyed(void);
	private: void BackTexturedAnimationIsCreated(void);
	private: void CurrentFrameIsChanged(int32 _index);
	public: Texture2D* GetFrontTexture(void)const;
	public: void BindFrontTexture(Texture2D* _texture);
	private: void FrontTextureHitboxEnableIsChanged(bool _enable);
	private: void FrontTextureHitboxIsChanged(StringANSI _name);
	private: void FrontTextureIsDestroyed(void);
	private: void FrontTextureIsCreated(void);
	public: Texture2D* GetBackTexture(void)const;
	public: void BindBackTexture(Texture2D* _texture);
	private: void BackTextureIsDestroyed(void);
	private: void BackTextureIsCreated(void);
	public: using AbstractObject::SaveToFile;
	public: virtual bool SaveToFile(File& _file);
	protected: void SaveDerivedToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	public: using AbstractObject::LoadFromFile;
	protected: virtual bool LoadFromFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
};

#endif 