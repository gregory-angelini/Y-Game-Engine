#ifndef GRAPHICSCORE_H
#define GRAPHICSCORE_H
#include <Engine/Core/RealTimeWorker/RealTimeWorker.h>
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>
#include <Engine/Math/Matrix/Matrix4x4/Matrix4x4.h>
#ifdef WindowsOS
  #include "GL/glew.h"   
  #include "wglext.h"
#endif
class Renderer: public RealTimeWorker
{
	public: enum ColorFormat
	{
		RGBA_FORMAT = PFD_TYPE_RGBA
	};
	public: enum Primitive
	{
		POINT = GL_POINT_SMOOTH,
		LINE = GL_LINE_SMOOTH,
		POLYGON = GL_POLYGON_SMOOTH
	};
	public: enum TransformType
	{
		MODELVIEW = GL_MODELVIEW,
		PROJECTION = GL_PROJECTION
	};
	public: enum
	{
		ENABLE_Z_BUFFER_MESSAGE = 276
	};
	public: class Define 
	{
	    public: RealTimeWorker::Define realTimeWorkerDef;
	    public: CallController drawCallController;
	    public: bool vSync;
	    public: Vector4 backgroundColor;
	    public: int32 colorDepth;
	    public: ColorFormat colorFormat;
	    public: bool zBufferEnable;
		public: float zBufferClearValue;
	    public: int32 zBufferDepth;
		public: bool frontFaceClockwise;
		public: bool wireframeEnable;

		public: bool pointSmoothEnable;
		public: float pointSizeInPixels;
		public: bool lineSmoothEnable;
		public: float lineWidthInPixels;
		public: Vector2 viewportPosition;
		public: Vector2 viewportSize;
		public: bool viewportAutoEnable;
		public: Define(void);
	};
	private: class OGLRenderContext
	{
		private: HGLRC renderContext;
		private: HDC deviceContext;
		public: OGLRenderContext(void);
		public: ~OGLRenderContext(void);
		public: void Destroy(void);
		public: bool IsExist(void)const;
		public: void Create(HDC _hdc);
		public: HGLRC GetRenderContext(void)const;
	};
	friend class Engine;
	friend class PixelBuffer;
	friend class GraphicsScene;
	friend class Texture2D;
	friend class TexturingSurface;
	private: CallController drawCallController;
    private: bool vSync;
    private: Vector4 backgroundColor;
	private: int32 colorDepth;
	private: ColorFormat colorFormat;   
	private: bool zBufferEnable;
    private: float zBufferClearValue;
	private: int32 zBufferDepth;
	private: bool frontFaceClockwise;
	private: bool wireframeEnable;
	private: GLbitfield clearActions;
	private: Vector2 viewportPosition;
	private: Vector2 viewportSize;
	private: bool viewportAutoEnable;
	private: OGLRenderContext renderContext;
    private: bool optimizer;
    private: bool valid;
	private: static TransformType currentTransformType;
	private: static int32 modelViewStackSize;
    private: static int32 projectionStackSize;
	private: static int32 modelViewMatrixPushed;
	private: static int32 projectionMatrixPushed;
    private: static Renderer* pickedRenderer;
	private: static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalExt;
    private: static PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalExt;
	private: static PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARBExt;
	private: static PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARBExt;
	private: static PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARBExt;
	private: static PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARBExt;
	private: static PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARBExt;
	private: static PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARBExt;
	private: static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARBExt;
    private: static PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARBExt;
	private: static PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARBExt;
    private: static bool pixelBufferSupported;
	private: static SimpleList<StringANSI> wglExtensions;
	private: static SimpleList<StringANSI> glExtensions;
    private: static int32 textureMaxAnisotropy;
	public: Renderer(void);
    public: virtual ~Renderer(void);
	public: static bool _IsPixelBufferSupported(void);
	public: void SetViewport(Vector2 _position, Vector2 _size);
	public: void SetViewportPosition(Vector2 _position);
	public: Vector2 GetViewportPosition(void)const;
	public: Vector2 GetViewportSize(void)const;
	public: void SetViewportSize(Vector2 _size);
	public: bool IsViewportAutoEnabled(void)const;
    public: void SetViewportAutoEnable(bool _enable);
    public: bool Pick(void);
	public: static Renderer* _GetPicked(void);
    public: bool IsPicked(void)const;
	private: bool IsWGLExtensionSupport(StringANSI _name);
	private: void InitWGLExtensions(void);
	private: bool IsGLExtensionSupport(StringANSI _name);
	private: void InitGLExtensions(void);
	private: bool InitOGL(Renderer::Define _define);
    public: bool Create(Renderer::Define _define);
	public: virtual void ClearScene(void);
	public: static void _LoadMatrix(Matrix4x4 _matrix);
	public: static void _LoadIdentityMatrix(void);
	public: static void _MultMatrix(Matrix4x4 _matrix);
	public: static void _SetCurrentStack(TransformType _type);
	public: static TransformType _GetCurrentStack(void);
	public: static bool _PushMatrix(void);
    public: static void _PopMatrix(void);
	public: static int32 _GetStackSize(TransformType _type);
	public: void SetVSyncEnable(bool _enable);
	public: bool IsVSyncEnabled(void);
	public: virtual void Draw(float _delta);
	public: CallController& GetDrawCallController(void);
	private: bool SetPixelFormat(Renderer::Define _define);
	public: int32 GetColorDepth(void)const;
	public: ColorFormat GetColorFormat(void)const;
    public: void SetBackgroundColor4(Vector4 _color);
	public: Vector4 GetBackgroundColor4(void)const;
    public: void SetBackgroundColor(RGBA _color);
	public: RGBA GetBackgroundColor(void)const;
    public: bool GetPixelColor(Vector2 _screenPosition, RGBA& _color);
	public: void SetZBufferEnable(bool _enable);
	public: bool IsZBufferEnabled(void);
	public: void SetZBufferValue(float _value);
	public: float GetZBufferClearValue(void);
	public: int32 GetZBufferDepth(void)const;
    public: Renderer::Define GetState(void)const;
	 public: void SetFrontFaceClockwise(bool _enable);
	 public: bool IsFrontFaceClockwise(void)const;
	public: void SetWireframeEnable(bool _enable);
	public: void SetSmoothEnable(Primitive _primitive, bool _enable);
	public: bool IsSmoothEnabled(Primitive _primitive)const;
	public: static int32 _GetTextureMaxAnisotropy(void);
    public: virtual int32 Frame(void);
	public: virtual LRESULT Proc(HWND _hWindow, UINT _message, WPARAM _wParam, LPARAM _lParam);
	public: virtual void ResizeEventFilter(Vector2 _size);
	public: static void _SetLineWidth(float _size);
	public: static float _GetLineWidth(void);
	public: static float _GetLineSizeGranularity(void);
	public: static void _SetStippleEnable(bool _enable);
    public: static bool _IsStippleEnabled(void);
    public: static void _SetStipple(uint16 _stipple, uint8 _factor);
	public: static float _GetPointSizeGranularity(void);
	public: static void _SetPointSize(float _size);
	public: static float _GetPointSize(void);
};









#endif 