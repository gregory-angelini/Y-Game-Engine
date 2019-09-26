#include "PixelBuffer.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>



PixelBuffer::PixelBuffer(void): renderContext(NIL), deviceContext(NIL), buffer(NIL), width(0), height(0), colorDepth(0), zBufferDepth(0), stencilDepth(0), clearActions(0)
{}

		

PixelBuffer::~PixelBuffer(void)
{
	Destroy();
}


int32 PixelBuffer::GetWidth(void)const
{
	return width;
}

				
int32 PixelBuffer::GetHeight(void)const
{
	return height;
}



bool PixelBuffer::Create(PixelBuffer::Define _define)
{  
	if(Renderer::_GetPicked())
	{
		if(!Renderer::pixelBufferSupported)
		{
			File file(Renderer::_GetPicked()->GetLogFile());
				
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(Renderer::_GetPicked()->GetTitle() + ".Renderer.PixelBuffer - NOT SUPPORTED\r\n");
			}
			return false;
		}

		colorDepth = _define.colorDepth;
		zBufferDepth = _define.zBufferDepth;
		stencilDepth = _define.stencilDepth;
		width = _define.width;
		height = _define.height;
		mode = _define.mode;
		filteringMode = _define.filteringMode;
		uint32 amountOfFormats; 
		GLint pixelFormat;
		int32* intAttribsList;
		float* floatAttribsList;
		const int32 standardIntAttribsList[] = { WGL_DRAW_TO_PBUFFER_ARB, 1,
											     WGL_COLOR_BITS_ARB, colorDepth,
											     WGL_ALPHA_BITS_ARB, colorDepth == 32 ? 8 : 0,
											     WGL_DEPTH_BITS_ARB, zBufferDepth,
											     WGL_STENCIL_BITS_ARB, stencilDepth }; 

		if(mode == RENDER_TO_TEXTURE)
		{
			const int32 extraIntAttribsList[] = { WGL_BIND_TO_TEXTURE_RGBA_ARB,
				                                  true, 
												  0 };

			int32 extraIntAttribs = 1;
			intAttribsList = new int32[(sizeof(standardIntAttribsList) / sizeof(int32)) + extraIntAttribs * 2 + 1];
			memcpy(intAttribsList, standardIntAttribsList, sizeof(standardIntAttribsList));
			memcpy(intAttribsList + (sizeof(standardIntAttribsList) / sizeof(int32)),
				   extraIntAttribsList, extraIntAttribs * 2 * sizeof(int32) + sizeof(int32));
			floatAttribsList = new float[1];
			floatAttribsList[0] = 0;
		} 						  
		if(!Renderer::wglChoosePixelFormatARBExt(wglGetCurrentDC(),
			                                     intAttribsList,
									             floatAttribsList,
									             1,
									             &pixelFormat,
									             &amountOfFormats))
		{
			File file(Renderer::_GetPicked()->GetLogFile());
				
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(Renderer::_GetPicked()->GetTitle() + ".Renderer.wglChoosePixelFormatARB - FAILED\r\n");
			}

			delete []intAttribsList; 
			delete []floatAttribsList;
			return false;
		}
		else 
		{ 
			delete []intAttribsList; 
			delete []floatAttribsList;
		}
		if(mode == RENDER_TO_TEXTURE)
		{
			const int32 flags[] = { WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB,
									WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,
									filteringMode == Texture2D::TRILINEAR_FILTERING ? WGL_MIPMAP_TEXTURE_ARB : 0,
									filteringMode == Texture2D::TRILINEAR_FILTERING ? true : 0,
									0 };

			intAttribsList = new int32[sizeof(flags) / sizeof(int32)];
			memcpy(intAttribsList, flags, sizeof(flags));
		}
		buffer = Renderer::wglCreatePbufferARBExt(wglGetCurrentDC(), pixelFormat, width, height, intAttribsList);
		
		delete[] intAttribsList; 

		if(!buffer)
		{
			File file(Renderer::_GetPicked()->GetLogFile());
				
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(Renderer::_GetPicked()->GetTitle() + ".Renderer.wglCreatePbufferARB - FAILED\r\n");
			}
			return false;
		}
		deviceContext = Renderer::wglGetPbufferDCARBExt(buffer); 
		
		if(!deviceContext)
		{
			File file(Renderer::_GetPicked()->GetLogFile());
				
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(Renderer::_GetPicked()->GetTitle() + ".Renderer.wglGetPbufferDCARB - FAILED\r\n");
			}
			return false;
		}
		renderContext = wglCreateContext(deviceContext);
		
		if(!renderContext)
		{
			File file(Renderer::_GetPicked()->GetLogFile());
				
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(Renderer::_GetPicked()->GetTitle() + ".Renderer.wglCreateContext - FAILED\r\n");
			}
			return false;
		}
		Renderer::wglQueryPbufferARBExt(buffer, WGL_PBUFFER_WIDTH_ARB, &width);
		Renderer::wglQueryPbufferARBExt(buffer, WGL_PBUFFER_HEIGHT_ARB, &height);
		HDC previousDeviceContext = wglGetCurrentDC();
		HGLRC previousRenderContext = wglGetCurrentContext();

		Renderer::Define def = Renderer::_GetPicked()->GetState();

		Pick();
		glViewport(0, 0, width, height);
		if(colorDepth > 0) { clearActions = GL_COLOR_BUFFER_BIT; }
		backgroundColor = _define.backgroundColor;
		glClearColor(backgroundColor.x, 
					 backgroundColor.y,  
					 backgroundColor.z, 
					 backgroundColor.w);
		zBufferEnable = _define.zBufferEnable; 
		zBufferClearValue = _define.zBufferClearValue; 

		if(zBufferEnable)
		{
			glClearDepth(zBufferClearValue);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			clearActions |= GL_DEPTH_BUFFER_BIT;
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			
			clearActions &= ~GL_DEPTH_BUFFER_BIT;
		}
		frontFaceClockwise = _define.frontFaceClockwise;
		
		if(frontFaceClockwise)
		{
			glFrontFace(GL_CW);
		}
		else
		{
			glFrontFace(GL_CCW);
		}
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_BLEND);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		wireframeEnable = _define.wireframeEnable;

		if(wireframeEnable)
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
		glBlendFunc(GL_SRC_ALPHA,
					GL_ONE_MINUS_SRC_ALPHA);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glShadeModel(GL_FLAT);

		if(def.pointSmoothEnable)
		{
			glEnable(GL_POINT_SMOOTH);
		}
		else
		{
			glDisable(GL_POINT_SMOOTH);
		}
		glPointSize(def.pointSizeInPixels);

		if(def.lineSmoothEnable)
		{
			glEnable(GL_LINE_SMOOTH);
		}
		else
		{
			glDisable(GL_LINE_SMOOTH);
		}
		glLineWidth(def.lineWidthInPixels);
	

		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		wglMakeCurrent(previousDeviceContext, previousRenderContext);
		return true;
	}
	return false;
}
		

void PixelBuffer::Destroy(void)
{
	if(Renderer::_GetPicked())
	{
		if(renderContext)
		{
			if(wglDeleteContext(renderContext))
			{
				renderContext = NIL;
			}
			else
			{
				File file(Renderer::_GetPicked()->GetLogFile());
				
				if(file.Open(File::WRITE))
				{
					file.WriteFormatString(Renderer::_GetPicked()->GetTitle() + ".Renderer.wglDeleteContext - FAILED\r\n");
				}
			}
		}

		if(Renderer::wglReleasePbufferDCARBExt(buffer, deviceContext))
		{
			deviceContext = NIL;
		}
		else
		{
			File file(Renderer::_GetPicked()->GetLogFile());
				
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(Renderer::_GetPicked()->GetTitle() + ".Renderer.wglReleasePbufferDCARBExt - FAILED\r\n");
			}
		}
		
		if(Renderer::wglDestroyPbufferARBExt(buffer))
		{
			buffer = NIL;
		}
		else
		{
			File file(Renderer::_GetPicked()->GetLogFile());
				
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(Renderer::_GetPicked()->GetTitle() + ".Renderer.wglDestroyPbufferARBExt - FAILED\r\n");
			}
		}
	}
}


bool PixelBuffer::Pick(void)
{
	if(wglMakeCurrent(deviceContext, renderContext))
	{
		return true;
	}

	return false;
}