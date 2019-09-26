#ifndef PIXELBUFFER_H
#define PIXELBUFFER_H
#include <Engine/Core/RealTimeWorker/RealTimeWorker.h>
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>
#include <Engine/Math/Matrix/Matrix4x4/Matrix4x4.h>
#ifdef WindowsOS
  #include "GL/glew.h"   
  #include "wglext.h"
#endif
class PixelBuffer
{
	public: enum Mode
	{
		RENDER_TO_TEXTURE
	};
	public: class Define
	{
		public: int32 colorDepth;
		public: Vector4 backgroundColor;
		public: int32 zBufferDepth;
		public: bool zBufferEnable;
		public: float zBufferClearValue;
		public: int32 stencilDepth;
		public: bool frontFaceClockwise;
		public: bool wireframeEnable;

		public: Mode mode;
		public: int32 width, height;
				 
		public: int32 filteringMode;
		public: Define(void);
	};
	friend class TexturingSurface;
	friend class Texture2D;
	friend class GraphicsScene;
	friend class TiledSpace3D;
	private: HGLRC renderContext;
	private: HDC deviceContext;
	private: HPBUFFERARB buffer;
	private: int32 colorDepth;
	private: Vector4 backgroundColor;
	private: int32 zBufferDepth;
	private: bool zBufferEnable;
	private: float zBufferClearValue;
	private: int32 stencilDepth;
	private: bool frontFaceClockwise;
	private: bool wireframeEnable;

	private: int32 width, height;
	private: Mode mode;
	
    private: int32 filteringMode;
	private: GLbitfield clearActions;
	public: PixelBuffer(void);
	public: virtual ~PixelBuffer(void);
	public: bool Create(Define _define);
	public: void Destroy(void);
	public: bool Pick(void);
	public: int32 GetWidth(void)const;
	public: int32 GetHeight(void)const;
};


#endif 