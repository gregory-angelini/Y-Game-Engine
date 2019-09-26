#include "Renderer.h"



Renderer::OGLRenderContext::OGLRenderContext(void): renderContext(NIL), deviceContext(NIL)
{}



Renderer::OGLRenderContext::~OGLRenderContext(void)
{}


void Renderer::OGLRenderContext::Destroy(void)
{
	if(renderContext)
	{
		if(wglMakeCurrent(deviceContext, NIL))
		{
			if(wglDeleteContext(renderContext))
			{
				renderContext = NIL;
			}
		}
	}
}


bool Renderer::OGLRenderContext::IsExist(void)const
{
	return (renderContext != NIL);
}


void Renderer::OGLRenderContext::Create(HDC _hdc)
{
	if(_hdc != NIL && !IsExist())
	{ 
		deviceContext = _hdc;
		renderContext = wglCreateContext(deviceContext); 
	}
}



HGLRC Renderer::OGLRenderContext::GetRenderContext(void)const
{
	return renderContext;
}