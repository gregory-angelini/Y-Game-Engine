#include "Renderer.h"
#include <Engine/Output/File/File.h>
#include <Engine/Core/Time/SystemTime/SystemTime.h> 
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>



  
Renderer::TransformType Renderer::currentTransformType = MODELVIEW;
int32 Renderer::modelViewStackSize = 0;
int32 Renderer::projectionStackSize = 0;
int32 Renderer::modelViewMatrixPushed = 0;
int32 Renderer::projectionMatrixPushed = 0;
Renderer* Renderer::pickedRenderer = NIL;


PFNWGLSWAPINTERVALEXTPROC Renderer::wglSwapIntervalExt = NIL;
PFNWGLGETSWAPINTERVALEXTPROC Renderer::wglGetSwapIntervalExt = NIL;
PFNWGLGETEXTENSIONSSTRINGARBPROC Renderer::wglGetExtensionsStringARBExt = NIL;
PFNWGLCREATEPBUFFERARBPROC Renderer::wglCreatePbufferARBExt = NIL;
PFNWGLGETPBUFFERDCARBPROC Renderer::wglGetPbufferDCARBExt = NIL;
PFNWGLRELEASEPBUFFERDCARBPROC Renderer::wglReleasePbufferDCARBExt = NIL;
PFNWGLDESTROYPBUFFERARBPROC Renderer::wglDestroyPbufferARBExt = NIL;
PFNWGLQUERYPBUFFERARBPROC Renderer::wglQueryPbufferARBExt = NIL;
PFNWGLCHOOSEPIXELFORMATARBPROC Renderer::wglChoosePixelFormatARBExt = NIL;
PFNWGLBINDTEXIMAGEARBPROC Renderer::wglBindTexImageARBExt = NIL;
PFNWGLRELEASETEXIMAGEARBPROC Renderer::wglReleaseTexImageARBExt = NIL;
bool Renderer::pixelBufferSupported = false;
SimpleList<StringANSI> Renderer::wglExtensions;
SimpleList<StringANSI> Renderer::glExtensions;
int32 Renderer::textureMaxAnisotropy = 0;



Renderer::Renderer(void): optimizer(false), valid(false)
{}



Renderer::~Renderer(void)
{
	if(IsPicked()) { Renderer::pickedRenderer = NIL; } 
}



Renderer::Define Renderer::GetState(void)const
{
	Renderer::Define define;

	define.realTimeWorkerDef = RealTimeWorker::GetState();
	define.drawCallController = drawCallController;
	define.vSync = vSync;

	define.backgroundColor = backgroundColor;
	define.colorDepth = colorDepth;
	define.colorFormat = colorFormat;

	define.zBufferEnable = zBufferEnable;
    define.zBufferClearValue = zBufferClearValue;
	define.zBufferDepth = zBufferDepth;

	define.frontFaceClockwise = frontFaceClockwise;
	define.wireframeEnable = wireframeEnable;

	define.pointSmoothEnable = IsSmoothEnabled(Renderer::POINT);
	define.pointSizeInPixels = _GetPointSize();
	define.lineSmoothEnable = IsSmoothEnabled(Renderer::LINE);
	define.lineWidthInPixels = _GetLineWidth();

	define.viewportPosition = viewportPosition;
	define.viewportSize = viewportSize;
	define.viewportAutoEnable = viewportAutoEnable;
	return define;
}
 


bool Renderer::_IsPixelBufferSupported(void)
{
	return pixelBufferSupported;
}
	

 
bool Renderer::IsGLExtensionSupport(StringANSI _name)
{
	SimpleList<StringANSI>::Node* it = glExtensions.GetHead();
	
	while(it)
	{
		if(it->key == _name)
		{
			return true;
		}
		it = it->GetRight();
	}
	return false;
}



void Renderer::InitGLExtensions(void)
{
	glExtensions = SplitString((const char*)glGetString(GL_EXTENSIONS), " ");
	
	if(IsGLExtensionSupport("GL_EXT_texture_filter_anisotropic"))
	{  
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &textureMaxAnisotropy);

		File file(GetLogFile());
		
		if(file.Open(File::WRITE))
		{
			file.WriteFormatString(GetTitle() + ".Renderer.GL_EXT_texture_filter_anisotropic  - SUPPORTED\r\n");
			file.WriteFormatString(GetTitle() + ".Renderer.Texture max anisotropy: %d\r\n", textureMaxAnisotropy);
		}
	}
	else
	{
		File file(GetLogFile());
		
		if(file.Open(File::WRITE))
		{
			file.WriteFormatString(GetTitle() + ".Renderer.GL_EXT_texture_filter_anisotropic  - NOT SUPPORTED\r\n");
		}
	}
}



bool Renderer::IsWGLExtensionSupport(StringANSI _name)
{
	SimpleList<StringANSI>::Node* it = wglExtensions.GetHead();
		
	while(it)
	{
		if(it->key == _name)
		{
			return true;
		}
		it = it->GetRight();
	}
	return false;
}



void Renderer::InitWGLExtensions(void)
{
	wglGetExtensionsStringARBExt = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

	if(wglGetExtensionsStringARBExt)
	{
		wglExtensions = SplitString(wglGetExtensionsStringARBExt(GetDeviceContext()), " ");
	
		if(IsWGLExtensionSupport("WGL_EXT_swap_control"))
		{ 
			wglSwapIntervalExt = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			wglGetSwapIntervalExt = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

			File file(GetLogFile());
			
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(GetTitle() + ".Renderer.WGL_EXT_swap_control  - SUPPORTED\r\n");
			}
		}
		else
		{
			File file(GetLogFile());
			
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(GetTitle() + ".Renderer.WGL_EXT_swap_control  - NOT SUPPORTED\r\n");
			}
		}

		if(IsWGLExtensionSupport("WGL_ARB_pbuffer"))
		{ 
			wglCreatePbufferARBExt = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
			wglGetPbufferDCARBExt = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
			wglReleasePbufferDCARBExt = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
			wglDestroyPbufferARBExt = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
			wglQueryPbufferARBExt = (PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");

			File file(GetLogFile());
			
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(GetTitle() + ".Renderer.WGL_ARB_pbuffer  - SUPPORTED\r\n");
			}
		}
		else
		{
			File file(GetLogFile());
			
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(GetTitle() + ".Renderer.WGL_ARB_pbuffer  - NOT SUPPORTED\r\n");
			}
		}

		if(IsWGLExtensionSupport("WGL_ARB_pixel_format"))
		{ 
			wglChoosePixelFormatARBExt = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	
			File file(GetLogFile());
			
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(GetTitle() + ".Renderer.WGL_ARB_pixel_format  - SUPPORTED\r\n");
			}
		}
		else
		{
			File file(GetLogFile());
			
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(GetTitle() + ".Renderer.WGL_ARB_pixel_format  - NOT SUPPORTED\r\n");
			}
		}

		if(IsWGLExtensionSupport("WGL_ARB_render_texture"))
		{ 
			wglBindTexImageARBExt = (PFNWGLBINDTEXIMAGEARBPROC)wglGetProcAddress("wglBindTexImageARB");
			wglReleaseTexImageARBExt = (PFNWGLRELEASETEXIMAGEARBPROC)wglGetProcAddress("wglReleaseTexImageARB");

			File file(GetLogFile());
			
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(GetTitle() + ".Renderer.WGL_ARB_render_texture  - SUPPORTED\r\n");
			}
		}
		else
		{
			File file(GetLogFile());
			
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(GetTitle() + ".Renderer.WGL_ARB_render_texture  - NOT SUPPORTED\r\n");
			}
		}
		if(!wglGetExtensionsStringARBExt ||
		   !wglCreatePbufferARBExt ||
		   !wglGetPbufferDCARBExt ||
		   !wglChoosePixelFormatARBExt ||
		   !wglQueryPbufferARBExt ||
		   !wglReleasePbufferDCARBExt ||
		   !wglDestroyPbufferARBExt ||
		   !wglBindTexImageARBExt ||
		   !wglReleaseTexImageARBExt)
		{
			pixelBufferSupported = false;
		}
		else
		{
			pixelBufferSupported = true;
		}
	}
	else
	{
		File file(GetLogFile());
		
		if(file.Open(File::WRITE))
		{
			file.WriteFormatString(GetTitle() + ".Renderer.wglGetExtensionsStringARB  - NOT SUPPORTED\r\n");
		}
	}
}



bool Renderer::InitOGL(Renderer::Define _define)
{
	renderContext.Create(GetDeviceContext());
	if(!wglMakeCurrent(GetDeviceContext(), renderContext.GetRenderContext()))
	{
		File file(GetLogFile());
		
		if(file.Open(File::WRITE))
		{
			file.WriteFormatString(GetTitle() + ".Renderer.wglMakeCurrent  - FAILED\r\n");
		}
		return false;
	}
	{
		File file(GetLogFile());
		
		if(file.Open(File::WRITE))
		{
			StringANSI glVersion = (const char*)glGetString(GL_VERSION);
			file.WriteFormatString(GetTitle() + ".Renderer.OpenGL version: " + glVersion + "\r\n");
		}
	}
 
	InitGLExtensions();
	InitWGLExtensions();
	return true;
}




bool Renderer::Create(Renderer::Define _define)
{
	if(RealTimeWorker::Create(_define.realTimeWorkerDef))
	{
		if(SetPixelFormat(_define))
		{
			if(InitOGL(_define))
			{
				valid = true;
				Pick();

				drawCallController = _define.drawCallController;

				if(!drawCallController.IsValid())
				{ 
					drawCallController.SetAction(Caller<>(this, &Renderer::Draw));
				}
				SetVSyncEnable(_define.vSync);
				clearActions = GL_COLOR_BUFFER_BIT;
				SetBackgroundColor(_define.backgroundColor);
				colorDepth = _define.colorDepth;
				colorFormat = _define.colorFormat;
				SetZBufferValue(_define.zBufferClearValue);
				SetZBufferEnable(_define.zBufferEnable);
				zBufferDepth = _define.zBufferDepth;
				SetFrontFaceClockwise(_define.frontFaceClockwise);
				glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &modelViewStackSize);
				glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &projectionStackSize);
                glMatrixMode(MODELVIEW);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glEnable(GL_BLEND);

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnable(GL_TEXTURE_2D);

				SetWireframeEnable(_define.wireframeEnable);
				glBlendFunc(GL_SRC_ALPHA,
							GL_ONE_MINUS_SRC_ALPHA);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glShadeModel(GL_FLAT);
				

				if(_define.pointSmoothEnable) { glEnable(GL_POINT_SMOOTH); }
				else { glDisable(GL_POINT_SMOOTH); }
				
				if(_define.lineSmoothEnable) { glEnable(GL_LINE_SMOOTH); }
				else { glDisable(GL_LINE_SMOOTH); }

				glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

				_SetPointSize(_define.pointSizeInPixels);
				_SetLineWidth(_define.lineWidthInPixels);


				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
				SetViewportAutoEnable(_define.viewportAutoEnable);
				SetViewport(_define.viewportPosition, _define.viewportSize);
				ResizeEventFilter(clientAreaSize);
				return true;
			}
		} 
	}
	{
		File file(GetLogFile());
		
		if(file.Open(File::WRITE))
		{
			file.WriteFormatString(GetTitle() + ".LogicCore.Create - FAILED\r\n");
		}
	}
	return false;
}



int32 Renderer::_GetTextureMaxAnisotropy(void)
{
	return textureMaxAnisotropy;
}



bool Renderer::IsSmoothEnabled(Primitive _primitive)const
{
	if(glIsEnabled(_primitive) == GL_TRUE) { return true; }
	else { return false; }
}


void Renderer::SetSmoothEnable(Renderer::Primitive _primitive, bool _enable)
{
	if(_enable)
	{
		glEnable(_primitive);
	}
	else
	{
		glDisable(_primitive);
	}
}


CallController& Renderer::GetDrawCallController(void)
{
	return drawCallController;
}



void Renderer::SetViewport(Vector2 _position, Vector2 _size)
{
	if(!viewportAutoEnable)
	{ 
		viewportPosition = _position;
		viewportSize = _size;
	    glViewport((GLsizei)viewportPosition.x, (GLsizei)viewportPosition.y, 
			       (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
	}
}



void Renderer::SetViewportPosition(Vector2 _position)
{ 
	if(!viewportAutoEnable)
	{ 
		viewportPosition = _position;
	    glViewport((GLsizei)viewportPosition.x, (GLsizei)viewportPosition.y, 
			       (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
	}
}


Vector2 Renderer::GetViewportPosition(void)const
{
	return viewportPosition;
}



Vector2 Renderer::GetViewportSize(void)const
{
	return viewportSize;
}
 

void Renderer::SetViewportSize(Vector2 _size)
{
	if(!viewportAutoEnable)
	{  
		viewportSize = _size;
	    glViewport((GLsizei)viewportPosition.x, (GLsizei)viewportPosition.y, 
			       (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
	}
}
 
bool Renderer::IsViewportAutoEnabled(void)const
{
	return viewportAutoEnable;
}



void Renderer::SetViewportAutoEnable(bool _enable)
{
	viewportAutoEnable = _enable;
}


void Renderer::SetVSyncEnable(bool _enable)
{
	vSync = _enable;

	if(valid)
	{	
		if(vSync)
		{
			wglSwapIntervalExt(1);
		}
		else
		{
			wglSwapIntervalExt(0); 
		}
	}
}



bool Renderer::IsVSyncEnabled(void)
{
	if(valid)
	{	
		if(wglGetSwapIntervalExt()) { vSync = true; }
		else { vSync = false; } 
	}
	return vSync;
}



LRESULT Renderer::Proc(HWND _hWindow, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch(_message)
	{
		case WM_DESTROY:
		{ 
			renderContext.Destroy();
			
			valid = false;
			break; 
		}
	}
	return RealTimeWorker::Proc(_hWindow, _message, _wParam, _lParam);
} 





void Renderer::Draw(float _delta)
{}



bool Renderer::Pick(void)
{
	if(valid) 
	{ 
		if(Renderer::pickedRenderer != this)
		{
			if(wglMakeCurrent(GetDeviceContext(), renderContext.GetRenderContext()))
			{
				Renderer::pickedRenderer = this;
				return true;
			}
		}
		else { return true; } 
	}

	return false;
}



bool Renderer::IsPicked(void)const
{
	return (Renderer::pickedRenderer == this);
}



Renderer* Renderer::_GetPicked(void)
{
	return Renderer::pickedRenderer;
}


void Renderer::SetWireframeEnable(bool _enable)
{
	if(_enable)
	{
		glPolygonMode(GL_FRONT, GL_LINE); 
		glPolygonMode(GL_BACK, GL_LINE); 
	
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glPolygonMode(GL_FRONT, GL_FILL); 
		glPolygonMode(GL_BACK, GL_FILL); 

		glEnable(GL_TEXTURE_2D);
	}
}

void Renderer::ClearScene(void)
{
	if(valid)
	{
		glClear(clearActions);
	}
}


void Renderer::SetBackgroundColor4(Vector4 _color)
{
	backgroundColor = _color;

	if(valid)
	{
		glClearColor(backgroundColor.x, 
					 backgroundColor.y, 
					 backgroundColor.z, 
					 backgroundColor.w);
	}
}



void Renderer::SetBackgroundColor(RGBA _color)
{
	backgroundColor = _color.GetRGBAf();

	if(valid)
	{
		glClearColor(backgroundColor.x, 
					 backgroundColor.y, 
					 backgroundColor.z, 
					 backgroundColor.w);
	}
}



RGBA Renderer::GetBackgroundColor(void)const
{
	return RGBA(backgroundColor);
}



Vector4 Renderer::GetBackgroundColor4(void)const
{
	return backgroundColor;
}
		


int32 Renderer::Frame(void)
{
	if(Window::Frame())
	{
		if((IsKeyboardFocus() && !IsMinimized()) || unsuspended)
		{
		    if(updateCallController.Call(GetDeltaTimeInSec()))
			{
				optimizer = true;
			}
		    if(valid && drawCallController.Call(GetDeltaTimeInSec()))
			{
				glFlush();
				SwapBuffers(GetDeviceContext());
				optimizer = true;
			}

			if(sleep)
			{
				if(!optimizer)
				{
					SleepEx(1, false);
				}
				optimizer = false;
			}
		}
		else
		{
			SleepEx(1, false);
		}
	}

	return result;
}




void Renderer::SetZBufferEnable(bool _enable)
{ 
	zBufferEnable = _enable;

	if(valid)
	{ 
		if(zBufferEnable)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			clearActions |= GL_DEPTH_BUFFER_BIT;
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			
			clearActions &= ~GL_DEPTH_BUFFER_BIT;
		}
	}
}


bool Renderer::IsZBufferEnabled(void)
{
	if(valid)
	{
		if(glIsEnabled(GL_DEPTH_TEST) == GL_TRUE) { zBufferEnable = true; }
		else { zBufferEnable = false; }
	}
	return zBufferEnable;
}


void Renderer::SetZBufferValue(float _value)
{
	zBufferClearValue = _value;

	if(valid)
	{
		glClearDepth(zBufferClearValue);
	}
}


float Renderer::GetZBufferClearValue(void)
{
	if(valid)
	{
		glGetFloatv(GL_DEPTH_CLEAR_VALUE, &zBufferClearValue);
	}
	return zBufferClearValue;
}



void Renderer::ResizeEventFilter(Vector2 _size)
{
	if(viewportAutoEnable)
	{ 
		viewportPosition.Set(0.0f, 0.0f);
		viewportSize.Set(_size.x, _size.y);
	    glViewport((GLsizei)viewportPosition.x, (GLsizei)viewportPosition.y, 
			       (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
	}
}



bool Renderer::SetPixelFormat(Renderer::Define _define)
{
	if(GetHandle())
	{
		PIXELFORMATDESCRIPTOR pixelFormat;
		memset(&pixelFormat, 0, sizeof(PIXELFORMATDESCRIPTOR));

		pixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pixelFormat.nVersion = 1;
		pixelFormat.dwFlags = PFD_DRAW_TO_WINDOW |
							  PFD_SUPPORT_OPENGL |
							  PFD_DOUBLEBUFFER;
		pixelFormat.iPixelType = _define.colorFormat;
		pixelFormat.cColorBits = _define.colorDepth;
		pixelFormat.cRedBits = 0;
		pixelFormat.cRedShift = 0;
		pixelFormat.cGreenBits = 0;
		pixelFormat.cGreenShift = 0;
		pixelFormat.cBlueBits = 0;
		pixelFormat.cBlueShift = 0;
		pixelFormat.cAlphaBits = 0;
		pixelFormat.cAlphaShift = 0;
		pixelFormat.cAccumBits = 0;
		pixelFormat.cAccumRedBits = 0;
		pixelFormat.cAccumGreenBits = 0;
		pixelFormat.cAccumBlueBits = 0;
		pixelFormat.cAccumAlphaBits = 0;
		pixelFormat.cDepthBits = _define.zBufferDepth;
		pixelFormat.cStencilBits = 0;
		pixelFormat.cAuxBuffers = 0;
		pixelFormat.iLayerType = PFD_MAIN_PLANE;
		pixelFormat.bReserved = 0;
		pixelFormat.dwLayerMask = 0;
		pixelFormat.dwVisibleMask = 0;
		pixelFormat.dwDamageMask = 0;
        GLuint pixelFormatIndex = ::ChoosePixelFormat(GetDeviceContext(), &pixelFormat);
		if(pixelFormatIndex != 0)
		{
			PIXELFORMATDESCRIPTOR bestMatchPixelFormat;
			::DescribePixelFormat(GetDeviceContext(), pixelFormatIndex, sizeof(pixelFormat), &bestMatchPixelFormat);
			if(bestMatchPixelFormat.cColorBits != pixelFormat.cColorBits)
			{
				File file(GetLogFile());
		
				if(file.Open(File::WRITE))
				{
					file.WriteFormatString(GetTitle() + ".ColorBits(" + NumberToString(pixelFormat.cColorBits) +") - FAILED\r\n");
				}
				return false;
			}
			if(bestMatchPixelFormat.cDepthBits != pixelFormat.cDepthBits)
			{
				File file(GetLogFile());
		
				if(file.Open(File::WRITE))
				{
					file.WriteFormatString(GetTitle() + ".DepthBits(" + NumberToString(pixelFormat.cDepthBits) +") - FAILED\r\n");
				}
				return false;
			}
			if(!::SetPixelFormat(GetDeviceContext(),
							     pixelFormatIndex,
							     &pixelFormat))
			{
				File file(GetLogFile());
		
				if(file.Open(File::WRITE))
				{
					file.WriteFormatString(GetTitle() + ".Renderer.SetPixelFormat - FAILED\r\n");
				}
				return false;
			}
		}
		else
		{
			File file(GetLogFile());
		
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(GetTitle() + ".Renderer.ChoosePixelFormat - FAILED\r\n");
			}
			return false;
		}

		return true;
	}

	File file(GetLogFile());
		
	if(file.Open(File::WRITE))
	{
		file.WriteFormatString(GetTitle() + ".Renderer.SetPixelFormat: window is not exist!\r\n");
	}
	return false;
}


int32 Renderer::GetColorDepth(void)const
{
	return colorDepth;
}


Renderer::ColorFormat Renderer::GetColorFormat(void)const
{
	return colorFormat;
}


int32 Renderer::GetZBufferDepth(void)const
{
	return zBufferDepth;
}


bool Renderer::GetPixelColor(Vector2 _screenPosition, RGBA& _color)
{
	_screenPosition.y = viewportSize.y - _screenPosition.y - 1;
	if(_screenPosition.y >= viewportPosition.x && _screenPosition.y < (viewportPosition.y + viewportSize.y))
	{
		if(_screenPosition.x >= viewportPosition.x && _screenPosition.x < (viewportPosition.x + viewportSize.x))
		{
			glReadPixels(_screenPosition.x, _screenPosition.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &_color.red);
			return true;
		}
	}
	return false;
}


void Renderer::SetFrontFaceClockwise(bool _enable)
{
	frontFaceClockwise = _enable;

	if(valid)
	{
		if(frontFaceClockwise)
		{
			glFrontFace(GL_CW);
		}
		else
		{
			glFrontFace(GL_CCW);
		}
	}
}


bool Renderer::IsFrontFaceClockwise(void)const
{
	return frontFaceClockwise;
}



void Renderer::_LoadMatrix(Matrix4x4 _matrix)
{
	glLoadMatrixf(_matrix.GetData());
}


void Renderer::_LoadIdentityMatrix(void)
{
	glLoadIdentity();
}


void Renderer::_MultMatrix(Matrix4x4 _matrix)
{
	glMultMatrixf(_matrix.GetData());
}



Renderer::TransformType Renderer::_GetCurrentStack(void)
{
	return currentTransformType;
}


void Renderer::_SetCurrentStack(Renderer::TransformType _type)
{
	currentTransformType = _type;
	glMatrixMode(currentTransformType);
}


int32 Renderer::_GetStackSize(Renderer::TransformType _type)
{
	if(_type == MODELVIEW) 
	{
		return modelViewStackSize;
	}
	return projectionStackSize;
}



bool Renderer::_PushMatrix(void)
{
	if(currentTransformType == MODELVIEW)
	{
		if(modelViewStackSize)
		{
			glPushMatrix();
			modelViewStackSize--;
			modelViewMatrixPushed++;
			return true;
		}
	}
	else if(currentTransformType == PROJECTION)
	{
		if(projectionStackSize)
		{
			glPushMatrix();
			projectionStackSize--;
			projectionMatrixPushed++;
			return true;
		}
	}
	return false;
}

			
void Renderer::_PopMatrix(void)
{
	if(currentTransformType == MODELVIEW)
	{
		if(modelViewMatrixPushed)
		{
			glPopMatrix();
			modelViewStackSize++;
			modelViewMatrixPushed--;
		}
	}
	else if(currentTransformType == PROJECTION)
	{
		if(projectionMatrixPushed)
		{
			glPopMatrix();
			projectionStackSize++;
			projectionMatrixPushed--;
		}
	}
}



void Renderer::_SetLineWidth(float _size)
{
	glLineWidth(_size);
}


float Renderer::_GetLineWidth(void)
{
	float lineWidth;
	glGetFloatv(GL_LINE_WIDTH, &lineWidth);
	return lineWidth;
}


float Renderer::_GetLineSizeGranularity(void)
{
	float granularity;
	glGetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY, &granularity);
	return granularity;
}


void Renderer::_SetStippleEnable(bool _enable)
{
	if(_enable)
	{
		glEnable(GL_LINE_STIPPLE);
	}
	else
	{
		glDisable(GL_LINE_STIPPLE);
	}
}


bool Renderer::_IsStippleEnabled(void)
{
	GLboolean stipple = false;
	glGetBooleanv(GL_LINE_STIPPLE, &stipple);
	return (stipple == GL_TRUE);
}


void Renderer::_SetStipple(uint16 _stipple, uint8 _factor)
{
	glLineStipple((GLint)_factor, _stipple);
}



float Renderer::_GetPointSizeGranularity(void)
{
	float granularity = 0.0f;
	glGetFloatv(GL_SMOOTH_POINT_SIZE_GRANULARITY, &granularity);
	return granularity;
}

void Renderer::_SetPointSize(float _size)
{
	glPointSize(_size);
}


float Renderer::_GetPointSize(void)
{
	float pointSize;
	glGetFloatv(GL_POINT_SIZE, &pointSize);
	return pointSize;
}