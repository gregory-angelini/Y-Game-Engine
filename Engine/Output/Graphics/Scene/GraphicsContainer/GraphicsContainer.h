#ifndef GRAPHICSCONTAINER_H
#define GRAPHICSCONTAINER_H
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>
#include <Engine/Core/Container/vector/vector.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>
class PixelBuffer;
class GraphicsContainer: public virtual GraphicsObject,
	                     public virtual ObjectScene
{
	public: enum: int64
	{
		GRAPHICS_CONTAINER_CLASS = 562949953421312
	};
	public: enum
	{
		CHANGE_CAMERA_MESSAGE = 79,
		CHANGE_RENDER_TARGET_MESSAGE = 288
	};
	friend class Scene;
	friend class GraphicsScene;
	friend class GraphicsContainer;
	friend class TiledSpace3D;
	private: MagicPointer<Texture2D> texture;
	private: MagicPointer<Camera3D> camera;
	private: PixelBuffer* pixelBuffer;
	public: GraphicsContainer(void);
	public: virtual ~GraphicsContainer(void);
	public: virtual void Destroy(void);
	public: Camera3D* GetCamera(bool _checkParentScene = true);
	public: void BindCamera(Camera3D* _camera);
	public: Texture2D* GetTexture(void)const;
	public: void BindTexture(Texture2D* _texture);
	private: void TextureIsDestroyed(void);
	private: void TextureIsCreated(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
};



#endif 